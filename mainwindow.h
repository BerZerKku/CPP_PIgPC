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

    /// ���������� ������� ����� ����������� �����.
    void showEvent( QShowEvent* event ) override;

    /// ������� ����������� �����.
    uint8_t calcCrc(pkg_t &pkg);
    /// �������� ������.
    eGB_COM checkPkg(pkg_t &pkg); 

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

private slots:


};
#endif // MAINWINDOW_H
