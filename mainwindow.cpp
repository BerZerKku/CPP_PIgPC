#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFontMetrics>
#include <QKeyEvent>
#include <QTimer>
#include <QScrollBar>
#include <QTextCodec>
#include <QSerialPortInfo>

//
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));

    connect(ui->textEdit, &QTextEdit::selectionChanged,
            this, &MainWindow::clearSelection);

    initParam();

    protBSPs = new clProtocolBspS(bspBuf, sizeof(bspBuf), &menu.sParam);
    protBSPs->setEnable(PRTS_STATUS_NO);

    installEventFilter(this);
    ui->textEdit->installEventFilter(this);

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(20);
    ui->textEdit->setFont(font);

    QFontMetrics mfont(font);
    QSize size = mfont.size(Qt::TextSingleLine, "12345678901234567890");

    // FIXME –азобратьс€ почему константы именно такие
    ui->textEdit->setFixedSize(size.width() + 12, size.height()*7 + 10);
    ui->kbd->setFixedSize(ui->textEdit->width(), ui->textEdit->width());
    setFixedSize(sizeHint());

    // ”дал€ет движение содержимого при прокрутке колесика мышки над testEdit
    ui->textEdit->verticalScrollBar()->blockSignals(true);

    refreshPortList();

    connect(ui->pbRefresh, &QPushButton::clicked,
            this, &MainWindow::refreshPortList);
    connect(ui->pbPort, &QPushButton::clicked,
            this, &MainWindow::connectSerialPort);

    QTimer *timerMenu = new QTimer(this);
    connect(timerMenu, &QTimer::timeout, this, &MainWindow::cycleMenu);
    timerMenu->start(100);
}

//
MainWindow::~MainWindow() {
    delete ui;
}

//
void MainWindow::initParam() {

}

//
bool MainWindow::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::KeyRelease) {
        // ” элементов созданных на вкладке Bsp имен нет.
        if (!focusWidget()->objectName().isEmpty()) {
            ui->kbd->keyPressed(static_cast<QKeyEvent*>(event)->key());
        }
    }

    return QMainWindow::eventFilter(object, event);
}

//
void MainWindow::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
}

//
void MainWindow::uartRead() {
    protBSPs->checkStat();
    if (protBSPs->getCurrentStatus() == PRTS_STATUS_READ_OK) {
        menu.setConnectionBsp(true);
        if (protBSPs->checkReadData()) {
            protBSPs->getData(false);
        }
    } else {
//        qWarning() << "Read packet error!";
    }
}

//
void MainWindow::uartWrite() {
    protBSPs->checkStat();
    if (protBSPs->getCurrentStatus() == PRTS_STATUS_NO) {
        eGB_COM com = menu.getTxCommand();
        if (com != GB_COM_NO) {
            uint16_t num = protBSPs->sendData(com);
            for(uint16_t i = 0; i < num; i++) {
                emit  writeByte(bspBuf[i]);
            }
            protBSPs->setCurrentStatus(PRTS_STATUS_NO);
        } else {
            qWarning() << "No send package!";
        }
    }
}

//
void MainWindow::cycleMenu() {
    static uint8_t menucnt = 0;
    static TUser::user_t user = TUser::MAX;

    uartRead();

    menucnt = (menucnt >= 2) ? (menu.proc(), 0) : menucnt + 1;
    vLCDled();

    uartWrite();

    if (user != menu.sParam.security.User.get()) {
        user = menu.sParam.security.User.get();
        emit userChanged(user);
    }
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
void MainWindow::setUser(int value) {
    menu.sParam.security.User.set(static_cast<TUser::user_t> (value));
}

//
void MainWindow::refreshPortList() {
    QString portname = ui->cmbPort->currentText();
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();

    ui->cmbPort->clear();
    for (const QSerialPortInfo &info :infos) {
        QString portname = info.portName();
        ui->cmbPort->addItem(portname);
    }

    if (portname.isEmpty()) {
        portname = "tnt0";
    }
    ui->cmbPort->setCurrentText(portname);
    ui->pbPort->setEnabled(ui->cmbPort->count() != 0);
}

//
void MainWindow::connectSerialPort() {
    qDebug() << "";

    if (port.isNull() && thread.isNull()) {

        port = new SerialPort(ui->cmbPort->currentText());
        thread = new QThread(this);

        connect(thread, &QThread::started, port, &SerialPort::start);
        connect(thread, &QThread::finished, port, &SerialPort::stop);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);

        connect(port, &SerialPort::finished, this, &MainWindow::closeSerialPort);

        connect(port, &SerialPort::finished, thread, &QThread::quit);
        connect(port, &SerialPort::finished, &timer, &QTimer::stop);
        connect(port, &SerialPort::finished, port, &SerialPort::deleteLater);

        connect(this, &MainWindow::writeByte, port, &SerialPort::writeByte);
        connect(port, &SerialPort::readByte, this, &MainWindow::readByte);

        disconnect(ui->pbPort, &QPushButton::clicked,
                this, &MainWindow::connectSerialPort);
        connect(ui->pbPort, &QPushButton::clicked, port, &SerialPort::stop);

        ui->cmbPort->setEnabled(false);
        ui->pbPort->setText("Close");
        ui->pbRefresh->setEnabled(false);

        port->moveToThread(thread);
        thread->start();

        QObject::connect(&timer, &QTimer::timeout, port, &SerialPort::proc);
        timer.start(5);
    } else {
        qDebug() << "";
    }
}

void MainWindow::closeSerialPort() {
    connect(ui->pbPort, &QPushButton::clicked,
            this, &MainWindow::connectSerialPort);

    ui->cmbPort->setEnabled(true);
    ui->pbPort->setText("Open");
    ui->pbPort->setEnabled(true);
    ui->pbRefresh->setEnabled(true);
}

void MainWindow::readByte(int value) {
    qDebug() << "12";
    protBSPs->checkByte(static_cast<uint8_t> (value));
}

