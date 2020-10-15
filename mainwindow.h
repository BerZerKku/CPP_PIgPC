#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "bsp.h"
#include <QDateTime>
#include <QPointer>
#include <QPushButton>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QThread>
#include <QWidget>
#include <serialport.h>


class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void writeByte(int value);

private:
    Bsp glb;
    Bsp oth;
    Bsp jrn;
    QVector<eGB_COM> viewCom;

    QComboBox *cmbPort;
    QPushButton *pbRefresh;
    QPushButton *pbPort;

    QPointer<SerialPort> port;
    QPointer<QThread> thread;
    QTimer timer;

    /// ���������� ������� ����� ����������� �����.
    void showEvent( QShowEvent* event ) override;

    /** �������� ����� ������ �� ���
     *
     *  @return ������.
     */
    pkg_t receiveFromBsp();

    /** �������� ����� ������ � ���.
     *
     *  @param[in] pkg ������.
     */
    void sendToBsp(pkg_t pkg);

    /// ��������� ������ ��� ����������.
    void initViewCommands();

private slots:
    void refreshPortList();
    void connectSerialPort();
    void closeSerialPort();
    void readByte(int value);
};
#endif // MAINWINDOW_H
