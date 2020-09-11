#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include <QMainWindow>
#include "wrapper.hpp"
#include "bsp.h"

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

private:
    Ui::MainWindow *ui;

    clMenu menu;        ///< Меню.

    Bsp bsp;                    ///< Блок БСП.
    uint8_t bspBuf[128];        ///< Буфер для протокола общения с БСП.
    clProtocolBspS *protBSPs;   ///< Протокол общения с БСП.

    /// Инициализация меню.
    void initParam();
    /// Обработчик событий.
    bool eventFilter(QObject* object, QEvent* event) override;
    /// Обработчик события после отображения формы.
    void showEvent( QShowEvent* event ) override;
    /// Обработка принятых сообщений из БСП
    void uartRead();
    /// Передача сообщений в БСП.
    void uartWrite();

private slots:
    void cycleMenu();  ///< Цикл 200 мс.
    void clearSelection();  ///< Очистка выделения в textEdit.
    void printDebug(QString msg);
    void setBacklight(bool enable);
};
#endif // MAINWINDOW_H
