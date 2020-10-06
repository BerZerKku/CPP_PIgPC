#include <QDebug>
#include <QtGlobal>
#include <QTest>
#include "serialport.h"

SerialPort::SerialPort(QString portname, qint32 baudrate) :
    m_baudrate(baudrate), m_portName(portname) {

}

SerialPort::~SerialPort() {
}

//
void SerialPort::error(QSerialPort::SerialPortError err) {
    if (err == QSerialPort::ResourceError) {
        stop();
    }
}

//
void SerialPort::start() {
    if (port == nullptr) {
        port = new QSerialPort(m_portName);
        if (!port->setBaudRate(m_baudrate)) {
            emit finished();
        }
        port->setStopBits(QSerialPort::StopBits::TwoStop);
    }

    if (timer.isNull()) {
        timer = new QTimer();
        timer->setInterval(1);
        connect(timer, &QTimer::timeout, this, &SerialPort::timeoutSlot);
    } else {
        timer->stop();
    }

    if (port->open(QSerialPort::ReadWrite)) {
        connect(port, &QSerialPort::errorOccurred, this, &SerialPort::error);
        connect(port, &QSerialPort::readyRead, this, &SerialPort::readyReadSlot);
        m_timeToFinishSendMs = 0.0;
        bufTx.clear();
        m_byteSendMs = calcOneByteSendTime();
        qDebug() << "m_byteSendMs = " << m_byteSendMs;
        port->clear();
    } else {
        emit finished();
    }
}

//
void SerialPort::stop() {
    timer->stop();
    port->close();
    emit finished();
}

//
void SerialPort::writeByte(int byte) {
//    const char b = static_cast<char> (byte);

    if (port->isOpen()) {        
        bufTx.append(byte);
        if (!timer->isActive()) {
            m_timeToFinishSendMs = 0.0;
            timer->start();
        }
    } else {
        stop();
    }
}

//
void SerialPort::readyReadSlot() {
    for(auto &byte: port->readAll()) {
        emit readByte(static_cast<uint8_t> (byte));
    }
}

//
void SerialPort::timeoutSlot() {
    m_timeToFinishSendMs += 1.0;    // 1.0 шаг таймера

    while(m_timeToFinishSendMs >= m_byteSendMs) {
        if (bufTx.isEmpty()) {
            timer->stop();
            emit sendFinished();
            break;
        }

        char byte = bufTx.takeFirst();
        port->write(&byte, 1);
    }
}

//
double SerialPort::calcOneByteSendTime() const {
    double numbites = 1.0;

    switch(port->dataBits()) {
        case QSerialPort::Data5: {
            numbites += 5.0;
        } break;
        case QSerialPort::Data6: {
            numbites += 6.0;
        } break;
        case QSerialPort::Data7: {
            numbites += 7.0;
        } break;
        case QSerialPort::Data8: {
            numbites += 8.0;
        } break;
        default: {
            numbites += 8.0;
            qCritical() << "Error data bits value";
        }
    }

    switch(port->stopBits()){
        case QSerialPort::OneStop: {
            numbites += 1.0;
        } break;
        case QSerialPort::OneAndHalfStop: {
            numbites += 1.5;
        } break;
        case  QSerialPort::TwoStop: {
            numbites += 2.0;
        }break;
        default: {
            numbites += 8.0;
            qCritical() << "Error stop bits value";
        }
    }

    return (1000 * numbites) / port->baudRate();
}
