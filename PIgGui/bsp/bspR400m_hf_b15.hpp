#ifndef BSP_R400M_HF_B15_HPP
#define BSP_R400M_HF_B15_HPP

#include "bsp.h"

typedef QVector<uint8_t> pkg_t;

/**
 * *****************************************************************************
 *
 *  @brief ��������� ��� ����� �400�-100-�, ������ 15
 *
 *  // @todo ��������� � �������� ��� ������� ������������ � ��������������� ���������
 *
 *  ��� ��       : F2.32
 *  ��� �� ��    : 01.46
 *  ��� DSP      : 58.33
 *  ��� ����     : 05.02
 *
 * *****************************************************************************
 */
class TBspR400mHf_b15 : public Bsp
{
public:
    explicit TBspR400mHf_b15(QTreeWidget *tree, QWidget *parent = nullptr);

    QSpinBox mStateFaultDeviceNumber;
    QSpinBox mTemperature;

private:
    void InitComMap() override;
    void InitParam() override;

    void crtTreeDef() override;
    void crtTreeDevice() override;
    void crtTreeDevieVersions(QTreeWidgetItem *top) override;
    void crtTreeGlb() override;
    void crtTreeMeasure() override;
    void crtTreePrd() override;
    void crtTreePrm() override;
    void crtTreeState() override;
    void crtTest() override;

    void FillComboboxListStateDef() override;

    void HdlrComDefx02(eGB_COM com, pkg_t &data) override;

    void HdlrComGlbx30(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx31(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx32(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx34(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx37(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx38(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx3B(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx3F(eGB_COM com, pkg_t &data) override;
};

#endif  // BSP_R400M_HF_B15_HPP
