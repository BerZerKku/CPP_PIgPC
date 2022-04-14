#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFontMetrics>
#include <QKeyEvent>
#include <QScrollBar>
#include <QSerialPortInfo>
#include <QTimer>

#include "PIg/src/menu/base.h"
#include "bsp/bsp.h"

//
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("BSP-PI");

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));

    ui->verticalLayout->setAlignment(ui->kbd, Qt::AlignHCenter);
    ui->verticalLayout->setAlignment(ui->textEdit, Qt::AlignHCenter);

    ui->serialBsp->setFixedWidth(ui->serialBsp->sizeHint().width());

    ui->serialBsp->setLabelText("Port BSP:");
    ui->serialBsp->setup(4800, QSerialPort::NoParity, QSerialPort::TwoStop);
    ui->serialBsp->addDefaultPort("COM20");
    ui->serialBsp->addDefaultPort("COM15");
    ui->serialBsp->addDefaultPort("COM3");
    ui->serialBsp->addDefaultPort("tnt0");

    connect(this, &MainWindow::writeByteToBsp, ui->serialBsp, &TSerial::write);
    connect(ui->serialBsp, &TSerial::sendFinished, [=]() { bspTxEnd(); });
    connect(ui->serialBsp, &TSerial::openPort, [&]() { ui->mBspConnect->setEnabled(false); });
    connect(ui->serialBsp, &TSerial::closePort, [&]() { ui->mBspConnect->setEnabled(true); });


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

    m_bsp.Init();
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

    mProtocolViewer.SetPattern("01 11");
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

    //    value16 = menu.sParam.password.get();
    //    view.userPwd.setText(QString::number(value16));
    //    value8 = menu.sParam.Uart.Protocol.get();
    //    view.lnProtocol.setText(codec->toUnicode(fcProtocol[value8]));
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
    QVector<uint8_t> pkg;

    vLCDled();

    bspRead();

    mainCycle();

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
        cntsendtobsp = 0;
        cnt1s        = 0;
    }

    hdlrView();

    ui->serialBsp->setLedLink(menu.IsConnection());
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
        connect(this, &MainWindow::writeByteToBsp, &m_bsp, &Bsp::SlotReadByte);

        connect(ui->serialBsp, &TSerial::read, this, &MainWindow::SlotByteBspToPi);
        connect(&m_bsp, &Bsp::SignalWriteByte, this, &MainWindow::SlotByteBspToPi);
        connect(&m_bsp, &Bsp::SignalSendFinished, [=]() { bspTxEnd(); });

        ui->mBspConnect->setText("Disconnect");
    }
    else
    {
        m_bsp.disconnect();

        disconnect(ui->serialBsp, &TSerial::read, this, &MainWindow::SlotByteBspToPi);
        disconnect(this, &MainWindow::writeByteToBsp, &m_bsp, &Bsp::SlotReadByte);


        ui->mBspConnect->setText("Connect");
    }

    m_bsp.SlotStart(ui->serialBsp->isEnabled());
    ui->serialBsp->setEnabled(!ui->serialBsp->isEnabled());
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
}
