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

    setWindowTitle("BSP-PI");

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));

    connect(ui->textEdit, &QTextEdit::selectionChanged,
            this, &MainWindow::clearSelection);

    initParam();

    protBSPs = new clProtocolBspS(bspBuf, SIZE_OF(bspBuf), &menu.sParam);
    protBSPs->setEnable(PRTS_STATUS_NO);

    protPCs = new clProtocolPcS(pcBuf, SIZE_OF(pcBuf), &menu.sParam);
    protPCs->setEnable(PRTS_STATUS_NO); // FIXME ��� ������ ������ ���������� ���� ���������!

    protPCi = new TProtocolPcI(&menu.sParam, pcBuf, SIZE_OF(pcBuf));
    protPCi->setDisable();

    protPCm = new TProtocolPcM(&menu.sParam, pcBuf, SIZE_OF(pcBuf));
    protPCm->setDisable();

    installEventFilter(this);
    ui->textEdit->installEventFilter(this);

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(20);
    ui->textEdit->setFont(font);

    QFontMetrics mfont(font);
    QSize size = mfont.size(Qt::TextSingleLine, "12345678901234567890");

    // FIXME ����������� ������ ��������� ������ �����
    ui->textEdit->setFixedSize(size.width() + 12, size.height()*7 + 10);
    ui->kbd->setFixedSize(ui->textEdit->width(), ui->textEdit->width());
    setFixedSize(sizeHint());

    // ������� �������� ����������� ��� ��������� �������� ����� ��� testEdit
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
        // � ��������� ��������� �� ������� Bsp ���� ���.
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
bool MainWindow::uartRead() {
    bool stat = true;
    // ��� ������������� � �� �������
    static uint8_t lastPcCom = 0;
    // ���-�� ������������ ��������� � ���
    static uint8_t cntLostCom = 0;

    // ����� ������� �������� ������ �� ���������
    protBSPs->checkStat();
    // �������� ������� ��������� � ��� � �� ���������
    if (protBSPs->getCurrentStatus() == PRTS_STATUS_READ_OK) {
        // �������� ����������� ����� ����������� ��������� �
        // ��������� ������ ���� ��� ������������� ����������        
        if (protBSPs->checkReadData()) {
            // ��������� ��������� ���������
            protBSPs->getData(lastPcCom == protBSPs->getCurrentCom());

            // �������� ������������ ������� ����������� � �� � �������
            // ���������� �� ��� � ���� ��������� ��������� ��������� �� ��
            // ��� ������� GB_COM_GET_VERS ���������� ���������� ������ ���-��
            if (lastPcCom == protBSPs->getCurrentCom()) {
                if (protPCs->isEnable()) {
                    if (protPCs->copyCommandFrom(protBSPs->buf)) {
                        protPCs->modifyVersionCom();
                    }
                }
            }
        }

        // ����� �������� � ��������� ��������� ������� ��� ����������
        // ������������� �������
        lastPcCom = 0;
        // ����� �������� ���������� ��������� � ���
        cntLostCom = 0;
    } else {
        // � ������ ���������� ������ ���������� ��������� ��� ������ � ���
        // ���� ��������� ������������ � False
        if (cntLostCom < MAX_LOST_COM_FROM_BSP)
            cntLostCom++;
        else
            stat = false;
    }

    if (protPCs->isEnable()) {
        // ����� ������� �������� ������ �� ���������
        protPCs->checkStat();
        // �������� ������� ������� � �� � �� ���������
        if (protPCs->getCurrentStatus() == PRTS_STATUS_READ_OK) {
            // �������� ����������� ����� ����������� ��������� �
            // ��������� ������ ���� ��� ������������� ����������
            if (protPCs->checkReadData()) {
                // ��������� ��������� ���������
                // ���� ��������� ������ ����������, �������� ��� � ���
                // (�.�. ���� ��� �� ������/��������� ������)
                if (!protPCs->getData()) {
                    etimer.start();
                    // ���������� ������������� �� �������
                    lastPcCom = protPCs->getCurrentCom();
                   // ��������� ��������� � ���
                    if (protBSPs->getCurrentStatus() == PRTS_STATUS_NO) {
                        protBSPs->copyCommandFrom(protPCs->buf);
                        protPCs->setCurrentStatus(PRTS_STATUS_WAIT_ANSWER);
                    }
                }
            }
        }
    } else if (protPCm->isEnable()) {
        if (protPCm->isReadData()) {
            protPCm->readData();
        }
    } else if (protPCi->isEnable()) {
        if (protPCi->isReadData()) {
            protPCi->readData();
        }
    }

    return stat;
}

uint8_t cntsendtobsp = 0;

//
bool MainWindow::uartWrite() {
    QVector<uint8_t> pkg;
    uint8_t len = 0;

    if (protPCs->isEnable()) {
        // ����� ��������� �������� ������ ��������� �� ���������.
        protPCs->checkStat();
        // �������� ������������� �������� ������� �� �� � �� ��������
        ePRTS_STATUS stat = protPCs->getCurrentStatus();
        if (stat == PRTS_STATUS_WRITE_PC) {            
            // ��������� ������ ���
            len = protPCs->trCom();            
        } else if (stat == PRTS_STATUS_WRITE) {
            // �������� ������ ��
            len = protPCs->trCom();
        }
    } else if (protPCm->isEnable()) {
        len = protPCm->sendData();
    } else if (protPCi->isEnable()) {
        len = protPCi->sendData();
    }

    pkg.clear();
    for(uint8_t i = 0; i < len; i++) {
        pkg.append(pcBuf[i]);
        emit writeByteToPc(pcBuf[i]);
    }

    // ����� ��������� �������� ������ ��������� �� ���������.
    len = 0;
    protBSPs->checkStat();
    // �������� ��� �� ������������� �������� ������� � ��
    // ���� ���, �� ������� ������� � ����
    ePRTS_STATUS stat = protBSPs->getCurrentStatus();
    if (stat == PRTS_STATUS_WRITE_PC) {
        // ��������� ������� ��
        len = protBSPs->trCom();
        pkg.clear();
        for(uint8_t i = 0; i < len; i++) {
            pkg.append(bspBuf[i]);
        }
    } else if (stat == PRTS_STATUS_NO) {
        // �������� ������� ���
        eGB_COM com = menu.getTxCommand();
        // ���� ���� �������, ���������� �� � ���
        if (com != GB_COM_NO) {
            len = protBSPs->sendData(com);
        }
    }

    if (len > 0) {
        cntsendtobsp++;
    }

    pkg.clear();
    for(uint16_t i = 0; i < len; i++) {
        pkg.append(bspBuf[i]);
        emit  writeByteToBsp(bspBuf[i]);
    }
//    qDebug() << "Send to BSP: " << showbase << hex << pkg;

    return true;
}

//
void MainWindow::cycleMenu() {
    static TUser::user_t user = TUser::MAX;
    static uint8_t cnt_lcd = 0;
    static uint8_t cnt1s = 0;

    bool connect = uartRead();
    menu.setConnectionBsp(connect);

    // ���������� ������
    // ��� 100 - ����� �������� �����
    if (++cnt_lcd >= (MENU_TIME_CYLCE / TIME_CYLCE)) {
        cnt_lcd = 0;
        menu.proc();
    }

    // �������� ��������� � ���/��
    uartWrite();

    vLCDled();

//    if (user != menu.sParam.security.User.get()) {
//        user = menu.sParam.security.User.get();
//        emit userChanged(user);
//    }

    cnt1s++;
    if (cnt1s >= 10) {
        qDebug() << "Send " << cntsendtobsp << " packet to BSP per second.";
        cntsendtobsp = 0;
        cnt1s = 0;
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
            ui->cmbPortBsp->setCurrentText("COM6");
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
    protBSPs->checkByte(static_cast<uint8_t> (value));
}

//
void MainWindow::resetStatusBSP() {
    protBSPs->setCurrentStatus(PRTS_STATUS_NO);
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
            ui->cmbPortPc->setCurrentText("COM12");
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
    if (protPCs->isEnable()) {
        // �������� "�����������"
        protPCs->checkByte(byte);
    } else if (protPCm->isEnable()) {
        // �������� MODBUS
        protPCm->push(byte);
    } else if (protPCi->isEnable()) {
        protPCi->push(byte);
    }
}

//
void MainWindow::resetStatusPc() {
    if (protPCs->isEnable()) {
        protPCs->setCurrentStatus(PRTS_STATUS_NO);
    } else if (protPCm->isEnable()) {
        protPCm->setReadState();
    } else if (protPCi->isEnable()) {
        protPCi->setReadState();
    }
}

