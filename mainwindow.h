#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include <QElapsedTimer>
#include <QLineEdit>
#include <QMainWindow>
#include <QPointer>
#include <QTextCodec>
#include <QTimer>
#include <QThread>
#include <QTreeWidgetItem>
#include "wrapper.hpp"
#include "serialport.h"

#include "PIg/src/drivers/ks0108.h"
#include "PIg/src/menu/menu.h"
#include "PIg/src/protocols/standart/protocolBspS.h"
#include "PIg/src/protocols/standart/protocolPcS.h"
#include "PIg/src/protocols/modbus/protocolPcM.h"
#include "PIg/src/protocols/iec101/protocolPcI.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/// Период вызова cycleMenu()
#define TIME_CYLCE 100

/// Максимальное кол-во неполученных сообщений от БСП для ошибки связи
#define MAX_LOST_COM_FROM_BSP 10

/// Размер буфера для общения с БСП
#define BUFF_SIZE_BSP 128

/// Размер буфера для общения с ПК
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

    struct view_t {
        QLineEdit userPi;
        QLineEdit userPc;
        QLineEdit engPwd;
        QLineEdit admPwd;
        QLineEdit engCounter;
        QLineEdit admCounter;
    } view;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

signals:
    void userChanged(int value);
    void writeByteToBsp(int value);
    void writeByteToPc(int value);

private:
    Ui::MainWindow *ui;
    QTextCodec *codec;

    QPointer<SerialPort> portBSP;
    QPointer<QThread> threadBSP;

    QPointer<SerialPort> portPC;
    QPointer<QThread> threadPC;

    QElapsedTimer etimer;

    /// Инициализация параметров.
    void initView();
    /// Настройка тестовых кнопок.
    void setupTestButtons();
    /// Отображение параметров.
    void hdlrView();
    /// Настраивает элемент отображаемых параметров.
    void addViewItem(QTreeWidgetItem *top, std::string name, QLineEdit *lineedit);
    /// Обработчик событий.
    bool eventFilter(QObject* object, QEvent* event) override;
    /// Обработчик события после отображения формы.
    void showEvent( QShowEvent* event ) override;

private slots:
    void cycleMenu();  ///< Цикл 200 мс.
    void clearSelection();  ///< Очистка выделения в textEdit.
    void setBacklight(bool enable);

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

    QString pwdToString(uint8_t *pwd);

    void test1();
    void test2();
    void test3();
    void test4();
};

// Для использвоании в wrapper
extern MainWindow *w;
#endif // MAINWINDOW_H
