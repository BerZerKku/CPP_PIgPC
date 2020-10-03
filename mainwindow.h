#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include <QWidget>
#include "bsp.h"

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Bsp glb;
    Bsp oth;

    /// Обработчик события после отображения формы.
    void showEvent( QShowEvent* event ) override;

    /// Подсчет контрольной суммы.
    uint8_t calcCrc(pkg_t &pkg);
    /// Проверка пакета.
    eGB_COM checkPkg(pkg_t &pkg); 

    /** Получает пакет данных из БСП
     *
     *  @return Данные.
     */
    pkg_t receiveFromBsp();

    /** Передает пакет данных в БСП.
     *
     *  @param[in] pkg Данные.
     */
    void sendToBsp(pkg_t pkg);

private slots:


};
#endif // MAINWINDOW_H
