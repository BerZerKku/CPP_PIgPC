#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "wrapper.hpp"

#include "PIg/src/drivers/ks0108.h"
#include "PIg/src/menu/menu.h"

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

    clMenu menu;    ///< Меню.

    /// Инициализация меню.
    void initParam();
//    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject* object, QEvent* event) override;

private slots:
    void cycleMenu();  ///< Цикл 200 мс.
    void clearSelection();  ///< Очистка выделения в textEdit.
    void setBacklight(bool enable);
};
#endif // MAINWINDOW_H
