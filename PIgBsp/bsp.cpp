#include "bsp.h"
#include "PIg/src/flash.h"
#include "PIg/src/menu/txCom.h"
#include "PIg/src/parameter/param.h"
#include "QTextCodec"
#include <QDebug>
#include <QHeaderView>
#include <QTimer>

const QString Bsp::msgSizeError       = "Wrong size of data in command %1: %2";
const QString Bsp::msgTimeSourceError = "Wrong source of time: %1";

QMap<eGB_PARAM, QVector<QComboBox *>> Bsp::mapCombobox;
QMap<eGB_PARAM, QLineEdit *>          Bsp::mapLineEdit;
QMap<eGB_PARAM, QVector<QSpinBox *>>  Bsp::mapSpinBox;

QTextCodec *  Bsp::codec       = QTextCodec::codecForName("CP1251");
const QString Bsp::kTimeFormat = "dd.MM.yyyy hh:mm:ss.zzz";

Bsp::device_t  Bsp::device;
Bsp::state_t   Bsp::stateDef;
Bsp::state_t   Bsp::stateGlb;
Bsp::state_t   Bsp::statePrm;
Bsp::state_t   Bsp::statePrd;
Bsp::test_t    Bsp::test;
Bsp::measure_t Bsp::m_measure;

QDateTime *Bsp::dt = nullptr;
pkg_t      Bsp::pkgTx;
pkg_t      Bsp::pkgRx;

Bsp::Bsp(QWidget *parent) : QTreeWidget(parent)
{
    // ��� ������ �� ������ �� ���������� ���������, �� ������ �� resize ����.
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
void Bsp::initParam()
{
    eGB_PARAM param = GB_PARAM_NULL_PARAM;

    void (QComboBox::*signal)(int) = &QComboBox::currentIndexChanged;
    connect(device.typeDevice, signal, this, &Bsp::updateCompatibility);

    setSpinBoxValue(device.versionBspMcu, 0x203);
    setSpinBoxValue(device.versionBspDsp, 0x330D);
    setSpinBoxValue(device.versionBspDspPlis, 0x3333);
    setSpinBoxValue(device.versionPiMcu, 0x151);
    setSpinBoxValue(device.versionBsk1PrdPlis, 0x25);
    setSpinBoxValue(device.versionBsk2PrdPlis, 0x25);
    setSpinBoxValue(device.versionBsk1PrmPlis, 0x23);
    setSpinBoxValue(device.versionBsk2PrmPlis, 0x23);
    setSpinBoxValue(device.versionBszPlis, 0x52);

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

    setSpinBoxValue(GB_PARAM_NET_ADDRESS, 17);

    setSpinBoxValue(GB_PARAM_PRM_TIME_ON, 5);
    setComboBoxValueBits(GB_PARAM_PRM_COM_BLOCK, 0x01, 1);
    setComboBoxValueBits(GB_PARAM_PRM_COM_BLOCK, 0x02, 2);
    setComboBoxValueBits(GB_PARAM_PRM_COM_BLOCK, 0x03, 3);
    setComboBoxValueBits(GB_PARAM_PRM_COM_BLOCK, 0x04, 4);

    param = GB_PARAM_PRM_TIME_OFF;
    for (uint8_t i = 1; i <= getAbsMaxNumOfSameParams(param); i++)
    {
        qint16 value = getAbsMin(param) + i * getDisc(param);
        value        = value % getAbsMax(param);
        setSpinBoxValue(GB_PARAM_PRM_TIME_OFF, value / getFract(param), i);
    }

    setComboBoxValue(device.isDef, 1);
    setSpinBoxValue(GB_PARAM_PRM_COM_NUMS, 1);  // ���������� ������ = num*4
    setSpinBoxValue(GB_PARAM_PRD_COM_NUMS, 1);  // ���������� ������ = num*4
    setComboBoxValue(GB_PARAM_NUM_OF_DEVICES, GB_NUM_DEVICES_2);
    setComboBoxValue(device.typeLine, GB_TYPE_LINE_UM);
    setComboBoxValue(device.typeDevice, AVANT_RZSK);
    setComboBoxValue(device.typeOpto, TYPE_OPTO_STANDART);
    setComboBoxValue(GB_PARAM_COMP_RZSK, GB_COMP_RZSK_M);

    setComboBoxValue(GB_PARAM_DEF_ONE_SIDE, 0);

    setSpinBoxValue(m_measure.R, 999);
    setSpinBoxValue(m_measure.I, 101);
    setSpinBoxValue(m_measure.U, 251);
    setSpinBoxValue(m_measure.Udef1, -32);
    setSpinBoxValue(m_measure.Udef2, 32);
    setSpinBoxValue(m_measure.Ucf1, -16);
    setSpinBoxValue(m_measure.Ucf2, 16);
    setSpinBoxValue(m_measure.Un1, -7);
    setSpinBoxValue(m_measure.Un2, 7);
    setSpinBoxValue(m_measure.Sd, 321);
    setSpinBoxValue(m_measure.T, 24);
    setSpinBoxValue(m_measure.dF, 3);
}

void Bsp::initClock()
{
    if (dt == nullptr)
    {
        dt  = new QDateTime();
        *dt = QDateTime::currentDateTime();

        QTimer *timerClock = new QTimer();
        QObject::connect(timerClock, &QTimer::timeout, &Bsp::updateClock);
        timerClock->start(1000);
    }
}

//
uint8_t Bsp::calcCrc(QVector<uint8_t> &pkg)
{
    uint8_t crc = 0;

    for (uint8_t byte : pkg)
    {
        crc += byte;
    }

    return crc;
}

//
bool Bsp::checkPkg(QVector<uint8_t> &pkg, eGB_COM &com)
{
    uint8_t byte;

    byte = pkg.takeFirst();
    if (byte != 0x55)
    {
        return false;
    }

    byte = pkg.takeFirst();
    if (byte != 0xAA)
    {
        return false;
    }

    byte = pkg.takeLast();
    if (byte != calcCrc(pkg))
    {
        return false;
    }

    com = static_cast<eGB_COM>(pkg.takeFirst());

    byte = pkg.takeFirst();
    if (byte != pkg.size())
    {
        return false;
    }

    return true;
}

void Bsp::crtTreeDef()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    insertTopLevelItem(topLevelItemCount(), top);

    top->setText(0, codec->toUnicode("������"));
    crtComboBox(GB_PARAM_DEF_ONE_SIDE);

    top->setExpanded(false);
}

void Bsp::crtTreeDevice()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    insertTopLevelItem(topLevelItemCount(), top);
    top->setText(0, codec->toUnicode("����������"));

    QTreeWidgetItem *item = nullptr;

    item              = new QTreeWidgetItem();
    device.typeDevice = new QComboBox();
    item->setText(0, codec->toUnicode("��� ��������"));
    top->addChild(item);
    fillComboboxListTypeDevice(device.typeDevice);
    setItemWidget(item, 1, device.typeDevice);

    item            = new QTreeWidgetItem();
    device.typeLine = new QComboBox();
    item->setText(0, codec->toUnicode("��� �����"));
    top->addChild(item);
    fillComboboxListTypeLine(device.typeLine);
    setItemWidget(item, 1, device.typeLine);

    item         = new QTreeWidgetItem();
    device.isDef = new QComboBox();
    item->setText(0, codec->toUnicode("������"));
    top->addChild(item);
    fillComboboxListOnOff(device.isDef);
    setItemWidget(item, 1, device.isDef);

    crtSpinBox(GB_PARAM_PRM_COM_NUMS);
    crtSpinBox(GB_PARAM_PRD_COM_NUMS);
    crtComboBox(GB_PARAM_NUM_OF_DEVICES);

    crtComboBox(GB_PARAM_COMP_K400);
    crtComboBox(GB_PARAM_COMP_P400);
    crtComboBox(GB_PARAM_COMP_RZSK);

    item            = new QTreeWidgetItem();
    device.typeOpto = new QComboBox();
    item->setText(0, codec->toUnicode("��� ������"));
    top->addChild(item);
    fillComboboxListTypeOpto(device.typeOpto);
    setItemWidget(item, 1, device.typeOpto);

    top->setExpanded(true);

    crtTreeDevieVersions(top);
}

void Bsp::crtTreeDevieVersions(QTreeWidgetItem *top)
{
    QTreeWidgetItem *branch = new QTreeWidgetItem();
    top->addChild(branch);
    branch->setText(0, codec->toUnicode("������ ��"));

    QTreeWidgetItem *item = nullptr;

    item                 = new QTreeWidgetItem();
    device.versionBspMcu = new QSpinBox();
    device.versionBspMcu->setDisplayIntegerBase(16);
    device.versionBspMcu->setRange(0, 0xFFFF);
    item->setText(0, codec->toUnicode("��� ��"));
    branch->addChild(item);
    setItemWidget(item, 1, device.versionBspMcu);

    // \todo ������ DSP � �400 ������� �� 2 ����, ����� (������ ����, ������ ���)
    item                 = new QTreeWidgetItem();
    device.versionBspDsp = new QSpinBox();
    device.versionBspDsp->setDisplayIntegerBase(16);
    device.versionBspDsp->setRange(0, 0xFFFF);
    item->setText(0, codec->toUnicode("��� ���"));
    branch->addChild(item);
    setItemWidget(item, 1, device.versionBspDsp);

    // \todo ������ ���� �������� ���������� ������ � �400
    item                     = new QTreeWidgetItem();
    device.versionBspDspPlis = new QSpinBox();
    device.versionBspDspPlis->setDisplayIntegerBase(16);
    device.versionBspDspPlis->setRange(0, 0xFFFF);
    item->setText(0, codec->toUnicode("��� ����"));
    branch->addChild(item);
    setItemWidget(item, 1, device.versionBspDspPlis);

    item                = new QTreeWidgetItem();
    device.versionPiMcu = new QSpinBox();
    device.versionPiMcu->setDisplayIntegerBase(16);
    device.versionPiMcu->setRange(0, 0xFFFF);
    item->setText(0, codec->toUnicode("�� ��"));
    branch->addChild(item);
    setItemWidget(item, 1, device.versionPiMcu);

    item                      = new QTreeWidgetItem();
    device.versionBsk1PrdPlis = new QSpinBox();
    device.versionBsk1PrdPlis->setDisplayIntegerBase(16);
    device.versionBsk1PrdPlis->setRange(0, 0xFF);
    item->setText(0, codec->toUnicode("��� ���1"));
    branch->addChild(item);
    setItemWidget(item, 1, device.versionBsk1PrdPlis);

    item                      = new QTreeWidgetItem();
    device.versionBsk2PrdPlis = new QSpinBox();
    device.versionBsk2PrdPlis->setDisplayIntegerBase(16);
    device.versionBsk2PrdPlis->setRange(0, 0xFF);
    item->setText(0, codec->toUnicode("��� ���2"));
    branch->addChild(item);
    setItemWidget(item, 1, device.versionBsk2PrdPlis);

    item                      = new QTreeWidgetItem();
    device.versionBsk1PrmPlis = new QSpinBox();
    device.versionBsk1PrmPlis->setDisplayIntegerBase(16);
    device.versionBsk1PrmPlis->setRange(0, 0xFF);
    item->setText(0, codec->toUnicode("��� ���1"));
    branch->addChild(item);
    setItemWidget(item, 1, device.versionBsk1PrmPlis);

    item                      = new QTreeWidgetItem();
    device.versionBsk2PrmPlis = new QSpinBox();
    device.versionBsk2PrmPlis->setDisplayIntegerBase(16);
    device.versionBsk2PrmPlis->setRange(0, 0xFF);
    item->setText(0, codec->toUnicode("��� ���2"));
    branch->addChild(item);
    setItemWidget(item, 1, device.versionBsk2PrmPlis);

    item                  = new QTreeWidgetItem();
    device.versionBszPlis = new QSpinBox();
    device.versionBszPlis->setDisplayIntegerBase(16);
    device.versionBszPlis->setRange(0, 0xFF);
    item->setText(0, codec->toUnicode("��� ����"));
    branch->addChild(item);
    setItemWidget(item, 1, device.versionBszPlis);

    branch->setExpanded(false);
}

void Bsp::crtTreeGlb()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    insertTopLevelItem(topLevelItemCount(), top);

    top->setText(0, codec->toUnicode("�����"));

    crtSpinBox(GB_PARAM_NUM_OF_DEVICE);
    crtComboBox(GB_PARAM_COM_PRD_KEEP);
    crtComboBox(GB_PARAM_COM_PRM_KEEP);
    crtComboBox(GB_PARAM_TIME_SYNCH);

    top->setExpanded(false);
}

void Bsp::crtTreeInterface()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    insertTopLevelItem(topLevelItemCount(), top);

    top->setText(0, codec->toUnicode("���������"));

    crtSpinBox(GB_PARAM_NET_ADDRESS);

    top->setExpanded(false);
}

void Bsp::crtTreeMeasure()
{
    QTreeWidgetItem *item   = nullptr;
    QSpinBox *       widget = nullptr;

    QTreeWidgetItem *top = new QTreeWidgetItem();
    insertTopLevelItem(topLevelItemCount(), top);

    // \todo ������� ��������� � ������ �����������

    top->setText(0, codec->toUnicode("���������"));

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(0, 999);
    widget->setToolTip(QString("[%1, %2] Ohm").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, codec->toUnicode("R"));
    top->addChild(item);
    setItemWidget(item, 1, widget);
    m_measure.R = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(0, 999);
    widget->setToolTip(QString("[%1, %2] mA").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, codec->toUnicode("I"));
    top->addChild(item);
    setItemWidget(item, 1, widget);
    m_measure.I = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(0, 999);
    widget->setToolTip(QString("[%1, %2] * 0.1V").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, codec->toUnicode("U"));
    top->addChild(item);
    setItemWidget(item, 1, widget);
    m_measure.U = widget;

    // \todo � �400 ������ U�1 ���������� D �� ���������� �� -64 �� 64.

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(-99, 99);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, codec->toUnicode("U�1 / D"));
    top->addChild(item);
    setItemWidget(item, 1, widget);
    m_measure.Udef1 = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(-99, 99);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, codec->toUnicode("U�2"));
    top->addChild(item);
    setItemWidget(item, 1, widget);
    m_measure.Udef2 = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(-99, 99);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, codec->toUnicode("U�1"));
    top->addChild(item);
    setItemWidget(item, 1, widget);
    m_measure.Ucf1 = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(-99, 99);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, codec->toUnicode("U�2"));
    top->addChild(item);
    setItemWidget(item, 1, widget);
    m_measure.Ucf2 = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(-99, 99);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, codec->toUnicode("U�1"));
    top->addChild(item);
    setItemWidget(item, 1, widget);
    m_measure.Un1 = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(-99, 99);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, codec->toUnicode("U�2"));
    top->addChild(item);
    setItemWidget(item, 1, widget);
    m_measure.Un2 = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(0, 360);
    widget->setToolTip(QString("[%1, %2] deg").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, codec->toUnicode("S�"));
    top->addChild(item);
    setItemWidget(item, 1, widget);
    m_measure.Sd = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(0, 100);
    widget->setToolTip(QString("[%1, %2] deg").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, codec->toUnicode("T"));
    top->addChild(item);
    setItemWidget(item, 1, widget);
    m_measure.T = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(0, 100);
    widget->setToolTip(QString("[%1, %2] Hz").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, codec->toUnicode("dF"));
    top->addChild(item);
    setItemWidget(item, 1, widget);
    m_measure.dF = widget;

    top->setExpanded(false);
}

void Bsp::crtTreePrd()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    insertTopLevelItem(topLevelItemCount(), top);

    top->setText(0, codec->toUnicode("����������"));

    crtSpinBox(GB_PARAM_PRD_IN_DELAY);
    //    crtComboBox(GB_PARAM_PRD_COM_BLOCK);
    //    crtSpinBox(GB_PARAM_PRD_DURATION_L);
    crtSpinBox(GB_PARAM_PRD_DURATION_O);
    crtComboBox(GB_PARAM_PRD_COM_LONG);
    crtComboBox(GB_PARAM_PRD_COM_SIGNAL);

    top->setExpanded(false);
}

void Bsp::crtTreePrm()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    insertTopLevelItem(topLevelItemCount(), top);

    top->setText(0, codec->toUnicode("��������"));

    // FIXME ���� ��� ���� �������� �� �������� �������!
    crtSpinBox(GB_PARAM_PRM_TIME_ON);
    //    crtComboBox(GB_PARAM_PRM_COM_BLOCK_ALL);
    crtComboBox(GB_PARAM_PRM_COM_BLOCK);
    crtSpinBox(GB_PARAM_PRM_TIME_OFF);
    crtComboBox(GB_PARAM_PRD_DR_ENABLE);
    crtComboBox(GB_PARAM_PRM_DR_COM_BLOCK);
    crtComboBox(GB_PARAM_PRM_DR_COM_TO_HF);
    crtComboBox(GB_PARAM_PRM_COM_SIGNAL);

    top->setExpanded(false);
}

//
void Bsp::crtTreeState()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();

    insertTopLevelItem(topLevelItemCount(), top);
    top->setText(0, codec->toUnicode("������� ���������"));

    errRegExp.setPattern("[0-9A-Fa-F]+");
    errValidator.setRegExp(errRegExp);

    crtTreeState(top, "�����", stateGlb);
    crtTreeState(top, "������", stateDef);
    crtTreeState(top, "��������", statePrm);
    crtTreeState(top, "����������", statePrd);

    connect(stateGlb.regime,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &Bsp::setRegime);

    connect(stateGlb.state, QOverload<int>::of(&QSpinBox::valueChanged), this, &Bsp::setState);

    connect(stateGlb.dopByte, QOverload<int>::of(&QSpinBox::valueChanged), this, &Bsp::setDopByte);

    top->setExpanded(true);
}

//
void Bsp::crtTreeState(QTreeWidgetItem *top, std::string name, Bsp::state_t &state)
{

    QTreeWidgetItem *item = nullptr;
    QTreeWidgetItem *ltop = new QTreeWidgetItem();

    ltop->setText(0, codec->toUnicode(name.c_str()));
    top->addChild(ltop);

    item = new QTreeWidgetItem();
    item->setText(0, codec->toUnicode("�����"));
    ltop->addChild(item);
    state.regime = new QComboBox();
    fillComboboxListRegime(state.regime);
    setItemWidget(item, 1, state.regime);

    item = new QTreeWidgetItem();
    item->setText(0, codec->toUnicode("���������"));
    ltop->addChild(item);
    state.state = new QSpinBox();
    state.state->setRange(0, MAX_NUM_DEVICE_STATE);
    state.state->setValue(0);
    setItemWidget(item, 1, state.state);

    item = new QTreeWidgetItem();
    item->setText(0, codec->toUnicode("���. ����"));
    ltop->addChild(item);
    state.dopByte = new QSpinBox();
    state.dopByte->setRange(0, 255);
    state.dopByte->setValue(0);
    setItemWidget(item, 1, state.dopByte);

    item = new QTreeWidgetItem();
    item->setText(0, codec->toUnicode("�������������"));
    ltop->addChild(item);
    state.fault = new QLineEdit();
    state.fault->setMaxLength(4);
    state.fault->setInputMask("HHHH");
    state.fault->setValidator(&errValidator);
    state.fault->setText("0000");
    setItemWidget(item, 1, state.fault);

    connect(state.fault, &QLineEdit::selectionChanged, state.fault, &QLineEdit::deselect);

    item = new QTreeWidgetItem();
    item->setText(0, codec->toUnicode("��������������"));
    ltop->addChild(item);
    state.warning = new QLineEdit();
    state.warning->setMaxLength(4);
    state.warning->setInputMask("HHHH");
    state.warning->setValidator(&errValidator);
    state.warning->setText("0000");
    setItemWidget(item, 1, state.warning);

    connect(state.warning, &QLineEdit::selectionChanged, state.warning, &QLineEdit::deselect);

    ltop->setExpanded(&state == &stateGlb);
    //    ltop->setExpanded(true);
}

//
void Bsp::crtTest()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    QTreeWidgetItem *item;
    QLineEdit *      lineedit;
    insertTopLevelItem(topLevelItemCount(), top);
    top->setText(0, codec->toUnicode("����"));

    item       = new QTreeWidgetItem();
    lineedit   = new QLineEdit();
    test.byte1 = lineedit;
    lineedit->setMaxLength(2);
    lineedit->setInputMask("HH");
    lineedit->setText("00");
    top->addChild(item);
    item->setText(0, codec->toUnicode("���� 1"));
    setItemWidget(item, 1, lineedit);

    item       = new QTreeWidgetItem();
    lineedit   = new QLineEdit();
    test.byte2 = lineedit;
    lineedit->setMaxLength(2);
    lineedit->setInputMask("HH");
    lineedit->setText("00");
    top->addChild(item);
    item->setText(0, codec->toUnicode("���� 2"));
    setItemWidget(item, 1, lineedit);

    item       = new QTreeWidgetItem();
    lineedit   = new QLineEdit();
    test.byte3 = lineedit;
    lineedit->setMaxLength(2);
    lineedit->setInputMask("HH");
    lineedit->setText("00");
    top->addChild(item);
    item->setText(0, codec->toUnicode("���� 3"));
    setItemWidget(item, 1, lineedit);

    item       = new QTreeWidgetItem();
    lineedit   = new QLineEdit();
    test.byte4 = lineedit;
    lineedit->setMaxLength(2);
    lineedit->setInputMask("HH");
    lineedit->setText("00");
    top->addChild(item);
    item->setText(0, codec->toUnicode("���� 4"));
    setItemWidget(item, 1, lineedit);

    item       = new QTreeWidgetItem();
    lineedit   = new QLineEdit();
    test.byte5 = lineedit;
    lineedit->setMaxLength(2);
    lineedit->setInputMask("HH");
    lineedit->setText("00");
    top->addChild(item);
    item->setText(0, codec->toUnicode("���� 5"));
    setItemWidget(item, 1, lineedit);

    top->setExpanded(false);
}

//
void Bsp::crtComboBox(eGB_PARAM param)
{
    QVector<QComboBox *> vcombobox;
    QComboBox *          combobox;
    QTreeWidgetItem *    item;
    QTreeWidgetItem *    top = topLevelItem(topLevelItemCount() - 1);

    uint8_t num = getAbsMaxNumOfSameParams(param);
    if (num == 0)
    {
        qCritical() << QString("Number of parameters %1 is 0!").arg(getParamName(param));
    }
    else
    {
        if (num > 1)
        {
            item = new QTreeWidgetItem();
            item->setText(0, getParamName(param));
            top->addChild(item);
            top = item;
        }

        for (uint8_t i = 1; i <= num; i++)
        {
            item = new QTreeWidgetItem();

            if (num > 1)
            {
                std::string name("����� ");
                name += std::to_string(i);
                item->setText(0, codec->toUnicode(name.c_str()));
            }
            else
            {
                item->setText(0, getParamName(param));
            }

            combobox = new QComboBox(this);
            vcombobox.append(combobox);

            if (getListOfValues(param) != nullptr)
            {
                fillComboboxList(combobox, param);
                combobox->setCurrentIndex(0);
            }
            else
            {
                qCritical() << QString("Parameter %1 is not LIST!").arg(getParamName(param));
            }

            if (getCom(param) == eGB_COM::GB_COM_NO)
            {
                QPalette pa;
                pa.setColor(QPalette::WindowText, Qt::red);
                item->setForeground(0, Qt::blue);
            }

            top->addChild(item);
            setItemWidget(item, 1, combobox);
        }

        mapCombobox.insert(param, vcombobox);
    }
}

//
void Bsp::fillComboboxList(QComboBox *combobox, eGB_PARAM param)
{
    if (param < GB_PARAM_MAX)
    {
        if (getListOfValues(param) != nullptr)
        {
            char const *l = getListOfValues(param);
            for (quint8 i = 0; i < getAbsMax(param); i++)
            {
                combobox->addItem(codec->toUnicode(l), getAbsMin(param) + i);
                l += VALUE_STRING_SIZE;
            }
        }
    }
}

//
void Bsp::fillComboboxListOnOff(QComboBox *combobox)
{
    if (combobox != nullptr)
    {
        for (uint8_t i = 0; i < SIZE_OF(fcOnOff); i++)
        {
            combobox->addItem(codec->toUnicode(fcOnOff[i]), i);
        }
    }
}

//
void Bsp::fillComboboxListRegime(QComboBox *combobox)
{
    if (combobox != nullptr)
    {
        for (uint8_t i = 0; i < eGB_REGIME::GB_REGIME_MAX; i++)
        {
            combobox->addItem(codec->toUnicode(fcRegime[i]), i);
        }
    }
}

//
void Bsp::fillComboboxListTypeDevice(QComboBox *combobox)
{
    if (combobox != nullptr)
    {
        combobox->addItem(codec->toUnicode("�400"), AVANT_R400);
        combobox->addItem(codec->toUnicode("����"), AVANT_RZSK);
        combobox->addItem(codec->toUnicode("�400"), AVANT_K400);
        combobox->addItem(codec->toUnicode("�400�"), AVANT_R400M);
    }
}

//
void Bsp::fillComboboxListTypeLine(QComboBox *combobox)
{
    if (combobox != nullptr)
    {
        combobox->addItem(codec->toUnicode("��"), GB_TYPE_LINE_UM);
        combobox->addItem(codec->toUnicode("����"), GB_TYPE_LINE_OPTO);
        combobox->addItem(codec->toUnicode("�1"), GB_TYPE_LINE_E1);
    }
}

//
void Bsp::fillComboboxListTypeOpto(QComboBox *combobox)
{
    if (combobox != nullptr)
    {
        combobox->addItem(codec->toUnicode("�����������"), TYPE_OPTO_STANDART);
        combobox->addItem(codec->toUnicode("������"), TYPE_OPTO_RING_UNI);
        combobox->addItem(codec->toUnicode("�������. ������"), TYPE_OPTO_RING_BI);
    }
}

//
void Bsp::crtLineEdit(eGB_PARAM param, std::string value)
{
    QLineEdit *      lineedit = new QLineEdit(this);
    QTreeWidgetItem *item     = new QTreeWidgetItem();
    QTreeWidgetItem *top      = topLevelItem(topLevelItemCount() - 1);

    // TODO ������� ����������� ����� �������� ������
    Q_ASSERT(false);

    //    if (getParamType(param) == Param::PARAM_PWD) {
    //        lineedit->setMaxLength(PWD_LEN);
    //        lineedit->setInputMask("99999999");
    //        lineedit->setValidator(&pwdValidator);;
    //    } else {
    //        qCritical() << QString("Parameter %1 is not PWD!").
    //                       arg(getParamName(param));
    //    }

    if (getCom(param) == eGB_COM::GB_COM_NO)
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText, Qt::red);
        item->setForeground(0, Qt::blue);
    }

    top->addChild(item);
    item->setText(0, getParamName(param));
    setItemWidget(item, 1, lineedit);

    mapLineEdit.insert(param, lineedit);
}

//
void Bsp::crtSpinBox(eGB_PARAM param)
{
    QVector<QSpinBox *> vspinbox;
    QSpinBox *          spinbox;
    QTreeWidgetItem *   item;
    QTreeWidgetItem *   top = topLevelItem(topLevelItemCount() - 1);

    uint8_t num = getAbsMaxNumOfSameParams(param);
    if (num == 0)
    {
        qCritical() << QString("Number of parameters %1 is 0!").arg(getParamName(param));
    }
    else
    {
        if (num > 1)
        {
            item = new QTreeWidgetItem();
            item->setText(0, getParamName(param));
            top->addChild(item);
            top = item;
        }

        for (uint8_t i = 1; i <= num; i++)
        {
            item = new QTreeWidgetItem();

            if (num > 1)
            {
                std::string name("����� ");
                name += std::to_string(i);
                item->setText(0, codec->toUnicode(name.c_str()));
            }
            else
            {
                item->setText(0, getParamName(param));
            }

            spinbox = new QSpinBox(this);
            vspinbox.append(spinbox);

            if (getParamType(param) == Param::PARAM_INT)
            {
                qint16 min = getAbsMin(param);
                qint16 max = getAbsMax(param);
                spinbox->setRange(min, max);
                spinbox->setSingleStep(getDisc(param));
                spinbox->setValue(getAbsMin(param));
                spinbox->setToolTip(QString("%1 - %2").arg(min).arg(max));
            }
            else
            {
                qCritical() << QString("Parameter %1 is not INT!").arg(getParamName(param));
            }

            if (getCom(param) == eGB_COM::GB_COM_NO)
            {
                QPalette pa;
                pa.setColor(QPalette::WindowText, Qt::red);
                item->setForeground(0, Qt::blue);
            }

            top->addChild(item);
            setItemWidget(item, 1, spinbox);
        }

        mapSpinBox.insert(param, vspinbox);
    }
}


//
quint8 Bsp::getComboBoxValue(eGB_PARAM param, uint8_t number)
{
    quint8 value = 0;

    if (number > 0 && mapCombobox.contains(param) && mapCombobox.value(param).size() >= number)
    {

        number -= 1;
        if (getParamType(param) == Param::PARAM_LIST)
        {
            value = getComboBoxValue(mapCombobox.value(param).at(number));
        }
        else if (getParamType(param) == Param::PARAM_BITES)
        {
            Q_ASSERT((number + CHAR_BIT) <= mapCombobox.value(param).size());

            for (uint8_t i = 0; i < CHAR_BIT; i++)
            {
                QComboBox *box = mapCombobox.value(param).at(number + i);
                value += box->currentData() > 0 ? (1 << i) : 0;
            }
        }
    }
    else
    {
        QString msg = QString("%1: Parameter '%2' (%3) not found.")
                          .arg(__FUNCTION__)
                          .arg(getParamName(param))
                          .arg(number);
        qWarning() << msg;
    }

    return value;
}

//
quint8 Bsp::getComboBoxValue(QComboBox *combobox)
{
    return static_cast<quint8>(combobox->currentData().toInt());
}

//
void Bsp::setComboBoxValue(eGB_PARAM param, quint8 value, uint8_t number)
{
    QComboBox *combobox = nullptr;

    if ((number > 0) && mapCombobox.contains(param) && (mapCombobox.value(param).size() >= number))
    {

        combobox = mapCombobox.value(param).at(number - 1);
        if (setComboBoxValue(combobox, value) == -1)
        {
            QString msg = QString("%1: Wrong value %2 for parameter '%3' (%4).")
                              .arg(__FUNCTION__)
                              .arg(value)
                              .arg(param)
                              .arg(number);
            qWarning() << msg;
        }
    }
    else
    {
        QString msg =
            QString("%1: Parameter '%2' (%3) not found.").arg(__FUNCTION__).arg(param).arg(number);
        qWarning() << msg;
    }
}

void Bsp::setComboBoxValueBits(eGB_PARAM param, quint8 value, uint8_t number)
{
    if ((number > 0) && (number * 8 <= getAbsMaxNumOfSameParams(param)))
    {
        uint8_t start = (number - 1) * CHAR_BIT;
        for (uint8_t i = 0; i < CHAR_BIT; i++)
        {
            uint8_t v = (value & (1 << i)) ? 1 : 0;
            setComboBoxValue(param, v, start + i + 1);
        }
    }
    else
    {
        QString msg = QString("%1: Wrong number for Parameter '%2' (%3).")
                          .arg(__FUNCTION__)
                          .arg(getParamName(param))
                          .arg(number);
        qWarning() << msg;
    }
}

//
int Bsp::setComboBoxValue(QComboBox *combobox, quint8 value)
{
    int index = combobox->findData(value);

    if (index != -1)
    {
        combobox->setCurrentIndex(index);
    }

    return index;
}

//
QString Bsp::getLineEditValue(eGB_PARAM param)
{
    QString    value;
    QLineEdit *lineedit = mapLineEdit.value(param, nullptr);

    if (lineedit != nullptr)
    {
        value = lineedit->text();
    }
    else
    {
        QString msg =
            QString("%1: Parameter '%2' not found.").arg(__FUNCTION__).arg(getParamName(param));
        qWarning() << msg;
    }

    return value;
}

//
QString Bsp::getLineEditValue(QLineEdit *lineedit)
{
    return lineedit->text();
}

//
void Bsp::setLineEditValue(eGB_PARAM param, std::string value)
{
    QLineEdit *lineedit = mapLineEdit.value(param, nullptr);

    if (lineedit != nullptr)
    {
        if (setLineEditValue(lineedit, value) == -1)
        {
            QString msg = QString("%1: Wrong value %2 for parameter '%3'.")
                              .arg(__FUNCTION__)
                              .arg(value.c_str())
                              .arg(getParamName(param));
            qWarning() << msg;
        }
    }
    else
    {
        QString msg =
            QString("%1: Parameter '%2' not found.").arg(__FUNCTION__).arg(getParamName(param));
        qWarning() << msg;
    }
}

//
int Bsp::setLineEditValue(QLineEdit *lineedit, std::string value)
{
    int index = -1;

    lineedit->setText(Bsp::codec->toUnicode(value.c_str()));
    index = 0;
    // FIXME �������� �������� ��������!

    return index;
}


//
qint16 Bsp::getSpinBoxValue(eGB_PARAM param, uint8_t number)
{
    qint16 value = 0;

    if (number > 0 && mapSpinBox.contains(param) && mapSpinBox.value(param).size() >= number)
    {

        number -= 1;
        if (getParamType(param) == Param::PARAM_INT)
        {
            value = getSpinBoxValue(mapSpinBox.value(param).at(number));
            value = (value / getDisc(param)) * getDisc(param);
            value /= getFract(param);
        }
        else
        {
            QString msg = QString("Parameter '%1' type (%2) not found.")
                              .arg(getParamName(param))
                              .arg(getParamType(param));
            qWarning() << msg;
        }
    }
    else
    {
        QString msg =
            QString("Parameter '%1' (%2) not found.").arg(getParamName(param)).arg(number);
        qWarning() << msg;
    }

    return value;
}

//
qint16 Bsp::getSpinBoxValue(QSpinBox *spinbox)
{
    return static_cast<qint16>(spinbox->value());
}

//
void Bsp::setSpinBoxValue(eGB_PARAM param, qint16 value, uint8_t number)
{
    QSpinBox *spinbox = nullptr;

    if ((number > 0) && mapSpinBox.contains(param) && (mapSpinBox.value(param).size() >= number))
    {

        value   = value * getFract(param);
        spinbox = mapSpinBox.value(param).at(number - 1);
        if (setSpinBoxValue(spinbox, value) == -1)
        {
            QString msg = QString("%1: Wrong value %2 for parameter '%3'.")
                              .arg(__FUNCTION__)
                              .arg(value)
                              .arg(param);
            qWarning() << msg;
        }
    }
    else
    {
        QString msg = QString("%1: Parameter '%2' not found.").arg(__FUNCTION__).arg(param);
        qWarning() << msg;
    }
}

//
int Bsp::setSpinBoxValue(QSpinBox *spinbox, qint16 value)
{
    int check = -1;

    if ((value >= spinbox->minimum()) && (value <= spinbox->maximum()))
    {
        spinbox->setValue(value);
        check = 0;
    }

    return check;
}

//
void Bsp::procCommand(eGB_COM com, pkg_t &data)
{
    pkgTx.clear();
    switch (com & GB_COM_MASK_GROUP)
    {
    case GB_COM_MASK_GROUP_READ_PARAM:
        {
            procCommandReadParam(com, data);
        }
        break;
    case GB_COM_MASK_GROUP_WRITE_REGIME:
        {
            procCommandWriteRegime(com, data);
        }
        break;
    case GB_COM_MASK_GROUP_WRITE_PARAM:
        {
            procCommandWriteParam(com, data);
        }
        break;
    case GB_COM_MASK_GROUP_READ_JOURNAL:
        {
            procCommandReadJournal(com, data);
        }
        break;
    }
}

//
QString Bsp::getParamName(eGB_PARAM param)
{
    QString name = "Noname";

    if (param < GB_PARAM_MAX)
    {
        name = codec->toUnicode(getNameOfParam(param));
    }

    return name;
}

//
void Bsp::keyPressEvent(QKeyEvent *event)
{
    return;
}

//
quint8 Bsp::bcd2int(quint8 bcd, bool &ok)
{
    quint8 value = 0;

    ok = ((bcd & 0x0F) < 0x0A) && ((bcd & 0xF0) < 0xA0);
    if (!ok)
    {
        qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << " Error: " << bcd;
    }

    value = bcd & 0x0F;
    value += ((bcd >> 4) & 0x0F) * 10;

    return value;
}

//
quint8 Bsp::int2bcd(quint8 val, bool &ok)
{
    quint8 bcd = 0;

    ok = (val < 100);
    if (!ok)
    {
        qWarning("Can't convert value 0x%.2X to BCD code!", val);
    }

    bcd = (val % 10);
    bcd += (val / 10) << 4;

    return bcd;
}

//
void Bsp::procCommandReadJournal(eGB_COM com, pkg_t &data)
{
    switch (com)
    {
    case GB_COM_DEF_GET_JRN_CNT:
        {
            if (!data.isEmpty())
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            // TODO ���������� ��� ������� ������ ���������� ������� �������.
            pkgTx.append(com);
            pkgTx.append(0);
            pkgTx.append(0);
        }
        break;

    case GB_COM_PRM_GET_JRN_CNT:
        {
            if (!data.isEmpty())
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            // TODO ���������� ��� ������� ������ ���������� ������� �������.
            pkgTx.append(com);
            pkgTx.append(0);
            pkgTx.append(0);
        }
        break;

    case GB_COM_PRD_GET_JRN_CNT:
        {
            if (!data.isEmpty())
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            // TODO ���������� ��� ������� ������ ���������� ������� �������.
            pkgTx.append(com);
            pkgTx.append(0);
            pkgTx.append(0);
        }
        break;

    case GB_COM_GET_JRN_CNT:
        {
            if (!data.isEmpty())
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            // TODO ���������� ��� ������� ������ ���������� ������� �������.
            pkgTx.append(com);
            pkgTx.append(0);
            pkgTx.append(0);
        }
        break;

    default:
        {
            qWarning("No command handler: 0x%.2X", com);
        }
    }
}

//
void Bsp::procCommandReadParam(eGB_COM com, pkg_t &data)
{
    switch (com)
    {
    case GB_COM_NO:
        {
            if (data.size() != 0)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            pkgTx.append(com);
            pkgTx.append(1);
            pkgTx.append(2);
            pkgTx.append(3);
        }
        break;

    case GB_COM_DEF_GET_LINE_TYPE:
        {
            if (data.size() != 0)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            // FIXME �������� ��������
            pkgTx.append(getComboBoxValue(GB_PARAM_NUM_OF_DEVICES));
        }
        break;

    case GB_COM_PRM_GET_TIME_ON:
        {
            if (data.size() != 0)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            // FIXME ���� ��� ������ ��������� "�������� �� �������� �������"
            pkgTx.append(Bsp::getSpinBoxValue(GB_PARAM_PRM_TIME_ON));
        }
        break;

    case GB_COM_PRM_GET_TIME_OFF:
        {
            if (data.size() != 0)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            // FIXME ������� ����������� �� ���������� ������.
            uint8_t number = getAbsMaxNumOfSameParams(GB_PARAM_PRM_TIME_OFF);
            for (uint8_t i = 1; i <= number; i++)
            {
                qint16 value = Bsp::getSpinBoxValue(GB_PARAM_PRM_TIME_OFF, i);
                pkgTx.append(static_cast<quint8>(value));
            }
        }
        break;

    case GB_COM_PRM_GET_BLOCK_COM:
        {
            if (data.size() != 0)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRM_COM_BLOCK, 1));
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRM_COM_BLOCK, 9));
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRM_COM_BLOCK, 17));
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRM_COM_BLOCK, 25));
        }
        break;

    case GB_COM_PRD_GET_TIME_ON:
        {
            if (data.size() != 0)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            pkgTx.append(Bsp::getSpinBoxValue(GB_PARAM_PRD_IN_DELAY));
        }
        break;

    case GB_COM_PRD_GET_DURATION:
        {
            if (data.size() != 0)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            // FIXME ��� ������ ��������� ��� ������ / ��
            pkgTx.append(Bsp::getSpinBoxValue(GB_PARAM_PRD_DURATION_O));
        }
        break;

    case GB_COM_PRD_GET_BLOCK_COM:
        {
            if (data.size() != 0)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_BLOCK, 1));
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_BLOCK, 9));
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_BLOCK, 17));
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_BLOCK, 25));
        }
        break;

    case GB_COM_PRD_GET_LONG_COM:
        {
            if (data.size() != 0)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_LONG, 1));
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_LONG, 9));
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_LONG, 17));
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_LONG, 25));
        }
        break;

    case GB_COM_PRD_GET_COM_SIGN:
        {
            if (data.size() != 0)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_SIGNAL, 1));
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_SIGNAL, 9));
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_SIGNAL, 17));
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_SIGNAL, 25));
        }
        break;

    case GB_COM_GET_SOST:
        {
            if (data.size() != 1)
            {
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
            // TODO ����������� ����� ����� ������ ��������
            pkgTx.append(getComboBoxValue(statePrm.regime));
            pkgTx.append(getSpinBoxValue(statePrd.state));
            pkgTx.append(getSpinBoxValue(statePrd.dopByte));
            // FIXME �������� ��������� ���������
        }
        break;

    case GB_COM_GET_FAULT:
        {
            bool ok    = false;
            uint value = 0;
            if (!data.isEmpty())
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            // FIXME ���� �� ���������� �������� ���� ����� �������������� ��
            // ��������� �� ����� "��������������", ��� ���� ��� �� ��������.
            pkgTx.append(com);
            value = getLineEditValue(stateDef.fault).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t>(value >> 8));
            pkgTx.append(static_cast<uint8_t>(value));
            value = getLineEditValue(stateDef.warning).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t>(value >> 8));
            pkgTx.append(static_cast<uint8_t>(value));
            value = getLineEditValue(statePrm.fault).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t>(value >> 8));
            pkgTx.append(static_cast<uint8_t>(value));
            value = getLineEditValue(statePrm.warning).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t>(value >> 8));
            pkgTx.append(static_cast<uint8_t>(value));
            value = getLineEditValue(statePrd.fault).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t>(value >> 8));
            pkgTx.append(static_cast<uint8_t>(value));
            value = getLineEditValue(statePrd.warning).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t>(value >> 8));
            pkgTx.append(static_cast<uint8_t>(value));
            value = getLineEditValue(stateGlb.fault).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t>(value >> 8));
            pkgTx.append(static_cast<uint8_t>(value));
            value = getLineEditValue(stateGlb.warning).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t>(value >> 8));
            pkgTx.append(static_cast<uint8_t>(value));

            // TODO ����������� ����� ����� ������ ��������
            value = getLineEditValue(statePrm.fault).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t>(value >> 8));
            pkgTx.append(static_cast<uint8_t>(value));
            value = getLineEditValue(statePrm.warning).toUInt(&ok, 16);
            pkgTx.append(static_cast<uint8_t>(value >> 8));
            pkgTx.append(static_cast<uint8_t>(value));

            // TODO �������� ����� ������ ��� ����
        }
        break;

    case GB_COM_GET_TIME:
        {
            // TODO �������� ��������� ����� ����� ������ �������
            if (data.size() != 1)
            {
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
            pkgTx.append(static_cast<uint8_t>(value));
            pkgTx.append(static_cast<uint8_t>(value >> 8));

            // TODO �������� ���������� ������ ������� ��� ��� ��.
        }
        break;

    case GB_COM_GET_TIME_SINCHR:
        {
            if (data.size() != 0)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_TIME_SYNCH));
            // TODO � ���� ������ ���� "��� ���������"
        }
        break;

    case GB_COM_GET_COM_PRM_KEEP:
        {
            if (data.size() != 0)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);
            // FIXME ����� ���� ��� U��� �����������!
            pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_COM_PRM_KEEP));
        }
        break;

    case GB_COM_GET_COM_PRD_KEEP:
        {
            if (data.size() != 0)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            pkgTx.append(com);

            uint8_t         value      = getComboBoxValue(device.typeDevice);
            eGB_TYPE_DEVICE typedevice = static_cast<eGB_TYPE_DEVICE>(value);

            if (typedevice == AVANT_K400)
            {
                // FIXME �������� ��������� ��������� ��� �400.
                pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_COM_PRD_KEEP));
                pkgTx.append(getCompatibility(typedevice));
                pkgTx.append(0);
                pkgTx.append(0);
                pkgTx.append(0);
                pkgTx.append(0);
                pkgTx.append(0);
                pkgTx.append(0);
                pkgTx.append(0);
                pkgTx.append(0);
                pkgTx.append(0);
                pkgTx.append(0);
                pkgTx.append(0);
                pkgTx.append(0);
                pkgTx.append(0);
                pkgTx.append(0);
                pkgTx.append(0);
                pkgTx.append(0);
                pkgTx.append(0);
                pkgTx.append(0);
                pkgTx.append(0);
                pkgTx.append(0);
            }
            else if (typedevice == AVANT_RZSK)
            {
                pkgTx.append(Bsp::getComboBoxValue(GB_PARAM_COM_PRD_KEEP));
                pkgTx.append(getCompatibility(typedevice));
            }
            else
            {
                pkgTx.append(getCompatibility(typedevice));
            }
        }
        break;

    case GB_COM_GET_NET_ADR:
        {
            hdlrComNetAdrGet(com, data);
        }
        break;

    case GB_COM_GET_TEST:
        {
            if (!data.isEmpty())
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            // TODO ������� ����� ��� ���� ����� ���������.
            hdlrComGetTest(com, data);
        }
        break;

    case GB_COM_GET_VERS: hdlrComGetVers(com, data); break;
    case GB_COM_GET_DEVICE_NUM: hdlrComDeviceNumGet(com, data); break;
    case GB_COM_DEF_GET_TYPE_AC: hdlrComDefTypeAcGet(com, data); break;
    case GB_COM_GET_MEAS: hdlrComMeasGet(com, data); break;

    default:
        {
            qWarning("No command handler: 0x%.2X", com);
        }
    }
}

//
void Bsp::procCommandWriteParam(eGB_COM com, pkg_t &data)
{
    pkgTx.append(com);
    pkgTx.append(data);

    switch (com)
    {
    case GB_COM_PRM_SET_TIME_ON:
        {
            if (data.size() != 1)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            else
            {
                uint8_t value = data.takeFirst();
                // FIXME ���� ��� ������ ��������� "�������� �� �������� �������"
                Bsp::setSpinBoxValue(GB_PARAM_PRM_TIME_ON, value);
            }
        }
        break;

    case GB_COM_PRM_SET_TIME_OFF:
        {
            if (data.size() != 2)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            else
            {
                uint8_t number = data.takeFirst();
                uint8_t value  = data.takeFirst();
                // FIXME ���� ��� ������ ��������� "�������� �� �������� �������"
                Bsp::setSpinBoxValue(GB_PARAM_PRM_TIME_OFF, value, number);
            }
        }
        break;

    case GB_COM_PRM_SET_BLOCK_COM:
        {
            if (data.size() != 2)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            else
            {
                uint8_t number = data.takeFirst();
                uint8_t value  = data.takeFirst();
                // FIXME ���� ��� ������ ��������� "�������� �� �������� �������"
                Bsp::setComboBoxValueBits(GB_PARAM_PRM_COM_BLOCK, value, number);
            }
        }
        break;

    case GB_COM_PRD_SET_TIME_ON:
        {
            if (data.size() != 1)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            else
            {
                uint8_t value = data.takeFirst();
                Bsp::setSpinBoxValue(GB_PARAM_PRD_IN_DELAY, value);
            }
        }
        break;

    case GB_COM_PRD_SET_DURATION:
        {
            if (data.size() != 1)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            else
            {
                uint8_t value = data.takeFirst();
                // FIXME ��� ����� ���� ������ �������� ��� �� / ������
                Bsp::setSpinBoxValue(GB_PARAM_PRD_DURATION_O, value);
            }
        }
        break;

    case GB_COM_PRD_SET_BLOCK_COM:
        {
            if (data.size() != 2)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            else
            {
                uint8_t number = data.takeFirst();
                uint8_t value  = data.takeFirst();
                Bsp::setComboBoxValueBits(GB_PARAM_PRD_COM_BLOCK, value, number);
            }
        }
        break;

    case GB_COM_PRD_SET_LONG_COM:
        {
            if (data.size() != 2)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            else
            {
                uint8_t number = data.takeFirst();
                uint8_t value  = data.takeFirst();
                Bsp::setComboBoxValueBits(GB_PARAM_PRD_COM_LONG, value, number);
            }
        }
        break;

    case GB_COM_PRD_SET_COM_SIGN:
        {
            if (data.size() != 2)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            else
            {
                uint8_t number = data.takeFirst();
                uint8_t value  = data.takeFirst();
                Bsp::setComboBoxValueBits(GB_PARAM_PRD_COM_SIGNAL, value, number);
            }
        }
        break;

    case GB_COM_SET_TIME:
        {
            if (data.size() != 9)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            else
            {
                bool    ok    = false;
                quint16 year  = bcd2int(data.takeFirst(), ok) + 2000;
                quint8  month = bcd2int(data.takeFirst(), ok);
                quint8  day   = bcd2int(data.takeFirst(), ok);
                dt->setDate(QDate(year, month, day));

                quint8 hour   = bcd2int(data.takeFirst(), ok);
                quint8 minute = bcd2int(data.takeFirst(), ok);
                quint8 second = bcd2int(data.takeFirst(), ok);
                dt->setTime(QTime(hour, minute, second));

                quint16 ms = data.takeFirst();
                ms += static_cast<quint16>(data.takeFirst()) << 8;
                if (ms != 0)
                {
                    qWarning() << Bsp::codec->toUnicode("Milliseconds is not 0.");
                }

                quint8 source = data.takeFirst();
                if (source != 0)
                {
                    qWarning() << msgTimeSourceError.arg(source);
                }
            }
        }
        break;

    case GB_COM_SET_TIME_SINCHR:
        {
            // TODO � ���� ��� �����, �.�. ����� ���� ��� ���������
            if (data.size() != 2)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            else
            {
                uint8_t value = data.takeFirst();
                Bsp::setComboBoxValue(GB_PARAM_TIME_SYNCH, value);
            }
        }
        break;

    case GB_COM_SET_COM_PRM_KEEP:
        {
            if (data.size() != 1)
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
            else
            {
                uint8_t value = data.takeFirst();
                // FIXME ����� ���� U��� �����������.
                Bsp::setComboBoxValue(GB_PARAM_COM_PRM_KEEP, value);
            }
        }
        break;

    case GB_COM_SET_COM_PRD_KEEP:
        {
            uint8_t value = data.takeFirst();
            uint8_t dop   = data.takeFirst();
            // FIXME �������� ��������� ��������� ��� �400.
            // ������ ��� � �400 ������ ���� ���� "�������������".

            switch (dop)
            {
            case POS_COM_PRD_KEEP_prdKeep:
                {
                    Bsp::setComboBoxValue(GB_PARAM_COM_PRD_KEEP, value);
                    Bsp::setComboBoxValue(GB_PARAM_COMP_P400, value);
                }
                break;
            case POS_COM_PRD_KEEP_compK400:
                {
                    Bsp::setComboBoxValue(GB_PARAM_COMP_K400, value);
                    Bsp::setComboBoxValue(GB_PARAM_COMP_RZSK, value);
                }
                break;

            default: qDebug("No dop byte handler: 0x%.2X", dop);
            }
        }
        break;

    case GB_COM_SET_NET_ADR: hdlrComNetAdrSet(com, data); break;
    case GB_COM_SET_DEVICE_NUM: hdlrComDeviceNumSet(com, data); break;
    case GB_COM_DEF_SET_TYPE_AC: hdlrComDefTypeAcSet(com, data); break;

    default:
        {
            qWarning("No command handler: 0x%.2X", com);
        }
    }
}

//
void Bsp::procCommandWriteRegime(eGB_COM com, pkg_t &data)
{
    switch (com)
    {
    case GB_COM_SET_REG_DISABLED:
        {
            if (!data.isEmpty())
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }

            setComboBoxValue(stateGlb.regime, eGB_REGIME::GB_REGIME_DISABLED);
            pkgTx.append(com);
        }
        break;
    case GB_COM_SET_REG_ENABLED:
        {
            if (!data.isEmpty())
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }

            setComboBoxValue(stateGlb.regime, eGB_REGIME::GB_REGIME_ENABLED);
            pkgTx.append(com);
        }
        break;
    case GB_COM_SET_CONTROL:
        {
            hdlrComSetControl(com, data);
        }
        break;
    case GB_COM_SET_REG_TEST_1:
        {
            // TODO ������� ��� ���� ������
            if (data.isEmpty())
            {
                setComboBoxValue(stateGlb.regime, eGB_REGIME::GB_REGIME_TEST_1);
                pkgTx.append(com);
            }
            else if (data.count() == 2)
            {
                uint8_t byte1 = 0;
                uint8_t byte2 = 0;
                uint8_t byte3 = 0;
                uint8_t byte4 = 0;
                uint8_t byte5 = 0;
                if (data.at(0) == 1)
                {
                    if (data.at(1) == 1)
                    {
                        byte1 = 1;
                    }
                    else if ((data.at(1) >= 3) && (data.at(1) <= 10))
                    {
                        byte2 = (1 << (data.at(1) - 3));
                    }
                    else if ((data.at(1) >= 11) && (data.at(1) <= 18))
                    {
                        byte3 = (1 << (data.at(1) - 11));
                    }
                    else if ((data.at(1) >= 19) && (data.at(1) <= 26))
                    {
                        byte4 = (1 << (data.at(1) - 19));
                    }
                    else if ((data.at(1) >= 27) && (data.at(1) <= 34))
                    {
                        byte5 = (1 << (data.at(1) - 27));
                    }

                    test.byte1->setText(QString("%1").arg(byte1, 2, 16, QLatin1Char('0')));
                    test.byte2->setText(QString("%1").arg(byte2, 2, 16, QLatin1Char('0')));
                    test.byte3->setText(QString("%1").arg(byte3, 2, 16, QLatin1Char('0')));
                    test.byte4->setText(QString("%1").arg(byte4, 2, 16, QLatin1Char('0')));
                    test.byte5->setText(QString("%1").arg(byte5, 2, 16, QLatin1Char('0')));
                }
                else if (data.at(0) == 2)
                {
                    if (data.at(1) != 0)
                    {
                        qWarning() << "Wrong test value fo RZ byte: " << data.at(1);
                    }
                }
                hdlrComGetTest(com, data);
            }
            else
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }
        }
        break;
    case GB_COM_SET_REG_TEST_2:
        {
            if (!data.isEmpty())
            {
                qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
            }

            setComboBoxValue(stateGlb.regime, eGB_REGIME::GB_REGIME_TEST_2);
            pkgTx.append(com);
        }
        break;
    default:
        {
            qWarning("No command handler: 0x%.2X", com);
        }
    }
}

void Bsp::hdlrComDefTypeAcGet(eGB_COM com, pkg_t &data)
{
    if (data.size() != 0)
    {
        qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
    }

    if ((device.typeDevice->currentData().toUInt() == AVANT_RZSK)
        && (getComboBoxValue(GB_PARAM_COMP_RZSK) == GB_COMP_RZSK_M))
    {
        pkgTx.append(com);
        pkgTx.append(getComboBoxValue(GB_PARAM_DEF_ONE_SIDE));
    }
    else
    {
        qWarning() << "�������� ������� ������ �� � ������� �� ��";
    }
}

void Bsp::hdlrComDefTypeAcSet(eGB_COM com, pkg_t &data)
{
    if (data.size() == 1)
    {
        setComboBoxValue(GB_PARAM_DEF_ONE_SIDE, data.takeFirst());
    }
    else
    {
        // ��������� ������� �� �� ���!
        qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
    }
}

void Bsp::hdlrComDeviceNumGet(eGB_COM com, pkg_t &data)
{
    if (data.size() != 0)
    {
        qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
    }

    pkgTx.append(com);
    pkgTx.append(getSpinBoxValue(GB_PARAM_NUM_OF_DEVICE));
}

void Bsp::hdlrComDeviceNumSet(eGB_COM com, pkg_t &data)
{
    if (data.size() != 1)
    {
        qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
    }

    setSpinBoxValue(GB_PARAM_NUM_OF_DEVICE, data.takeFirst());
}

void Bsp::hdlrComGetVers(eGB_COM com, pkg_t &data)
{
    quint16         vers = 0;
    eGB_TYPE_DEVICE typedevice;

    typedevice = static_cast<eGB_TYPE_DEVICE>(getComboBoxValue(device.typeDevice));

    if (data.size() != 0)
    {
        qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
    }
    //
    pkgTx.append(com);
    pkgTx.append(getComboBoxValue(device.isDef) ? 1 : 0);

    if ((typedevice == AVANT_R400) || (typedevice == AVANT_R400M))
    {
        pkgTx.append(0);  // ���1
        pkgTx.append(0);  // ���2
        pkgTx.append(0);
    }
    else
    {
        pkgTx.append(getSpinBoxValue(GB_PARAM_PRM_COM_NUMS));  // ���1
        pkgTx.append(getSpinBoxValue(GB_PARAM_PRM_COM_NUMS));  // ���2
        pkgTx.append(getSpinBoxValue(GB_PARAM_PRD_COM_NUMS));
    }
    pkgTx.append(getComboBoxValue(GB_PARAM_NUM_OF_DEVICES) + 1);
    pkgTx.append(getComboBoxValue(device.typeLine));

    vers = static_cast<quint16>(getSpinBoxValue(device.versionBspMcu));
    pkgTx.append(static_cast<quint8>(vers >> 8));
    pkgTx.append(static_cast<quint8>(vers));
    vers = static_cast<quint16>(getSpinBoxValue(device.versionBspDsp));
    pkgTx.append(static_cast<quint8>(vers >> 8));
    pkgTx.append(static_cast<quint8>(vers));

    pkgTx.append(getCompatibility(typedevice));

    vers = static_cast<quint8>(getSpinBoxValue(device.versionBsk1PrdPlis));
    pkgTx.append(static_cast<quint8>(vers));
    vers = static_cast<quint8>(getSpinBoxValue(device.versionBsk2PrdPlis));
    pkgTx.append(static_cast<quint8>(vers));
    vers = static_cast<quint8>(getSpinBoxValue(device.versionBsk1PrmPlis));
    pkgTx.append(static_cast<quint8>(vers));
    vers = static_cast<quint8>(getSpinBoxValue(device.versionBsk1PrmPlis));
    pkgTx.append(static_cast<quint8>(vers));
    vers = static_cast<quint8>(getSpinBoxValue(device.versionBszPlis));
    pkgTx.append(static_cast<quint8>(vers));

    pkgTx.append(typedevice);

    vers = static_cast<quint16>(getSpinBoxValue(device.versionPiMcu));
    pkgTx.append(static_cast<quint8>(vers >> 8));
    pkgTx.append(static_cast<quint8>(vers));

    // FIXME � ������ ������ ������ ���������� 0xAB, ����� ����� ������ ��������
    uint8_t typeopto = getComboBoxValue(device.typeOpto);
    if (typeopto != TYPE_OPTO_STANDART)
    {
        typeopto = 0xAB;
    }
    pkgTx.append(typeopto);

    vers = static_cast<quint16>(getSpinBoxValue(device.versionBspDspPlis));
    pkgTx.append(static_cast<quint8>(vers >> 8));
    pkgTx.append(static_cast<quint8>(vers));
}

void Bsp::hdlrComMeasGet(eGB_COM com, pkg_t &data)
{
    qint16          value = 0;
    eGB_TYPE_DEVICE typedevice;

    typedevice = static_cast<eGB_TYPE_DEVICE>(getComboBoxValue(device.typeDevice));

    if (data.size() != 1)
    {
        qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
    }

    // \todo �������� �������� D � �400, ������ U�1.

    qDebug() << "MEAS RX DATA" << data;

    pkgTx.append(com);
    pkgTx.append(0);

    value = getSpinBoxValue(m_measure.R);
    pkgTx.append(static_cast<quint8>(value >> 8));
    pkgTx.append(static_cast<quint8>(value));

    value = getSpinBoxValue(m_measure.I);
    pkgTx.append(static_cast<quint8>(value >> 8));
    pkgTx.append(static_cast<quint8>(value));

    value = getSpinBoxValue(m_measure.U);
    pkgTx.append(static_cast<quint8>(value / 10));
    pkgTx.append(static_cast<quint8>((value % 10) * 10));

    value = getSpinBoxValue(m_measure.Udef1);
    pkgTx.append(static_cast<quint8>(value));

    value = getSpinBoxValue(m_measure.Udef2);
    pkgTx.append(static_cast<quint8>(value));

    value = getSpinBoxValue(m_measure.Ucf1);
    pkgTx.append(static_cast<quint8>(value));

    value = getSpinBoxValue(m_measure.Ucf2);
    pkgTx.append(static_cast<quint8>(value));

    value = getSpinBoxValue(m_measure.Un1);
    pkgTx.append(static_cast<quint8>(value));

    value = getSpinBoxValue(m_measure.Un2);
    pkgTx.append(static_cast<quint8>(value));

    value = getSpinBoxValue(m_measure.Sd);
    pkgTx.append(static_cast<quint8>(value >> 8));
    pkgTx.append(static_cast<quint8>(value));

    value = getSpinBoxValue(m_measure.T);
    pkgTx.append(static_cast<quint8>(value));

    value = getSpinBoxValue(m_measure.dF);
    pkgTx.append(static_cast<quint8>(value));

    qDebug() << "MEAS TX DATA" << pkgTx;
}

//
void Bsp::hdlrComNetAdrGet(eGB_COM com, pkg_t &data)
{
    //
    pkgTx.append(com);
    qint16 value = getSpinBoxValue(GB_PARAM_NET_ADDRESS);
    pkgTx.append(static_cast<uint8_t>(value));
}

//
void Bsp::hdlrComNetAdrSet(eGB_COM com, pkg_t &data)
{
    uint8_t dop = data.takeFirst();
    switch (dop)
    {
    case POS_COM_NET_ADR_netAdr:
        {
            uint8_t byte = data.takeFirst();
            setSpinBoxValue(GB_PARAM_NET_ADDRESS, byte);
        }
        break;

    default: qDebug("No dop byte handler: 0x%.2X", dop);
    }
}

//
void Bsp::hdlrComSetControl(eGB_COM com, pkg_t &data)
{
    pkgTx.append(com);
    pkgTx.append(data);

    if (data.count() != 1)
    {
        qWarning() << msgSizeError.arg(com, 2, 16).arg(data.size());
        return;
    }

    switch (data.at(0))
    {
    case 0x08:
        {  // ���� �����. ���.
            setSpinBoxValue(stateDef.state, 7);
        }
        break;
    case 0x09:
        {  // ���� �����. ����.
            setSpinBoxValue(stateDef.state, 1);
        }
        break;
    }
}

//
void Bsp::hdlrComGetTest(eGB_COM com, pkg_t &data)
{
    pkgTx.append(com);
    pkgTx.append(test.byte1->text().toUInt(nullptr, 16));
    pkgTx.append(test.byte2->text().toUInt(nullptr, 16));
    pkgTx.append(test.byte3->text().toUInt(nullptr, 16));
    pkgTx.append(test.byte4->text().toUInt(nullptr, 16));
    pkgTx.append(test.byte5->text().toUInt(nullptr, 16));
}

uint8_t Bsp::getCompatibility(eGB_TYPE_DEVICE typedevice)
{
    uint8_t compatibility = 0;

    switch (typedevice)
    {
    case AVANT_R400:  // DOWN
    case AVANT_R400M:
        {
            compatibility = getComboBoxValue(GB_PARAM_COMP_P400);
        }
        break;
    case AVANT_RZSK:
        {
            compatibility = getComboBoxValue(GB_PARAM_COMP_RZSK);
        }
        break;
    case AVANT_K400:
        {
            compatibility = getComboBoxValue(GB_PARAM_COMP_K400);
        }
        break;
    case AVANT_OPTO:  // DOWN
    case AVANT_NO:
    case AVANT_MAX:
        {
            compatibility = 0;
        }
        break;
    }

    return compatibility;
}

//
void Bsp::setRegime(int index)
{
    stateDef.regime->setCurrentIndex(index);
    statePrm.regime->setCurrentIndex(index);
    statePrd.regime->setCurrentIndex(index);
}

//
void Bsp::setState(int index)
{
    stateDef.state->setValue(index);
    statePrm.state->setValue(index);
    statePrd.state->setValue(index);
}

//
void Bsp::setDopByte(int index)
{
    stateDef.dopByte->setValue(index);
    statePrm.dopByte->setValue(index);
    statePrd.dopByte->setValue(index);
}

//
void Bsp::updateClock()
{
    *dt = dt->addSecs(1);
}

void Bsp::updateCompatibility(int index)
{
    Q_UNUSED(index)

    int             value  = device.typeDevice->currentData().toInt();
    eGB_TYPE_DEVICE device = static_cast<eGB_TYPE_DEVICE>(value);

    bool r400 = (device == AVANT_R400) || (device == AVANT_R400M);
    mapCombobox.value(GB_PARAM_COMP_P400).at(0)->setEnabled(r400);
    mapCombobox.value(GB_PARAM_COMP_RZSK).at(0)->setEnabled(device == AVANT_RZSK);
    mapCombobox.value(GB_PARAM_COMP_K400).at(0)->setEnabled(device == AVANT_K400);
}
