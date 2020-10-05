#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include <QElapsedTimer>
#include <QMainWindow>
#include <QPointer>
#include <QTimer>
#include <QThread>
#include "wrapper.hpp"
#include "serialport.h"

#include "PIg/src/drivers/ks0108.h"
#include "PIg/src/menu/menu.h"
#include "PIg/src/protocols/standart/protocolBspS.h"
#include "PIg/src/protocols/standart/protocolPcS.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/// ������ ������ cycleMenu()
#define TIME_CYLCE 100

/// ������������ ���-�� ������������ ��������� �� ��� ��� ������ �����
#define MAX_LOST_COM_FROM_BSP 10

/// ������ ������ ��� ������� � ���
#define BUFF_SIZE_BSP 128

/// ������ ������ ��� ������� � ��
#define BUFF_SIZE_PC 128

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend void vKEYmain(void);
    friend void vKEYset(eGB_TYPE_DEVICE type);
    friend eKEY eKEYget(void);
    friend uint8_t timePressKey();
    friend void vLCDclear(void);
    friend void vLCDinit(void);
    friend void vLCDmain(void);
    friend void vLCDrefresh(void);
    friend bool vLCDdrawBoard(uint8_t num);
    friend bool vLCDputchar(const char* buf, uint8_t num);
    friend void vLCDsetLed(eLCD_LED val);
    friend void vLCDled();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

signals:
    void userChanged(int value);
    void writeByteToBsp(int value);
    void writeByteToPc(int value);

private:
    Ui::MainWindow *ui;
    clMenu menu;                ///< ����.
    uint8_t bspBuf[BUFF_SIZE_BSP];        ///< ����� ��� ��������� ������� � ���.
    clProtocolBspS *protBSPs;   ///< �������� ������� � ���.
    uint8_t pcBuf[BUFF_SIZE_PC];        ///< ����� ��� ��������� ������� � ���.
    clProtocolPcS *protPCs;   ///< �������� ������� � ���.

    QPointer<SerialPort> portBSP;
    QPointer<QThread> threadBSP;

    QPointer<SerialPort> portPC;
    QPointer<QThread> threadPC;

    QElapsedTimer etimer;

    /// ������������� ����������.
    void initParam();
    /// ���������� �������.
    bool eventFilter(QObject* object, QEvent* event) override;
    /// ���������� ������� ����� ����������� �����.
    void showEvent( QShowEvent* event ) override;
    /// ��������� �������� ���������.
    bool uartRead();
    /// �������� ���������.
    bool uartWrite();

private slots:
    void cycleMenu();  ///< ���� 200 ��.
    void clearSelection();  ///< ������� ��������� � textEdit.
    void setBacklight(bool enable);
    void setUser(int value);

    void refreshPortListBsp();
    void connectSerialPortBSP();
    void closeSerialPortBSP();
    void readByteFromBsp(int value);
    void resetStatusBSP();

    void refreshPortListPc();
    void connectSerialPortPc();
    void closeSerialPortPc();
    void readByteFromPc(int value);
    void resetStatusPc();
};

// ��� ������������� � wrapper
extern MainWindow *w;
#endif // MAINWINDOW_H
