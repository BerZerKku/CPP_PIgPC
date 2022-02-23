#include <QDebug>
#include <QHeaderView>

#include "bspK400_hf.hpp"

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
TBspK400Hf::TBspK400Hf(QTreeWidget *tree, QWidget *parent) : Bsp(tree, parent)
{
}


/**
 * *****************************************************************************
 *
 * @brief Настраивает карту команд.
 *
 * *****************************************************************************
 */
void TBspK400Hf::InitComMap()
{
    mComMap.insert(0x02, &Bsp::HdlrComDefx02);  // чтение количества аппаратов в линии

    mComMap.insert(0x10, &Bsp::HdlrComPrmx10);  // чтение параметров прм с конфигуратора
    mComMap.insert(0x11, &Bsp::HdlrComPrmx11);  // чтение задержки на включение
    mComMap.insert(0x12, &Bsp::HdlrComPrmx12);  // чтение тестовой команды
    mComMap.insert(0x13, &Bsp::HdlrComPrmx13);  // чтение задержки на выключение
    mComMap.insert(0x14, &Bsp::HdlrComPrmx14);  // чтение блокированных команд
    mComMap.insert(0x15, &Bsp::HdlrComPrmx15);  // чтение коррекция частоты ПРМ
    mComMap.insert(0x17, &Bsp::HdlrComPrmx17);  // чтение наличия ЦПП
    mComMap.insert(0x18, &Bsp::HdlrComPrmx18);  // чтение блокированных команд ЦПП
    mComMap.insert(0x19, &Bsp::HdlrComPrmx19);  // чтение переназначений команд ЦПП
    mComMap.insert(0x1C, &Bsp::HdlrComPrmx1C);  // чтение количества команд на приеме
    mComMap.insert(0x1D, &Bsp::HdlrComPrmx1D);  // чтение разрешений сигнализаций команд

    mComMap.insert(0x20, &Bsp::HdlrComPrdx20);  // чтение параметров прд с конфигуратора
    mComMap.insert(0x21, &Bsp::HdlrComPrdx21);  // чтение задержки на включение
    mComMap.insert(0x22, &Bsp::HdlrComPrdx22);  // чтение длительности команды
    mComMap.insert(0x23, &Bsp::HdlrComPrdx23);  // чтение коррекции частоты
    mComMap.insert(0x24, &Bsp::HdlrComPrdx24);  // чтение блокированных команд
    mComMap.insert(0x25, &Bsp::HdlrComPrdx25);  // чтение длительных команд
    mComMap.insert(0x26, &Bsp::HdlrComPrdx26);  // чтение наличия тестовой команды
    mComMap.insert(0x27, &Bsp::HdlrComPrdx27);  // чтение наличия ЦПП
    mComMap.insert(0x28, &Bsp::HdlrComPrdx28);  // чтение блокированных команд ЦПП
    mComMap.insert(0x29, &Bsp::HdlrComPrdx29);  // чтение количество команд А
    mComMap.insert(0x2C, &Bsp::HdlrComPrdx2C);  // чтение количества команд на передаче
    mComMap.insert(0x2D, &Bsp::HdlrComPrdx2D);  // чтение снижение уровней КС и ТМ
    mComMap.insert(0x2E, &Bsp::HdlrComPrdx2E);  // чтение разрешений сигнализаций команд

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
    mComMap.insert(0x3F, &Bsp::HdlrComGlbx3F);  // чтение информации об устройстве

    mComMap.insert(0x51, &Bsp::HdlrComRegx51);  // пуск ПРМ

    mComMap.insert(0x70, &Bsp::HdlrComRegx70);  // смена режима на "Выведен"
    mComMap.insert(0x71, &Bsp::HdlrComRegx71);  // смена режима на "Введен"
    mComMap.insert(0x72, &Bsp::HdlrComRegx72);  // команда управления
    mComMap.insert(0x7D, &Bsp::HdlrComRegx7D);  // смена режима на "Тест 2 (ПРМ)"
    mComMap.insert(0x7E, &Bsp::HdlrComRegx7E);  // смена режима на "Тест 1 (ПРД)"

    mComMap.insert(0x82, &Bsp::HdlrComDefx02);  // запись количества аппаратов в линии

    mComMap.insert(0x90, &Bsp::HdlrComPrmx10);  // запись параметров прм с конфигуратора
    mComMap.insert(0x91, &Bsp::HdlrComPrmx11);  // запись задержки на включение
    mComMap.insert(0x92, &Bsp::HdlrComPrmx12);  // запись тестовой команды
    mComMap.insert(0x93, &Bsp::HdlrComPrmx13);  // запись задержки на выключение
    mComMap.insert(0x94, &Bsp::HdlrComPrmx14);  // запись блокированных команд
    mComMap.insert(0x95, &Bsp::HdlrComPrmx15);  // запись коррекция частоты ПРМ
    mComMap.insert(0x97, &Bsp::HdlrComPrmx17);  // запись наличия ЦПП
    mComMap.insert(0x98, &Bsp::HdlrComPrmx18);  // запись блокированных команд ЦПП
    mComMap.insert(0x99, &Bsp::HdlrComPrmx19);  // запись переназначений команд ЦПП
    mComMap.insert(0x9A, &Bsp::HdlrComRegx9A);  // команда сброса индикации
    mComMap.insert(0x9B, &Bsp::HdlrComPrmx1B);  // запись всех переназначенных команд ЦПП
    mComMap.insert(0x9C, &Bsp::HdlrComPrmx1C);  // запись количества команд на приеме
    mComMap.insert(0x9D, &Bsp::HdlrComPrmx1D);  // запись разрешений сигнализаций команд

    mComMap.insert(0xA0, &Bsp::HdlrComPrdx20);  // запись параметров прд с конфигуратора
    mComMap.insert(0xA1, &Bsp::HdlrComPrdx21);  // запись задержки на включение
    mComMap.insert(0xA2, &Bsp::HdlrComPrdx22);  // запись длительности команды
    mComMap.insert(0xA3, &Bsp::HdlrComPrdx23);  // запись коррекции частоты
    mComMap.insert(0xA4, &Bsp::HdlrComPrdx24);  // запись блокированных команд
    mComMap.insert(0xA5, &Bsp::HdlrComPrdx25);  // запись длительных команд
    mComMap.insert(0xA6, &Bsp::HdlrComPrdx26);  // запись наличия тестовой команды
    mComMap.insert(0xA7, &Bsp::HdlrComPrdx27);  // запись наличия ЦПП
    mComMap.insert(0xA8, &Bsp::HdlrComPrdx28);  // запись блокированных команд ЦПП
    mComMap.insert(0xA9, &Bsp::HdlrComPrdx29);  // запись количество команд А
    mComMap.insert(0xAC, &Bsp::HdlrComPrdx2C);  // запись количества команд на передаче
    mComMap.insert(0xAD, &Bsp::HdlrComPrdx2D);  // запись снижение уровней КС и ТМ
    mComMap.insert(0xAE, &Bsp::HdlrComPrdx2E);  // запись разрешений сигнализаций команд

    mComMap.insert(0xB0, &Bsp::HdlrComGlbx30);  // запись общих параметров с конфигуратора
    mComMap.insert(0xB2, &Bsp::HdlrComGlbx32);  // запись времени
    mComMap.insert(0xB3, &Bsp::HdlrComGlbx33);  // запись коррекций тока и напряжения
    mComMap.insert(0xB5, &Bsp::HdlrComGlbx35);  // запись синхронизации времени / всех
    mComMap.insert(0xB6, &Bsp::HdlrComGlbx36);  // запись удержания реле прм
    mComMap.insert(0xB7, &Bsp::HdlrComGlbx37);  // запись удерж. реле прд и совмест.
    mComMap.insert(0xB8, &Bsp::HdlrComGlbx38);  // запись адреса в локальной сети
    mComMap.insert(0xB9, &Bsp::HdlrComGlbx39);  // запись времени перезапуска
    mComMap.insert(0xBA, &Bsp::HdlrComGlbx3A);  // запись частоты
    mComMap.insert(0xBB, &Bsp::HdlrComGlbx3B);  // запись номера аппарата
    mComMap.insert(0xBC, &Bsp::HdlrComGlbx3C);  // запись порога предупр. КЧ и загр. чувств.
    mComMap.insert(0xBD, &Bsp::HdlrComGlbx3D);  // запись контроля выхода

    mComMap.insert(0xD1, &Bsp::HdlrComJrnxD1);  // чтение количества записей в журнале прм
    mComMap.insert(0xD2, &Bsp::HdlrComJrnxD2);  // чтение записей журнала прм
    mComMap.insert(0xE1, &Bsp::HdlrComJrnxE1);  // чтение количества записей в журнале прд
    mComMap.insert(0xE2, &Bsp::HdlrComJrnxE2);  // чтение записей журнала прд
    mComMap.insert(0xF1, &Bsp::HdlrComJrnxF1);  // чтение количества записей в журнале событий
    mComMap.insert(0xF2, &Bsp::HdlrComJrnxF2);  // чтение записей журнала событий
}


//
void TBspK400Hf::InitParam()
{
    setSpinBoxValue(mDevice.versionBspMcu, 0x0148);
    setSpinBoxValue(mDevice.versionBspDsp, 0x1D33);
    setSpinBoxValue(mDevice.versionBsk1PrdPlis, 0x25);
    setSpinBoxValue(mDevice.versionBsk2PrdPlis, 0x25);
    setSpinBoxValue(mDevice.versionBsk1PrmPlis, 0x23);
    setSpinBoxValue(mDevice.versionBsk2PrmPlis, 0x23);

    setComboBoxValue(stateGlb.regime, eGB_REGIME::GB_REGIME_ENABLED);
    setComboBoxValue(stateGlb.state, 1);  // 1 - контроль
    setSpinBoxValue(stateGlb.dopByte, 1);

    // Параметры интерфейса
    setSpinBoxValue(GB_PARAM_NET_ADDRESS, 17);

    // Параметры общие
    SetParamValue(GB_PARAM_COMP_K400, GB_COMP_K400_AVANT);
    SetParamValue(GB_PARAM_NUM_OF_DEVICES, GB_NUM_DEVICES_2);
    SetParamValue(GB_PARAM_NUM_OF_DEVICE, 1);
    SetParamValue(GB_PARAM_TIME_SYNCH, 1);  // 1 - вкл.
    SetParamValue(GB_PARAM_OUT_CHECK, 0);   // 0 - выкл.
    SetParamValue(GB_PARAM_WARN_THD, 11);
    SetParamValue(GB_PARAM_TIME_RERUN, 3);
    SetParamValue(GB_PARAM_COM_PRD_KEEP, 0);  // 0 - выкл.
    SetParamValue(GB_PARAM_COM_PRM_KEEP, 1);  // 1 - вкл.
    SetParamValue(GB_PARAM_IN_DEC, 12, 1);
    SetParamValue(GB_PARAM_IN_DEC, 13, 2);
    SetParamValue(GB_PARAM_FREQ, 123);
    SetParamValue(GB_PARAM_COR_U, -105);  // -10.5 В
    SetParamValue(GB_PARAM_COR_I, 20);    // 20 мА
    SetParamValue(GB_PARAM_TM_K400, 1);   // 1 - вкл.
    SetParamValue(GB_PARAM_WARN_D, -16);
    SetParamValue(GB_PARAM_ALARM_D, 24);
    SetParamValue(GB_PARAM_TEMP_MONITOR, 0);  // 0 -выкл.
    SetParamValue(GB_PARAM_TEMP_THR_HI, 54);
    SetParamValue(GB_PARAM_TEMP_THR_LOW, 17);
    SetParamValue(GB_PARAM_TM_SPEED, 1);  // 400 бод

    // Параметры передатчика
    SetParamValue(GB_PARAM_PRD_COM_NUMS, 8);
    SetParamValue(GB_PARAM_PRD_IN_DELAY, 7);
    SetParamValue(GB_PARAM_PRD_DURATION_L, 50);
    SetParamValue(GB_PARAM_PRD_TEST_COM, 1);         // 1 - вкл.
    SetParamValue(GB_PARAM_PRD_COM_LONG, 0x0F, 1);   // биты, 1 - вкл.
    SetParamValue(GB_PARAM_PRD_COM_BLOCK, 0xF0, 2);  // биты, 1 - вкл.
    SetParamValue(GB_PARAM_PRD_COM_NUMS_A, 5);
    SetParamValue(GB_PARAM_PRD_DR_ENABLE, 0);           // 0 - выкл
    SetParamValue(GB_PARAM_PRD_DR_COM_BLOCK, 0x07, 3);  // биты, 1 - вкл.
    SetParamValue(GB_PARAM_PRD_FREQ_CORR, -37);         //
    SetParamValue(GB_PARAM_PRD_DEC_CF, 10);
    SetParamValue(GB_PARAM_PRD_DEC_TM, 12);
    SetParamValue(GB_PARAM_PRD_COM_SIGNAL, 1);  // 1 - вкл.

    // Параметры приемника
    SetParamValue(GB_PARAM_PRM_COM_NUMS, 8);
    SetParamValue(GB_PARAM_PRM_TIME_ON, 6);
    SetParamValue(GB_PARAM_PRM_COM_BLOCK, 0x05, 1);  // биты, 1 - вкл.
    SetParamValue(GB_PARAM_PRM_TIME_OFF, 250 / 10, 1);
    SetParamValue(GB_PARAM_PRM_TIME_OFF, 500 / 10, 4);
    SetParamValue(GB_PARAM_PRM_DR_ENABLE, 1);           // 1 - вкл
    SetParamValue(GB_PARAM_PRM_DR_COM_BLOCK, 0xC1, 3);  // биты, 1 - вкл.
    SetParamValue(GB_PARAM_PRM_DR_COM_TO_HF, 32, 1);
    SetParamValue(GB_PARAM_PRM_DR_COM_TO_HF, 27, 32);
    SetParamValue(GB_PARAM_PRM_TEST_COM, 0);  // 0 - выкл
    SetParamValue(GB_PARAM_PRM_FREQ_CORR, 35);
    SetParamValue(GB_PARAM_PRM_COM_SIGNAL, 0xEE, 2);  // биты, 1 - вкл.

    setSpinBoxValue(m_measure.R, 75);
    setSpinBoxValue(m_measure.I, 101);
    setSpinBoxValue(m_measure.U, 251);
    setSpinBoxValue(m_measure.D, 13);
    setSpinBoxValue(m_measure.Ucf1, -16);
    setSpinBoxValue(m_measure.Ucf2, 16);
    setSpinBoxValue(m_measure.Un1, -7);
    setSpinBoxValue(m_measure.Un2, 7);
    setSpinBoxValue(m_measure.T, 25);
    setSpinBoxValue(m_measure.dF, 13);

    mJrnGlbCounter.setValue(15);
    mJrnPrmCounter.setValue(17);
    mJrnPrdCounter.setValue(18);
}


//
void TBspK400Hf::crtTreePrd(QTreeWidgetItem *top)
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Передатчик"));
    top->addChild(ltop);

    CrtParamWidget(ltop, GB_PARAM_PRD_COM_NUMS);
    CrtParamWidget(ltop, GB_PARAM_PRD_IN_DELAY);
    CrtParamWidget(ltop, GB_PARAM_PRD_DURATION_L);
    CrtParamWidget(ltop, GB_PARAM_PRD_TEST_COM);
    CrtParamWidget(ltop, GB_PARAM_PRD_COM_LONG);
    CrtParamWidget(ltop, GB_PARAM_PRD_COM_BLOCK);
    CrtParamWidget(ltop, GB_PARAM_PRD_COM_NUMS_A);
    CrtParamWidget(ltop, GB_PARAM_PRD_DR_ENABLE);
    CrtParamWidget(ltop, GB_PARAM_PRD_DR_COM_BLOCK);
    CrtParamWidget(ltop, GB_PARAM_PRD_FREQ_CORR);
    CrtParamWidget(ltop, GB_PARAM_PRD_DEC_CF);
    CrtParamWidget(ltop, GB_PARAM_PRD_DEC_TM);
    CrtParamWidget(ltop, GB_PARAM_PRD_COM_SIGNAL);

    mapSpinBox.value(GB_PARAM_PRD_COM_NUMS).at(0)->setMinimum(0);
}


//
void TBspK400Hf::crtTreePrm(QTreeWidgetItem *top)
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Приемник"));
    top->addChild(ltop);

    CrtParamWidget(ltop, GB_PARAM_PRM_COM_NUMS);
    CrtParamWidget(ltop, GB_PARAM_PRM_TIME_ON);
    CrtParamWidget(ltop, GB_PARAM_PRM_COM_BLOCK);
    CrtParamWidget(ltop, GB_PARAM_PRM_TIME_OFF);
    CrtParamWidget(ltop, GB_PARAM_PRM_DR_ENABLE);
    CrtParamWidget(ltop, GB_PARAM_PRM_DR_COM_BLOCK);
    CrtParamWidget(ltop, GB_PARAM_PRM_DR_COM_TO_HF);
    CrtParamWidget(ltop, GB_PARAM_PRM_TEST_COM);
    CrtParamWidget(ltop, GB_PARAM_PRM_FREQ_CORR);
    CrtParamWidget(ltop, GB_PARAM_PRM_COM_SIGNAL);

    mapSpinBox.value(GB_PARAM_PRM_COM_NUMS).at(0)->setMinimum(0);
}


void TBspK400Hf::crtTreeGlb(QTreeWidgetItem(*top))
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Общие"));
    top->addChild(ltop);

    CrtParamWidget(ltop, GB_PARAM_COMP_K400);
    CrtParamWidget(ltop, GB_PARAM_NUM_OF_DEVICES);
    CrtParamWidget(ltop, GB_PARAM_NUM_OF_DEVICE);
    CrtParamWidget(ltop, GB_PARAM_TIME_SYNCH);
    CrtParamWidget(ltop, GB_PARAM_OUT_CHECK);
    CrtParamWidget(ltop, GB_PARAM_WARN_THD);
    CrtParamWidget(ltop, GB_PARAM_TIME_RERUN);
    CrtParamWidget(ltop, GB_PARAM_COM_PRD_KEEP);
    CrtParamWidget(ltop, GB_PARAM_COM_PRM_KEEP);
    CrtParamWidget(ltop, GB_PARAM_IN_DEC);
    CrtParamWidget(ltop, GB_PARAM_FREQ);
    CrtParamWidget(ltop, GB_PARAM_COR_U);
    CrtParamWidget(ltop, GB_PARAM_COR_I);
    CrtParamWidget(ltop, GB_PARAM_TM_K400);
    CrtParamWidget(ltop, GB_PARAM_WARN_D);
    CrtParamWidget(ltop, GB_PARAM_ALARM_D);
    CrtParamWidget(ltop, GB_PARAM_TEMP_MONITOR);
    CrtParamWidget(ltop, GB_PARAM_TEMP_THR_HI);
    CrtParamWidget(ltop, GB_PARAM_TEMP_THR_LOW);
    CrtParamWidget(ltop, GB_PARAM_TM_SPEED);

    connect(mapCombobox.value(GB_PARAM_COMP_K400).at(0),
            &QComboBox::currentTextChanged,
            this,
            &TBspK400Hf::SlotChangeCompatibility);
    SlotChangeCompatibility();
}


void TBspK400Hf::crtTreeInterface(QTreeWidgetItem *top)
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("Интерфейс"));
    top->addChild(ltop);

    CrtParamWidget(ltop, GB_PARAM_NET_ADDRESS);
}


void TBspK400Hf::crtTreeDevice()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();

    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("Устройство"));

    crtTreeDevieVersions(top);
}


void TBspK400Hf::crtTreeDevieVersions(QTreeWidgetItem *top)
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

    item                  = new QTreeWidgetItem();
    mDevice.versionBspDsp = new QHexSpinBox(false);
    item->setText(0, kCodec->toUnicode("БСП ЦСП"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBspDsp);

    item                   = new QTreeWidgetItem();
    mDevice.versionDspPlis = new QHexSpinBox(false);
    item->setText(0, kCodec->toUnicode("БСП ПЛИС ЦСП"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionDspPlis);


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


void TBspK400Hf::crtTreeMeasure()
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
    widget->setRange(-64, 64);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("D"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.D = widget;

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
    widget->setRange(-100, 125);
    widget->setToolTip(QString("[%1, %2] °C").arg(widget->minimum()).arg(widget->maximum()));
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
void TBspK400Hf::crtTreeState()
{
    QTreeWidgetItem *item;
    QTreeWidgetItem *top = new QTreeWidgetItem();

    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("Текущее состояние"));

    errRegExp.setPattern("[0-9A-Fa-F]+");
    errValidator.setRegExp(errRegExp);

    Bsp::crtTreeState(top, "Общее", stateGlb);
    Bsp::crtTreeState(top, "Приемник", statePrm);
    Bsp::crtTreeState(top, "Передатчик", statePrd);
    Bsp::crtTreeState(top, "Приемник2", statePrm2);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Управление"));
    top->insertChild(0, item);
    mTree->setItemWidget(item, 1, &mControl);

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
    item->setText(0, kCodec->toUnicode("Подсветка ПИ"));
    top->insertChild(0, item);
    mTree->setItemWidget(item, 1, &mLcdLightOn);

    connect(stateGlb.regime,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &TBspK400Hf::setRegime);

    connect(stateGlb.state,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &TBspK400Hf::setState);

    connect(stateGlb.dopByte,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &TBspK400Hf::setDopByte);

    top->setExpanded(true);
}


//
void TBspK400Hf::crtTest()
{
    QTreeWidgetItem *top = new QTreeWidgetItem(mTree);
    QTreeWidgetItem *item;
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("Тест"));

    item = new QTreeWidgetItem(top);
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("Сигналы 1"));
    mTree->setItemWidget(item, 1, &mTestCf1Signal);

    item = new QTreeWidgetItem(top);
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("Сигналы 2"));
    mTree->setItemWidget(item, 1, &mTestCf2Signal);
}


/**
 * *****************************************************************************
 *
 * @brief Создает ветку журналов.
 *
 * *****************************************************************************
 */
void TBspK400Hf::crtJrn()
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
 * @brief Создает журнал событий.
 * @param[in] top Верхний уровень.
 *
 * *****************************************************************************
 */
void TBspK400Hf::crtJrnGlb(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Количество записей"));
    mTree->setItemWidget(item, 1, &mJrnGlbCounter);
    mJrnGlbCounter.setRange(0, 512);

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
void TBspK400Hf::crtJrnPrm(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Количество записей"));
    mTree->setItemWidget(item, 1, &mJrnPrmCounter);
    mJrnPrmCounter.setRange(0, 512);

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
void TBspK400Hf::crtJrnPrd(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Количество записей"));
    mTree->setItemWidget(item, 1, &mJrnPrdCounter);
    mJrnPrdCounter.setRange(0, 512);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Переполнение"));
    mTree->setItemWidget(item, 1, &mJrnPrdOverflow);
}


/**
 * *****************************************************************************
 *
 * @brief Заполняет список состояний приемника.
 *
 * *****************************************************************************
 */
void TBspK400Hf::FillComboboxListStatePrm()
{
    QComboBox *combobox  = statePrm.state;
    QComboBox *combobox2 = statePrm2.state;

    Bsp::FillComboboxListStatePrm();

    if (combobox != nullptr)
    {
        int index = combobox->findData(14);
        if (index > 0)
        {
            combobox->setItemText(index, kCodec->toUnicode(fcPrmSost14rzsk));
        }
    }

    if (combobox2 != nullptr)
    {
        int index = combobox2->findData(14);
        if (index > 0)
        {
            combobox2->setItemText(index, kCodec->toUnicode(fcPrmSost14rzsk));
        }
    }
}


/**
 * *****************************************************************************
 *
 * @brief Заполнение списка возможных команд управления.
 *
 * *****************************************************************************
 */
void TBspK400Hf::FillComboBoxListControl()
{
    mControl.addItem(kCodec->toUnicode("0 - Нет"), 0);
    mControl.addItem(kCodec->toUnicode("1 - Сброс своего"), 1);
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
void TBspK400Hf::FillComboboxListTest()
{
    mTestCf1Signal.addItem(kCodec->toUnicode("Нет"), 0);
    mTestCf1Signal.addItem(kCodec->toUnicode("КЧ1"), 1);
    mTestCf1Signal.addItem(kCodec->toUnicode("КЧ2"), 2);

    for (int i = 1; i <= 32; i++)
    {
        QString text = kCodec->toUnicode("Команда %1").arg(i);
        mTestCf1Signal.addItem(text, i + 2);
    }

    for (int i = 0; i < mTestCf1Signal.count(); i++)
    {
        mTestCf2Signal.addItem(mTestCf1Signal.itemText(i), mTestCf1Signal.itemData(i));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обработчик изменения совместимости.
 *
 * *****************************************************************************
 */
void TBspK400Hf::SlotChangeCompatibility()
{
    quint8    qty    = 0;
    bool      is_prd = false;
    bool      is_prm = false;
    QSpinBox *spin   = nullptr;

    eGB_COMP_K400 comp = static_cast<eGB_COMP_K400>(GetParamValue(GB_PARAM_COMP_K400));
    switch (comp)
    {
    case GB_COMP_K400_AVANT:
        is_prd = true;
        is_prm = true;
        break;

    case GB_COMP_K400_AVANT_PRD: [[fallthrow]];
    case GB_COMP_K400_AKPA_PRD: [[fallthrow]];
    case GB_COMP_K400_KEDR_PRD: [[fallthrow]];
    case GB_COMP_K400_UPKC_PRD: [[fallthrow]];
    case GB_COMP_K400_VCTO_PRD: [[fallthrow]];
    case GB_COMP_K400_ANKA_PRD: is_prd = true; break;

    case GB_COMP_K400_AVANT_PRM: [[fallthrow]];
    case GB_COMP_K400_AKPA_PRM: [[fallthrow]];
    case GB_COMP_K400_KEDR_PRM: [[fallthrow]];
    case GB_COMP_K400_UPKC_PRM: [[fallthrow]];
    case GB_COMP_K400_VCTO_PRM: [[fallthrow]];
    case GB_COMP_K400_ANKA_PRM: is_prm = true; break;

    case GB_COMP_K400_MAX:
        QString message = "Wrong K400 compatibility value: %1";
        qWarning() << message.arg(comp);
        break;
    }

    spin = mapSpinBox.value(GB_PARAM_PRD_COM_NUMS).at(0);
    qty  = spin->value();
    spin->setValue((is_prd) ? ((qty > 0) ? (qty) : (8)) : (0));
    spin->setEnabled(is_prd);

    spin = mapSpinBox.value(GB_PARAM_PRM_COM_NUMS).at(0);
    qty  = spin->value();
    spin->setValue((is_prm) ? ((qty > 0) ? (qty) : (8)) : (0));
    spin->setEnabled(is_prm);
}

/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи всех параметров приемника.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspK400Hf::HdlrComPrmx10(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == 0x10 || com == 0x90);

    if (com == 0x10)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(GetParamValue(GB_PARAM_PRM_TIME_ON));
        mPkgTx.append(0);  // контроль длительности
        mPkgTx.append(GetParamValue(GB_PARAM_PRM_COM_BLOCK, 1));
        mPkgTx.append(GetParamValue(GB_PARAM_PRM_COM_BLOCK, 2));
        mPkgTx.append(GetParamValue(GB_PARAM_PRM_COM_BLOCK, 3));
        mPkgTx.append(GetParamValue(GB_PARAM_PRM_COM_BLOCK, 4));

        for (int i = 1; i <= 32; i++)
        {
            mPkgTx.append(GetParamValue(GB_PARAM_PRM_TIME_OFF, i));
        }

        mPkgTx.append(GetParamValue(GB_PARAM_PRM_COM_NUMS) * 4);
        mPkgTx.append(GetParamValue(GB_PARAM_PRM_TEST_COM));
        mPkgTx.append(GetParamValue(GB_PARAM_PRM_FREQ_CORR));
        mPkgTx.append(0);
        mPkgTx.append(GetParamValue(GB_PARAM_PRM_DR_ENABLE));
        mPkgTx.append(GetParamValue(GB_PARAM_PRM_DR_COM_BLOCK, 1));
        mPkgTx.append(GetParamValue(GB_PARAM_PRM_DR_COM_BLOCK, 2));
        mPkgTx.append(GetParamValue(GB_PARAM_PRM_DR_COM_BLOCK, 3));
        mPkgTx.append(GetParamValue(GB_PARAM_PRM_DR_COM_BLOCK, 4));

        for (int i = 1; i <= 32; i++)
        {
            mPkgTx.append(GetParamValue(GB_PARAM_PRM_DR_COM_TO_HF, i));
        }

        mPkgTx.append(GetParamValue(GB_PARAM_PRM_COM_SIGNAL, 1));
        mPkgTx.append(GetParamValue(GB_PARAM_PRM_COM_SIGNAL, 2));
        mPkgTx.append(GetParamValue(GB_PARAM_PRM_COM_SIGNAL, 3));
        mPkgTx.append(GetParamValue(GB_PARAM_PRM_COM_SIGNAL, 4));

        int len = 95 - mPkgTx.size();
        for (int i = 0; i < len; i++)
        {
            mPkgTx.append(0);
        }

        Q_ASSERT(mPkgTx.size() == 96);  // команда + 95 байт данных
    }

    if (com == 0x90)
    {
        if (!CheckSize(com, data.size(), { 95 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        int index = 0;
        SetParamValue(GB_PARAM_PRM_TIME_ON, data.at(index++));
        index++;  // контроль длительности
        SetParamValue(GB_PARAM_PRM_COM_BLOCK, data.at(index++), 1);
        SetParamValue(GB_PARAM_PRM_COM_BLOCK, data.at(index++), 2);
        SetParamValue(GB_PARAM_PRM_COM_BLOCK, data.at(index++), 3);
        SetParamValue(GB_PARAM_PRM_COM_BLOCK, data.at(index++), 4);

        for (int i = 1; i <= 32; i++)
        {
            SetParamValue(GB_PARAM_PRM_TIME_OFF, data.at(index++), i);
        }

        SetParamValue(GB_PARAM_PRM_COM_NUMS, data.at(index++) / 4);
        SetParamValue(GB_PARAM_PRM_TEST_COM, data.at(index++));
        SetParamValue(GB_PARAM_PRM_FREQ_CORR, data.at(index++));
        index++;
        index += 37;  // количество параметров ЦПП

        SetParamValue(GB_PARAM_PRM_COM_SIGNAL, data.at(index++), 1);
        SetParamValue(GB_PARAM_PRM_COM_SIGNAL, data.at(index++), 2);
        SetParamValue(GB_PARAM_PRM_COM_SIGNAL, data.at(index++), 3);
        SetParamValue(GB_PARAM_PRM_COM_SIGNAL, data.at(index++), 4);
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи всех параметров передатчика.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspK400Hf::HdlrComPrdx20(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == 0x20 || com == 0xA0);

    if (com == 0x20)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_IN_DELAY));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_DURATION_L));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_BLOCK, 1));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_BLOCK, 2));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_BLOCK, 3));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_BLOCK, 4));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_LONG, 1));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_LONG, 2));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_LONG, 3));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_LONG, 4));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_TEST_COM));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_NUMS_A));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_NUMS) * 4);
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_FREQ_CORR));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_DR_ENABLE));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_DR_COM_BLOCK, 1));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_DR_COM_BLOCK, 2));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_DR_COM_BLOCK, 3));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_DR_COM_BLOCK, 4));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_DEC_CF));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_DEC_TM));
        mPkgTx.append(0);  // КС по умолчанию
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_SIGNAL, 1));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_SIGNAL, 2));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_SIGNAL, 3));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_SIGNAL, 4));


        int len = 32 - mPkgTx.size();
        for (int i = 0; i < len; i++)
        {
            mPkgTx.append(0);
        }

        Q_ASSERT(mPkgTx.size() == 32);  // команда + 31 байт данных
    }

    if (com == 0xA0)
    {
        // из конфигуратора почему-то передается 32 байта
        if (!CheckSize(com, data.size(), { 31, 32 }))
        {
            return;
        }

        // ответ на команду записи совпадает с запросом
        mPkgTx.append(com);
        mPkgTx.append(data);

        int index = 0;
        SetParamValue(GB_PARAM_PRD_IN_DELAY, data.at(index++));
        SetParamValue(GB_PARAM_PRD_DURATION_L, data.at(index++));
        SetParamValue(GB_PARAM_PRD_COM_BLOCK, data.at(index++), 1);
        SetParamValue(GB_PARAM_PRD_COM_BLOCK, data.at(index++), 2);
        SetParamValue(GB_PARAM_PRD_COM_BLOCK, data.at(index++), 3);
        SetParamValue(GB_PARAM_PRD_COM_BLOCK, data.at(index++), 4);
        SetParamValue(GB_PARAM_PRD_COM_LONG, data.at(index++), 1);
        SetParamValue(GB_PARAM_PRD_COM_LONG, data.at(index++), 2);
        SetParamValue(GB_PARAM_PRD_COM_LONG, data.at(index++), 3);
        SetParamValue(GB_PARAM_PRD_COM_LONG, data.at(index++), 4);
        SetParamValue(GB_PARAM_PRD_TEST_COM, data.at(index++));
        SetParamValue(GB_PARAM_PRD_COM_NUMS_A, data.at(index++));
        SetParamValue(GB_PARAM_PRD_COM_NUMS, data.at(index++) / 4);
        SetParamValue(GB_PARAM_PRD_FREQ_CORR, static_cast<qint8>(data.at(index++)));
        index += 5;  // параметры ЦПП
        SetParamValue(GB_PARAM_PRD_DEC_CF, data.at(index++));
        SetParamValue(GB_PARAM_PRD_DEC_TM, data.at(index++));
        index++;  // КС по умолчанию
        SetParamValue(GB_PARAM_PRD_COM_SIGNAL, data.at(index++), 1);
        SetParamValue(GB_PARAM_PRD_COM_SIGNAL, data.at(index++), 2);
        SetParamValue(GB_PARAM_PRD_COM_SIGNAL, data.at(index++), 3);
        SetParamValue(GB_PARAM_PRD_COM_SIGNAL, data.at(index++), 4);
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
void TBspK400Hf::HdlrComGlbx30(eGB_COM com, pkg_t &data)
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

        if (data.size() == 1)
        {
            setSpinBoxValue(m_measure.T, data.at(0));
        }

        mPkgTx.append(com);
        mPkgTx.append(0);
        mPkgTx.append(0);
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

        mPkgTx.append((mLcdLightOn.isChecked()) ? 1 : 0);

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
        SetParamValue(GB_PARAM_WARN_THD, data.at(index++));
        SetParamValue(GB_PARAM_NUM_OF_DEVICE, data.at(index++));
        SetParamValue(GB_PARAM_TIME_SYNCH, data.at(index++));
        SetParamValue(GB_PARAM_NET_ADDRESS, data.at(index++));

        quint16 freq = data.at(index++);
        freq += static_cast<quint16>(data.at(index++)) << 8;
        SetParamValue(GB_PARAM_FREQ, freq);

        // коррекции с ПК не устанавливаются
        index++;  // Коррекция значения выходного напряжения (целая часть)
        index++;  // Коррекция значения выходного напряжения (дробная цасть)
        index++;  // Коррекция значения тока 1 (младший байт)
        index++;  // Коррекция значения тока 1 (старший байт)
        index++;  // Коррекция значения тока 2 (младший байт)
        index++;  // Коррекция значения тока 2 (старший байт)

        SetParamValue(GB_PARAM_NUM_OF_DEVICES, data.at(index++));
        SetParamValue(GB_PARAM_COMP_K400, data.at(index++));
        SetParamValue(GB_PARAM_TM_K400, data.at(index++));
        SetParamValue(GB_PARAM_WARN_D, static_cast<qint8>(data.at(index++)));
        SetParamValue(GB_PARAM_ALARM_D, static_cast<qint8>(data.at(index++)));
        SetParamValue(GB_PARAM_TEMP_MONITOR, data.at(index++));
        SetParamValue(GB_PARAM_TEMP_THR_HI, data.at(index++));
        SetParamValue(GB_PARAM_TEMP_THR_LOW, data.at(index++));
        SetParamValue(GB_PARAM_TM_SPEED, data.at(index++));
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
void TBspK400Hf::HdlrComGlbx31(eGB_COM com, pkg_t &data)
{
    bool    ok;
    quint16 prm_alarm  = getLineEditValue(statePrm.fault).toUInt(&ok, 16);
    quint16 prm_warn   = getLineEditValue(statePrm.warning).toUInt(&ok, 16);
    quint16 prm2_alarm = getLineEditValue(statePrm2.fault).toUInt(&ok, 16);
    quint16 prm2_warn  = getLineEditValue(statePrm2.warning).toUInt(&ok, 16);
    quint16 prd_alarm  = getLineEditValue(statePrd.fault).toUInt(&ok, 16);
    quint16 prd_warn   = getLineEditValue(statePrd.warning).toUInt(&ok, 16);
    quint16 glb_alarm  = getLineEditValue(stateGlb.fault).toUInt(&ok, 16);
    quint16 glb_warn   = getLineEditValue(stateGlb.warning).toUInt(&ok, 16);

    Q_ASSERT(com == GB_COM_GET_FAULT);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    mPkgTx.append(com);
    mPkgTx.append(0);
    mPkgTx.append(0);
    mPkgTx.append(0);
    mPkgTx.append(0);

    mPkgTx.append(static_cast<uint8_t>(prm_alarm >> 8));
    mPkgTx.append(static_cast<uint8_t>(prm_alarm));
    mPkgTx.append(static_cast<uint8_t>(prm_warn >> 8));
    mPkgTx.append(static_cast<uint8_t>(prm_warn));

    mPkgTx.append(static_cast<uint8_t>(prd_alarm >> 8));
    mPkgTx.append(static_cast<uint8_t>(prd_alarm));
    mPkgTx.append(static_cast<uint8_t>(prd_warn >> 8));
    mPkgTx.append(static_cast<uint8_t>(prd_warn));

    mPkgTx.append(static_cast<uint8_t>(glb_alarm >> 8));
    mPkgTx.append(static_cast<uint8_t>(glb_alarm));
    mPkgTx.append(static_cast<uint8_t>(glb_warn >> 8));
    mPkgTx.append(static_cast<uint8_t>(glb_warn));

    mPkgTx.append(static_cast<uint8_t>(prm2_alarm >> 8));
    mPkgTx.append(static_cast<uint8_t>(prm2_alarm));
    mPkgTx.append(static_cast<uint8_t>(prm2_warn >> 8));
    mPkgTx.append(static_cast<uint8_t>(prm2_warn));

    Q_ASSERT(mPkgTx.size() == 21);  // команда + 20 байт данных
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
void TBspK400Hf::HdlrComGlbx34(eGB_COM com, pkg_t &data)
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

    mPkgTx.append(getSpinBoxValue(m_measure.D));
    mPkgTx.append(0);  // в БСП 2Uз
    mPkgTx.append(getSpinBoxValue(m_measure.Ucf1));
    mPkgTx.append(getSpinBoxValue(m_measure.Ucf2));
    mPkgTx.append(getSpinBoxValue(m_measure.Un1));
    mPkgTx.append(getSpinBoxValue(m_measure.Un2));
    mPkgTx.append(0);
    mPkgTx.append(0);
    mPkgTx.append(getSpinBoxValue(m_measure.T));
    mPkgTx.append(getSpinBoxValue(m_measure.dF));

    Q_ASSERT(mPkgTx.size() == 18);  // команда + 17 байт данных
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи синхронизации времени.
 * @note Эта же команда используется для чтения/записи всех параметров приемника с
 конфигуратора
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspK400Hf::HdlrComGlbx35(eGB_COM com, pkg_t &data)
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
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_WARN_THD)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_NUM_OF_DEVICE)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TIME_SYNCH)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_NET_ADDRESS)));

            quint16 freq = GetParamValue(GB_PARAM_FREQ);
            mPkgTx.append(static_cast<uint8_t>(freq));
            mPkgTx.append(static_cast<uint8_t>(freq >> 8));

            qint16 corr_u = GetParamValue(GB_PARAM_COR_U);
            mPkgTx.append(corr_u / 10);
            mPkgTx.append((corr_u % 10) * 10);

            qint16 corr_i = GetParamValue(GB_PARAM_COR_I);
            mPkgTx.append(static_cast<uint8_t>(corr_i));
            mPkgTx.append(static_cast<uint8_t>(corr_i >> 8));
            mPkgTx.append(0);  // Коррекция значения тока 2, младший байт
            mPkgTx.append(0);  // Коррекция значения тока 2, старший байт

            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_NUM_OF_DEVICES)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_COMP_K400)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TM_K400)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_WARN_D)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_ALARM_D)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TEMP_MONITOR)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TEMP_THR_HI)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TEMP_THR_LOW)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TM_SPEED)));

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

            Q_ASSERT(mPkgTx.size() == 2);  // команда + 1 байт данных
        }
    }

    if (com == GB_COM_SET_TIME_SINCHR)
    {
        // из меню всегда идет 2 байта, второй с номером параметрв
        if (!CheckSize(com, data.size(), { 1, 2 }))
        {
            return;
        }

        SetParamValue(GB_PARAM_TIME_SYNCH, data.at(0));
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
void TBspK400Hf::HdlrComGlbx37(eGB_COM com, pkg_t &data)
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
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_COMP_K400)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TM_K400)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_WARN_D)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_ALARM_D)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TEMP_MONITOR)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TEMP_THR_HI)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TEMP_THR_LOW)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TM_SPEED)));

        Q_ASSERT(mPkgTx.size() == 10);  // команда + 9 байт данных
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

        quint8 value = data.at(0);
        int    param = (data.size() == 1) ? 1 : data.at(1);
        switch (param)
        {
        case 1: SetParamValue(GB_PARAM_COM_PRD_KEEP, value); break;
        case 2: SetParamValue(GB_PARAM_COMP_K400, value); break;
        case 3: SetParamValue(GB_PARAM_TM_K400, value); break;
        case 4: SetParamValue(GB_PARAM_WARN_D, qint8(value)); break;
        case 5: SetParamValue(GB_PARAM_ALARM_D, qint8(value)); break;
        case 6: SetParamValue(GB_PARAM_TEMP_MONITOR, value); break;
        case 7: SetParamValue(GB_PARAM_TEMP_THR_HI, value); break;
        case 8: SetParamValue(GB_PARAM_TEMP_THR_LOW, value); break;
        case 9: SetParamValue(GB_PARAM_TM_SPEED, value); break;
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
 * @brief Обрабатывает команду чтения/записи порога предупреждения КЧ и загруб. чувств.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspK400Hf::HdlrComGlbx3C(eGB_COM com, pkg_t &data)
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
        case 1: SetParamValue(GB_PARAM_WARN_THD, data.at(1)); break;
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
void TBspK400Hf::HdlrComGlbx3E(eGB_COM com, pkg_t &data)
{
    quint8  cf1 = 0, cf2 = 0;
    quint32 cm1 = 0, cm2 = 0;
    quint8  signal1 = mTestCf1Signal.currentData().toUInt();
    quint8  signal2 = mTestCf2Signal.currentData().toUInt();

    Q_ASSERT(com == GB_COM_GET_TEST);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    if (signal1 > 0)
    {
        if (signal1 == 1 || signal1 == 2)
        {
            cf1 = 1 << (signal1 - 1);
        }
        else if (signal1 <= (32 + 2))
        {
            cm1 = 1UL << (signal1 - 3);
        }
    }

    if (signal2 > 0)
    {
        if (signal2 == 1 || signal2 == 2)
        {
            cf2 = 1 << (signal2 - 1);
        }
        else if (signal1 <= (32 + 2))
        {
            cm2 = 1UL << (signal2 - 3);
        }
    }

    mPkgTx.append(com);
    mPkgTx.append(cf1);
    mPkgTx.append(static_cast<quint8>(cm1));
    mPkgTx.append(static_cast<quint8>(cm1 >> 8));
    mPkgTx.append(static_cast<quint8>(cm1 >> 16));
    mPkgTx.append(static_cast<quint8>(cm1 >> 24));
    mPkgTx.append(cf2);
    mPkgTx.append(static_cast<quint8>(cm2));
    mPkgTx.append(static_cast<quint8>(cm2 >> 8));
    mPkgTx.append(static_cast<quint8>(cm2 >> 16));
    mPkgTx.append(static_cast<quint8>(cm2 >> 24));

    Q_ASSERT(mPkgTx.size() == 11);  // команда + 10 байт данных
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
void TBspK400Hf::HdlrComGlbx3F(eGB_COM com, pkg_t &data)
{
    quint16 vers_bsp_mcu      = getSpinBoxValue(mDevice.versionBspMcu);
    quint16 vers_bsp_dsp      = getSpinBoxValue(mDevice.versionBspDsp);
    quint16 vers_bsp_dsp_plis = getSpinBoxValue(mDevice.versionDspPlis);

    Q_ASSERT(com == GB_COM_GET_VERS);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    mPkgTx.append(com);
    mPkgTx.append(0);                                     // защита
    mPkgTx.append(GetParamValue(GB_PARAM_PRM_COM_NUMS));  // прм1
    mPkgTx.append(0);                                     // прм2
    mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_NUMS));  // прд
    mPkgTx.append(GetParamValue(GB_PARAM_NUM_OF_DEVICES) + 1);
    mPkgTx.append(GB_TYPE_LINE_UM);
    mPkgTx.append(static_cast<quint8>(vers_bsp_mcu >> 8));
    mPkgTx.append(static_cast<quint8>(vers_bsp_mcu));
    mPkgTx.append(static_cast<quint8>(vers_bsp_dsp >> 8));
    mPkgTx.append(static_cast<quint8>(vers_bsp_dsp));
    mPkgTx.append(GetParamValue(GB_PARAM_COMP_K400));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(mDevice.versionBsk1PrdPlis)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(mDevice.versionBsk2PrdPlis)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(mDevice.versionBsk1PrmPlis)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(mDevice.versionBsk2PrmPlis)));
    mPkgTx.append(0);
    mPkgTx.append(AVANT_K400);
    mPkgTx.append(0);  // версия ПИ
    mPkgTx.append(0);  // версия ПИ
    mPkgTx.append(0);  // признак оптического кольца
    mPkgTx.append(static_cast<quint8>(vers_bsp_dsp_plis >> 8));
    mPkgTx.append(static_cast<quint8>(vers_bsp_dsp_plis));

    Q_ASSERT(mPkgTx.size() == 23);  // команда + 22 байт данных
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
void TBspK400Hf::HdlrComRegx7E(eGB_COM com, pkg_t &data)
{
    quint8  cf = 0;
    quint32 cm = 0;

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
        mTestCf2Signal.setCurrentIndex(0);
    }
    else
    {
        switch (data.at(0))
        {
        case 1:
            {
                quint8 signal = data.at(1);

                int index = mTestCf1Signal.findData(signal);
                if (index < 0)
                {
                    QString message = "Wrong test CF value: %1";
                    qWarning() << message.arg(signal);
                }
                mTestCf1Signal.setCurrentIndex(index);

                if (signal > 0)
                {
                    if (signal == 1 || signal == 2)
                    {
                        cf = 1 << (signal - 1);
                    }
                    else if (signal <= (32 + 2))
                    {
                        cm = 1UL << (signal - 3);
                    }
                }

                break;
            }
        case 2:
            // Из меню всегда посылается две группы сигналов: КЧ и РЗ.
            break;

        default:
            QString message = "Wrong test group value: %1";
            qWarning() << message.arg(data.at(0));
        }
    }

    mPkgTx.append(com);
    mPkgTx.append(cf);
    mPkgTx.append(static_cast<quint8>(cm));
    mPkgTx.append(static_cast<quint8>(cm >> 8));
    mPkgTx.append(static_cast<quint8>(cm >> 16));
    mPkgTx.append(static_cast<quint8>(cm >> 24));

    Q_ASSERT(mPkgTx.size() == 6);  // команда + 5 байт данных
}
