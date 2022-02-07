#include <QDebug>
#include <QHeaderView>

#include "bspR400m_hf_b15.hpp"

#include "PIg/src/flash.h"
#include "PIg/src/menu/txCom.h"
#include "PIg/src/parameter/param.h"


TBspR400mHf_b15::TBspR400mHf_b15(QTreeWidget *tree, QWidget *parent) : Bsp(tree, parent)
{
}


/**
 * *****************************************************************************
 *
 * @brief ����������� ����� ������.
 *
 * *****************************************************************************
 */
void TBspR400mHf_b15::InitComMap()
{
    mComMap.insert(0x02, &Bsp::HdlrComDefx02);  // ������ ���������� ��������� � �����

    mComMap.insert(0x30, &Bsp::HdlrComGlbx30);  // ������ �������
    mComMap.insert(0x31, &Bsp::HdlrComGlbx31);  // ������ ��������������
    mComMap.insert(0x32, &Bsp::HdlrComGlbx32);  // ������ �������
    mComMap.insert(0x34, &Bsp::HdlrComGlbx34);  // ������ ���������
    mComMap.insert(0x37, &Bsp::HdlrComGlbx37);  // ������ �������������
    mComMap.insert(0x38, &Bsp::HdlrComGlbx38);  // ������ ������ � ��������� ����
    mComMap.insert(0x3B, &Bsp::HdlrComGlbx3B);  // ������ ������ ��������
    mComMap.insert(0x3f, &Bsp::HdlrComGlbx3F);  // ������ ���������� �� ����������

    mComMap.insert(0x82, &Bsp::HdlrComDefx02);  // ������ ���������� ��������� � �����

    mComMap.insert(0xB2, &Bsp::HdlrComGlbx32);  // ������ �������
    mComMap.insert(0xB7, &Bsp::HdlrComGlbx37);  // ������ �������������
    mComMap.insert(0xB8, &Bsp::HdlrComGlbx38);  // ������ ������ � ��������� ����
    mComMap.insert(0xBB, &Bsp::HdlrComGlbx3B);  // ������ ������ ��������
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
    setComboBoxValue(stateGlb.state, 1);  // 1 - ��������
    setSpinBoxValue(stateGlb.dopByte, 1);

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
}


void TBspR400mHf_b15::crtTreeDef()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);

    top->setText(0, kCodec->toUnicode("������"));
    crtComboBox(GB_PARAM_NUM_OF_DEVICES);
    crtComboBox(GB_PARAM_DEF_ONE_SIDE);

    top->setExpanded(false);
}


void TBspR400mHf_b15::crtTreeDevice()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("����������"));

    top->setExpanded(true);

    crtTreeDevieVersions(top);
}


void TBspR400mHf_b15::crtTreeDevieVersions(QTreeWidgetItem *top)
{
    QTreeWidgetItem *branch = new QTreeWidgetItem();
    top->addChild(branch);
    branch->setText(0, kCodec->toUnicode("������ ��"));

    QTreeWidgetItem *item = nullptr;

    item                  = new QTreeWidgetItem();
    mDevice.versionBspMcu = new QHexSpinBox(false);
    item->setText(0, kCodec->toUnicode("��� ��"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBspMcu);

    // \todo ������ DSP � �400 ������� �� 2 ����, ����� (������ ����, ������ ���)
    item                  = new QTreeWidgetItem();
    mDevice.versionBspDsp = new QHexSpinBox(false);
    item->setText(0, kCodec->toUnicode("��� ���"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBspDsp);

    item                   = new QTreeWidgetItem();
    mDevice.versionBszPlis = new QHexSpinBox(true);
    item->setText(0, kCodec->toUnicode("��� ����"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBszPlis);

    branch->setExpanded(false);
}


void TBspR400mHf_b15::crtTreeGlb()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);

    top->setText(0, kCodec->toUnicode("�����"));

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

    top->setText(0, kCodec->toUnicode("���������"));

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
    item->setText(0, kCodec->toUnicode("U�1"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.Udef1 = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(-99, 99);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("U�2"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.Udef2 = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(-99, 99);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("U�1"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.Ucf1 = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(-99, 99);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("U�2"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.Ucf2 = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(-99, 99);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("U�1"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.Un1 = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(-99, 99);
    widget->setToolTip(QString("[%1, %2] dB").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("U�2"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.Un2 = widget;

    item   = new QTreeWidgetItem();
    widget = new QSpinBox();
    widget->setRange(0, 360);
    widget->setToolTip(QString("[%1, %2] deg").arg(widget->minimum()).arg(widget->maximum()));
    item->setText(0, kCodec->toUnicode("S�"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, widget);
    m_measure.Sd = widget;

    top->setExpanded(false);
}


void TBspR400mHf_b15::crtTreePrd()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();
    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);

    top->setText(0, kCodec->toUnicode("����������"));

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

    top->setText(0, kCodec->toUnicode("��������"));

    // FIXME ���� ��� ���� �������� �� �������� �������!
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
    top->setText(0, kCodec->toUnicode("������� ���������"));

    errRegExp.setPattern("[0-9A-Fa-F]+");
    errValidator.setRegExp(errRegExp);

    Bsp::crtTreeState(top, "�����", stateGlb);

    def  = Bsp::crtTreeState(top, "������", stateDef);
    item = new QTreeWidgetItem();
    item->setText(0, kCodec->toUnicode("����� ���������� ����"));
    def->addChild(item);
    mTree->setItemWidget(item, 1, &mStateFaultDeviceNumber);
    mStateFaultDeviceNumber.setRange(0, 255);  // @fixme �������� �� �������� ��������

    item = new QTreeWidgetItem();
    item->setText(0, kCodec->toUnicode("�����������"));
    top->addChild(item);
    mTree->setItemWidget(item, 1, &mTemperature);
    mTemperature.setRange(-100, 125);
    mTemperature.setReadOnly(true);

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
    top->setText(0, kCodec->toUnicode("����"));

    item       = new QTreeWidgetItem();
    lineedit   = new QLineEdit();
    test.byte1 = lineedit;
    lineedit->setMaxLength(2);
    lineedit->setInputMask("HH");
    lineedit->setText("00");
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("���� 1"));
    mTree->setItemWidget(item, 1, lineedit);

    item       = new QTreeWidgetItem();
    lineedit   = new QLineEdit();
    test.byte2 = lineedit;
    lineedit->setMaxLength(2);
    lineedit->setInputMask("HH");
    lineedit->setText("00");
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("���� 2"));
    mTree->setItemWidget(item, 1, lineedit);

    item       = new QTreeWidgetItem();
    lineedit   = new QLineEdit();
    test.byte3 = lineedit;
    lineedit->setMaxLength(2);
    lineedit->setInputMask("HH");
    lineedit->setText("00");
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("���� 3"));
    mTree->setItemWidget(item, 1, lineedit);

    item       = new QTreeWidgetItem();
    lineedit   = new QLineEdit();
    test.byte4 = lineedit;
    lineedit->setMaxLength(2);
    lineedit->setInputMask("HH");
    lineedit->setText("00");
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("���� 4"));
    mTree->setItemWidget(item, 1, lineedit);

    item       = new QTreeWidgetItem();
    lineedit   = new QLineEdit();
    test.byte5 = lineedit;
    lineedit->setMaxLength(2);
    lineedit->setInputMask("HH");
    lineedit->setText("00");
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("���� 5"));
    mTree->setItemWidget(item, 1, lineedit);

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

    //    for (int i = 0; i < combobox->maxCount(); i++)
    //    {
    //        QVariant data = combobox->itemData(i);
    //        QString  text = combobox->itemText(i);

    //        combobox->insertItem(i, QString("%1 - %2").arg(data.toInt()).arg(text), data);
    //    }
}


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������ ���������� ��������� � �����.
 * @param[in] �������.
 * @param[in] ������.
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

        Q_ASSERT(mPkgTx.size() == 2);  // ������� + ���� ������
    }

    if (com == GB_COM_DEF_SET_LINE_TYPE)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ����� �� ������� ������ ��������� � ��������
        mPkgTx.append(com);
        mPkgTx.append(data);

        setComboBoxValue(GB_PARAM_NUM_OF_DEVICES, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������ ������� � ���������.
 * @param[in] �������.
 * @param[in] ������.
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

    // @todo �������� �������������� �� ����������� � �400�
    setSpinBoxValue(&mTemperature, data.at(0));

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

    Q_ASSERT(mPkgTx.size() == 13);  // ������� + 12 ���� ������
}


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������ �������������� � ��������������.
 * @param[in] �������.
 * @param[in] ������.
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


    Q_ASSERT(mPkgTx.size() == 21);  // ������� + 20 ���� ������
}


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������/������ ���� � �������.
 * @param[in] �������.
 * @param[in] ������.
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
        // C �� ���������� ���� ������� ��������� ��� �������� � ��� (0..3), � �� ��� ������
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

        Q_ASSERT(mPkgTx.size() == 9);  // ������� + 8 ���� ������
    }

    if (com == GB_COM_SET_TIME)
    {
        // � �� ���������� 6 ����, ��� �� / c �� ���������� 9 ����
        if (!CheckSize(com, data.size(), { 6, 9 }))
        {
            return;
        }

        // ����� �� ������� ������ ��������� � ��������
        mPkgTx.append(com);
        mPkgTx.append(data);

        // @todo �������� ��������� ����� ��������� ������������� �������
        // ��� ��������� ������� ���� ��������� ���� ���������:
        // - 0, ��������� � ����������/��
        // - 1, ��������� �� ���, ���� ��������� �������� "�����. �������"

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
 * @brief ������������ ������� ������ ���������.
 * @param[in] �������.
 * @param[in] ������.
 *
 * *****************************************************************************
 */
void TBspR400mHf_b15::HdlrComGlbx34(eGB_COM com, pkg_t &data)
{
    qint16 value = 0;

    Q_ASSERT(com == GB_COM_GET_MEAS);

    // ���������� ��� ����������� ���������
    if (!CheckSize(com, data.size(), { 1 }))
    {
        return;
    }

    // � ������� ������ 0, �.�. ������ ���� ���������
    if (data.at(0) != 0)
    {
        QString message = "Wrong data in command %1: %2";
        qWarning() << message.arg(com, 2, 16, QLatin1Char('0')).arg(data.at(0));
        return;
    }

    mPkgTx.append(com);
    mPkgTx.append(data.at(0));

    // @todo ����������� ��� ��� �� ��������� "noiseCounter_pvzu" � ��� ����� "����� ����"
    mPkgTx.append(0);  // Uart1_ArrData[5] = noiseCounter_pvzu >> 8;
    mPkgTx.append(0);  // Uart1_ArrData[5] = noiseCounter_pvzu

    value = getSpinBoxValue(m_measure.I);
    mPkgTx.append(static_cast<quint8>(value >> 8));
    mPkgTx.append(static_cast<quint8>(value));

    value = getSpinBoxValue(m_measure.U);
    mPkgTx.append(static_cast<quint8>(value / 10));
    mPkgTx.append(static_cast<quint8>((value % 10) * 10));

    // @todo ����������� ����� �� U�2, � ���� �� ���������
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Udef1)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Udef2)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Ucf1)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Ucf2)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Un1)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Un2)));

    value = getSpinBoxValue(m_measure.Sd);
    mPkgTx.append(static_cast<quint8>(value >> 8));
    mPkgTx.append(static_cast<quint8>(value));

    Q_ASSERT(mPkgTx.size() == 16);  // ������� + 15 ���� ������
}


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������/������ �������������.
 * @param[in] �������.
 * @param[in] ������.
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

        Q_ASSERT(mPkgTx.size() == 2);  // ������� + ���� ������
    }

    if (com == GB_COM_SET_COM_PRD_KEEP)
    {
        // ������������ ������ ������ ����
        if (!CheckSize(com, data.size(), { 1, 2 }))
        {
            return;
        }

        // ����� �� ������� ������ ��������� � ��������
        mPkgTx.append(com);
        mPkgTx.append(data);

        setComboBoxValue(GB_PARAM_COMP_P400, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������/������ ������ � ��������� ����.
 * @param[in] �������.
 * @param[in] ������.
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

        Q_ASSERT(mPkgTx.size() == 2);  // ������� + ���� ������
    }

    if (com == GB_COM_SET_NET_ADR)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ����� �� ������� ������ ��������� � ��������
        mPkgTx.append(com);
        mPkgTx.append(data);

        setSpinBoxValue(GB_PARAM_NET_ADDRESS, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������/������ ������ ��������
 * @param[in] �������.
 * @param[in] ������.
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

        Q_ASSERT(mPkgTx.size() == 2);  // ������� + ���� ������
    }

    if (com == GB_COM_SET_DEVICE_NUM)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ����� �� ������� ������ ��������� � ��������
        mPkgTx.append(com);
        mPkgTx.append(data);

        setSpinBoxValue(GB_PARAM_NUM_OF_DEVICE, data.at(0));
    }
}

/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������ ������ ����������
 * @param[in] �������.
 * @param[in] ������.
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
    mPkgTx.append(1);  // ������
    mPkgTx.append(0);  // ���1
    mPkgTx.append(0);  // ���2
    mPkgTx.append(0);  // ���
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
    mPkgTx.append(0);  // ������ ���-��, ������� ����
    mPkgTx.append(0);  // ������ ���-��, ������� ����

    Q_ASSERT(mPkgTx.size() == 20);  // ������� + 19 ���� ������
}
