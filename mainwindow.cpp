#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFontMetrics>
#include <QKeyEvent>
#include <QTimer>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->textEdit, &QTextEdit::selectionChanged,
            this, &MainWindow::clearSelection);

    initParam();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::cycleMenu);
    timer->start(200);

    installEventFilter(this);
    ui->textEdit->installEventFilter(this);

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(20);
    ui->textEdit->setFont(font);

    QFontMetrics mfont(font);
    QSize size = mfont.size(Qt::TextSingleLine, "12345678901234567890");

    // FIXME Разобраться почему константы именно такие
    ui->textEdit->setFixedSize(size.width() + 10, size.height()*7 + 10);
    ui->kbd->setFixedSize(ui->textEdit->width(), ui->textEdit->width());
    setFixedSize(sizeHint());
    // Удаляет движение содержимого при прокрутке колесика мышки над testEdit
    ui->textEdit->verticalScrollBar()->blockSignals(true);
}

//
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeParam()
{
    eGB_COM com = menu.getTxCommand();

    switch(com) {
        case GB_COM_SET_REG_DISABLED: {
            menu.sParam.glb.status.setRegime(GB_REGIME_DISABLED);
            menu.sParam.def.status.setRegime(GB_REGIME_DISABLED);
            menu.sParam.prd.status.setRegime(GB_REGIME_DISABLED);
            menu.sParam.prm.status.setRegime(GB_REGIME_DISABLED);
        } break;
        case GB_COM_SET_REG_ENABLED: {
            menu.sParam.glb.status.setRegime(GB_REGIME_ENABLED);
            menu.sParam.def.status.setRegime(GB_REGIME_ENABLED);
            menu.sParam.prd.status.setRegime(GB_REGIME_ENABLED);
            menu.sParam.prm.status.setRegime(GB_REGIME_ENABLED);
        } break;

        case GB_COM_SET_TIME: {
            bool ok = false;
            uint8_t *buf = menu.sParam.txComBuf.getBuferAddress();
            menu.sParam.DateTime.setYear(bcd2int(buf[0], ok));
            menu.sParam.DateTime.setMonth(bcd2int(buf[1], ok));
            menu.sParam.DateTime.setDay(bcd2int(buf[2], ok));
            menu.sParam.DateTime.setHour(bcd2int(buf[3], ok));
            menu.sParam.DateTime.setMinute(bcd2int(buf[4], ok));
            menu.sParam.DateTime.setSecond(bcd2int(buf[5], ok));
            Q_ASSERT(buf[6] == 0);
            Q_ASSERT(buf[7] == 0);
            menu.sParam.DateTime.setMsSecond(buf[6] + ((quint16) buf[7] << 8));
            Q_ASSERT(buf[8] == 0);
        } break;

        default: {
            qDebug() << "Нет обработчика команды: " << com;
        }
    }
}

//
void MainWindow::clearSelection()
{
    QTextCursor c = ui->textEdit->textCursor();
    c.clearSelection();
    ui->textEdit->setTextCursor(c);
}

//
void MainWindow::cycleMenu()
{
    menu.proc();
    vLCDled();
    changeParam();
}

//
bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyRelease) {
        ui->kbd->keyPressed(static_cast<QKeyEvent*>(event)->key());
    }

    return QMainWindow::eventFilter(object, event);
}

//
void MainWindow::initParam()
{
    menu.sParam.password.init(0000);
    menu.sParam.Uart.Interface.set(TInterface::USB);
    menu.sParam.Uart.Protocol.set(TProtocol::STANDART);
    menu.sParam.Uart.BaudRate.set(TBaudRate::_19200);
    menu.sParam.Uart.DataBits.set(TDataBits::_8);
    menu.sParam.Uart.Parity.set(TParity::EVEN);
    menu.sParam.Uart.StopBits.set(TStopBits::ONE);

    menu.sParam.def.status.setEnable(false);

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

    menu.sParam.glb.setTypeOpto(TYPE_OPTO_STANDART);
    menu.sParam.glb.setTypeDevice(AVANT_OPTO);

    menu.sParam.device = false;

    menu.setConnectionBsp(true);

    menu.sParam.DateTime.setYear(20);
    menu.sParam.DateTime.setMonth(9);
    menu.sParam.DateTime.setDay(8);
    menu.sParam.DateTime.setHour(0);
    menu.sParam.DateTime.setMinute(8);
    menu.sParam.DateTime.setSecond(37);

    menu.sParam.glb.status.setRegime(GB_REGIME_ENABLED);
    menu.sParam.glb.status.setFault(0);
    menu.sParam.glb.status.setWarning(0);

    menu.sParam.prm.status.setRegime(GB_REGIME_ENABLED);
    menu.sParam.prm.status.setState(1);
    menu.sParam.prm.status.setFault(0);
    menu.sParam.prm.status.setWarning(0);

    menu.sParam.prd.status.setRegime(GB_REGIME_ENABLED);
    menu.sParam.prd.status.setState(1);
    menu.sParam.prd.status.setFault(0);
    menu.sParam.prd.status.setWarning(0);
}

//
void MainWindow::setBacklight(bool enable)
{
    QColor color = enable ? Qt::green : Qt::gray;

    if (color.isValid()) {
         QString qss = QString("background-color: %1").arg(color.name());
         ui->textEdit->setStyleSheet(qss);
    }
}

quint8 MainWindow::bcd2int(quint8 bcd, bool &ok) const
{
    quint8 value;

    ok = ((bcd & 0x0F) < 0x0A) && ((bcd & 0xF0) < 0xA0);
    if (!ok) {
        qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << " Error: " << bcd;
    }

    value = bcd & 0x0F;
    value += ((bcd >> 4) & 0x0F) * 10;

    return value;
}



