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
#include "PIg/src/protocols/iec101/protocolPcI.h"
#include "PIg/src/protocols/modbus/protocolPcM.h"
#include "PIg/src/protocols/standart/protocolBspS.h"
#include "PIg/src/protocols/standart/protocolPcS.h"

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

    friend void    vKEYmain(void);
    friend uint8_t eKEYget(void);
    friend uint8_t timePressKey();
    friend void    vLCDclear(void);
    friend void    vLCDinit(void);
    friend void    vLCDmain(void);
    friend void    vLCDrefresh(void);
    friend bool    vLCDdrawBoard(uint8_t num);
    friend bool    vLCDputchar(const char *buf, uint8_t num);
    friend void    vLCDsetLed(eLCD_LED val);
    friend void    vLCDled();

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

    /// ����������.
    enum eDevice
    {
        DEVICE_R400M_100_HF_b15 = 0,
        DEVICE_RZSK_111_HF,
        DEVICE_K400_088_HF,
        DEVICE_R400_100_VOLS,
        DEVICE_RZSK_111_VOLS,
        DEVICE_K400_088_VOLS
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

signals:
    void userChanged(int value);
    void writeByteToBsp(int value);
    void writeByteToPc(int value);

private:
    Ui::MainWindow *ui;
    QTextCodec *    codec;
    Bsp *           mBsp = nullptr;

    QPalette      pred;
    QPalette      pblue;
    QPalette      pdefault;
    QElapsedTimer etimer;

    QProtocolViewer mProtocolViewer;

    void initBsp();
    void initEeprom();
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
    void SlotBspChange();

    void SlotByteBspToPi(uint8_t byte);
    void SlotBytePiToBsp(uint8_t byte);

    void test1();
    void test2();
    void test3();
    void test4();

    QString getDeviceName(eGB_TYPE_DEVICE type) const;
    QString getTypeLine(eGB_TYPE_LINE type) const;
    void    viewNumComPrd();
    void    viewNumComPrm();
    void    viewTypeComp();
    QString getTypeOpto(eGB_TYPE_OPTO type) const;
};

// ��� ������������� � wrapper
extern MainWindow *w;
#endif  // MAINWINDOW_H
