#ifndef BSP_R400_HF_HPP
#define BSP_R400_HF_HPP

#include "bsp.h"

typedef QVector<uint8_t> pkg_t;

class TBspR400Hf : public Bsp
{
public:
    explicit TBspR400Hf(QTreeWidget *tree, QWidget *parent = nullptr);

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

    void HdlrComGlbx30(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx31(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx32(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx3F(eGB_COM com, pkg_t &data) override;
};

#endif  // BSP_R400_HF_HPP
