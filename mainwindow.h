#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
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
    QDateTime dt;

    stGBparam params;   ///< Параметры.
    clMenu menu;        ///< Меню.

    /// Преобразование bcd кода в целое.
    quint8 bcd2int(quint8 bcd, bool &ok) const;
    /// Обработка команды.
    void procCommand();
    /// Обработка команд чтения журналов.
    void procCommandReadJournal(eGB_COM com);
    /// Обработка команд чтения параметров.
    void procCommandReadParam(eGB_COM com);
    /// Обработка команд записи параметров.
    void procCommandWriteParam(eGB_COM com);
    /// Обработка команд записи режима.
    void procCommandWriteRegime(eGB_COM com);
    /// Инициализация времени.
    void initClock();
    /// Инициализация меню.
    void initParam();
    /// Обработчик событий.
    bool eventFilter(QObject* object, QEvent* event) override;
    /// Обработчик события после отображения формы.
    void showEvent( QShowEvent* event ) override;


private slots:
    void cycleMenu();  ///< Цикл 200 мс.
    void clearSelection();  ///< Очистка выделения в textEdit.
    void printDebug(QString msg);
    void setBacklight(bool enable);
    void updateClock();
};
#endif // MAINWINDOW_H
