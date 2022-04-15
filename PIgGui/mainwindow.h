#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "wrapper.h"
#include <QDateTime>
#include <QElapsedTimer>
#include <QLineEdit>
#include <QMainWindow>
#include <QPointer>
#include <QPushButton>
#include <QTextCodec>
#include <QThread>
#include <QTimer>
#include <QTreeWidgetItem>

#include "PIg/src/drivers/ks0108.h"
#include "PIg/src/menu/menu.h"

#include "bsp/bsp.h"
#include "qprotocolviewer.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
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

    friend void     vKEYmain(void);
    friend uint16_t eKEYget(void);
    friend uint8_t  timePressKey();
    friend void     vLCDclear(void);
    friend void     vLCDinit(void);
    friend void     vLCDmain(void);
    friend void     vLCDrefresh(void);
    friend bool     vLCDdrawBoard(uint8_t num);
    friend bool     vLCDputchar(const char *buf, uint8_t num);
    friend void     vLCDsetLed(eLCD_LED val);
    friend void     vLCDled();

    struct view_t
    {
        QLineEdit userPwd;
        QLineEdit admPwd;
        QLineEdit lnProtocol;
        QLineEdit lnAddress;
        QLineEdit lnBaudrate;
        QLineEdit lnDataBits;
        QLineEdit lnParity;
        QLineEdit lnStopBits;
        QLineEdit regimeDef;
        QLineEdit regimePrm;
        QLineEdit regimePrd;
        QLineEdit typeDevice;
        QLineEdit def;
        QLineEdit numComPrm;
        QLineEdit numComPrd;
        QLineEdit deviceNumber;
        QLineEdit numDevices;
        QLineEdit typeCommLine;
        QLineEdit typeCompatibility;
        QLineEdit typeOpto;
    } view;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

signals:
    void writeByteToBsp(int value);

private:
    Ui::MainWindow *ui;
    QTextCodec *    codec;
    Bsp *           m_bsp;

    QPalette      pred;
    QPalette      pblue;
    QPalette      pdefault;
    QElapsedTimer etimer;

    QProtocolViewer mProtocolViewer;

    void initView();
    void initKeyboard();
    void InitProtocolViewer();

    /// ��������� �������� ������.
    void setupTestButtons();
    /// ����������� ����������.
    void hdlrView();
    /// ����������� ������� ������������ ����������.
    void addViewItem(QTreeWidgetItem *top, std::string name, QLineEdit *lineedit);
    /// ���������� �������.
    bool eventFilter(QObject *object, QEvent *event) override;
    /// ���������� ������� ����� ����������� �����.
    void showEvent(QShowEvent *event) override;

private slots:
    void cycleMenu();       ///< ���� 200 ��.
    void clearSelection();  ///< ������� ��������� � textEdit.
    void setBacklight(bool enable);
    void SlotBspConnection();

    void SlotBytePiToBsp(uint8_t byte);
    void SlotByteBspToPi(uint8_t byte);

    void test1();
    void test2();
    void test3();
    void test4();
};

// ��� ������������� � wrapper
extern MainWindow *w;
#endif  // MAINWINDOW_H
