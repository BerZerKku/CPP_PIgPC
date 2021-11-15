#include "mainwindow.h"
//#include "ui_mainwindow.h"

#include "PIg/src/parameter/param.h"
#include <QDebug>
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QScrollBar>
#include <QTextCodec>
#include <QTimer>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));

    setWindowTitle("BSP");

    QVBoxLayout *vl = new QVBoxLayout(this);

    port = new TSerial();
    port->setup(4800, QSerialPort::NoParity, QSerialPort::TwoStop);
    port->addDefaultPort("COM21");
    port->addDefaultPort("tnt1");

    connect(port, &TSerial::read, this, &MainWindow::readByte);
    connect(this, &MainWindow::writeByte, port, &TSerial::write);

    mBsp.crtTreeDevice();
    mBsp.crtTreeState();
    mBsp.crtTreePrm();
    mBsp.crtTreePrd();
    mBsp.crtTreeGlb();
    mBsp.crtTreeInterface();
    mBsp.crtTest();
    mBsp.crtTreeDef();
    mBsp.crtTreeMeasure();

    mBsp.initParam();

    vl->addWidget(port);
    vl->addWidget(&mBsp);

    Bsp::initClock();
    initViewCommands();
}

//
MainWindow::~MainWindow()
{
    //    delete ui;
}

//
void MainWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    setMinimumHeight(750);
    setFixedWidth(width());
    resize(sizeHint());
}

//
void MainWindow::initViewCommands()
{
    //    viewCom.append(GB_COM_GET_JRN_IS_CNT);
    //    viewCom.append(GB_COM_GET_JRN_IS_ENTRY);
    //    viewCom.append(GB_COM_JRN_IS_CLR);
    //    viewCom.append(GB_COM_JRN_IS_SET_ENTRY);
    //    viewCom.append(GB_COM_SET_REG_TEST_1);
    //    viewCom.append(GB_COM_SET_REG_TEST_2);
    //    viewCom.append(GB_COM_GET_TEST);
    //    viewCom.append(GB_COM_GET_VERS);

    viewCom.append(GB_COM_DEF_GET_TYPE_AC);
    viewCom.append(GB_COM_DEF_SET_TYPE_AC);
    viewCom.append(GB_COM_GET_MEAS);
}

//
void MainWindow::readByte(int value)
{
    static pkg_t   pkgrx;
    static uint8_t len = 0;

    switch (pkgrx.size())
    {
    case 0:
        {
            if (value == 0x55)
            {
                pkgrx.clear();
                pkgrx.append(value);
            }
        }
        break;
    case 1:
        {
            if (value == 0xAA)
            {
                pkgrx.append(value);
            }
            else
            {
                pkgrx.clear();
            }
        }
        break;
    case 2:
        {
            pkgrx.append(value);
        }
        break;
    case 3:
        {
            pkgrx.append(value);
            len = value;
        }
        break;
    default:
        {
            pkgrx.append(value);
            if (len == 0)
            {
                pkg_t   temprx = pkgrx;
                eGB_COM com;
                if (Bsp::checkPkg(pkgrx, com))
                {
                    if (viewCom.count(com) != 0)
                    {
                        qDebug() << "comRx <<< " << showbase << hex << temprx;
                    }

                    Bsp::procCommand(com, pkgrx);

                    if (!Bsp::pkgTx.isEmpty())
                    {
                        Bsp::pkgTx.insert(1, Bsp::pkgTx.size() - 1);
                        Bsp::pkgTx.append(Bsp::calcCrc(Bsp::pkgTx));
                        Bsp::pkgTx.insert(0, 0x55);
                        Bsp::pkgTx.insert(1, 0xAA);

                        com = static_cast<eGB_COM>(Bsp::pkgTx.at(2));
                        if (viewCom.count(com) != 0)
                        {
                            qDebug() << "comTx >>> " << showbase << hex << Bsp::pkgTx;
                        }

                        for (uint8_t byte : Bsp::pkgTx)
                        {
                            emit writeByte(byte);
                        }
                        Bsp::pkgTx.clear();
                    }
                }
                pkgrx.clear();
            }
            else
            {
                len--;
            }
        }
    }
}
