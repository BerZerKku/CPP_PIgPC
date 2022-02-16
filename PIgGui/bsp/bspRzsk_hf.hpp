#ifndef BSP_RZSK_HF_HPP
#define BSP_RZSK_HF_HPP

#include "bsp.h"

typedef QVector<uint8_t> pkg_t;

/**
 * *****************************************************************************
 *
 *  @brief ��������� ��� ����� ����-111-�, ������ xx
 *
 *  // @todo ��������� � �������� ��� ������� ������������ � ��������������� ���������
 *
 *  ��� ��       : 01.34
 *  ��� �� ��    : 01.52
 *  ��� DSP      : 0D.33
 *  ��� ����     : 05.02
 *  ��� ���� ��� : 02.05
 *  ��� ���� ��� : 02.03
 *
 * *****************************************************************************
 */
class TBspRzskHf : public Bsp
{
public:
    explicit TBspRzskHf(QTreeWidget *tree, QWidget *parent = nullptr);

private:
    QSpinBox mPrdLightPA;           // ���������� ��� �����������
    QSpinBox mPrmLightPA;           // ���������� ��� ���������
    QSpinBox mExternalInputsState;  // ������� ����� ���

    void InitComMap() override;
    void InitParam() override;

    void crtTreeDef(QTreeWidgetItem *top) override;
    void crtTreePrd(QTreeWidgetItem *top) override { Q_UNUSED(top); }
    void crtTreePrm(QTreeWidgetItem *top) override { Q_UNUSED(top); }
    void crtTreeGlb(QTreeWidgetItem *top) override;
    void crtTreeInterface(QTreeWidgetItem *top) override;
    void crtTreeDevice() override;
    void crtTreeDevieVersions(QTreeWidgetItem *top) override;

    void crtTreeMeasure() override;

    void crtTreeState() override;
    void crtTest() override;
    void crtJrn() override;
    void crtJrnDef(QTreeWidgetItem *top) override;
    void crtJrnGlb(QTreeWidgetItem *top) override;

    void FillComboboxListStateDef() override;
    void FillComboBoxListControl() override;

    void HdlrComDefx00(eGB_COM com, pkg_t &data) override;
    //    void HdlrComDefx01(eGB_COM com, pkg_t &data) override;
    //    void HdlrComDefx02(eGB_COM com, pkg_t &data) override;
    //    void HdlrComDefx03(eGB_COM com, pkg_t &data) override;
    //    void HdlrComDefx05(eGB_COM com, pkg_t &data) override;
    //    void HdlrComDefx06(eGB_COM com, pkg_t &data) override;
    //    void HdlrComDefx07(eGB_COM com, pkg_t &data) override;
    //    void HdlrComDefx08(eGB_COM com, pkg_t &data) override;
    //    void HdlrComDefx09(eGB_COM com, pkg_t &data) override;
    //    void HdlrComDefx0A(eGB_COM com, pkg_t &data) override;

    void HdlrComGlbx30(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx31(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx32(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx34(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx35(eGB_COM com, pkg_t &data) override;
    //    void HdlrComGlbx36(eGB_COM com, pkg_t &data) override;
    //    void HdlrComGlbx37(eGB_COM com, pkg_t &data) override;
    //    void HdlrComGlbx38(eGB_COM com, pkg_t &data) override;
    //    void HdlrComGlbx39(eGB_COM com, pkg_t &data) override;
    //    void HdlrComGlbx3A(eGB_COM com, pkg_t &data) override;
    //    void HdlrComGlbx3B(eGB_COM com, pkg_t &data) override;
    //    void HdlrComGlbx3C(eGB_COM com, pkg_t &data) override;
    //    void HdlrComGlbx3D(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx3F(eGB_COM com, pkg_t &data) override;

    //    void HdlrComRegx70(eGB_COM com, pkg_t &data) override;
    //    void HdlrComRegx71(eGB_COM com, pkg_t &data) override;
    //    void HdlrComRegx72(eGB_COM com, pkg_t &data) override;

    //    void HdlrComJrnxC1(eGB_COM com, pkg_t &data) override;
    //    void HdlrComJrnxC2(eGB_COM com, pkg_t &data) override;
    //    void HdlrComJrnxF1(eGB_COM com, pkg_t &data) override;
    //    void HdlrComJrnxF2(eGB_COM com, pkg_t &data) override;

private slots:
    void SlotChangeCompatibility();
};

#endif  // BSP_RZSK_HF_HPP
