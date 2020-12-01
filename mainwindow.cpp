#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFontMetrics>
#include <QKeyEvent>
#include <QScrollBar>
#include <QSerialPortInfo>
#include <QTimer>

#include "PIg/src/flash.h"
//#include "PIg/src/flashParams.h"
#include "PIg/src/menu/base.hpp"


//
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow) {
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
    ui->serialBsp->addDefaultPort("tnt0");

    connect(ui->serialBsp, &TSerial::read,
            [=](int byte) {bspPushByteFrom(byte, false);});
    connect(this, &MainWindow::writeByteToBsp, ui->serialBsp, &TSerial::write);
    connect(ui->serialBsp, &TSerial::sendFinished, [=]() {bspTxEnd();} );

    ui->serialPc->setLabelText("Port PC:");
    ui->serialPc->setup(19200, QSerialPort::NoParity, QSerialPort::TwoStop);
    ui->serialPc->addDefaultPort("COM8");
    ui->serialPc->addDefaultPort("tnt2");

    connect(ui->serialPc, &TSerial::read,
            [=](int byte) {pcPushByteFrom(byte, false);});
    connect(this, &MainWindow::writeByteToPc, ui->serialPc, &TSerial::write);
    connect(ui->serialPc, &TSerial::sendFinished, [=]() {pcTxEnd();} );

    codec = QTextCodec::codecForName("CP1251");
    connect(ui->textEdit, &QTextEdit::selectionChanged,
            this, &MainWindow::clearSelection);

    initView();

    installEventFilter(this);
    ui->textEdit->installEventFilter(this);

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(20);
    ui->textEdit->setFont(font);

    QFontMetrics mfont(font);
    QSize size = mfont.size(Qt::TextSingleLine, "12345678901234567890");

    // FIXME Разобраться почему константы именно такие
    ui->textEdit->setFixedSize(size.width() + 12, size.height()*7 + 10);
    ui->kbd->setFixedSize(ui->textEdit->width(), ui->textEdit->width());

    // Удаляет движение содержимого при прокрутке колесика мышки над testEdit
    ui->textEdit->verticalScrollBar()->blockSignals(true);

    setupTestButtons();

    QTimer *timerMenu = new QTimer(this);
    connect(timerMenu, &QTimer::timeout, this, &MainWindow::cycleMenu);
    timerMenu->start(100);
}

//
MainWindow::~MainWindow() {
    delete ui;
}

//
void MainWindow::initView() {
    QTreeWidgetItem *top = nullptr;
    QTreeWidgetItem *item = nullptr;

    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->treeWidget->header()->resizeSection(0, 150);
    ui->treeWidget->setFixedWidth(300);
    ui->treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->treeWidget->setFocusPolicy(Qt::NoFocus);

    top = new QTreeWidgetItem();
    ui->treeWidget->addTopLevelItem(top);
    top->setText(0, codec->toUnicode("Пользователь"));
    addViewItem(top, "Роль ПИ", &view.userPi);
    addViewItem(top, "Роль ПК", &view.userPc);
    addViewItem(top, "Пароль инженера", &view.engPwd);
    addViewItem(top, "Пароль админисратора", &view.admPwd);
    addViewItem(top, "Счетчик инж.", &view.engCounter);
    addViewItem(top, "Счетчик адм.", &view.admCounter);

    top = new QTreeWidgetItem();
    ui->treeWidget->addTopLevelItem(top);
    top->setText(0, codec->toUnicode("Сетевые настройки"));
    addViewItem(top, "Интерфейс", &view.interface);

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
    addViewItem(top, "Совместимость", &view.typeComp);
    addViewItem(top, "Совместимость K400", &view.typeCompK400);
    addViewItem(top, "Тип оптики", &view.typeOpto);

    ui->treeWidget->expandAll();
    ui->treeWidget->resizeColumnToContents(0);
    ui->treeWidget->resizeColumnToContents(1);

    pred = view.engCounter.palette();
    pred.setColor(QPalette::Text, Qt::red);
    pblue = view.engCounter.palette();
    pblue.setColor(QPalette::Text, Qt::blue);
}

//
void MainWindow::setupTestButtons() {
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
void MainWindow::hdlrView() {
    bool locked = false;
    quint8 value  = 0;
    quint16 time = 0;
    user_t user;

    value = menu.sParam.security.getUser(USER_SOURCE_pi);
    time = menu.sParam.security.getUserTime(USER_SOURCE_pi);
    view.userPi.setText(QString("%1 / %2c").
                        arg(codec->toUnicode(fcUser[value])).
                        arg(time));

    value = menu.sParam.security.getUser(USER_SOURCE_pc);
    time = menu.sParam.security.getUserTime(USER_SOURCE_pc);
    view.userPc.setText(QString("%1 / %2c").
                        arg(codec->toUnicode(fcUser[value])).
                        arg(time));


    user = USER_engineer;
    view.engPwd.setText(pwdToString(menu.sParam.security.pwd.getPwd(user)));
    value = menu.sParam.security.pwd.getCounter(user);
    time = menu.sParam.security.pwd.getLockTime(user);
    locked = menu.sParam.security.pwd.isLocked(user);
    view.engCounter.setText(QString("%1 / %2c").
                            arg(value, 2, 16, QLatin1Char('0')).
                            arg(time));
    view.engCounter.setPalette(locked > 0 ? pred : pblue);

    user = USER_admin;
    view.admPwd.setText(pwdToString(menu.sParam.security.pwd.getPwd(user)));
    value = menu.sParam.security.pwd.getCounter(user);
    time = menu.sParam.security.pwd.getLockTime(user);
    locked = menu.sParam.security.pwd.isLocked(user);
    view.admCounter.setText(QString("%1 / %2c").
                            arg(value, 2, 16, QLatin1Char('0')).
                            arg(time));
    view.admCounter.setPalette(locked ? pred : pblue);

    value = menu.sParam.Uart.Interface.get();
    view.interface.setText(codec->toUnicode(fcInterface[value]));

    value = menu.sParam.def.status.getRegime();
    view.regimeDef.setText(codec->toUnicode(fcRegime[value]));
    value = menu.sParam.prm.status.getRegime();
    view.regimePrm.setText(codec->toUnicode(fcRegime[value]));
    value = menu.sParam.prd.status.getRegime();
    view.regimePrd.setText(codec->toUnicode(fcRegime[value]));

    view.typeDevice.setText(getDeviceName(menu.sParam.typeDevice));
    view.def.setText(menu.sParam.def.status.isEnable() ? "ok" : "no");
    viewNumComPrm();
    viewNumComPrd();
    value = menu.sParam.glb.getNumDevices();
    view.numDevices.setText(codec->toUnicode(fcNumDevices[value - 1]));
    view.typeCommLine.setText(getTypeLine(menu.sParam.glb.getTypeLine()));
    value = menu.sParam.glb.getCompatibility();
    view.typeComp.setText(codec->toUnicode(fcCompatibility[value]));
    viewTypeCompK400();
    view.typeOpto.setText(getTypeOpto(menu.sParam.glb.getTypeOpto()));
}

//
void MainWindow::addViewItem(QTreeWidgetItem *top, std::string name,
                             QLineEdit *lineedit) {
    QTreeWidgetItem *item = new QTreeWidgetItem();

    top->addChild(item);
    item->setText(0, codec->toUnicode(name.c_str()));
    ui->treeWidget->setItemWidget(item, 1, lineedit);
    lineedit->setReadOnly(true);
    lineedit->setFocusPolicy(Qt::NoFocus);
    connect(lineedit, &QLineEdit::selectionChanged,
            lineedit, &QLineEdit::deselect);
}

//
bool MainWindow::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::KeyRelease) {
        // У элементов созданных на вкладке Bsp имен нет.
        if (!focusWidget()->objectName().isEmpty()) {
            ui->kbd->keyPressed(static_cast<QKeyEvent*>(event)->key());
        }
    }

    return QMainWindow::eventFilter(object, event);
}

//
void MainWindow::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    mainInit();
    ui->treeWidget->setFixedWidth(ui->treeWidget->columnWidth(0)*2.2);
    setFixedSize(sizeHint());
}

//
void MainWindow::cycleMenu() {
    uint8_t len = 0;

    static uint8_t cnt1s = 0;
    static uint8_t cntsendtobsp = 0;
    static uint8_t cntsendtopc = 0;
    QVector<uint8_t> pkg;

    vLCDled();

    bspRead();
    pcRead();

    mainCycle();

    pkg.clear();
    len = pcWrite();
    for(uint8_t i = 0; i < len; i++) {
        pkg.append(uBufUartPc[i]);
        emit writeByteToPc(uBufUartPc[i]);
    }

    if (len > 0) {
        qDebug() << "Pkg to PC: " << Qt::showbase << Qt::hex << pkg;
        cntsendtopc++;
    }

    pkg.clear();
    len = bspWrite();
    for(uint8_t i = 0; i < len; i++) {
        pkg.append(uBufUartBsp[i]);
        emit writeByteToBsp(uBufUartBsp[i]);
    }

    if (len > 0) {
        if ((pkg.at(2) == GB_COM_SET_CONTROL) ||
            (pkg.at(2) == GB_COM_PRD_RES_IND) ||
            (pkg.at(2) == GB_COM_PRM_RES_IND) ||
            (pkg.at(2) == GB_COM_PRM_ENTER) ||
            (pkg.at(2) == GB_COM_DEF_SET_TYPE_AC)) {
            qDebug() << "Pkg to BSP: " << Qt::showbase << Qt::hex << pkg;
        }
        cntsendtobsp++;
    }

    cnt1s++;
    if (cnt1s >= 10) {
//        qDebug() << "Send " << cntsendtobsp << " packet to BSP per second.";
//        qDebug() << "Send " << cntsendtopc << " packet to PC per second.";
        cntsendtopc = 0;
        cntsendtobsp = 0;
        cnt1s = 0;
    }

    hdlrView();

    ui->serialBsp->setLedLink(menu.sParam.connectionBsp);
    ui->serialPc->setLedLink(menu.sParam.connectionPc);
}

//
void MainWindow::clearSelection() {
    QTextCursor c = ui->textEdit->textCursor();
    c.clearSelection();
    ui->textEdit->setTextCursor(c);
}

//
void MainWindow::setBacklight(bool enable) {
    QColor color = enable ? Qt::green : Qt::gray;

    if (color.isValid()) {
        QString qss = QString("background-color: %1").arg(color.name());
        ui->textEdit->setStyleSheet(qss);
    }
}

//
QString MainWindow::pwdToString(uint8_t *pwd) {
    QString password;
    for(uint8_t i = 0; i < PWD_LEN; i++) {
        uint8_t ch = pwd[i];
        if ((ch < '0') || (ch > '9')) {
            if (!password.isEmpty() && (password.back() != ' ')) {
                password += " ";
            }
            password += QString("%1 ").arg(ch, 2, 16, QLatin1Char('0'));
        } else {
            password += QChar(pwd[i]);
        }
    }
    return password;
}

//
void MainWindow::test1() {
    qDebug() << "Set user ENGINEER for PC";
    menu.sParam.security.setUser(USER_engineer, USER_SOURCE_pc);
}

//
void MainWindow::test2() {
    qDebug() << "Set user ADMIN for PI";
    menu.sParam.security.setUser(USER_admin, USER_SOURCE_pi);
}

//
void MainWindow::test3() {
    qDebug() << "Clear Security logs";
    menu.sParam.txComBuf.addFastCom(GB_COM_JRN_IS_CLR, GB_SEND_NO_DATA);
}

//
void MainWindow::test4() {
    qDebug() << "Set Disable Regime";
    menu.sParam.txComBuf.addFastCom(GB_COM_SET_REG_DISABLED, GB_SEND_NO_DATA);
}

//
QString
MainWindow::getDeviceName(eGB_TYPE_DEVICE type) const {
    QString typeName = codec->toUnicode("Ошибка");

    switch(type) {
        case AVANT_NO:
            typeName = codec->toUnicode("Нет");
            break;
        case AVANT_R400:
            typeName = codec->toUnicode("Р400");
            break;
        case AVANT_RZSK:
            typeName = codec->toUnicode("РЗСК");
            break;
        case AVANT_K400:
            typeName = codec->toUnicode("К400");
            break;
        case AVANT_R400M:
            typeName = codec->toUnicode("Р400М");
            break;
        case AVANT_OPTO:
            typeName = codec->toUnicode("Оптика");
            break;
        case AVANT_MAX:
            typeName = codec->toUnicode("MAX");
            break;
    }

    return typeName;
}

//
QString
MainWindow::getTypeLine(eGB_TYPE_LINE type) const {
    QString typeName = codec->toUnicode("Ошибка");

    switch(type) {
        case GB_TYPE_LINE_UM:
            typeName = codec->toUnicode("ВЧ");
            break;
        case GB_TYPE_LINE_OPTO:
            typeName = codec->toUnicode("Оптика");
            break;
        case GB_TYPE_LINE_E1:
            typeName = codec->toUnicode("Е1");
            break;
        case GB_TYPE_LINE_MAX:
            typeName = codec->toUnicode("MAX");
            break;
    }

    return typeName;
}

//
void
MainWindow::viewNumComPrd() {
    uint8_t number = menu.sParam.prd.getNumCom();
    bool enable = menu.sParam.prd.status.isEnable();

    view.numComPrd.setText(QString::number(number));
    view.numComPrd.setPalette((enable == (number > 0)) ? pblue : pred);
}

//
void
MainWindow::viewNumComPrm() {
    uint8_t number = menu.sParam.prm.getNumCom();
    bool enable = menu.sParam.prm.status.isEnable();

    view.numComPrm.setText(QString::number(number));
    view.numComPrm.setPalette((enable == (number > 0)) ? pblue : pred);
}

//
void
MainWindow::viewTypeCompK400() {
    eGB_COMP_K400 comp = menu.sParam.glb.getCompK400();

    if (comp < GB_COMP_K400_MAX) {
        view.typeCompK400.setText(codec->toUnicode(fcCompK400[comp]));
        view.typeCompK400.setPalette(pblue);
    } else {
        view.typeCompK400.setText(QString::number(comp));
        view.typeCompK400.setPalette(pred);
    }


}

//
QString
MainWindow::getTypeOpto(eGB_TYPE_OPTO type) const {
    QString typeName = codec->toUnicode("Ошибка");

    switch(type) {
        case TYPE_OPTO_STANDART:
            typeName = codec->toUnicode("Стандартная");
            break;
        case TYPE_OPTO_RING_UNI:
            typeName = codec->toUnicode("Кольцо однонапр.");
            break;
        case TYPE_OPTO_RING_BI:
            typeName = codec->toUnicode("Кольцо двунапр.");
            break;
        case TYPE_OPTO_MAX:
            typeName = codec->toUnicode("MAX");
            break;
    }

    return typeName;
}

