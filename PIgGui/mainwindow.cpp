#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFontMetrics>
#include <QKeyEvent>
#include <QScrollBar>
#include <QSerialPortInfo>
#include <QTimer>

#include "PIg/src/flash.h"
#include "PIg/src/menu/base.h"

//
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("BSP-PI");

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));

    ui->verticalLayout->setAlignment(ui->kbd, Qt::AlignHCenter);
    ui->verticalLayout->setAlignment(ui->textEdit, Qt::AlignHCenter);

    ui->serialBsp->setFixedWidth(ui->serialBsp->sizeHint().width());
    ui->serialPc->setFixedWidth(ui->serialBsp->width());

    ui->serialBsp->setLabelText("Port BSP:");
    ui->serialBsp->setup(4800, QSerialPort::NoParity, QSerialPort::TwoStop);
    ui->serialBsp->addDefaultPort("COM20");
    ui->serialBsp->addDefaultPort("COM15");
    ui->serialBsp->addDefaultPort("COM3");
    ui->serialBsp->addDefaultPort("tnt0");

    connect(ui->serialBsp, &TSerial::read, this, &MainWindow::SlotByteBspToPi);
    connect(this, &MainWindow::writeByteToBsp, ui->serialBsp, &TSerial::write);
    connect(ui->serialBsp, &TSerial::sendFinished, [=]() { bspTxEnd(); });
    connect(ui->serialBsp, &TSerial::openPort, [&]() { ui->mBspConnect->setEnabled(false); });
    connect(ui->serialBsp, &TSerial::closePort, [&]() { ui->mBspConnect->setEnabled(true); });

    ui->serialPc->setLabelText("Port PC:");
    ui->serialPc->setup(19200, QSerialPort::NoParity, QSerialPort::TwoStop);
    ui->serialPc->addDefaultPort("COM8");
    ui->serialPc->addDefaultPort("tnt2");

    connect(ui->serialPc, &TSerial::read, [=](int byte) { pcPushByteFrom(byte, false); });
    connect(this, &MainWindow::writeByteToPc, ui->serialPc, &TSerial::write);
    connect(ui->serialPc, &TSerial::sendFinished, [=]() { pcTxEnd(); });

    codec = QTextCodec::codecForName("CP1251");
    connect(ui->textEdit, &QTextEdit::selectionChanged, this, &MainWindow::clearSelection);

    connect(ui->mBspConnect, &QPushButton::clicked, this, &MainWindow::SlotBspConnection);

    //    палитры
    QLineEdit lineedit;
    pdefault = lineedit.palette();
    pred     = pdefault;
    pred.setColor(QPalette::Text, Qt::red);
    pblue = pdefault;
    pblue.setColor(QPalette::Text, Qt::blue);

    initBsp();
    initEeprom();
    initView();
    initKeyboard();
    InitProtocolViewer();

    installEventFilter(this);
    ui->textEdit->installEventFilter(this);

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(20);
    ui->textEdit->setFont(font);

    QFontMetrics mfont(font);
    QSize        size = mfont.size(Qt::TextSingleLine, "12345678901234567890");

    // FIXME Разобраться почему константы именно такие
    ui->textEdit->setFixedSize(size.width() + 12, size.height() * 7 + 10);
    ui->kbd->setFixedSize(ui->textEdit->width(), ui->textEdit->width());

    // Удаляет движение содержимого при прокрутке колесика мышки над testEdit
    ui->textEdit->verticalScrollBar()->blockSignals(true);

    setupTestButtons();

    QTimer *timerMenu = new QTimer(this);
    connect(timerMenu, &QTimer::timeout, this, &MainWindow::cycleMenu);
    timerMenu->start(100);

    setFocusPolicy(Qt::StrongFocus);
}

//
MainWindow::~MainWindow()
{
    delete ui;
}

//
void MainWindow::initBsp()
{
    ui->mBspTree->crtTreeDevice();
    ui->mBspTree->crtTreeState();
    ui->mBspTree->crtTreePrm();
    ui->mBspTree->crtTreePrd();
    ui->mBspTree->crtTreeGlb();
    ui->mBspTree->crtTreeInterface();
    ui->mBspTree->crtTest();
    ui->mBspTree->crtTreeDef();
    ui->mBspTree->crtTreeMeasure();
    ui->mBspTree->initParam();

    ui->mBspTree->expandAll();
    ui->mBspTree->resizeColumnToContents(0);
    //    ui->mBspTree->resizeColumnToContents(1);
    ui->mBspTree->header()->setSectionResizeMode(0, QHeaderView::Fixed);

    //    ui->mBspTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mBspTree->collapseAll();
}

void MainWindow::initEeprom()
{
    menu.sParam.password.init(0);
}

//
void MainWindow::initView()
{
    QTreeWidgetItem *top = nullptr;

    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->treeWidget->header()->resizeSection(0, 150);
    ui->treeWidget->setFixedWidth(300);
    ui->treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->treeWidget->setFocusPolicy(Qt::NoFocus);

    top = new QTreeWidgetItem();
    ui->treeWidget->addTopLevelItem(top);
    top->setText(0, codec->toUnicode("Пользователь"));
    addViewItem(top, "Пароль пользователя", &view.userPwd);
    addViewItem(top, "Пароль админисратора", &view.admPwd);

    top = new QTreeWidgetItem();
    ui->treeWidget->addTopLevelItem(top);
    top->setText(0, codec->toUnicode("Локальная сеть"));
    addViewItem(top, getNameOfParam(GB_PARAM_INTF_PROTOCOL), &view.lnProtocol);
    addViewItem(top, getNameOfParam(GB_PARAM_NET_ADDRESS), &view.lnAddress);
    addViewItem(top, getNameOfParam(GB_PARAM_INTF_BAUDRATE), &view.lnBaudrate);
    addViewItem(top, getNameOfParam(GB_PARAM_INTF_DATA_BITS), &view.lnDataBits);
    addViewItem(top, getNameOfParam(GB_PARAM_INTF_PARITY), &view.lnParity);
    addViewItem(top, getNameOfParam(GB_PARAM_INTF_STOP_BITS), &view.lnStopBits);

    top = new QTreeWidgetItem();
    ui->treeWidget->addTopLevelItem(top);
    top->setText(0, codec->toUnicode("Текущее состояние"));
    addViewItem(top, "Режим ЗАЩ", &view.regimeDef);
    addViewItem(top, "Режим ПРМ", &view.regimePrm);
    addViewItem(top, "Режим ПРД", &view.regimePrd);

    top = new QTreeWidgetItem();
    ui->treeWidget->addTopLevelItem(top);
    top->setText(0, codec->toUnicode("Устройство"));
    addViewItem(top, "Устройство", &view.typeDevice);
    addViewItem(top, "Защита", &view.def);
    addViewItem(top, "Кол-во команд ПРМ", &view.numComPrm);
    addViewItem(top, "Кол-во команд ПРД", &view.numComPrd);
    addViewItem(top, "Кол-во аппаратов", &view.numDevices);
    addViewItem(top, "Тип линии", &view.typeCommLine);
    addViewItem(top, "Совместимость Р400", &view.typeCompatibility);
    addViewItem(top, "Тип оптики", &view.typeOpto);

    top = new QTreeWidgetItem();
    ui->treeWidget->addTopLevelItem(top);
    top->setText(0, codec->toUnicode("Параметры общие"));
    addViewItem(top, "Номер аппарата", &view.deviceNumber);

    // Обновление расскладки клавиатуры при смене аппарата или совместимости
    // задержка в 200мс нужна для обновления расскладки в ПИ
    connect(&view.typeDevice,
            &QLineEdit::textChanged,
            [=]() { QTimer::singleShot(200, this, &MainWindow::initKeyboard); });
    connect(&view.typeCompatibility,
            &QLineEdit::textChanged,
            [=]() { QTimer::singleShot(200, this, &MainWindow::initKeyboard); });

    ui->treeWidget->expandAll();
    ui->treeWidget->resizeColumnToContents(0);
    ui->treeWidget->resizeColumnToContents(1);
}

void MainWindow::initKeyboard()
{
    for (uint8_t i = 1; i <= 2 * NUM_KEY_IN_LAYOUT; i++)
    {
        ui->kbd->setLayoutButton(i, vKEYgetButtonLayout(i));
    }
}

/**
 * *****************************************************************************
 *
 * @brief Настройка просмотрщика протокола.
 *
 * *****************************************************************************
 */
void MainWindow::InitProtocolViewer()
{
    connect(ui->pbView, &QPushButton::clicked, &mProtocolViewer, &QProtocolViewer::show);
    connect(this, &MainWindow::writeByteToBsp, this, &MainWindow::SlotBytePiToBsp);

    mProtocolViewer.SetPattern("51 72 8A 9A FF");
    mProtocolViewer.resize(width(), height());

    auto flags = mProtocolViewer.windowFlags();
    mProtocolViewer.setWindowFlags(flags | Qt::WindowStaysOnTopHint);
}

//
void MainWindow::setupTestButtons()
{
    ui->pbTest1->setText("Engeneer PC");
    connect(ui->pbTest1, &QPushButton::clicked, this, &MainWindow::test1);

    ui->pbTest2->setText("Admin PI");
    connect(ui->pbTest2, &QPushButton::clicked, this, &MainWindow::test2);

    ui->pbTest3->setText("Clear logs");
    connect(ui->pbTest3, &QPushButton::clicked, this, &MainWindow::test3);

    ui->pbTest4->setText("Disable");
    connect(ui->pbTest4, &QPushButton::clicked, this, &MainWindow::test4);
}

//
void MainWindow::hdlrView()
{
    std::string text;
    quint8      value8  = 0;
    quint16     value16 = 0;

    value16 = menu.sParam.password.get();
    view.userPwd.setText(QString::number(value16));

    value8 = menu.sParam.Uart.Protocol.get();
    view.lnProtocol.setText(codec->toUnicode(fcProtocol[value8]));
    value8 = menu.sParam.Uart.NetAddress.get();
    view.lnAddress.setText(QString::number(value8));
    value8 = menu.sParam.Uart.BaudRate.get();
    view.lnBaudrate.setText(codec->toUnicode(fcBaudRate[value8]));
    value8 = menu.sParam.Uart.DataBits.get();
    view.lnDataBits.setText(codec->toUnicode(fcDataBits[value8]));
    value8 = menu.sParam.Uart.Parity.get();
    view.lnParity.setText(codec->toUnicode(fcParity[value8]));
    value8 = menu.sParam.Uart.StopBits.get();
    view.lnStopBits.setText(codec->toUnicode(fcStopBits[value8]));

    value8 = menu.sParam.def.status.getRegime();
    view.regimeDef.setText(codec->toUnicode(fcRegime[value8]));
    value8 = menu.sParam.prm.status.getRegime();
    view.regimePrm.setText(codec->toUnicode(fcRegime[value8]));
    value8 = menu.sParam.prd.status.getRegime();
    view.regimePrd.setText(codec->toUnicode(fcRegime[value8]));

    view.typeDevice.setText(getDeviceName(menu.sParam.typeDevice));
    view.def.setText(menu.sParam.def.status.isEnable() ? "ok" : "---");
    viewNumComPrm();
    viewNumComPrd();

    text = getTextValue(GB_PARAM_NUM_OF_DEVICES, menu.sParam.glb.getNumDevices());
    view.numDevices.setText(codec->toUnicode(text.c_str()));

    view.typeCommLine.setText(getTypeLine(menu.sParam.glb.getTypeLine()));
    viewTypeComp();
    view.typeOpto.setText(getTypeOpto(menu.sParam.glb.getTypeOpto()));

    view.deviceNumber.setText(QString::number(menu.sParam.glb.getDeviceNum()));
}

//
void MainWindow::addViewItem(QTreeWidgetItem *top, std::string name, QLineEdit *lineedit)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    top->addChild(item);
    item->setText(0, codec->toUnicode(name.c_str()));
    ui->treeWidget->setItemWidget(item, 1, lineedit);
    lineedit->setReadOnly(true);
    lineedit->setFocusPolicy(Qt::NoFocus);
    connect(lineedit, &QLineEdit::selectionChanged, lineedit, &QLineEdit::deselect);
}

//
bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyRelease)
    {
        // У элементов созданных на вкладке Bsp имен нет.
        if (!focusWidget()->objectName().isEmpty())
        {
            ui->kbd->keyPressed(static_cast<QKeyEvent *>(event)->key());
        }
    }

    return QMainWindow::eventFilter(object, event);
}

//
void MainWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    mainInit();
    ui->treeWidget->setFixedWidth(ui->treeWidget->columnWidth(0) * 2.2);
    setFixedSize(sizeHint());
}

//
void MainWindow::cycleMenu()
{
    uint8_t len = 0;

    static uint8_t   cnt1s        = 0;
    static uint8_t   cntsendtobsp = 0;
    static uint8_t   cntsendtopc  = 0;
    QVector<uint8_t> pkg;

    vLCDled();

    bspRead();
    pcRead();

    mainCycle();

    pkg.clear();
    len = pcWrite();
    for (uint8_t i = 0; i < len; i++)
    {
        pkg.append(uBufUartPc[i]);
        emit writeByteToPc(uBufUartPc[i]);
    }

    if (len > 0)
    {
        //        qDebug() << "Pkg to PC: " << Qt::showbase << Qt::hex << pkg;
        cntsendtopc++;
    }

    pkg.clear();
    len = bspWrite();
    for (uint8_t i = 0; i < len; i++)
    {
        pkg.append(uBufUartBsp[i]);
        emit writeByteToBsp(uBufUartBsp[i]);
    }

    if (len > 0)
    {
        //        if ((pkg.at(2) == GB_COM_SET_CONTROL) ||
        //            (pkg.at(2) == GB_COM_PRD_RES_IND) ||
        //            (pkg.at(2) == GB_COM_PRM_RES_IND) ||
        //            (pkg.at(2) == GB_COM_PRM_ENTER) ||
        //            (pkg.at(2) == GB_COM_DEF_SET_TYPE_AC)) {
        //            qDebug() << "Pkg to BSP: " << Qt::showbase << Qt::hex << pkg;
        //        }
        cntsendtobsp++;
    }

    cnt1s++;
    if (cnt1s >= 10)
    {
        //                qDebug() << "Send " << cntsendtobsp << " packet to BSP per second.";
        //                qDebug() << "Send " << cntsendtopc << " packet to PC per second.";
        cntsendtopc  = 0;
        cntsendtobsp = 0;
        cnt1s        = 0;
    }

    hdlrView();

    ui->serialBsp->setLedLink(menu.sParam.connectionBsp);
    ui->serialPc->setLedLink(menu.sParam.connectionPc);
}

//
void MainWindow::clearSelection()
{
    QTextCursor c = ui->textEdit->textCursor();
    c.clearSelection();
    ui->textEdit->setTextCursor(c);
}

//
void MainWindow::setBacklight(bool enable)
{
    QColor color = enable ? Qt::green : Qt::gray;

    if (color.isValid())
    {
        QString qss = QString("background-color: %1").arg(color.name());
        ui->textEdit->setStyleSheet(qss);
    }
}

void MainWindow::SlotBspConnection()
{
    if (ui->serialBsp->isEnabled())
    {
        connect(this, &MainWindow::writeByteToBsp, ui->mBspTree, &Bsp::SlotReadByte);

        connect(ui->mBspTree, &Bsp::SignalWriteByte, this, &MainWindow::SlotByteBspToPi);
        connect(ui->mBspTree, &Bsp::SignalSendFinished, [=]() { bspTxEnd(); });

        ui->serialBsp->setEnabled(false);
        ui->mBspConnect->setText("Disconnect");
    }
    else
    {
        ui->mBspTree->disconnect();
        disconnect(this, &MainWindow::writeByteToBsp, ui->mBspTree, &Bsp::SlotReadByte);
        disconnect(this,
                   &MainWindow::writeByteToBsp,
                   &mProtocolViewer,
                   &QProtocolViewer::AddTxByte);

        ui->serialBsp->setEnabled(true);
        ui->mBspConnect->setText("Connect");
    }
}


//
void MainWindow::SlotByteBspToPi(uint8_t byte)
{
    mProtocolViewer.AddRxByte(byte);
    bspPushByteFrom(byte, false);
}


//
void MainWindow::SlotBytePiToBsp(uint8_t byte)
{
    mProtocolViewer.AddTxByte(byte);
}

//
void MainWindow::test1()
{
    qDebug() << "test1 button pressed";
}

//
void MainWindow::test2()
{
    qDebug() << "test2 button pressed";
}

//
void MainWindow::test3()
{
    qDebug() << "test3 button pressed";
}

//
void MainWindow::test4()
{
    qDebug() << "Set Disable Regime";
    menu.sParam.txComBuf.addFastCom(GB_COM_SET_REG_DISABLED);
}

//
QString MainWindow::getDeviceName(eGB_TYPE_DEVICE type) const
{
    QString typeName = codec->toUnicode("Ошибка");

    if (type <= AVANT_MAX)
    {
        switch (type)
        {
        case AVANT_NO: typeName = codec->toUnicode("Нет"); break;
        case AVANT_R400: typeName = codec->toUnicode("Р400"); break;
        case AVANT_RZSK: typeName = codec->toUnicode("РЗСК"); break;
        case AVANT_K400: typeName = codec->toUnicode("К400"); break;
        case AVANT_R400M: typeName = codec->toUnicode("Р400М"); break;
        case AVANT_OPTO: typeName = codec->toUnicode("Оптика"); break;
        case AVANT_MAX: typeName = codec->toUnicode("MAX"); break;
        }
    }
    else
    {
    }

    return typeName;
}

//
QString MainWindow::getTypeLine(eGB_TYPE_LINE type) const
{
    QString typeName = codec->toUnicode("Ошибка");

    switch (type)
    {
    case GB_TYPE_LINE_UM: typeName = codec->toUnicode("ВЧ"); break;
    case GB_TYPE_LINE_OPTO: typeName = codec->toUnicode("Оптика"); break;
    case GB_TYPE_LINE_E1: typeName = codec->toUnicode("Е1"); break;
    case GB_TYPE_LINE_MAX: typeName = codec->toUnicode("MAX"); break;
    }

    return typeName;
}

//
void MainWindow::viewNumComPrd()
{
    uint8_t number = menu.sParam.prd.getNumCom();
    bool    enable = menu.sParam.prd.status.isEnable();

    view.numComPrd.setText(QString::number(number));
    view.numComPrd.setPalette((enable == (number > 0)) ? pblue : pred);
}

//
void MainWindow::viewNumComPrm()
{
    uint8_t number = menu.sParam.prm.getNumCom();
    bool    enable = menu.sParam.prm.status.isEnable();

    view.numComPrm.setText(QString::number(number));
    view.numComPrm.setPalette((enable == (number > 0)) ? pblue : pred);
}

//
void MainWindow::viewTypeComp()
{
    eGB_PARAM pn    = GB_PARAM_MAX;
    uint8_t   min   = 0;
    uint8_t   max   = 0;
    uint8_t   value = 0;

    switch (menu.sParam.typeDevice)
    {
    case AVANT_K400:
        {
            pn    = GB_PARAM_COMP_K400;
            value = menu.sParam.glb.getCompK400();
        }
        break;

    case AVANT_RZSK:
        {
            pn    = GB_PARAM_COMP_RZSK;
            value = menu.sParam.glb.getCompRZSK();
        }
        break;

    case AVANT_R400:  // DOWN
    case AVANT_R400M:
        {
            pn    = GB_PARAM_COMP_P400;
            value = menu.sParam.glb.getCompR400m();
        }
        break;

    case AVANT_OPTO: break;
    case AVANT_NO: break;
    case AVANT_MAX: break;
    }

    std::string text = "---";
    if (pn < GB_PARAM_MAX)
    {
        if (value < getAbsMax(pn))
        {
            text = getTextValue(pn, value);
            view.typeCompatibility.setPalette(pblue);
        }
        else
        {
            text = std::to_string(value);
            view.typeCompatibility.setPalette(pred);
        }
    }
    else
    {
        view.typeCompatibility.setPalette(pdefault);
    }
    view.typeCompatibility.setText(codec->toUnicode(text.c_str()));
}

//
QString MainWindow::getTypeOpto(eGB_TYPE_OPTO type) const
{
    QString typeName = codec->toUnicode("Ошибка");

    switch (type)
    {
    case TYPE_OPTO_STANDART: typeName = codec->toUnicode("Стандартная"); break;
    case TYPE_OPTO_RING_UNI: typeName = codec->toUnicode("Кольцо однонапр."); break;
    case TYPE_OPTO_RING_BI: typeName = codec->toUnicode("Кольцо двунапр."); break;
    case TYPE_OPTO_MAX: typeName = codec->toUnicode("MAX"); break;
    }

    return typeName;
}
