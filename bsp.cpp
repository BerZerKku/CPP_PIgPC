#include "bsp.h"
#include "PIg/src/flash.h"
#include "QTextCodec"
#include <QTimer>
#include <QHeaderView>

Bsp::Bsp(QWidget *parent) : QTreeWidget(parent)
{
    codec = QTextCodec::codecForName("CP1251");
    lp.setFlashParams(fParams);

    // Эти строки не влияют на содержимое заголовка, но влияют на resize ниже.
    headerItem()->setText(0, codec->toUnicode("Parameter"));
    headerItem()->setText(1, codec->toUnicode("Value"));

    crtTree();
    initClock();

    QTimer *timerClock = new QTimer(this);
    connect(timerClock, &QTimer::timeout, this, &Bsp::updateClock);
    timerClock->start(1000);

//    expandAll();
    header()->resizeSections(QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(0, QHeaderView::Fixed);
    header()->resizeSection(0, header()->sectionSize(0) + 5);
    setFixedWidth(1.75*header()->sectionSize(0));

    setSelectionMode(QAbstractItemView::NoSelection);
    setFocusPolicy(Qt::NoFocus);
}


//
quint8 Bsp::bcd2int(quint8 bcd, bool &ok) const
{
    quint8 value = 0;

    ok = ((bcd & 0x0F) < 0x0A) && ((bcd & 0xF0) < 0xA0);
    if (!ok) {
        qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << " Error: " << bcd;
    }

    value = bcd & 0x0F;
    value += ((bcd >> 4) & 0x0F) * 10;

    return value;
}

//
uint8_t Bsp::calcCrc(QVector<uint8_t> &pkg)
{
    uint8_t crc = 0;

    for(uint8_t byte: pkg) {
        crc += byte;
    }

    return crc;
}

//
eGB_COM Bsp::checkPkg(QVector<uint8_t> &pkg)
{
    eGB_COM com = GB_COM_NO;

    uint8_t byte;

    byte = pkg.takeFirst();
    if (byte != 0x55) {
        return GB_COM_NO;
    }

    byte = pkg.takeFirst();
    if (byte != 0xAA) {
        return GB_COM_NO;
    }

    byte = pkg.takeLast();
    if (byte != calcCrc(pkg)) {
        return GB_COM_NO;
    }

    com = static_cast<eGB_COM> (pkg.takeFirst());

    byte = pkg.takeFirst();
    if (byte != pkg.size()) {
        return GB_COM_NO;
    }

    return com;
}

//
void Bsp::initClock()
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
void Bsp::initParam()
{
    setLineEditValue(eGB_PARAM::GB_PARAM_IS_PWD_ENGINEER, "87654321");
    setLineEditValue(eGB_PARAM::GB_PARAM_IS_PWD_ADMIN, "12345678");
    setComboBoxValue(eGB_PARAM::GB_PARAM_IS_USER, TUser::USER::OPERATOR);

    setComboBoxValue(&stateGlb.regime, eGB_REGIME::GB_REGIME_ENABLED);
    setSpinBoxValue(&stateGlb.state, 1);
    setSpinBoxValue(&stateGlb.dopByte, 1);
    setLineEditValue(&stateGlb.fault, "0000");
    setLineEditValue(&stateGlb.warning, "0000");
    setLineEditValue(&stateDef.fault, "0000");
    setLineEditValue(&stateDef.warning, "0000");
    setLineEditValue(&statePrm.fault, "0000");
    setLineEditValue(&statePrm.warning, "0000");
    setLineEditValue(&statePrd.fault, "0000");
    setLineEditValue(&statePrd.warning, "0000");

    params.Uart.Interface.set(TInterface::USB);
    params.Uart.Protocol.set(TProtocol::STANDART);
    params.Uart.BaudRate.set(TBaudRate::_19200);
    params.Uart.DataBits.set(TDataBits::_8);
    params.Uart.Parity.set(TParity::EVEN);
    params.Uart.StopBits.set(TStopBits::ONE);

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
    params.glb.setDeviceNum(2);
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

    // TypeDevice + TypeOpto = TypeDevice(AVANT_OPTO)
    params.glb.setTypeDevice(AVANT_K400);
    params.glb.setTypeOpto(TYPE_OPTO_STANDART);

    params.def.status.setEnable(false);
    params.def.status.setFault(0);
    params.def.status.setWarning(0);
    params.def.status.setRemoteNumber(1);

    params.prm.status.setFault(0);
    params.prm.status.setWarning(0);

    params.prd.status.setFault(0);
    params.prd.status.setWarning(0);

    params.glb.status.setFault(0);
    params.glb.status.setWarning(0);

    params.glb.setDInputState(0);
    params.prm.setIndCom8(0, 0);
    params.prm.setIndCom8(1, 0);
    params.prm.setIndCom8(2, 0);
    params.prm.setIndCom8(3, 0);
    params.prd.setIndCom8(0, 0);
    params.prd.setIndCom8(1, 0);
    params.prd.setIndCom8(2, 0);
    params.prd.setIndCom8(3, 0);
}

void Bsp::setUser(int value)
{
    setComboBoxValue(eGB_PARAM::GB_PARAM_IS_USER, value);
}

//
quint8 Bsp::int2bcd(quint8 val, bool &ok) const
{
    quint8 bcd = 0;

    ok = (val < 100);
    if (!ok) {
        qWarning("Can't convert value 0x%.2X to BCD code!", val);
    }

    bcd = (val % 10);
    bcd += (val / 10) << 4;

    return bcd;
}

void Bsp::crtTree()
{
    crtTreeState();
    crtTreeUser();
}

//
void Bsp::crtTreeState()
{
    QTreeWidgetItem* top = new QTreeWidgetItem();

    insertTopLevelItem(topLevelItemCount(), top);
    top->setText(0, codec->toUnicode("Текущее состояние"));

    errRegExp.setPattern("[0-9A-Fa-F]+");
    errValidator.setRegExp(errRegExp);

    crtTreeState(top, "Общее", stateGlb);
    crtTreeState(top, "Защита", stateDef);
    crtTreeState(top, "Приемник", statePrm);
    crtTreeState(top, "Передатчик", statePrd);

    connect(&stateGlb.regime, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Bsp::setRegime);

    connect(&stateGlb.state, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &Bsp::setState);

    connect(&stateGlb.dopByte, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &Bsp::setDopByte);

    top->setExpanded(true);
}

//
void Bsp::crtTreeState(QTreeWidgetItem *top, std::string name, Bsp::state_t &state)
{
    QTreeWidgetItem *item = nullptr;
    QTreeWidgetItem *ltop= new QTreeWidgetItem();

    ltop->setText(0, codec->toUnicode(name.c_str()));
    top->addChild(ltop);

    item = new QTreeWidgetItem();
    item->setText(0, codec->toUnicode("Режим"));
    ltop->addChild(item);
    fillComboboxListRegime(&state.regime);
    setItemWidget(item, 1, &state.regime);

    item = new QTreeWidgetItem();
    item->setText(0, codec->toUnicode("Состояние"));
    ltop->addChild(item);
    state.state.setRange(0, MAX_NUM_DEVICE_STATE);
    state.state.setValue(0);
    setItemWidget(item, 1, &state.state);

    item = new QTreeWidgetItem();
    item->setText(0, codec->toUnicode("Доп. байт"));
    ltop->addChild(item);
    state.dopByte.setRange(0, 255);
    state.dopByte.setValue(0);
    setItemWidget(item, 1, &state.dopByte);

    item = new QTreeWidgetItem();
    item->setText(0, codec->toUnicode("Неисправности"));
    ltop->addChild(item);
    state.fault.setMaxLength(4);
    state.fault.setInputMask("HHHH");
    state.fault.setValidator(&errValidator);
    state.fault.setText("0000");
    setItemWidget(item, 1, &state.fault);

    connect(&state.fault, &QLineEdit::selectionChanged,
            &state.fault, &QLineEdit::deselect);

    item = new QTreeWidgetItem();
    item->setText(0, codec->toUnicode("Предупреждения"));
    ltop->addChild(item);
    state.warning.setMaxLength(4);
    state.warning.setInputMask("HHHH");
    state.warning.setValidator(&errValidator);
    state.warning.setText("0000");
    setItemWidget(item, 1, &state.warning);

    connect(&state.warning, &QLineEdit::selectionChanged,
            &state.warning, &QLineEdit::deselect);

    ltop->setExpanded(&state == &stateGlb);
//    ltop->setExpanded(true);
}

//
void Bsp::crtTreeUser()
{
    QTreeWidgetItem* top = new QTreeWidgetItem();
    insertTopLevelItem(topLevelItemCount(), top);

    top->setText(0, codec->toUnicode("Пользователь"));

    pwdRegExp.setPattern("[0-9]+");
    pwdValidator.setRegExp(pwdRegExp);

    crtComboBox(GB_PARAM_IS_USER, TUser::USER::OPERATOR);
    crtLineEdit(GB_PARAM_IS_PWD_ENGINEER, "87657321");
    crtLineEdit(GB_PARAM_IS_PWD_ADMIN, "12345678");

    top->setExpanded(true);
}

void Bsp::crtComboBox(eGB_PARAM param, qint16 value)
{
    QComboBox *combobox = new QComboBox(this);
    QTreeWidgetItem *item = new QTreeWidgetItem();
    QTreeWidgetItem *top = topLevelItem(topLevelItemCount()-1);
    std::string name("Noname");

    switch(param) {
        case eGB_PARAM::GB_PARAM_IS_USER: {
            fillComboboxList(combobox, param);
        } break;

        default: {
            QString msg = QString("%1: Parameter '%2' not found.").
                          arg(__FUNCTION__).arg(getParamName(param));
            qWarning() << msg;
        }
    }

    top->addChild(item);
    item->setText(0, getParamName(param));
    setItemWidget(item, 1, combobox);

    mapCombobox.insert(param, combobox);
}

void Bsp::crtLineEdit(eGB_PARAM param, std::string value)
{
    QLineEdit *lineedit = new QLineEdit(this);
    QTreeWidgetItem *item = new QTreeWidgetItem();
    QTreeWidgetItem *top = topLevelItem(topLevelItemCount()-1);

    switch(param) {
        case eGB_PARAM::GB_PARAM_IS_PWD_ADMIN: {
            lineedit->setMaxLength(PWD_LEN);
            lineedit->setInputMask("99999999");
            lineedit->setValidator(&pwdValidator);

        } break;

        case eGB_PARAM::GB_PARAM_IS_PWD_ENGINEER: {
            lineedit->setMaxLength(PWD_LEN);
            lineedit->setInputMask("99999999");
            lineedit->setValidator(&pwdValidator);
        } break;

        default: {
            QString msg = QString("%1: Parameter '%2' not found.").
                          arg(__FUNCTION__).arg(getParamName(param));
            qWarning() << msg;
        }
    }

    top->addChild(item);
    item->setText(0, getParamName(param));
    setItemWidget(item, 1, lineedit);

//    connect(lineedit, &QLineEdit::selectionChanged,
//            lineedit, &QLineEdit::deselect);
    mapLineEdit.insert(param, lineedit);
}

void Bsp::crtSpinBox(eGB_PARAM param, qint16 value)
{
    QSpinBox *spinbox = new QSpinBox(this);
    QTreeWidgetItem *item = new QTreeWidgetItem();
    QTreeWidgetItem *top = topLevelItem(topLevelItemCount()-1);
    std::string name("Noname");

    switch(param) {

        default: {
            QString msg = QString("%1: Parameter '%2' not found.").
                          arg(__FUNCTION__).arg(getParamName(param));
            qWarning() << msg;
        }
    }
}

//
quint8 Bsp::getComboBoxValue(eGB_PARAM param) {
    quint8 value = 0;
    QComboBox *combobox = mapCombobox.value(param, nullptr);

    if (combobox != nullptr) {
        value = getComboBoxValue(combobox);
    } else {
        QString msg = QString("%1: Parameter '%2' not found.").
                      arg(__FUNCTION__).arg(getParamName(param));
        emit debug(msg);
    }

    return value;
}

//
quint8 Bsp::getComboBoxValue(QComboBox *combobox) {
    return static_cast<quint8> (combobox->currentData().toInt());
}

//
QString Bsp::getLineEditValue(eGB_PARAM param)
{
    QString value;
    QLineEdit *lineedit = mapLineEdit.value(param, nullptr);

    if (lineedit != nullptr) {
        value = lineedit->text();
    } else {
        QString msg = QString("%1: Parameter '%2' not found.").
                      arg(__FUNCTION__).arg(getParamName(param));
        emit debug(msg);
    }

    return value;
}

QString Bsp::getLineEditValue(QLineEdit *lineedit)
{
    return lineedit->text();
}

qint16 Bsp::getSpinBoxValue(eGB_PARAM param)
{
    quint16 value = 0;
    QSpinBox *spinbox = mapSpinBox.value(param, nullptr);

    if (spinbox != nullptr) {
        value = getSpinBoxValue(spinbox);
    } else {
        QString msg = QString("%1: Parameter '%2' not found.").
                      arg(__FUNCTION__).arg(getParamName(param));
        emit debug(msg);
    }

    return value;
}

qint16 Bsp::getSpinBoxValue(QSpinBox *spinbox)
{
    return spinbox->value();
}

//
void Bsp::setComboBoxValue(eGB_PARAM param, quint8 value)
{
    QComboBox *combobox = mapCombobox.value(param, nullptr);

    if (combobox != nullptr) {
        if (setComboBoxValue(combobox, value) == -1) {
            QString msg = QString("%1: Wrong value %1 for parameter '%2'.").
                          arg(__FUNCTION__).arg(value).arg(getParamName(param));
            emit debug(msg);
        }
    } else {
        QString msg = QString("%1: Parameter '%2' not found.").
                      arg(__FUNCTION__).arg(getParamName(param));
        emit debug(msg);
    }
}

//
int Bsp::setComboBoxValue(QComboBox *combobox, quint8 value)
{
    int index = combobox->findData(value);

    if (index != -1) {
        combobox->setCurrentIndex(index);
    }

    return index;
}

void Bsp::setLineEditValue(eGB_PARAM param, std::string value)
{
    QLineEdit *lineedit = mapLineEdit.value(param, nullptr);

    if (lineedit != nullptr) {
        if (setLineEditValue(lineedit, value) == -1 ) {
            QString msg = QString("%1: Wrong value %1 for parameter '%2'.").
                          arg(__FUNCTION__).arg(value.c_str()).
                          arg(getParamName(param));
            emit debug(msg);
        }
    } else {
        QString msg = QString("%1: Parameter '%2' not found.").
                      arg(__FUNCTION__).arg(getParamName(param));
        emit debug(msg);
    }
}

int Bsp::setLineEditValue(QLineEdit *lineedit, std::string value)
{
    int index = -1;

    lineedit->setText(codec->toUnicode(value.c_str()));
    index = 0;
    // FIXME Добавить проверку значения!

    return index;
}

void Bsp::setSpinBoxValue(eGB_PARAM param, qint16 value)
{
    QSpinBox *spinbox = mapSpinBox.value(param, nullptr);

    if (spinbox != nullptr) {
        if (setSpinBoxValue(spinbox, value) == -1) {
            QString msg = QString("%1: Wrong value %1 for parameter '%2'.").
                          arg(__FUNCTION__).arg(value).arg(getParamName(param));
            emit debug(msg);
        }
    } else {
        QString msg = QString("%1: Parameter '%2' not found.").
                      arg(__FUNCTION__).arg(getParamName(param));
        emit debug(msg);
    }
}

int Bsp::setSpinBoxValue(QSpinBox *spinbox, qint16 value)
{
    int check = -1;

    if ((value >= spinbox->minimum()) && (value <= spinbox->maximum())) {
        spinbox->setValue(value);
    }

    return check;
}

QString Bsp::getParamName(eGB_PARAM param)
{
    QString name = "Noname";

    if (param < GB_PARAM_MAX) {
        lp.clearParams();
        lp.addParam(param);

        name = codec->toUnicode(lp.getNameOfParam());
    }

    return name;
}

void Bsp::fillComboboxList(QComboBox *combobox, eGB_PARAM param)
{
    if (param < GB_PARAM_MAX) {
        lp.clearParams();
        lp.addParam(param);

        if (lp.getParamType() == Param::PARAM_LIST) {
            char const *l = lp.getListOfValues();
            for(quint8 i = 0; i <= lp.getMax(); i++) {
                combobox->addItem(codec->toUnicode(l), lp.getMin() + i);
                l += STRING_LENGHT;
            }
        }

        if (lp.getCom() == eGB_COM::GB_COM_NO) {
            combobox->setEnabled(false);
        }
    }
}

//
void Bsp::fillComboboxListRegime(QComboBox *combobox)
{
    if (combobox != nullptr) {
        for(uint8_t i = 0; i < eGB_REGIME::GB_REGIME_MAX; i++) {
            combobox->addItem(codec->toUnicode(fcRegime[i]), i);
        }
    }
}

//
void Bsp::keyPressEvent(QKeyEvent *event)
{
    qDebug() << event;
}

//
void Bsp::procCommand(eGB_COM com, pkg_t &data)
{
    switch(com & GB_COM_MASK_GROUP) {
        case GB_COM_MASK_GROUP_READ_PARAM: {
            procCommandReadParam(com, data);
        } break;
        case GB_COM_MASK_GROUP_WRITE_REGIME: {
            procCommandWriteRegime(com, data);
        } break;
        case GB_COM_MASK_GROUP_WRITE_PARAM: {
            procCommandWriteParam(com, data);
        } break;
        case GB_COM_MASK_GROUP_READ_JOURNAL: {
            procCommandReadJournal(com, data);
        }break;
    }
}

//
void Bsp::procCommandReadJournal(eGB_COM com, pkg_t &data)
{
    switch(com) {
        case GB_COM_DEF_GET_JRN_CNT: {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }
            // TODO Обработчик для команды чтения количества записей журнала.
            pkgTx.append(com);
            pkgTx.append(0);
            pkgTx.append(0);
        } break;

        case GB_COM_PRM_GET_JRN_CNT: {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }
            // TODO Обработчик для команды чтения количества записей журнала.
            pkgTx.append(com);
            pkgTx.append(0);
            pkgTx.append(0);
        } break;

        case GB_COM_PRD_GET_JRN_CNT: {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }
            // TODO Обработчик для команды чтения количества записей журнала.
            pkgTx.append(com);
            pkgTx.append(0);
            pkgTx.append(0);
        } break;

        case GB_COM_GET_JRN_CNT : {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }
            // TODO Обработчик для команды чтения количества записей журнала.
            pkgTx.append(com);
            pkgTx.append(0);
            pkgTx.append(0);
        } break;

        default: {
            qWarning("No command handler: 0x%.2X", com);
        }
    }
}

//
void Bsp::procCommandReadParam(eGB_COM com, pkg_t &data)
{
    switch(com) {
        case GB_COM_NO: {
        } break;

        case GB_COM_DEF_GET_LINE_TYPE: {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }
            //
            pkgTx.append(com);
            pkgTx.append(params.glb.getNumDevices());
        } break;

        case GB_COM_GET_SOST: {
            if (data.size() != 1) {
                emit debug(msgSizeError.arg(data.size()));
            }
            params.measParam.setTemperature(data.first());

            pkgTx.append(com);
            pkgTx.append(getComboBoxValue(&stateDef.regime));
            pkgTx.append(getSpinBoxValue(&stateDef.state));
            pkgTx.append(getSpinBoxValue(&stateDef.dopByte));
            pkgTx.append(getComboBoxValue(&statePrm.regime));
            pkgTx.append(getSpinBoxValue(&statePrm.state));
            pkgTx.append(getSpinBoxValue(&statePrm.dopByte));
            pkgTx.append(getComboBoxValue(&statePrd.regime));
            pkgTx.append(getSpinBoxValue(&statePrd.state));
            pkgTx.append(getSpinBoxValue(&statePrd.dopByte));
            // TODO Разобраться зачем нужен второй приемник
            pkgTx.append(getComboBoxValue(&statePrm.regime));
            pkgTx.append(getSpinBoxValue(&statePrd.state));
            pkgTx.append(getSpinBoxValue(&statePrd.dopByte));
            pkgTx.append(params.prm.getIndCom8(0));
            pkgTx.append(params.prm.getIndCom8(1));
            pkgTx.append(params.prm.getIndCom8(2));
            pkgTx.append(params.prm.getIndCom8(3));
            pkgTx.append(params.prd.getIndCom8(0));
            pkgTx.append(params.prd.getIndCom8(1));
            pkgTx.append(params.prd.getIndCom8(2));
            pkgTx.append(params.prd.getIndCom8(3));

            uint8_t di = 0;
            di |= params.glb.getDInputPuskPrm() ? 0x01 : 0x00;
            di |= params.glb.getDInputResetInd() ? 0x02 : 0x00;
            di |= params.glb.getDInputTmControl() ? 0x04 : 0x00;
            pkgTx.append(di);
        } break;

        case GB_COM_GET_FAULT: {
            bool ok = false;
            uint16_t value = 0;
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }

            // FIXME Если на передатчик приходит одно любое предупреждение то выводится на экран "Предупреждение".

            pkgTx.append(com);
            value = getLineEditValue(&stateDef.fault).toUInt(&ok, 16);
            pkgTx.append(value >> 8);
            pkgTx.append(value);
            value = getLineEditValue(&stateDef.warning).toUInt(&ok, 16);
            pkgTx.append(value >> 8);
            pkgTx.append(value);
            value = getLineEditValue(&statePrm.fault).toUInt(&ok, 16);
            pkgTx.append(value >> 8);
            pkgTx.append(value);
            value = getLineEditValue(&statePrm.warning).toUInt(&ok, 16);
            pkgTx.append(value >> 8);
            pkgTx.append(value);
            value = getLineEditValue(&statePrd.fault).toUInt(&ok, 16);
            pkgTx.append(value >> 8);
            pkgTx.append(value);
            value = getLineEditValue(&statePrd.warning).toUInt(&ok, 16);
            pkgTx.append(value >> 8);
            pkgTx.append(value);
             value = getLineEditValue(&stateGlb.fault).toUInt(&ok, 16);
            pkgTx.append(value >> 8);
            pkgTx.append(value);
            value = getLineEditValue(&stateGlb.warning).toUInt(&ok, 16);
            pkgTx.append(value >> 8);
            pkgTx.append(value);

            // TODO Разобраться зачем нужен второй приемник
            value = getLineEditValue(&statePrm.fault).toUInt(&ok, 16);
            pkgTx.append(value >> 8);
            pkgTx.append(value);
            value = getLineEditValue(&statePrm.warning).toUInt(&ok, 16);
            pkgTx.append(value >> 8);
            pkgTx.append(value);

            // TODO Добавить байты данных для РЗСК
        } break;

        case GB_COM_GET_TIME: {
            // TODO Добавить обработку флага новой записи журнала
            if (data.size() != 1) {
                emit debug(msgSizeError.arg(data.size()));
            }

            bool ok;
            pkgTx.append(com);
            pkgTx.append(int2bcd(params.DateTime.getYear(), ok));
            pkgTx.append(int2bcd(params.DateTime.getMonth(), ok));
            pkgTx.append(int2bcd(params.DateTime.getDay(), ok));
            pkgTx.append(int2bcd(params.DateTime.getHour(), ok));
            pkgTx.append(int2bcd(params.DateTime.getMinute(), ok));
            pkgTx.append(int2bcd(params.DateTime.getSecond(), ok));

            uint16_t value = params.DateTime.getMsSecond();
            pkgTx.append(value);
            pkgTx.append(value >> 8);

            // TODO Добавить считывание записи журнала для АСУ ТП.
        } break;

        case GB_COM_GET_NET_ADR: {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }
            //
            pkgTx.append(com);
            pkgTx.append(params.glb.getNetAddress());
            pkgTx.append(params.Uart.Interface.get());
            pkgTx.append(params.Uart.Protocol.get());
            pkgTx.append(params.Uart.BaudRate.get());
            pkgTx.append(params.Uart.DataBits.get());
            pkgTx.append(params.Uart.Parity.get());
            pkgTx.append(params.Uart.StopBits.get());

            char const* val = getLineEditValue(GB_PARAM_IS_PWD_ENGINEER).
                           toStdString().c_str();
            for(uint8_t i = 0; i < PWD_LEN; i++) {
                pkgTx.append(static_cast <uint8_t> (*val));
                val++;
            }

            val = getLineEditValue(GB_PARAM_IS_PWD_ADMIN).
                                       toStdString().c_str();
            for(uint8_t i = 0; i < PWD_LEN; i++) {
                pkgTx.append(static_cast <uint8_t> (*val));
                val++;
            }
        } break;

        case GB_COM_GET_DEVICE_NUM : {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }
            //
            pkgTx.append(com);
            pkgTx.append(params.glb.getDeviceNum());
        } break;

        case GB_COM_GET_VERS: {
            uint16_t vers = 0;

            pkgTx.append(com);
            pkgTx.append(params.def.status.isEnable() ? 1 : 0);
            pkgTx.append(params.prm.getNumCom() / 4); // прм1
            pkgTx.append(params.prm.getNumCom() / 4); // прм2
            pkgTx.append(params.prd.getNumCom() / 4);
            pkgTx.append(params.glb.getNumDevices());
            pkgTx.append(params.glb.getTypeLine());

            vers = params.glb.getVersProgIC(GB_IC_BSP_MCU);
            pkgTx.append(vers >> 8);
            pkgTx.append(vers);
            vers = params.glb.getVersProgIC(GB_IC_BSP_DSP);
            pkgTx.append(vers >> 8);
            pkgTx.append(vers);

            pkgTx.append(params.glb.getCompatibility());

            vers = params.glb.getVersProgIC(GB_IC_BSK_PLIS_PRD1);
            pkgTx.append((vers >> 4) + vers);
            vers = params.glb.getVersProgIC(GB_IC_BSK_PLIS_PRD2);
            pkgTx.append((vers >> 4) + vers);
            vers = params.glb.getVersProgIC(GB_IC_BSK_PLIS_PRM1);
            pkgTx.append((vers >> 4) + vers);
            vers = params.glb.getVersProgIC(GB_IC_BSK_PLIS_PRM2);
            pkgTx.append((vers >> 4) + vers);
            vers = params.glb.getVersProgIC(GB_IC_BSZ_PLIS);
            pkgTx.append((vers >> 4) + vers);

            pkgTx.append(params.glb.getTypeDevice());

            vers = params.glb.getVersProgIC(GB_IC_PI_MCU);
            pkgTx.append(vers >> 8);
            pkgTx.append(vers);

            pkgTx.append(params.glb.getTypeOpto());

            vers = params.glb.getVersProgIC(GB_IC_BSP_DSP);
            pkgTx.append(vers >> 8);
            pkgTx.append(vers);
        } break;

        default: {
            qWarning("No command handler: 0x%.2X", com);
        }
    }
}

//
void Bsp::procCommandWriteParam(eGB_COM com, pkg_t &data)
{
    switch(com) {
        case GB_COM_SET_TIME: {
            if (data.size() != 9) {
                emit debug(msgSizeError.arg(data.size()));
            } else {
                bool ok = false;
                params.DateTime.setYear(bcd2int(data.takeFirst(), ok));
                params.DateTime.setMonth(bcd2int(data.takeFirst(), ok));
                params.DateTime.setDay(bcd2int(data.takeFirst(), ok));
                params.DateTime.setHour(bcd2int(data.takeFirst(), ok));
                params.DateTime.setMinute(bcd2int(data.takeFirst(), ok));
                params.DateTime.setSecond(bcd2int(data.takeFirst(), ok));

                quint16 ms = data.takeFirst();
                ms += data.takeFirst() << 8;
                params.DateTime.setMsSecond(ms);

                if (data.first() != 0) {
                    emit debug(msgTimeSourceError.arg(data.first()));
                }
                data.removeFirst();
                initClock();
            }
        } break;

        case GB_COM_SET_NET_ADR: {
            uint8_t dop = data.takeFirst();
            switch(dop) {
                case 1: {
                    uint8_t byte = data.takeFirst();
                    params.glb.setNetAddress(byte);
                } break;
                case 2: {
                    uint8_t byte = data.takeFirst();
                    params.Uart.Interface.set((TInterface::INTERFACE) byte);
                } break;
                case 3: {
                    uint8_t byte = data.takeFirst();
                    params.Uart.Protocol.set((TProtocol::PROTOCOL) byte);
                } break;
                case 4: {
                    uint8_t byte = data.takeFirst();
                    params.Uart.BaudRate.set((TBaudRate::BAUD_RATE) byte);
                } break;
                case 5: {
                    uint8_t byte = data.takeFirst();
                    params.Uart.DataBits.set((TDataBits::DATA_BITS) byte);
                } break;
                case 6: {
                    uint8_t byte = data.takeFirst();
                    params.Uart.Parity.set((TParity::PARITY) byte);
                } break;
                case 7: {
                    uint8_t byte = data.takeFirst();
                    params.Uart.StopBits.set((TStopBits::STOP_BITS) byte);
                } break;
                case 8: {
                    if (data.size() != 8) {
                        emit debug(msgSizeError.arg(data.size()));
                    } else {
                        QString value;
                        for(uint8_t i = 0; i < PWD_LEN; i++) {
                            value.append(data.takeFirst());
                        }
                        mapLineEdit.value(GB_PARAM_IS_PWD_ADMIN)->setText(value);
                    }
                } break;
                case 9: {
                    if (data.size() != 8) {
                        emit debug(msgSizeError.arg(data.size()));
                    } else {
                        QString value;
                        for(uint8_t i = 0; i < PWD_LEN; i++) {
                            value.append(data.takeFirst());
                        }
                        mapLineEdit.value(GB_PARAM_IS_PWD_ENGINEER)->setText(value);
                    }
                } break;
                default: qDebug("No dop byte handler: 0x%.2X", dop);
            }
        } break;

        default: {
            qWarning("No command handler: 0x%.2X", com);
        }
    }
}

//
void Bsp::procCommandWriteRegime(eGB_COM com, pkg_t &data)
{
    switch(com) {
        case GB_COM_SET_REG_DISABLED: {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }

            setComboBoxValue(&stateGlb.regime, eGB_REGIME::GB_REGIME_DISABLED);
            pkgTx.append(com);
        } break;
        case GB_COM_SET_REG_ENABLED: {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }

            setComboBoxValue(&stateGlb.regime, eGB_REGIME::GB_REGIME_ENABLED);
            pkgTx.append(com);
        } break;

        default: {
            qWarning("No command handler: 0x%.2X", com);
        }
    }
}

//
pkg_t Bsp::receiveFromBsp()
{
    pkg_t pkg;

    if (!pkgTx.isEmpty()) {
        pkg = pkgTx;
        pkgTx.clear();

        pkg.insert(1, pkg.size() - 1);
        pkg.append(calcCrc(pkg));
        pkg.insert(0, 0xAA);
        pkg.insert(0, 0x55);

        //        qDebug() << "receiveFromBsp: " << showbase << hex << pkg;
    }

    return pkg;
}

//
void Bsp::sendToBsp(pkg_t pkg)
{
    eGB_COM com = checkPkg(pkg);

    pkgTx.clear();
    if (com != GB_COM_NO) {
        procCommand(com, pkg);

//        if (com == GB_COM_SET_NET_ADR) {
//            qDebug() << QString("sendToBsp: ") << showbase << hex << pkg;
//        }
    } else {
        qWarning() << "Message check error: " << showbase << hex << pkg;
    }
}

//
void Bsp::updateClock()
{
    dt = dt.addSecs(1);

    params.DateTime.setYear(dt.date().year()-2000);
    params.DateTime.setMonth(dt.date().month());
    params.DateTime.setDay(dt.date().day());

    params.DateTime.setHour(dt.time().hour());
    params.DateTime.setMinute(dt.time().minute());
    params.DateTime.setSecond(dt.time().second());
}

void Bsp::setRegime(int index)
{
    stateDef.regime.setCurrentIndex(index);
    statePrm.regime.setCurrentIndex(index);
    statePrd.regime.setCurrentIndex(index);
}

void Bsp::setState(int index)
{
    stateDef.state.setValue(index);
    statePrm.state.setValue(index);
    statePrd.state.setValue(index);
}

void Bsp::setDopByte(int index)
{
    stateDef.dopByte.setValue(index);
    statePrm.dopByte.setValue(index);
    statePrd.dopByte.setValue(index);
}
