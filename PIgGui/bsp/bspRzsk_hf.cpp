#include <QDebug>
#include <QHeaderView>

#include "bspRzsk_hf.hpp"

#include "PIg/src/flash.h"
#include "PIg/src/menu/control.h"
#include "PIg/src/menu/txCom.h"
#include "PIg/src/parameter/param.h"


/**
 * *****************************************************************************
 *
 * @brief Конструктор.
 * @param[in] tree Виджет дерева параметров.
 * @param[in] parent Родительский объект.
 *
 * *****************************************************************************
 */
TBspRzskHf::TBspRzskHf(QTreeWidget *tree, QWidget *parent) : Bsp(tree, parent)
{
}


/**
 * *****************************************************************************
 *
 * @brief Настраивает карту команд.
 *
 * *****************************************************************************
 */
void TBspRzskHf::InitComMap()
{
    mComMap.insert(0x00, &Bsp::HdlrComDefx00);  // чтение параметров защиты с конфигуратора
    mComMap.insert(0x01, &Bsp::HdlrComDefx01);  // чтение типа защиты
    mComMap.insert(0x02, &Bsp::HdlrComDefx02);  // чтение количества аппаратов в линии
    mComMap.insert(0x03, &Bsp::HdlrComDefx03);  // чтение допустимого времени без манипуляции
    mComMap.insert(0x04, &Bsp::HdlrComDefx04);  // чтение компенсаций задержек на линии
    mComMap.insert(0x05, &Bsp::HdlrComDefx05);  // чтение перекртия импульсов
    mComMap.insert(0x06, &Bsp::HdlrComDefx06);  // чтение загрубления чувств.
    mComMap.insert(0x07, &Bsp::HdlrComDefx07);  // чтение количества полупериодов
    mComMap.insert(0x09, &Bsp::HdlrComDefx09);  // чтение порога предупреждения по РЗ
    mComMap.insert(0x0A, &Bsp::HdlrComDefx0A);  // чтение одностороннего режима

    mComMap.insert(0x30, &Bsp::HdlrComGlbx30);  // чтение режимов
    mComMap.insert(0x31, &Bsp::HdlrComGlbx31);  // чтение неисправностей
    mComMap.insert(0x32, &Bsp::HdlrComGlbx32);  // чтение времени
    mComMap.insert(0x33, &Bsp::HdlrComGlbx33);  // чтение коррекций тока и напряжения
    mComMap.insert(0x34, &Bsp::HdlrComGlbx34);  // чтение измерений
    mComMap.insert(0x35, &Bsp::HdlrComGlbx35);  // чтение синхр. времени / всех параметров
    mComMap.insert(0x36, &Bsp::HdlrComGlbx36);  // чтение удержания реле прм.
    mComMap.insert(0x37, &Bsp::HdlrComGlbx37);  // чтение удерж. реле прд и совмест.
    mComMap.insert(0x38, &Bsp::HdlrComGlbx38);  // чтение адреса в локальной сети
    mComMap.insert(0x39, &Bsp::HdlrComGlbx39);  // чтение времени перезапуска
    mComMap.insert(0x3A, &Bsp::HdlrComGlbx3A);  // чтение частоты
    mComMap.insert(0x3B, &Bsp::HdlrComGlbx3B);  // чтение номера аппарата
    mComMap.insert(0x3C, &Bsp::HdlrComGlbx3C);  // чтение порога предупр. КЧ и загр. чувств.
    mComMap.insert(0x3D, &Bsp::HdlrComGlbx3D);  // чтение контроля выхода
    mComMap.insert(0x3E, &Bsp::HdlrComGlbx3E);  // чтение сигналов в тесте
    mComMap.insert(0x3f, &Bsp::HdlrComGlbx3F);  // чтение информации об устройстве

    mComMap.insert(0x70, &Bsp::HdlrComRegx70);  // запись режима "Выведен"
    mComMap.insert(0x71, &Bsp::HdlrComRegx71);  // запись режима "Введен"
    //    mComMap.insert(0x72, &Bsp::HdlrComRegx72);  // запись команды управления
    mComMap.insert(0x7D, &Bsp::HdlrComRegx7D);  // запись режима "Тест 1 (ПРМ)"
    mComMap.insert(0x7E, &Bsp::HdlrComRegx7E);  // запись режима "Тест 1 (ПРД)"

    mComMap.insert(0x80, &Bsp::HdlrComDefx00);  // запись параметров защиты с конфигуратора
    mComMap.insert(0x81, &Bsp::HdlrComDefx01);  // запись типа защиты
    mComMap.insert(0x82, &Bsp::HdlrComDefx02);  // запись количества аппаратов в линии
    mComMap.insert(0x83, &Bsp::HdlrComDefx03);  // запись допустимого времени без манипуляции
    mComMap.insert(0x84, &Bsp::HdlrComDefx04);  // запись компенсаций задержек на линии
    mComMap.insert(0x85, &Bsp::HdlrComDefx05);  // запись перекрытия импульсов
    mComMap.insert(0x86, &Bsp::HdlrComDefx06);  // запись загрубления чувств.
    mComMap.insert(0x87, &Bsp::HdlrComDefx07);  // запись количества полупериодов
    mComMap.insert(0x89, &Bsp::HdlrComDefx09);  // запись порога предупреждения по РЗ
    mComMap.insert(0x8A, &Bsp::HdlrComDefx0A);  // запись одностороннего режима

    mComMap.insert(0xB0, &Bsp::HdlrComGlbx30);  // запись общих параметров с конфигуратора
    mComMap.insert(0xB2, &Bsp::HdlrComGlbx32);  // запись времени
    mComMap.insert(0xB3, &Bsp::HdlrComGlbx33);  // запись коррекций тока и напряжения
    mComMap.insert(0xB5, &Bsp::HdlrComGlbx35);  // запись синхронизации времени / всех параметров
    mComMap.insert(0xB6, &Bsp::HdlrComGlbx36);  // запись удержания реле прм
    mComMap.insert(0xB7, &Bsp::HdlrComGlbx37);  // запись удерж. реле прд и совмест.
    mComMap.insert(0xB8, &Bsp::HdlrComGlbx38);  // запись адреса в локальной сети
    mComMap.insert(0xB9, &Bsp::HdlrComGlbx39);  // запись времени перезапуска
    mComMap.insert(0xBA, &Bsp::HdlrComGlbx3A);  // запись частоты
    mComMap.insert(0xBB, &Bsp::HdlrComGlbx3B);  // запись номера аппарата
    mComMap.insert(0xBC, &Bsp::HdlrComGlbx3C);  // запись порога предупр. КЧ и загр. чувств.
    mComMap.insert(0xBD, &Bsp::HdlrComGlbx3D);  // запись контроля выхода

    mComMap.insert(0xC1, &Bsp::HdlrComJrnxC1);  // чтение количества записей в журнале защиты
    mComMap.insert(0xC2, &Bsp::HdlrComJrnxC2);  // чтение записей журнала защиты
    mComMap.insert(0xD1, &Bsp::HdlrComJrnxD1);  // чтение количества записей в журнале прм
    mComMap.insert(0xD2, &Bsp::HdlrComJrnxD2);  // чтение записей журнала прм
    mComMap.insert(0xE1, &Bsp::HdlrComJrnxE1);  // чтение количества записей в журнале прд
    mComMap.insert(0xE2, &Bsp::HdlrComJrnxE2);  // чтение записей журнала прд
    mComMap.insert(0xF1, &Bsp::HdlrComJrnxF1);  // чтение количества записей в журнале событий
    mComMap.insert(0xF2, &Bsp::HdlrComJrnxF2);  // чтение записей журнала событий
}


//
void TBspRzskHf::InitParam()
{
    setSpinBoxValue(mDevice.versionBspMcu, 0x0134);
    setSpinBoxValue(mDevice.versionBspDsp, 0x0D33);
    setSpinBoxValue(mDevice.versionBszPlis, 0x52);
    setSpinBoxValue(mDevice.versionBsk1PrdPlis, 0x25);
    setSpinBoxValue(mDevice.versionBsk2PrdPlis, 0x25);
    setSpinBoxValue(mDevice.versionBsk1PrmPlis, 0x23);
    setSpinBoxValue(mDevice.versionBsk2PrmPlis, 0x23);

    setComboBoxValue(stateGlb.regime, eGB_REGIME::GB_REGIME_ENABLED);
    setComboBoxValue(stateGlb.state, 1);  // 1 - контроль
    setSpinBoxValue(stateGlb.dopByte, 1);

    setLineEditValue(stateGlb.fault, "0000");
    setLineEditValue(stateGlb.warning, "0000");
    setLineEditValue(stateDef.fault, "0000");
    setLineEditValue(stateDef.warning, "0000");

    // Параметры интерфейса
    setSpinBoxValue(GB_PARAM_NET_ADDRESS, 17);

    // Параметры общие
    SetParamValue(GB_PARAM_COMP_RZSK, GB_COMP_RZSK);
    //    SetParamValue(GB_PARAM_NUM_OF_DEVICES, 1); - auto change
    SetParamValue(GB_PARAM_TIME_SYNCH, 1);  // 1 - вкл.
    SetParamValue(GB_PARAM_NUM_OF_DEVICE, 1);
    SetParamValue(GB_PARAM_OUT_CHECK, 0);  // 0 - выкл.
    SetParamValue(GB_PARAM_WARN_THD_CF, 11);
    SetParamValue(GB_PARAM_TIME_RERUN, 3);
    SetParamValue(GB_PARAM_COM_PRD_KEEP, 0);  // 0 - выкл.
    SetParamValue(GB_PARAM_COM_PRM_KEEP, 1);  // 1 - вкл.
    SetParamValue(GB_PARAM_IN_DEC, 12, 1);
    SetParamValue(GB_PARAM_IN_DEC, 13, 2);
    SetParamValue(GB_PARAM_FREQ, 123);
    SetParamValue(GB_PARAM_DETECTOR, 2);
    SetParamValue(GB_PARAM_COR_U, -105);  // -10.5 В
    SetParamValue(GB_PARAM_COR_I, 20);    // 20 мА

    // Параметры защиты
    SetParamValue(GB_PARAM_DEF_TYPE, 0);  // 0 - ДФЗ ПрПд
    SetParamValue(GB_PARAM_TIME_NO_MAN, 10);
    SetParamValue(GB_PARAM_OVERLAP, 28);
    SetParamValue(GB_PARAM_DELAY, 8);
    SetParamValue(GB_PARAM_WARN_THD_RZ, 13);
    SetParamValue(GB_PARAM_SENS_DEC_RZ, 31);
    SetParamValue(GB_PARAM_PRM_TYPE, 0);      // 0 - акт+пасс
    SetParamValue(GB_PARAM_DEF_ONE_SIDE, 0);  // 0 - выкл.

    // Параметры передатчика
    SetParamValue(GB_PARAM_PRD_IN_DELAY, 7);
    SetParamValue(GB_PARAM_PRD_DURATION_L, 50);
    SetParamValue(GB_PARAM_PRD_COM_LONG, 0x0F, 1);   // биты, 1 - вкл.
    SetParamValue(GB_PARAM_PRD_COM_BLOCK, 0xF0, 1);  // биты, 1 - вкл.

    // Параметры приемника
    SetParamValue(GB_PARAM_PRM_TIME_ON, 8);
    SetParamValue(GB_PARAM_PRM_COM_BLOCK, 0x07, 1);  // биты, 1 - вкл.
    SetParamValue(GB_PARAM_PRM_TIME_OFF, 250 / 10, 1);
    SetParamValue(GB_PARAM_PRM_TIME_OFF, 500 / 10, 2);
    SetParamValue(GB_PARAM_PRM_INC_SAFETY, 1);  // 1 - вкл.

    setSpinBoxValue(m_measure.I, 101);
    setSpinBoxValue(m_measure.U, 251);
    setSpinBoxValue(m_measure.Udef1, -32);
    setSpinBoxValue(m_measure.Udef2, 32);
    setSpinBoxValue(m_measure.Ucf1, -16);
    setSpinBoxValue(m_measure.Ucf2, 16);
    setSpinBoxValue(m_measure.Un1, -7);
    setSpinBoxValue(m_measure.Un2, 7);

    mJrnGlbCounter.setValue(15);
    mJrnDefCounter.setValue(16);
    mJrnPrmCounter.setValue(17);
    mJrnPrdCounter.setValue(18);
}


void TBspRzskHf::crtTreeDef(QTreeWidgetItem *top)
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Защита"));
    top->addChild(ltop);

    CrtParamWidget(ltop, GB_PARAM_DEF_TYPE);
    CrtParamWidget(ltop, GB_PARAM_TIME_NO_MAN);
    CrtParamWidget(ltop, GB_PARAM_OVERLAP);
    CrtParamWidget(ltop, GB_PARAM_DELAY);
    CrtParamWidget(ltop, GB_PARAM_WARN_THD_RZ);
    CrtParamWidget(ltop, GB_PARAM_SENS_DEC_RZ);
    CrtParamWidget(ltop, GB_PARAM_PRM_TYPE);

    // параметр Односторонний режим меняется командой управления
    CrtParamWidget(ltop, GB_PARAM_DEF_ONE_SIDE);
}


//
void TBspRzskHf::crtTreePrd(QTreeWidgetItem *top)
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Передатчик"));
    top->addChild(ltop);

    // параметр меняется автоматически при смене совместимости
    CrtParamWidget(ltop, GB_PARAM_PRD_COM_NUMS);
    mapSpinBox.value(GB_PARAM_PRD_COM_NUMS).at(0)->setEnabled(false);
    mapSpinBox.value(GB_PARAM_PRD_COM_NUMS).at(0)->setMaximum(8);

    CrtParamWidget(ltop, GB_PARAM_PRD_IN_DELAY);
    CrtParamWidget(ltop, GB_PARAM_PRD_DURATION_L);
    CrtParamWidget(ltop, GB_PARAM_PRD_COM_LONG);
    CrtParamWidget(ltop, GB_PARAM_PRD_COM_BLOCK);
}


//
void TBspRzskHf::crtTreePrm(QTreeWidgetItem *top)
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Приемник"));
    top->addChild(ltop);

    // параметр меняется автоматически при смене совместимости
    CrtParamWidget(ltop, GB_PARAM_PRM_COM_NUMS);
    mapSpinBox.value(GB_PARAM_PRM_COM_NUMS).at(0)->setEnabled(false);
    mapSpinBox.value(GB_PARAM_PRM_COM_NUMS).at(0)->setMaximum(8);

    CrtParamWidget(ltop, GB_PARAM_PRM_TIME_ON);
    CrtParamWidget(ltop, GB_PARAM_PRM_COM_BLOCK);
    CrtParamWidget(ltop, GB_PARAM_PRM_TIME_OFF);
    CrtParamWidget(ltop, GB_PARAM_PRM_INC_SAFETY);
}


void TBspRzskHf::crtTreeGlb(QTreeWidgetItem(*top))
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Общие"));
    top->addChild(ltop);

    CrtParamWidget(ltop, GB_PARAM_COMP_RZSK);
    CrtParamWidget(ltop, GB_PARAM_NUM_OF_DEVICES);
    CrtParamWidget(ltop, GB_PARAM_TIME_SYNCH);
    CrtParamWidget(ltop, GB_PARAM_NUM_OF_DEVICE);
    CrtParamWidget(ltop, GB_PARAM_OUT_CHECK);
    CrtParamWidget(ltop, GB_PARAM_WARN_THD_CF);
    CrtParamWidget(ltop, GB_PARAM_TIME_RERUN);
    CrtParamWidget(ltop, GB_PARAM_COM_PRD_KEEP);
    CrtParamWidget(ltop, GB_PARAM_COM_PRM_KEEP);
    CrtParamWidget(ltop, GB_PARAM_IN_DEC);
    CrtParamWidget(ltop, GB_PARAM_FREQ);
    CrtParamWidget(ltop, GB_PARAM_DETECTOR);
    CrtParamWidget(ltop, GB_PARAM_COR_U);
    CrtParamWidget(ltop, GB_PARAM_COR_I);

    connect(mapCombobox.value(GB_PARAM_COMP_RZSK).at(0),
            &QComboBox::currentTextChanged,
            this,
            &TBspRzskHf::SlotChangeCompatibility);
    SlotChangeCompatibility();
}


void TBspRzskHf::crtTreeInterface(QTreeWidgetItem *top)
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Интерфейс"));
    top->addChild(ltop);

    CrtParamWidget(ltop, GB_PARAM_NET_ADDRESS);
}


void TBspRzskHf::crtTreeDevice()
{
    QTreeWidgetItem *top  = new QTreeWidgetItem();
    QTreeWidgetItem *item = nullptr;

    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("Устройство"));

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Защита"));
    top->addChild(item);
    mDefEnable.setEnabled(false);
    mTree->setItemWidget(item, 1, &mDefEnable);

    crtTreeDevieVersions(top);
}


void TBspRzskHf::crtTreeDevieVersions(QTreeWidgetItem *top)
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

    item                   = new QTreeWidgetItem();
    mDevice.versionBszPlis = new QHexSpinBox(true);
    item->setText(0, kCodec->toUnicode("БСЗ ПЛИС"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBszPlis);

    item                       = new QTreeWidgetItem();
    mDevice.versionBsk1PrdPlis = new QHexSpinBox(true);
    item->setText(0, kCodec->toUnicode("БСК1 ПЛИС ПРД"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBsk1PrdPlis);

    item                       = new QTreeWidgetItem();
    mDevice.versionBsk2PrdPlis = new QHexSpinBox(true);
    item->setText(0, kCodec->toUnicode("БСК1 ПЛИС ПРД"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBsk2PrdPlis);

    item                       = new QTreeWidgetItem();
    mDevice.versionBsk1PrmPlis = new QHexSpinBox(true);
    item->setText(0, kCodec->toUnicode("БСК2 ПЛИС ПРМ"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBsk1PrmPlis);

    item                       = new QTreeWidgetItem();
    mDevice.versionBsk2PrmPlis = new QHexSpinBox(true);
    item->setText(0, kCodec->toUnicode("БСК2 ПЛИС ПРМ"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBsk2PrmPlis);

    branch->setExpanded(false);
}


void TBspRzskHf::crtTreeMeasure()
{
    QTreeWidgetItem *item   = nullptr;
    QSpinBox *       widget = nullptr;

    QTreeWidgetItem *top = new QTreeWidgetItem();
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);

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

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(-99, 99);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("Uз1"));
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

    top->setExpanded(false);
}


//
void TBspRzskHf::crtTreeState()
{
    QTreeWidgetItem *item;
    QTreeWidgetItem *top = new QTreeWidgetItem();

    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("Текущее состояние"));

    errRegExp.setPattern("[0-9A-Fa-F]+");
    errValidator.setRegExp(errRegExp);

    Bsp::crtTreeState(top, "Общее", stateGlb);
    Bsp::crtTreeState(top, "Защита", stateDef);
    Bsp::crtTreeState(top, "Приемник 1", statePrm);
    Bsp::crtTreeState(top, "Приемник 2", statePrm2);
    Bsp::crtTreeState(top, "Передатчик", statePrd);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Светодиоды БСК ПРМ"));
    top->insertChild(0, item);
    mPrmLightPA.setMinimum(0x00);
    mPrmLightPA.setMaximum(0x0F);
    mPrmLightPA.setDisplayIntegerBase(16);
    mPrmLightPA.setPrefix("0x");
    mTree->setItemWidget(item, 1, &mPrmLightPA);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Светодиоды БСК ПРД"));
    top->insertChild(0, item);
    mPrdLightPA.setMinimum(0x00);
    mPrdLightPA.setMaximum(0x0F);
    mPrdLightPA.setDisplayIntegerBase(16);
    mPrdLightPA.setPrefix("0x");
    mTree->setItemWidget(item, 1, &mPrdLightPA);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Внешние входы КВП"));
    top->insertChild(0, item);
    mExternalInputsState.setMinimum(0x00);
    mExternalInputsState.setMaximum(0x0F);
    mExternalInputsState.setDisplayIntegerBase(16);
    mExternalInputsState.setPrefix("0x");
    mTree->setItemWidget(item, 1, &mExternalInputsState);

    connect(stateGlb.regime,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &TBspRzskHf::setRegime);

    connect(stateGlb.state,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &TBspRzskHf::setState);

    connect(stateGlb.dopByte,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &TBspRzskHf::setDopByte);

    top->setExpanded(true);
}


//
void TBspRzskHf::crtTest()
{
    QTreeWidgetItem *top = new QTreeWidgetItem(mTree);
    QTreeWidgetItem *item;
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("Тест"));

    item = new QTreeWidgetItem(top);
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("Сигналы КЧ1"));
    mTree->setItemWidget(item, 1, &mTestCf1Signal);

    item = new QTreeWidgetItem(top);
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("Сигналы РЗ1"));
    mTree->setItemWidget(item, 1, &mTestRz1Signal);

    item = new QTreeWidgetItem(top);
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("Сигналы КЧ2"));
    mTree->setItemWidget(item, 1, &mTestCf2Signal);

    item = new QTreeWidgetItem(top);
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("Сигналы РЗ2"));
    mTree->setItemWidget(item, 1, &mTestRz2Signal);
}


/**
 * *****************************************************************************
 *
 * @brief Создает ветку журналов.
 *
 * *****************************************************************************
 */
void TBspRzskHf::crtJrn()
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

    ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Журнал приемника"));
    top->insertChild(0, ltop);
    crtJrnPrm(ltop);

    ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Журнал передатчика"));
    top->insertChild(0, ltop);
    crtJrnPrd(ltop);
}


/**
 * *****************************************************************************
 *
 * @brief Создает журнал защиты.
 * @param[in] top Верхний уровень.
 *
 * *****************************************************************************
 */
void TBspRzskHf::crtJrnDef(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Количество записей"));
    mTree->setItemWidget(item, 1, &mJrnDefCounter);
    mJrnDefCounter.setRange(0, 256);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Переполнение"));
    mTree->setItemWidget(item, 1, &mJrnDefOverflow);
}


/**
 * *****************************************************************************
 *
 * @brief Создает журнал событий.
 * @param[in] top Верхний уровень.
 *
 * *****************************************************************************
 */
void TBspRzskHf::crtJrnGlb(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Количество записей"));
    mTree->setItemWidget(item, 1, &mJrnGlbCounter);
    mJrnGlbCounter.setRange(0, 256);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Переполнение"));
    mTree->setItemWidget(item, 1, &mJrnGlbOverflow);
}


/**
 * *****************************************************************************
 *
 * @brief Создает журнал приемника.
 * @param[in] top Верхний уровень.
 *
 * *****************************************************************************
 */
void TBspRzskHf::crtJrnPrm(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Количество записей"));
    mTree->setItemWidget(item, 1, &mJrnPrmCounter);
    mJrnPrmCounter.setRange(0, 256);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Переполнение"));
    mTree->setItemWidget(item, 1, &mJrnPrmOverflow);
}


/**
 * *****************************************************************************
 *
 * @brief Создает журнал передатчика.
 * @param[in] top Верхний уровень.
 *
 * *****************************************************************************
 */
void TBspRzskHf::crtJrnPrd(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Количество записей"));
    mTree->setItemWidget(item, 1, &mJrnPrdCounter);
    mJrnPrdCounter.setRange(0, 256);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Переполнение"));
    mTree->setItemWidget(item, 1, &mJrnPrdOverflow);
}


//
void TBspRzskHf::FillComboboxListStateDef()
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
}


/**
 * *****************************************************************************
 *
 * @brief Заполнение списка возможных команд управления.
 *
 * @todo Изменять список команд управления в зависимости от текущей совместимости.
 *
 * *****************************************************************************
 */
void TBspRzskHf::FillComboBoxListControl()
{
    mControl.addItem(kCodec->toUnicode("0 - Нет"), 0);
    mControl.addItem(kCodec->toUnicode("1 - Сброс своего"), 1);
    mControl.addItem(kCodec->toUnicode("2 - Сброс удаленного"), 2);
    mControl.addItem(kCodec->toUnicode("3 - Сброс АК"), 3);
    mControl.addItem(kCodec->toUnicode("4 - Пуск удаленного (1)"), 4);
    mControl.addItem(kCodec->toUnicode("5 - Пуск удаленного (2)"), 5);
    mControl.addItem(kCodec->toUnicode("6 - Пуск удаленных"), 6);
    mControl.addItem(kCodec->toUnicode("7 - Вызов"), 7);
    mControl.addItem(kCodec->toUnicode("8 - Наладочный пуск вкл."), 8);
    mControl.addItem(kCodec->toUnicode("9 - Наладочный пуск выкл."), 9);
    mControl.addItem(kCodec->toUnicode("10 - Пуск АК удаленный"), 10);
    mControl.addItem(kCodec->toUnicode("11 - Сброс удаленного (1)"), 11);
    mControl.addItem(kCodec->toUnicode("12 - Сброс удаленного (2)"), 12);
    mControl.addItem(kCodec->toUnicode("13 - Пуск удаленного (3)"), 13);
    mControl.addItem(kCodec->toUnicode("14 - Режим АК"), 14);
    mControl.addItem(kCodec->toUnicode("15 - Сброс удаленного (3)"), 15);
    mControl.addItem(kCodec->toUnicode("16 - Пуск уд. МАН (1)"), 16);
    mControl.addItem(kCodec->toUnicode("17 - Пуск уд. МАН (2)"), 17);
    mControl.addItem(kCodec->toUnicode("18 - Пуск уд. МАН (3)"), 18);
    mControl.addItem(kCodec->toUnicode("19 - Пуск удаленных МАН"), 19);
}


/**
 * *****************************************************************************
 *
 * @brief Заполнение сигналов для тестов.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::FillComboboxListTest()
{
    mTestCf1Signal.addItem(kCodec->toUnicode("Нет"), 0);
    mTestCf1Signal.addItem(kCodec->toUnicode("КЧ1"), 1);
    mTestCf1Signal.addItem(kCodec->toUnicode("КЧ2"), 2);
    mTestCf1Signal.addItem(kCodec->toUnicode("Команда 1"), 3);
    mTestCf1Signal.addItem(kCodec->toUnicode("Команда 2"), 4);
    mTestCf1Signal.addItem(kCodec->toUnicode("Команда 3"), 5);
    mTestCf1Signal.addItem(kCodec->toUnicode("Команда 4"), 6);
    mTestCf1Signal.addItem(kCodec->toUnicode("Команда 5"), 7);
    mTestCf1Signal.addItem(kCodec->toUnicode("Команда 6"), 8);
    mTestCf1Signal.addItem(kCodec->toUnicode("Команда 7"), 9);
    mTestCf1Signal.addItem(kCodec->toUnicode("Команда 8"), 10);

    for (int i = 0; i < mTestCf1Signal.count(); i++)
    {
        mTestCf2Signal.addItem(mTestCf1Signal.itemText(i), mTestCf1Signal.itemData(i));
    }

    mTestRz1Signal.addItem(kCodec->toUnicode("Нет"), 0);
    mTestRz1Signal.addItem(kCodec->toUnicode("РЗ1"), 1);
    mTestRz1Signal.addItem(kCodec->toUnicode("РЗ2"), 2);

    for (int i = 0; i < mTestRz1Signal.count(); i++)
    {
        mTestRz2Signal.addItem(mTestRz1Signal.itemText(i), mTestRz1Signal.itemData(i));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи параметров защиты с конфигуратора.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComDefx00(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == 0x00 || com == 0x80);

    if (com == 0x00)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_DEF_TYPE)));
        mPkgTx.append(0);  // тип линии
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TIME_NO_MAN)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_DELAY)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_OVERLAP)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SENS_DEC_RZ)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PRM_TYPE)));
        mPkgTx.append(0);  // задержка на линии 2
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_WARN_THD_RZ)));
        mPkgTx.append(0);  // загрубление ДФЗ 2
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_DEF_ONE_SIDE)));

        int len = 16 - mPkgTx.size();
        for (int i = 0; i < len; i++)
        {
            mPkgTx.append(0);
        }

        Q_ASSERT(mPkgTx.size() == 16);  // команда + 15 байт данных
    }

    if (com == 0x80)
    {
        if (!CheckSize(com, data.size(), { 15 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        int index = 0;
        SetParamValue(GB_PARAM_DEF_TYPE, data.at(index++));
        index++;  // тип линии
        SetParamValue(GB_PARAM_TIME_NO_MAN, data.at(index++));
        SetParamValue(GB_PARAM_DELAY, data.at(index++));
        SetParamValue(GB_PARAM_OVERLAP, data.at(index++));
        SetParamValue(GB_PARAM_SENS_DEC_RZ, data.at(index++));
        SetParamValue(GB_PARAM_PRM_TYPE, data.at(index++));
        index++;  // задержка на линии 2
        SetParamValue(GB_PARAM_WARN_THD_RZ, data.at(index++));
        index++;  // загрубление ДФЗ 2
        index++;  // односторонний режимSetParamValue(GB_PARAM_DEF_ONE_SIDE)));
    }
}


///**
// * *****************************************************************************
// *
// * @brief Обрабатывает команду чтения/записи количества аппаратов в линии.
// * @param[in] Команда.
// * @param[in] Данные.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComDefx02(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_DEF_GET_LINE_TYPE || com == GB_COM_DEF_SET_LINE_TYPE);

//    if (com == GB_COM_DEF_GET_LINE_TYPE)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }

//        mPkgTx.append(com);
//        mPkgTx.append(getComboBoxValue(GB_PARAM_NUM_OF_DEVICES));

//        Q_ASSERT(mPkgTx.size() == 2);  // команда + байт данных
//    }

//    if (com == GB_COM_DEF_SET_LINE_TYPE)
//    {
//        if (!CheckSize(com, data.size(), { 1 }))
//        {
//            return;
//        }

//        // ответ на команду записи совпадает с запросом
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        setComboBoxValue(GB_PARAM_NUM_OF_DEVICES, data.at(0));
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief Обрабатывает команду чтения/записи допустимого времени без манипуляции.
// * @param[in] Команда.
// * @param[in] Данные.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComDefx03(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_DEF_GET_T_NO_MAN || com == GB_COM_DEF_SET_T_NO_MAN);

//    if (com == GB_COM_DEF_GET_T_NO_MAN)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }

//        mPkgTx.append(com);
//        mPkgTx.append(uint8_t(GetParamValue(GB_PARAM_TIME_NO_MAN)));

//        Q_ASSERT(mPkgTx.size() == 2);  // команда + байт данных
//    }

//    if (com == GB_COM_DEF_SET_T_NO_MAN)
//    {
//        if (!CheckSize(com, data.size(), { 1 }))
//        {
//            return;
//        }

//        // ответ на команду записи совпадает с запросом
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        SetParamValue(GB_PARAM_TIME_NO_MAN, data.at(0));
//    }
//}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи перекрытия импульсов.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComDefx05(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_OVERLAP || com == GB_COM_DEF_SET_OVERLAP);

    if (com == GB_COM_DEF_GET_OVERLAP)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_OVERLAP)));

        Q_ASSERT(mPkgTx.size() == 2);  // команда + 1 байт данных
    }

    if (com == GB_COM_DEF_SET_OVERLAP)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        SetParamValue(GB_PARAM_OVERLAP, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи загрубления чувствительности
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComDefx06(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_RZ_DEC || com == GB_COM_DEF_SET_RZ_DEC);

    if (com == GB_COM_DEF_GET_RZ_DEC)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SENS_DEC_RZ)));

        Q_ASSERT(mPkgTx.size() == 2);  // команда + 1 байт данных
    }

    if (com == GB_COM_DEF_SET_RZ_DEC)
    {
        if (!CheckSize(com, data.size(), { 2 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        uint8_t index = data.at(0);
        switch (index)
        {
        case 1: [[fallthrow]];
        case 2: SetParamValue(GB_PARAM_SENS_DEC_RZ, data.at(1), index); break;
        default:
            QString message = "Wrong data in command %1: %2";
            qWarning() << message.arg(com, 2, 16, QLatin1Char('0')).arg(data.at(0));
        }
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи количества полупериодов.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComDefx07(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_PRM_TYPE || com == GB_COM_DEF_SET_PRM_TYPE);

    if (com == GB_COM_DEF_GET_PRM_TYPE)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PRM_TYPE)));

        Q_ASSERT(mPkgTx.size() == 2);  // команда + 1 байт данных
    }

    if (com == GB_COM_DEF_SET_PRM_TYPE)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        SetParamValue(GB_PARAM_PRM_TYPE, data.at(0));
    }
}


///**
// * *****************************************************************************
// *
// * @brief Обрабатывает команду чтения/записи смещения частоты ПРД
// * @param[in] Команда.
// * @param[in] Данные.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComDefx08(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_DEF_GET_FREQ_PRD || com == GB_COM_DEF_SET_FREQ_PRD);

//    if (com == GB_COM_DEF_GET_FREQ_PRD)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }

//        mPkgTx.append(com);
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_FREQ_PRD)));

//        Q_ASSERT(mPkgTx.size() == 2);  // команда + 1 байт данных
//    }

//    if (com == GB_COM_DEF_SET_FREQ_PRD)
//    {
//        if (!CheckSize(com, data.size(), { 1 }))
//        {
//            return;
//        }

//        // ответ на команду записи совпадает с запросом
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        SetParamValue(GB_PARAM_FREQ_PRD, data.at(0));
//    }
//}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи порога предупреждения по РЗ
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComDefx09(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_RZ_THRESH || com == GB_COM_DEF_SET_RZ_THRESH);

    if (com == GB_COM_DEF_GET_RZ_THRESH)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_WARN_THD_RZ)));

        Q_ASSERT(mPkgTx.size() == 2);  // команда + 1 байт данных
    }

    if (com == GB_COM_DEF_SET_RZ_THRESH)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        SetParamValue(GB_PARAM_WARN_THD_RZ, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи одностороннего режима.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComDefx0A(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_TYPE_AC || com == GB_COM_DEF_SET_TYPE_AC);

    if (com == GB_COM_DEF_GET_TYPE_AC)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(GetParamValue(GB_PARAM_DEF_ONE_SIDE));

        Q_ASSERT(mPkgTx.size() == 2);  // команда + 1 байт данных
    }

    if (com == GB_COM_DEF_SET_TYPE_AC)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        SetParamValue(GB_PARAM_DEF_ONE_SIDE, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения режимов и состояний.
 * @note Команда записи исползуется для установки всех общих параметров с конфигуратора.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComGlbx30(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_SOST /*|| 0xB0*/);

    if (com == GB_COM_GET_SOST)
    {
        // В сообщении от ПИ передается температура, с ПК нет данных
        // В Р400м температура не обрабатывается
        if (!CheckSize(com, data.size(), { 0, 1 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(getComboBoxValue(stateDef.regime));
        mPkgTx.append(getComboBoxValue(stateDef.state));
        mPkgTx.append(0);
        mPkgTx.append(getComboBoxValue(statePrm.regime));
        mPkgTx.append(getComboBoxValue(statePrm.state));
        mPkgTx.append(getSpinBoxValue(statePrm.dopByte));
        mPkgTx.append(getComboBoxValue(statePrd.regime));
        mPkgTx.append(getComboBoxValue(statePrd.state));
        mPkgTx.append(getSpinBoxValue(statePrd.dopByte));
        mPkgTx.append(getComboBoxValue(statePrm2.regime));
        mPkgTx.append(getComboBoxValue(statePrm2.state));
        mPkgTx.append(getSpinBoxValue(statePrm2.dopByte));

        mPkgTx.append(getSpinBoxValue(&mPrmLightPA));
        mPkgTx.append(0);
        mPkgTx.append(0);
        mPkgTx.append(0);
        mPkgTx.append(getSpinBoxValue(&mPrdLightPA));
        mPkgTx.append(0);
        mPkgTx.append(0);
        mPkgTx.append(0);

        mPkgTx.append(getSpinBoxValue(&mExternalInputsState));

        Q_ASSERT(mPkgTx.size() == 22);  // команда + 21 байт данных
    }

    if (com == 0xB0)
    {
        if (!CheckSize(com, data.size(), { 31 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        int index = 0;
        SetParamValue(GB_PARAM_IN_DEC, data.at(index++), 1);
        SetParamValue(GB_PARAM_COM_PRM_KEEP, data.at(index++));
        SetParamValue(GB_PARAM_COM_PRD_KEEP, data.at(index++));
        SetParamValue(GB_PARAM_IN_DEC, data.at(index++), 2);
        SetParamValue(GB_PARAM_TIME_RERUN, data.at(index++));
        SetParamValue(GB_PARAM_OUT_CHECK, data.at(index++));
        SetParamValue(GB_PARAM_WARN_THD_CF, data.at(index++));
        SetParamValue(GB_PARAM_DETECTOR, data.at(index++));

        // коррекции с ПК не устанавливаются
        index++;  // Коррекция значения выходного напряжения (целая часть)
        index++;  // Коррекция значения выходного напряжения (дробная цасть)
        index++;  // Коррекция значения тока 1 (младший байт)
        index++;  // Коррекция значения тока 1 (старший байт)
        index++;  // Коррекция значения тока 2 (младший байт)
        index++;  // Коррекция значения тока 2 (старший байт)

        SetParamValue(GB_PARAM_TIME_SYNCH, data.at(index++));
        SetParamValue(GB_PARAM_NET_ADDRESS, data.at(index++));
        SetParamValue(GB_PARAM_NUM_OF_DEVICE, data.at(index++));

        quint16 freq = data.at(index++);
        freq += static_cast<quint16>(data.at(index++)) << 8;
        SetParamValue(GB_PARAM_FREQ, freq);

        SetParamValue(GB_PARAM_NUM_OF_DEVICES, data.at(index++));
        SetParamValue(GB_PARAM_COMP_RZSK, data.at(index++));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения несиправностей и предупреждений.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComGlbx31(eGB_COM com, pkg_t &data)
{
    bool    ok;
    quint16 def_alarm = getLineEditValue(stateDef.fault).toUInt(&ok, 16);
    quint16 def_warn  = getLineEditValue(stateDef.warning).toUInt(&ok, 16);
    quint16 prm_alarm = getLineEditValue(statePrm.fault).toUInt(&ok, 16);
    quint16 prm_warn  = getLineEditValue(statePrm.warning).toUInt(&ok, 16);
    quint16 prd_alarm = getLineEditValue(statePrd.fault).toUInt(&ok, 16);
    quint16 glb_alarm = getLineEditValue(stateGlb.fault).toUInt(&ok, 16);
    quint16 glb_warn  = getLineEditValue(stateGlb.warning).toUInt(&ok, 16);

    Q_ASSERT(com == GB_COM_GET_FAULT);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    mPkgTx.append(com);
    mPkgTx.append(static_cast<uint8_t>(def_alarm >> 8));
    mPkgTx.append(static_cast<uint8_t>(def_alarm));
    mPkgTx.append(static_cast<uint8_t>(def_warn >> 8));
    mPkgTx.append(static_cast<uint8_t>(def_warn));

    mPkgTx.append(static_cast<uint8_t>(prm_alarm >> 8));
    mPkgTx.append(static_cast<uint8_t>(prm_alarm));
    mPkgTx.append(static_cast<uint8_t>(prm_warn >> 8));
    mPkgTx.append(static_cast<uint8_t>(prm_warn));

    mPkgTx.append(static_cast<uint8_t>(prd_alarm >> 8));
    mPkgTx.append(static_cast<uint8_t>(prd_alarm));
    mPkgTx.append(0);
    mPkgTx.append(0);

    mPkgTx.append(static_cast<uint8_t>(glb_alarm >> 8));
    mPkgTx.append(static_cast<uint8_t>(glb_alarm));
    mPkgTx.append(static_cast<uint8_t>(glb_warn >> 8));
    mPkgTx.append(static_cast<uint8_t>(glb_warn));

    // @todo Разобраться что за коды удаленных неисправностей, в меню их нет

    mPkgTx.append(0);  // код неисправности удаленной защиты 02
    mPkgTx.append(0);  // код неисправности удаленной защиты 01
    mPkgTx.append(0);  // код неисправности удаленного приемника 02
    mPkgTx.append(0);  // код неисправности удаленного приемника 01

    mPkgTx.append(0);  // код неисправности удаленного передатчика 02
    mPkgTx.append(0);  // код неисправности удаленнного передатчика 01
    mPkgTx.append(0);  // код неисправности удаленного общий 02
    mPkgTx.append(0);  // код неисправности удаленного общий 01

    Q_ASSERT(mPkgTx.size() == 25);  // команда + 24 байт данных
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи даты и времени.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComGlbx32(eGB_COM com, pkg_t &data)
{
    uint16_t msec = dt.time().msec();

    Q_ASSERT(com == GB_COM_GET_TIME || com == GB_COM_SET_TIME);

    if (com == GB_COM_GET_TIME)
    {
        // C ПИ передается флаг запроса сообщения для передачи в АСУ (0..3), с ПК нет данных
        // В Р400м флаг запроса не обрабатывается
        if (!CheckSize(com, data.size(), { 0, 1 }))
        {
            return;
        }

        // @todo Добавить обработчик команды запроса времени с байтом чтения журналов
        // Для запроса журналов передается один байт данных:
        // 0 - передать только дату и время
        // 1 - повторить последнюю запись журнала
        // 2 - передать новую запись журнала (предыдущая принята)
        // иначе в ответ возвращается команда ошибки 55 AA FF 00 FF

        mPkgTx.append(com);
        mPkgTx.append(int2bcd(dt.date().year() - 2000));
        mPkgTx.append(int2bcd(dt.date().month()));
        mPkgTx.append(int2bcd(dt.date().day()));
        mPkgTx.append(int2bcd(dt.time().hour()));
        mPkgTx.append(int2bcd(dt.time().minute()));
        mPkgTx.append(int2bcd(dt.time().second()));
        mPkgTx.append(static_cast<uint8_t>(msec));
        mPkgTx.append(static_cast<uint8_t>(msec >> 8));

        Q_ASSERT(mPkgTx.size() == 9);  // команда + 8 байт данных
    }

    if (com == GB_COM_SET_TIME)
    {
        // с ПК передается 6 байт, без мс / c МК передается 9 байт
        if (!CheckSize(com, data.size(), { 6, 9 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        // @todo Добавить обработку флага источника синхронизации времени
        // При установке времени надо проверить флаг источника:
        // - 0, изменение с клавиатуры/пк
        // - 1, изменение по АСУ, надо учитывать параметр "Синхр. времени"

        quint16 year  = bcd2int(data.takeFirst()) + 2000;
        quint8  month = bcd2int(data.takeFirst());
        quint8  day   = bcd2int(data.takeFirst());

        quint8 hour   = bcd2int(data.takeFirst());
        quint8 minute = bcd2int(data.takeFirst());
        quint8 second = bcd2int(data.takeFirst());

        quint16 ms = 0;
        if (data.size() >= 3)
        {
            ms = data.takeFirst();
            ms += static_cast<quint16>(data.takeFirst()) << 8;

            quint8 source = data.takeFirst();
            if (source > 1)
            {
                qWarning() << kMsgTimeSourceError.arg(source);
            }
        }

        dt.setDate(QDate(year, month, day));
        dt.setTime(QTime(hour, minute, second, ms));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения измерений.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComGlbx34(eGB_COM com, pkg_t &data)
{
    qint16 value = 0;

    Q_ASSERT(com == GB_COM_GET_MEAS);

    // передается код необходимых измерений
    if (!CheckSize(com, data.size(), { 1 }))
    {
        return;
    }

    // в запросе всегда 0, т.е. запрос всех измерений
    if (data.at(0) != 0)
    {
        QString message = "Wrong data in command %1: %2";
        qWarning() << message.arg(com, 2, 16, QLatin1Char('0')).arg(data.at(0));
        return;
    }

    mPkgTx.append(com);
    mPkgTx.append(data.at(0));

    value = getSpinBoxValue(m_measure.R);
    mPkgTx.append(static_cast<quint8>(value >> 8));
    mPkgTx.append(static_cast<quint8>(value));

    value = getSpinBoxValue(m_measure.I);
    value += GetParamValue(GB_PARAM_COR_I);
    mPkgTx.append(static_cast<quint8>(value >> 8));
    mPkgTx.append(static_cast<quint8>(value));

    value = getSpinBoxValue(m_measure.U);
    value += GetParamValue(GB_PARAM_COR_U);
    mPkgTx.append(static_cast<quint8>(value / 10));
    mPkgTx.append(static_cast<quint8>((value % 10) * 10));

    // @todo Разобраться нужен ли Uз2, в меню не выводится
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Udef1)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Udef2)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Ucf1)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Ucf2)));

    // В БСП: Коэфф. использ. динамич. диапазона ПРМ
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Un1)));  // K_Din;
    // В БСП: Коэфф. переполнения АЦП
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Un2)));  // K_Over
    // В БСП: Вероятность пропуска команд
    mPkgTx.append(0);  // P_PA_High
    mPkgTx.append(0);  // P_PA_Low

    Q_ASSERT(mPkgTx.size() == 16);  // команда + 15 байт данных
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи синхронизации времени.
 * @note Эта же команда используется для чтения/записи всех параметров приемника с конфигуратора
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComGlbx35(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_TIME_SINCHR || com == GB_COM_SET_TIME_SINCHR);

    if (com == GB_COM_GET_TIME_SINCHR)
    {
        // При считывании всех параметров передается один байт данных
        // При считывании синхронизации времени данных нет
        if (!CheckSize(com, data.size(), { 0, 1 }))
        {
            qDebug() << Qt::hex << com << ", " << data;
            return;
        }

        if (data.size() == 1)
        {
            mPkgTx.append(com);
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_IN_DEC, 1)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_COM_PRM_KEEP)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_COM_PRD_KEEP)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_IN_DEC, 2)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TIME_RERUN)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_OUT_CHECK)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_WARN_THD_CF)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_DETECTOR)));

            qint16 corr_u = GetParamValue(GB_PARAM_COR_U);
            mPkgTx.append(corr_u / 10);
            mPkgTx.append((corr_u % 10) * 10);

            qint16 corr_i = GetParamValue(GB_PARAM_COR_I);
            mPkgTx.append(static_cast<uint8_t>(corr_i));
            mPkgTx.append(static_cast<uint8_t>(corr_i >> 8));
            mPkgTx.append(0);  // Коррекция значения тока 2, младший байт
            mPkgTx.append(0);  // Коррекция значения тока 2, старший байт

            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TIME_SYNCH)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_NET_ADDRESS)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_NUM_OF_DEVICE)));

            quint16 freq = GetParamValue(GB_PARAM_FREQ);
            mPkgTx.append(static_cast<uint8_t>(freq));
            mPkgTx.append(static_cast<uint8_t>(freq >> 8));

            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_NUM_OF_DEVICES)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_COMP_RZSK)));

            int len = 32 - mPkgTx.size();
            for (int i = 0; i < len; i++)
            {
                mPkgTx.append(0);
            }

            Q_ASSERT(mPkgTx.size() == 32);  // команда + 31 байт данных
        }
        else
        {
            mPkgTx.append(com);
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TIME_SYNCH)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_DETECTOR)));

            Q_ASSERT(mPkgTx.size() == 3);  // команда + 2 байт данных
        }
    }

    if (com == GB_COM_SET_TIME_SINCHR)
    {
        if (!CheckSize(com, data.size(), { 1, 2 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        int param = (data.size() == 1) ? 1 : data.at(1);

        switch (param)
        {
        case 1: SetParamValue(GB_PARAM_TIME_SYNCH, data.at(0)); break;
        case 2: SetParamValue(GB_PARAM_DETECTOR, data.at(0)); break;
        default:
            {
                QString message("Wrong value in command %1: %2");
                qWarning() << message.arg(com, 2, 16, QLatin1Char('0')).arg(param);
            }
        }
    }
}

/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи удержания реле команд приемника.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComGlbx36(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_COM_PRM_KEEP || com == GB_COM_SET_COM_PRM_KEEP);

    if (com == GB_COM_GET_COM_PRM_KEEP)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_COM_PRM_KEEP)));

        Q_ASSERT(mPkgTx.size() == 2);  // команда + 1 байт данных
    }

    if (com == GB_COM_SET_COM_PRM_KEEP)
    {
        // используется только первый байт
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        SetParamValue(GB_PARAM_COM_PRM_KEEP, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи удержания реле команд прд и совместимости.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComGlbx37(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_COM_PRD_KEEP || com == GB_COM_SET_COM_PRD_KEEP);

    if (com == GB_COM_GET_COM_PRD_KEEP)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_COM_PRD_KEEP)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_COMP_RZSK)));

        Q_ASSERT(mPkgTx.size() == 3);  // команда + 2 байта данных
    }

    if (com == GB_COM_SET_COM_PRD_KEEP)
    {
        // используется только первый байт
        if (!CheckSize(com, data.size(), { 1, 2 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        int param = (data.size() == 1) ? 1 : data.at(1);

        switch (param)
        {
        case 1: SetParamValue(GB_PARAM_COM_PRD_KEEP, data.at(0)); break;
        case 2: SetParamValue(GB_PARAM_COMP_RZSK, data.at(0)); break;
        default:
            {
                QString message("Wrong value in command %1: %2");
                qWarning() << message.arg(com, 2, 16, QLatin1Char('0')).arg(param);
            }
        }
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи времени перезапуска.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComGlbx39(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_TIME_RERUN || com == GB_COM_SET_TIME_RERUN);

    if (com == GB_COM_GET_TIME_RERUN)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TIME_RERUN)));

        Q_ASSERT(mPkgTx.size() == 2);  // команда + 1 байт данных
    }

    if (com == GB_COM_SET_TIME_RERUN)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        SetParamValue(GB_PARAM_TIME_RERUN, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи порога предупреждения КЧ и загруб. чувств.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComGlbx3C(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_CF_THRESHOLD || com == GB_COM_SET_CF_THRESHOLD);

    if (com == GB_COM_GET_CF_THRESHOLD)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_WARN_THD_CF)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_IN_DEC, 1)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_IN_DEC, 2)));

        Q_ASSERT(mPkgTx.size() == 4);  // команда + 3 байта данных
    }

    if (com == GB_COM_SET_CF_THRESHOLD)
    {
        if (!CheckSize(com, data.size(), { 2 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);


        int index = data.at(0);
        switch (index)
        {
        case 1: SetParamValue(GB_PARAM_WARN_THD_CF, data.at(1)); break;
        case 2: SetParamValue(GB_PARAM_IN_DEC, data.at(1), 1); break;
        case 3: SetParamValue(GB_PARAM_IN_DEC, data.at(1), 2); break;
        default:
            {
                QString message("Wrong value in command %1: %2");
                qWarning() << message.arg(com, 2, 16, QLatin1Char('0')).arg(index);
            }
        }
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения сигналов в тестах.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComGlbx3E(eGB_COM com, pkg_t &data)
{
    quint8 value;
    quint8 cf1, cf2;
    quint8 rz1, rz2;
    quint8 cm1, cm2;

    Q_ASSERT(com == GB_COM_GET_TEST);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    value = mTestCf1Signal.currentData().toUInt();
    cf1   = ((value > 0) && (value < 3)) ? (1 << (value - 1)) : (0);
    cm1   = (value >= 3) ? (1 << (value - 3)) : (0);

    value = mTestCf2Signal.currentData().toUInt();
    cf2   = ((value > 0) && (value < 3)) ? (1 << (value - 1)) : (0);
    cm2   = (value >= 3) ? (1 << (value - 3)) : (0);

    value = mTestRz1Signal.currentData().toUInt();
    rz1   = ((value > 0) && (value < 3)) ? (1 << (value - 1)) : (0);

    value = mTestRz2Signal.currentData().toUInt();
    rz2   = ((value > 0) && (value < 3)) ? (1 << (value - 1)) : (0);

    mPkgTx.append(com);
    mPkgTx.append(((rz1 & 0x03) << 2) | (cf1 & 0x03));
    mPkgTx.append(cm1);
    mPkgTx.append(0);
    mPkgTx.append(((rz2 & 0x03) << 2) | (cf2 & 0x03));
    mPkgTx.append(cm2);
    mPkgTx.append(0);

    Q_ASSERT(mPkgTx.size() == 7);  // команда + 6 байт данных
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения версии устройства.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComGlbx3F(eGB_COM com, pkg_t &data)
{
    quint16 vers_bsp_mcu = getSpinBoxValue(mDevice.versionBspMcu);
    quint16 vers_bsp_dsp = getSpinBoxValue(mDevice.versionBspDsp);

    Q_ASSERT(com == GB_COM_GET_VERS);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    mPkgTx.append(com);
    mPkgTx.append(mDefEnable.isChecked() ? 1 : 0);        // защита
    mPkgTx.append(GetParamValue(GB_PARAM_PRM_COM_NUMS));  // прм1
    mPkgTx.append(0);                                     // прм2
    mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_NUMS));  // прд
    mPkgTx.append(GetParamValue(GB_PARAM_NUM_OF_DEVICES) + 1);
    mPkgTx.append(GB_TYPE_LINE_UM);
    mPkgTx.append(static_cast<quint8>(vers_bsp_mcu >> 8));
    mPkgTx.append(static_cast<quint8>(vers_bsp_mcu));
    mPkgTx.append(static_cast<quint8>(vers_bsp_dsp >> 8));
    mPkgTx.append(static_cast<quint8>(vers_bsp_dsp));
    mPkgTx.append(GetParamValue(GB_PARAM_COMP_RZSK));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(mDevice.versionBsk1PrdPlis)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(mDevice.versionBsk2PrdPlis)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(mDevice.versionBsk1PrmPlis)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(mDevice.versionBsk2PrmPlis)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(mDevice.versionBszPlis)));
    mPkgTx.append(AVANT_RZSK);

    Q_ASSERT(mPkgTx.size() == 18);  // команда + 17 байт данных
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду изменения режима на Тест 1 (передатчик)
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComRegx7E(eGB_COM com, pkg_t &data)
{

    Q_ASSERT(com == GB_COM_SET_REG_TEST_1);

    if (!CheckSize(com, data.size(), { 0, 2 }))
    {
        return;
    }

    if (data.size() == 0)
    {
        stateGlb.regime->setCurrentIndex(GB_REGIME_TEST_1);
        stateGlb.state->setCurrentIndex(11);

        mTestCf1Signal.setCurrentIndex(0);
        mTestRz1Signal.setCurrentIndex(0);

        mPkgTx.append(com);
        mPkgTx.append(0);
        mPkgTx.append(0);
        mPkgTx.append(0);
    }
    else
    {
        switch (data.at(0))
        {
        case 1:
            {
                quint8 cf    = data.at(1);
                int    index = mTestCf1Signal.findData(cf);

                if (index < 0)
                {
                    QString message = "Wrong test CF value: %1";
                    qWarning() << message.arg(cf);
                }

                mTestCf1Signal.setCurrentIndex(index);

                quint32 value = mTestCf1Signal.currentData().toUInt();
                if (value > 1)
                {
                    value = 1 << value;
                }

                // ответ
                mPkgTx.append(com);
                mPkgTx.append(static_cast<quint8>(value));
                mPkgTx.append(static_cast<quint8>(value >> 8));
                mPkgTx.append(static_cast<quint8>(value >> 16));
                break;
            }
        case 2:
            {
                quint8 rz    = data.at(1);
                int    index = mTestRz1Signal.findData(rz);

                if (index < 0)
                {
                    QString message = "Wrong test RZ value: %1";
                    qWarning() << message.arg(rz);
                }

                mTestRz1Signal.setCurrentIndex(index);

                quint32 value = mTestCf1Signal.currentData().toUInt();
                if (value > 1)
                {
                    value = 1 << value;
                }

                // ответ
                mPkgTx.append(com);
                mPkgTx.append(static_cast<quint8>(value));
                mPkgTx.append(0);
                mPkgTx.append(0);
                break;
            }
        default:
            QString message = "Wrong test group value: %1";
            qWarning() << message.arg(data.at(0));
        }
    }

    Q_ASSERT(mPkgTx.size() == 4);  // команда + 3 байт данных
}


/**
 * *****************************************************************************
 *
 * @brief Обработчик изменения совместимости.
 *
 * *****************************************************************************
 */
void TBspRzskHf::SlotChangeCompatibility()
{
    eGB_COMP_RZSK comp = static_cast<eGB_COMP_RZSK>(GetParamValue(GB_PARAM_COMP_RZSK));

    switch (comp)
    {
    case GB_COMP_RZSK: [[fallthrow]];
    case GB_COMP_RZSK_M:
        mDefEnable.setChecked(true);
        SetParamValue(GB_PARAM_PRD_COM_NUMS, 4 / 4);
        SetParamValue(GB_PARAM_PRM_COM_NUMS, 4 / 4);
        mapCombobox.value(GB_PARAM_NUM_OF_DEVICES).at(0)->setEnabled(true);
        break;
    case GB_COMP_RZSK_3E8:
        mDefEnable.setChecked(false);
        SetParamValue(GB_PARAM_PRD_COM_NUMS, 8 / 4);
        SetParamValue(GB_PARAM_PRM_COM_NUMS, 8 / 4);
        SetParamValue(GB_PARAM_NUM_OF_DEVICES, GB_NUM_DEVICES_3);
        mapCombobox.value(GB_PARAM_NUM_OF_DEVICES).at(0)->setEnabled(false);
        break;

    case GB_COMP_RZSK_MAX:
        QString message = "Wrong RZSK compatibility value: %1";
        qWarning() << message.arg(comp);
        break;
    }
}


///**
// * *****************************************************************************
// *
// * @brief Обрабатывает команду установки режима "Выведен".
// * @param[in] Команда.
// * @param[in] Данные.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComRegx70(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_SET_REG_DISABLED);

//    if (!CheckSize(com, data.size(), { 0 }))
//    {
//        return;
//    }

//    // ответ на команду записи совпадает с запросом
//    mPkgTx.append(com);
//    mPkgTx.append(data);

//    setComboBoxValue(stateGlb.regime, eGB_REGIME::GB_REGIME_DISABLED);
//}


///**
// * *****************************************************************************
// *
// * @brief Обрабатывает команду установки режима "Введен".
// * @param[in] Команда.
// * @param[in] Данные.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComRegx71(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_SET_REG_ENABLED);

//    if (!CheckSize(com, data.size(), { 0 }))
//    {
//        return;
//    }

//    // ответ на команду записи совпадает с запросом
//    mPkgTx.append(com);
//    mPkgTx.append(data);

//    setComboBoxValue(stateGlb.regime, eGB_REGIME::GB_REGIME_ENABLED);
//}


///**
// * *****************************************************************************
// *
// * @brief Обрабатывает команду управления.
// * @param[in] Команда.
// * @param[in] Данные.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComRegx72(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_SET_CONTROL);

//    if (!CheckSize(com, data.size(), { 1 }))
//    {
//        return;
//    }

//    // ответ на команду записи совпадает с запросом
//    mPkgTx.append(com);
//    mPkgTx.append(data);

//    uint8_t control = data.takeFirst();
//    if (control >= mControl.count())
//    {
//        QString message("Wrong value in command %1: %2");
//        qWarning() << message.arg(com, 2, 16, QLatin1Char('0')).arg(control);
//    }

//    if (control == 8)
//    {
//        setComboBoxValue(stateDef.state, 7);  // Пуск налад. вкл
//    }
//    else if (control == 9)
//    {
//        setComboBoxValue(stateDef.state, 1);  // Пуск налад. выкл
//    }

//    mControl.setCurrentIndex(mControl.findData(control));
//}
