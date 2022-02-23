#ifndef BSP_K400_HF_HPP
#define BSP_K400_HF_HPP

#include "bsp.h"

/**
 * *****************************************************************************
 *
 *  @brief Симулятор БСП АВАНТ K400-088-В, сборка xx
 *
 *  // @todo Проверить в аппарате что реально показывается и скорректировать показания
 *
 *  БСП МК       : 01.48
 *  БСП ПИ МК    : 01.52
 *  БСП DSP      : 1D.33
 *  БСЗ ПЛИС     : 05.02
 *  БСК ПЛИС ПРД : 02.05
 *  БСК ПЛИС ПРМ : 02.03
 *
 * *****************************************************************************
 */
class TBspK400Hf : public Bsp
{
public:
    explicit TBspK400Hf(QTreeWidget *tree, QWidget *parent = nullptr);

private:
    void InitComMap() override;
    void InitParam() override;

    void crtTreePrd(QTreeWidgetItem *top) override;
    void crtTreePrm(QTreeWidgetItem *top) override;
    void crtTreeGlb(QTreeWidgetItem *top) override;
    void crtTreeInterface(QTreeWidgetItem *top) override;
    void crtTreeDevice() override;
    void crtTreeDevieVersions(QTreeWidgetItem *top) override;

    void crtTreeMeasure() override;

    void crtTreeState() override;
    void crtTest() override;
    void crtJrn() override;
    void crtJrnGlb(QTreeWidgetItem *top) override;
    void crtJrnPrm(QTreeWidgetItem *top) override;
    void crtJrnPrd(QTreeWidgetItem *top) override;

    void FillComboboxListStatePrm() override;
    void FillComboBoxListControl() override;
    void FillComboboxListTest() override;

    void HdlrComPrmx10(eGB_COM com, pkg_t &data) override;

    void HdlrComPrdx20(eGB_COM com, pkg_t &data) override;

    void HdlrComGlbx30(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx31(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx34(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx35(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx37(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx3C(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx3E(eGB_COM com, pkg_t &data) override;
    void HdlrComGlbx3F(eGB_COM com, pkg_t &data) override;

    void HdlrComRegx7E(eGB_COM com, pkg_t &data) override;

private slots:
    void SlotChangeCompatibility();
};

#endif  // BSP_K400_HF_HPP
