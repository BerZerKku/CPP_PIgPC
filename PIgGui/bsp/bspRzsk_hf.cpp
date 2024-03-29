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
 * @brief �����������.
 * @param[in] tree ������ ������ ����������.
 * @param[in] parent ������������ ������.
 *
 * *****************************************************************************
 */
TBspRzskHf::TBspRzskHf(QTreeWidget *tree, QWidget *parent) : Bsp(tree, parent)
{
}


/**
 * *****************************************************************************
 *
 * @brief ����������� ����� ������.
 *
 * *****************************************************************************
 */
void TBspRzskHf::InitComMap()
{
    //    mComMap.insert(0x00, &Bsp::HdlrComDefx00);  // ������ ���������� ������ � �������������
    //    mComMap.insert(0x01, &Bsp::HdlrComDefx01);  // ������ ���� ������
    //    mComMap.insert(0x02, &Bsp::HdlrComDefx02);  // ������ ���������� ��������� � �����
    //    mComMap.insert(0x03, &Bsp::HdlrComDefx03);  // ������ ����������� ������� ��� �����������
    //                                                // 0x04 - ������� ���
    //    mComMap.insert(0x05, &Bsp::HdlrComDefx05);  // ������ ������� ��� � ���
    //    mComMap.insert(0x06, &Bsp::HdlrComDefx06);  // ������ ����������� ������.
    //    mComMap.insert(0x07, &Bsp::HdlrComDefx07);  // ������ �������� ������ ��
    //    mComMap.insert(0x08, &Bsp::HdlrComDefx08);  // ������ �������� ������� ���
    //    mComMap.insert(0x09, &Bsp::HdlrComDefx09);  // ������ �������� ������� ���
    //    mComMap.insert(0x0A, &Bsp::HdlrComDefx0A);  // ������ ������ �� � ������� �� ��

    mComMap.insert(0x30, &Bsp::HdlrComGlbx30);  // ������ �������
    //    mComMap.insert(0x31, &Bsp::HdlrComGlbx31);  // ������ ��������������
    //    mComMap.insert(0x32, &Bsp::HdlrComGlbx32);  // ������ �������
    //    mComMap.insert(0x33, &Bsp::HdlrComGlbx33);  // ������ ��������� ���� � ����������
    //    mComMap.insert(0x34, &Bsp::HdlrComGlbx34);  // ������ ���������
    //    mComMap.insert(0x35, &Bsp::HdlrComGlbx35);  // ������ ������������� ������� / ����
    //    ���������� mComMap.insert(0x36, &Bsp::HdlrComGlbx36);  // ������ ������ ���������
    //    ���������� mComMap.insert(0x37, &Bsp::HdlrComGlbx37);  // ������ �������������
    //    mComMap.insert(0x38, &Bsp::HdlrComGlbx38);  // ������ ������ � ��������� ����
    //    mComMap.insert(0x39, &Bsp::HdlrComGlbx39);  // ������ ���������� ����-�
    //    mComMap.insert(0x3A, &Bsp::HdlrComGlbx3A);  // ������ �������
    //    mComMap.insert(0x3B, &Bsp::HdlrComGlbx3B);  // ������ ������ ��������
    //    mComMap.insert(0x3C, &Bsp::HdlrComGlbx3C);  // ������ ������ ��
    //    mComMap.insert(0x3D, &Bsp::HdlrComGlbx3D);  // ������ �������� ������
    mComMap.insert(0x3f, &Bsp::HdlrComGlbx3F);  // ������ ���������� �� ����������

    //    mComMap.insert(0x70, &Bsp::HdlrComRegx70);  // ������ ������ "�������"
    //    mComMap.insert(0x71, &Bsp::HdlrComRegx71);  // ������ ������ "������"
    //    mComMap.insert(0x72, &Bsp::HdlrComRegx72);  // ������ ������� ����������

    //    mComMap.insert(0x80, &Bsp::HdlrComDefx00);  // ������ ���������� ������ � �������������
    //    mComMap.insert(0x81, &Bsp::HdlrComDefx01);  // ������ ���� ������
    //    mComMap.insert(0x82, &Bsp::HdlrComDefx02);  // ������ ���������� ��������� � �����
    //    mComMap.insert(0x83, &Bsp::HdlrComDefx03);  // ������ ����������� ������� ��� �����������
    //                                                // 0x84 - ������� ���
    //    mComMap.insert(0x85, &Bsp::HdlrComDefx05);  // ������ ������� ��� � ���
    //    mComMap.insert(0x86, &Bsp::HdlrComDefx06);  // ������ ����������� ������.
    //    mComMap.insert(0x87, &Bsp::HdlrComDefx07);  // ������ �������� ������ ��
    //    mComMap.insert(0x88, &Bsp::HdlrComDefx08);  // ������ �������� ������� ���
    //    mComMap.insert(0x89, &Bsp::HdlrComDefx09);  // ������ �������� ������� ���
    //    mComMap.insert(0x8A, &Bsp::HdlrComDefx0A);  // ������ ������ �� � ������� �� ��

    //    mComMap.insert(0xB0, &Bsp::HdlrComGlbx30);  // ������ ����� ���������� � �������������
    //    mComMap.insert(0xB2, &Bsp::HdlrComGlbx32);  // ������ �������
    //    mComMap.insert(0xB3, &Bsp::HdlrComGlbx33);  // ��������������� ���� � ����������
    //    mComMap.insert(0xB5, &Bsp::HdlrComGlbx35);  // ������ ������������� ������� / ����
    //    ���������� mComMap.insert(0xB6, &Bsp::HdlrComGlbx36);  // ������ ������ ���������
    //    ���������� mComMap.insert(0xB7, &Bsp::HdlrComGlbx37);  // ������ �������������
    //    mComMap.insert(0xB8, &Bsp::HdlrComGlbx38);  // ������ ������ � ��������� ����
    //    mComMap.insert(0xB9, &Bsp::HdlrComGlbx39);  // ������ ���������� ����-�
    //    mComMap.insert(0xBA, &Bsp::HdlrComGlbx3A);  // ������ �������
    //    mComMap.insert(0xBB, &Bsp::HdlrComGlbx3B);  // ������ ������ ��������
    //    mComMap.insert(0xBC, &Bsp::HdlrComGlbx3C);  // ������ ������ ��
    //    mComMap.insert(0xBD, &Bsp::HdlrComGlbx3D);  // ������ �������� ������

    //    mComMap.insert(0xC1, &Bsp::HdlrComJrnxC1);  // ������ ���������� ������� � ������� ������
    //    mComMap.insert(0xC2, &Bsp::HdlrComJrnxC2);  // ������ ������� ������� ������
    //    mComMap.insert(0xF1, &Bsp::HdlrComJrnxF1);  // ������ ���������� ������� � ������� �������
    //    mComMap.insert(0xF2, &Bsp::HdlrComJrnxF2);  // ������ ������� ������� �������
}


//
void TBspRzskHf::InitParam()
{
    setComboBoxValue(GB_PARAM_COMP_P400, GB_COMP_R400M_LINER);

    setSpinBoxValue(mDevice.versionBspMcu, 0x0134);
    setSpinBoxValue(mDevice.versionBspDsp, 0x0D33);
    setSpinBoxValue(mDevice.versionBszPlis, 0x52);
    setSpinBoxValue(mDevice.versionBsk1PrdPlis, 0x25);
    setSpinBoxValue(mDevice.versionBsk2PrdPlis, 0x25);
    setSpinBoxValue(mDevice.versionBsk1PrmPlis, 0x23);
    setSpinBoxValue(mDevice.versionBsk2PrmPlis, 0x23);

    setComboBoxValue(stateGlb.regime, eGB_REGIME::GB_REGIME_ENABLED);
    setComboBoxValue(stateGlb.state, 1);  // 1 - ��������
    setSpinBoxValue(stateGlb.dopByte, 1);

    setLineEditValue(stateGlb.fault, "0000");
    setLineEditValue(stateGlb.warning, "0000");
    setLineEditValue(stateDef.fault, "0000");
    setLineEditValue(stateDef.warning, "0000");

    // ��������� ����������
    setSpinBoxValue(GB_PARAM_NET_ADDRESS, 17);

    // ��������� �����
    SetParamValue(GB_PARAM_COMP_P400, GB_COMP_R400M_R400);
    SetParamValue(GB_PARAM_TIME_SYNCH_SRC, 1);  // 1 - �� ������ �����
    SetParamValue(GB_PARAM_NUM_OF_DEVICE, 1);
    SetParamValue(GB_PARAM_OUT_CHECK, 0);  // 0 - ����.
    SetParamValue(GB_PARAM_WARN_THD, 11);
    SetParamValue(GB_PARAM_U_OUT_NOM, 20);
    SetParamValue(GB_PARAM_FREQ, 123);
    SetParamValue(GB_PARAM_IN_DEC_AC_ANSWER, 7);
    SetParamValue(GB_PARAM_PVZUE_PROTOCOL, 1);  // 1 - �������
    SetParamValue(GB_PARAM_PVZUE_PARITY, 2);    // 2 - ����.
    SetParamValue(GB_PARAM_PVZUE_FAIL, 20);
    SetParamValue(GB_PARAM_PVZUE_NOISE_THD, 33);
    SetParamValue(GB_PARAM_PVZUE_NOISE_LVL, 40);
    SetParamValue(GB_PARAM_PVZUE_AC_TYPE, 2);  // 2 - ����������
    SetParamValue(GB_PARAM_PVZUE_AC_PERIOD, 152);
    SetParamValue(GB_PARAM_PVZUE_AC_PER_RE, 213);
    SetParamValue(GB_PARAM_PVZU_AC_CORRECT, 19);
    SetParamValue(GB_PARAM_ALARM_CF, 11);
    SetParamValue(GB_PARAM_COR_U, -105);  // -10.5 �
    SetParamValue(GB_PARAM_COR_I, 20);    // 20 ��

    // ��������� ������
    SetParamValue(GB_PARAM_NUM_OF_DEVICES, GB_NUM_DEVICES_2);
    SetParamValue(GB_PARAM_DEF_TYPE, 0);  // 0 - ��� ����
    SetParamValue(GB_PARAM_TIME_NO_MAN, 10);
    SetParamValue(GB_PARAM_SHIFT_FRONT, 18);
    SetParamValue(GB_PARAM_SHIFT_BACK, 36);
    SetParamValue(GB_PARAM_SHIFT_PRM, 48);
    SetParamValue(GB_PARAM_SHIFT_PRD, 64);
    SetParamValue(GB_PARAM_SENS_DEC, 6);
    SetParamValue(GB_PARAM_AC_IN_DEC, 0);  // 0 - ����
    SetParamValue(GB_PARAM_FREQ_PRD, 1);   // -500 ��
    SetParamValue(GB_PARAM_FREQ_PRM, 3);   // 500 ��

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


void TBspRzskHf::crtTreeDef(QTreeWidgetItem *top)
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("������"));
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


void TBspRzskHf::crtTreeGlb(QTreeWidgetItem(*top))
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("�����"));
    top->addChild(ltop);

    CrtParamWidget(ltop, GB_PARAM_COMP_RZSK);

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

    connect(mapCombobox.value(GB_PARAM_COMP_RZSK).at(0),
            &QComboBox::currentTextChanged,
            this,
            &TBspRzskHf::SlotChangeCompatibility);
    SlotChangeCompatibility();
}


void TBspRzskHf::crtTreeInterface(QTreeWidgetItem *top)
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("���������"));
    top->addChild(ltop);

    CrtParamWidget(ltop, GB_PARAM_NET_ADDRESS);
}


void TBspRzskHf::crtTreeDevice()
{
    QTreeWidgetItem *top  = new QTreeWidgetItem();
    QTreeWidgetItem *item = nullptr;

    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("����������"));

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("������"));
    top->addChild(item);
    mDefEnable.setEnabled(false);
    mTree->setItemWidget(item, 1, &mDefEnable);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("������� �����������"));
    mNumComPrd.setEnabled(false);
    top->addChild(item);
    mTree->setItemWidget(item, 1, &mNumComPrd);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("������� ���������"));
    mNumComPrm.setEnabled(false);
    top->addChild(item);
    mTree->setItemWidget(item, 1, &mNumComPrm);

    crtTreeDevieVersions(top);
}


void TBspRzskHf::crtTreeDevieVersions(QTreeWidgetItem *top)
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

    item                       = new QTreeWidgetItem();
    mDevice.versionBsk1PrdPlis = new QHexSpinBox(true);
    item->setText(0, kCodec->toUnicode("���1 ���� ���"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBsk1PrdPlis);

    item                       = new QTreeWidgetItem();
    mDevice.versionBsk2PrdPlis = new QHexSpinBox(true);
    item->setText(0, kCodec->toUnicode("���1 ���� ���"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBsk2PrdPlis);

    item                       = new QTreeWidgetItem();
    mDevice.versionBsk1PrmPlis = new QHexSpinBox(true);
    item->setText(0, kCodec->toUnicode("���2 ���� ���"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBsk1PrmPlis);

    item                       = new QTreeWidgetItem();
    mDevice.versionBsk2PrmPlis = new QHexSpinBox(true);
    item->setText(0, kCodec->toUnicode("���2 ���� ���"));
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


//
void TBspRzskHf::crtTreeState()
{
    QTreeWidgetItem *item;
    QTreeWidgetItem *top = new QTreeWidgetItem();

    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("������� ���������"));

    errRegExp.setPattern("[0-9A-Fa-F]+");
    errValidator.setRegExp(errRegExp);

    Bsp::crtTreeState(top, "�����", stateGlb);
    Bsp::crtTreeState(top, "������", stateDef);
    Bsp::crtTreeState(top, "�������� 1", statePrm);
    Bsp::crtTreeState(top, "�������� 2", statePrm2);
    Bsp::crtTreeState(top, "����������", statePrd);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("���������� ��� ���"));
    top->insertChild(0, item);
    mPrmLightPA.setMinimum(0x00);
    mPrmLightPA.setMaximum(0x0F);
    mPrmLightPA.setDisplayIntegerBase(16);
    mPrmLightPA.setPrefix("0x");
    mTree->setItemWidget(item, 1, &mPrmLightPA);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("���������� ��� ���"));
    top->insertChild(0, item);
    mPrdLightPA.setMinimum(0x00);
    mPrdLightPA.setMaximum(0x0F);
    mPrdLightPA.setDisplayIntegerBase(16);
    mPrdLightPA.setPrefix("0x");
    mTree->setItemWidget(item, 1, &mPrdLightPA);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("������� ����� ���"));
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
    top->setText(0, kCodec->toUnicode("����"));
}


/**
 * *****************************************************************************
 *
 * @brief ������� ����� ��������.
 *
 * *****************************************************************************
 */
void TBspRzskHf::crtJrn()
{
    QTreeWidgetItem *top = new QTreeWidgetItem(mTree);
    QTreeWidgetItem *ltop;

    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("�������"));

    ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("������ �������"));
    top->insertChild(0, ltop);
    crtJrnGlb(ltop);

    ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("������ ������"));
    top->insertChild(0, ltop);
    crtJrnDef(ltop);
}


/**
 * *****************************************************************************
 *
 * @brief ������� ������ ������.
 * @param[in] top ������� �������.
 *
 * *****************************************************************************
 */
void TBspRzskHf::crtJrnDef(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("���������� �������"));
    mTree->setItemWidget(item, 1, &mJrnDefCounter);
    mJrnDefCounter.setRange(0, 1024);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("������������"));
    mTree->setItemWidget(item, 1, &mJrnDefOverflow);

    top->setExpanded(false);
}


/**
 * *****************************************************************************
 *
 * @brief ������� ������ �������.
 * @param[in] top ������� �������.
 *
 * *****************************************************************************
 */
void TBspRzskHf::crtJrnGlb(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("���������� �������"));
    mTree->setItemWidget(item, 1, &mJrnGlbCounter);
    mJrnGlbCounter.setRange(0, 1024);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("������������"));
    mTree->setItemWidget(item, 1, &mJrnGlbOverflow);

    top->setExpanded(false);
}


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
 * @brief ���������� ������ ��������� ������ ����������.
 *
 * @todo �������� ������ ������ ���������� � ����������� �� ������� �������������.
 *
 * *****************************************************************************
 */
void TBspRzskHf::FillComboBoxListControl()
{
    mControl.addItem(kCodec->toUnicode("0 - ���"), 0);
    mControl.addItem(kCodec->toUnicode("1 - ����� ������"), 1);
    mControl.addItem(kCodec->toUnicode("2 - ����� ����������"), 2);
    mControl.addItem(kCodec->toUnicode("3 - ����� ��"), 3);
    mControl.addItem(kCodec->toUnicode("4 - ���� ���������� (1)"), 4);
    mControl.addItem(kCodec->toUnicode("5 - ���� ���������� (2)"), 5);
    mControl.addItem(kCodec->toUnicode("6 - ���� ���������"), 6);
    mControl.addItem(kCodec->toUnicode("7 - �����"), 7);
    mControl.addItem(kCodec->toUnicode("8 - ���������� ���� ���."), 8);
    mControl.addItem(kCodec->toUnicode("9 - ���������� ���� ����."), 9);
    mControl.addItem(kCodec->toUnicode("10 - ���� �� ���������"), 10);
    mControl.addItem(kCodec->toUnicode("11 - ����� ���������� (1)"), 11);
    mControl.addItem(kCodec->toUnicode("12 - ����� ���������� (2)"), 12);
    mControl.addItem(kCodec->toUnicode("13 - ���� ���������� (3)"), 13);
    mControl.addItem(kCodec->toUnicode("14 - ����� ��"), 14);
    mControl.addItem(kCodec->toUnicode("15 - ����� ���������� (3)"), 15);
    mControl.addItem(kCodec->toUnicode("16 - ���� ��. ��� (1)"), 16);
    mControl.addItem(kCodec->toUnicode("17 - ���� ��. ��� (2)"), 17);
    mControl.addItem(kCodec->toUnicode("18 - ���� ��. ��� (3)"), 18);
    mControl.addItem(kCodec->toUnicode("19 - ���� ��������� ���"), 19);
}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������/������ ���������� ������ � �������������.
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComDefx00(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == 0x00 || com == 0x80);

//    if (com == 0x00)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }

//        mPkgTx.append(com);
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_DEF_TYPE)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_NUM_OF_DEVICES)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TIME_NO_MAN)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SHIFT_FRONT)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SHIFT_BACK)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SENS_DEC)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_AC_IN_DEC)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_FREQ_PRD)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_FREQ_PRM)));
//        mPkgTx.append(getComboBoxValue(&mAc));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SHIFT_PRM)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SHIFT_PRD)));


//        int len = 32 - mPkgTx.size();
//        for (int i = 0; i < len; i++)
//        {
//            mPkgTx.append(0);
//        }

//        Q_ASSERT(mPkgTx.size() == 33);  // ������� + 31 ���� ������
//    }

//    if (com == 0x80)
//    {
//        // c �� ������-�� ���� ������ 15 ����
//        if (!CheckSize(com, data.size(), { 15, 31 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        int index = 0;
//        SetParamValue(GB_PARAM_DEF_TYPE, data.at(index++));
//        SetParamValue(GB_PARAM_NUM_OF_DEVICES, data.at(index++));
//        SetParamValue(GB_PARAM_TIME_NO_MAN, data.at(index++));
//        SetParamValue(GB_PARAM_SHIFT_FRONT, data.at(index++));
//        SetParamValue(GB_PARAM_SHIFT_BACK, data.at(index++));
//        SetParamValue(GB_PARAM_SENS_DEC, data.at(index++));
//        SetParamValue(GB_PARAM_AC_IN_DEC, data.at(index++));
//        SetParamValue(GB_PARAM_FREQ_PRD, data.at(index++));
//        SetParamValue(GB_PARAM_FREQ_PRM, data.at(index++));
//        index++;  // ����� �� �� ��������
//        SetParamValue(GB_PARAM_SHIFT_PRM, data.at(index++));
//        SetParamValue(GB_PARAM_SHIFT_PRD, data.at(index++));
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������/������ ���� ������.
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComDefx01(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_DEF_GET_DEF_TYPE || com == GB_COM_DEF_SET_DEF_TYPE);

//    if (com == GB_COM_DEF_GET_DEF_TYPE)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }

//        mPkgTx.append(com);
//        mPkgTx.append(getComboBoxValue(GB_PARAM_DEF_TYPE));

//        Q_ASSERT(mPkgTx.size() == 2);  // ������� + ���� ������
//    }

//    if (com == GB_COM_DEF_SET_DEF_TYPE)
//    {
//        if (!CheckSize(com, data.size(), { 1 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        setComboBoxValue(GB_PARAM_DEF_TYPE, data.at(0));
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������/������ ���������� ��������� � �����.
// * @param[in] �������.
// * @param[in] ������.
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

//        Q_ASSERT(mPkgTx.size() == 2);  // ������� + ���� ������
//    }

//    if (com == GB_COM_DEF_SET_LINE_TYPE)
//    {
//        if (!CheckSize(com, data.size(), { 1 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        setComboBoxValue(GB_PARAM_NUM_OF_DEVICES, data.at(0));
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������/������ ����������� ������� ��� �����������.
// * @param[in] �������.
// * @param[in] ������.
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

//        Q_ASSERT(mPkgTx.size() == 2);  // ������� + ���� ������
//    }

//    if (com == GB_COM_DEF_SET_T_NO_MAN)
//    {
//        if (!CheckSize(com, data.size(), { 1 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        SetParamValue(GB_PARAM_TIME_NO_MAN, data.at(0));
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������/������ ������� ��� � ���
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComDefx05(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_DEF_GET_OVERLAP || com == GB_COM_DEF_SET_OVERLAP);

//    if (com == GB_COM_DEF_GET_OVERLAP)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }

//        mPkgTx.append(com);
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SHIFT_FRONT)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SHIFT_BACK)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SHIFT_PRM)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SHIFT_PRD)));

//        Q_ASSERT(mPkgTx.size() == 5);  // ������� + 4 ���� ������
//    }

//    if (com == GB_COM_DEF_SET_OVERLAP)
//    {
//        if (!CheckSize(com, data.size(), { 2 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        quint8 value = data.at(0);
//        quint8 index = data.at(1);

//        switch (index)
//        {
//        case 1: SetParamValue(GB_PARAM_SHIFT_FRONT, value); break;
//        case 2: SetParamValue(GB_PARAM_SHIFT_BACK, value); break;
//        case 3: SetParamValue(GB_PARAM_SHIFT_PRM, value); break;
//        case 4: SetParamValue(GB_PARAM_SHIFT_PRD, value); break;
//        default:
//            qWarning() << QString("Wrong index (%1) in command %2")
//                              .arg(index)
//                              .arg(com, 2, 16, QLatin1Char('0'));
//        }
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������/������ ����������� ����������������
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComDefx06(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_DEF_GET_RZ_DEC || com == GB_COM_DEF_SET_RZ_DEC);

//    if (com == GB_COM_DEF_GET_RZ_DEC)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }

//        mPkgTx.append(com);
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SENS_DEC)));

//        Q_ASSERT(mPkgTx.size() == 2);  // ������� + 1 ���� ������
//    }

//    if (com == GB_COM_DEF_SET_RZ_DEC)
//    {
//        if (!CheckSize(com, data.size(), { 1 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        SetParamValue(GB_PARAM_SENS_DEC, data.at(0));
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������/������ �������� ������ ��
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComDefx07(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_DEF_GET_PRM_TYPE || com == GB_COM_DEF_SET_PRM_TYPE);

//    if (com == GB_COM_DEF_GET_PRM_TYPE)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }

//        mPkgTx.append(com);
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_AC_IN_DEC)));

//        Q_ASSERT(mPkgTx.size() == 2);  // ������� + 1 ���� ������
//    }

//    if (com == GB_COM_DEF_SET_PRM_TYPE)
//    {
//        if (!CheckSize(com, data.size(), { 1 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        SetParamValue(GB_PARAM_AC_IN_DEC, data.at(0));
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������/������ �������� ������� ���
// * @param[in] �������.
// * @param[in] ������.
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

//        Q_ASSERT(mPkgTx.size() == 2);  // ������� + 1 ���� ������
//    }

//    if (com == GB_COM_DEF_SET_FREQ_PRD)
//    {
//        if (!CheckSize(com, data.size(), { 1 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        SetParamValue(GB_PARAM_FREQ_PRD, data.at(0));
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������/������ �������� ������� ���
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComDefx09(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_DEF_GET_RZ_THRESH || com == GB_COM_DEF_SET_RZ_THRESH);

//    if (com == GB_COM_DEF_GET_RZ_THRESH)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }

//        mPkgTx.append(com);
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_FREQ_PRM)));

//        Q_ASSERT(mPkgTx.size() == 2);  // ������� + 1 ���� ������
//    }

//    if (com == GB_COM_DEF_SET_RZ_THRESH)
//    {
//        if (!CheckSize(com, data.size(), { 1 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        SetParamValue(GB_PARAM_FREQ_PRM, data.at(0));
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������/������ ������ �� � ������� �� ��
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComDefx0A(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_DEF_GET_TYPE_AC || com == GB_COM_DEF_SET_TYPE_AC);

//    if (com == GB_COM_DEF_GET_TYPE_AC)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }

//        mPkgTx.append(com);
//        mPkgTx.append(getComboBoxValue(&mAc));

//        uint32_t ms = static_cast<uint32_t>(mAcTime.value()) * 1000;

//        mPkgTx.append(static_cast<uint8_t>(ms));
//        mPkgTx.append(static_cast<uint8_t>(ms >> 8));
//        mPkgTx.append(static_cast<uint8_t>(ms >> 16));
//        mPkgTx.append(static_cast<uint8_t>(ms >> 24));

//        Q_ASSERT(mPkgTx.size() == 6);  // ������� + 5 ���� ������
//    }

//    if (com == GB_COM_DEF_SET_TYPE_AC)
//    {
//        if (!CheckSize(com, data.size(), { 1 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        setComboBoxValue(&mAc, data.at(0));
//    }
//}


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������ ������� � ���������.
 * @note ������� ������ ����������� ��� ��������� ���� ����� ���������� � �������������.
 * @param[in] �������.
 * @param[in] ������.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComGlbx30(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_SOST /*|| 0xB0*/);

    if (com == GB_COM_GET_SOST)
    {
        // � ��������� �� �� ���������� �����������, � �� ��� ������
        // � �400� ����������� �� ��������������
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

        Q_ASSERT(mPkgTx.size() == 22);  // ������� + 21 ���� ������
    }

    //    if (com == 0xB0)
    //    {
    //        if (!CheckSize(com, data.size(), { 31 }))
    //        {
    //            return;
    //        }

    //        // ����� �� ������� ������ ��������� � ��������
    //        mPkgTx.append(com);
    //        mPkgTx.append(data);

    //        int index = 0;
    //        SetParamValue(GB_PARAM_TIME_SYNCH_SRC, data.at(index++));
    //        SetParamValue(GB_PARAM_U_OUT_NOM, data.at(index++));
    //        SetParamValue(GB_PARAM_COMP_P400, data.at(index++));
    //        SetParamValue(GB_PARAM_NET_ADDRESS, data.at(index++));
    //        index++;  // ����� ����������� ���

    //        quint16 freq = data.at(index++);
    //        freq += static_cast<quint16>(data.at(index++)) << 8;
    //        SetParamValue(GB_PARAM_FREQ, freq);

    //        SetParamValue(GB_PARAM_NUM_OF_DEVICE, data.at(index++));
    //        SetParamValue(GB_PARAM_OUT_CHECK, data.at(index++));
    //        SetParamValue(GB_PARAM_WARN_THD, data.at(index++));

    //        // @fixme ����������� ��� ��� ������ ����
    //        index++;  // ����������� ���������������� �� �� ���_1
    //        index++;  // ����������� ���������������� �� �� ���_2

    //        // ��������� � �� �� ���������������
    //        index++;  // ��������� �������� ��������� ���������� (����� �����)
    //        index++;  // ��������� �������� ��������� ���������� (������� �����)
    //        index++;  // ��������� �������� ���� 1 (������� ����)
    //        index++;  // ��������� �������� ���� 1 (������� ����)
    //        index++;  // ��������� �������� ���� 2 (������� ����)
    //        index++;  // ��������� �������� ���� 2 (������� ����)

    //        SetParamValue(GB_PARAM_PVZUE_PROTOCOL, data.at(index++));
    //        SetParamValue(GB_PARAM_PVZUE_PARITY, data.at(index++));
    //        SetParamValue(GB_PARAM_PVZUE_FAIL, data.at(index++));
    //        SetParamValue(GB_PARAM_PVZUE_NOISE_THD, data.at(index++));
    //        SetParamValue(GB_PARAM_PVZUE_NOISE_LVL, data.at(index++));
    //        SetParamValue(GB_PARAM_PVZUE_AC_TYPE, data.at(index++));
    //        SetParamValue(GB_PARAM_PVZUE_AC_PERIOD, data.at(index++));
    //        SetParamValue(GB_PARAM_PVZUE_AC_PER_RE, data.at(index++));
    //        SetParamValue(GB_PARAM_PVZU_AC_CORRECT, data.at(index++));
    //        SetParamValue(GB_PARAM_ALARM_CF, data.at(index++));
    //    }
}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������ �������������� � ��������������.
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComGlbx31(eGB_COM com, pkg_t &data)
//{
//    bool    ok;
//    quint16 def_alarm = getLineEditValue(stateDef.fault).toUInt(&ok, 16);
//    quint16 def_warn  = getLineEditValue(stateDef.warning).toUInt(&ok, 16);
//    quint16 glb_alarm = getLineEditValue(stateGlb.fault).toUInt(&ok, 16);
//    quint16 glb_warn  = getLineEditValue(stateGlb.warning).toUInt(&ok, 16);

//    Q_ASSERT(com == GB_COM_GET_FAULT);

//    if (!CheckSize(com, data.size(), { 0 }))
//    {
//        return;
//    }

//    mPkgTx.append(com);
//    mPkgTx.append(static_cast<uint8_t>(def_alarm >> 8));
//    mPkgTx.append(static_cast<uint8_t>(def_alarm));
//    mPkgTx.append(static_cast<uint8_t>(def_warn >> 8));
//    mPkgTx.append(static_cast<uint8_t>(def_warn));

//    mPkgTx.append(getSpinBoxValue(&mStateFaultDeviceNumber));
//    mPkgTx.append(0);
//    mPkgTx.append(0);
//    mPkgTx.append(0);

//    mPkgTx.append(0);
//    mPkgTx.append(0);
//    mPkgTx.append(0);
//    mPkgTx.append(0);

//    mPkgTx.append(static_cast<uint8_t>(glb_alarm >> 8));
//    mPkgTx.append(static_cast<uint8_t>(glb_alarm));
//    mPkgTx.append(static_cast<uint8_t>(glb_warn >> 8));
//    mPkgTx.append(static_cast<uint8_t>(glb_warn));

//    mPkgTx.append(0);
//    mPkgTx.append(0);
//    mPkgTx.append(0);
//    mPkgTx.append(0);


//    Q_ASSERT(mPkgTx.size() == 21);  // ������� + 20 ���� ������
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������/������ ���� � �������.
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComGlbx32(eGB_COM com, pkg_t &data)
//{
//    uint16_t msec = dt.time().msec();

//    Q_ASSERT(com == GB_COM_GET_TIME || com == GB_COM_SET_TIME);

//    if (com == GB_COM_GET_TIME)
//    {
//        // C �� ���������� ���� ������� ��������� ��� �������� � ��� (0..3), � �� ��� ������
//        // � �400� ���� ������� �� ��������������
//        if (!CheckSize(com, data.size(), { 0, 1 }))
//        {
//            return;
//        }

//        mPkgTx.append(com);
//        mPkgTx.append(int2bcd(dt.date().year() - 2000));
//        mPkgTx.append(int2bcd(dt.date().month()));
//        mPkgTx.append(int2bcd(dt.date().day()));
//        mPkgTx.append(int2bcd(dt.time().hour()));
//        mPkgTx.append(int2bcd(dt.time().minute()));
//        mPkgTx.append(int2bcd(dt.time().second()));
//        mPkgTx.append(static_cast<uint8_t>(msec));
//        mPkgTx.append(static_cast<uint8_t>(msec >> 8));

//        Q_ASSERT(mPkgTx.size() == 9);  // ������� + 8 ���� ������
//    }

//    if (com == GB_COM_SET_TIME)
//    {
//        // � �� ���������� 6 ����, ��� �� / c �� ���������� 9 ����
//        if (!CheckSize(com, data.size(), { 6, 9 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        // @todo �������� ��������� ����� ��������� ������������� �������
//        // ��� ��������� ������� ���� ��������� ���� ���������:
//        // - 0, ��������� � ����������/��
//        // - 1, ��������� �� ���, ���� ��������� �������� "�����. �������"

//        quint16 year  = bcd2int(data.takeFirst()) + 2000;
//        quint8  month = bcd2int(data.takeFirst());
//        quint8  day   = bcd2int(data.takeFirst());

//        quint8 hour   = bcd2int(data.takeFirst());
//        quint8 minute = bcd2int(data.takeFirst());
//        quint8 second = bcd2int(data.takeFirst());

//        quint16 ms = 0;
//        if (data.size() >= 3)
//        {
//            ms = data.takeFirst();
//            ms += static_cast<quint16>(data.takeFirst()) << 8;

//            quint8 source = data.takeFirst();
//            if (source > 1)
//            {
//                qWarning() << kMsgTimeSourceError.arg(source);
//            }
//        }

//        dt.setDate(QDate(year, month, day));
//        dt.setTime(QTime(hour, minute, second, ms));
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������ ��������� ���� � ����������.
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComGlbx33(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_GET_COR_U_I || com == GB_COM_SET_COR_U_I);

//    if (com == GB_COM_GET_COR_U_I)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }

//        qint16 value;

//        mPkgTx.append(com);

//        value = GetParamValue(GB_PARAM_COR_U);
//        mPkgTx.append(static_cast<uint8_t>(value / 10));
//        mPkgTx.append(static_cast<uint8_t>(value % 10) * 10);

//        value = GetParamValue(GB_PARAM_COR_I);
//        mPkgTx.append(static_cast<uint8_t>(value >> 8));
//        mPkgTx.append(static_cast<uint8_t>(value));
//        mPkgTx.append(0);  // ��������� ���� 2, ������� ����
//        mPkgTx.append(0);  // ��������� ���� 2, ������� ����

//        Q_ASSERT(mPkgTx.size() == 7);  // ������� + 6 ���� ������
//    }

//    if (com == GB_COM_SET_COR_U_I)
//    {
//        if (!CheckSize(com, data.size(), { 3 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        qint16 value;

//        switch (data.at(0))
//        {
//        case 1:
//            value = static_cast<qint16>(static_cast<qint8>(data.at(1))) * 10;
//            value += static_cast<qint16>(static_cast<qint8>(data.at(2))) / 10;
//            SetParamValue(GB_PARAM_COR_U, value);
//            break;
//        case 2:
//            value = static_cast<qint16>(data.at(1)) << 8;
//            value += static_cast<qint16>(data.at(2));
//            SetParamValue(GB_PARAM_COR_I, value);
//            break;
//        case 4: SetParamValue(GB_PARAM_COR_U, 0); break;
//        case 5: SetParamValue(GB_PARAM_COR_I, 0); break;

//        default:
//            // 3 - ��������� ���� 2 ��� ������
//            // 6 - ����� ��������� ���� 2 ��� ������
//            QString message = "Wrong data in command %1: %2";
//            qWarning() << message.arg(com, 2, 16, QLatin1Char('0')).arg(data.at(0));
//        }
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������ ���������.
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComGlbx34(eGB_COM com, pkg_t &data)
//{
//    qint16 value = 0;

//    Q_ASSERT(com == GB_COM_GET_MEAS);

//    // ���������� ��� ����������� ���������
//    if (!CheckSize(com, data.size(), { 1 }))
//    {
//        return;
//    }

//    // � ������� ������ 0, �.�. ������ ���� ���������
//    if (data.at(0) != 0)
//    {
//        QString message = "Wrong data in command %1: %2";
//        qWarning() << message.arg(com, 2, 16, QLatin1Char('0')).arg(data.at(0));
//        return;
//    }

//    mPkgTx.append(com);
//    mPkgTx.append(data.at(0));

//    mPkgTx.append(0);
//    mPkgTx.append(0);

//    value = getSpinBoxValue(m_measure.I);
//    value += GetParamValue(GB_PARAM_COR_I);

//    mPkgTx.append(static_cast<quint8>(value >> 8));
//    mPkgTx.append(static_cast<quint8>(value));

//    value = getSpinBoxValue(m_measure.U);
//    value += GetParamValue(GB_PARAM_COR_U);
//    mPkgTx.append(static_cast<quint8>(value / 10));
//    mPkgTx.append(static_cast<quint8>((value % 10) * 10));

//    // @todo ����������� ����� �� U�2, � ���� �� ���������
//    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Udef1)));
//    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Udef2)));
//    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Ucf1)));
//    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Ucf2)));
//    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Un1)));
//    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Un2)));

//    value = getSpinBoxValue(m_measure.Sd);
//    mPkgTx.append(static_cast<quint8>(value >> 8));
//    mPkgTx.append(static_cast<quint8>(value));

//    Q_ASSERT(mPkgTx.size() == 16);  // ������� + 15 ���� ������
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������/������ ������������� �������.
// * @note ��� �� ������� ������������ ��� ������/������ ���� ���������� ��������� � �������������
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComGlbx35(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_GET_TIME_SINCHR || com == GB_COM_SET_TIME_SINCHR);

//    if (com == GB_COM_GET_TIME_SINCHR)
//    {
//        // ��� ���������� ���� ���������� ���������� ���� ���� ������
//        // ��� ���������� ������������� ������� ������ ���
//        if (!CheckSize(com, data.size(), { 0, 1 }))
//        {
//            qDebug() << Qt::hex << com << ", " << data;
//            return;
//        }

//        if (data.size() == 1)
//        {
//            mPkgTx.append(com);
//            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TIME_SYNCH_SRC)));
//            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_U_OUT_NOM)));
//            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_COMP_P400)));
//            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_NET_ADDRESS)));
//            mPkgTx.append(0);  // ����� ����������� ���, ���

//            quint16 freq = GetParamValue(GB_PARAM_FREQ);
//            mPkgTx.append(static_cast<uint8_t>(freq));
//            mPkgTx.append(static_cast<uint8_t>(freq >> 8));

//            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_NUM_OF_DEVICE)));
//            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_OUT_CHECK)));  // 0 - ����.
//            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_WARN_THD)));

//            mPkgTx.append(0);  // ����������� ���������������� �� �� ���_1
//            mPkgTx.append(0);  // ����������� ���������������� �� �� ���_2

//            qint16 corr_u = GetParamValue(GB_PARAM_COR_U);
//            mPkgTx.append(corr_u / 10);
//            mPkgTx.append((corr_u % 10) * 10);

//            qint16 corr_i = GetParamValue(GB_PARAM_COR_I);
//            mPkgTx.append(static_cast<uint8_t>(corr_i));
//            mPkgTx.append(static_cast<uint8_t>(corr_i >> 8));
//            mPkgTx.append(0);  // ��������� �������� ���� 2, ������� ����
//            mPkgTx.append(0);  // ��������� �������� ���� 2, ������� ����

//            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_PROTOCOL)));
//            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_PARITY)));
//            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_FAIL)));
//            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_NOISE_THD)));
//            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_NOISE_LVL)));
//            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_AC_TYPE)));
//            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_AC_PERIOD)));
//            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_AC_PER_RE)));
//            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZU_AC_CORRECT)));
//            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_ALARM_CF)));

//            int len = 32 - mPkgTx.size();
//            for (int i = 0; i < len; i++)
//            {
//                mPkgTx.append(0);
//            }

//            Q_ASSERT(mPkgTx.size() == 32);  // ������� + 31 ���� ������
//        }
//        else
//        {
//            mPkgTx.append(com);
//            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TIME_SYNCH_SRC)));
//        }
//    }

//    if (com == GB_COM_SET_TIME_SINCHR)
//    {
//        if (!CheckSize(com, data.size(), { 1, 2 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        SetParamValue(GB_PARAM_TIME_SYNCH_SRC, data.at(0));
//    }
//}

///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������/������ ������ ��������� ����������.
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComGlbx36(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_GET_COM_PRM_KEEP || com == GB_COM_SET_COM_PRM_KEEP);

//    if (com == GB_COM_GET_COM_PRM_KEEP)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }

//        mPkgTx.append(com);
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_U_OUT_NOM)));

//        Q_ASSERT(mPkgTx.size() == 2);  // ������� + ���� ������
//    }

//    if (com == GB_COM_SET_COM_PRM_KEEP)
//    {
//        // ������������ ������ ������ ����
//        if (!CheckSize(com, data.size(), { 1 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        SetParamValue(GB_PARAM_U_OUT_NOM, data.at(0));
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������/������ �������������.
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComGlbx37(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_GET_COM_PRD_KEEP || com == GB_COM_SET_COM_PRD_KEEP);

//    if (com == GB_COM_GET_COM_PRD_KEEP)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }

//        mPkgTx.append(com);
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_COMP_P400)));

//        Q_ASSERT(mPkgTx.size() == 2);  // ������� + ���� ������
//    }

//    if (com == GB_COM_SET_COM_PRD_KEEP)
//    {
//        // ������������ ������ ������ ����
//        if (!CheckSize(com, data.size(), { 1, 2 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        SetParamValue(GB_PARAM_COMP_P400, data.at(0));
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������/������ ������ � ��������� ����.
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComGlbx38(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_GET_NET_ADR || com == GB_COM_SET_NET_ADR);

//    if (com == GB_COM_GET_NET_ADR)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }

//        mPkgTx.append(com);
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_NET_ADDRESS)));

//        Q_ASSERT(mPkgTx.size() == 2);  // ������� + ���� ������
//    }

//    if (com == GB_COM_SET_NET_ADR)
//    {
//        if (!CheckSize(com, data.size(), { 1 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        SetParamValue(GB_PARAM_NET_ADDRESS, data.at(0));
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������/������ ���������� ����-�
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComGlbx39(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_GET_TIME_RERUN || com == GB_COM_SET_TIME_RERUN);

//    if (com == GB_COM_GET_TIME_RERUN)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }

//        mPkgTx.append(com);
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_PROTOCOL)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_PARITY)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_FAIL)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_NOISE_THD)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_NOISE_LVL)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_AC_TYPE)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_AC_PERIOD)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZUE_AC_PER_RE)));
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PVZU_AC_CORRECT)));

//        Q_ASSERT(mPkgTx.size() == 10);  // ������� + 9 ���� ������
//    }

//    if (com == GB_COM_SET_TIME_RERUN)
//    {
//        if (!CheckSize(com, data.size(), { 2 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        switch (data.at(0))
//        {
//        case 1: SetParamValue(GB_PARAM_PVZUE_PROTOCOL, data.at(1)); break;
//        case 2: SetParamValue(GB_PARAM_PVZUE_PARITY, data.at(1)); break;
//        case 3: SetParamValue(GB_PARAM_PVZUE_FAIL, data.at(1)); break;
//        case 4: SetParamValue(GB_PARAM_PVZUE_NOISE_THD, data.at(1)); break;
//        case 5: SetParamValue(GB_PARAM_PVZUE_NOISE_LVL, data.at(1)); break;
//        case 6: SetParamValue(GB_PARAM_PVZUE_AC_TYPE, data.at(1)); break;
//        case 7: SetParamValue(GB_PARAM_PVZUE_AC_PERIOD, data.at(1)); break;
//        case 8: SetParamValue(GB_PARAM_PVZUE_AC_PER_RE, data.at(1)); break;
//        case 9: SetParamValue(GB_PARAM_PVZU_AC_CORRECT, data.at(1)); break;
//        default:
//            {
//                QString message("Wrong value in command %1: %2");
//                qWarning() << message.arg(com, 2, 16, QLatin1Char('0')).arg(data.at(0));
//            }
//        }
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������/������ �������.
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComGlbx3A(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_GET_FREQ || com == GB_COM_SET_FREQ);

//    quint16 freq;

//    if (com == GB_COM_GET_FREQ)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }


//        mPkgTx.append(com);
//        freq = GetParamValue(GB_PARAM_FREQ);
//        mPkgTx.append(static_cast<uint8_t>(freq >> 8));
//        mPkgTx.append(static_cast<uint8_t>(freq));

//        Q_ASSERT(mPkgTx.size() == 3);  // ������� + 2 ����� ������
//    }

//    if (com == GB_COM_SET_FREQ)
//    {
//        if (!CheckSize(com, data.size(), { 2 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        freq = (static_cast<quint16>(data.at(0)) << 8) + data.at(1);
//        SetParamValue(GB_PARAM_FREQ, freq);
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������/������ ������ ��������.
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComGlbx3B(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_GET_DEVICE_NUM || com == GB_COM_SET_DEVICE_NUM);

//    if (com == GB_COM_GET_DEVICE_NUM)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }

//        mPkgTx.append(com);
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_NUM_OF_DEVICE)));

//        Q_ASSERT(mPkgTx.size() == 2);  // ������� + ���� ������
//    }

//    if (com == GB_COM_SET_DEVICE_NUM)
//    {
//        if (!CheckSize(com, data.size(), { 1 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        SetParamValue(GB_PARAM_NUM_OF_DEVICE, data.at(0));
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������ ������ ��.
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComGlbx3C(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_GET_CF_THRESHOLD || com == GB_COM_SET_CF_THRESHOLD);

//    if (com == GB_COM_GET_CF_THRESHOLD)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }

//        mPkgTx.append(com);
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_WARN_THD)));
//        mPkgTx.append(0);  // ���, ����������� ���������������� �� �� ���_1
//        mPkgTx.append(0);  // ���, ����������� ���������������� �� �� ���_2
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_ALARM_CF)));

//        Q_ASSERT(mPkgTx.size() == 5);  // ������� + 4 ����� ������
//    }

//    if (com == GB_COM_SET_CF_THRESHOLD)
//    {
//        if (!CheckSize(com, data.size(), { 2 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        switch (data.at(0))
//        {
//        case 1: SetParamValue(GB_PARAM_WARN_THD, data.at(1)); break;
//        case 4: SetParamValue(GB_PARAM_ALARM_CF, data.at(1)); break;
//        default:
//            {
//                QString message("Wrong value in command %1: %2");
//                qWarning() << message.arg(com, 2, 16, QLatin1Char('0')).arg(data.at(0));
//            }
//        }
//    }
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������ �������� ���������� ������.
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComGlbx3D(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_GET_OUT_CHECK || com == GB_COM_SET_OUT_CHECK);

//    if (com == GB_COM_GET_OUT_CHECK)
//    {
//        if (!CheckSize(com, data.size(), { 0 }))
//        {
//            return;
//        }

//        mPkgTx.append(com);
//        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_OUT_CHECK)));

//        Q_ASSERT(mPkgTx.size() == 2);  // ������� + ���� ������
//    }

//    if (com == GB_COM_SET_OUT_CHECK)
//    {
//        if (!CheckSize(com, data.size(), { 1 }))
//        {
//            return;
//        }

//        // ����� �� ������� ������ ��������� � ��������
//        mPkgTx.append(com);
//        mPkgTx.append(data);

//        SetParamValue(GB_PARAM_OUT_CHECK, data.at(0));
//    }
//}

/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������ ������ ����������.
 * @param[in] �������.
 * @param[in] ������.
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
    mPkgTx.append(mDefEnable.isChecked() ? 1 : 0);                         // ������
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(&mNumComPrm) / 4));  // ���1
    mPkgTx.append(0);                                                      // ���2
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(&mNumComPrd) / 4));  // ���
    mPkgTx.append(GetParamValue(GB_PARAM_NUM_OF_DEVICES) + 1);
    mPkgTx.append(GB_TYPE_LINE_UM);
    mPkgTx.append(static_cast<quint8>(vers_bsp_mcu >> 8));
    mPkgTx.append(static_cast<quint8>(vers_bsp_mcu));
    mPkgTx.append(static_cast<quint8>(vers_bsp_dsp >> 8));
    mPkgTx.append(static_cast<quint8>(vers_bsp_dsp));
    mPkgTx.append(GetParamValue(GB_PARAM_COMP_P400));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(mDevice.versionBsk1PrdPlis)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(mDevice.versionBsk2PrdPlis)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(mDevice.versionBsk1PrmPlis)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(mDevice.versionBsk2PrmPlis)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(mDevice.versionBszPlis)));
    mPkgTx.append(AVANT_RZSK);

    Q_ASSERT(mPkgTx.size() == 18);  // ������� + 17 ���� ������
}


/**
 * *****************************************************************************
 *
 * @brief ���������� ��������� �������������.
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
        mNumComPrd.setValue(4);
        mNumComPrm.setValue(4);
        break;
    case GB_COMP_RZSK_3E8:
        mDefEnable.setChecked(false);
        mNumComPrd.setValue(8);
        mNumComPrm.setValue(8);
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
// * @brief ������������ ������� ��������� ������ "�������".
// * @param[in] �������.
// * @param[in] ������.
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

//    // ����� �� ������� ������ ��������� � ��������
//    mPkgTx.append(com);
//    mPkgTx.append(data);

//    setComboBoxValue(stateGlb.regime, eGB_REGIME::GB_REGIME_DISABLED);
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ��������� ������ "������".
// * @param[in] �������.
// * @param[in] ������.
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

//    // ����� �� ������� ������ ��������� � ��������
//    mPkgTx.append(com);
//    mPkgTx.append(data);

//    setComboBoxValue(stateGlb.regime, eGB_REGIME::GB_REGIME_ENABLED);
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ����������.
// * @param[in] �������.
// * @param[in] ������.
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

//    // ����� �� ������� ������ ��������� � ��������
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
//        setComboBoxValue(stateDef.state, 7);  // ���� �����. ���
//    }
//    else if (control == 9)
//    {
//        setComboBoxValue(stateDef.state, 1);  // ���� �����. ����
//    }

//    mControl.setCurrentIndex(mControl.findData(control));
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������ ���������� ������� � ������� ������.
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComJrnxC1(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_DEF_GET_JRN_CNT);

//    if (!CheckSize(com, data.size(), { 0 }))
//    {
//        return;
//    }

//    uint16_t len = static_cast<uint16_t>(mJrnDefCounter.value());

//    if (mJrnDefOverflow.isChecked())
//    {
//        len |= 0x8000;
//    }

//    mPkgTx.append(com);
//    mPkgTx.append(uint8_t(len));
//    mPkgTx.append(uint8_t(len >> 8));

//    Q_ASSERT(mPkgTx.size() == 3);  // ������� + 2 ����� ������
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������ ������� ������� ������.
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComJrnxC2(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_DEF_GET_JRN_ENTRY);

//    if (!CheckSize(com, data.size(), { 2 }))
//    {
//        return;
//    }

//    int event_number = data.takeFirst();
//    event_number += static_cast<int>(data.takeFirst());

//    mPkgTx.append(com);
//    mPkgTx.append(GB_DEVICE_K400_DEF);          // ����������
//    mPkgTx.append(event_number);                // zzzz z[���][����][����]
//    mPkgTx.append(event_number % 17);           // ��������� ������
//    mPkgTx.append(event_number);                // zzzz z[���][���][���]
//    mPkgTx.append(0);                           // b5
//    mPkgTx.append(0);                           // b6
//    mPkgTx.append(0);                           // b7
//    mPkgTx.append(999 & 0xFF);                  // ������������, ������� ����
//    mPkgTx.append((999 >> 8) & 0xFF);           // ������������, ������� ����
//    mPkgTx.append(int2bcd(event_number % 60));  // �������, bcd
//    mPkgTx.append(int2bcd(event_number / 60));  // ������, bcd
//    mPkgTx.append(0x23);                        // ����, bcd
//    mPkgTx.append(0);                           // ���� ������
//    mPkgTx.append(0x01);                        // ����, bcd
//    mPkgTx.append(0x02);                        // �����, bcd
//    mPkgTx.append(0x21);                        // ���, bcd

//    Q_ASSERT(mPkgTx.size() == 17);  // ������� + 16 ���� ������
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������ ���������� ������� � ������� �������.
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComJrnxF1(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_GET_JRN_CNT);

//    if (!CheckSize(com, data.size(), { 0 }))
//    {
//        return;
//    }

//    // @todo �������� ����������� �����������
//    uint16_t len = static_cast<uint16_t>(mJrnGlbCounter.value());

//    if (mJrnGlbOverflow.isChecked())
//    {
//        len |= 0x8000;
//    }

//    mPkgTx.append(com);
//    mPkgTx.append(uint8_t(len));
//    mPkgTx.append(uint8_t(len >> 8));

//    Q_ASSERT(mPkgTx.size() == 3);  // ������� + 2 ����� ������
//}


///**
// * *****************************************************************************
// *
// * @brief ������������ ������� ������ ������� ������� �������.
// * @param[in] �������.
// * @param[in] ������.
// *
// * *****************************************************************************
// */
// void TBspRzskHf::HdlrComJrnxF2(eGB_COM com, pkg_t &data)
//{
//    Q_ASSERT(com == GB_COM_GET_JRN_ENTRY);

//    if (!CheckSize(com, data.size(), { 2 }))
//    {
//        return;
//    }

//    int event_number = data.takeFirst();
//    event_number += static_cast<int>(data.takeFirst());

//    mPkgTx.append(com);
//    mPkgTx.append(event_number % 10);  // ��������� ������� ��� ����, �����
//    mPkgTx.append(event_number % (JRN_EVENT_R400M_SIZE + 1));  // �������
//    mPkgTx.append(event_number % 8);                           // �����
//    mPkgTx.append(0);                                          // b4
//    mPkgTx.append(0);                                          // b5
//    mPkgTx.append(0);                                          // b6
//    mPkgTx.append(0);                                          // b7
//    mPkgTx.append(999 & 0xFF);                  // ������������, ������� ����
//    mPkgTx.append((999 >> 8) & 0xFF);           // ������������, ������� ����
//    mPkgTx.append(int2bcd(event_number % 60));  // �������, bcd
//    mPkgTx.append(int2bcd(event_number / 60));  // ������, bcd
//    mPkgTx.append(0x23);                        // ����, bcd
//    mPkgTx.append(0);                           // ���� ������
//    mPkgTx.append(0x01);                        // ����, bcd
//    mPkgTx.append(0x02);                        // �����, bcd
//    mPkgTx.append(0x21);                        // ���, bcd

//    Q_ASSERT(mPkgTx.size() == 17);  // ������� + 16 ���� ������
//}
