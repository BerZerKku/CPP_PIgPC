#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::cycleMenu);
    timer->start(200);

    connect(ui->textEdit, &QTextEdit::selectionChanged,
            this, &MainWindow::clearSelection);

    // FIXME Сделать подсчет необходимого размера виджета.
    ui->textEdit->setFixedSize(340, 240);

    menuInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test()
{
    qDebug() << __FUNCTION__;
}

//
void MainWindow::menuInit()
{
    menu.sParam.password.init(0000);
    menu.sParam.Uart.Interface.set(TInterface::USB);
    menu.sParam.Uart.Protocol.set(TProtocol::STANDART);
    menu.sParam.Uart.BaudRate.set(TBaudRate::_19200);
    menu.sParam.Uart.DataBits.set(TDataBits::_8);
    menu.sParam.Uart.Parity.set(TParity::EVEN);
    menu.sParam.Uart.StopBits.set(TStopBits::ONE);

    menu.sParam.def.status.setEnable(true);

    menu.sParam.prm.setNumCom(32);
    menu.sParam.local.setNumComPrm(32);

    menu.sParam.prd.setNumCom(32);
    menu.sParam.local.setNumComPrd(32);

    menu.sParam.glb.setNumDevices(GB_NUM_DEVICES_2);
    menu.sParam.def.setNumDevices(GB_NUM_DEVICES_2);
    menu.sParam.local.setNumDevices(menu.sParam.glb.getNumDevices()+1);

    menu.sParam.glb.setTypeLine(GB_TYPE_LINE_OPTO);

    menu.sParam.glb.setVersProgIC16(0x1112, GB_IC_BSP_MCU);
    menu.sParam.glb.setVersProgIC16(0x2122, GB_IC_BSP_DSP);
    menu.sParam.glb.setVersProgIC16(0x3132, GB_IC_PI_MCU);
    menu.sParam.glb.setVersProgIC16(0x4142, GB_IC_BSP_DSP_PLIS);
    menu.sParam.glb.setVersProgIC8(0x51, GB_IC_BSK_PLIS_PRD1);
    menu.sParam.glb.setVersProgIC8(0x61, GB_IC_BSK_PLIS_PRD2);
    menu.sParam.glb.setVersProgIC8(0x71, GB_IC_BSK_PLIS_PRM1);
    menu.sParam.glb.setVersProgIC8(0x81, GB_IC_BSK_PLIS_PRM2);
    menu.sParam.glb.setVersProgIC8(0x91, GB_IC_BSZ_PLIS);

    menu.sParam.glb.setCompatibility(GB_COMPATIBILITY_AVANT);
    menu.sParam.glb.setCompK400(GB_COMP_K400_AVANT);

    menu.sParam.glb.setTypeDevice(AVANT_OPTO);
    menu.sParam.glb.setTypeOpto(TYPE_OPTO_STANDART);

    menu.sParam.device = false;

    menu.setConnectionBsp(true);

    menu.sParam.DateTime.setYear(20);
    menu.sParam.DateTime.setMonth(9);
    menu.sParam.DateTime.setDay(8);
    menu.sParam.DateTime.setHour(0);
    menu.sParam.DateTime.setMinute(8);
    menu.sParam.DateTime.setSecond(37);
}

void MainWindow::cycleMenu()
{
    menu.main();
}

void MainWindow::clearSelection()
{
    QTextCursor c = ui->textEdit->textCursor();
    c.clearSelection();
    ui->textEdit->setTextCursor(c);
}
