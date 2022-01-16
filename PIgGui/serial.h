#ifndef SERIAL_H
#define SERIAL_H

#include <QPointer>
#include <QThread>
#include <QTimer>
#include <QWidget>

#include "serial/serialport.h"

namespace Ui
{
class TSerial;
}

class TSerial : public QWidget
{
    Q_OBJECT

    const uint16_t kTimeLedOnMs = 200;
    const QString  kInternal    = "internal";

public:
    explicit TSerial(QWidget *parent = nullptr);
    ~TSerial();

    void setLabelText(QString text);
    bool setup(uint32_t baudrate, QSerialPort::Parity parity, QSerialPort::StopBits stopbits);
    void addDefaultPort(QString portname);

    void setLedLink(bool enabled);

signals:
    void write(int value);
    void openPort();
    void closePort();
    void read(int byte);
    void sendFinished();

private:
    Ui::TSerial *         ui;
    QPointer<TSerialPort> sport;
    QPointer<QThread>     thread;
    QVector<QString>      defaultPorts;
    QTimer                timerLedRx;
    QTimer                timerLedTx;

    uint32_t              mBaudrate = 0;
    QSerialPort::Parity   mParity   = QSerialPort::NoParity;
    QSerialPort::StopBits mStopBits = QSerialPort::OneStop;

private slots:
    void refreshPortList();
    void connectSerialPort();
    void closeSerialPort();
    void setLedRx(bool enable);
    void setLedTx(bool enable);
};

#endif  // SERIAL_H
