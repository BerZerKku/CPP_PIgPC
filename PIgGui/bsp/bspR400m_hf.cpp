#include <QDebug>
#include <QHeaderView>

#include "bspR400m_hf.hpp"

#include "PIg/src/flash.h"
#include "PIg/src/menu/control.h"
#include "PIg/src/menu/txCom.h"
#include "PIg/src/parameter/param.h"

TBspR400mHf::TBspR400mHf(QTreeWidget *tree, QWidget *parent) : Bsp(tree, parent)
{
    FillComboboxListAc();
}


/**
 * *****************************************************************************
 *
 * @brief Настраивает карту команд.
 *
 * *****************************************************************************
 */
void TBspR400mHf::InitComMap()
{
    mComMap.insert(0x00, &Bsp::HdlrComDefx00);  // чтение параметров защиты с конфигуратора
    mComMap.insert(0x01, &Bsp::HdlrComDefx01);  // чтение типа защиты
    mComMap.insert(0x02, &Bsp::HdlrComDefx02);  // чтение количества аппаратов в линии
    mComMap.insert(0x03, &Bsp::HdlrComDefx03);  // чтение допустимого времени без манипуляции
                                                // 0x04 - команды нет
    mComMap.insert(0x05, &Bsp::HdlrComDefx05);  // чтение сдвигов ПРД и ПРМ
    mComMap.insert(0x06, &Bsp::HdlrComDefx06);  // чтение загрубления чувств.
    mComMap.insert(0x07, &Bsp::HdlrComDefx07);  // чтение снижения уровня АК
    mComMap.insert(0x08, &Bsp::HdlrComDefx08);  // чтение смещения частоты ПРД
    mComMap.insert(0x09, &Bsp::HdlrComDefx09);  // чтение смещения частоты ПРМ
    mComMap.insert(0x0A, &Bsp::HdlrComDefx0A);  // чтение режима АК и времени до АК

    mComMap.insert(0x30, &Bsp::HdlrComGlbx30);  // чтение режимов
    mComMap.insert(0x31, &Bsp::HdlrComGlbx31);  // чтение неисправностей
    mComMap.insert(0x32, &Bsp::HdlrComGlbx32);  // чтение времени
    mComMap.insert(0x33, &Bsp::HdlrComGlbx33);  // чтение коррекций тока и напряжения
    mComMap.insert(0x34, &Bsp::HdlrComGlbx34);  // чтение измерений
    mComMap.insert(0x35, &Bsp::HdlrComGlbx35);  // чтение синхронизации времени / всех параметров
    mComMap.insert(0x36, &Bsp::HdlrComGlbx36);  // чтение уровня выходного напряжения
    mComMap.insert(0x37, &Bsp::HdlrComGlbx37);  // чтение совместимости
    mComMap.insert(0x38, &Bsp::HdlrComGlbx38);  // чтение адреса в локальной сети
    mComMap.insert(0x39, &Bsp::HdlrComGlbx39);  // чтение параметров ПВЗУ-Е
    mComMap.insert(0x3A, &Bsp::HdlrComGlbx3A);  // чтение частоты
    mComMap.insert(0x3B, &Bsp::HdlrComGlbx3B);  // чтение номера аппарата
    mComMap.insert(0x3C, &Bsp::HdlrComGlbx3C);  // чтение порога КЧ
    mComMap.insert(0x3D, &Bsp::HdlrComGlbx3D);  // чтение контроля выхода
    mComMap.insert(0x3E, &Bsp::HdlrComGlbx3E);  // чтение сигналов в тесте
    mComMap.insert(0x3f, &Bsp::HdlrComGlbx3F);  // чтение информации об устройстве

    mComMap.insert(0x70, &Bsp::HdlrComRegx70);  // запись режима "Выведен"
    mComMap.insert(0x71, &Bsp::HdlrComRegx71);  // запись режима "Введен"
    mComMap.insert(0x72, &Bsp::HdlrComRegx72);  // запись команды управления
    mComMap.insert(0x7D, &Bsp::HdlrComRegx7D);  // запись режима "Тест 1 (ПРМ)"
    mComMap.insert(0x7E, &Bsp::HdlrComRegx7E);  // запись режима "Тест 1 (ПРД)"

    mComMap.insert(0x80, &Bsp::HdlrComDefx00);  // запись параметров защиты с конфигуратора
    mComMap.insert(0x81, &Bsp::HdlrComDefx01);  // запись типа защиты
    mComMap.insert(0x82, &Bsp::HdlrComDefx02);  // запись количества аппаратов в линии
    mComMap.insert(0x83, &Bsp::HdlrComDefx03);  // запись допустимого времени без манипуляции
                                                // 0x84 - команды нет
    mComMap.insert(0x85, &Bsp::HdlrComDefx05);  // запись сдвигов ПРД и ПРМ
    mComMap.insert(0x86, &Bsp::HdlrComDefx06);  // запись загрубления чувств.
    mComMap.insert(0x87, &Bsp::HdlrComDefx07);  // запись снижения уровня АК
    mComMap.insert(0x88, &Bsp::HdlrComDefx08);  // запись смещения частоты ПРД
    mComMap.insert(0x89, &Bsp::HdlrComDefx09);  // запись смещения частоты ПРМ
    mComMap.insert(0x8A, &Bsp::HdlrComDefx0A);  // запись режима АК и времени до АК

    mComMap.insert(0xB0, &Bsp::HdlrComGlbx30);  // запись общих параметров с конфигуратора
    mComMap.insert(0xB2, &Bsp::HdlrComGlbx32);  // запись времени
    mComMap.insert(0xB3, &Bsp::HdlrComGlbx33);  // записькоррекций тока и напряжения
    mComMap.insert(0xB5, &Bsp::HdlrComGlbx35);  // запись синхронизации времени / всех параметров
    mComMap.insert(0xB6, &Bsp::HdlrComGlbx36);  // запись уровня выходного напряжения
    mComMap.insert(0xB7, &Bsp::HdlrComGlbx37);  // запись совместимости
    mComMap.insert(0xB8, &Bsp::HdlrComGlbx38);  // запись адреса в локальной сети
    mComMap.insert(0xB9, &Bsp::HdlrComGlbx39);  // запись параметров ПВЗУ-Е
    mComMap.insert(0xBA, &Bsp::HdlrComGlbx3A);  // чтение частоты
    mComMap.insert(0xBB, &Bsp::HdlrComGlbx3B);  // запись номера аппарата
    mComMap.insert(0xBC, &Bsp::HdlrComGlbx3C);  // запись порога КЧ
    mComMap.insert(0xBD, &Bsp::HdlrComGlbx3D);  // запись контроля выхода

    mComMap.insert(0xC1, &Bsp::HdlrComJrnxC1);  // чтение количества записей в журнале защиты
    mComMap.insert(0xC2, &Bsp::HdlrComJrnxC2);  // чтение записей журнала защиты
    mComMap.insert(0xF1, &Bsp::HdlrComJrnxF1);  // чтение количества записей в журнале событий
    mComMap.insert(0xF2, &Bsp::HdlrComJrnxF2);  // чтение записей журнала событий
}


//
void TBspR400mHf::InitParam()
{
    setComboBoxValue(GB_PARAM_COMP_P400, GB_COMP_R400M_LINER);

    setSpinBoxValue(mDevice.versionBspMcu, 0xF233);
    setSpinBoxValue(mDevice.versionBspDsp, 0x5833);
    setSpinBoxValue(mDevice.versionBszPlis, 0x52);

    setComboBoxValue(stateGlb.regime, eGB_REGIME::GB_REGIME_ENABLED);
    setComboBoxValue(stateGlb.state, 1);  // 1 - контроль
    setSpinBoxValue(stateGlb.dopByte, 1);
    setComboBoxValue(&mAc, GB_TYPE_AC_AUTO_NORM);
    mAcTime.setValue(123);

    setLineEditValue(stateGlb.fault, "0000");
    setLineEditValue(stateGlb.warning, "0000");
    setLineEditValue(stateDef.fault, "0000");
    setLineEditValue(stateDef.warning, "0000");
    setSpinBoxValue(&mStateFaultDeviceNumber, 0);

    // Параметры интерфейса
    setSpinBoxValue(GB_PARAM_NET_ADDRESS, 17);

    // Параметры общие
    SetParamValue(GB_PARAM_COMP_P400, GB_COMP_R400M_R400);
    SetParamValue(GB_PARAM_TIME_SYNCH_SRC, 1);  // 1 - по каналу связи
    SetParamValue(GB_PARAM_NUM_OF_DEVICE, 1);
    SetParamValue(GB_PARAM_OUT_CHECK, 0);  // 0 - выкл.
    SetParamValue(GB_PARAM_WARN_THD, 11);
    SetParamValue(GB_PARAM_U_OUT_NOM, 20);
    SetParamValue(GB_PARAM_FREQ, 123);
    SetParamValue(GB_PARAM_IN_DEC_AC_ANSWER, 7);
    SetParamValue(GB_PARAM_PVZUE_PROTOCOL, 1);  // 1 - быстрый
    SetParamValue(GB_PARAM_PVZUE_PARITY, 2);    // 2 - выкл.
    SetParamValue(GB_PARAM_PVZUE_FAIL, 20);
    SetParamValue(GB_PARAM_PVZUE_NOISE_THD, 33);
    SetParamValue(GB_PARAM_PVZUE_NOISE_LVL, 40);
    SetParamValue(GB_PARAM_PVZUE_AC_TYPE, 2);  // 2 - нормальный
    SetParamValue(GB_PARAM_PVZUE_AC_PERIOD, 152);
    SetParamValue(GB_PARAM_PVZUE_AC_PER_RE, 213);
    SetParamValue(GB_PARAM_PVZU_AC_CORRECT, 19);
    SetParamValue(GB_PARAM_ALARM_CF, 11);
    SetParamValue(GB_PARAM_COR_U, -105);  // -10.5 В
    SetParamValue(GB_PARAM_COR_I, 20);    // 20 мА

    // Параметры защиты
    SetParamValue(GB_PARAM_NUM_OF_DEVICES, GB_NUM_DEVICES_2);
    SetParamValue(GB_PARAM_DEF_TYPE, 0);  // 0 - ДФЗ ПрПд
    SetParamValue(GB_PARAM_TIME_NO_MAN, 10);
    SetParamValue(GB_PARAM_SHIFT_FRONT, 18);
    SetParamValue(GB_PARAM_SHIFT_BACK, 36);
    SetParamValue(GB_PARAM_SHIFT_PRM, 48);
    SetParamValue(GB_PARAM_SHIFT_PRD, 64);
    SetParamValue(GB_PARAM_SENS_DEC, 6);
    SetParamValue(GB_PARAM_AC_IN_DEC, 0);  // 0 - выкл
    SetParamValue(GB_PARAM_FREQ_PRD, 1);   // -500 Гц
    SetParamValue(GB_PARAM_FREQ_PRM, 3);   // 500 Гц

    setSpinBoxValue(m_measure.I, 101);
    setSpinBoxValue(m_measure.U, 251);
    setSpinBoxValue(m_measure.Udef1, -32);
    setSpinBoxValue(m_measure.Udef2, 32);
    setSpinBoxValue(m_measure.Ucf1, -16);
    setSpinBoxValue(m_measure.Ucf2, 16);
    setSpinBoxValue(m_measure.Un1, -7);
    setSpinBoxValue(m_measure.Un2, 7);
    setSpinBoxValue(m_measure.Sd, 321);

    mJrnDefCounter.setValue(15);
    mJrnGlbCounter.setValue(15);
}


void TBspR400mHf::crtTreeDef(QTreeWidgetItem *top)
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Защита"));
    top->addChild(ltop);

    CrtParamWidget(ltop, GB_PARAM_NUM_OF_DEVICES);
    CrtParamWidget(ltop, GB_PARAM_DEF_TYPE);
    CrtParamWidget(ltop, GB_PARAM_TIME_NO_MAN);
    CrtParamWidget(ltop, GB_PARAM_SHIFT_FRONT);
    CrtParamWidget(ltop, GB_PARAM_SHIFT_BACK);
    CrtParamWidget(ltop, GB_PARAM_SHIFT_PRM);
    CrtParamWidget(ltop, GB_PARAM_SHIFT_PRD);
    CrtParamWidget(ltop, GB_PARAM_SENS_DEC);
    CrtParamWidget(ltop, GB_PARAM_AC_IN_DEC);
    CrtParamWidget(ltop, GB_PARAM_FREQ_PRD);
    CrtParamWidget(ltop, GB_PARAM_FREQ_PRM);
}


void TBspR400mHf::crtTreeGlb(QTreeWidgetItem(*top))
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Общие"));
    top->addChild(ltop);

    CrtParamWidget(ltop, GB_PARAM_COMP_P400);
    CrtParamWidget(ltop, GB_PARAM_TIME_SYNCH_SRC);
    CrtParamWidget(ltop, GB_PARAM_NUM_OF_DEVICE);
    CrtParamWidget(ltop, GB_PARAM_OUT_CHECK);
    CrtParamWidget(ltop, GB_PARAM_WARN_THD);
    CrtParamWidget(ltop, GB_PARAM_U_OUT_NOM);
    CrtParamWidget(ltop, GB_PARAM_FREQ);
    CrtParamWidget(ltop, GB_PARAM_IN_DEC_AC_ANSWER);
    CrtParamWidget(ltop, GB_PARAM_PVZUE_PROTOCOL);
    CrtParamWidget(ltop, GB_PARAM_PVZUE_PARITY);
    CrtParamWidget(ltop, GB_PARAM_PVZUE_FAIL);
    CrtParamWidget(ltop, GB_PARAM_PVZUE_NOISE_THD);
    CrtParamWidget(ltop, GB_PARAM_PVZUE_NOISE_LVL);
    CrtParamWidget(ltop, GB_PARAM_PVZUE_AC_TYPE);
    CrtParamWidget(ltop, GB_PARAM_PVZUE_AC_PERIOD);
    CrtParamWidget(ltop, GB_PARAM_PVZUE_AC_PER_RE);
    CrtParamWidget(ltop, GB_PARAM_PVZU_AC_CORRECT);
    CrtParamWidget(ltop, GB_PARAM_ALARM_CF);
    CrtParamWidget(ltop, GB_PARAM_COR_U);
    CrtParamWidget(ltop, GB_PARAM_COR_I);
}


void TBspR400mHf::crtTreeInterface(QTreeWidgetItem *top)
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Интерфейс"));
    top->addChild(ltop);

    CrtParamWidget(ltop, GB_PARAM_NET_ADDRESS);
}


void TBspR400mHf::crtTreeDevice()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("Устройство"));

    crtTreeDevieVersions(top);
}


void TBspR400mHf::crtTreeDevieVersions(QTreeWidgetItem *top)
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

    branch->setExpanded(false);
}


void TBspR400mHf::crtTreeMeasure()
{
    QTreeWidgetItem *item   = nullptr;
    QSpinBox *       widget = nullptr;

    QTreeWidgetItem *top = new QTreeWidgetItem();
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);

    top->setText(0, kCodec->toUnicode("Измерения"));

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

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(0, 360);
    widget->setToolTip(QString("[%1, %2] deg").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("Sд"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.Sd = widget;

    top->setExpanded(false);
}


//
void TBspR400mHf::crtTreeState()
{
    QTreeWidgetItem *item;
    QTreeWidgetItem *def;
    QTreeWidgetItem *top = new QTreeWidgetItem();

    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("Текущее состояние"));

    errRegExp.setPattern("[0-9A-Fa-F]+");
    errValidator.setRegExp(errRegExp);

    Bsp::crtTreeState(top, "Общее", stateGlb);

    def  = Bsp::crtTreeState(top, "Защита", stateDef);
    item = new QTreeWidgetItem();
    item->setText(0, kCodec->toUnicode("Номер устройства ПВЗУ"));
    def->addChild(item);
    mTree->setItemWidget(item, 1, &mStateFaultDeviceNumber);
    mStateFaultDeviceNumber.setRange(0, 255);  // @fixme Изменить на реальный диапазон

    item = new QTreeWidgetItem();
    item->setText(0, kCodec->toUnicode("Режим АК"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, &mAc);

    item = new QTreeWidgetItem();
    item->setText(0, kCodec->toUnicode("Время до АК, с"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, &mAcTime);
    mAcTime.setRange(0, 60000);
    mAcTime.setToolTip(QString("%1 - %2").arg(mAcTime.minimum()).arg(mAcTime.maximum()));

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Управление"));
    top->insertChild(0, item);
    mTree->setItemWidget(item, 1, &mControl);

    connect(stateGlb.regime,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &TBspR400mHf::setRegime);

    connect(stateGlb.state,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &TBspR400mHf::setState);

    connect(stateGlb.dopByte,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &TBspR400mHf::setDopByte);

    top->setExpanded(true);
}


//
void TBspR400mHf::crtTest()
{
    QTreeWidgetItem *top = new QTreeWidgetItem(mTree);
    QTreeWidgetItem *item;
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("Тест"));

    item = new QTreeWidgetItem(top);
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("Сигналы КЧ"));
    mTree->setItemWidget(item, 1, &mTestCf1Signal);

    item = new QTreeWidgetItem(top);
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("Сигналы РЗ"));
    mTree->setItemWidget(item, 1, &mTestRz1Signal);
}


/**
 * *****************************************************************************
 *
 * @brief Создает ветку журналов.
 *
 * *****************************************************************************
 */
void TBspR400mHf::crtJrn()
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
void TBspR400mHf::crtJrnDef(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Количество записей"));
    mTree->setItemWidget(item, 1, &mJrnDefCounter);
    mJrnDefCounter.setRange(0, 1024);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Переполнение"));
    mTree->setItemWidget(item, 1, &mJrnDefOverflow);

    top->setExpanded(false);
}


/**
 * *****************************************************************************
 *
 * @brief Создает журнал событий.
 * @param[in] top Верхний уровень.
 *
 * *****************************************************************************
 */
void TBspR400mHf::crtJrnGlb(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Количество записей"));
    mTree->setItemWidget(item, 1, &mJrnGlbCounter);
    mJrnGlbCounter.setRange(0, 1024);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Переполнение"));
    mTree->setItemWidget(item, 1, &mJrnGlbOverflow);

    top->setExpanded(false);
}


void TBspR400mHf::FillComboboxListStateDef()
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
 * @brief Заполнение списка возможных режимов АК.
 *
 * *****************************************************************************
 */
void TBspR400mHf::FillComboboxListAc()
{
    // из fcAcType

    mAc.addItem(kCodec->toUnicode(fcAcType[0]), 0);

    QString value_1 = QString("%1 / %2")
                          .arg(kCodec->toUnicode(fcAcType[GB_TYPE_AC_AUTO_NORM]))
                          .arg(kCodec->toUnicode(fcAcType[GB_TYPE_AC_AUTO]));
    mAc.addItem(value_1, 1);

    mAc.addItem(value_1, GB_TYPE_AC_AUTO_REPEAT);
    mAc.addItem(kCodec->toUnicode(fcAcType[GB_TYPE_AC_ACCEL]), GB_TYPE_AC_ACCEL);
    mAc.addItem(kCodec->toUnicode(fcAcType[GB_TYPE_AC_OFF]), GB_TYPE_AC_OFF);

    QString value_5 = QString("%1 / %2")
                          .arg(kCodec->toUnicode(fcAcType[GB_TYPE_AC_CHECK]))
                          .arg(kCodec->toUnicode(fcAcType[GB_TYPE_AC_CHECK_1]));
    mAc.addItem(value_5, 1);

    mAc.addItem(kCodec->toUnicode(fcAcType[GB_TYPE_AC_PUSK_SELF]), GB_TYPE_AC_PUSK_SELF);
    mAc.addItem(kCodec->toUnicode(fcAcType[GB_TYPE_AC_PUSK]), GB_TYPE_AC_PUSK);
    mAc.addItem(kCodec->toUnicode(fcAcType[GB_TYPE_AC_ONE_SIDE]), GB_TYPE_AC_ONE_SIDE);
    mAc.addItem(kCodec->toUnicode(fcAcType[GB_TYPE_AC_MAX]), GB_TYPE_AC_MAX);
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
void TBspR400mHf::FillComboBoxListControl()
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
void TBspR400mHf::FillComboboxListTest()
{
    mTestCf1Signal.addItem(kCodec->toUnicode("Нет"), 0x00);
    mTestCf1Signal.addItem(kCodec->toUnicode("КЧ1"), 0x01);
    mTestCf1Signal.addItem(kCodec->toUnicode("КЧ2"), 0x02);
    mTestCf1Signal.addItem(kCodec->toUnicode("КЧ3"), 0x04);
    mTestCf1Signal.addItem(kCodec->toUnicode("КЧ4"), 0x08);

    mTestRz1Signal.addItem(kCodec->toUnicode("Нет"), 0x00);
    mTestRz1Signal.addItem(kCodec->toUnicode("РЗ"), 0x01);
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
void TBspR400mHf::HdlrComDefx00(eGB_COM com, pkg_t &data)
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
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_NUM_OF_DEVICES)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TIME_NO_MAN)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SHIFT_FRONT)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SHIFT_BACK)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SENS_DEC)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_AC_IN_DEC)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_FREQ_PRD)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_FREQ_PRM)));
        mPkgTx.append(getComboBoxValue(&mAc));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SHIFT_PRM)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SHIFT_PRD)));


        int len = 32 - mPkgTx.size();
        for (int i = 0; i < len; i++)
        {
            mPkgTx.append(0);
        }

        Q_ASSERT(mPkgTx.size() == 33);  // команда + 31 байт данных
    }

    if (com == 0x80)
    {
        // c ПК почему-то идет только 15 байт
        if (!CheckSize(com, data.size(), { 15, 31 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        int index = 0;
        SetParamValue(GB_PARAM_DEF_TYPE, data.at(index++));
        SetParamValue(GB_PARAM_NUM_OF_DEVICES, data.at(index++));
        SetParamValue(GB_PARAM_TIME_NO_MAN, data.at(index++));
        SetParamValue(GB_PARAM_SHIFT_FRONT, data.at(index++));
        SetParamValue(GB_PARAM_SHIFT_BACK, data.at(index++));
        SetParamValue(GB_PARAM_SENS_DEC, data.at(index++));
        SetParamValue(GB_PARAM_AC_IN_DEC, data.at(index++));
        SetParamValue(GB_PARAM_FREQ_PRD, data.at(index++));
        SetParamValue(GB_PARAM_FREQ_PRM, data.at(index++));
        index++;  // Режим АК не меняется
        SetParamValue(GB_PARAM_SHIFT_PRM, data.at(index++));
        SetParamValue(GB_PARAM_SHIFT_PRD, data.at(index++));
    }
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
void TBspR400mHf::HdlrComDefx01(eGB_COM com, pkg_t &data)
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
void TBspR400mHf::HdlrComDefx02(eGB_COM com, pkg_t &data)
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
void TBspR400mHf::HdlrComDefx03(eGB_COM com, pkg_t &data)
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
 * @brief Обрабатывает команду чтения/записи сдвигов ПРД и ПРМ
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf::HdlrComDefx05(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_OVERLAP || com == GB_COM_DEF_SET_OVERLAP);

    if (com == GB_COM_DEF_GET_OVERLAP)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SHIFT_FRONT)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SHIFT_BACK)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SHIFT_PRM)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SHIFT_PRD)));

        Q_ASSERT(mPkgTx.size() == 5);  // команда + 4 байт данных
    }

    if (com == GB_COM_DEF_SET_OVERLAP)
    {
        if (!CheckSize(com, data.size(), { 2 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        quint8 value = data.at(0);
        quint8 index = data.at(1);

        switch (index)
        {
        case 1: SetParamValue(GB_PARAM_SHIFT_FRONT, value); break;
        case 2: SetParamValue(GB_PARAM_SHIFT_BACK, value); break;
        case 3: SetParamValue(GB_PARAM_SHIFT_PRM, value); break;
        case 4: SetParamValue(GB_PARAM_SHIFT_PRD, value); break;
        default:
            qWarning() << QString("Wrong index (%1) in command %2")
                              .arg(index)
                              .arg(com, 2, 16, QLatin1Char('0'));
        }
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
void TBspR400mHf::HdlrComDefx06(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_RZ_DEC || com == GB_COM_DEF_SET_RZ_DEC);

    if (com == GB_COM_DEF_GET_RZ_DEC)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SENS_DEC)));

        Q_ASSERT(mPkgTx.size() == 2);  // команда + 1 байт данных
    }

    if (com == GB_COM_DEF_SET_RZ_DEC)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        SetParamValue(GB_PARAM_SENS_DEC, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи снижения уровня АК
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf::HdlrComDefx07(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_PRM_TYPE || com == GB_COM_DEF_SET_PRM_TYPE);

    if (com == GB_COM_DEF_GET_PRM_TYPE)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_AC_IN_DEC)));

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

        SetParamValue(GB_PARAM_AC_IN_DEC, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи смещения частоты ПРД
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf::HdlrComDefx08(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_FREQ_PRD || com == GB_COM_DEF_SET_FREQ_PRD);

    if (com == GB_COM_DEF_GET_FREQ_PRD)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_FREQ_PRD)));

        Q_ASSERT(mPkgTx.size() == 2);  // команда + 1 байт данных
    }

    if (com == GB_COM_DEF_SET_FREQ_PRD)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        SetParamValue(GB_PARAM_FREQ_PRD, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи смещения частоты ПРМ
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf::HdlrComDefx09(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_RZ_THRESH || com == GB_COM_DEF_SET_RZ_THRESH);

    if (com == GB_COM_DEF_GET_RZ_THRESH)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_FREQ_PRM)));

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

        SetParamValue(GB_PARAM_FREQ_PRM, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи режима АК и времени до АК
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf::HdlrComDefx0A(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_TYPE_AC || com == GB_COM_DEF_SET_TYPE_AC);

    if (com == GB_COM_DEF_GET_TYPE_AC)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(getComboBoxValue(&mAc));

        uint32_t ms = static_cast<uint32_t>(mAcTime.value()) * 1000;

        mPkgTx.append(static_cast<uint8_t>(ms));
        mPkgTx.append(static_cast<uint8_t>(ms >> 8));
        mPkgTx.append(static_cast<uint8_t>(ms >> 16));
        mPkgTx.append(static_cast<uint8_t>(ms >> 24));

        Q_ASSERT(mPkgTx.size() == 6);  // команда + 5 байт данных
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

        setComboBoxValue(&mAc, data.at(0));
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
void TBspR400mHf::HdlrComGlbx30(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_SOST || 0xB0);

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
        mPkgTx.append(0);
        mPkgTx.append(0);
        mPkgTx.append(0);
        mPkgTx.append(0);
        mPkgTx.append(0);
        mPkgTx.append(0);
        mPkgTx.append(0);
        mPkgTx.append(0);
        mPkgTx.append(0);

        Q_ASSERT(mPkgTx.size() == 13);  // команда + 12 байт данных
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
        SetParamValue(GB_PARAM_TIME_SYNCH_SRC, data.at(index++));
        SetParamValue(GB_PARAM_U_OUT_NOM, data.at(index++));
        SetParamValue(GB_PARAM_COMP_P400, data.at(index++));
        SetParamValue(GB_PARAM_NET_ADDRESS, data.at(index++));
        index++;  // время перезапуска ПРМ

        quint16 freq = data.at(index++);
        freq += static_cast<quint16>(data.at(index++)) << 8;
        SetParamValue(GB_PARAM_FREQ, freq);

        SetParamValue(GB_PARAM_NUM_OF_DEVICE, data.at(index++));
        SetParamValue(GB_PARAM_OUT_CHECK, data.at(index++));
        SetParamValue(GB_PARAM_WARN_THD, data.at(index++));

        // @fixme Разобраться что тут должно быть
        index++;  // загрубление чувствительности по КЧ ПРМ_1
        index++;  // загрубление чувствительности по КЧ ПРМ_2

        // коррекции с ПК не устанавливаются
        index++;  // Коррекция значения выходного напряжения (целая часть)
        index++;  // Коррекция значения выходного напряжения (дробная цасть)
        index++;  // Коррекция значения тока 1 (младший байт)
        index++;  // Коррекция значения тока 1 (старший байт)
        index++;  // Коррекция значения тока 2 (младший байт)
        index++;  // Коррекция значения тока 2 (старший байт)

        SetParamValue(GB_PARAM_PVZUE_PROTOCOL, data.at(index++));
        SetParamValue(GB_PARAM_PVZUE_PARITY, data.at(index++));
        SetParamValue(GB_PARAM_PVZUE_FAIL, data.at(index++));
        SetParamValue(GB_PARAM_PVZUE_NOISE_THD, data.at(index++));
        SetParamValue(GB_PARAM_PVZUE_NOISE_LVL, data.at(index++));
        SetParamValue(GB_PARAM_PVZUE_AC_TYPE, data.at(index++));
        SetParamValue(GB_PARAM_PVZUE_AC_PERIOD, data.at(index++));
        SetParamValue(GB_PARAM_PVZUE_AC_PER_RE, data.at(index++));
        SetParamValue(GB_PARAM_PVZU_AC_CORRECT, data.at(index++));
        SetParamValue(GB_PARAM_ALARM_CF, data.at(index++));
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
void TBspR400mHf::HdlrComGlbx31(eGB_COM com, pkg_t &data)
{
    bool    ok;
    quint16 def_alarm = getLineEditValue(stateDef.fault).toUInt(&ok, 16);
    quint16 def_warn  = getLineEditValue(stateDef.warning).toUInt(&ok, 16);
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

    mPkgTx.append(getSpinBoxValue(&mStateFaultDeviceNumber));
    mPkgTx.append(0);
    mPkgTx.append(0);
    mPkgTx.append(0);

    mPkgTx.append(0);
    mPkgTx.append(0);
    mPkgTx.append(0);
    mPkgTx.append(0);

    mPkgTx.append(static_cast<uint8_t>(glb_alarm >> 8));
    mPkgTx.append(static_cast<uint8_t>(glb_alarm));
    mPkgTx.append(static_cast<uint8_t>(glb_warn >> 8));
    mPkgTx.append(static_cast<uint8_t>(glb_warn));

    mPkgTx.append(0);
    mPkgTx.append(0);
    mPkgTx.append(0);
    mPkgTx.append(0);


    Q_ASSERT(mPkgTx.size() == 21);  // команда + 20 байт данных
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
void TBspR400mHf::HdlrComGlbx32(eGB_COM com, pkg_t &data)
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
 * @brief Обрабатывает команду чтения коррекция тока и напряжения.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf::HdlrComGlbx33(eGB_COM com, pkg_t &data)
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
 * @brief Обрабатывает команду чтения измерений.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf::HdlrComGlbx34(eGB_COM com, pkg_t &data)
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

    mPkgTx.append(0);
    mPkgTx.append(0);

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
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Un1)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Un2)));

    value = getSpinBoxValue(m_measure.Sd);
    mPkgTx.append(static_cast<quint8>(value >> 8));
    mPkgTx.append(static_cast<quint8>(value));

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
void TBspR400mHf::HdlrComGlbx35(eGB_COM com, pkg_t &data)
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
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TIME_SYNCH_SRC)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_U_OUT_NOM)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_COMP_P400)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_NET_ADDRESS)));
            mPkgTx.append(0);  // время перезапуска ПРМ, сек

            quint16 freq = GetParamValue(GB_PARAM_FREQ);
            mPkgTx.append(static_cast<uint8_t>(freq));
            mPkgTx.append(static_cast<uint8_t>(freq >> 8));

            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_NUM_OF_DEVICE)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_OUT_CHECK)));  // 0 - выкл.
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_WARN_THD)));

            mPkgTx.append(0);  // загрубление чувствительности по КЧ ПРМ_1
            mPkgTx.append(0);  // загрубление чувствительности по КЧ ПРМ_2

            qint16 corr_u = GetParamValue(GB_PARAM_COR_U);
            mPkgTx.append(corr_u / 10);
            mPkgTx.append((corr_u % 10) * 10);

            qint16 corr_i = GetParamValue(GB_PARAM_COR_I);
            mPkgTx.append(static_cast<uint8_t>(corr_i));
            mPkgTx.append(static_cast<uint8_t>(corr_i >> 8));
            mPkgTx.append(0);  // Коррекция значения тока 2, младший байт
            mPkgTx.append(0);  // Коррекция значения тока 2, старший байт

            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_PROTOCOL)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_PARITY)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_FAIL)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_NOISE_THD)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_NOISE_LVL)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_AC_TYPE)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_AC_PERIOD)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_AC_PER_RE)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZU_AC_CORRECT)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_ALARM_CF)));

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
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TIME_SYNCH_SRC)));
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

        SetParamValue(GB_PARAM_TIME_SYNCH_SRC, data.at(0));
    }
}

/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи уровня выходного напряжения.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf::HdlrComGlbx36(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_COM_PRM_KEEP || com == GB_COM_SET_COM_PRM_KEEP);

    if (com == GB_COM_GET_COM_PRM_KEEP)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_U_OUT_NOM)));

        Q_ASSERT(mPkgTx.size() == 2);  // команда + байт данных
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

        SetParamValue(GB_PARAM_U_OUT_NOM, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи совместимости.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf::HdlrComGlbx37(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_COM_PRD_KEEP || com == GB_COM_SET_COM_PRD_KEEP);

    if (com == GB_COM_GET_COM_PRD_KEEP)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_COMP_P400)));

        Q_ASSERT(mPkgTx.size() == 2);  // команда + байт данных
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

        SetParamValue(GB_PARAM_COMP_P400, data.at(0));
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
void TBspR400mHf::HdlrComGlbx38(eGB_COM com, pkg_t &data)
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
 * @brief Обрабатывает команду чтения/записи параметров ПВЗУ-Е
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf::HdlrComGlbx39(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_TIME_RERUN || com == GB_COM_SET_TIME_RERUN);

    if (com == GB_COM_GET_TIME_RERUN)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_PROTOCOL)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_PARITY)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_FAIL)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_NOISE_THD)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_NOISE_LVL)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_AC_TYPE)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_AC_PERIOD)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_AC_PER_RE)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZU_AC_CORRECT)));

        Q_ASSERT(mPkgTx.size() == 10);  // команда + 9 байт данных
    }

    if (com == GB_COM_SET_TIME_RERUN)
    {
        if (!CheckSize(com, data.size(), { 2 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        switch (data.at(0))
        {
        case 1: SetParamValue(GB_PARAM_PVZUE_PROTOCOL, data.at(1)); break;
        case 2: SetParamValue(GB_PARAM_PVZUE_PARITY, data.at(1)); break;
        case 3: SetParamValue(GB_PARAM_PVZUE_FAIL, data.at(1)); break;
        case 4: SetParamValue(GB_PARAM_PVZUE_NOISE_THD, data.at(1)); break;
        case 5: SetParamValue(GB_PARAM_PVZUE_NOISE_LVL, data.at(1)); break;
        case 6: SetParamValue(GB_PARAM_PVZUE_AC_TYPE, data.at(1)); break;
        case 7: SetParamValue(GB_PARAM_PVZUE_AC_PERIOD, data.at(1)); break;
        case 8: SetParamValue(GB_PARAM_PVZUE_AC_PER_RE, data.at(1)); break;
        case 9: SetParamValue(GB_PARAM_PVZU_AC_CORRECT, data.at(1)); break;
        default:
            {
                QString message("Wrong value in command %1: %2");
                qWarning() << message.arg(com, 2, 16, QLatin1Char('0')).arg(data.at(0));
            }
        }
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
void TBspR400mHf::HdlrComGlbx3A(eGB_COM com, pkg_t &data)
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
void TBspR400mHf::HdlrComGlbx3B(eGB_COM com, pkg_t &data)
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
 * @brief Обрабатывает команду чтения порога КЧ.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf::HdlrComGlbx3C(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_CF_THRESHOLD || com == GB_COM_SET_CF_THRESHOLD);

    if (com == GB_COM_GET_CF_THRESHOLD)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_WARN_THD)));
        mPkgTx.append(0);  // нет, загрубление чувствительности по КЧ ПРМ_1
        mPkgTx.append(0);  // нет, загрубление чувствительности по КЧ ПРМ_2
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_ALARM_CF)));

        Q_ASSERT(mPkgTx.size() == 5);  // команда + 4 байта данных
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

        switch (data.at(0))
        {
        case 1: SetParamValue(GB_PARAM_WARN_THD, data.at(1)); break;
        case 4: SetParamValue(GB_PARAM_ALARM_CF, data.at(1)); break;
        default:
            {
                QString message("Wrong value in command %1: %2");
                qWarning() << message.arg(com, 2, 16, QLatin1Char('0')).arg(data.at(0));
            }
        }
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
void TBspR400mHf::HdlrComGlbx3D(eGB_COM com, pkg_t &data)
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
 * @brief Обрабатывает команду чтения сигналов в тестах.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf::HdlrComGlbx3E(eGB_COM com, pkg_t &data)
{
    quint8 cf = mTestCf1Signal.currentData().toUInt();
    quint8 rz = mTestRz1Signal.currentData().toUInt();

    Q_ASSERT(com == GB_COM_GET_TEST);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    if (GetParamValue(GB_PARAM_NUM_OF_DEVICES) == GB_NUM_DEVICES_3)
    {
        mPkgTx.append(com);
        mPkgTx.append((cf & 0x0F) | ((rz & 0x0F) << 4));
        mPkgTx.append(0);
        mPkgTx.append(0);
        mPkgTx.append(0);
        mPkgTx.append(0);
        mPkgTx.append(0);

        Q_ASSERT(mPkgTx.size() == 7);  // команда + 6 байт данных
    }
    else
    {
        mPkgTx.append(com);
        mPkgTx.append((cf & 0x0F) | ((rz & 0x0F) << 4));
        mPkgTx.append(0);
        mPkgTx.append(0);

        Q_ASSERT(mPkgTx.size() == 4);  // команда + 3 байт данных
    }
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
void TBspR400mHf::HdlrComGlbx3F(eGB_COM com, pkg_t &data)
{
    quint16 vers_bsp_mcu = getSpinBoxValue(mDevice.versionBspMcu);
    quint16 vers_bsp_dsp = getSpinBoxValue(mDevice.versionBspDsp);

    Q_ASSERT(com == GB_COM_GET_VERS);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    mPkgTx.append(com);
    mPkgTx.append(1);  // защита
    mPkgTx.append(0);  // прм1
    mPkgTx.append(0);  // прм2
    mPkgTx.append(0);  // прд
    mPkgTx.append(getComboBoxValue(GB_PARAM_NUM_OF_DEVICES) + 1);
    mPkgTx.append(GB_TYPE_LINE_UM);
    mPkgTx.append(static_cast<quint8>(vers_bsp_mcu >> 8));
    mPkgTx.append(static_cast<quint8>(vers_bsp_mcu));
    mPkgTx.append(static_cast<quint8>(vers_bsp_dsp >> 8));
    mPkgTx.append(static_cast<quint8>(vers_bsp_dsp));
    mPkgTx.append(getComboBoxValue(GB_PARAM_COMP_P400));
    mPkgTx.append(0);
    mPkgTx.append(0);
    mPkgTx.append(0);
    mPkgTx.append(0);
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(mDevice.versionBszPlis)));
    mPkgTx.append(AVANT_R400M);
    mPkgTx.append(0);  // версия БСП-ПИ, старший байт
    mPkgTx.append(0);  // версия БСП-ПИ, младший байт

    Q_ASSERT(mPkgTx.size() == 20);  // команда + 19 байт данных
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
void TBspR400mHf::HdlrComRegx70(eGB_COM com, pkg_t &data)
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
void TBspR400mHf::HdlrComRegx71(eGB_COM com, pkg_t &data)
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
 * @brief Обрабатывает команду управления.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf::HdlrComRegx72(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_SET_CONTROL);

    if (!CheckSize(com, data.size(), { 1 }))
    {
        return;
    }

    // ответ на команду записи совпадает с запросом
    mPkgTx.append(com);
    mPkgTx.append(data);

    uint8_t control = data.takeFirst();
    if (control >= mControl.count())
    {
        QString message("Wrong value in command %1: %2");
        qWarning() << message.arg(com, 2, 16, QLatin1Char('0')).arg(control);
    }

    if (control == 8)
    {
        setComboBoxValue(stateDef.state, 7);  // Пуск налад. вкл
    }
    else if (control == 9)
    {
        setComboBoxValue(stateDef.state, 1);  // Пуск налад. выкл
    }

    mControl.setCurrentIndex(mControl.findData(control));
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
void TBspR400mHf::HdlrComRegx7D(eGB_COM com, pkg_t &data)
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
 * @brief Обрабатывает команду изменения режима на Тест 1 (передатчик)
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf::HdlrComRegx7E(eGB_COM com, pkg_t &data)
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
    }
    else
    {
        switch (data.at(0))
        {
        case 1:
            {
                quint8 cf    = data.at(1);
                int    index = -1;

                switch (cf)
                {
                case 0: index = mTestCf1Signal.findData(0x00); break;
                case 1: index = mTestCf1Signal.findData(0x01); break;
                case 2: index = mTestCf1Signal.findData(0x02); break;
                case 3: index = mTestCf1Signal.findData(0x04); break;
                case 4: index = mTestCf1Signal.findData(0x08); break;
                }

                if (index < 0)
                {
                    QString message = "Wrong test CF value: %1";
                    qWarning() << message.arg(cf);
                }

                mTestCf1Signal.setCurrentIndex(index);

                // ответ
                mPkgTx.append(com);
                mPkgTx.append(mTestCf1Signal.currentData().toUInt());
                mPkgTx.append(0);
                mPkgTx.append(0);
                break;
            }
        case 2:
            {
                quint8 rz    = data.at(1);
                int    index = -1;

                switch (rz)
                {
                case 0: index = mTestRz1Signal.findData(0x00); break;
                case 1: index = mTestRz1Signal.findData(0x01); break;
                }

                if (index < 0)
                {
                    QString message = "Wrong test RZ value: %1";
                    qWarning() << message.arg(rz);
                }

                mTestRz1Signal.setCurrentIndex(index);


                // ответ
                mPkgTx.append(com);
                mPkgTx.append(mTestRz1Signal.currentData().toUInt());
                mPkgTx.append(0);
                mPkgTx.append(0);
                break;
            }
        default:
            QString message = "Wrong test group value: %1";
            qWarning() << message.arg(data.at(0));
        }
    }
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
void TBspR400mHf::HdlrComJrnxC1(eGB_COM com, pkg_t &data)
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
void TBspR400mHf::HdlrComJrnxC2(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_JRN_ENTRY);

    if (!CheckSize(com, data.size(), { 2 }))
    {
        return;
    }

    int event_number = data.takeFirst();
    event_number += static_cast<int>(data.takeFirst());

    mPkgTx.append(com);
    mPkgTx.append(GB_DEVICE_K400_DEF);          // устройство
    mPkgTx.append(event_number);                // zzzz z[ман][пуск][стоп]
    mPkgTx.append(event_number % 17);           // состояние защиты
    mPkgTx.append(event_number);                // zzzz z[вых][прд][прм]
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
void TBspR400mHf::HdlrComJrnxF1(eGB_COM com, pkg_t &data)
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
void TBspR400mHf::HdlrComJrnxF2(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_JRN_ENTRY);

    if (!CheckSize(com, data.size(), { 2 }))
    {
        return;
    }

    int event_number = data.takeFirst();
    event_number += static_cast<int>(data.takeFirst());

    mPkgTx.append(com);
    mPkgTx.append(event_number % 10);  // удаленный аппарат для ПВЗУ, ПВЗУЕ
    mPkgTx.append(event_number % (JRN_EVENT_R400M_SIZE + 1));  // событие
    mPkgTx.append(event_number % 8);                           // режим
    mPkgTx.append(0);                                          // b4
    mPkgTx.append(0);                                          // b5
    mPkgTx.append(0);                                          // b6
    mPkgTx.append(0);                                          // b7
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
