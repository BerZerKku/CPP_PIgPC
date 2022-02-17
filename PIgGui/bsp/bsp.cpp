#include <QDebug>
#include <QHeaderView>
#include <QTextCodec>
#include <cmath>

#include "bsp.h"

#include "PIg/src/flash.h"
#include "PIg/src/menu/txCom.h"

const QString Bsp::kMsgSizeError       = "Wrong size of data in command %1: %2";
const QString Bsp::kMsgTimeSourceError = "Wrong source of time: %1";

const QTextCodec *Bsp::kCodec      = QTextCodec::codecForName("CP1251");
const QString     Bsp::kTimeFormat = "dd.MM.yyyy hh:mm:ss.zzz";


// @todo Разделить класс на Р400/Р400м/РЗСК/ОПТИКа

Bsp::Bsp(QTreeWidget *tree, QWidget *parent) : QWidget(parent), mTree(tree)
{
    // Эти строки не влияют на содержимое заголовка, но влияют на resize ниже.
    mTree->headerItem()->setText(0, kCodec->toUnicode("Parameter"));
    mTree->headerItem()->setText(1, kCodec->toUnicode("Value"));

    //    expandAll();
    //    header()->resizeSections(QHeaderView::ResizeToContents);
    //    header()->setSectionResizeMode(0, QHeaderView::Fixed);
    //    header()->resizeSection(0, header()->sectionSize(0) + 5);
    //    setFixedWidth(1.75*header()->sectionSize(0));

    mTree->header()->setSectionResizeMode(0, QHeaderView::Fixed);
    mTree->header()->resizeSection(0, 220);
    mTree->setFixedWidth(400);

    mTree->setSelectionMode(QAbstractItemView::NoSelection);
    mTree->setFocusPolicy(Qt::NoFocus);

    mControl.setEnabled(false);

    mTree->clear();
}

//
void Bsp::InitParam()
{
    eGB_PARAM param = GB_PARAM_NULL_PARAM;

    setComboBoxValue(mDevice.isDef, 1);
    setSpinBoxValue(GB_PARAM_PRM_COM_NUMS, 1);  // количество команд = num*4
    setSpinBoxValue(GB_PARAM_PRD_COM_NUMS, 1);  // количество команд = num*4
    setComboBoxValue(GB_PARAM_NUM_OF_DEVICES, GB_NUM_DEVICES_2);
    setComboBoxValue(GB_PARAM_COMP_P400, GB_COMP_R400M_LINER);

    setSpinBoxValue(mDevice.versionBspMcu, 0xF233);
    setSpinBoxValue(mDevice.versionBspDsp, 0x330D);
    setSpinBoxValue(mDevice.versionBspDspPlis, 0x3333);
    setSpinBoxValue(mDevice.versionPiMcu, 0x151);
    setSpinBoxValue(mDevice.versionBsk1PrdPlis, 0x25);
    setSpinBoxValue(mDevice.versionBsk2PrdPlis, 0x25);
    setSpinBoxValue(mDevice.versionBsk1PrmPlis, 0x23);
    setSpinBoxValue(mDevice.versionBsk2PrmPlis, 0x23);
    setSpinBoxValue(mDevice.versionBszPlis, 0x52);

    setComboBoxValue(stateGlb.regime, eGB_REGIME::GB_REGIME_ENABLED);
    setComboBoxValue(stateGlb.state, 1);
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


/**
 * *****************************************************************************
 *
 * @brief Создает ветку параметрв.
 *
 * *****************************************************************************
 */
void Bsp::crtTreeParam()
{
    QTreeWidgetItem *top = new QTreeWidgetItem(mTree);
    top->setText(0, kCodec->toUnicode("Параметры"));
    mTree->addTopLevelItem(top);

    crtTreePrd(top);
    crtTreePrm(top);
    crtTreeGlb(top);
    crtTreeDef(top);  // под глобальными, т.к. нужно количество аппаратов в линии
    crtTreeInterface(top);
}

void Bsp::InitClock()
{
    dt = QDateTime::currentDateTime();

    QTimer *timerClock = new QTimer(this);
    QObject::connect(timerClock, &QTimer::timeout, this, &Bsp::updateClock);
    timerClock->start(1000);
}


void Bsp::crtTreeDef(QTreeWidgetItem *top)
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Защита"));
    top->addChild(ltop);

    // Add params here
    // ex. CrtParamWidget(ltop, GB_PARAM_NET_ADDRESS);
}


void Bsp::crtTreePrd(QTreeWidgetItem(*top))
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Передатчик"));
    top->addChild(ltop);

    // Add params here
    // ex. CrtParamWidget(ltop, GB_PARAM_NET_ADDRESS);
}


void Bsp::crtTreePrm(QTreeWidgetItem(*top))
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Приемник"));
    top->addChild(ltop);

    // Add params here
    // ex. CrtParamWidget(ltop, GB_PARAM_NET_ADDRESS);
}


void Bsp::crtTreeGlb(QTreeWidgetItem *top)
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Общие"));
    top->addChild(ltop);

    // Add params here
    // ex. CrtParamWidget(ltop, GB_PARAM_NET_ADDRESS);
}

void Bsp::crtTreeInterface(QTreeWidgetItem *top)
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    top->setText(0, kCodec->toUnicode("Интерфейс"));
    top->addChild(ltop);

    // Add params here
    // ex. CrtParamWidget(ltop, GB_PARAM_NET_ADDRESS);
}


/**
 * *****************************************************************************
 *
 * @brief Настраивает карту команд.
 *
 * *****************************************************************************
 */
void Bsp::InitComMap()
{
    mComMap.clear();
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


/**
 * *****************************************************************************
 *
 * @brief Создает дерево параметров и производит начальные настройки.
 *
 * *****************************************************************************
 */
void Bsp::Init()
{
    crtTreeDevice();
    crtTreeState();
    crtTreeParam();
    crtTest();
    crtTreeMeasure();
    crtJrn();

    FillComboboxListStateDef();
    FillComboboxListStatePrm();
    FillComboboxListStatePrd();
    FillComboboxListStateGlb();
    FillComboBoxListControl();
    FillComboboxListTest();

    InitComMap();
    InitParam();
    InitClock();
}


void Bsp::crtTreeDevice()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("Устройство"));

    QTreeWidgetItem *item = nullptr;

    item          = new QTreeWidgetItem();
    mDevice.isDef = new QComboBox();
    item->setText(0, kCodec->toUnicode("Защита"));
    top->addChild(item);
    fillComboboxListOnOff(mDevice.isDef);
    mTree->setItemWidget(item, 1, mDevice.isDef);

    crtSpinBox(top, GB_PARAM_PRM_COM_NUMS);
    crtSpinBox(top, GB_PARAM_PRD_COM_NUMS);
    crtComboBox(top, GB_PARAM_NUM_OF_DEVICES);

    crtComboBox(top, GB_PARAM_COMP_K400);
    crtComboBox(top, GB_PARAM_COMP_P400);
    crtComboBox(top, GB_PARAM_COMP_RZSK);

    top->setExpanded(true);

    crtTreeDevieVersions(top);
}

void Bsp::crtTreeDevieVersions(QTreeWidgetItem *top)
{
    QTreeWidgetItem *branch = new QTreeWidgetItem();
    top->addChild(branch);
    branch->setText(0, kCodec->toUnicode("Версии ПО"));

    QTreeWidgetItem *item = nullptr;

    item                  = new QTreeWidgetItem();
    mDevice.versionBspMcu = new QHexSpinBox(false);
    item->setText(0, kCodec->toUnicode("БСП МК"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBspMcu);

    // \todo Версия DSP в К400 состоит из 2 байт, иначе (версия плис, версия дсп)
    item                  = new QTreeWidgetItem();
    mDevice.versionBspDsp = new QHexSpinBox(false);
    item->setText(0, kCodec->toUnicode("БСП ЦСП"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBspDsp);

    // \todo Версия плис отдельно передается только в К400
    item                      = new QTreeWidgetItem();
    mDevice.versionBspDspPlis = new QHexSpinBox(false);
    item->setText(0, kCodec->toUnicode("ЦСП ПЛИС"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBspDspPlis);

    item                 = new QTreeWidgetItem();
    mDevice.versionPiMcu = new QHexSpinBox(false);
    item->setText(0, kCodec->toUnicode("ПИ МК"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionPiMcu);

    item                       = new QTreeWidgetItem();
    mDevice.versionBsk1PrdPlis = new QHexSpinBox(true);
    item->setText(0, kCodec->toUnicode("БСК ПРД1"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBsk1PrdPlis);

    item                       = new QTreeWidgetItem();
    mDevice.versionBsk2PrdPlis = new QHexSpinBox(true);
    item->setText(0, kCodec->toUnicode("БСК ПРД2"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBsk2PrdPlis);

    item                       = new QTreeWidgetItem();
    mDevice.versionBsk1PrmPlis = new QHexSpinBox(true);
    item->setText(0, kCodec->toUnicode("БСК ПРМ1"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBsk1PrmPlis);

    item                       = new QTreeWidgetItem();
    mDevice.versionBsk2PrmPlis = new QHexSpinBox(true);
    item->setText(0, kCodec->toUnicode("БСК ПРМ2"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBsk2PrmPlis);

    item                   = new QTreeWidgetItem();
    mDevice.versionBszPlis = new QHexSpinBox(true);
    item->setText(0, kCodec->toUnicode("БСЗ ПЛИС"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBszPlis);

    branch->setExpanded(false);
}


void Bsp::crtTreeMeasure()
{
    QTreeWidgetItem *item   = nullptr;
    QSpinBox *       widget = nullptr;

    QTreeWidgetItem *top = new QTreeWidgetItem();
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);

    // \todo Сделать измерения в оптике неактивными

    top->setText(0, kCodec->toUnicode("Измерения"));

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(0, 999);
    widget->setToolTip(QString("[%1, %2] Ohm").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("R"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.R = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(0, 999);
    widget->setToolTip(QString("[%1, %2] mA").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("I"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.I = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(0, 999);
    widget->setToolTip(QString("[%1, %2] * 0.1V").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("U"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.U = widget;

    // \todo В К400 вместо Uз1 передается D со значениями от -64 до 64.

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(-99, 99);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("Uз1 / D"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.Udef1 = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(-99, 99);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("Uз2"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.Udef2 = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(-99, 99);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("Uк1"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.Ucf1 = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(-99, 99);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("Uк2"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.Ucf2 = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(-99, 99);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("Uш1"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.Un1 = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(-99, 99);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("Uш2"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.Un2 = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(0, 360);
    widget->setToolTip(QString("[%1, %2] deg").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("Sд"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.Sd = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(0, 100);
    widget->setToolTip(QString("[%1, %2] deg").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("T"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.T = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(0, 100);
    widget->setToolTip(QString("[%1, %2] Hz").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("dF"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.dF = widget;

    top->setExpanded(false);
}


//
void Bsp::crtTreeState()
{
    QTreeWidgetItem *item;
    QTreeWidgetItem *top = new QTreeWidgetItem(mTree);

    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("Текущее состояние"));

    errRegExp.setPattern("[0-9A-Fa-F]+");
    errValidator.setRegExp(errRegExp);

    crtTreeState(top, "Общее", stateGlb);
    crtTreeState(top, "Защита", stateDef);
    crtTreeState(top, "Приемник", statePrm);
    crtTreeState(top, "Передатчик", statePrd);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Управление"));
    top->insertChild(0, item);
    mTree->setItemWidget(item, 1, &mControl);

    connect(stateGlb.regime,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &Bsp::setRegime);

    connect(stateGlb.state,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &Bsp::setState);

    connect(stateGlb.dopByte, QOverload<int>::of(&QSpinBox::valueChanged), this, &Bsp::setDopByte);

    top->setExpanded(true);
}

//
QTreeWidgetItem *Bsp::crtTreeState(QTreeWidgetItem *top, std::string name, Bsp::state_t &state)
{

    QTreeWidgetItem *item = nullptr;
    QTreeWidgetItem *ltop = new QTreeWidgetItem();

    ltop->setText(0, kCodec->toUnicode(name.c_str()));
    top->addChild(ltop);

    item = new QTreeWidgetItem();
    item->setText(0, kCodec->toUnicode("Режим"));
    ltop->addChild(item);
    state.regime = new QComboBox();
    fillComboboxListRegime(state.regime);
    mTree->setItemWidget(item, 1, state.regime);

    item = new QTreeWidgetItem();
    item->setText(0, kCodec->toUnicode("Состояние"));
    ltop->addChild(item);
    state.state = new QComboBox();
    mTree->setItemWidget(item, 1, state.state);

    item = new QTreeWidgetItem();
    item->setText(0, kCodec->toUnicode("Доп. байт"));
    ltop->addChild(item);
    state.dopByte = new QSpinBox();
    state.dopByte->setRange(0, 255);
    state.dopByte->setValue(0);
    mTree->setItemWidget(item, 1, state.dopByte);

    item = new QTreeWidgetItem();
    item->setText(0, kCodec->toUnicode("Неисправности"));
    ltop->addChild(item);
    state.fault = new QLineEdit();
    state.fault->setMaxLength(4);
    state.fault->setInputMask("HHHH");
    state.fault->setValidator(&errValidator);
    state.fault->setText("0000");
    mTree->setItemWidget(item, 1, state.fault);

    connect(state.fault, &QLineEdit::selectionChanged, state.fault, &QLineEdit::deselect);

    item = new QTreeWidgetItem();
    item->setText(0, kCodec->toUnicode("Предупреждения"));
    ltop->addChild(item);
    state.warning = new QLineEdit();
    state.warning->setMaxLength(4);
    state.warning->setInputMask("HHHH");
    state.warning->setValidator(&errValidator);
    state.warning->setText("0000");
    mTree->setItemWidget(item, 1, state.warning);

    connect(state.warning, &QLineEdit::selectionChanged, state.warning, &QLineEdit::deselect);

    ltop->setExpanded(&state == &stateGlb);
    //    ltop->setExpanded(true);
    return ltop;
}

//
void Bsp::crtTest()
{
    QTreeWidgetItem *top = new QTreeWidgetItem(mTree);
    QTreeWidgetItem *item;
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("Тест"));
}


/**
 * *****************************************************************************
 *
 * @brief Создает ветку журналов.
 *
 * *****************************************************************************
 */
void Bsp::crtJrn()
{
    QTreeWidgetItem *top = new QTreeWidgetItem(mTree);
    QTreeWidgetItem *ltop;

    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("Журналы"));

    ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Журнал событий"));
    top->insertChild(0, ltop);
    crtJrnGlb(ltop);

    ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Журнал защиты"));
    top->insertChild(0, ltop);
    crtJrnDef(ltop);
}


/**
 * *****************************************************************************
 *
 * @brief Создает журнал защиты.
 * @param[in] top Верхний уровень.
 *
 * *****************************************************************************
 */
void Bsp::crtJrnDef(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Количество записей"));
    mTree->setItemWidget(item, 1, &mJrnDefCounter);

    top->setExpanded(false);
}


/**
 * *****************************************************************************
 *
 * @brief Создает журнал приемника.
 * @param[in] top Верхний уровень.
 *
 * *****************************************************************************
 */
void Bsp::crtJrnPrm(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Количество записей"));
    mTree->setItemWidget(item, 1, &mJrnPrmCounter);
}


/**
 * *****************************************************************************
 *
 * @brief Создает журнал передатчика.
 * @param[in] top Верхний уровень.
 *
 * *****************************************************************************
 */
void Bsp::crtJrnPrd(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Количество записей"));
    mTree->setItemWidget(item, 1, &mJrnPrdCounter);
}


/**
 * *****************************************************************************
 *
 * @brief Создает журнал событий.
 * @param[in] top Верхний уровень.
 *
 * *****************************************************************************
 */
void Bsp::crtJrnGlb(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Количество записей"));
    mTree->setItemWidget(item, 1, &mJrnGlbCounter);

    top->setExpanded(false);
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
                combobox->addItem(kCodec->toUnicode(l), getAbsMin(param) + i);
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
            combobox->addItem(kCodec->toUnicode(fcOnOff[i]), i);
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
            combobox->addItem(kCodec->toUnicode(fcRegime[i]), i);
        }
    }
}


/**
 * *****************************************************************************
 *
 * @brief Заполняет список общих состояний.
 *
 * Список заполняется на основе списков всех имеющихся устройств.
 *
 * *****************************************************************************
 */
void Bsp::FillComboboxListStateGlb()
{
    QComboBox *combobox = stateGlb.state;

    if (combobox == nullptr)
    {
        return;
    }

    // @todo Сделать формирование списка общих состояний на основе имеющихся устройств.
    for (uint8_t i = 0; i <= 15; i++)
    {
        combobox->addItem(QString::number(i), i);
    }
}


/**
 * *****************************************************************************
 *
 * @brief Возвращает максимальное количество однотипных параметров.
 * @param[in] param Параметр.
 * @return Количество однотипных параметрв.
 * @retval 0 В случае ошибки.
 *
 * *****************************************************************************
 */
quint16 Bsp::GetDependSame(eGB_PARAM param) const
{
    int qty = 0;

    Param::DEPEND_SAME depend = getDependSame(param);
    switch (depend)
    {
    case Param::DEPEND_SAME_NO: qty = getAbsMaxNumOfSameParams(param); break;
    case Param::DEPEND_SAME_ON_COM_PRD:
        qty = mapSpinBox.value(GB_PARAM_PRD_COM_NUMS).at(0)->maximum();
        break;
    case Param::DEPEND_SAME_ON_COM_PRM:
        qty = mapSpinBox.value(GB_PARAM_PRM_COM_NUMS).at(0)->maximum();
        break;
    case Param::DEPEND_SAME_ON_NUM_DEVS:
        {
            QComboBox *combobox = mapCombobox.value(GB_PARAM_NUM_OF_DEVICES).at(0);
            qty                 = combobox->itemData(combobox->count() - 1).toUInt();
            break;
        }
    default: qty = 0;
    }

    if (qty == 0)
    {
        qCritical() << QString("Number of parameters %1 is 0!").arg(param);
        Q_ASSERT(false);
    }

    return qty;
}


//
void Bsp::crtComboBox(QTreeWidgetItem *top, eGB_PARAM param)
{
    QVector<QComboBox *> vcombobox;
    QComboBox *          combobox;
    QTreeWidgetItem *    item;

    uint8_t num = GetDependSame(param);
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
                std::string name("Номер ");
                name += std::to_string(i);
                item->setText(0, kCodec->toUnicode(name.c_str()));
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
            mTree->setItemWidget(item, 1, combobox);
        }

        mapCombobox.insert(param, vcombobox);
    }
}


/**
 * *****************************************************************************
 *
 * @brief Создает SpinBox для каждого байта битовых наборов
 * @param[in] top
 * @param[in] param Параметр.
 *
 * *****************************************************************************
 */
void Bsp::crtSpinbBoxBits(QTreeWidgetItem *top, eGB_PARAM param)
{
    QVector<QSpinBox *> vspinbox;
    QSpinBox *          spinbox;
    QTreeWidgetItem *   ltop;
    QTreeWidgetItem *   item;

    Q_ASSERT(param < GB_PARAM_MAX);
    Q_ASSERT(getParamType(param) == Param::PARAM_BITES);

    uint8_t qty = GetDependSame(param);
    if (qty == 0)
    {
        return;
    }

    ltop = new QTreeWidgetItem(top);
    ltop->setText(0, getParamName(param));
    top->addChild(ltop);

    for (int i = 0; i < qty; i++)
    {
        if (i % 8 == 0)
        {
            spinbox = new QSpinBox(this);

            int    bits = (qty - i > 7) ? 8 : qty - i;
            qint16 min  = 0;
            qint16 max  = std::pow(2, bits);
            spinbox->setRange(min, max);
            spinbox->setPrefix("0x");
            spinbox->setDisplayIntegerBase(16);
            spinbox->setToolTip(QString("%1 - %2").arg(min).arg(max));

            item         = new QTreeWidgetItem(ltop);
            QString text = kCodec->toUnicode("Номера [%1..%2]");
            item->setText(0, text.arg(i + bits).arg(i + 1));
            ltop->addChild(item);
            mTree->setItemWidget(item, 1, spinbox);

            vspinbox.append(spinbox);
        }
    }

    mapSpinBox.insert(param, vspinbox);
}


//
void Bsp::crtLineEdit(QTreeWidgetItem *top, eGB_PARAM param, std::string value)
{
    QLineEdit *      lineedit = new QLineEdit(this);
    QTreeWidgetItem *item     = new QTreeWidgetItem(top);

    // TODO Сделать возможность ввода значений руками
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
    mTree->setItemWidget(item, 1, lineedit);

    mapLineEdit.insert(param, lineedit);
}


//
void Bsp::crtSpinBox(QTreeWidgetItem *top, eGB_PARAM param)
{
    QVector<QSpinBox *> vspinbox;
    QSpinBox *          spinbox;
    QTreeWidgetItem *   item;

    uint8_t num = GetDependSame(param);
    if (num == 0)
    {
        qCritical() << QString("Number of parameters %1 is 0!").arg(getParamName(param));
    }
    else
    {
        if (num > 1)
        {
            item = new QTreeWidgetItem(top);
            item->setText(0, getParamName(param));
            top->addChild(item);
            top = item;
        }

        for (uint8_t i = 1; i <= num; i++)
        {
            item = new QTreeWidgetItem(top);

            if (num > 1)
            {
                std::string name("Номер ");
                name += std::to_string(i);
                item->setText(0, kCodec->toUnicode(name.c_str()));
            }
            else
            {
                item->setText(0, getParamName(param));
            }

            spinbox = new QSpinBox(this);
            vspinbox.append(spinbox);

            Param::PARAM_TYPE type = getParamType(param);
            if (type == Param::PARAM_INT || type == Param::PARAM_I_COR)
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
            mTree->setItemWidget(item, 1, spinbox);
        }

        mapSpinBox.insert(param, vspinbox);
    }
}


/**
 * *****************************************************************************
 *
 * @brief Создает виджет для параметров с вещественным значением.
 * @param[in] param Параметр.
 *
 * *****************************************************************************
 */
void Bsp::crtDoubleSpinBox(QTreeWidgetItem *top, eGB_PARAM param)
{
    QVector<QDoubleSpinBox *> vdspinbox;
    QDoubleSpinBox *          dspinbox;
    QTreeWidgetItem *         item;

    uint8_t num = getAbsMaxNumOfSameParams(param);
    if (num == 0)
    {
        qCritical() << QString("Number of parameters %1 is 0!").arg(getParamName(param));
    }
    else
    {
        if (num > 1)
        {
            item = new QTreeWidgetItem(top);
            item->setText(0, getParamName(param));
            top->addChild(item);
            top = item;
        }

        for (uint8_t i = 1; i <= num; i++)
        {
            item = new QTreeWidgetItem(top);

            if (num > 1)
            {
                std::string name("Номер ");
                name += std::to_string(i);
                item->setText(0, kCodec->toUnicode(name.c_str()));
            }
            else
            {
                item->setText(0, getParamName(param));
            }

            dspinbox = new QDoubleSpinBox(this);
            vdspinbox.append(dspinbox);

            Param::PARAM_TYPE type = getParamType(param);
            if (type == Param::PARAM_U_COR)
            {
                double min = getAbsMin(param) / 10;
                double max = getAbsMax(param) / 10;
                dspinbox->setRange(min, max);
                dspinbox->setSingleStep(double(getDisc(param)) / 10);
                dspinbox->setDecimals(1);
                dspinbox->setValue(dspinbox->minimum());
                dspinbox->setToolTip(QString("%1 - %2").arg(min).arg(max));
            }
            else
            {
                qCritical() << QString("Unknown parameter type %1").arg(getParamName(param));
            }

            if (getCom(param) == eGB_COM::GB_COM_NO)
            {
                QPalette pa;
                pa.setColor(QPalette::WindowText, Qt::red);
                item->setForeground(0, Qt::blue);
            }

            top->addChild(item);
            mTree->setItemWidget(item, 1, dspinbox);
        }

        mapDoubleSpinBox.insert(param, vdspinbox);
    }
}


/**
 * *****************************************************************************
 *
 * @brief Заполняет список состояний защиты.
 *
 * *****************************************************************************
 */
void Bsp::FillComboboxListStateDef()
{
    QComboBox *combobox = stateDef.state;

    if (combobox == nullptr)
        return;

    combobox->addItem(kCodec->toUnicode(fcDefSost00), 0);
    combobox->addItem(kCodec->toUnicode(fcDefSost01), 1);
    combobox->addItem(kCodec->toUnicode(fcDefSost02), 2);
    combobox->addItem(kCodec->toUnicode(fcDefSost03), 3);
    combobox->addItem(kCodec->toUnicode(fcDefSost04), 4);
    combobox->addItem(kCodec->toUnicode(fcDefSost05), 5);
    combobox->addItem(kCodec->toUnicode(fcDefSost06), 6);
    combobox->addItem(kCodec->toUnicode(fcDefSost07), 7);
    combobox->addItem(kCodec->toUnicode(fcDefSost08), 8);
    combobox->addItem(kCodec->toUnicode(fcDefSost09), 9);
    combobox->addItem(kCodec->toUnicode(fcDefSost10), 10);
    combobox->addItem(kCodec->toUnicode(fcDefSost11), 11);
    combobox->addItem(kCodec->toUnicode(fcDefSost12), 12);
    combobox->addItem(kCodec->toUnicode(fcDefSost13), 13);
    combobox->addItem(kCodec->toUnicode(fcDefSost14), 14);
}


/**
 * *****************************************************************************
 *
 * @brief Заполняет список состояний приемника.
 *
 * *****************************************************************************
 */
void Bsp::FillComboboxListStatePrm()
{
    QComboBox *combobox  = statePrm.state;
    QComboBox *combobox2 = statePrm2.state;

    if (combobox == nullptr)
        return;

    combobox->addItem(kCodec->toUnicode(fcPrmSost00), 0);
    combobox->addItem(kCodec->toUnicode(fcPrmSost01), 1);
    combobox->addItem(kCodec->toUnicode(fcPrmSost02), 2);
    combobox->addItem(kCodec->toUnicode(fcPrmSost03), 3);
    combobox->addItem(kCodec->toUnicode(fcPrmSost04), 4);
    combobox->addItem(kCodec->toUnicode(fcPrmSost05), 5);
    combobox->addItem(kCodec->toUnicode(fcPrmSost06), 6);
    combobox->addItem(kCodec->toUnicode(fcPrmSost07), 7);
    combobox->addItem(kCodec->toUnicode(fcPrmSost08), 8);
    combobox->addItem(kCodec->toUnicode(fcPrmSost09), 9);
    combobox->addItem(kCodec->toUnicode(fcPrmSost10), 10);
    combobox->addItem(kCodec->toUnicode(fcPrmSost11), 11);
    combobox->addItem(kCodec->toUnicode(fcPrmSost12), 12);
    combobox->addItem(kCodec->toUnicode(fcPrmSost13), 13);
    combobox->addItem(kCodec->toUnicode(fcPrmSost14), 14);

    if (combobox2 != nullptr)
    {
        for (int i = 0; i < combobox->count(); i++)
        {
            combobox2->addItem(combobox->itemText(i), combobox->itemData(i));
        }
    }
}


/**
 * *****************************************************************************
 *
 * @brief Заполняет список состояний передатчика.
 *
 * *****************************************************************************
 */
void Bsp::FillComboboxListStatePrd()
{
    QComboBox *combobox = statePrd.state;

    if (combobox == nullptr)
        return;

    combobox->addItem(kCodec->toUnicode(fcPrdSost00), 0);
    combobox->addItem(kCodec->toUnicode(fcPrdSost01), 1);
    combobox->addItem(kCodec->toUnicode(fcPrdSost02), 2);
    combobox->addItem(kCodec->toUnicode(fcPrdSost03), 3);
    combobox->addItem(kCodec->toUnicode(fcPrdSost04), 4);
    combobox->addItem(kCodec->toUnicode(fcPrdSost05), 5);
    combobox->addItem(kCodec->toUnicode(fcPrdSost06), 6);
    combobox->addItem(kCodec->toUnicode(fcPrdSost07), 7);
    combobox->addItem(kCodec->toUnicode(fcPrdSost08), 8);
    combobox->addItem(kCodec->toUnicode(fcPrdSost09), 9);
    combobox->addItem(kCodec->toUnicode(fcPrdSost10), 10);
    combobox->addItem(kCodec->toUnicode(fcPrdSost11), 11);
    combobox->addItem(kCodec->toUnicode(fcPrdSost12), 12);
    combobox->addItem(kCodec->toUnicode(fcPrdSost13), 13);
    combobox->addItem(kCodec->toUnicode(fcPrdSost14), 14);
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
                value += (box->currentData().toUInt() > 0) ? (1 << i) : 0;
            }
        }
    }
    else
    {
        QString msg =
            QString("%1: Parameter '%2' (%3) not found.").arg(__FUNCTION__).arg(param).arg(number);
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

    lineedit->setText(Bsp::kCodec->toUnicode(value.c_str()));
    index = 0;
    // FIXME Добавить проверку значения!

    return index;
}


//
qint16 Bsp::getSpinBoxValue(eGB_PARAM param, uint8_t number)
{
    qint16 value = 0;

    if (number > 0 && mapSpinBox.contains(param) && mapSpinBox.value(param).size() >= number)
    {
        Param::Param::PARAM_TYPE type = getParamType(param);
        if (type == Param::PARAM_INT || type == Param::Param::PARAM_I_COR)
        {
            value = getSpinBoxValue(mapSpinBox.value(param).at(number - 1));
            value = (value / getDisc(param)) * getDisc(param);
            value /= getFract(param);
        }
        else
        {
            QString message = QString("Parameter '%1' type (%2) not found.");
            qWarning() << message.arg(getParamName(param)).arg(type);
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


/**
 * *****************************************************************************
 *
 * @brief Возвращает значение для вещественных параметров.
 * @param[in] param Параметр.
 * @param[in] number Подномер для параметра.
 * @return
 *
 * *****************************************************************************
 */
qint16 Bsp::getDoubleSpinBoxValue(eGB_PARAM param, uint8_t number)
{
    qint16 value = 0;

    if (number > 0 && mapDoubleSpinBox.contains(param)
        && mapDoubleSpinBox.value(param).size() >= number)
    {
        double fvalue = 0.0;
        number -= 1;

        if (getParamType(param) == Param::PARAM_U_COR)
        {
            fvalue = mapDoubleSpinBox.value(param).at(number)->value();
            value  = fvalue * 10;
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

/**
 * *****************************************************************************
 *
 * @brief Устаналивает значение для вещественных параметров.
 * @param[in] param Параметр.
 * @param[in] value Значение.
 * @param[in] number Подномер для параметра.
 *
 * *****************************************************************************
 */
void Bsp::setDoubleSpinBoxValue(eGB_PARAM param, qint16 value, uint8_t number)
{
    QDoubleSpinBox *dspinbox = nullptr;

    if ((number > 0) && mapDoubleSpinBox.contains(param)
        && (mapDoubleSpinBox.value(param).size() >= number))
    {
        double fvalue = 0.0;

        if (getParamType(param) == Param::PARAM_U_COR)
        {
            fvalue   = double(value) / 10;
            dspinbox = mapDoubleSpinBox.value(param).at(number - 1);
            dspinbox->setValue(fvalue);
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
        QString msg = QString("%1: Parameter '%2' not found.").arg(__FUNCTION__).arg(param);
        qWarning() << msg;
    }
}


/**
 * *****************************************************************************
 *
 * @brief Вызывает обработчик для принятой команды.
 * @param[in] com Команда.
 * @param[in] data Данные.
 *
 * *****************************************************************************
 */
void Bsp::procCommand(eGB_COM com, pkg_t &data)
{
    mPkgTx.clear();

    HdlrCom_t hdlr = mComMap.value(com, &Bsp::HdlrComDummy);
    (this->*hdlr)(com, data);
}

void Bsp::SlotReadByte(int value)
{
    static uint8_t len = 0;

    // @fixme Добавить время на передачу байт.
    // @fixme Найти сигналу окончания передачи данных из БСП более подходящее место!
    emit SignalSendFinished();

    switch (mPkgRx.size())
    {
    case 0:
        {
            if (value == 0x55)
            {
                mPkgRx.clear();
                mPkgRx.append(value);
            }
        }
        break;
    case 1:
        {
            if (value == 0xAA)
            {
                mPkgRx.append(value);
            }
            else
            {
                mPkgRx.clear();
            }
        }
        break;
    case 2:
        {
            mPkgRx.append(value);
        }
        break;
    case 3:
        {
            mPkgRx.append(value);
            len = value;
        }
        break;
    default:
        {
            mPkgRx.append(value);
            if (len == 0)
            {
                eGB_COM com;

                if (checkPkg(mPkgRx, com))
                {
                    //                    if (viewCom.count(com) != 0)
                    //                    {
                    //                    qDebug() << "comRx <<< " << showbase << hex << mPkgRx
                    //                    <<
                    //                    ", com = " << com;
                    //                    }

                    procCommand(com, mPkgRx);

                    if (!mPkgTx.isEmpty())
                    {
                        mPkgTx.insert(1, mPkgTx.size() - 1);
                        mPkgTx.append(calcCrc(mPkgTx));
                        mPkgTx.insert(0, 0x55);
                        mPkgTx.insert(1, 0xAA);

                        com = static_cast<eGB_COM>(mPkgTx.at(2));
                        //                        if (viewCom.count(com) != 0)
                        //                        {
                        //                            qDebug() << "comTx >>> " << showbase <<
                        //                            hex << Bsp::pkgTx;
                        //                        }

                        for (uint8_t byte : mPkgTx)
                        {
                            emit SignalWriteByte(byte);
                        }


                        mPkgTx.clear();
                    }
                }
                mPkgRx.clear();
            }
            else
            {
                len--;
            }
        }
    }
}


//
QString Bsp::getParamName(eGB_PARAM param)
{
    QString name = "Noname";

    if (param < GB_PARAM_MAX)
    {
        name = kCodec->toUnicode(getNameOfParam(param));
    }

    return name;
}


/**
 * *****************************************************************************
 *
 * @brief Создает виджет для параметра.
 * @param[in] param Параметр.
 *
 * *****************************************************************************
 */
void Bsp::CrtParamWidget(QTreeWidgetItem *top, eGB_PARAM param)
{
    Q_ASSERT(param < GB_PARAM_MAX);

    switch (getParamType(param))
    {
    case Param::PARAM_LIST: crtComboBox(top, param); break;
    case Param::PARAM_INT: crtSpinBox(top, param); break;
    case Param::PARAM_U_COR: crtDoubleSpinBox(top, param); break;
    case Param::PARAM_I_COR: crtSpinBox(top, param); break;
    case Param::PARAM_BITES: crtSpinbBoxBits(top, param); break;

    case Param::PARAM_NO: Q_ASSERT(false);
    }
}


/**
 * *****************************************************************************
 *
 * @brief Возвращает значение параметра.
 * @param[in] param Параметр.
 * @param[in] number Дополнительный номер параметра.
 *
 * *****************************************************************************
 */
qint16 Bsp::GetParamValue(eGB_PARAM param, quint8 number)
{
    qint16 value = 0;

    switch (getParamType(param))
    {
    case Param::PARAM_LIST: value = getComboBoxValue(param, number); break;
    case Param::PARAM_INT: value = getSpinBoxValue(param, number); break;
    case Param::PARAM_U_COR: value = getDoubleSpinBoxValue(param); break;
    case Param::PARAM_I_COR: value = getSpinBoxValue(param, number); break;
    case Param::PARAM_BITES: value = getSpinBoxValue(param, number % 8); break;

    case Param::PARAM_NO: Q_ASSERT(false);
    }

    return value;
}


/**
 * *****************************************************************************
 *
 * @brief Устанавливает значение параметра.
 * @param[in] param Параметр.
 * @param[in] value Значение.
 * @param[in] number Дополнительный номер параметра.
 *
 * *****************************************************************************
 */
void Bsp::SetParamValue(eGB_PARAM param, qint16 value, quint8 number)
{
    Q_ASSERT(param < GB_PARAM_MAX);

    switch (getParamType(param))
    {
    case Param::PARAM_LIST: setComboBoxValue(param, quint8(value), number); break;
    case Param::PARAM_INT: setSpinBoxValue(param, value, number); break;
    case Param::PARAM_U_COR: setDoubleSpinBoxValue(param, value, number); break;
    case Param::PARAM_I_COR: setSpinBoxValue(param, value, number); break;
    case Param::PARAM_BITES: setSpinBoxValue(param, value, number % 8); break;

    case Param::PARAM_NO: Q_ASSERT(false);
    }
}


//
void Bsp::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

//
quint8 Bsp::bcd2int(quint8 bcd, bool *ok)
{
    quint8 value = 0;
    bool   t_ok  = ((bcd & 0x0F) < 0x0A) && ((bcd & 0xF0) < 0xA0);

    if (t_ok)
    {
        value = bcd & 0x0F;
        value += ((bcd >> 4) & 0x0F) * 10;
    }
    else
    {
        qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << " Error: " << bcd;
    }

    if (ok != nullptr)
    {
        *ok = t_ok;
    }


    return value;
}

//
quint8 Bsp::int2bcd(quint8 val, bool *ok)
{
    quint8 bcd  = 0;
    bool   t_ok = (val < 100);

    if (t_ok)
    {
        bcd = (val % 10);
        bcd += (val / 10) << 4;
    }
    else
    {
        qWarning("Can't convert value 0x%.2X to BCD code!", val);
    }

    if (ok != nullptr)
    {
        *ok = t_ok;
    }

    return bcd;
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
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
            }
            mPkgTx.append(com);
            mPkgTx.append(1);
            mPkgTx.append(2);
            mPkgTx.append(3);
        }
        break;


    case GB_COM_PRM_GET_TIME_ON:
        {
            if (data.size() != 0)
            {
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            mPkgTx.append(com);
            // FIXME Есть два разных параметра "Задержка на фиксацию команды"
            mPkgTx.append(Bsp::getSpinBoxValue(GB_PARAM_PRM_TIME_ON));
        }
        break;

    case GB_COM_PRM_GET_TIME_OFF:
        {
            if (data.size() != 0)
            {
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            mPkgTx.append(com);
            // FIXME Сделать зависимость от количества команд.
            uint8_t number = getAbsMaxNumOfSameParams(GB_PARAM_PRM_TIME_OFF);
            for (uint8_t i = 1; i <= number; i++)
            {
                qint16 value = Bsp::getSpinBoxValue(GB_PARAM_PRM_TIME_OFF, i);
                mPkgTx.append(static_cast<quint8>(value));
            }
        }
        break;

    case GB_COM_PRM_GET_BLOCK_COM:
        {
            if (data.size() != 0)
            {
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            mPkgTx.append(com);
            mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRM_COM_BLOCK, 1));
            mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRM_COM_BLOCK, 9));
            mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRM_COM_BLOCK, 17));
            mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRM_COM_BLOCK, 25));
        }
        break;

    case GB_COM_PRD_GET_TIME_ON:
        {
            if (data.size() != 0)
            {
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            mPkgTx.append(com);
            mPkgTx.append(Bsp::getSpinBoxValue(GB_PARAM_PRD_IN_DELAY));
        }
        break;

    case GB_COM_PRD_GET_DURATION:
        {
            if (data.size() != 0)
            {
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            mPkgTx.append(com);
            // FIXME Тут разные параметры для оптики / вч
            mPkgTx.append(Bsp::getSpinBoxValue(GB_PARAM_PRD_DURATION_O));
        }
        break;

    case GB_COM_PRD_GET_BLOCK_COM:
        {
            if (data.size() != 0)
            {
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            mPkgTx.append(com);
            mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_BLOCK, 1));
            mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_BLOCK, 9));
            mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_BLOCK, 17));
            mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_BLOCK, 25));
        }
        break;

    case GB_COM_PRD_GET_LONG_COM:
        {
            if (data.size() != 0)
            {
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            mPkgTx.append(com);
            mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_LONG, 1));
            mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_LONG, 9));
            mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_LONG, 17));
            mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_LONG, 25));
        }
        break;

    case GB_COM_PRD_GET_COM_SIGN:
        {
            if (data.size() != 0)
            {
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            mPkgTx.append(com);
            mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_SIGNAL, 1));
            mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_SIGNAL, 9));
            mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_SIGNAL, 17));
            mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_PRD_COM_SIGNAL, 25));
        }
        break;

    case GB_COM_GET_TIME_SINCHR:
        {
            if (data.size() != 0)
            {
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            mPkgTx.append(com);
            mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_TIME_SYNCH));
            // TODO В РЗСК второй байт "Тип детектора"
        }
        break;

    case GB_COM_GET_COM_PRM_KEEP:
        {
            if (data.size() != 0)
            {
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            mPkgTx.append(com);
            // FIXME Может быть еще Uвых номинальное!
            mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_COM_PRM_KEEP));
        }
        break;

    case GB_COM_GET_COM_PRD_KEEP:
        {
            if (data.size() != 0)
            {
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
            }
            //
            mPkgTx.append(com);

            //            uint8_t         value      = getComboBoxValue(mDevice.typeDevice);
            //            eGB_TYPE_DEVICE typedevice = static_cast<eGB_TYPE_DEVICE>(value);

            //            if (typedevice == AVANT_K400)
            //            {
            //                // FIXME Добавить остальные параметры для К400.
            //                mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_COM_PRD_KEEP));
            //                mPkgTx.append(getCompatibility(typedevice));
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //                mPkgTx.append(0);
            //            }
            //            else if (typedevice == AVANT_RZSK)
            //            {
            //                mPkgTx.append(Bsp::getComboBoxValue(GB_PARAM_COM_PRD_KEEP));
            //                mPkgTx.append(getCompatibility(typedevice));
            //            }
            //            else
            //            {
            //                mPkgTx.append(getCompatibility(typedevice));
            //            }
        }
        break;

    default:
        {
            qWarning("No command handler: 0x%.2X", com);
        }
    }
}

//
void Bsp::procCommandWriteParam(eGB_COM com, pkg_t &data)
{
    mPkgTx.append(com);
    mPkgTx.append(data);

    switch (com)
    {
    case GB_COM_PRM_SET_TIME_ON:
        {
            if (data.size() != 1)
            {
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
            }
            else
            {
                uint8_t value = data.takeFirst();
                // FIXME Есть два разных параметра "Задержка на фиксацию команды"
                Bsp::setSpinBoxValue(GB_PARAM_PRM_TIME_ON, value);
            }
        }
        break;

    case GB_COM_PRM_SET_TIME_OFF:
        {
            if (data.size() != 2)
            {
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
            }
            else
            {
                uint8_t number = data.takeFirst();
                uint8_t value  = data.takeFirst();
                // FIXME Есть два разных параметра "Задержка на фиксацию команды"
                Bsp::setSpinBoxValue(GB_PARAM_PRM_TIME_OFF, value, number);
            }
        }
        break;

    case GB_COM_PRM_SET_BLOCK_COM:
        {
            if (data.size() != 2)
            {
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
            }
            else
            {
                uint8_t number = data.takeFirst();
                uint8_t value  = data.takeFirst();
                // FIXME Есть два разных параметра "Задержка на фиксацию команды"
                Bsp::setComboBoxValueBits(GB_PARAM_PRM_COM_BLOCK, value, number);
            }
        }
        break;

    case GB_COM_PRD_SET_TIME_ON:
        {
            if (data.size() != 1)
            {
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
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
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
            }
            else
            {
                uint8_t value = data.takeFirst();
                // FIXME Тут может быть разный параметр для ВЧ / Оптика
                Bsp::setSpinBoxValue(GB_PARAM_PRD_DURATION_O, value);
            }
        }
        break;

    case GB_COM_PRD_SET_BLOCK_COM:
        {
            if (data.size() != 2)
            {
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
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
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
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
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
            }
            else
            {
                uint8_t number = data.takeFirst();
                uint8_t value  = data.takeFirst();
                Bsp::setComboBoxValueBits(GB_PARAM_PRD_COM_SIGNAL, value, number);
            }
        }
        break;

    case GB_COM_SET_TIME_SINCHR:
        {
            // TODO В РЗСК два байта, т.к. может быть два параметра
            if (data.size() != 2)
            {
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
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
                qWarning() << kMsgSizeError.arg(com, 2, 16).arg(data.size());
            }
            else
            {
                uint8_t value = data.takeFirst();
                // FIXME Может быть Uвых номинальное.
                Bsp::setComboBoxValue(GB_PARAM_COM_PRM_KEEP, value);
            }
        }
        break;

    case GB_COM_SET_COM_PRD_KEEP:
        {
            uint8_t value = data.takeFirst();
            uint8_t dop   = data.takeFirst();
            // FIXME Добавить остальные параметры для К400.
            // Учесть что в Р400 только один байт "Совместимость".

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

    default:
        {
            qWarning("No command handler: 0x%.2X", com);
        }
    }
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


/**
 * *****************************************************************************
 *
 * @brief Проверяет количество байт данных в сообщении
 * @param[in] com Команда.
 * @param[in] size Количество байт данных в сообщении.
 * @param[in] size_allowed Возможное количество данных в сообщении.
 * @return True если корректное количество байт, иначе false.
 *
 * *****************************************************************************
 */
bool Bsp::CheckSize(uint8_t com, int size, QVector<int> size_allowed)
{
    Q_ASSERT(size_allowed.size() > 0);

    bool check = (size_allowed.count(size) != 0);
    if (!check)
    {
        QString message = "Wrong size of data in command %1: %2";
        qWarning() << message.arg(com, 2, 16, QLatin1Char('0')).arg(size);
    }

    return check;
}


/**
 * *****************************************************************************
 *
 * @brief Заглушка для команды. Выводит предупреждение.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComDummy(eGB_COM com, pkg_t &data)
{
    Q_UNUSED(data);

    qWarning() << QString("Command handler missing: 0x%1").arg(com, 2, 16, QLatin1Char('0'));
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи типа защиты.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComDefx01(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_DEF_TYPE || com == GB_COM_DEF_SET_DEF_TYPE);

    if (com == GB_COM_DEF_GET_DEF_TYPE)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(getComboBoxValue(GB_PARAM_DEF_TYPE));

        Q_ASSERT(mPkgTx.size() == 2);  // команда + байт данных
    }

    if (com == GB_COM_DEF_SET_DEF_TYPE)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        setComboBoxValue(GB_PARAM_DEF_TYPE, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи количества аппаратов в линии.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComDefx02(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_LINE_TYPE || com == GB_COM_DEF_SET_LINE_TYPE);

    if (com == GB_COM_DEF_GET_LINE_TYPE)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(getComboBoxValue(GB_PARAM_NUM_OF_DEVICES));

        Q_ASSERT(mPkgTx.size() == 2);  // команда + байт данных
    }

    if (com == GB_COM_DEF_SET_LINE_TYPE)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        setComboBoxValue(GB_PARAM_NUM_OF_DEVICES, data.at(0));
    }
}

/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи допустимого времени без манипуляции.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComDefx03(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_T_NO_MAN || com == GB_COM_DEF_SET_T_NO_MAN);

    if (com == GB_COM_DEF_GET_T_NO_MAN)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(uint8_t(GetParamValue(GB_PARAM_TIME_NO_MAN)));

        Q_ASSERT(mPkgTx.size() == 2);  // команда + байт данных
    }

    if (com == GB_COM_DEF_SET_T_NO_MAN)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        SetParamValue(GB_PARAM_TIME_NO_MAN, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи компенсации задержки линии 1 и 2
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComDefx04(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_DELAY || com == GB_COM_DEF_SET_DELAY);

    if (com == GB_COM_DEF_GET_DELAY)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(uint8_t(GetParamValue(GB_PARAM_DELAY, 1)));
        mPkgTx.append(uint8_t(GetParamValue(GB_PARAM_DELAY, 2)));

        Q_ASSERT(mPkgTx.size() == 3);  // команда + 2 байт данных
    }

    if (com == GB_COM_DEF_SET_DELAY)
    {
        if (!CheckSize(com, data.size(), { 2 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        uint8_t index = data.at(1);
        switch (index)
        {
        case 1: [[fallthrow]];
        case 2: SetParamValue(GB_PARAM_DELAY, data.at(0), index); break;
        default:
            QString message = "Wrong data in command %1: %2";
            qWarning() << message.arg(com, 2, 16, QLatin1Char('0')).arg(data.at(0));
        }


        SetParamValue(GB_PARAM_TIME_NO_MAN, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения коррекция тока и напряжения.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComGlbx33(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_COR_U_I || com == GB_COM_SET_COR_U_I);

    if (com == GB_COM_GET_COR_U_I)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        qint16 value;

        mPkgTx.append(com);

        value = GetParamValue(GB_PARAM_COR_U);
        mPkgTx.append(static_cast<uint8_t>(value / 10));
        mPkgTx.append(static_cast<uint8_t>(value % 10) * 10);

        value = GetParamValue(GB_PARAM_COR_I);
        mPkgTx.append(static_cast<uint8_t>(value >> 8));
        mPkgTx.append(static_cast<uint8_t>(value));
        mPkgTx.append(0);  // коррекция тока 2, старший байт
        mPkgTx.append(0);  // коррекция тока 2, младший байт

        Q_ASSERT(mPkgTx.size() == 7);  // команда + 6 байт данных
    }

    if (com == GB_COM_SET_COR_U_I)
    {
        if (!CheckSize(com, data.size(), { 3 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        qint16 value;

        switch (data.at(0))
        {
        case 1:
            value = static_cast<qint16>(static_cast<qint8>(data.at(1))) * 10;
            value += static_cast<qint16>(static_cast<qint8>(data.at(2))) / 10;
            SetParamValue(GB_PARAM_COR_U, value);
            break;
        case 2:
            value = static_cast<qint16>(data.at(1)) << 8;
            value += static_cast<qint16>(data.at(2));
            SetParamValue(GB_PARAM_COR_I, value);
            break;
        case 4: SetParamValue(GB_PARAM_COR_U, 0); break;
        case 5: SetParamValue(GB_PARAM_COR_I, 0); break;

        default:
            // 3 - коррекция тока 2 для трешки
            // 6 - сброс коррекции тока 2 для трешки
            QString message = "Wrong data in command %1: %2";
            qWarning() << message.arg(com, 2, 16, QLatin1Char('0')).arg(data.at(0));
        }
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи адреса в локальной сети.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComGlbx38(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_NET_ADR || com == GB_COM_SET_NET_ADR);

    if (com == GB_COM_GET_NET_ADR)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_NET_ADDRESS)));

        Q_ASSERT(mPkgTx.size() == 2);  // команда + байт данных
    }

    if (com == GB_COM_SET_NET_ADR)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        SetParamValue(GB_PARAM_NET_ADDRESS, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи частоты.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComGlbx3A(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_FREQ || com == GB_COM_SET_FREQ);

    quint16 freq;

    if (com == GB_COM_GET_FREQ)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }


        mPkgTx.append(com);
        freq = GetParamValue(GB_PARAM_FREQ);
        mPkgTx.append(static_cast<uint8_t>(freq >> 8));
        mPkgTx.append(static_cast<uint8_t>(freq));

        Q_ASSERT(mPkgTx.size() == 3);  // команда + 2 байта данных
    }

    if (com == GB_COM_SET_FREQ)
    {
        if (!CheckSize(com, data.size(), { 2 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        freq = (static_cast<quint16>(data.at(0)) << 8) + data.at(1);
        SetParamValue(GB_PARAM_FREQ, freq);
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи номера аппарата.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComGlbx3B(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_DEVICE_NUM || com == GB_COM_SET_DEVICE_NUM);

    if (com == GB_COM_GET_DEVICE_NUM)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_NUM_OF_DEVICE)));

        Q_ASSERT(mPkgTx.size() == 2);  // команда + байт данных
    }

    if (com == GB_COM_SET_DEVICE_NUM)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        SetParamValue(GB_PARAM_NUM_OF_DEVICE, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения контроля напряжения выхода.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComGlbx3D(eGB_COM com, pkg_t &data)
{

    Q_ASSERT(com == GB_COM_GET_OUT_CHECK || com == GB_COM_SET_OUT_CHECK);

    if (com == GB_COM_GET_OUT_CHECK)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_OUT_CHECK)));

        Q_ASSERT(mPkgTx.size() == 2);  // команда + байт данных
    }

    if (com == GB_COM_SET_OUT_CHECK)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        SetParamValue(GB_PARAM_OUT_CHECK, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду пуск ПРМ
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComRegx51(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_PRM_ENTER);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    // ответ на команду записи совпадает с запросом
    mPkgTx.append(com);
    mPkgTx.append(data);

    if (statePrm.regime != nullptr)
    {
        if (statePrm.regime->currentData().toUInt() == GB_REGIME_READY)
        {
            statePrm.regime->setCurrentIndex(statePrm.regime->findData(GB_REGIME_ENABLED));
        }
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду установки режима "Выведен".
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComRegx70(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_SET_REG_DISABLED);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    // ответ на команду записи совпадает с запросом
    mPkgTx.append(com);
    mPkgTx.append(data);

    setComboBoxValue(stateGlb.regime, eGB_REGIME::GB_REGIME_DISABLED);
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду установки режима "Введен".
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComRegx71(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_SET_REG_ENABLED);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    // ответ на команду записи совпадает с запросом
    mPkgTx.append(com);
    mPkgTx.append(data);

    setComboBoxValue(stateGlb.regime, eGB_REGIME::GB_REGIME_ENABLED);
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду изменения режима на Тест 2 (приемник)
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComRegx7D(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_SET_REG_TEST_2);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    stateGlb.regime->setCurrentIndex(GB_REGIME_TEST_2);
    stateGlb.state->setCurrentIndex(12);

    // ответ на команду записи совпадает с запросом (?!)
    mPkgTx.append(com);
    mPkgTx.append(data);
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду сброса индикации.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComRegx9A(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_PRM_RES_IND);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    // ответ на команду записи совпадает с запросом (?!)
    mPkgTx.append(com);
    mPkgTx.append(data);

    mPrdLightPA.setValue(0);
    mPrmLightPA.setValue(0);
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения количества записей в журнале защиты.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComJrnxC1(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_JRN_CNT);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    uint16_t len = static_cast<uint16_t>(mJrnDefCounter.value());

    if (mJrnDefOverflow.isChecked())
    {
        len |= 0x8000;
    }

    mPkgTx.append(com);
    mPkgTx.append(uint8_t(len));
    mPkgTx.append(uint8_t(len >> 8));

    Q_ASSERT(mPkgTx.size() == 3);  // команда + 2 байта данных
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения записей журнала защиты.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComJrnxC2(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_JRN_ENTRY);

    if (!CheckSize(com, data.size(), { 2 }))
    {
        return;
    }

    int event_number = data.takeFirst();
    event_number += static_cast<int>(data.takeFirst());

    mPkgTx.append(com);
    mPkgTx.append(GB_DEVICE_DEF);             // устройство
    mPkgTx.append(event_number);              // zzzz z[ман][пуск][стоп]
    mPkgTx.append(event_number % 17);         // состояние защиты
    mPkgTx.append(event_number);              // zzzz z[вых][прд][прм]
    mPkgTx.append(0);                         // b5
    mPkgTx.append(0);                         // b6
    mPkgTx.append(0);                         // b7
    mPkgTx.append(static_cast<quint8>(999));  // миллисекунды, младший байт
    mPkgTx.append(static_cast<quint8>(999 >> 8));  // миллисекунды, старший байт
    mPkgTx.append(int2bcd(event_number % 60));     // секунды, bcd
    mPkgTx.append(int2bcd(event_number / 60));     // минуты, bcd
    mPkgTx.append(0x23);                           // часы, bcd
    mPkgTx.append(1);                              // день недели
    mPkgTx.append(0x01);                           // день, bcd
    mPkgTx.append(0x02);                           // месяц, bcd
    mPkgTx.append(0x21);                           // год, bcd

    Q_ASSERT(mPkgTx.size() == 17);  // команда + 16 байт данных
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения количества записей в журнале прм.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComJrnxD1(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_PRM_GET_JRN_CNT);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    uint16_t len = static_cast<uint16_t>(mJrnPrmCounter.value());

    if (mJrnPrmOverflow.isChecked())
    {
        len |= 0x8000;
    }

    mPkgTx.append(com);
    mPkgTx.append(uint8_t(len));
    mPkgTx.append(uint8_t(len >> 8));

    Q_ASSERT(mPkgTx.size() == 3);  // команда + 2 байта данных
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения записей журнала приемника.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComJrnxD2(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_PRM_GET_JRN_ENTRY);

    if (!CheckSize(com, data.size(), { 2 }))
    {
        return;
    }

    int event_number = data.takeFirst();
    event_number += static_cast<int>(data.takeFirst());

    mPkgTx.append(com);
    mPkgTx.append(GB_DEVICE_PRM);               // устройство
    mPkgTx.append(event_number % 33);           // номер команды
    mPkgTx.append(event_number % 3);            // 0 - конец, 1 - начало
    mPkgTx.append(0);                           // b4
    mPkgTx.append(0);                           // b5
    mPkgTx.append(0);                           // b6
    mPkgTx.append(0);                           // b7
    mPkgTx.append(999 & 0xFF);                  // миллисекунды, младший байт
    mPkgTx.append((999 >> 8) & 0xFF);           // миллисекунды, старший байт
    mPkgTx.append(int2bcd(event_number % 60));  // секунды, bcd
    mPkgTx.append(int2bcd(event_number / 60));  // минуты, bcd
    mPkgTx.append(0x23);                        // часы, bcd
    mPkgTx.append(0);                           // день недели
    mPkgTx.append(0x01);                        // день, bcd
    mPkgTx.append(0x02);                        // месяц, bcd
    mPkgTx.append(0x21);                        // год, bcd

    Q_ASSERT(mPkgTx.size() == 17);  // команда + 16 байт данных
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения количества записей в журнале прд.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComJrnxE1(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_PRD_GET_JRN_CNT);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    uint16_t len = static_cast<uint16_t>(mJrnPrdCounter.value());

    if (mJrnPrdOverflow.isChecked())
    {
        len |= 0x8000;
    }

    mPkgTx.append(com);
    mPkgTx.append(uint8_t(len));
    mPkgTx.append(uint8_t(len >> 8));

    Q_ASSERT(mPkgTx.size() == 3);  // команда + 2 байта данных
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения записей журнала передатчика.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComJrnxE2(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_PRD_GET_JRN_ENTRY);

    if (!CheckSize(com, data.size(), { 2 }))
    {
        return;
    }

    int event_number = data.takeFirst();
    event_number += static_cast<int>(data.takeFirst());

    mPkgTx.append(com);
    mPkgTx.append(GB_DEVICE_PRD);               // устройство
    mPkgTx.append(event_number % 33);           // номер команды
    mPkgTx.append(event_number % 3);            // 0 - конец, 1 - начало
    mPkgTx.append(0);                           // b4
    mPkgTx.append(0);                           // b5
    mPkgTx.append(0);                           // b6
    mPkgTx.append(0);                           // b7
    mPkgTx.append(999 & 0xFF);                  // миллисекунды, младший байт
    mPkgTx.append((999 >> 8) & 0xFF);           // миллисекунды, старший байт
    mPkgTx.append(int2bcd(event_number % 60));  // секунды, bcd
    mPkgTx.append(int2bcd(event_number / 60));  // минуты, bcd
    mPkgTx.append(0x23);                        // часы, bcd
    mPkgTx.append(0);                           // день недели
    mPkgTx.append(0x01);                        // день, bcd
    mPkgTx.append(0x02);                        // месяц, bcd
    mPkgTx.append(0x21);                        // год, bcd

    Q_ASSERT(mPkgTx.size() == 17);  // команда + 16 байт данных
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения количества записей в журнале событий.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComJrnxF1(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_JRN_CNT);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    // @todo Добавить возможность переполнени
    uint16_t len = static_cast<uint16_t>(mJrnGlbCounter.value());

    if (mJrnGlbOverflow.isChecked())
    {
        len |= 0x8000;
    }

    mPkgTx.append(com);
    mPkgTx.append(uint8_t(len));
    mPkgTx.append(uint8_t(len >> 8));

    Q_ASSERT(mPkgTx.size() == 3);  // команда + 2 байта данных
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения записей журнала событий.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComJrnxF2(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_JRN_ENTRY);

    if (!CheckSize(com, data.size(), { 2 }))
    {
        return;
    }

    int event_number = data.takeFirst();
    event_number += static_cast<int>(data.takeFirst());

    mPkgTx.append(com);
    mPkgTx.append(GB_DEVICE_GLB);               // устройство
    mPkgTx.append(event_number % 43);           // событие
    mPkgTx.append(event_number % 8);            // режим
    mPkgTx.append(0);                           // b4
    mPkgTx.append(0);                           // b5
    mPkgTx.append(0);                           // b6
    mPkgTx.append(0);                           // b7
    mPkgTx.append(999 & 0xFF);                  // миллисекунды, младший байт
    mPkgTx.append((999 >> 8) & 0xFF);           // миллисекунды, старший байт
    mPkgTx.append(int2bcd(event_number % 60));  // секунды, bcd
    mPkgTx.append(int2bcd(event_number / 60));  // минуты, bcd
    mPkgTx.append(0x23);                        // часы, bcd
    mPkgTx.append(0);                           // день недели
    mPkgTx.append(0x01);                        // день, bcd
    mPkgTx.append(0x02);                        // месяц, bcd
    mPkgTx.append(0x21);                        // год, bcd

    Q_ASSERT(mPkgTx.size() == 17);  // команда + 16 байт данных
}


//
void Bsp::setRegime(int index)
{
    index = stateGlb.regime->currentData().toUInt();

    if (stateDef.regime != nullptr)
    {
        stateDef.regime->setCurrentIndex(index);
    }

    if (statePrm.regime != nullptr)
    {
        statePrm.regime->setCurrentIndex(index);
    }

    if (statePrm2.regime != nullptr)
    {
        statePrm2.regime->setCurrentIndex(index);
    }

    if (statePrd.regime != nullptr)
    {
        statePrd.regime->setCurrentIndex(index);
    }
}

//
void Bsp::setState(int index)
{
    index = stateGlb.state->currentData().toUInt();

    if (stateDef.state != nullptr)
    {
        stateDef.state->setCurrentIndex(stateDef.state->findData(index));
    }

    if (statePrm.state != nullptr)
    {
        statePrm.state->setCurrentIndex(statePrm.state->findData(index));
    }

    if (statePrm2.state != nullptr)
    {
        statePrm2.state->setCurrentIndex(statePrm2.state->findData(index));
    }

    if (statePrd.state != nullptr)
    {
        statePrd.state->setCurrentIndex(statePrd.state->findData(index));
    }
}

//
void Bsp::setDopByte(int index)
{
    index = stateGlb.dopByte->value();

    if (stateDef.dopByte != nullptr)
    {
        stateDef.dopByte->setValue(index);
    }

    if (statePrm.dopByte != nullptr)
    {
        statePrm.dopByte->setValue(index);
    }

    if (statePrm2.dopByte != nullptr)
    {
        statePrm2.dopByte->setValue(index);
    }

    if (statePrd.dopByte != nullptr)
    {
        statePrd.dopByte->setValue(index);
    }
}

//
void Bsp::updateClock()
{
    dt = dt.addSecs(1);
}

/**
 * *****************************************************************************
 *
 * @brief Bsp::SlotReadPackageFinished
 * @note slot
 *
 * Слот обработки окончания передаваемого сообщения.
 *
 * *****************************************************************************
 */
void Bsp::SlotReadPackageFinished()
{
}
