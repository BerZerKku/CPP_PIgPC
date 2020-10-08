#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFontMetrics>
#include <QKeyEvent>
#include <QScrollBar>
#include <QSerialPortInfo>
#include <QTimer>

#include "PIg/src/flashParams.h"
#include "PIg/src/menu/base.hpp"


//
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setWindowTitle("BSP-PI");

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));


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
    setFixedSize(sizeHint());

    // Удаляет движение содержимого при прокрутке колесика мышки над testEdit
    ui->textEdit->verticalScrollBar()->blockSignals(true);

    refreshPortListBsp();
    connect(ui->pbRefreshBsp, &QPushButton::clicked,
            this, &MainWindow::refreshPortListBsp);
    connect(ui->pbPortBsp, &QPushButton::clicked,
            this, &MainWindow::connectSerialPortBSP);

    refreshPortListPc();
    connect(ui->pbRefreshPc, &QPushButton::clicked,
            this, &MainWindow::refreshPortListPc);
    connect(ui->pbPortPc, &QPushButton::clicked,
            this, &MainWindow::connectSerialPortPc);

    ui->lPortBsp->setFixedWidth(ui->lPortBsp->sizeHint().width());
    ui->lPortPc->setFixedWidth(ui->lPortBsp->width());

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
    addViewItem(top, "Счетчик инж.", &view.engCounter);
    addViewItem(top, "Счетчик адм.", &view.admCounter);

    ui->treeWidget->expandAll();
}

//
void MainWindow::setupTestButtons() {
    ui->pbTest1->setText("Engeneer PC");
    connect(ui->pbTest1, &QPushButton::clicked, this, &MainWindow::test1);

    connect(ui->pbTest2, &QPushButton::clicked, this, &MainWindow::test2);

    connect(ui->pbTest3, &QPushButton::clicked, this, &MainWindow::test3);

    connect(ui->pbTest4, &QPushButton::clicked, this, &MainWindow::test4);
}

//
void MainWindow::hdlrView() {
    bool locked = false;
    quint8 value  = 0;
    quint16 time = 0;
    TUser::user_t user;

    QPalette pred = view.engCounter.palette();
    pred.setColor(QPalette::Text, Qt::red);
    QPalette pblue = view.engCounter.palette();
    pblue.setColor(QPalette::Text, Qt::blue);

    value = menu.sParam.security.UserPi.get();
    time = menu.sParam.security.UserPi.getTimer();
    view.userPi.setText(QString("%1 / %2c").
                            arg(codec->toUnicode(fcUser[value])).
                            arg((time * MENU_TIME_CYLCE) / 1000));

    value = menu.sParam.security.UserPc.get();
    time = menu.sParam.security.UserPc.getTimer();
    view.userPc.setText(QString("%1 / %2c").
                        arg(codec->toUnicode(fcUser[value])).
                        arg((time * MENU_TIME_CYLCE) / 1000));

    value = menu.sParam.security.UserPi.get();
    time = menu.sParam.security.UserPi.getTimer();
    view.userPi.setText(QString("%1 / %2c").
                            arg(codec->toUnicode(fcUser[value])).
                            arg((time * MENU_TIME_CYLCE) / 1000));


    user = TUser::ENGINEER;
    value = menu.sParam.security.pwd.getCounter(user);
    time = menu.sParam.security.pwd.getLockTime(user);
    locked = menu.sParam.security.pwd.isLocked(user);
    view.engCounter.setText(QString("%1 / %2c").
                            arg(value, 2, 16, QLatin1Char('0')).
                            arg(time));
    view.engCounter.setPalette(locked > 0 ? pred : pblue);

    user = TUser::ADMIN;
    value = menu.sParam.security.pwd.getCounter(user);
    time = menu.sParam.security.pwd.getLockTime(user);
    locked = menu.sParam.security.pwd.isLocked(user);
    view.admCounter.setText(QString("%1 / %2c").
                            arg(value, 2, 16, QLatin1Char('0')).
                            arg(time));
    view.admCounter.setPalette(locked ? pred : pblue);
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
}

//
void MainWindow::cycleMenu() {
    uint8_t len = 0;

    static uint8_t cnt1s = 0;
    static uint8_t cntsendtobsp = 0;
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
//        qDebug() << "Pkg to PC: " << showbase << hex << pkg;
    }

    pkg.clear();
    len = bspWrite();
    for(uint8_t i = 0; i < len; i++) {
        pkg.append(uBufUartBsp[i]);
        emit writeByteToBsp(uBufUartBsp[i]);
    }

    if (len > 0) {
        cntsendtobsp++;
//        qDebug() << "Pkg to BSP: " << showbase << hex << pkg;
    }

    cnt1s++;
    if (cnt1s >= 10) {
//        qDebug() << "Send " << cntsendtobsp << " packet to BSP per second.";
        cntsendtobsp = 0;
        cnt1s = 0;
    }

    hdlrView();
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
void MainWindow::refreshPortListBsp() {
    QString portname;
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();

    if (ui->cmbPortBsp->isEnabled()) {
        portname = ui->cmbPortBsp->currentText();
        ui->cmbPortBsp->clear();

        for (const QSerialPortInfo &info :infos) {
            QString portname = info.portName();
            ui->cmbPortBsp->addItem(portname);
        }

        if (portname.isEmpty()) {
            ui->cmbPortBsp->setCurrentText("COM20");
            ui->cmbPortBsp->setCurrentText("tnt0");
        } else {
            ui->cmbPortBsp->setCurrentText(portname);
        }
        ui->pbPortBsp->setEnabled(ui->cmbPortBsp->count() != 0);
    }
}

//
void MainWindow::connectSerialPortBSP() {
    if (portBSP.isNull() && threadBSP.isNull()) {
        portBSP = new SerialPort(ui->cmbPortBsp->currentText(), 4800);
        threadBSP = new QThread(this);

        connect(threadBSP, &QThread::started, portBSP, &SerialPort::start);
        connect(threadBSP, &QThread::finished, portBSP, &SerialPort::stop);
        connect(threadBSP, &QThread::finished, threadBSP, &QThread::deleteLater);

        connect(portBSP, &SerialPort::finished, this, &MainWindow::closeSerialPortBSP);
        connect(portBSP, &SerialPort::finished, threadBSP, &QThread::quit);
        connect(portBSP, &SerialPort::finished, portBSP, &SerialPort::deleteLater);

        connect(this, &MainWindow::writeByteToBsp, portBSP, &SerialPort::writeByte);
        connect(portBSP, &SerialPort::readByte, this, &MainWindow::readByteFromBsp);
        connect(portBSP, &SerialPort::sendFinished, this, &MainWindow::resetStatusBSP);

        disconnect(ui->pbPortBsp, &QPushButton::clicked,
                this, &MainWindow::connectSerialPortBSP);
        connect(ui->pbPortBsp, &QPushButton::clicked, portBSP, &SerialPort::stop);

        ui->cmbPortBsp->setEnabled(false);
        ui->pbPortBsp->setText("Close");
        ui->pbRefreshBsp->setEnabled(false);

        portBSP->moveToThread(threadBSP);
        threadBSP->start();
    } else {
        qDebug() << "portBSP.isNull() && threadBSP.isNull()";
    }
}

//
void MainWindow::closeSerialPortBSP() {
    connect(ui->pbPortBsp, &QPushButton::clicked,
            this, &MainWindow::connectSerialPortBSP);

    refreshPortListBsp();
    ui->cmbPortBsp->setEnabled(true);
    ui->pbPortBsp->setText("Open");
    ui->pbPortBsp->setEnabled(true);
    ui->pbRefreshBsp->setEnabled(true);
}

//
void MainWindow::readByteFromBsp(int value) {
    uint8_t byte = static_cast<uint8_t> (value);
    bspPushByteFrom(byte, false);
}

//
void MainWindow::resetStatusBSP() {
    bspTxEnd();
}

//
void MainWindow::refreshPortListPc() {
    QString portname;
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();

    if (ui->cmbPortPc->isEnabled()) {
        portname = ui->cmbPortPc->currentText();
        ui->cmbPortPc->clear();

        for (const QSerialPortInfo &info :infos) {
            QString portname = info.portName();
            ui->cmbPortPc->addItem(portname);
        }

        if (portname.isEmpty()) {
            ui->cmbPortPc->setCurrentText("COM8");
            ui->cmbPortPc->setCurrentText("tnt2");
        } else {
            ui->cmbPortPc->setCurrentText(portname);
        }
        ui->pbPortPc->setEnabled(ui->cmbPortPc->count() != 0);
    }
}

//
void MainWindow::connectSerialPortPc() {
    if (portPC.isNull() && threadPC.isNull()) {
        portPC = new SerialPort(ui->cmbPortPc->currentText(), 19200);
        threadPC = new QThread(this);

        connect(threadPC, &QThread::started, portPC, &SerialPort::start);
        connect(threadPC, &QThread::finished, portPC, &SerialPort::stop);
        connect(threadPC, &QThread::finished, threadPC, &QThread::deleteLater);

        connect(portPC, &SerialPort::finished, this, &MainWindow::closeSerialPortPc);
        connect(portPC, &SerialPort::finished, threadPC, &QThread::quit);
        connect(portPC, &SerialPort::finished, portPC, &SerialPort::deleteLater);

        connect(this, &MainWindow::writeByteToPc, portPC, &SerialPort::writeByte);
        connect(portPC, &SerialPort::readByte, this, &MainWindow::readByteFromPc);
        connect(portPC, &SerialPort::sendFinished, this, &MainWindow::resetStatusPc);

        disconnect(ui->pbPortPc, &QPushButton::clicked,
                this, &MainWindow::connectSerialPortPc);
        connect(ui->pbPortPc, &QPushButton::clicked, portPC, &SerialPort::stop);

        ui->cmbPortPc->setEnabled(false);
        ui->pbPortPc->setText("Close");
        ui->pbRefreshPc->setEnabled(false);

        portPC->moveToThread(threadPC);
        threadPC->start();
    } else {
        qDebug() << "";
    }
}

//
void MainWindow::closeSerialPortPc() {
    connect(ui->pbPortPc, &QPushButton::clicked,
            this, &MainWindow::connectSerialPortPc);

    refreshPortListPc();
    ui->cmbPortPc->setEnabled(true);
    ui->pbPortPc->setText("Open");
    ui->pbPortPc->setEnabled(true);
    ui->pbRefreshPc->setEnabled(true);
}

//
void MainWindow::readByteFromPc(int value) {
    uint8_t byte = static_cast<uint8_t> (value);
    pcPushByteFrom(byte, false);
}

//
void MainWindow::resetStatusPc() {
    pcTxEnd();
}

//
void MainWindow::test1() {
    qDebug() << "Set user ENGINEER for pc";
    menu.sParam.security.UserPc.set(TUser::ENGINEER);
}

//
void MainWindow::test2() {

}

//
void MainWindow::test3() {

}

//
void MainWindow::test4() {

}

