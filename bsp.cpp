#include "bsp.h"
#include "PIg/src/flash.h"
#include "PIg/src/parameter/param.h"
#include "PIg/src/menu/txCom.hpp"
#include <QDebug>
#include "QTextCodec"
#include <QTimer>
#include <QHeaderView>

const QString Bsp::msgSizeError = "Wrong size of data in command %1: %2";
const QString Bsp::msgTimeSourceError = "Wrong source of time: %1";

QMap<eGB_PARAM, QVector<QComboBox*>> Bsp::mapCombobox;
QMap<eGB_PARAM, QLineEdit*> Bsp::mapLineEdit;
QMap<eGB_PARAM, QVector<QSpinBox*>> Bsp::mapSpinBox;

QTextCodec *Bsp::codec = QTextCodec::codecForName("CP1251");
const QString Bsp::kTimeFormat = "dd.MM.yyyy hh:mm:ss.zzz";

Bsp::device_t Bsp::device;
Bsp::state_t Bsp::stateDef;
Bsp::state_t Bsp::stateGlb;
Bsp::state_t Bsp::statePrm;
Bsp::state_t Bsp::statePrd;
Bsp::jrn_t Bsp::journals;

QDateTime *Bsp::dt = nullptr;
pkg_t Bsp::pkgTx;
pkg_t Bsp::pkgRx;

Bsp::Bsp(QWidget *parent) : QTreeWidget(parent) {
    COMPILE_TIME_ASSERT(MAX_NUM_COM_SEND_IN_CYLCE == (MAX_NUM_COM_BUF2 + 4));

    // Эти строки не влияют на содержимое заголовка, но влияют на resize ниже.
    headerItem()->setText(0, codec->toUnicode("Parameter"));
    headerItem()->setText(1, codec->toUnicode("Value"));

//    expandAll();
//    header()->resizeSections(QHeaderView::ResizeToContents);
//    header()->setSectionResizeMode(0, QHeaderView::Fixed);
//    header()->resizeSection(0, header()->sectionSize(0) + 5);
//    setFixedWidth(1.75*header()->sectionSize(0));

    header()->setSectionResizeMode(0, QHeaderView::Fixed);
    header()->resizeSection(0, 220);
    setFixedWidth(400);

    setSelectionMode(QAbstractItemView::NoSelection);
    setFocusPolicy(Qt::NoFocus);
}

//
void Bsp::initParam() {
    eGB_PARAM param = GB_PARAM_NULL_PARAM;

    setLineEditValue(eGB_PARAM::GB_PARAM_IS_PWD_ENGINEER, "00000001");
    setLineEditValue(eGB_PARAM::GB_PARAM_IS_PWD_ADMIN, "00000002");
    setSpinBoxValue(eGB_PARAM::GB_PARAM_IS_PWD_ENG_CNT, 2);
    setSpinBoxValue(eGB_PARAM::GB_PARAM_IS_PWD_ADM_CNT, 3);

    setComboBoxValue(stateGlb.regime, eGB_REGIME::GB_REGIME_ENABLED);
    setSpinBoxValue(stateGlb.state, 1);
    setSpinBoxValue(stateGlb.dopByte, 1);
    setLineEditValue(stateGlb.fault, "0000");
    setLineEditValue(stateGlb.warning, "0000");
    setLineEditValue(stateDef.fault, "0000");
    setLineEditValue(stateDef.warning, "0000");
    setLineEditValue(statePrm.fault, "0000");
    setLineEditValue(statePrm.warning, "0000");
    setLineEditValue(statePrd.fault, "0000");
    setLineEditValue(statePrd.warning, "0000");

    setComboBoxValue(GB_PARAM_COM_PRD_KEEP, 0);
    setComboBoxValue(GB_PARAM_COM_PRM_KEEP, 0);
    setComboBoxValue(GB_PARAM_TIME_SYNCH, 0);

    setComboBoxValue(GB_PARAM_INTF_INTERFACE, TInterface::USB);
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

    setComboBoxValue(device.isDef, 0);
    setSpinBoxValue(GB_PARAM_PRM_COM_NUMS, 8);  // количество команд = num*4
    setSpinBoxValue(GB_PARAM_PRD_COM_NUMS, 8);  // количество команд = num*4
    setComboBoxValue(GB_PARAM_NUM_OF_DEVICES, GB_NUM_DEVICES_2);
    setComboBoxValue(device.typeLine, GB_TYPE_LINE_OPTO);
    setComboBoxValue(GB_PARAM_COMP_K400, 0);
    setComboBoxValue(device.typeDevice, AVANT_K400);
    setComboBoxValue(device.typeOpto, TYPE_OPTO_STANDART);

//    params.glb.setDInputState(0);
//    params.prm.setIndCom8(0, 0);
//    params.prm.setIndCom8(1, 0);
//    params.prm.setIndCom8(2, 0);
//    params.prm.setIndCom8(3, 0);
//    params.prd.setIndCom8(0, 0);
//    params.prd.setIndCom8(1, 0);
//    params.prd.setIndCom8(2, 0);
    //    params.prd.setIndCom8(3, 0);


    addJSEntry(USER_admin, USER_SOURCE_pc, TSecurityEvent::EVENT_blkAdmin);
    addJSEntry(USER_operator, USER_SOURCE_pi, TSecurityEvent::EVENT_MAX);
}

void Bsp::initClock() {
    if (dt == nullptr) {
        dt = new QDateTime();
        *dt = QDateTime::currentDateTime();

        QTimer *timerClock = new QTimer();
        QObject::connect(timerClock, &QTimer::timeout, &Bsp::updateClock);
        timerClock->start(1000);
    }
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
bool Bsp::checkPkg(QVector<uint8_t> &pkg, eGB_COM &com) {
    uint8_t byte;

    byte = pkg.takeFirst();
    if (byte != 0x55) {
        return false;
    }

    byte = pkg.takeFirst();
    if (byte != 0xAA) {
        return false;
    }

    byte = pkg.takeLast();
    if (byte != calcCrc(pkg)) {
        return false;
    }

    com = static_cast<eGB_COM> (pkg.takeFirst());

    byte = pkg.takeFirst();
    if (byte != pkg.size()) {
        return false;
    }

    return true;
}

void Bsp::crtTreeDevice() {
    QTreeWidgetItem* top = new QTreeWidgetItem();
    insertTopLevelItem(topLevelItemCount(), top);
    top->setText(0, codec->toUnicode("Устройство"));

    QTreeWidgetItem *item = nullptr;

    item = new QTreeWidgetItem();
    device.isDef = new QComboBox();
    item->setText(0, codec->toUnicode("Защита"));
    top->addChild(item);
    fillComboboxListOnOff(device.isDef);
    setItemWidget(item, 1, device.isDef);

    crtSpinBox(GB_PARAM_PRM_COM_NUMS);
    crtSpinBox(GB_PARAM_PRD_COM_NUMS);
    crtComboBox(GB_PARAM_NUM_OF_DEVICES);

    item = new QTreeWidgetItem();
    device.typeLine = new QComboBox();
    item->setText(0, codec->toUnicode("Тип линии"));
    top->addChild(item);
    fillComboboxListTypeLine(device.typeLine);
    setItemWidget(item, 1, device.typeLine);

    // FIXME Может быть еще Р400
    crtComboBox(GB_PARAM_COMP_K400);

    item = new QTreeWidgetItem();
    device.typeDevice = new QComboBox();
    item->setText(0, codec->toUnicode("Тип аппарата"));
    top->addChild(item);
    fillComboboxListTypeDevice(device.typeDevice);
    setItemWidget(item, 1, device.typeDevice);

    item = new QTreeWidgetItem();
    device.typeOpto = new QComboBox();
    item->setText(0, codec->toUnicode("Тип оптики"));
    top->addChild(item);
    fillComboboxListTypeOpto(device.typeOpto);
    setItemWidget(item, 1, device.typeOpto);

    top->setExpanded(true);
}

void Bsp::crtTreeGlb() {
    QTreeWidgetItem* top = new QTreeWidgetItem();
    insertTopLevelItem(topLevelItemCount(), top);

    top->setText(0, codec->toUnicode("Общие"));

    crtComboBox(GB_PARAM_COM_PRD_KEEP);
    crtComboBox(GB_PARAM_COM_PRM_KEEP);
    crtComboBox(GB_PARAM_TIME_SYNCH);

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

    connect(stateGlb.regime, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Bsp::setRegime);

    connect(stateGlb.state, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &Bsp::setState);

    connect(stateGlb.dopByte, QOverload<int>::of(&QSpinBox::valueChanged),
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
    state.regime = new QComboBox();
    fillComboboxListRegime(state.regime);
    setItemWidget(item, 1, state.regime);

    item = new QTreeWidgetItem();
    item->setText(0, codec->toUnicode("Состояние"));
    ltop->addChild(item);
    state.state = new QSpinBox();
    state.state->setRange(0, MAX_NUM_DEVICE_STATE);
    state.state->setValue(0);
    setItemWidget(item, 1, state.state);

    item = new QTreeWidgetItem();
    item->setText(0, codec->toUnicode("Доп. байт"));
    ltop->addChild(item);
    state.dopByte = new QSpinBox();
    state.dopByte->setRange(0, 255);
    state.dopByte->setValue(0);
    setItemWidget(item, 1, state.dopByte);

    item = new QTreeWidgetItem();
    item->setText(0, codec->toUnicode("Неисправности"));
    ltop->addChild(item);
    state.fault = new QLineEdit();
    state.fault->setMaxLength(4);
    state.fault->setInputMask("HHHH");
    state.fault->setValidator(&errValidator);
    state.fault->setText("0000");
    setItemWidget(item, 1, state.fault);

    connect(state.fault, &QLineEdit::selectionChanged,
            state.fault, &QLineEdit::deselect);

    item = new QTreeWidgetItem();
    item->setText(0, codec->toUnicode("Предупреждения"));
    ltop->addChild(item);
    state.warning = new QLineEdit();
    state.warning->setMaxLength(4);
    state.warning->setInputMask("HHHH");
    state.warning->setValidator(&errValidator);
    state.warning->setText("0000");
    setItemWidget(item, 1, state.warning);

    connect(state.warning, &QLineEdit::selectionChanged,
            state.warning, &QLineEdit::deselect);

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

//    crtComboBox(GB_PARAM_IS_USER);
    crtLineEdit(GB_PARAM_IS_PWD_ENGINEER, "00000000");
    crtSpinBox(GB_PARAM_IS_PWD_ENG_CNT);
    crtLineEdit(GB_PARAM_IS_PWD_ADMIN, "00000000");
    crtSpinBox(GB_PARAM_IS_PWD_ADM_CNT);

    top->setExpanded(true);
}

void Bsp::crtJournals() {
    QTreeWidgetItem* top = new QTreeWidgetItem();
    insertTopLevelItem(topLevelItemCount(), top);
    top->setText(0, codec->toUnicode("Журналы"));

    journals.security = new QTreeWidgetItem();
    top->addChild(journals.security);
    journals.security->setText(0, codec->toUnicode("Безопасность"));

    for(quint16 i = 0 ; i < SIZE_OF_SECURITY_JRN; i++) {
        journals.security->addChild(crtJSEntry(i));
    }

    journals.securityTw = this;

    expandItem(journals.security);
    expandItem(top);
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
void Bsp::fillComboboxListOnOff(QComboBox *combobox) {
    if (combobox != nullptr) {
        for(uint8_t i = 0; i < SIZE_OF(fcOnOff); i++) {
            combobox->addItem(codec->toUnicode(fcOnOff[i]), i);
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
void Bsp::fillComboboxListTypeDevice(QComboBox *combobox) {
    if (combobox != nullptr) {
        combobox->addItem(codec->toUnicode("Р400"), AVANT_R400);
        combobox->addItem(codec->toUnicode("РЗСК"), AVANT_RZSK);
        combobox->addItem(codec->toUnicode("К400"), AVANT_K400);
        combobox->addItem(codec->toUnicode("Р400М"), AVANT_R400M);
    }
}

//
void Bsp::fillComboboxListTypeLine(QComboBox *combobox) {
    if (combobox != nullptr) {
        combobox->addItem(codec->toUnicode("ВЧ"), GB_TYPE_LINE_UM);
        combobox->addItem(codec->toUnicode("ВОЛС"), GB_TYPE_LINE_OPTO);
        combobox->addItem(codec->toUnicode("Е1"), GB_TYPE_LINE_E1);
    }
}

//
void Bsp::fillComboboxListTypeOpto(QComboBox *combobox) {
    if (combobox != nullptr) {
        combobox->addItem(codec->toUnicode("Стандартная"), TYPE_OPTO_STANDART);
        combobox->addItem(codec->toUnicode("Кольцо"), TYPE_OPTO_RING_UNI);
        combobox->addItem(codec->toUnicode("Двунапр. кольцо"), TYPE_OPTO_RING_BI);
    }
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
            qWarning() << msg;
        }
    } else {
        QString msg = QString("%1: Parameter '%2' (%3) not found.").
                      arg(__FUNCTION__).arg(param).arg(number);
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
QString Bsp::getLineEditValue(eGB_PARAM param) {
    QString value;
    QLineEdit *lineedit = mapLineEdit.value(param, nullptr);

    if (lineedit != nullptr) {
        value = lineedit->text();
    } else {
        QString msg = QString("%1: Parameter '%2' not found.").
                      arg(__FUNCTION__).arg(getParamName(param));
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
            qWarning() << msg;
        }
    } else {
        QString msg = QString("%1: Parameter '%2' not found.").
                      arg(__FUNCTION__).arg(getParamName(param));
        qWarning() << msg;
    }
}

//
int Bsp::setLineEditValue(QLineEdit *lineedit, std::string value) {
    int index = -1;

    lineedit->setText(Bsp::codec->toUnicode(value.c_str()));
    index = 0;
    // FIXME Добавить проверку значения!

    return index;
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
                          arg(__FUNCTION__).arg(value).arg(param);
            qWarning() << msg;
        }
    } else {
        QString msg = QString("%1: Parameter '%2' not found.").
                      arg(__FUNCTION__).arg(param);
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

//
void Bsp::procCommand(eGB_COM com, pkg_t &data) {
    pkgTx.clear();
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

void Bsp::addJSEntry(user_t user, userSrc_t src, TSecurityEvent::event_t event) {
    QTreeWidget *tw = journals.securityTw;
    QTreeWidgetItem *item;
    QComboBox *combobox;

    if (tw == nullptr) {
        return;
    }

    if (journals.cntSecurity != 0) {
        item = journals.security->child(journals.posSecurity);
        item->setForeground(0, Qt::black);
        journals.posSecurity = (journals.posSecurity + 1) % SIZE_OF_SECURITY_JRN;
    } else {
        journals.posSecurity = 0;
    }

    item = journals.security->child(journals.posSecurity);

    combobox = static_cast<QComboBox*> (tw->itemWidget(item->child(0), 1));
    std::string ustr = (user < USER_MAX) ? fcUser[user] : std::to_string(user);
    combobox->clear();
    combobox->addItem(codec->toUnicode(ustr.c_str()), user);

    combobox = static_cast<QComboBox*> (tw->itemWidget(item->child(1), 1));
    std::string sstr = TSecurityEvent::getUserSourceString(src);
    combobox->clear();
    combobox->addItem(codec->toUnicode(sstr.c_str()), src);

    combobox = static_cast<QComboBox*> (tw->itemWidget(item->child(2), 1));
    std::string estr = TSecurityEvent::getEventString(event);
    estr += " (" + std::to_string(event) + ")";
    combobox->clear();
    combobox->addItem(codec->toUnicode(estr.c_str()), event);

    combobox = static_cast<QComboBox*> (tw->itemWidget(item->child(3), 1));
    combobox->clear();
    combobox->addItem(QDateTime::currentDateTime().toString(kTimeFormat));

    if (journals.cntSecurity <  SIZE_OF_SECURITY_JRN) {
        journals.cntSecurity++;
    }

    item->setForeground(0, Qt::blue);
    item->setHidden(false);
}

//
QTreeWidgetItem* Bsp::crtJSEntry(quint16 index) {
    QTreeWidgetItem *item;
    QComboBox *combobox;

    item = new QTreeWidgetItem();
    QTreeWidgetItem *subitem;

    subitem = new QTreeWidgetItem();
    subitem->setText(0, codec->toUnicode("Пользователь"));
    combobox = new QComboBox();
    combobox->setEditable(false);
    combobox->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    item->addChild(subitem);
    setItemWidget(subitem, 1, combobox);

    // FIXME брать строки из основного проекта.
    subitem = new QTreeWidgetItem();
    subitem->setText(0, codec->toUnicode("Доступ"));
    combobox = new QComboBox();
    combobox->setEditable(false);
    combobox->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    item->addChild(subitem);
    setItemWidget(subitem, 1, combobox);

    subitem = new QTreeWidgetItem();
    subitem->setText(0, codec->toUnicode("Событие"));
    combobox = new QComboBox();
    combobox->setEditable(false);
    combobox->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    item->addChild(subitem);
    setItemWidget(subitem, 1, combobox);

    subitem = new QTreeWidgetItem();
    subitem->setText(0, codec->toUnicode("Время"));
    combobox = new QComboBox();
    combobox->setEditable(false);
    combobox->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    item->addChild(subitem);
    setItemWidget(subitem, 1, combobox);

    journals.security->addChild(item);
    std::string name = "Запись ";
    name += std::to_string(index);
    item->setText(0, codec->toUnicode(name.c_str()));
    item->setHidden(true);

    return item;
}

//
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
quint8 Bsp::bcd2int(quint8 bcd, bool &ok) {
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
quint8 Bsp::int2bcd(quint8 val, bool &ok) {
    quint8 bcd = 0;

    ok = (val < 100);
    if (!ok) {
        qWarning("Can't convert value 0x%.2X to BCD code!", val);
    }

    bcd = (val % 10);
    bcd += (val / 10) << 4;

    return bcd;
}

//
void Bsp::procCommandReadJournal(eGB_COM com, pkg_t &data) {
    switch(com) {
        case GB_COM_DEF_GET_JRN_CNT: {
            if (!data.isEmpty()) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            // TODO Обработчик для команды чтения количества записей журнала.
            pkgTx.append(com);
            pkgTx.append(0);
            pkgTx.append(0);
        } break;

        case GB_COM_PRM_GET_JRN_CNT: {
            if (!data.isEmpty()) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            // TODO Обработчик для команды чтения количества записей журнала.
            pkgTx.append(com);
            pkgTx.append(0);
            pkgTx.append(0);
        } break;

        case GB_COM_PRD_GET_JRN_CNT: {
            if (!data.isEmpty()) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            // TODO Обработчик для команды чтения количества записей журнала.
            pkgTx.append(com);
            pkgTx.append(0);
            pkgTx.append(0);
        } break;

        case GB_COM_GET_JRN_CNT : {
            if (!data.isEmpty()) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            // TODO Обработчик для команды чтения количества записей журнала.
            pkgTx.append(com);
            pkgTx.append(0);
            pkgTx.append(0);
        } break;

        case GB_COM_GET_JRN_IS_CNT: {
            hdlrComGetJrnIsCnt(com, data);
        } break;

        case GB_COM_GET_JRN_IS_ENTRY: {
            hdlrComGetJrnIsEntry(com, data);
        } break;

        case GB_COM_JRN_IS_CLR: {
            hdlrComJrnIsClr(com, data);
        } break;

        case GB_COM_JRN_IS_SET_ENTRY: {
            hdlrComGetJrnIsSetEntry(com, data);
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
            if (data.size() != 0) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            pkgTx.append(com);
            pkgTx.append(1);
            pkgTx.append(2);
            pkgTx.append(3);
        } break;

        case GB_COM_DEF_GET_LINE_TYPE: {
            if (data.size() != 0) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            // FIXME Добавить параметр
            pkgTx.append(0x01);
        } break;

        case GB_COM_PRM_GET_TIME_ON: {
            if (data.size() != 0) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            // FIXME Есть два разных параметра "Задержка на фиксацию команды"
            pkgTx.append(Bsp::getSpinBoxValue(GB_PARAM_PRM_TIME_ON));
        } break;

        case GB_COM_PRM_GET_TIME_OFF: {
            if (data.size() != 0) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            // FIXME Сделать зависимость от количества команд.
            uint8_t number = getAbsMaxNumOfSameParams(GB_PARAM_PRM_TIME_OFF);
            for(uint8_t i = 1; i <= number; i++) {
                qint16 value = Bsp::getSpinBoxValue(GB_PARAM_PRM_TIME_OFF, i);
                pkgTx.append(static_cast<quint8> (value));
            }
        } break;

        case GB_COM_PRM_GET_BLOCK_COM: {
            if (data.size() != 0) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRM_COM_BLOCK, 1));
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRM_COM_BLOCK, 9));
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRM_COM_BLOCK, 17));
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRM_COM_BLOCK, 25));
        } break;

        case GB_COM_GET_SOST: {
            if (data.size() != 1) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            pkgTx.append(getComboBoxValue(stateDef.regime));
            pkgTx.append(getSpinBoxValue(stateDef.state));
            pkgTx.append(getSpinBoxValue(stateDef.dopByte));
            pkgTx.append(getComboBoxValue(statePrm.regime));
            pkgTx.append(getSpinBoxValue(statePrm.state));
            pkgTx.append(getSpinBoxValue(statePrm.dopByte));
            pkgTx.append(getComboBoxValue(statePrd.regime));
            pkgTx.append(getSpinBoxValue(statePrd.state));
            pkgTx.append(getSpinBoxValue(statePrd.dopByte));
            // TODO Разобраться зачем нужен второй приемник
            pkgTx.append(getComboBoxValue(statePrm.regime));
            pkgTx.append(getSpinBoxValue(statePrd.state));
            pkgTx.append(getSpinBoxValue(statePrd.dopByte));
            // FIXME Добавить состояние индикации
        } break;

        case GB_COM_GET_FAULT: {
            bool ok = false;
            uint value = 0;
            if (!data.isEmpty()) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            // FIXME Если на передатчик приходит одно любое предупреждение то
            // выводится на экран "Предупреждение", при этом код не известен.
            pkgTx.append(com);
            value = getLineEditValue(stateDef.fault).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));
            value = getLineEditValue(stateDef.warning).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));
            value = getLineEditValue(statePrm.fault).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));
            value = getLineEditValue(statePrm.warning).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));
            value = getLineEditValue(statePrd.fault).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));
            value = getLineEditValue(statePrd.warning).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));
            value = getLineEditValue(stateGlb.fault).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));
            value = getLineEditValue(stateGlb.warning).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));

            // TODO Разобраться зачем нужен второй приемник
            value = getLineEditValue(statePrm.fault).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));
            value = getLineEditValue(statePrm.warning).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t> (value >> 8));
            pkgTx.append(static_cast<uint8_t> (value));

            // TODO Добавить байты данных для РЗСК
        } break;

        case GB_COM_GET_TIME: {
            // TODO Добавить обработку флага новой записи журнала
            if (data.size() != 1) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            bool ok;
            pkgTx.append(com);
            pkgTx.append(int2bcd(dt->date().year() - 2000, ok));
            pkgTx.append(int2bcd(dt->date().month(), ok));
            pkgTx.append(int2bcd(dt->date().day(), ok));
            pkgTx.append(int2bcd(dt->time().hour(), ok));
            pkgTx.append(int2bcd(dt->time().minute(), ok));
            pkgTx.append(int2bcd(dt->time().second(), ok));

            uint16_t value = dt->time().msec();
            pkgTx.append(static_cast<uint8_t> (value));
            pkgTx.append(static_cast<uint8_t> (value >> 8));

            // TODO Добавить считывание записи журнала для АСУ ТП.
        } break;

        case GB_COM_GET_TIME_SINCHR : {
            if (data.size() != 0) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_TIME_SYNCH));
            // TODO В РЗСК второй байт "Тип детектора"
        } break;

        case GB_COM_GET_COM_PRM_KEEP: {
            if (data.size() != 0) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            // FIXME Может быть еще Uвых номинальное!
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_COM_PRM_KEEP));
        } break;

        case GB_COM_GET_COM_PRD_KEEP: {
            if (data.size() != 0) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            // FIXME Добавить остальные параметры для К400. Учесть что в Р400 только совместимость!.
            pkgTx.append(com);
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_COM_PRD_KEEP));
        } break;

        case GB_COM_GET_NET_ADR: {
            hdlrComNetAdrGet(com, data);
        } break;

        case GB_COM_GET_DEVICE_NUM : {
            if (!data.isEmpty()) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            // FIXME Добавить параметр.
            pkgTx.append(0x02);
        } break;

        case GB_COM_GET_VERS: {
            hdlrComGetVers(com, data);
        } break;

        default: {
            qWarning("No command handler: 0x%.2X", com);
        }
    }
}

//
void Bsp::procCommandWriteParam(eGB_COM com, pkg_t &data) {
    pkgTx.append(com);
    pkgTx.append(data);

    switch(com) {
        case GB_COM_PRM_SET_TIME_ON: {
            if (data.size() != 1) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            } else {
                uint8_t value = data.takeFirst();
                // FIXME Есть два разных параметра "Задержка на фиксацию команды"
                Bsp::setSpinBoxValue(GB_PARAM_PRM_TIME_ON, value);
            }
        } break;

        case GB_COM_PRM_SET_TIME_OFF: {
            if (data.size() != 2) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            } else {
                uint8_t number = data.takeFirst();
                uint8_t value = data.takeFirst();
                // FIXME Есть два разных параметра "Задержка на фиксацию команды"
                Bsp::setSpinBoxValue(GB_PARAM_PRM_TIME_OFF, value, number);
            }
        } break;

        case GB_COM_PRM_SET_BLOCK_COM: {
            if (data.size() != 2) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            } else {
                uint8_t number = data.takeFirst();
                uint8_t value = data.takeFirst();
                // FIXME Есть два разных параметра "Задержка на фиксацию команды"
                Bsp::setComboBoxValueBits(GB_PARAM_PRM_COM_BLOCK, value, number);
            }
        } break;

        case GB_COM_SET_TIME: {
            if (data.size() != 9) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            } else {
                bool ok = false;
                quint16 year = bcd2int(data.takeFirst(), ok) + 2000;
                quint8 month = bcd2int(data.takeFirst(), ok);
                quint8 day = bcd2int(data.takeFirst(), ok);
                dt->setDate(QDate(year, month, day));

                quint8 hour = bcd2int(data.takeFirst(), ok);
                quint8 minute = bcd2int(data.takeFirst(), ok);
                quint8 second = bcd2int(data.takeFirst(), ok);
                dt->setTime(QTime(hour, minute, second));

                quint16 ms = data.takeFirst();
                ms += static_cast<quint16> (data.takeFirst()) << 8;
                if (ms != 0) {
                    qWarning() << Bsp::codec->toUnicode("Milliseconds is not 0.");
                }

                quint8 source = data.takeFirst();
                if (source != 0) {
                    qWarning() << msgTimeSourceError.arg(source);
                }
            }
        } break;

        case GB_COM_SET_TIME_SINCHR:{
            // TODO В РЗСК два байта, т.к. может быть два параметра
            if (data.size() != 2) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            } else {
                uint8_t value = data.takeFirst();
                Bsp::setComboBoxValue(GB_PARAM_TIME_SYNCH, value);
            }
        } break;

        case GB_COM_SET_COM_PRM_KEEP: {
            if (data.size() != 1) {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            } else {
                uint8_t value = data.takeFirst();
                // FIXME Может быть Uвых номинальное.
                Bsp::setComboBoxValue(GB_PARAM_COM_PRM_KEEP, value);
            }
        } break;

        case GB_COM_SET_COM_PRD_KEEP: {
            uint8_t value = data.takeFirst();
            uint8_t dop = data.takeFirst();
            // FIXME Добавить остальные параметры для К400.
            // Учесть что в Р400 только один байт "Совместимость".
            switch(dop) {
                case 1: {
                    Bsp::setComboBoxValue(GB_PARAM_COM_PRD_KEEP, value);
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
            qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
        }

        setComboBoxValue(stateGlb.regime, eGB_REGIME::GB_REGIME_DISABLED);
        pkgTx.append(com);
    } break;
    case GB_COM_SET_REG_ENABLED: {
        if (!data.isEmpty()) {
            qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
        }

        setComboBoxValue(stateGlb.regime, eGB_REGIME::GB_REGIME_ENABLED);
        pkgTx.append(com);
    } break;

    default: {
        qWarning("No command handler: 0x%.2X", com);
    }
    }
}

void Bsp::hdlrComGetVers(eGB_COM com, pkg_t &data) {
    uint16_t vers = 0;

    if (data.size() != 0) {
        qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
    }
    //
    pkgTx.append(com);
    pkgTx.append(getComboBoxValue(device.isDef) ? 1 : 0);
    pkgTx.append(getSpinBoxValue(GB_PARAM_PRM_COM_NUMS) / 4); // прм1
    pkgTx.append(getSpinBoxValue(GB_PARAM_PRM_COM_NUMS) / 4); // прм2
    pkgTx.append(getSpinBoxValue(GB_PARAM_PRD_COM_NUMS) / 4);
    pkgTx.append(getComboBoxValue(GB_PARAM_NUM_OF_DEVICES)+1);
    pkgTx.append(getComboBoxValue(device.typeLine));

    vers = 0x0111;  // GB_IC_BSP_MCU
    pkgTx.append(vers >> 8);
    pkgTx.append(vers);
    vers = 0x2002;  // GB_IC_BSP_DSP
    pkgTx.append(vers >> 8);
    pkgTx.append(vers);

    // FIXME В Р400 отдельная
    pkgTx.append(getComboBoxValue(GB_PARAM_COMP_K400));

    vers = 0x33;    // GB_IC_BSK_PLIS_PRD1
    pkgTx.append(vers);
    vers = 0x44;    // GB_IC_BSK_PLIS_PRD2
    pkgTx.append(vers);
    vers = 0x55;    // GB_IC_BSK_PLIS_PRM1
    pkgTx.append(vers);
    vers = 0x66;    // GB_IC_BSK_PLIS_PRM2
    pkgTx.append(vers);
    vers = 0x77;    // GB_IC_BSZ_PLIS
    pkgTx.append(vers);

    pkgTx.append(getComboBoxValue(device.typeDevice));

    vers = 0x0128;  // GB_IC_PI_MCU
    pkgTx.append(vers >> 8);
    pkgTx.append(vers);

    pkgTx.append(getComboBoxValue(device.typeOpto));

    vers = 0x9009;  // GB_IC_BSP_DSP
    pkgTx.append(vers >> 8);
    pkgTx.append(vers);
}

//
void  Bsp::hdlrComNetAdrGet(eGB_COM com, pkg_t &data) {

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
void  Bsp::hdlrComNetAdrSet(eGB_COM com, pkg_t &data) {
    uint8_t dop = data.takeFirst();
    switch(dop) {
    case POS_COM_NET_ADR_netAdr: {
        uint8_t byte = data.takeFirst();
        setSpinBoxValue(GB_PARAM_NET_ADDRESS, byte);
    } break;
    case POS_COM_NET_ADR_interface: {
        uint8_t byte = data.takeFirst();
        setComboBoxValue(GB_PARAM_INTF_INTERFACE, byte);
    } break;
    case POS_COM_NET_ADR_protocol: {
        uint8_t byte = data.takeFirst();
        setComboBoxValue(GB_PARAM_INTF_PROTOCOL, byte);
    } break;
    case POS_COM_NET_ADR_baudrate: {
        uint8_t byte = data.takeFirst();
        setComboBoxValue(GB_PARAM_INTF_BAUDRATE, byte);
    } break;
    case POS_COM_NET_ADR_dataBits: {
        uint8_t byte = data.takeFirst();
        setComboBoxValue(GB_PARAM_INTF_DATA_BITS, byte);
    } break;
    case POS_COM_NET_ADR_parity: {
        uint8_t byte = data.takeFirst();
        setComboBoxValue(GB_PARAM_INTF_PARITY, byte);
    } break;
    case POS_COM_NET_ADR_stopBits: {
        uint8_t byte = data.takeFirst();
        setComboBoxValue(GB_PARAM_INTF_STOP_BITS, byte);
    } break;
    case POS_COM_NET_ADR_pwdEngineer: {
        if (data.size() != 8) {
            qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
        } else {
            QString value;
            for(uint8_t i = 0; i < PWD_LEN; i++) {
                value.append(data.takeFirst());
            }
            mapLineEdit.value(GB_PARAM_IS_PWD_ENGINEER)->setText(value);
        }
    } break;
    case POS_COM_NET_ADR_pwdAdmin: {
        if (data.size() != 8) {
            qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
        } else {
            QString value;
            for(uint8_t i = 0; i < PWD_LEN; i++) {
                value.append(data.takeFirst());
            }
            mapLineEdit.value(GB_PARAM_IS_PWD_ADMIN)->setText(value);
        }
    } break;
    case POS_COM_NET_ADR_cntEngineer: {
        if (data.size() != 1) {
            qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
        } else {
            uint8_t byte = data.takeFirst();
            setSpinBoxValue(GB_PARAM_IS_PWD_ENG_CNT, byte);
        }
    } break;
    case POS_COM_NET_ADR_cntAdmin: {
        if (data.size() != 1) {
            qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
        } else {
            uint8_t byte = data.takeFirst();
            setSpinBoxValue(GB_PARAM_IS_PWD_ADM_CNT, byte);
        }
    } break;

    default: qDebug("No dop byte handler: 0x%.2X", dop);
    }
}

//
void
Bsp::hdlrComJrnIsClr(eGB_COM com, pkg_t &data) {
    if (!data.isEmpty()) {
        qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
        return;
    }

    journals.cntSecurity = 0;
    journals.posSecurity = 0;

    for(quint16 i = 0; i < journals.security->childCount(); i++) {
        journals.security->child(i)->setHidden(0);
    }

}

//
void
Bsp::hdlrComGetJrnIsCnt(eGB_COM com, pkg_t &data) {
    uint16_t len;

    if (!data.isEmpty()) {
        qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
        return;
    }

    if (journals.cntSecurity == 0) {
        len = 0;
    } else if (journals.cntSecurity < SIZE_OF_SECURITY_JRN) {
        len = journals.cntSecurity;
    } else {
        len = journals.posSecurity;
        len |= 0xC000;
    }

    pkgTx.append(com);
    pkgTx.append(len);
    pkgTx.append(len >> 8);
}

//
void
Bsp::hdlrComGetJrnIsEntry(eGB_COM com, pkg_t &data) {
    bool ok;
    QComboBox *combobox;
    QTreeWidgetItem *item;
    QTreeWidget *tw;

    if (data.count() != 2) {
        qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
        return;
    }

    // FIXME Порядок байт в запросе Р400м отличается от других (перевернут)
    uint16_t num = data.takeFirst();
    num = (num << 8) + data.takeFirst();

    if (num >= journals.cntSecurity) {
        qWarning() << "Number of entry is too big! num = " << num <<
            ", cntSecurity = " << journals.cntSecurity;
        return;
    }

    pkgTx.append(com);

    tw = journals.securityTw;
    item = journals.security->child(num);

    // User
    combobox = static_cast<QComboBox*> (tw->itemWidget(item->child(0), 1));
    pkgTx.append(combobox->currentData().toUInt());

    // Source
    combobox = static_cast<QComboBox*> (tw->itemWidget(item->child(1), 1));
    pkgTx.append(combobox->currentData().toUInt());

    // Event
    combobox = static_cast<QComboBox*> (tw->itemWidget(item->child(2), 1));
    pkgTx.append(combobox->currentData().toUInt());

    combobox = static_cast<QComboBox*> (tw->itemWidget(item->child(3), 1));
    QDateTime dt = QDateTime::fromString(combobox->currentText(), kTimeFormat);

    uint16_t ms = dt.time().msec();
    pkgTx.append(int2bcd(dt.date().year() - 2000, ok));
    pkgTx.append(int2bcd(dt.date().month(), ok));
    pkgTx.append(int2bcd(dt.date().day(), ok));
    pkgTx.append(int2bcd(dt.time().hour(), ok));
    pkgTx.append(int2bcd(dt.time().minute(), ok));
    pkgTx.append(int2bcd(dt.time().second(), ok));
    pkgTx.append(ms >> 8);
    pkgTx.append(ms);
}

//
void
Bsp::hdlrComGetJrnIsSetEntry(eGB_COM com, pkg_t &data) {
    pkgTx.append(com);
    pkgTx.append(data);

    if (data.count() != 3) {
        qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
        return;
    }

    user_t user = static_cast<user_t> (data.takeFirst());
    userSrc_t src = static_cast<userSrc_t> (data.takeFirst());
    TSecurityEvent::event_t event;
    event = static_cast<TSecurityEvent::event_t>(data.takeFirst());
    addJSEntry(user, src, event);

}

//
void Bsp::setRegime(int index) {
    stateDef.regime->setCurrentIndex(index);
    statePrm.regime->setCurrentIndex(index);
    statePrd.regime->setCurrentIndex(index);
}

//
void Bsp::setState(int index) {
    stateDef.state->setValue(index);
    statePrm.state->setValue(index);
    statePrd.state->setValue(index);
}

//
void Bsp::setDopByte(int index) {
    stateDef.dopByte->setValue(index);
    statePrm.dopByte->setValue(index);
    statePrd.dopByte->setValue(index);
}

//
void Bsp::updateClock() {
    *dt = dt->addSecs(1);
}
