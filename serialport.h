#ifndef SLAVEPORT_H
#define SLAVEPORT_H

#include <QObject>
#include <QString>
#include <QSerialPort>
#include <QVector>

class SerialPort : public QObject {
    Q_OBJECT

public:
    explicit SerialPort(QString portname);
    ~SerialPort();

public slots:
    void start();
    void proc();
    void stop();
    void writeByte(int byte);

signals:
    void finished();
    void readByte(int byte);

private:
    bool m_stop = false;
    QSerialPort *port = nullptr;
    QString m_portName;
};

#endif // SLAVEPORT_H
