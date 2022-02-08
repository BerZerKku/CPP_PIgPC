#include <QDebug>
#include <QHeaderView>

#include "bspR400m_hf_b15.hpp"

#include "PIg/src/flash.h"
#include "PIg/src/menu/control.h"
#include "PIg/src/menu/txCom.h"
#include "PIg/src/parameter/param.h"

TBspR400mHf_b15::TBspR400mHf_b15(QTreeWidget *tree, QWidget *parent) : Bsp(tree, parent)
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
void TBspR400mHf_b15::InitComMap()
{
    mComMap.insert(0x02, &Bsp::HdlrComDefx02);  // чтение количества аппаратов в линии
    mComMap.insert(0x0A, &Bsp::HdlrComDefx0A);  // чтение режима АК и времени до АК

    mComMap.insert(0x30, &Bsp::HdlrComGlbx30);  // чтение режимов
    mComMap.insert(0x31, &Bsp::HdlrComGlbx31);  // чтение неисправностей
    mComMap.insert(0x32, &Bsp::HdlrComGlbx32);  // чтение времени
    mComMap.insert(0x34, &Bsp::HdlrComGlbx34);  // чтение измерений
    mComMap.insert(0x37, &Bsp::HdlrComGlbx37);  // чтение совместимости
    mComMap.insert(0x38, &Bsp::HdlrComGlbx38);  // чтение адреса в локальной сети
    mComMap.insert(0x3B, &Bsp::HdlrComGlbx3B);  // чтение номера аппарата
    mComMap.insert(0x3f, &Bsp::HdlrComGlbx3F);  // чтение информации об устройстве

    mComMap.insert(0x72, &Bsp::HdlrComRegx72);  // запись команды управления

    mComMap.insert(0x82, &Bsp::HdlrComDefx02);  // запись количества аппаратов в линии
    mComMap.insert(0x8A, &Bsp::HdlrComDefx0A);  // запись режима АК и времени до АК

    mComMap.insert(0xB2, &Bsp::HdlrComGlbx32);  // запись времени
    mComMap.insert(0xB7, &Bsp::HdlrComGlbx37);  // запись совместимости
    mComMap.insert(0xB8, &Bsp::HdlrComGlbx38);  // запись адреса в локальной сети
    mComMap.insert(0xBB, &Bsp::HdlrComGlbx3B);  // запись номера аппарата

    mComMap.insert(0xC1, &Bsp::HdlrComJrnxC1);  // запись адреса в локальной сети
    mComMap.insert(0xF1, &Bsp::HdlrComJrnxF1);  // запись номера аппарата
}


//
void TBspR400mHf_b15::InitParam()
{
    eGB_PARAM param = GB_PARAM_NULL_PARAM;

    setComboBoxValue(GB_PARAM_NUM_OF_DEVICES, GB_NUM_DEVICES_2);
    setComboBoxValue(GB_PARAM_COMP_P400, GB_COMP_R400M_LINER);

    setSpinBoxValue(mDevice.versionBspMcu, 0xF232);
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


void TBspR400mHf_b15::crtTreeDef()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);

    top->setText(0, kCodec->toUnicode("Защита"));
    crtComboBox(GB_PARAM_NUM_OF_DEVICES);
    crtComboBox(GB_PARAM_DEF_ONE_SIDE);

    top->setExpanded(false);
}


void TBspR400mHf_b15::crtTreeDevice()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("Устройство"));

    top->setExpanded(true);

    crtTreeDevieVersions(top);
}


void TBspR400mHf_b15::crtTreeDevieVersions(QTreeWidgetItem *top)
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


void TBspR400mHf_b15::crtTreeGlb()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);

    top->setText(0, kCodec->toUnicode("Общие"));

    crtComboBox(GB_PARAM_COMP_P400);
    crtSpinBox(GB_PARAM_NUM_OF_DEVICE);
    crtComboBox(GB_PARAM_COM_PRD_KEEP);
    crtComboBox(GB_PARAM_COM_PRM_KEEP);
    crtComboBox(GB_PARAM_TIME_SYNCH);

    top->setExpanded(false);
}


void TBspR400mHf_b15::crtTreeMeasure()
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


void TBspR400mHf_b15::crtTreePrd()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);

    top->setText(0, kCodec->toUnicode("Передатчик"));

    crtSpinBox(GB_PARAM_PRD_IN_DELAY);
    //    crtComboBox(GB_PARAM_PRD_COM_BLOCK);
    //    crtSpinBox(GB_PARAM_PRD_DURATION_L);
    crtSpinBox(GB_PARAM_PRD_DURATION_O);
    crtComboBox(GB_PARAM_PRD_COM_LONG);
    crtComboBox(GB_PARAM_PRD_COM_SIGNAL);

    top->setExpanded(false);
}


void TBspR400mHf_b15::crtTreePrm()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);

    top->setText(0, kCodec->toUnicode("Приемник"));

    // FIXME Есть два вида задержки на фиксацию команды!
    crtSpinBox(GB_PARAM_PRM_TIME_ON);
    //    crtComboBox(GB_PARAM_PRM_COM_BLOCK_ALL);
    crtComboBox(GB_PARAM_PRM_COM_BLOCK);
    crtSpinBox(GB_PARAM_PRM_TIME_OFF);
    crtComboBox(GB_PARAM_PRD_DR_ENABLE);
    crtComboBox(GB_PARAM_PRM_DR_COM_BLOCK);
    crtSpinBox(GB_PARAM_PRM_DR_COM_TO_HF);
    crtComboBox(GB_PARAM_PRM_COM_SIGNAL);

    top->setExpanded(false);
}


//
void TBspR400mHf_b15::crtTreeState()
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
            &TBspR400mHf_b15::setRegime);

    connect(stateGlb.state,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &TBspR400mHf_b15::setState);

    connect(stateGlb.dopByte,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &TBspR400mHf_b15::setDopByte);

    top->setExpanded(true);
}


//
void TBspR400mHf_b15::crtTest()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    QTreeWidgetItem *item;
    QLineEdit *      lineedit;
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("Тест"));

    item       = new QTreeWidgetItem();
    lineedit   = new QLineEdit();
    test.byte1 = lineedit;
    lineedit->setMaxLength(2);
    lineedit->setInputMask("HH");
    lineedit->setText("00");
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("Байт 1"));
    mTree->setItemWidget(item, 1, lineedit);

    item       = new QTreeWidgetItem();
    lineedit   = new QLineEdit();
    test.byte2 = lineedit;
    lineedit->setMaxLength(2);
    lineedit->setInputMask("HH");
    lineedit->setText("00");
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("Байт 2"));
    mTree->setItemWidget(item, 1, lineedit);

    item       = new QTreeWidgetItem();
    lineedit   = new QLineEdit();
    test.byte3 = lineedit;
    lineedit->setMaxLength(2);
    lineedit->setInputMask("HH");
    lineedit->setText("00");
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("Байт 3"));
    mTree->setItemWidget(item, 1, lineedit);

    item       = new QTreeWidgetItem();
    lineedit   = new QLineEdit();
    test.byte4 = lineedit;
    lineedit->setMaxLength(2);
    lineedit->setInputMask("HH");
    lineedit->setText("00");
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("Байт 4"));
    mTree->setItemWidget(item, 1, lineedit);

    item       = new QTreeWidgetItem();
    lineedit   = new QLineEdit();
    test.byte5 = lineedit;
    lineedit->setMaxLength(2);
    lineedit->setInputMask("HH");
    lineedit->setText("00");
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("Байт 5"));
    mTree->setItemWidget(item, 1, lineedit);

    top->setExpanded(false);
}


/**
 * *****************************************************************************
 *
 * @brief Создает ветку журналов.
 *
 * *****************************************************************************
 */
void TBspR400mHf_b15::crtJrn()
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
void TBspR400mHf_b15::crtJrnDef(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Количество записей"));
    mTree->setItemWidget(item, 1, &mJrnGlbCounter);
    mJrnGlbCounter.setRange(0, 1024);

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
void TBspR400mHf_b15::crtJrnGlb(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("Количество записей"));
    mTree->setItemWidget(item, 1, &mJrnDefCounter);
    mJrnDefCounter.setRange(0, 1024);

    top->setExpanded(false);
}


void TBspR400mHf_b15::FillComboboxListStateDef()
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
void TBspR400mHf_b15::FillComboboxListAc()
{
    // из fcAcType

    mAc.addItem(kCodec->toUnicode(fcAcType[0]), 0);

    QString value_1 = QString("%1 / %2")
                          .arg(kCodec->toUnicode(fcAcType[GB_TYPE_AC_AUTO_NORM]))
                          .arg(kCodec->toUnicode(fcAcType[GB_TYPE_AC_AUTO]));
    mAc.addItem(value_1, 1);

    mAc.addItem(kCodec->toUnicode(fcAcType[GB_TYPE_AC_AUTO_REPEAT]), GB_TYPE_AC_AUTO_REPEAT);
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
void TBspR400mHf_b15::FillComboBoxListControl()
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
 * @brief Обрабатывает команду чтения количества аппаратов в линии.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf_b15::HdlrComDefx02(eGB_COM com, pkg_t &data)
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
 * @brief Обрабатывает команду чтения режима АК и времени до АК
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf_b15::HdlrComDefx0A(eGB_COM com, pkg_t &data)
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
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf_b15::HdlrComGlbx30(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_SOST);

    if (!CheckSize(com, data.size(), { 1 }))
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


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения несиправностей и предупреждений.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf_b15::HdlrComGlbx31(eGB_COM com, pkg_t &data)
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
void TBspR400mHf_b15::HdlrComGlbx32(eGB_COM com, pkg_t &data)
{
    bool     ok;
    uint16_t msec = dt.time().msec();

    Q_ASSERT(com == GB_COM_GET_TIME || com == GB_COM_SET_TIME);

    if (com == GB_COM_GET_TIME)
    {
        // C ПИ передается флаг запроса сообщения для передачи в АСУ (0..3), с ПК нет данных
        if (!CheckSize(com, data.size(), { 0, 1 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(int2bcd(dt.date().year() - 2000, ok));
        mPkgTx.append(int2bcd(dt.date().month(), ok));
        mPkgTx.append(int2bcd(dt.date().day(), ok));
        mPkgTx.append(int2bcd(dt.time().hour(), ok));
        mPkgTx.append(int2bcd(dt.time().minute(), ok));
        mPkgTx.append(int2bcd(dt.time().second(), ok));
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

        bool    ok    = false;
        quint16 year  = bcd2int(data.takeFirst(), ok) + 2000;
        quint8  month = bcd2int(data.takeFirst(), ok);
        quint8  day   = bcd2int(data.takeFirst(), ok);

        quint8 hour   = bcd2int(data.takeFirst(), ok);
        quint8 minute = bcd2int(data.takeFirst(), ok);
        quint8 second = bcd2int(data.takeFirst(), ok);

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
void TBspR400mHf_b15::HdlrComGlbx34(eGB_COM com, pkg_t &data)
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

    // @todo Разобраться что это за измерение "noiseCounter_pvzu" и что такое "супер меню"
    mPkgTx.append(0);  // Uart1_ArrData[5] = noiseCounter_pvzu >> 8;
    mPkgTx.append(0);  // Uart1_ArrData[5] = noiseCounter_pvzu

    value = getSpinBoxValue(m_measure.I);
    mPkgTx.append(static_cast<quint8>(value >> 8));
    mPkgTx.append(static_cast<quint8>(value));

    value = getSpinBoxValue(m_measure.U);
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
 * @brief Обрабатывает команду чтения/записи совместимости.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf_b15::HdlrComGlbx37(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_COM_PRD_KEEP || com == GB_COM_SET_COM_PRD_KEEP);

    if (com == GB_COM_GET_COM_PRD_KEEP)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(getComboBoxValue(GB_PARAM_COMP_P400));

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

        setComboBoxValue(GB_PARAM_COMP_P400, data.at(0));
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
void TBspR400mHf_b15::HdlrComGlbx38(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_NET_ADR || com == GB_COM_SET_NET_ADR);

    if (com == GB_COM_GET_NET_ADR)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(getSpinBoxValue(GB_PARAM_NET_ADDRESS));

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

        setSpinBoxValue(GB_PARAM_NET_ADDRESS, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения/записи номера аппарата
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf_b15::HdlrComGlbx3B(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_DEVICE_NUM || com == GB_COM_SET_DEVICE_NUM);

    if (com == GB_COM_GET_DEVICE_NUM)
    {
        if (!CheckSize(com, data.size(), { 0 }))
        {
            return;
        }

        mPkgTx.append(com);
        mPkgTx.append(getSpinBoxValue(GB_PARAM_NUM_OF_DEVICE));

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

        setSpinBoxValue(GB_PARAM_NUM_OF_DEVICE, data.at(0));
    }
}

/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения версии устройства
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf_b15::HdlrComGlbx3F(eGB_COM com, pkg_t &data)
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
 * @brief Обрабатывает команду управления.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void TBspR400mHf_b15::HdlrComRegx72(eGB_COM com, pkg_t &data)
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

    mControl.setCurrentIndex(mControl.findData(control));
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
void TBspR400mHf_b15::HdlrComJrnxC1(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_DEF_GET_JRN_CNT);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    uint16_t len = mJrnDefCounter.value();

    mPkgTx.append(com);
    mPkgTx.append(len);
    mPkgTx.append(len >> 8);

    Q_ASSERT(mPkgTx.size() == 3);  // команда + 2 байта данных
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
void TBspR400mHf_b15::HdlrComJrnxF1(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_JRN_CNT);

    if (!CheckSize(com, data.size(), { 0 }))
    {
        return;
    }

    // @todo Добавить возможность переполнени
    uint16_t len = mJrnGlbCounter.value();

    mPkgTx.append(com);
    mPkgTx.append(len);
    mPkgTx.append(len >> 8);

    Q_ASSERT(mPkgTx.size() == 3);  // команда + 2 байта данных
}
