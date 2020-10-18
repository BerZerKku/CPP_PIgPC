#include "mainwindow.h"
//#include "ui_mainwindow.h"

#include <QDebug>
#include <QFontMetrics>
#include <QKeyEvent>
#include <QLabel>
#include <QScrollBar>
#include <QTextCodec>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "PIg/src/parameter/param.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));

    setWindowTitle("BSP");

    QHBoxLayout *hl = new QHBoxLayout(this);

    QLabel *lport = new QLabel("Port:");

    cmbPort = new QComboBox();
    QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy2.setHorizontalStretch(1);
    sizePolicy2.setVerticalStretch(0);
    cmbPort->setSizePolicy(sizePolicy2);
    pbRefresh = new QPushButton("Refresh");
    pbPort = new QPushButton("Open");

    QHBoxLayout *hlport = new QHBoxLayout();
    hlport->addWidget(lport);
    hlport->addWidget(cmbPort);
    hlport->addWidget(pbRefresh);
    hlport->addWidget(pbPort);
    QVBoxLayout *vl = new QVBoxLayout();

    glb.crtTreeUser();
    glb.crtTreeState();
    glb.crtTreeInterface();
    glb.crtTreeDevice();

    oth.crtTreePrm();
    oth.crtTreePrd();
    oth.crtTreeGlb();

    vl->addLayout(hlport);
    vl->addWidget(&glb);

    hl->addLayout(vl);
    hl->addWidget(&oth);
    hl->addWidget(&jrn);

    jrn.crtJournals();

    glb.initParam();

    Bsp::initClock();
    initViewCommands();

    refreshPortList();
    connect(pbRefresh, &QPushButton::clicked,
            this, &MainWindow::refreshPortList);
    connect(pbPort, &QPushButton::clicked,
            this, &MainWindow::connectSerialPort);
}

//
MainWindow::~MainWindow() {
    //    delete ui;
}

//
void MainWindow::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    setMinimumHeight(750);
    setFixedWidth(width());
    resize(sizeHint());
}

//
void
MainWindow::initViewCommands() {
    viewCom.append(GB_COM_GET_JRN_IS_CNT);
    viewCom.append(GB_COM_GET_JRN_IS_ENTRY);
    viewCom.append(GB_COM_JRN_IS_CLR);
    viewCom.append(GB_COM_JRN_IS_SET_ENTRY);
}

//
void MainWindow::refreshPortList() {
    QString portname = cmbPort->currentText();
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();

    cmbPort->clear();
    for (const QSerialPortInfo &info :infos) {
        QString portname = info.portName();
        cmbPort->addItem(portname);
    }

    if (portname.isEmpty()) {
        cmbPort->setCurrentText("COM21");
        cmbPort->setCurrentText("tnt1");
    } else {
        cmbPort->setCurrentText(portname);
    }
    pbPort->setEnabled(cmbPort->count() != 0);
}

//
void MainWindow::connectSerialPort() {
    if (port.isNull() && thread.isNull()) {
        port = new SerialPort(cmbPort->currentText());
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

        disconnect(pbPort, &QPushButton::clicked,
                   this, &MainWindow::connectSerialPort);
        connect(pbPort, &QPushButton::clicked, port, &SerialPort::stop);

        cmbPort->setEnabled(false);
        pbPort->setText("Close");
        pbRefresh->setEnabled(false);

        port->moveToThread(thread);
        thread->start();

        QObject::connect(&timer, &QTimer::timeout, port, &SerialPort::proc);
        timer.start(5);
    } else {
        qDebug() << "";
    }
}

void MainWindow::closeSerialPort() {
    connect(pbPort, &QPushButton::clicked,
            this, &MainWindow::connectSerialPort);

    cmbPort->setEnabled(true);
    pbPort->setText("Open");
    pbPort->setEnabled(true);
    pbRefresh->setEnabled(true);
}

void MainWindow::readByte(int value) {
    static pkg_t pkgrx;
    static uint8_t len = 0;

    switch(pkgrx.size()) {
    case 0: {
        if (value == 0x55) {
            pkgrx.clear();
            pkgrx.append(value);
        }
    } break;
    case 1: {
        if (value == 0xAA) {
            pkgrx.append(value);
        } else {
            pkgrx.clear();
        }
    }break;
    case 2: {
        pkgrx.append(value);
    } break;
    case 3: {
        pkgrx.append(value);
        len = value;
    } break;
    default: {
        pkgrx.append(value);
        if (len == 0) {
            pkg_t temprx = pkgrx;
            eGB_COM com = Bsp::checkPkg(pkgrx);
            if (com != GB_COM_NO) {
                if (viewCom.count(com) != 0) {
                    qDebug() << "comRx <<< " << showbase << hex << temprx;
                }

                Bsp::procCommand(com, pkgrx);

                if (!Bsp::pkgTx.isEmpty()) {
                    Bsp::pkgTx.insert(1, Bsp::pkgTx.size() - 1);
                    Bsp::pkgTx.append(Bsp::calcCrc(Bsp::pkgTx));
                    Bsp::pkgTx.insert(0, 0x55);
                    Bsp::pkgTx.insert(1, 0xAA);

                    com = static_cast<eGB_COM> (Bsp::pkgTx.at(2));
                    if (viewCom.count(com) != 0) {
                        qDebug() << "comTx >>> " << showbase << hex << Bsp::pkgTx;
                    }

                    for(uint8_t byte: Bsp::pkgTx) {
                        emit writeByte(byte);
                    }
                    Bsp::pkgTx.clear();
                }
            }
            pkgrx.clear();
        } else {
            len--;
        }
    }
    }
}
