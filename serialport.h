#ifndef SLAVEPORT_H
#define SLAVEPORT_H

#include <QObject>
#include <QPointer>
#include <QString>
#include <QSerialPort>
#include <QTimer>
#include <QVector>

class SerialPort : public QObject {
    Q_OBJECT

public:
    explicit SerialPort(QString portname, qint32 baudrate);
    ~SerialPort();

public slots:
    void error(QSerialPort::SerialPortError err);
    void start();
    void stop();
    void writeByte(int byte);
    void readyReadSlot();
    void timeoutSlot();

signals:
    void finished();
    void readByte(int byte);
    void sendFinished();

private:
    qint32 m_baudrate = 0;
    QSerialPort *port = nullptr;
    QString m_portName;
    bool m_send = false;
    QPointer<QTimer> timer;
    double m_timeToFinishSendMs = 0.0;
    double m_byteSendMs = 0.0;

    double calcOneByteSendTime() const;
};

#endif // SLAVEPORT_H
