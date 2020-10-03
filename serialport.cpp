#include <QDebug>
#include <QtGlobal>
#include <QTest>
#include "serialport.h"

SerialPort::SerialPort(QString portname) :
    m_portName(portname) {

}

SerialPort::~SerialPort() {

}

void SerialPort::start() {
    if (port == nullptr) {
        port = new QSerialPort(m_portName);
        if (!port->setBaudRate(19200)) {
            emit finished();
        }
    }

    if (port->open(QSerialPort::ReadWrite)) {
        m_stop = false;
        port->clear();
    } else {
        emit finished();
    }
}

void SerialPort::proc() {
    if(!m_stop) {
        QByteArray data(port->readAll());
        for(auto &byte: data) {
            emit readByte(static_cast<uint8_t> (byte));
        }
    } else {
        port->close();
        emit finished();
    }
}

void SerialPort::stop() {
    m_stop = true;
}

void SerialPort::writeByte(int byte) {
    const char b = byte;
    port->write(&b, 1);
}

