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
    mComMap.insert(0x00, &Bsp::HdlrComDefx00);  // ������ ���������� ������ � �������������
    mComMap.insert(0x01, &Bsp::HdlrComDefx01);  // ������ ���� ������
    mComMap.insert(0x02, &Bsp::HdlrComDefx02);  // ������ ���������� ��������� � �����
    mComMap.insert(0x03, &Bsp::HdlrComDefx03);  // ������ ����������� ������� ��� �����������
    mComMap.insert(0x04, &Bsp::HdlrComDefx04);  // ������ ����������� �������� �� �����
    mComMap.insert(0x05, &Bsp::HdlrComDefx05);  // ������ ��������� ���������
    mComMap.insert(0x06, &Bsp::HdlrComDefx06);  // ������ ����������� ������.
    mComMap.insert(0x07, &Bsp::HdlrComDefx07);  // ������ ���������� ������������
    mComMap.insert(0x09, &Bsp::HdlrComDefx09);  // ������ ������ �������������� �� ��
    mComMap.insert(0x0A, &Bsp::HdlrComDefx0A);  // ������ �������������� ������

    mComMap.insert(0x30, &Bsp::HdlrComGlbx30);  // ������ �������
    mComMap.insert(0x31, &Bsp::HdlrComGlbx31);  // ������ ��������������
    mComMap.insert(0x32, &Bsp::HdlrComGlbx32);  // ������ �������
    mComMap.insert(0x33, &Bsp::HdlrComGlbx33);  // ������ ��������� ���� � ����������
    mComMap.insert(0x34, &Bsp::HdlrComGlbx34);  // ������ ���������
    mComMap.insert(0x35, &Bsp::HdlrComGlbx35);  // ������ �����. ������� / ���� ����������
    mComMap.insert(0x36, &Bsp::HdlrComGlbx36);  // ������ ��������� ���� ���.
    mComMap.insert(0x37, &Bsp::HdlrComGlbx37);  // ������ �����. ���� ��� � �������.
    mComMap.insert(0x38, &Bsp::HdlrComGlbx38);  // ������ ������ � ��������� ����
    mComMap.insert(0x39, &Bsp::HdlrComGlbx39);  // ������ ������� �����������
    mComMap.insert(0x3A, &Bsp::HdlrComGlbx3A);  // ������ �������
    mComMap.insert(0x3B, &Bsp::HdlrComGlbx3B);  // ������ ������ ��������
    mComMap.insert(0x3C, &Bsp::HdlrComGlbx3C);  // ������ ������ �������. �� � ����. ������.
    mComMap.insert(0x3D, &Bsp::HdlrComGlbx3D);  // ������ �������� ������
    mComMap.insert(0x3E, &Bsp::HdlrComGlbx3E);  // ������ �������� � �����
    mComMap.insert(0x3f, &Bsp::HdlrComGlbx3F);  // ������ ���������� �� ����������

    mComMap.insert(0x70, &Bsp::HdlrComRegx70);  // ������ ������ "�������"
    mComMap.insert(0x71, &Bsp::HdlrComRegx71);  // ������ ������ "������"
    //    mComMap.insert(0x72, &Bsp::HdlrComRegx72);  // ������ ������� ����������
    mComMap.insert(0x7D, &Bsp::HdlrComRegx7D);  // ������ ������ "���� 1 (���)"
    mComMap.insert(0x7E, &Bsp::HdlrComRegx7E);  // ������ ������ "���� 1 (���)"

    mComMap.insert(0x80, &Bsp::HdlrComDefx00);  // ������ ���������� ������ � �������������
    mComMap.insert(0x81, &Bsp::HdlrComDefx01);  // ������ ���� ������
    mComMap.insert(0x82, &Bsp::HdlrComDefx02);  // ������ ���������� ��������� � �����
    mComMap.insert(0x83, &Bsp::HdlrComDefx03);  // ������ ����������� ������� ��� �����������
    mComMap.insert(0x84, &Bsp::HdlrComDefx04);  // ������ ����������� �������� �� �����
    mComMap.insert(0x85, &Bsp::HdlrComDefx05);  // ������ ���������� ���������
    mComMap.insert(0x86, &Bsp::HdlrComDefx06);  // ������ ����������� ������.
    mComMap.insert(0x87, &Bsp::HdlrComDefx07);  // ������ ���������� ������������
    mComMap.insert(0x89, &Bsp::HdlrComDefx09);  // ������ ������ �������������� �� ��
    mComMap.insert(0x8A, &Bsp::HdlrComDefx0A);  // ������ �������������� ������

    mComMap.insert(0xB0, &Bsp::HdlrComGlbx30);  // ������ ����� ���������� � �������������
    mComMap.insert(0xB2, &Bsp::HdlrComGlbx32);  // ������ �������
    mComMap.insert(0xB3, &Bsp::HdlrComGlbx33);  // ������ ��������� ���� � ����������
    mComMap.insert(0xB5, &Bsp::HdlrComGlbx35);  // ������ ������������� ������� / ���� ����������
    mComMap.insert(0xB6, &Bsp::HdlrComGlbx36);  // ������ ��������� ���� ���
    mComMap.insert(0xB7, &Bsp::HdlrComGlbx37);  // ������ �����. ���� ��� � �������.
    mComMap.insert(0xB8, &Bsp::HdlrComGlbx38);  // ������ ������ � ��������� ����
    mComMap.insert(0xB9, &Bsp::HdlrComGlbx39);  // ������ ������� �����������
    mComMap.insert(0xBA, &Bsp::HdlrComGlbx3A);  // ������ �������
    mComMap.insert(0xBB, &Bsp::HdlrComGlbx3B);  // ������ ������ ��������
    mComMap.insert(0xBC, &Bsp::HdlrComGlbx3C);  // ������ ������ �������. �� � ����. ������.
    mComMap.insert(0xBD, &Bsp::HdlrComGlbx3D);  // ������ �������� ������

    mComMap.insert(0xC1, &Bsp::HdlrComJrnxC1);  // ������ ���������� ������� � ������� ������
    mComMap.insert(0xC2, &Bsp::HdlrComJrnxC2);  // ������ ������� ������� ������
    mComMap.insert(0xD1, &Bsp::HdlrComJrnxD1);  // ������ ���������� ������� � ������� ���
    mComMap.insert(0xD2, &Bsp::HdlrComJrnxD2);  // ������ ������� ������� ���
    mComMap.insert(0xE1, &Bsp::HdlrComJrnxE1);  // ������ ���������� ������� � ������� ���
    mComMap.insert(0xE2, &Bsp::HdlrComJrnxE2);  // ������ ������� ������� ���
    mComMap.insert(0xF1, &Bsp::HdlrComJrnxF1);  // ������ ���������� ������� � ������� �������
    mComMap.insert(0xF2, &Bsp::HdlrComJrnxF2);  // ������ ������� ������� �������
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
    setComboBoxValue(stateGlb.state, 1);  // 1 - ��������
    setSpinBoxValue(stateGlb.dopByte, 1);

    setLineEditValue(stateGlb.fault, "0000");
    setLineEditValue(stateGlb.warning, "0000");
    setLineEditValue(stateDef.fault, "0000");
    setLineEditValue(stateDef.warning, "0000");

    // ��������� ����������
    setSpinBoxValue(GB_PARAM_NET_ADDRESS, 17);

    // ��������� �����
    SetParamValue(GB_PARAM_COMP_RZSK, GB_COMP_RZSK);
    //    SetParamValue(GB_PARAM_NUM_OF_DEVICES, 1); - auto change
    SetParamValue(GB_PARAM_TIME_SYNCH, 1);  // 1 - ���.
    SetParamValue(GB_PARAM_NUM_OF_DEVICE, 1);
    SetParamValue(GB_PARAM_OUT_CHECK, 0);  // 0 - ����.
    SetParamValue(GB_PARAM_WARN_THD_CF, 11);
    SetParamValue(GB_PARAM_TIME_RERUN, 3);
    SetParamValue(GB_PARAM_COM_PRD_KEEP, 0);  // 0 - ����.
    SetParamValue(GB_PARAM_COM_PRM_KEEP, 1);  // 1 - ���.
    SetParamValue(GB_PARAM_IN_DEC, 12, 1);
    SetParamValue(GB_PARAM_IN_DEC, 13, 2);
    SetParamValue(GB_PARAM_FREQ, 123);
    SetParamValue(GB_PARAM_DETECTOR, 2);
    SetParamValue(GB_PARAM_COR_U, -105);  // -10.5 �
    SetParamValue(GB_PARAM_COR_I, 20);    // 20 ��

    // ��������� ������
    SetParamValue(GB_PARAM_DEF_TYPE, 0);  // 0 - ��� ����
    SetParamValue(GB_PARAM_TIME_NO_MAN, 10);
    SetParamValue(GB_PARAM_OVERLAP, 28);
    SetParamValue(GB_PARAM_DELAY, 8);
    SetParamValue(GB_PARAM_WARN_THD_RZ, 13);
    SetParamValue(GB_PARAM_SENS_DEC_RZ, 31);
    SetParamValue(GB_PARAM_PRM_TYPE, 0);      // 0 - ���+����
    SetParamValue(GB_PARAM_DEF_ONE_SIDE, 0);  // 0 - ����.

    // ��������� �����������
    SetParamValue(GB_PARAM_PRD_IN_DELAY, 7);
    SetParamValue(GB_PARAM_PRD_DURATION_L, 50);
    SetParamValue(GB_PARAM_PRD_COM_LONG, 0x0F, 1);   // ����, 1 - ���.
    SetParamValue(GB_PARAM_PRD_COM_BLOCK, 0xF0, 1);  // ����, 1 - ���.

    // ��������� ���������
    SetParamValue(GB_PARAM_PRM_TIME_ON, 8);
    SetParamValue(GB_PARAM_PRM_COM_BLOCK, 0x07, 1);  // ����, 1 - ���.
    SetParamValue(GB_PARAM_PRM_TIME_OFF, 250 / 10, 1);
    SetParamValue(GB_PARAM_PRM_TIME_OFF, 500 / 10, 2);
    SetParamValue(GB_PARAM_PRM_INC_SAFETY, 1);  // 1 - ���.

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
    ltop->setText(0, kCodec->toUnicode("������"));
    top->addChild(ltop);

    CrtParamWidget(ltop, GB_PARAM_DEF_TYPE);
    CrtParamWidget(ltop, GB_PARAM_TIME_NO_MAN);
    CrtParamWidget(ltop, GB_PARAM_OVERLAP);
    CrtParamWidget(ltop, GB_PARAM_DELAY);
    CrtParamWidget(ltop, GB_PARAM_WARN_THD_RZ);
    CrtParamWidget(ltop, GB_PARAM_SENS_DEC_RZ);
    CrtParamWidget(ltop, GB_PARAM_PRM_TYPE);

    // �������� ������������� ����� �������� �������� ����������
    CrtParamWidget(ltop, GB_PARAM_DEF_ONE_SIDE);
}


//
void TBspRzskHf::crtTreePrd(QTreeWidgetItem *top)
{
    QTreeWidgetItem *ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("����������"));
    top->addChild(ltop);

    // �������� �������� ������������� ��� ����� �������������
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
    ltop->setText(0, kCodec->toUnicode("��������"));
    top->addChild(ltop);

    // �������� �������� ������������� ��� ����� �������������
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
    ltop->setText(0, kCodec->toUnicode("�����"));
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

    item = new QTreeWidgetItem(top);
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("������� ��1"));
    mTree->setItemWidget(item, 1, &mTestCf1Signal);

    item = new QTreeWidgetItem(top);
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("������� ��1"));
    mTree->setItemWidget(item, 1, &mTestRz1Signal);

    item = new QTreeWidgetItem(top);
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("������� ��2"));
    mTree->setItemWidget(item, 1, &mTestCf2Signal);

    item = new QTreeWidgetItem(top);
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("������� ��2"));
    mTree->setItemWidget(item, 1, &mTestRz2Signal);
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

    ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("������ ���������"));
    top->insertChild(0, ltop);
    crtJrnPrm(ltop);

    ltop = new QTreeWidgetItem(top);
    ltop->setText(0, kCodec->toUnicode("������ �����������"));
    top->insertChild(0, ltop);
    crtJrnPrd(ltop);
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
    mJrnDefCounter.setRange(0, 256);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("������������"));
    mTree->setItemWidget(item, 1, &mJrnDefOverflow);
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
    mJrnGlbCounter.setRange(0, 256);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("������������"));
    mTree->setItemWidget(item, 1, &mJrnGlbOverflow);
}


/**
 * *****************************************************************************
 *
 * @brief ������� ������ ���������.
 * @param[in] top ������� �������.
 *
 * *****************************************************************************
 */
void TBspRzskHf::crtJrnPrm(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("���������� �������"));
    mTree->setItemWidget(item, 1, &mJrnPrmCounter);
    mJrnPrmCounter.setRange(0, 256);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("������������"));
    mTree->setItemWidget(item, 1, &mJrnPrmOverflow);
}


/**
 * *****************************************************************************
 *
 * @brief ������� ������ �����������.
 * @param[in] top ������� �������.
 *
 * *****************************************************************************
 */
void TBspRzskHf::crtJrnPrd(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("���������� �������"));
    mTree->setItemWidget(item, 1, &mJrnPrdCounter);
    mJrnPrdCounter.setRange(0, 256);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("������������"));
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


/**
 * *****************************************************************************
 *
 * @brief ���������� �������� ��� ������.
 * @param[in] �������.
 * @param[in] ������.
 *
 * *****************************************************************************
 */
void TBspRzskHf::FillComboboxListTest()
{
    mTestCf1Signal.addItem(kCodec->toUnicode("���"), 0);
    mTestCf1Signal.addItem(kCodec->toUnicode("��1"), 1);
    mTestCf1Signal.addItem(kCodec->toUnicode("��2"), 2);
    mTestCf1Signal.addItem(kCodec->toUnicode("������� 1"), 3);
    mTestCf1Signal.addItem(kCodec->toUnicode("������� 2"), 4);
    mTestCf1Signal.addItem(kCodec->toUnicode("������� 3"), 5);
    mTestCf1Signal.addItem(kCodec->toUnicode("������� 4"), 6);
    mTestCf1Signal.addItem(kCodec->toUnicode("������� 5"), 7);
    mTestCf1Signal.addItem(kCodec->toUnicode("������� 6"), 8);
    mTestCf1Signal.addItem(kCodec->toUnicode("������� 7"), 9);
    mTestCf1Signal.addItem(kCodec->toUnicode("������� 8"), 10);

    for (int i = 0; i < mTestCf1Signal.count(); i++)
    {
        mTestCf2Signal.addItem(mTestCf1Signal.itemText(i), mTestCf1Signal.itemData(i));
    }

    mTestRz1Signal.addItem(kCodec->toUnicode("���"), 0);
    mTestRz1Signal.addItem(kCodec->toUnicode("��1"), 1);
    mTestRz1Signal.addItem(kCodec->toUnicode("��2"), 2);

    for (int i = 0; i < mTestRz1Signal.count(); i++)
    {
        mTestRz2Signal.addItem(mTestRz1Signal.itemText(i), mTestRz1Signal.itemData(i));
    }
}


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������/������ ���������� ������ � �������������.
 * @param[in] �������.
 * @param[in] ������.
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
        mPkgTx.append(0);  // ��� �����
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TIME_NO_MAN)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_DELAY)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_OVERLAP)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_SENS_DEC_RZ)));
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_PRM_TYPE)));
        mPkgTx.append(0);  // �������� �� ����� 2
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_WARN_THD_RZ)));
        mPkgTx.append(0);  // ����������� ��� 2
        mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_DEF_ONE_SIDE)));

        int len = 16 - mPkgTx.size();
        for (int i = 0; i < len; i++)
        {
            mPkgTx.append(0);
        }

        Q_ASSERT(mPkgTx.size() == 16);  // ������� + 15 ���� ������
    }

    if (com == 0x80)
    {
        if (!CheckSize(com, data.size(), { 15 }))
        {
            return;
        }

        // ����� �� ������� ������ ��������� � ��������
        mPkgTx.append(com);
        mPkgTx.append(data);

        int index = 0;
        SetParamValue(GB_PARAM_DEF_TYPE, data.at(index++));
        index++;  // ��� �����
        SetParamValue(GB_PARAM_TIME_NO_MAN, data.at(index++));
        SetParamValue(GB_PARAM_DELAY, data.at(index++));
        SetParamValue(GB_PARAM_OVERLAP, data.at(index++));
        SetParamValue(GB_PARAM_SENS_DEC_RZ, data.at(index++));
        SetParamValue(GB_PARAM_PRM_TYPE, data.at(index++));
        index++;  // �������� �� ����� 2
        SetParamValue(GB_PARAM_WARN_THD_RZ, data.at(index++));
        index++;  // ����������� ��� 2
        index++;  // ������������� �����SetParamValue(GB_PARAM_DEF_ONE_SIDE)));
    }
}


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


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������/������ ���������� ���������.
 * @param[in] �������.
 * @param[in] ������.
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

        Q_ASSERT(mPkgTx.size() == 2);  // ������� + 1 ���� ������
    }

    if (com == GB_COM_DEF_SET_OVERLAP)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ����� �� ������� ������ ��������� � ��������
        mPkgTx.append(com);
        mPkgTx.append(data);

        SetParamValue(GB_PARAM_OVERLAP, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������/������ ����������� ����������������
 * @param[in] �������.
 * @param[in] ������.
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

        Q_ASSERT(mPkgTx.size() == 2);  // ������� + 1 ���� ������
    }

    if (com == GB_COM_DEF_SET_RZ_DEC)
    {
        if (!CheckSize(com, data.size(), { 2 }))
        {
            return;
        }

        // ����� �� ������� ������ ��������� � ��������
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
 * @brief ������������ ������� ������/������ ���������� ������������.
 * @param[in] �������.
 * @param[in] ������.
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

        Q_ASSERT(mPkgTx.size() == 2);  // ������� + 1 ���� ������
    }

    if (com == GB_COM_DEF_SET_PRM_TYPE)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ����� �� ������� ������ ��������� � ��������
        mPkgTx.append(com);
        mPkgTx.append(data);

        SetParamValue(GB_PARAM_PRM_TYPE, data.at(0));
    }
}


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


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������/������ ������ �������������� �� ��
 * @param[in] �������.
 * @param[in] ������.
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

        Q_ASSERT(mPkgTx.size() == 2);  // ������� + 1 ���� ������
    }

    if (com == GB_COM_DEF_SET_RZ_THRESH)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ����� �� ������� ������ ��������� � ��������
        mPkgTx.append(com);
        mPkgTx.append(data);

        SetParamValue(GB_PARAM_WARN_THD_RZ, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������/������ �������������� ������.
 * @param[in] �������.
 * @param[in] ������.
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

        Q_ASSERT(mPkgTx.size() == 2);  // ������� + 1 ���� ������
    }

    if (com == GB_COM_DEF_SET_TYPE_AC)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ����� �� ������� ������ ��������� � ��������
        mPkgTx.append(com);
        mPkgTx.append(data);

        SetParamValue(GB_PARAM_DEF_ONE_SIDE, data.at(0));
    }
}


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

    if (com == 0xB0)
    {
        if (!CheckSize(com, data.size(), { 31 }))
        {
            return;
        }

        // ����� �� ������� ������ ��������� � ��������
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

        // ��������� � �� �� ���������������
        index++;  // ��������� �������� ��������� ���������� (����� �����)
        index++;  // ��������� �������� ��������� ���������� (������� �����)
        index++;  // ��������� �������� ���� 1 (������� ����)
        index++;  // ��������� �������� ���� 1 (������� ����)
        index++;  // ��������� �������� ���� 2 (������� ����)
        index++;  // ��������� �������� ���� 2 (������� ����)

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
 * @brief ������������ ������� ������ �������������� � ��������������.
 * @param[in] �������.
 * @param[in] ������.
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

    // @todo ����������� ��� �� ���� ��������� ��������������, � ���� �� ���

    mPkgTx.append(0);  // ��� ������������� ��������� ������ 02
    mPkgTx.append(0);  // ��� ������������� ��������� ������ 01
    mPkgTx.append(0);  // ��� ������������� ���������� ��������� 02
    mPkgTx.append(0);  // ��� ������������� ���������� ��������� 01

    mPkgTx.append(0);  // ��� ������������� ���������� ����������� 02
    mPkgTx.append(0);  // ��� ������������� ����������� ����������� 01
    mPkgTx.append(0);  // ��� ������������� ���������� ����� 02
    mPkgTx.append(0);  // ��� ������������� ���������� ����� 01

    Q_ASSERT(mPkgTx.size() == 25);  // ������� + 24 ���� ������
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
void TBspRzskHf::HdlrComGlbx32(eGB_COM com, pkg_t &data)
{
    uint16_t msec = dt.time().msec();

    Q_ASSERT(com == GB_COM_GET_TIME || com == GB_COM_SET_TIME);

    if (com == GB_COM_GET_TIME)
    {
        // C �� ���������� ���� ������� ��������� ��� �������� � ��� (0..3), � �� ��� ������
        // � �400� ���� ������� �� ��������������
        if (!CheckSize(com, data.size(), { 0, 1 }))
        {
            return;
        }

        // @todo �������� ���������� ������� ������� ������� � ������ ������ ��������
        // ��� ������� �������� ���������� ���� ���� ������:
        // 0 - �������� ������ ���� � �����
        // 1 - ��������� ��������� ������ �������
        // 2 - �������� ����� ������ ������� (���������� �������)
        // ����� � ����� ������������ ������� ������ 55 AA FF 00 FF

        mPkgTx.append(com);
        mPkgTx.append(int2bcd(dt.date().year() - 2000));
        mPkgTx.append(int2bcd(dt.date().month()));
        mPkgTx.append(int2bcd(dt.date().day()));
        mPkgTx.append(int2bcd(dt.time().hour()));
        mPkgTx.append(int2bcd(dt.time().minute()));
        mPkgTx.append(int2bcd(dt.time().second()));
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
 * @brief ������������ ������� ������ ���������.
 * @param[in] �������.
 * @param[in] ������.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComGlbx34(eGB_COM com, pkg_t &data)
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

    // @todo ����������� ����� �� U�2, � ���� �� ���������
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Udef1)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Udef2)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Ucf1)));
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Ucf2)));

    // � ���: �����. �������. �������. ��������� ���
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Un1)));  // K_Din;
    // � ���: �����. ������������ ���
    mPkgTx.append(static_cast<quint8>(getSpinBoxValue(m_measure.Un2)));  // K_Over
    // � ���: ����������� �������� ������
    mPkgTx.append(0);  // P_PA_High
    mPkgTx.append(0);  // P_PA_Low

    Q_ASSERT(mPkgTx.size() == 16);  // ������� + 15 ���� ������
}


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������/������ ������������� �������.
 * @note ��� �� ������� ������������ ��� ������/������ ���� ���������� ��������� � �������������
 * @param[in] �������.
 * @param[in] ������.
 *
 * *****************************************************************************
 */
void TBspRzskHf::HdlrComGlbx35(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == GB_COM_GET_TIME_SINCHR || com == GB_COM_SET_TIME_SINCHR);

    if (com == GB_COM_GET_TIME_SINCHR)
    {
        // ��� ���������� ���� ���������� ���������� ���� ���� ������
        // ��� ���������� ������������� ������� ������ ���
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
            mPkgTx.append(0);  // ��������� �������� ���� 2, ������� ����
            mPkgTx.append(0);  // ��������� �������� ���� 2, ������� ����

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

            Q_ASSERT(mPkgTx.size() == 32);  // ������� + 31 ���� ������
        }
        else
        {
            mPkgTx.append(com);
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TIME_SYNCH)));
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_DETECTOR)));

            Q_ASSERT(mPkgTx.size() == 3);  // ������� + 2 ���� ������
        }
    }

    if (com == GB_COM_SET_TIME_SINCHR)
    {
        if (!CheckSize(com, data.size(), { 1, 2 }))
        {
            return;
        }

        // ����� �� ������� ������ ��������� � ��������
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
 * @brief ������������ ������� ������/������ ��������� ���� ������ ���������.
 * @param[in] �������.
 * @param[in] ������.
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

        Q_ASSERT(mPkgTx.size() == 2);  // ������� + 1 ���� ������
    }

    if (com == GB_COM_SET_COM_PRM_KEEP)
    {
        // ������������ ������ ������ ����
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ����� �� ������� ������ ��������� � ��������
        mPkgTx.append(com);
        mPkgTx.append(data);

        SetParamValue(GB_PARAM_COM_PRM_KEEP, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������/������ ��������� ���� ������ ��� � �������������.
 * @param[in] �������.
 * @param[in] ������.
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

        Q_ASSERT(mPkgTx.size() == 3);  // ������� + 2 ����� ������
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
 * @brief ������������ ������� ������/������ ������� �����������.
 * @param[in] �������.
 * @param[in] ������.
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

        Q_ASSERT(mPkgTx.size() == 2);  // ������� + 1 ���� ������
    }

    if (com == GB_COM_SET_TIME_RERUN)
    {
        if (!CheckSize(com, data.size(), { 1 }))
        {
            return;
        }

        // ����� �� ������� ������ ��������� � ��������
        mPkgTx.append(com);
        mPkgTx.append(data);

        SetParamValue(GB_PARAM_TIME_RERUN, data.at(0));
    }
}


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������/������ ������ �������������� �� � ������. ������.
 * @param[in] �������.
 * @param[in] ������.
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

        Q_ASSERT(mPkgTx.size() == 4);  // ������� + 3 ����� ������
    }

    if (com == GB_COM_SET_CF_THRESHOLD)
    {
        if (!CheckSize(com, data.size(), { 2 }))
        {
            return;
        }

        // ����� �� ������� ������ ��������� � ��������
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
 * @brief ������������ ������� ������ �������� � ������.
 * @param[in] �������.
 * @param[in] ������.
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

    Q_ASSERT(mPkgTx.size() == 7);  // ������� + 6 ���� ������
}


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
    mPkgTx.append(mDefEnable.isChecked() ? 1 : 0);        // ������
    mPkgTx.append(GetParamValue(GB_PARAM_PRM_COM_NUMS));  // ���1
    mPkgTx.append(0);                                     // ���2
    mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_NUMS));  // ���
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

    Q_ASSERT(mPkgTx.size() == 18);  // ������� + 17 ���� ������
}


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ��������� ������ �� ���� 1 (����������)
 * @param[in] �������.
 * @param[in] ������.
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

                // �����
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

                // �����
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

    Q_ASSERT(mPkgTx.size() == 4);  // ������� + 3 ���� ������
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
