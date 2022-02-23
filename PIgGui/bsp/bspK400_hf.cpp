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
 * @brief �����������.
 * @param[in] tree ������ ������ ����������.
 * @param[in] parent ������������ ������.
 *
 * *****************************************************************************
 */
TBspK400Hf::TBspK400Hf(QTreeWidget *tree, QWidget *parent) : Bsp(tree, parent)
{
}


/**
 * *****************************************************************************
 *
 * @brief ����������� ����� ������.
 *
 * *****************************************************************************
 */
void TBspK400Hf::InitComMap()
{
    mComMap.insert(0x02, &Bsp::HdlrComDefx02);  // ������ ���������� ��������� � �����

    mComMap.insert(0x10, &Bsp::HdlrComPrmx10);  // ������ ���������� ��� � �������������
    mComMap.insert(0x11, &Bsp::HdlrComPrmx11);  // ������ �������� �� ���������
    mComMap.insert(0x12, &Bsp::HdlrComPrmx12);  // ������ �������� �������
    mComMap.insert(0x13, &Bsp::HdlrComPrmx13);  // ������ �������� �� ����������
    mComMap.insert(0x14, &Bsp::HdlrComPrmx14);  // ������ ������������� ������
    mComMap.insert(0x15, &Bsp::HdlrComPrmx15);  // ������ ��������� ������� ���
    mComMap.insert(0x17, &Bsp::HdlrComPrmx17);  // ������ ������� ���
    mComMap.insert(0x18, &Bsp::HdlrComPrmx18);  // ������ ������������� ������ ���
    mComMap.insert(0x19, &Bsp::HdlrComPrmx19);  // ������ �������������� ������ ���
    mComMap.insert(0x1C, &Bsp::HdlrComPrmx1C);  // ������ ���������� ������ �� ������
    mComMap.insert(0x1D, &Bsp::HdlrComPrmx1D);  // ������ ���������� ������������ ������

    mComMap.insert(0x20, &Bsp::HdlrComPrdx20);  // ������ ���������� ��� � �������������
    mComMap.insert(0x21, &Bsp::HdlrComPrdx21);  // ������ �������� �� ���������
    mComMap.insert(0x22, &Bsp::HdlrComPrdx22);  // ������ ������������ �������
    mComMap.insert(0x23, &Bsp::HdlrComPrdx23);  // ������ ��������� �������
    mComMap.insert(0x24, &Bsp::HdlrComPrdx24);  // ������ ������������� ������
    mComMap.insert(0x25, &Bsp::HdlrComPrdx25);  // ������ ���������� ������
    mComMap.insert(0x26, &Bsp::HdlrComPrdx26);  // ������ ������� �������� �������
    mComMap.insert(0x27, &Bsp::HdlrComPrdx27);  // ������ ������� ���
    mComMap.insert(0x28, &Bsp::HdlrComPrdx28);  // ������ ������������� ������ ���
    mComMap.insert(0x29, &Bsp::HdlrComPrdx29);  // ������ ���������� ������ �
    mComMap.insert(0x2C, &Bsp::HdlrComPrdx2C);  // ������ ���������� ������ �� ��������
    mComMap.insert(0x2D, &Bsp::HdlrComPrdx2D);  // ������ �������� ������� �� � ��
    mComMap.insert(0x2E, &Bsp::HdlrComPrdx2E);  // ������ ���������� ������������ ������

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
    mComMap.insert(0x3F, &Bsp::HdlrComGlbx3F);  // ������ ���������� �� ����������

    mComMap.insert(0x51, &Bsp::HdlrComRegx51);  // ���� ���

    mComMap.insert(0x70, &Bsp::HdlrComRegx70);  // ����� ������ �� "�������"
    mComMap.insert(0x71, &Bsp::HdlrComRegx71);  // ����� ������ �� "������"
    mComMap.insert(0x72, &Bsp::HdlrComRegx72);  // ������� ����������
    mComMap.insert(0x7D, &Bsp::HdlrComRegx7D);  // ����� ������ �� "���� 2 (���)"
    mComMap.insert(0x7E, &Bsp::HdlrComRegx7E);  // ����� ������ �� "���� 1 (���)"

    mComMap.insert(0x82, &Bsp::HdlrComDefx02);  // ������ ���������� ��������� � �����

    mComMap.insert(0x90, &Bsp::HdlrComPrmx10);  // ������ ���������� ��� � �������������
    mComMap.insert(0x91, &Bsp::HdlrComPrmx11);  // ������ �������� �� ���������
    mComMap.insert(0x92, &Bsp::HdlrComPrmx12);  // ������ �������� �������
    mComMap.insert(0x93, &Bsp::HdlrComPrmx13);  // ������ �������� �� ����������
    mComMap.insert(0x94, &Bsp::HdlrComPrmx14);  // ������ ������������� ������
    mComMap.insert(0x95, &Bsp::HdlrComPrmx15);  // ������ ��������� ������� ���
    mComMap.insert(0x97, &Bsp::HdlrComPrmx17);  // ������ ������� ���
    mComMap.insert(0x98, &Bsp::HdlrComPrmx18);  // ������ ������������� ������ ���
    mComMap.insert(0x99, &Bsp::HdlrComPrmx19);  // ������ �������������� ������ ���
    mComMap.insert(0x9A, &Bsp::HdlrComRegx9A);  // ������� ������ ���������
    mComMap.insert(0x9B, &Bsp::HdlrComPrmx1B);  // ������ ���� ��������������� ������ ���
    mComMap.insert(0x9C, &Bsp::HdlrComPrmx1C);  // ������ ���������� ������ �� ������
    mComMap.insert(0x9D, &Bsp::HdlrComPrmx1D);  // ������ ���������� ������������ ������

    mComMap.insert(0xA0, &Bsp::HdlrComPrdx20);  // ������ ���������� ��� � �������������
    mComMap.insert(0xA1, &Bsp::HdlrComPrdx21);  // ������ �������� �� ���������
    mComMap.insert(0xA2, &Bsp::HdlrComPrdx22);  // ������ ������������ �������
    mComMap.insert(0xA3, &Bsp::HdlrComPrdx23);  // ������ ��������� �������
    mComMap.insert(0xA4, &Bsp::HdlrComPrdx24);  // ������ ������������� ������
    mComMap.insert(0xA5, &Bsp::HdlrComPrdx25);  // ������ ���������� ������
    mComMap.insert(0xA6, &Bsp::HdlrComPrdx26);  // ������ ������� �������� �������
    mComMap.insert(0xA7, &Bsp::HdlrComPrdx27);  // ������ ������� ���
    mComMap.insert(0xA8, &Bsp::HdlrComPrdx28);  // ������ ������������� ������ ���
    mComMap.insert(0xA9, &Bsp::HdlrComPrdx29);  // ������ ���������� ������ �
    mComMap.insert(0xAC, &Bsp::HdlrComPrdx2C);  // ������ ���������� ������ �� ��������
    mComMap.insert(0xAD, &Bsp::HdlrComPrdx2D);  // ������ �������� ������� �� � ��
    mComMap.insert(0xAE, &Bsp::HdlrComPrdx2E);  // ������ ���������� ������������ ������

    mComMap.insert(0xB0, &Bsp::HdlrComGlbx30);  // ������ ����� ���������� � �������������
    mComMap.insert(0xB2, &Bsp::HdlrComGlbx32);  // ������ �������
    mComMap.insert(0xB3, &Bsp::HdlrComGlbx33);  // ������ ��������� ���� � ����������
    mComMap.insert(0xB5, &Bsp::HdlrComGlbx35);  // ������ ������������� ������� / ����
    mComMap.insert(0xB6, &Bsp::HdlrComGlbx36);  // ������ ��������� ���� ���
    mComMap.insert(0xB7, &Bsp::HdlrComGlbx37);  // ������ �����. ���� ��� � �������.
    mComMap.insert(0xB8, &Bsp::HdlrComGlbx38);  // ������ ������ � ��������� ����
    mComMap.insert(0xB9, &Bsp::HdlrComGlbx39);  // ������ ������� �����������
    mComMap.insert(0xBA, &Bsp::HdlrComGlbx3A);  // ������ �������
    mComMap.insert(0xBB, &Bsp::HdlrComGlbx3B);  // ������ ������ ��������
    mComMap.insert(0xBC, &Bsp::HdlrComGlbx3C);  // ������ ������ �������. �� � ����. ������.
    mComMap.insert(0xBD, &Bsp::HdlrComGlbx3D);  // ������ �������� ������

    mComMap.insert(0xD1, &Bsp::HdlrComJrnxD1);  // ������ ���������� ������� � ������� ���
    mComMap.insert(0xD2, &Bsp::HdlrComJrnxD2);  // ������ ������� ������� ���
    mComMap.insert(0xE1, &Bsp::HdlrComJrnxE1);  // ������ ���������� ������� � ������� ���
    mComMap.insert(0xE2, &Bsp::HdlrComJrnxE2);  // ������ ������� ������� ���
    mComMap.insert(0xF1, &Bsp::HdlrComJrnxF1);  // ������ ���������� ������� � ������� �������
    mComMap.insert(0xF2, &Bsp::HdlrComJrnxF2);  // ������ ������� ������� �������
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
    setComboBoxValue(stateGlb.state, 1);  // 1 - ��������
    setSpinBoxValue(stateGlb.dopByte, 1);

    // ��������� ����������
    setSpinBoxValue(GB_PARAM_NET_ADDRESS, 17);

    // ��������� �����
    SetParamValue(GB_PARAM_COMP_K400, GB_COMP_K400_AVANT);
    SetParamValue(GB_PARAM_NUM_OF_DEVICES, GB_NUM_DEVICES_2);
    SetParamValue(GB_PARAM_NUM_OF_DEVICE, 1);
    SetParamValue(GB_PARAM_TIME_SYNCH, 1);  // 1 - ���.
    SetParamValue(GB_PARAM_OUT_CHECK, 0);   // 0 - ����.
    SetParamValue(GB_PARAM_WARN_THD, 11);
    SetParamValue(GB_PARAM_TIME_RERUN, 3);
    SetParamValue(GB_PARAM_COM_PRD_KEEP, 0);  // 0 - ����.
    SetParamValue(GB_PARAM_COM_PRM_KEEP, 1);  // 1 - ���.
    SetParamValue(GB_PARAM_IN_DEC, 12, 1);
    SetParamValue(GB_PARAM_IN_DEC, 13, 2);
    SetParamValue(GB_PARAM_FREQ, 123);
    SetParamValue(GB_PARAM_COR_U, -105);  // -10.5 �
    SetParamValue(GB_PARAM_COR_I, 20);    // 20 ��
    SetParamValue(GB_PARAM_TM_K400, 1);   // 1 - ���.
    SetParamValue(GB_PARAM_WARN_D, -16);
    SetParamValue(GB_PARAM_ALARM_D, 24);
    SetParamValue(GB_PARAM_TEMP_MONITOR, 0);  // 0 -����.
    SetParamValue(GB_PARAM_TEMP_THR_HI, 54);
    SetParamValue(GB_PARAM_TEMP_THR_LOW, 17);
    SetParamValue(GB_PARAM_TM_SPEED, 1);  // 400 ���

    // ��������� �����������
    SetParamValue(GB_PARAM_PRD_COM_NUMS, 8);
    SetParamValue(GB_PARAM_PRD_IN_DELAY, 7);
    SetParamValue(GB_PARAM_PRD_DURATION_L, 50);
    SetParamValue(GB_PARAM_PRD_TEST_COM, 1);         // 1 - ���.
    SetParamValue(GB_PARAM_PRD_COM_LONG, 0x0F, 1);   // ����, 1 - ���.
    SetParamValue(GB_PARAM_PRD_COM_BLOCK, 0xF0, 2);  // ����, 1 - ���.
    SetParamValue(GB_PARAM_PRD_COM_NUMS_A, 5);
    SetParamValue(GB_PARAM_PRD_DR_ENABLE, 0);           // 0 - ����
    SetParamValue(GB_PARAM_PRD_DR_COM_BLOCK, 0x07, 3);  // ����, 1 - ���.
    SetParamValue(GB_PARAM_PRD_FREQ_CORR, -37);         //
    SetParamValue(GB_PARAM_PRD_DEC_CF, 10);
    SetParamValue(GB_PARAM_PRD_DEC_TM, 12);
    SetParamValue(GB_PARAM_PRD_COM_SIGNAL, 1);  // 1 - ���.

    // ��������� ���������
    SetParamValue(GB_PARAM_PRM_COM_NUMS, 8);
    SetParamValue(GB_PARAM_PRM_TIME_ON, 6);
    SetParamValue(GB_PARAM_PRM_COM_BLOCK, 0x05, 1);  // ����, 1 - ���.
    SetParamValue(GB_PARAM_PRM_TIME_OFF, 250 / 10, 1);
    SetParamValue(GB_PARAM_PRM_TIME_OFF, 500 / 10, 4);
    SetParamValue(GB_PARAM_PRM_DR_ENABLE, 1);           // 1 - ���
    SetParamValue(GB_PARAM_PRM_DR_COM_BLOCK, 0xC1, 3);  // ����, 1 - ���.
    SetParamValue(GB_PARAM_PRM_DR_COM_TO_HF, 32, 1);
    SetParamValue(GB_PARAM_PRM_DR_COM_TO_HF, 27, 32);
    SetParamValue(GB_PARAM_PRM_TEST_COM, 0);  // 0 - ����
    SetParamValue(GB_PARAM_PRM_FREQ_CORR, 35);
    SetParamValue(GB_PARAM_PRM_COM_SIGNAL, 0xEE, 2);  // ����, 1 - ���.

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
    ltop->setText(0, kCodec->toUnicode("����������"));
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
    ltop->setText(0, kCodec->toUnicode("��������"));
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
    ltop->setText(0, kCodec->toUnicode("�����"));
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
    ltop->setText(0, kCodec->toUnicode("���������"));
    top->addChild(ltop);

    CrtParamWidget(ltop, GB_PARAM_NET_ADDRESS);
}


void TBspK400Hf::crtTreeDevice()
{
    QTreeWidgetItem *top = new QTreeWidgetItem();

    mTree->insertTopLevelItem(mTree->topLevelItemCount(), top);
    top->setText(0, kCodec->toUnicode("����������"));

    crtTreeDevieVersions(top);
}


void TBspK400Hf::crtTreeDevieVersions(QTreeWidgetItem *top)
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

    item                  = new QTreeWidgetItem();
    mDevice.versionBspDsp = new QHexSpinBox(false);
    item->setText(0, kCodec->toUnicode("��� ���"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionBspDsp);

    item                   = new QTreeWidgetItem();
    mDevice.versionDspPlis = new QHexSpinBox(false);
    item->setText(0, kCodec->toUnicode("��� ���� ���"));
    branch->addChild(item);
    mTree->setItemWidget(item, 1, mDevice.versionDspPlis);


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


void TBspK400Hf::crtTreeMeasure()
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
    widget->setRange(-100, 125);
    widget->setToolTip(QString("[%1, %2] �C").arg(widget->minimum()).arg(widget->maximum()));
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
    top->setText(0, kCodec->toUnicode("������� ���������"));

    errRegExp.setPattern("[0-9A-Fa-F]+");
    errValidator.setRegExp(errRegExp);

    Bsp::crtTreeState(top, "�����", stateGlb);
    Bsp::crtTreeState(top, "��������", statePrm);
    Bsp::crtTreeState(top, "����������", statePrd);
    Bsp::crtTreeState(top, "��������2", statePrm2);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("����������"));
    top->insertChild(0, item);
    mTree->setItemWidget(item, 1, &mControl);

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
    item->setText(0, kCodec->toUnicode("��������� ��"));
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
    top->setText(0, kCodec->toUnicode("����"));

    item = new QTreeWidgetItem(top);
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("������� 1"));
    mTree->setItemWidget(item, 1, &mTestCf1Signal);

    item = new QTreeWidgetItem(top);
    top->addChild(item);
    item->setText(0, kCodec->toUnicode("������� 2"));
    mTree->setItemWidget(item, 1, &mTestCf2Signal);
}


/**
 * *****************************************************************************
 *
 * @brief ������� ����� ��������.
 *
 * *****************************************************************************
 */
void TBspK400Hf::crtJrn()
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
 * @brief ������� ������ �������.
 * @param[in] top ������� �������.
 *
 * *****************************************************************************
 */
void TBspK400Hf::crtJrnGlb(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("���������� �������"));
    mTree->setItemWidget(item, 1, &mJrnGlbCounter);
    mJrnGlbCounter.setRange(0, 512);

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
void TBspK400Hf::crtJrnPrm(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("���������� �������"));
    mTree->setItemWidget(item, 1, &mJrnPrmCounter);
    mJrnPrmCounter.setRange(0, 512);

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
void TBspK400Hf::crtJrnPrd(QTreeWidgetItem *top)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("���������� �������"));
    mTree->setItemWidget(item, 1, &mJrnPrdCounter);
    mJrnPrdCounter.setRange(0, 512);

    item = new QTreeWidgetItem(top);
    item->setText(0, kCodec->toUnicode("������������"));
    mTree->setItemWidget(item, 1, &mJrnPrdOverflow);
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ������ ��������� ���������.
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
 * @brief ���������� ������ ��������� ������ ����������.
 *
 * *****************************************************************************
 */
void TBspK400Hf::FillComboBoxListControl()
{
    mControl.addItem(kCodec->toUnicode("0 - ���"), 0);
    mControl.addItem(kCodec->toUnicode("1 - ����� ������"), 1);
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
void TBspK400Hf::FillComboboxListTest()
{
    mTestCf1Signal.addItem(kCodec->toUnicode("���"), 0);
    mTestCf1Signal.addItem(kCodec->toUnicode("��1"), 1);
    mTestCf1Signal.addItem(kCodec->toUnicode("��2"), 2);

    for (int i = 1; i <= 32; i++)
    {
        QString text = kCodec->toUnicode("������� %1").arg(i);
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
 * @brief ���������� ��������� �������������.
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
 * @brief ������������ ������� ������/������ ���� ���������� ���������.
 * @param[in] �������.
 * @param[in] ������.
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
        mPkgTx.append(0);  // �������� ������������
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

        Q_ASSERT(mPkgTx.size() == 96);  // ������� + 95 ���� ������
    }

    if (com == 0x90)
    {
        if (!CheckSize(com, data.size(), { 95 }))
        {
            return;
        }

        // ����� �� ������� ������ ��������� � ��������
        mPkgTx.append(com);
        mPkgTx.append(data);

        int index = 0;
        SetParamValue(GB_PARAM_PRM_TIME_ON, data.at(index++));
        index++;  // �������� ������������
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
        index += 37;  // ���������� ���������� ���

        SetParamValue(GB_PARAM_PRM_COM_SIGNAL, data.at(index++), 1);
        SetParamValue(GB_PARAM_PRM_COM_SIGNAL, data.at(index++), 2);
        SetParamValue(GB_PARAM_PRM_COM_SIGNAL, data.at(index++), 3);
        SetParamValue(GB_PARAM_PRM_COM_SIGNAL, data.at(index++), 4);
    }
}


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������/������ ���� ���������� �����������.
 * @param[in] �������.
 * @param[in] ������.
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
        mPkgTx.append(0);  // �� �� ���������
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_SIGNAL, 1));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_SIGNAL, 2));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_SIGNAL, 3));
        mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_SIGNAL, 4));


        int len = 32 - mPkgTx.size();
        for (int i = 0; i < len; i++)
        {
            mPkgTx.append(0);
        }

        Q_ASSERT(mPkgTx.size() == 32);  // ������� + 31 ���� ������
    }

    if (com == 0xA0)
    {
        // �� ������������� ������-�� ���������� 32 �����
        if (!CheckSize(com, data.size(), { 31, 32 }))
        {
            return;
        }

        // ����� �� ������� ������ ��������� � ��������
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
        index += 5;  // ��������� ���
        SetParamValue(GB_PARAM_PRD_DEC_CF, data.at(index++));
        SetParamValue(GB_PARAM_PRD_DEC_TM, data.at(index++));
        index++;  // �� �� ���������
        SetParamValue(GB_PARAM_PRD_COM_SIGNAL, data.at(index++), 1);
        SetParamValue(GB_PARAM_PRD_COM_SIGNAL, data.at(index++), 2);
        SetParamValue(GB_PARAM_PRD_COM_SIGNAL, data.at(index++), 3);
        SetParamValue(GB_PARAM_PRD_COM_SIGNAL, data.at(index++), 4);
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
void TBspK400Hf::HdlrComGlbx30(eGB_COM com, pkg_t &data)
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
        SetParamValue(GB_PARAM_WARN_THD, data.at(index++));
        SetParamValue(GB_PARAM_NUM_OF_DEVICE, data.at(index++));
        SetParamValue(GB_PARAM_TIME_SYNCH, data.at(index++));
        SetParamValue(GB_PARAM_NET_ADDRESS, data.at(index++));

        quint16 freq = data.at(index++);
        freq += static_cast<quint16>(data.at(index++)) << 8;
        SetParamValue(GB_PARAM_FREQ, freq);

        // ��������� � �� �� ���������������
        index++;  // ��������� �������� ��������� ���������� (����� �����)
        index++;  // ��������� �������� ��������� ���������� (������� �����)
        index++;  // ��������� �������� ���� 1 (������� ����)
        index++;  // ��������� �������� ���� 1 (������� ����)
        index++;  // ��������� �������� ���� 2 (������� ����)
        index++;  // ��������� �������� ���� 2 (������� ����)

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
 * @brief ������������ ������� ������ �������������� � ��������������.
 * @param[in] �������.
 * @param[in] ������.
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

    Q_ASSERT(mPkgTx.size() == 21);  // ������� + 20 ���� ������
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
void TBspK400Hf::HdlrComGlbx34(eGB_COM com, pkg_t &data)
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

    mPkgTx.append(getSpinBoxValue(m_measure.D));
    mPkgTx.append(0);  // � ��� 2U�
    mPkgTx.append(getSpinBoxValue(m_measure.Ucf1));
    mPkgTx.append(getSpinBoxValue(m_measure.Ucf2));
    mPkgTx.append(getSpinBoxValue(m_measure.Un1));
    mPkgTx.append(getSpinBoxValue(m_measure.Un2));
    mPkgTx.append(0);
    mPkgTx.append(0);
    mPkgTx.append(getSpinBoxValue(m_measure.T));
    mPkgTx.append(getSpinBoxValue(m_measure.dF));

    Q_ASSERT(mPkgTx.size() == 18);  // ������� + 17 ���� ������
}


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������/������ ������������� �������.
 * @note ��� �� ������� ������������ ��� ������/������ ���� ���������� ��������� �
 �������������
 * @param[in] �������.
 * @param[in] ������.
 *
 * *****************************************************************************
 */
void TBspK400Hf::HdlrComGlbx35(eGB_COM com, pkg_t &data)
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
            mPkgTx.append(0);  // ��������� �������� ���� 2, ������� ����
            mPkgTx.append(0);  // ��������� �������� ���� 2, ������� ����

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

            Q_ASSERT(mPkgTx.size() == 32);  // ������� + 31 ���� ������
        }
        else
        {
            mPkgTx.append(com);
            mPkgTx.append(static_cast<uint8_t>(GetParamValue(GB_PARAM_TIME_SYNCH)));

            Q_ASSERT(mPkgTx.size() == 2);  // ������� + 1 ���� ������
        }
    }

    if (com == GB_COM_SET_TIME_SINCHR)
    {
        // �� ���� ������ ���� 2 �����, ������ � ������� ���������
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
 * @brief ������������ ������� ������/������ ��������� ���� ������ ��� � �������������.
 * @param[in] �������.
 * @param[in] ������.
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

        Q_ASSERT(mPkgTx.size() == 10);  // ������� + 9 ���� ������
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
 * @brief ������������ ������� ������/������ ������ �������������� �� � ������. ������.
 * @param[in] �������.
 * @param[in] ������.
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
 * @brief ������������ ������� ������ �������� � ������.
 * @param[in] �������.
 * @param[in] ������.
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

    Q_ASSERT(mPkgTx.size() == 11);  // ������� + 10 ���� ������
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
    mPkgTx.append(0);                                     // ������
    mPkgTx.append(GetParamValue(GB_PARAM_PRM_COM_NUMS));  // ���1
    mPkgTx.append(0);                                     // ���2
    mPkgTx.append(GetParamValue(GB_PARAM_PRD_COM_NUMS));  // ���
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
    mPkgTx.append(0);  // ������ ��
    mPkgTx.append(0);  // ������ ��
    mPkgTx.append(0);  // ������� ����������� ������
    mPkgTx.append(static_cast<quint8>(vers_bsp_dsp_plis >> 8));
    mPkgTx.append(static_cast<quint8>(vers_bsp_dsp_plis));

    Q_ASSERT(mPkgTx.size() == 23);  // ������� + 22 ���� ������
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
            // �� ���� ������ ���������� ��� ������ ��������: �� � ��.
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

    Q_ASSERT(mPkgTx.size() == 6);  // ������� + 5 ���� ������
}
