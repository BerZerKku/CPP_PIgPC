#include "bsp.h"
#include "PIg/src/flash.h"
#include "QTextCodec"
#include <QTimer>
#include <QHeaderView>

Bsp::Bsp(QWidget *parent) : QTreeWidget(parent) {
    codec = QTextCodec::codecForName("CP1251");

    COMPILE_TIME_ASSERT(MAX_NUM_COM_SEND_IN_CYLCE == (MAX_NUM_COM_BUF2 + 3));

    // Эти строки не влияют на содержимое заголовка, но влияют на resize ниже.
    headerItem()->setText(0, codec->toUnicode("Parameter"));
    headerItem()->setText(1, codec->toUnicode("Value"));

    crtTree();

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

    initDebug();
}


//
quint8 Bsp::bcd2int(quint8 bcd, bool &ok) const {
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
uint8_t Bsp::calcCrc(QVector<uint8_t> &pkg) {
    uint8_t crc = 0;

    for(uint8_t byte: pkg) {
        crc += byte;
    }

    return crc;
}

//
eGB_COM Bsp::checkPkg(QVector<uint8_t> &pkg) {
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
void Bsp::initClock() {
    dt = QDateTime::currentDateTime();
}

void Bsp::initDebug() {
    viewCom.append(GB_COM_SET_TIME);
}

//
void Bsp::initParam() {
    eGB_PARAM param = GB_PARAM_NULL_PARAM;

    setLineEditValue(eGB_PARAM::GB_PARAM_IS_PWD_ENGINEER, "00000001");
    setLineEditValue(eGB_PARAM::GB_PARAM_IS_PWD_ADMIN, "00000002");
    setComboBoxValue(eGB_PARAM::GB_PARAM_IS_USER, TUser::OPERATOR);
    setSpinBoxValue(eGB_PARAM::GB_PARAM_IS_PWD_ENG_CNT, 2);
    setSpinBoxValue(eGB_PARAM::GB_PARAM_IS_PWD_ADM_CNT, 3);

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

    setComboBoxValue(GB_PARAM_COM_PRD_KEEP, 0);
    setComboBoxValue(GB_PARAM_COM_PRM_KEEP, 0);

    setComboBoxValue(GB_PARAM_INTF_INTERFACE, TInterface::RS485);
    setComboBoxValue(GB_PARAM_INTF_PROTOCOL, TProtocol::IEC_101);
    setSpinBoxValue(GB_PARAM_NET_ADDRESS, 17);
    setComboBoxValue(GB_PARAM_INTF_BAUDRATE, TBaudRate::_9600);
    setComboBoxValue(GB_PARAM_INTF_DATA_BITS, TDataBits::_8);
    setComboBoxValue(GB_PARAM_INTF_PARITY, TParity::EVEN);
    setComboBoxValue(GB_PARAM_INTF_STOP_BITS, TStopBits::ONE);

    setSpinBoxValue(GB_PARAM_PRM_TIME_ON, 5);
    setComboBoxValueBits(GB_PARAM_PRM_COM_BLOCK, 0x01, 1);
    setComboBoxValueBits(GB_PARAM_PRM_COM_BLOCK, 0x02, 2);
    setComboBoxValueBits(GB_PARAM_PRM_COM_BLOCK, 0x03, 3);
    setComboBoxValueBits(GB_PARAM_PRM_COM_BLOCK, 0x04, 4);

    param = GB_PARAM_PRM_TIME_OFF;
    for(uint8_t i = 1; i <= getAbsMaxNumOfSameParams(param); i++) {
        qint16 value = getMin(param) + i*getDisc(param);
        value = value % getAbsMax(param);
        setSpinBoxValue(GB_PARAM_PRM_TIME_OFF, value / getFract(param), i);
    }

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

    params.glb.setDInputState(0);
    params.prm.setIndCom8(0, 0);
    params.prm.setIndCom8(1, 0);
    params.prm.setIndCom8(2, 0);
    params.prm.setIndCom8(3, 0);
    params.prd.setIndCom8(0, 0);
    params.prd.setIndCom8(1, 0);
    params.prd.setIndCom8(2, 0);
    params.prd.setIndCom8(3, 0);

    initClock();
}

void Bsp::setUser(int value) {
    setComboBoxValue(eGB_PARAM::GB_PARAM_IS_USER, value);
}

//
quint8 Bsp::int2bcd(quint8 val, bool &ok) const {
    quint8 bcd = 0;

    ok = (val < 100);
    if (!ok) {
        qWarning("Can't convert value 0x%.2X to BCD code!", val);
    }

    bcd = (val % 10);
    bcd += (val / 10) << 4;

    return bcd;
}

void Bsp::crtTree() {
    crtTreeUser();
    crtTreeState();
    crtTreePrm();
    crtTreePrd();
    crtTreeGlb();
    crtTreeInterface();
}

void Bsp::crtTreeGlb() {
    QTreeWidgetItem* top = new QTreeWidgetItem();
    insertTopLevelItem(topLevelItemCount(), top);

    top->setText(0, codec->toUnicode("Общие"));

    crtComboBox(GB_PARAM_COM_PRD_KEEP);
    crtComboBox(GB_PARAM_COM_PRM_KEEP);

    top->setExpanded(false);
}

void Bsp::crtTreeInterface() {
    QTreeWidgetItem* top = new QTreeWidgetItem();
    insertTopLevelItem(topLevelItemCount(), top);

    top->setText(0, codec->toUnicode("Интерфейс"));

    crtComboBox(GB_PARAM_INTF_INTERFACE);
    crtComboBox(GB_PARAM_INTF_PROTOCOL);
    crtSpinBox(GB_PARAM_NET_ADDRESS);
    crtComboBox(GB_PARAM_INTF_BAUDRATE);
    crtComboBox(GB_PARAM_INTF_DATA_BITS);
    crtComboBox(GB_PARAM_INTF_PARITY);
    crtComboBox(GB_PARAM_INTF_STOP_BITS);


    top->setExpanded(false);
}

void Bsp::crtTreePrd() {
    QTreeWidgetItem* top = new QTreeWidgetItem();
    insertTopLevelItem(topLevelItemCount(), top);

    top->setText(0, codec->toUnicode("Передатчик"));

    top->setExpanded(false);
}

void Bsp::crtTreePrm() {
    QTreeWidgetItem* top = new QTreeWidgetItem();
    insertTopLevelItem(topLevelItemCount(), top);

    top->setText(0, codec->toUnicode("Приемник"));

    // FIXME Есть два вида задержки на фиксацию команды!
    crtSpinBox(GB_PARAM_PRM_TIME_ON);
    crtComboBox(GB_PARAM_PRM_COM_BLOCK);
    crtSpinBox(GB_PARAM_PRM_TIME_OFF);
//    crtComboBox(GB_PARAM_PRD_DR_ENABLE);
//    crtComboBox(GB_PARAM_PRM_DR_COM_BLOCK);
//    crtComboBox(GB_PARAM_PRM_DR_COM_TO_HF);
//    crtComboBox(GB_PARAM_PRM_COM_SIGNAL);

    top->setExpanded(true);
}

//
void Bsp::crtTreeState() {
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
void Bsp::crtTreeState(QTreeWidgetItem *top, std::string name,
                       Bsp::state_t &state) {

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
void Bsp::crtTreeUser() {
    QTreeWidgetItem* top = new QTreeWidgetItem();
    insertTopLevelItem(topLevelItemCount(), top);

    top->setText(0, codec->toUnicode("Пользователь"));

    pwdRegExp.setPattern("[0-9]+");
    pwdValidator.setRegExp(pwdRegExp);

    crtComboBox(GB_PARAM_IS_USER);
    crtLineEdit(GB_PARAM_IS_PWD_ENGINEER, "00000000");
    crtSpinBox(GB_PARAM_IS_PWD_ENG_CNT);
    crtLineEdit(GB_PARAM_IS_PWD_ADMIN, "00000000");
    crtSpinBox(GB_PARAM_IS_PWD_ADM_CNT);

    mapCombobox.value(GB_PARAM_IS_USER).at(0)->setEnabled(true);
    connect(mapCombobox.value(GB_PARAM_IS_USER).at(0),
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Bsp::setLocalValues);

    top->setExpanded(true);
}

//
void Bsp::crtComboBox(eGB_PARAM param) {
    QVector<QComboBox *> vcombobox;
    QComboBox *combobox;
    QTreeWidgetItem *item;
    QTreeWidgetItem *top = topLevelItem(topLevelItemCount()-1);

    uint8_t num = getAbsMaxNumOfSameParams(param);
    if (num == 0) {
        qCritical() << QString("Number of parameters %1 is 0!").
                       arg(getParamName(param));
    } else {
        if (num > 1) {
            item = new QTreeWidgetItem();
            item->setText(0, getParamName(param));
            top->addChild(item);
            top = item;
        }

        for(uint8_t i = 1 ; i <= num; i++) {
            item = new QTreeWidgetItem();

            if (num > 1) {
                std::string name("Номер ");
                name += std::to_string(i);
                item->setText(0, codec->toUnicode(name.c_str()));
            } else {
                item->setText(0, getParamName(param));
            }

            combobox = new QComboBox(this);
            vcombobox.append(combobox);

            if (getListOfValues(param) != nullptr) {
                fillComboboxList(combobox, param);
                combobox->setCurrentIndex(0);
            } else {
                qCritical() << QString("Parameter %1 is not LIST!").
                               arg(getParamName(param));
            }

            if (getCom(param) == eGB_COM::GB_COM_NO) {
                QPalette pa;
                pa.setColor(QPalette::WindowText,Qt::red);
                item->setForeground(0, Qt::blue);
            }

            top->addChild(item);
            setItemWidget(item, 1, combobox);
        }

        mapCombobox.insert(param, vcombobox);
    }
}

//
void Bsp::fillComboboxList(QComboBox *combobox, eGB_PARAM param) {
    if (param < GB_PARAM_MAX) {
        if (getListOfValues(param) != nullptr) {
            char const *l = getListOfValues(param);
            for(quint8 i = 0; i < getAbsMax(param); i++) {
                combobox->addItem(codec->toUnicode(l), getMin(param) + i);
                l += STRING_LENGHT;
            }
        }
    }
}

//
void Bsp::fillComboboxListRegime(QComboBox *combobox) {
    if (combobox != nullptr) {
        for(uint8_t i = 0; i < eGB_REGIME::GB_REGIME_MAX; i++) {
            combobox->addItem(codec->toUnicode(fcRegime[i]), i);
        }
    }
}

//
quint8 Bsp::getComboBoxValue(eGB_PARAM param, uint8_t number) {
    quint8 value = 0;

    if (number > 0 && mapCombobox.contains(param) &&
        mapCombobox.value(param).size() >= number) {

        number -= 1;
        if (getParamType(param) == Param::PARAM_LIST) {
            value = getComboBoxValue(mapCombobox.value(param).at(number));
        } else if (getParamType(param) == Param::PARAM_BITES) {
            Q_ASSERT((number + CHAR_BIT) <= mapCombobox.value(param).size());

            for(uint8_t i = 0; i < CHAR_BIT; i++) {
                QComboBox *box = mapCombobox.value(param).at(number + i);
                value += box->currentData() > 0 ? (1 << i) : 0;
            }
        }
    } else {
        QString msg = QString("%1: Parameter '%2' (%3) not found.").
                      arg(__FUNCTION__).arg(getParamName(param)).arg(number);
        emit debug(msg);
        qWarning() << msg;
    }

    return value;
}

//
quint8 Bsp::getComboBoxValue(QComboBox *combobox) {
    return static_cast<quint8> (combobox->currentData().toInt());
}

//
void Bsp::setComboBoxValue(eGB_PARAM param, quint8 value, uint8_t number) {
    QComboBox *combobox = nullptr;

    if ((number > 0) && mapCombobox.contains(param) &&
        (mapCombobox.value(param).size() >= number)) {

        combobox = mapCombobox.value(param).at(number-1);
        if (setComboBoxValue(combobox, value) == -1) {
            QString msg = QString("%1: Wrong value %2 for parameter '%3' (%4).").
                          arg(__FUNCTION__).arg(value).
                          arg(getParamName(param)).arg(number);
            emit debug(msg);
            qWarning() << msg;
        }
    } else {
        QString msg = QString("%1: Parameter '%2' (%3) not found.").
                      arg(__FUNCTION__).arg(getParamName(param)).arg(number);
        emit debug(msg);
        qWarning() << msg;
    }
}

void Bsp::setComboBoxValueBits(eGB_PARAM param, quint8 value, uint8_t number) {
   if ((number > 0) && (number*8 <= getAbsMaxNumOfSameParams(param))) {
       uint8_t start = (number - 1) * CHAR_BIT;
       for(uint8_t i = 0; i < CHAR_BIT; i++) {
           uint8_t v = (value & (1 << i)) ? 1 : 0;
           setComboBoxValue(param, v, start + i + 1);
       }
   } else {
       QString msg = QString("%1: Wrong number for Parameter '%2' (%3).").
                     arg(__FUNCTION__).arg(getParamName(param)).arg(number);
       emit debug(msg);
       qWarning() << msg;
   }
}

//
int Bsp::setComboBoxValue(QComboBox *combobox, quint8 value) {
    int index = combobox->findData(value);

    if (index != -1) {
        combobox->setCurrentIndex(index);
    }

    return index;
}

//
void Bsp::crtLineEdit(eGB_PARAM param, std::string value) {
    QLineEdit *lineedit = new QLineEdit(this);
    QTreeWidgetItem *item = new QTreeWidgetItem();
    QTreeWidgetItem *top = topLevelItem(topLevelItemCount()-1);

    if (getParamType(param) == Param::PARAM_PWD) {
        lineedit->setMaxLength(PWD_LEN);
        lineedit->setInputMask("99999999");
        lineedit->setValidator(&pwdValidator);;
    } else {
        qCritical() << QString("Parameter %1 is not PWD!").
                      arg(getParamName(param));
    }

    if (getCom(param) == eGB_COM::GB_COM_NO) {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        item->setForeground(0, Qt::blue);
    }

    top->addChild(item);
    item->setText(0, getParamName(param));
    setItemWidget(item, 1, lineedit);

    mapLineEdit.insert(param, lineedit);
}

//
QString Bsp::getLineEditValue(eGB_PARAM param) {
    QString value;
    QLineEdit *lineedit = mapLineEdit.value(param, nullptr);

    if (lineedit != nullptr) {
        value = lineedit->text();
    } else {
        QString msg = QString("%1: Parameter '%2' not found.").
                      arg(__FUNCTION__).arg(getParamName(param));
        emit debug(msg);
        qWarning() << msg;
    }

    return value;
}

//
QString Bsp::getLineEditValue(QLineEdit *lineedit) {
    return lineedit->text();
}

//
void Bsp::setLineEditValue(eGB_PARAM param, std::string value) {
    QLineEdit *lineedit = mapLineEdit.value(param, nullptr);

    if (lineedit != nullptr) {
        if (setLineEditValue(lineedit, value) == -1 ) {
            QString msg = QString("%1: Wrong value %2 for parameter '%3'.").
                          arg(__FUNCTION__).arg(value.c_str()).
                          arg(getParamName(param));
            emit debug(msg);
            qWarning() << msg;
        }
    } else {
        QString msg = QString("%1: Parameter '%2' not found.").
                      arg(__FUNCTION__).arg(getParamName(param));
        emit debug(msg);
        qWarning() << msg;
    }
}

//
int Bsp::setLineEditValue(QLineEdit *lineedit, std::string value) {
    int index = -1;

    lineedit->setText(codec->toUnicode(value.c_str()));
    index = 0;
    // FIXME Добавить проверку значения!

    return index;
}

//
void Bsp::crtSpinBox(eGB_PARAM param) {
    QVector<QSpinBox *> vspinbox;
    QSpinBox *spinbox;
    QTreeWidgetItem *item;
    QTreeWidgetItem *top = topLevelItem(topLevelItemCount()-1);

    uint8_t num = getAbsMaxNumOfSameParams(param);
    if (num == 0) {
        qCritical() << QString("Number of parameters %1 is 0!").
                       arg(getParamName(param));
    } else {
        if (num > 1) {
            item = new QTreeWidgetItem();
            item->setText(0, getParamName(param));
            top->addChild(item);
            top = item;
        }

        for(uint8_t i = 1 ; i <= num; i++) {
            item = new QTreeWidgetItem();

            if (num > 1) {
                std::string name("Номер ");
                name += std::to_string(i);
                item->setText(0, codec->toUnicode(name.c_str()));
            } else {
                item->setText(0, getParamName(param));
            }

            spinbox = new QSpinBox(this);
            vspinbox.append(spinbox);

            if (getParamType(param) == Param::PARAM_INT) {
                qint16 min = getMin(param);
                qint16 max = getAbsMax(param);
                spinbox->setRange(min, max);
                spinbox->setSingleStep(getDisc(param));
                spinbox->setValue(getMin(param));
                spinbox->setToolTip(QString("%1 - %2").arg(min).arg(max));
            } else {
                qCritical() << QString("Parameter %1 is not INT!").
                               arg(getParamName(param));
            }

            if (getCom(param) == eGB_COM::GB_COM_NO) {
                QPalette pa;
                pa.setColor(QPalette::WindowText,Qt::red);
                item->setForeground(0, Qt::blue);
            }

            top->addChild(item);
            setItemWidget(item, 1, spinbox);
        }

        mapSpinBox.insert(param, vspinbox);
    }
}

//
qint16 Bsp::getSpinBoxValue(eGB_PARAM param, uint8_t number) {
    qint16 value = 0;

    if (number > 0 && mapSpinBox.contains(param) &&
        mapSpinBox.value(param).size() >= number) {

        number -= 1;
        if (getParamType(param) == Param::PARAM_INT) {
            value = getSpinBoxValue(mapSpinBox.value(param).at(number));
            value = (value / getDisc(param)) * getDisc(param);
            value /= getFract(param);
        }
    } else {
        QString msg = QString("%1: Parameter '%2' (%3) not found.").
                      arg(__FUNCTION__).arg(getParamName(param)).arg(number);
        emit debug(msg);
        qWarning() << msg;
    }

    return value;
}

//
qint16 Bsp::getSpinBoxValue(QSpinBox *spinbox) {
    return static_cast<qint16> (spinbox->value());
}

//
void Bsp::setSpinBoxValue(eGB_PARAM param, qint16 value, uint8_t number) {
    QSpinBox *spinbox = nullptr;

    if ((number > 0) && mapSpinBox.contains(param) &&
        (mapSpinBox.value(param).size() >= number)) {

        value = value * getFract(param);
        spinbox = mapSpinBox.value(param).at(number-1);
        if (setSpinBoxValue(spinbox, value) == -1) {
            QString msg = QString("%1: Wrong value %2 for parameter '%3'.").
                          arg(__FUNCTION__).arg(value).arg(getParamName(param));
            emit debug(msg);
            qWarning() << msg;
        }
    } else {
        QString msg = QString("%1: Parameter '%2' not found.").
                      arg(__FUNCTION__).arg(getParamName(param));
        emit debug(msg);
        qWarning() << msg;
    }
}

//
int Bsp::setSpinBoxValue(QSpinBox *spinbox, qint16 value) {
    int check = -1;

    if ((value >= spinbox->minimum()) && (value <= spinbox->maximum())) {
        spinbox->setValue(value);
        check = 0;
    }

    return check;
}

QString Bsp::getParamName(eGB_PARAM param) {
    QString name = "Noname";

    if (param < GB_PARAM_MAX) {
        name = codec->toUnicode(getNameOfParam(param));
    }

    return name;
}

//
void Bsp::keyPressEvent(QKeyEvent *event) {
    return;
}

//
void Bsp::procCommand(eGB_COM com, pkg_t &data) {
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
void Bsp::procCommandReadJournal(eGB_COM com, pkg_t &data) {
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
void Bsp::procCommandReadParam(eGB_COM com, pkg_t &data) {
    switch(com) {
        case GB_COM_NO: {
        } break;

        case GB_COM_DEF_GET_LINE_TYPE: {
            if (data.size() != 0) {
                emit debug(msgSizeError.arg(com, 2, 16).arg(data.size()));
            }
            //
            pkgTx.append(com);
            pkgTx.append(params.glb.getNumDevices());
        } break;

        case GB_COM_PRM_GET_TIME_ON: {
            if (data.size() != 0) {
                emit debug(msgSizeError.arg(com, 2, 16).arg(data.size()));
            }
            //
            pkgTx.append(com);
            // FIXME Есть два разных параметра "Задержка на фиксацию команды"
            pkgTx.append(getSpinBoxValue(GB_PARAM_PRM_TIME_ON));
        } break;

        case GB_COM_PRM_GET_TIME_OFF: {
            if (data.size() != 0) {
                emit debug(msgSizeError.arg(com, 2, 16).arg(data.size()));
            }
            //
            pkgTx.append(com);
            // FIXME Сделать зависимость от количества команд.
            uint8_t number = getAbsMaxNumOfSameParams(GB_PARAM_PRM_TIME_OFF);
            for(uint8_t i = 1; i <= number; i++) {
                qint16 value = getSpinBoxValue(GB_PARAM_PRM_TIME_OFF, i);
                pkgTx.append(static_cast<quint8> (value));
            }
        } break;

        case GB_COM_PRM_GET_BLOCK_COM: {
            if (data.size() != 0) {
                emit debug(msgSizeError.arg(com, 2, 16).arg(data.size()));
            }
            //
            pkgTx.append(com);
            pkgTx.append(getComboBoxValue(GB_PARAM_PRM_COM_BLOCK, 1));
            pkgTx.append(getComboBoxValue(GB_PARAM_PRM_COM_BLOCK, 9));
            pkgTx.append(getComboBoxValue(GB_PARAM_PRM_COM_BLOCK, 17));
            pkgTx.append(getComboBoxValue(GB_PARAM_PRM_COM_BLOCK, 25));
        } break;

        case GB_COM_GET_SOST: {
            if (data.size() != 1) {
                emit debug(msgSizeError.arg(com, 2, 16).arg(data.size()));
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
            uint value = 0;
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(com, 2, 16).arg(data.size()));
            }
            // FIXME Если на передатчик приходит одно любое предупреждение то выводится на экран "Предупреждение".
            pkgTx.append(com);
            value = getLineEditValue(&stateDef.fault).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));
            value = getLineEditValue(&stateDef.warning).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));
            value = getLineEditValue(&statePrm.fault).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));
            value = getLineEditValue(&statePrm.warning).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));
            value = getLineEditValue(&statePrd.fault).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));
            value = getLineEditValue(&statePrd.warning).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));
            value = getLineEditValue(&stateGlb.fault).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));
            value = getLineEditValue(&stateGlb.warning).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));

            // TODO Разобраться зачем нужен второй приемник
            value = getLineEditValue(&statePrm.fault).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));
            value = getLineEditValue(&statePrm.warning).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));

            // TODO Добавить байты данных для РЗСК
        } break;

        case GB_COM_GET_TIME: {
            // TODO Добавить обработку флага новой записи журнала
            if (data.size() != 1) {
                emit debug(msgSizeError.arg(com, 2, 16).arg(data.size()));
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
            pkgTx.append(static_cast<uint8_t> (value));
            pkgTx.append(static_cast<uint8_t> (value >> 8));

            // TODO Добавить считывание записи журнала для АСУ ТП.
        } break;

        case GB_COM_GET_COM_PRM_KEEP: {
            if (data.size() != 0) {
                emit debug(msgSizeError.arg(com, 2, 16).arg(data.size()));
            }
            //
            pkgTx.append(com);
            // FIXME Может быть еще Uвых номинальное!
            pkgTx.append(getComboBoxValue(GB_PARAM_COM_PRM_KEEP));
        } break;

        case GB_COM_GET_COM_PRD_KEEP: {
            if (data.size() != 0) {
                emit debug(msgSizeError.arg(com, 2, 16).arg(data.size()));
            }
            //
            // FIXME Добавить остальные параметры для К400. Учесть что в Р400 только совместимость!.
            pkgTx.append(com);
            pkgTx.append(getComboBoxValue(GB_PARAM_COM_PRD_KEEP));
        } break;

        case GB_COM_GET_NET_ADR: {
            hdlrComNetAdrGet(com, data);
        } break;

        case GB_COM_GET_DEVICE_NUM : {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(com, 2, 16).arg(data.size()));
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
void Bsp::procCommandWriteParam(eGB_COM com, pkg_t &data) {
    switch(com) {
        case GB_COM_PRM_SET_TIME_ON: {
            if (data.size() != 1) {
                emit debug(msgSizeError.arg(com, 2, 16).arg(data.size()));
            } else {
                uint8_t value = data.takeFirst();
                // FIXME Есть два разных параметра "Задержка на фиксацию команды"
                setSpinBoxValue(GB_PARAM_PRM_TIME_ON, value);
            }
        } break;

        case GB_COM_PRM_SET_TIME_OFF: {
            if (data.size() != 2) {
                emit debug(msgSizeError.arg(com, 2, 16).arg(data.size()));
            } else {
                uint8_t number = data.takeFirst();
                uint8_t value = data.takeFirst();
                // FIXME Есть два разных параметра "Задержка на фиксацию команды"
                setSpinBoxValue(GB_PARAM_PRM_TIME_OFF, value, number);
            }
         } break;

        case GB_COM_PRM_SET_BLOCK_COM: {
            if (data.size() != 2) {
                emit debug(msgSizeError.arg(com, 2, 16).arg(data.size()));
            } else {
                uint8_t number = data.takeFirst();
                uint8_t value = data.takeFirst();
                // FIXME Есть два разных параметра "Задержка на фиксацию команды"
                setComboBoxValueBits(GB_PARAM_PRM_COM_BLOCK, value, number);
            }
        } break;

        case GB_COM_SET_TIME: {
            if (data.size() != 9) {
                emit debug(msgSizeError.arg(com, 2, 16).arg(data.size()));
            } else {
                pkgTx.append(com);
                pkgTx.append(data);

                bool ok = false;
                quint16 year = bcd2int(data.takeFirst(), ok) + 2000;
                quint8 month = bcd2int(data.takeFirst(), ok);
                quint8 day = bcd2int(data.takeFirst(), ok);
                dt.setDate(QDate(year, month, day));

                quint8 hour = bcd2int(data.takeFirst(), ok);
                quint8 minute = bcd2int(data.takeFirst(), ok);
                quint8 second = bcd2int(data.takeFirst(), ok);
                dt.setTime(QTime(hour, minute, second));

                quint16 ms = data.takeFirst();
                ms += static_cast<quint16> (data.takeFirst()) << 8;
                if (ms != 0) {
                    qWarning() << codec->toUnicode("Milliseconds is not 0.");
                }

                quint8 source = data.takeFirst();
                if (source != 0) {
                    emit debug(msgTimeSourceError.arg(source));
                }
            }
        } break;

        case GB_COM_SET_COM_PRM_KEEP: {
            if (data.size() != 1) {
                emit debug(msgSizeError.arg(com, 2, 16).arg(data.size()));
            } else {
                uint8_t value = data.takeFirst();
                // FIXME Может быть Uвых номинальное.
                setComboBoxValue(GB_PARAM_COM_PRM_KEEP, value);
            }
        } break;

        case GB_COM_SET_COM_PRD_KEEP: {
            uint8_t value = data.takeFirst();
            uint8_t dop = data.takeFirst();
            // FIXME Добавить остальные параметры для К400.
            // Учесть что в Р400 только один байт "Совместимость".
            switch(dop) {
                case 1: {
                    setComboBoxValue(GB_PARAM_COM_PRD_KEEP, value);
                } break;

                default: qDebug("No dop byte handler: 0x%.2X", dop);
            }
        } break;

        case GB_COM_SET_NET_ADR: {
            hdlrComNetAdrSet(com, data);
        } break;

        default: {
            qWarning("No command handler: 0x%.2X", com);
        }
    }
}

//
void Bsp::procCommandWriteRegime(eGB_COM com, pkg_t &data) {
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
pkg_t Bsp::receiveFromBsp() {
    pkg_t pkg;

    if (!pkgTx.isEmpty()) {
        pkg = pkgTx;
        pkgTx.clear();

        eGB_COM com = static_cast<eGB_COM> (pkg.first());
        if (viewCom.contains(com)) {

            qDebug() << showbase << hex <<
                        QString("receiveFromBsp: ") << pkg;
        }

        pkg.insert(1, static_cast<uint8_t> (pkg.size() - 1));
        pkg.append(calcCrc(pkg));
        pkg.insert(0, 0xAA);
        pkg.insert(0, 0x55);


    }

    return pkg;
}

//
void Bsp::sendToBsp(pkg_t pkg) {
    eGB_COM com = checkPkg(pkg);

    if (viewCom.contains(com)) {
        qDebug() << showbase << hex <<
                    QString("sendToBsp command ") << com <<
                    QString(" with data: ") << pkg;
    }

    pkgTx.clear();
    if (com != GB_COM_NO) {
        procCommand(com, pkg);
    } else {
        qWarning() << "Message check error: " << showbase << hex << pkg;
    }


}

//
void Bsp::updateClock() {
    dt = dt.addSecs(1);

    params.DateTime.setYear(dt.date().year()-2000);
    params.DateTime.setMonth(dt.date().month());
    params.DateTime.setDay(dt.date().day());

    params.DateTime.setHour(dt.time().hour());
    params.DateTime.setMinute(dt.time().minute());
    params.DateTime.setSecond(dt.time().second());
}

void Bsp::setRegime(int index) {
    stateDef.regime.setCurrentIndex(index);
    statePrm.regime.setCurrentIndex(index);
    statePrd.regime.setCurrentIndex(index);
}

void Bsp::setState(int index) {
    stateDef.state.setValue(index);
    statePrm.state.setValue(index);
    statePrd.state.setValue(index);
}

void Bsp::setDopByte(int index) {
    stateDef.dopByte.setValue(index);
    statePrm.dopByte.setValue(index);
    statePrd.dopByte.setValue(index);
}

void Bsp::setLocalValues(int value) {
    // FIXME Сейчас используется только для роли пользователя. Сделать для любого!
    emit userChanged(value);
}

//
void Bsp::hdlrComNetAdrGet(eGB_COM com, pkg_t data) {
    if (data.size() != 0) {
        emit debug(msgSizeError.arg(com, 2, 16).arg(data.size()));
    }
    //
    pkgTx.append(com);
    qint16 value = getSpinBoxValue(GB_PARAM_NET_ADDRESS);
    pkgTx.append(static_cast<uint8_t> (value));
    pkgTx.append(getComboBoxValue(GB_PARAM_INTF_INTERFACE));
    pkgTx.append(getComboBoxValue(GB_PARAM_INTF_PROTOCOL));
    pkgTx.append(getComboBoxValue(GB_PARAM_INTF_BAUDRATE));
    pkgTx.append(getComboBoxValue(GB_PARAM_INTF_DATA_BITS));
    pkgTx.append(getComboBoxValue(GB_PARAM_INTF_PARITY));
    pkgTx.append(getComboBoxValue(GB_PARAM_INTF_STOP_BITS));

    char const* pwd = getLineEditValue(GB_PARAM_IS_PWD_ENGINEER).
                   toStdString().c_str();
    for(uint8_t i = 0; i < PWD_LEN; i++) {
        pkgTx.append(static_cast <uint8_t> (*pwd));
        pwd++;
    }

    pwd = getLineEditValue(GB_PARAM_IS_PWD_ADMIN).
                               toStdString().c_str();
    for(uint8_t i = 0; i < PWD_LEN; i++) {
        pkgTx.append(static_cast <uint8_t> (*pwd));
        pwd++;
    }

    pkgTx.append((uint8_t) getSpinBoxValue(GB_PARAM_IS_PWD_ENG_CNT));
    pkgTx.append((uint8_t) getSpinBoxValue(GB_PARAM_IS_PWD_ADM_CNT));
}

//
void Bsp::hdlrComNetAdrSet(eGB_COM com, pkg_t data)
{
    uint8_t dop = data.takeFirst();
    switch(dop) {
        case 1: {
            uint8_t byte = data.takeFirst();
            setSpinBoxValue(GB_PARAM_NET_ADDRESS, byte);
        } break;
        case 2: {
            uint8_t byte = data.takeFirst();
            setComboBoxValue(GB_PARAM_INTF_INTERFACE, byte);
        } break;
        case 3: {
            uint8_t byte = data.takeFirst();
            setComboBoxValue(GB_PARAM_INTF_PROTOCOL, byte);
        } break;
        case 4: {
            uint8_t byte = data.takeFirst();
            setComboBoxValue(GB_PARAM_INTF_BAUDRATE, byte);
        } break;
        case 5: {
            uint8_t byte = data.takeFirst();
            setComboBoxValue(GB_PARAM_INTF_DATA_BITS, byte);
        } break;
        case 6: {
            uint8_t byte = data.takeFirst();
            setComboBoxValue(GB_PARAM_INTF_PARITY, byte);
        } break;
        case 7: {
            uint8_t byte = data.takeFirst();
            setComboBoxValue(GB_PARAM_INTF_STOP_BITS, byte);
        } break;
        case 8: {
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
        case 9: {
            if (data.size() != 8) {
                emit debug(msgSizeError.arg(com, 2, 16).arg(data.size()));
            } else {
                QString value;
                for(uint8_t i = 0; i < PWD_LEN; i++) {
                    value.append(data.takeFirst());
                }
                mapLineEdit.value(GB_PARAM_IS_PWD_ADMIN)->setText(value);
            }
        } break;
        case 10: {
            if (data.size() != 1) {
                emit debug(msgSizeError.arg(data.size()));
            } else {
                uint8_t byte = data.takeFirst();
                setSpinBoxValue(GB_PARAM_IS_PWD_ENG_CNT, byte);
            }
        } break;
        case 11: {
            if (data.size() != 1) {
                emit debug(msgSizeError.arg(data.size()));
            } else {
                uint8_t byte = data.takeFirst();
                setSpinBoxValue(GB_PARAM_IS_PWD_ADM_CNT, byte);
            }
        } break;

        default: qDebug("No dop byte handler: 0x%.2X", dop);
    }

    data.clear();
    hdlrComNetAdrGet(GB_COM_GET_NET_ADR, data);
}
