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
    initClock();

    installEventFilter(this);
    ui->textEdit->installEventFilter(this);

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    font.setPointSize(20);
    ui->textEdit->setFont(font);

    QFontMetrics mfont(font);
    QSize size = mfont.size(Qt::TextSingleLine, "12345678901234567890");

    // FIXME Разобраться почему константы именно такие
    ui->textEdit->setFixedSize(size.width() + 12, size.height()*7 + 10);
    ui->kbd->setFixedSize(ui->textEdit->width(), ui->textEdit->width());
    setFixedSize(sizeHint());
    // Удаляет движение содержимого при прокрутке колесика мышки над testEdit
    ui->textEdit->verticalScrollBar()->blockSignals(true);

    QTimer *timerMenu = new QTimer(this);
    connect(timerMenu, &QTimer::timeout, this, &MainWindow::cycleMenu);
    timerMenu->start(200);

    QTimer *timerCommand = new QTimer(this);
    connect(timerCommand, &QTimer::timeout, this, &MainWindow::procCommand);
    timerMenu->start(100);

    connect(ui->kbd, &QKeyboard::debug, this, &MainWindow::printDebug);

    QTimer *timerClock = new QTimer(this);
    connect(timerClock, &QTimer::timeout, this, &MainWindow::updateClock);
    timerClock->start(1000);
}

//
MainWindow::~MainWindow()
{
    delete ui;
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
    procCommand();
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
void MainWindow::initClock()
{
    QDate date;
    QTime time;

    date.setDate(params.DateTime.getYear()+2000,
                 params.DateTime.getMonth(),
                 params.DateTime.getDay());

    time.setHMS(params.DateTime.getHour(),
                params.DateTime.getMinute(),
                params.DateTime.getSecond());


    dt.setDate(date);
    dt.setTime(time);
}

//
void MainWindow::initParam()
{
    // Параметры хранящиеся во внутренней памяти МК

    menu.sParam.password.init(0000);
    menu.sParam.Uart.Interface.set(TInterface::USB);
    menu.sParam.Uart.Protocol.set(TProtocol::STANDART);
    menu.sParam.Uart.BaudRate.set(TBaudRate::_19200);
    menu.sParam.Uart.DataBits.set(TDataBits::_8);
    menu.sParam.Uart.Parity.set(TParity::EVEN);
    menu.sParam.Uart.StopBits.set(TStopBits::ONE);

    // Параметры получаемые из БСП

    params.glb.setNetAddress(15);

    params.DateTime.setYear(20);
    params.DateTime.setMonth(9);
    params.DateTime.setDay(9);
    params.DateTime.setHour(22);
    params.DateTime.setMinute(6);
    params.DateTime.setSecond(37);
    params.DateTime.setMsSecond(987);

    params.def.status.setEnable(false);
    params.prm.setNumCom(32);
    params.prd.setNumCom(32);
    params.glb.setNumDevices(GB_NUM_DEVICES_2);
    params.glb.setTypeLine(GB_TYPE_LINE_OPTO);
    params.glb.setVersProgIC16(0x1112, GB_IC_BSP_MCU);
    params.glb.setVersProgIC16(0x2122, GB_IC_BSP_DSP);
    params.glb.setVersProgIC16(0x3132, GB_IC_PI_MCU);
    params.glb.setVersProgIC16(0x4142, GB_IC_BSP_DSP_PLIS);
    params.glb.setVersProgIC8(0x51, GB_IC_BSK_PLIS_PRD1);
    params.glb.setVersProgIC8(0x61, GB_IC_BSK_PLIS_PRD2);
    params.glb.setVersProgIC8(0x71, GB_IC_BSK_PLIS_PRM1);
    params.glb.setVersProgIC8(0x81, GB_IC_BSK_PLIS_PRM2);
    params.glb.setVersProgIC8(0x91, GB_IC_BSZ_PLIS);
    params.glb.setCompatibility(GB_COMPATIBILITY_AVANT);
    params.glb.setCompK400(GB_COMP_K400_AVANT);
    params.glb.setTypeDevice(AVANT_OPTO);
    params.glb.setTypeOpto(TYPE_OPTO_STANDART);

    params.def.status.setRegime(GB_REGIME_ENABLED);
    params.def.status.setState(1);
    params.def.status.setDopByte(0);
    params.def.status.setFault(0);
    params.def.status.setWarning(0);
    params.def.status.setRemoteNumber(1);

    params.prm.status.setRegime(GB_REGIME_ENABLED);
    params.prm.status.setState(1);
    params.prm.status.setDopByte(0);
    params.prm.status.setFault(0);
    params.prm.status.setWarning(0);

    params.prd.status.setRegime(GB_REGIME_ENABLED);
    params.prd.status.setState(1);
    params.prd.status.setDopByte(0);
    params.prd.status.setFault(0);
    params.prd.status.setWarning(0);

    params.glb.status.setFault(0);
    params.glb.status.setWarning(0);
}

//
void MainWindow::printDebug(QString msg)
{
    ui->debug->append(msg);
}

//
void MainWindow::procCommand()
{
    eGB_COM com = menu.getTxCommand();

    switch(com & GB_COM_MASK_GROUP) {
        case GB_COM_MASK_GROUP_READ_PARAM: {
            procCommandReadParam(com);
        } break;
        case GB_COM_MASK_GROUP_WRITE_REGIME: {
            procCommandWriteRegime(com);
        } break;
        case GB_COM_MASK_GROUP_WRITE_PARAM: {
            procCommandWriteParam(com);
        } break;
        case GB_COM_MASK_GROUP_READ_JOURNAL: {
            procCommandReadJournal(com);
        }break;
    }

    menu.setConnectionBsp(true);
}

//
void MainWindow::procCommandReadJournal(eGB_COM com)
{
    switch(com) {
        default: {
            qDebug() << __FILE__ << __FUNCTION__ <<
                        "Нет обработчика команды: 0x" << hex << com;
        }
    }
}

//
void MainWindow::procCommandReadParam(eGB_COM com)
{
    uint16_t value = 0;

    switch(com) {
        case GB_COM_NO: {
        } break;
        case GB_COM_DEF_GET_LINE_TYPE: {
            uint8_t act = GB_ACT_NO;
            act = menu.sParam.glb.setNumDevices(params.glb.getNumDevices());
            menu.sParam.def.setNumDevices(params.glb.getNumDevices());
            menu.sParam.local.setNumDevices(menu.sParam.glb.getNumDevices()+1);
            if (act & GB_ACT_NEW) {
                menu.sParam.device = false;
            }
        } break;
        case GB_COM_GET_NET_ADR: {
            value = params.glb.getNetAddress();
            menu.sParam.glb.setNetAddress(value);
        } break;
        case GB_COM_GET_SOST: {
            menu.sParam.def.status.setRegime(params.def.status.getRegime());
            menu.sParam.def.status.setState(params.def.status.getState());
            menu.sParam.def.status.setDopByte(params.def.status.getDopByte());

            menu.sParam.prm.status.setRegime(params.prm.status.getRegime());
            menu.sParam.prm.status.setState(params.prm.status.getState());
            menu.sParam.prm.status.setDopByte(params.prm.status.getDopByte());

            // TODO Разобраться почему не используется второй приемник.

            menu.sParam.prd.status.setRegime(params.prd.status.getRegime());
            menu.sParam.prd.status.setState(params.prd.status.getState());
            menu.sParam.prd.status.setDopByte(params.prd.status.getDopByte());

            // NOTE В данном случае режимы всегда меняются одинаково. Логика не нужна.
            menu.sParam.glb.status.setRegime(params.prd.status.getRegime());

            // TODO Добавить состояние светодиодов и дискретных входов.
            // TODO Добавить управление подсветкой.
        } break;
        case GB_COM_GET_FAULT: {
            menu.sParam.def.status.setFault(params.def.status.getFault());
            menu.sParam.def.status.setWarning(params.def.status.getWarning());
            menu.sParam.def.status.setRemoteNumber(params.def.status.getRemoteNumber());

            menu.sParam.prm.status.setFault(params.prm.status.getFault());
            menu.sParam.prm.status.setWarning(params.prm.status.getWarning());

            menu.sParam.prd.status.setFault(params.prm.status.getFault());
            menu.sParam.prd.status.setWarning(params.prm.status.getWarning());

            menu.sParam.glb.status.setFault(params.glb.status.getFault());
            menu.sParam.glb.status.setWarning(params.glb.status.getWarning());
        } break;
        case GB_COM_GET_TIME: {
            menu.sParam.DateTime.setYear(params.DateTime.getYear());
            menu.sParam.DateTime.setMonth(params.DateTime.getMonth());
            menu.sParam.DateTime.setDay(params.DateTime.getDay());
            menu.sParam.DateTime.setHour(params.DateTime.getHour());
            menu.sParam.DateTime.setMinute(params.DateTime.getMinute());
            menu.sParam.DateTime.setSecond(params.DateTime.getSecond());
            menu.sParam.DateTime.setMsSecond(params.DateTime.getMsSecond());

            // TODO Добавить считывание записи журнала для АСУ ТП.
        } break;
        case GB_COM_GET_VERS: {
            menu.sParam.def.status.setEnable(false);

            menu.sParam.prm.setNumCom(params.prm.getNumCom());
            menu.sParam.local.setNumComPrm(params.prm.getNumCom());
            // FIXME Узнать что значит количество команд второго приемника.

            menu.sParam.prd.setNumCom(params.prd.getNumCom());
            menu.sParam.local.setNumComPrd(params.prd.getNumCom());

            menu.sParam.glb.setNumDevices(params.glb.getNumDevices());
            menu.sParam.def.setNumDevices(params.glb.getNumDevices());
            menu.sParam.local.setNumDevices(menu.sParam.glb.getNumDevices()+1);

            menu.sParam.glb.setTypeLine(params.glb.getTypeLine());

            menu.sParam.glb.setVersProgIC16(
                        params.glb.getVersProgIC(GB_IC_BSP_MCU),
                        GB_IC_BSP_MCU);
            menu.sParam.glb.setVersProgIC16(
                        params.glb.getVersProgIC(GB_IC_BSP_DSP),
                        GB_IC_BSP_DSP);
            menu.sParam.glb.setVersProgIC16(
                        params.glb.getVersProgIC(GB_IC_PI_MCU),
                        GB_IC_PI_MCU);
            menu.sParam.glb.setVersProgIC16(
                        params.glb.getVersProgIC(GB_IC_BSP_DSP_PLIS),
                        GB_IC_BSP_DSP_PLIS);

            // NOTE Версии ниже 8-и битные, но в данном случае записывать как 16
            menu.sParam.glb.setVersProgIC16(
                        params.glb.getVersProgIC(GB_IC_BSK_PLIS_PRD1),
                        GB_IC_BSK_PLIS_PRD1);
            menu.sParam.glb.setVersProgIC16(
                        params.glb.getVersProgIC(GB_IC_BSK_PLIS_PRD2),
                        GB_IC_BSK_PLIS_PRD2);
            menu.sParam.glb.setVersProgIC16(
                        params.glb.getVersProgIC(GB_IC_BSK_PLIS_PRM1),
                        GB_IC_BSK_PLIS_PRM1);
            menu.sParam.glb.setVersProgIC16(
                        params.glb.getVersProgIC(GB_IC_BSK_PLIS_PRM2),
                        GB_IC_BSK_PLIS_PRM2);
            menu.sParam.glb.setVersProgIC16(
                        params.glb.getVersProgIC(GB_IC_BSZ_PLIS),
                        GB_IC_BSZ_PLIS);

            menu.sParam.glb.setCompatibility(params.glb.getCompatibility());
            menu.sParam.glb.setCompK400(params.glb.getCompK400());

            menu.sParam.glb.setTypeDevice(params.glb.getTypeDevice());
            menu.sParam.glb.setTypeOpto(params.glb.getTypeOpto());

            menu.sParam.device = false;
        } break;
        default: {
            qDebug() << __FILE__ << __FUNCTION__ <<
                        "Нет обработчика команды: 0x" << hex << com;
        }
    }

    if ((com != GB_COM_NO) && (com == menu.sParam.local.getCom())) {
        // FIXME Проверить правильность установки всех параметров.
        menu.sParam.local.setValue(value);
    }
}

//
void MainWindow::procCommandWriteParam(eGB_COM com)
{
    uint8_t *buf = menu.sParam.txComBuf.getBuferAddress();

    switch(com) {
        case GB_COM_SET_TIME: {
            bool ok = false;
            params.DateTime.setYear(bcd2int(buf[0], ok));
            params.DateTime.setMonth(bcd2int(buf[1], ok));
            params.DateTime.setDay(bcd2int(buf[2], ok));
            params.DateTime.setHour(bcd2int(buf[3], ok));
            params.DateTime.setMinute(bcd2int(buf[4], ok));
            params.DateTime.setSecond(bcd2int(buf[5], ok));
            Q_ASSERT(buf[6] == 0);
            Q_ASSERT(buf[7] == 0);
            params.DateTime.setMsSecond(buf[6] + ((quint16) buf[7] << 8));
            Q_ASSERT(buf[8] == 0);
            initClock();
        } break;

        case GB_COM_SET_NET_ADR: {
            params.glb.setNetAddress(buf[0]);
        } break;

        default: {
            qDebug() << __FILE__ << __FUNCTION__ <<
                        "Нет обработчика команды: 0x" << hex << com;
        }
    }
}

//
void MainWindow::procCommandWriteRegime(eGB_COM com)
{
    switch(com) {
        case GB_COM_SET_REG_DISABLED: {
            params.def.status.setRegime(GB_REGIME_DISABLED);
            params.prd.status.setRegime(GB_REGIME_DISABLED);
            params.prm.status.setRegime(GB_REGIME_DISABLED);
        } break;
        case GB_COM_SET_REG_ENABLED: {
            params.def.status.setRegime(GB_REGIME_ENABLED);
            params.prd.status.setRegime(GB_REGIME_ENABLED);
            params.prm.status.setRegime(GB_REGIME_ENABLED);
        } break;

        default: {
            qDebug() << __FILE__ << __FUNCTION__ <<
                        "Нет обработчика команды: 0x" << hex << com;
        }
    }
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

//
void MainWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}

//
void MainWindow::updateClock()
{
    dt = dt.addSecs(1);

    params.DateTime.setYear(dt.date().year()-2000);
    params.DateTime.setMonth(dt.date().month());
    params.DateTime.setDay(dt.date().day());

    params.DateTime.setHour(dt.time().hour());
    params.DateTime.setMinute(dt.time().minute());
    params.DateTime.setSecond(dt.time().second());
}

