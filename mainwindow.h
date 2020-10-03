#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include <QMainWindow>
#include <QPointer>
#include <QTimer>
#include <QThread>
#include "wrapper.hpp"
#include "serialport.h"

#include "PIg/src/drivers/ks0108.h"
#include "PIg/src/menu/menu.h"
#include "PIg/src/protocols/standart/protocolBspS.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    ~MainWindow();

signals:
    void userChanged(int value);
    void writeByte(int value);

private:
    Ui::MainWindow *ui;
    clMenu menu;                ///< ����.
    uint8_t bspBuf[128];        ///< ����� ��� ��������� ������� � ���.
    clProtocolBspS *protBSPs;   ///< �������� ������� � ���.
    QPointer<SerialPort> port;
    QPointer<QThread> thread;
    QTimer timer;

    /// ������������� ����������.
    void initParam();
    /// ���������� �������.
    bool eventFilter(QObject* object, QEvent* event) override;
    /// ���������� ������� ����� ����������� �����.
    void showEvent( QShowEvent* event ) override;
    /// ��������� �������� ��������� �� ���
    void uartRead();
    /// �������� ��������� � ���.
    void uartWrite();

private slots:
    void cycleMenu();  ///< ���� 200 ��.
    void clearSelection();  ///< ������� ��������� � textEdit.
    void setBacklight(bool enable);
    void setUser(int value);
    void refreshPortList();
    void connectSerialPort();
    void closeSerialPort();
    void readByte(int value);
};

// ��� ������������� � wrapper
extern MainWindow *w;
#endif // MAINWINDOW_H
