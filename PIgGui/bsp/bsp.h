#ifndef BSP_H
#define BSP_H

#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QMap>
#include <QRegExp>
#include <QSpinBox>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QTimer>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "PIg/src/glbDefine.h"
#include "widget/qhexspinbox.h"

typedef QVector<uint8_t> pkg_t;

class Bsp : public QWidget
{
    Q_OBJECT

protected:
    typedef void (Bsp::*HdlrCom_t)(eGB_COM com, pkg_t &data);

    static const QString     kMsgSizeError;
    static const QString     kMsgTimeSourceError;
    static const QTextCodec *kCodec;
    static const QString     kTimeFormat;

    struct state_t
    {
        QComboBox *regime  = nullptr;
        QComboBox *state   = nullptr;
        QSpinBox * dopByte = nullptr;
        QLineEdit *warning = nullptr;
        QLineEdit *fault   = nullptr;
    };

    struct device_t
    {
        QComboBox *isDef = nullptr;
        //
        QSpinBox *versionPiMcu       = nullptr;
        QSpinBox *versionBspMcu      = nullptr;
        QSpinBox *versionBspDsp      = nullptr;
        QSpinBox *versionBspDspPlis  = nullptr;
        QSpinBox *versionBsk1PrmPlis = nullptr;
        QSpinBox *versionBsk1PrdPlis = nullptr;
        QSpinBox *versionBsk2PrmPlis = nullptr;
        QSpinBox *versionBsk2PrdPlis = nullptr;
        QSpinBox *versionBszPlis     = nullptr;
    };

    struct test_t
    {
        QLineEdit *byte1;
        QLineEdit *byte2;
        QLineEdit *byte3;
        QLineEdit *byte4;
        QLineEdit *byte5;
    };

    struct measure_t
    {
        QSpinBox *R;
        QSpinBox *I;
        QSpinBox *U;
        QSpinBox *Udef1;
        QSpinBox *Udef2;
        QSpinBox *Ucf1;
        QSpinBox *Ucf2;
        QSpinBox *Un1;
        QSpinBox *Un2;
        QSpinBox *Sd;
        QSpinBox *T;
        QSpinBox *dF;
    };

    QMap<uint8_t, HdlrCom_t> mComMap;

    QSpinBox  mJrnDefCounter;
    QCheckBox mJrnDefOverflow;

    QSpinBox  mJrnPrdCounter;
    QCheckBox mJrnPrdOverflow;

    QSpinBox  mJrnPrmCounter;
    QCheckBox mJrnPrmOverflow;

    QSpinBox  mJrnGlbCounter;
    QCheckBox mJrnGlbOverflow;

    QComboBox mControl;

public:
    explicit Bsp(QTreeWidget *tree, QWidget *parent = nullptr);

    /// Подсчет контрольной суммы.
    uint8_t calcCrc(pkg_t &pkg);
    /// Проверка пакета.
    bool checkPkg(pkg_t &pkg, eGB_COM &com);

    void Init();

    QMap<eGB_PARAM, QVector<QComboBox *>>      mapCombobox;
    QMap<eGB_PARAM, QLineEdit *>               mapLineEdit;
    QMap<eGB_PARAM, QVector<QSpinBox *>>       mapSpinBox;
    QMap<eGB_PARAM, QVector<QDoubleSpinBox *>> mapDoubleSpinBox;

    QString getParamName(eGB_PARAM param);

    pkg_t mPkgRx;  ///< Данные на приеме.
    pkg_t mPkgTx;  ///< Данные для передачи.

    void   CrtParamWidget(QTreeWidgetItem *top, eGB_PARAM param);
    qint16 GetParamValue(eGB_PARAM param, quint8 number = 1);
    void   SetParamValue(eGB_PARAM param, qint16 value, quint8 number = 1);

    quint8 getComboBoxValue(eGB_PARAM param, uint8_t number = 1);
    quint8 getComboBoxValue(QComboBox *combobox);
    void   setComboBoxValue(eGB_PARAM param, quint8 value, uint8_t number = 1);
    void   setComboBoxValueBits(eGB_PARAM param, quint8 value, uint8_t number = 1);
    int    setComboBoxValue(QComboBox *combobox, quint8 value);

    QString getLineEditValue(eGB_PARAM param);
    QString getLineEditValue(QLineEdit *lineedit);
    void    setLineEditValue(eGB_PARAM param, std::string value);
    int     setLineEditValue(QLineEdit *lineedit, std::string value);

    qint16 getSpinBoxValue(eGB_PARAM param, uint8_t number = 1);
    void   setSpinBoxValue(eGB_PARAM param, qint16 value, uint8_t number = 1);
    qint16 getSpinBoxValue(QSpinBox *spinbox);
    int    setSpinBoxValue(QSpinBox *spinbox, qint16 value);

    qint16 getDoubleSpinBoxValue(eGB_PARAM param, uint8_t number = 1);
    void   setDoubleSpinBoxValue(eGB_PARAM param, qint16 value, uint8_t number = 1);

    device_t  mDevice;
    state_t   stateDef;
    state_t   stateGlb;
    state_t   statePrm;
    state_t   statePrd;
    test_t    test;
    measure_t m_measure;

public slots:
    void SlotReadByte(int value);

signals:
    void SignalWriteByte(int value);
    void SignalSendFinished();

protected:
    QTreeWidget *const mTree;

    QDateTime dt;  ///< Дата и время.

    QRegExp          pwdRegExp;
    QRegExpValidator pwdValidator;
    QRegExp          errRegExp;
    QRegExpValidator errValidator;
    QTimer           mTimerSignalSendFinished;

    void         InitClock();
    virtual void InitComMap();
    virtual void InitParam();

    void             crtTreeParam();
    virtual void     crtTreeDef(QTreeWidgetItem *top);
    virtual void     crtTreePrd(QTreeWidgetItem *top);
    virtual void     crtTreePrm(QTreeWidgetItem *top);
    virtual void     crtTreeGlb(QTreeWidgetItem *top);
    virtual void     crtTreeInterface(QTreeWidgetItem *top);
    virtual void     crtTreeDevice();
    virtual void     crtTreeDevieVersions(QTreeWidgetItem *top);
    virtual void     crtTreeMeasure();
    virtual void     crtTreeState();
    QTreeWidgetItem *crtTreeState(QTreeWidgetItem *top, std::string name, state_t &state);
    virtual void     crtTest();
    virtual void     crtJrn();
    virtual void     crtJrnGlb(QTreeWidgetItem *top);
    virtual void     crtJrnDef(QTreeWidgetItem *top);

    void fillComboboxList(QComboBox *combobox, eGB_PARAM param);
    void fillComboboxListOnOff(QComboBox *combobox);
    void fillComboboxListRegime(QComboBox *combobox);

    void crtComboBox(QTreeWidgetItem *top, eGB_PARAM param);
    void crtLineEdit(QTreeWidgetItem *top, eGB_PARAM param, std::string);
    void crtSpinBox(QTreeWidgetItem *top, eGB_PARAM param);
    void crtDoubleSpinBox(QTreeWidgetItem *top, eGB_PARAM param);

    virtual void FillComboboxListStateDef() { }
    virtual void FillComboboxListStatePrm() { }
    virtual void FillComboboxListStatePrd() { }
    void         FillComboboxListStateGlb();
    virtual void FillComboBoxListControl() {};

    void keyPressEvent(QKeyEvent *event) override;

    /// Преобразование bcd кода в целое.
    quint8 bcd2int(quint8 bcd, bool *ok = nullptr);
    /// Преобразование целого числа в bcd код.
    quint8 int2bcd(quint8 val, bool *ok = nullptr);

    void procCommand(eGB_COM com, pkg_t &data);
    void procCommandReadParam(eGB_COM com, pkg_t &data);
    void procCommandWriteParam(eGB_COM com, pkg_t &data);
    void procCommandWriteRegime(eGB_COM com, pkg_t &data);

    void hdlrComGetTest(eGB_COM com, pkg_t &data);

    uint8_t getCompatibility(eGB_TYPE_DEVICE device);

    bool CheckSize(uint8_t com, int size, QVector<int> size_allowed);

public:
    virtual void HdlrComDummy(eGB_COM com, pkg_t &data);

    // Change: 0x80-0x8F
    virtual void HdlrComDefx00(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComDefx01(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComDefx02(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComDefx03(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComDefx04(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComDefx05(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComDefx06(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); }
    virtual void HdlrComDefx07(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComDefx08(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComDefx09(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComDefx0A(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComDefx0B(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };

    // Change: 0x90-0x9F
    virtual void HdlrComPrmx11(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrmx12(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrmx13(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrmx14(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrmx15(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrmx16(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrmx17(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrmx18(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrmx19(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrmx1B(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrmx1C(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrmx1D(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };

    // Change: 0xA0-0xAF
    virtual void HdlrComPrdx21(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrdx22(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrdx23(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrdx24(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrdx25(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrdx26(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrdx27(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrdx28(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrdx29(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrdx2B(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrdx2C(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrdx2D(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComPrdx2E(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };

    // Change:0xB0-0xBF
    virtual void HdlrComGlbx30(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComGlbx31(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComGlbx32(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComGlbx33(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComGlbx34(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComGlbx35(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComGlbx36(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComGlbx37(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComGlbx38(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComGlbx39(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComGlbx3A(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComGlbx3B(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComGlbx3C(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComGlbx3D(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComGlbx3E(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComGlbx3F(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };

    virtual void HdlrComRegx70(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComRegx71(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComRegx72(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComRegx73(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComRegx74(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComRegx7D(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComRegx7E(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };

    virtual void HdlrComJrnxC1(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComJrnxC2(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComJrnxCA(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComJrnxD1(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComJrnxD2(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComJrnxDA(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComJrnxE1(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComJrnxE2(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComJrnxEA(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComJrnxF1(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComJrnxF2(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };
    virtual void HdlrComJrnxFA(eGB_COM com, pkg_t &data) { HdlrComDummy(com, data); };

protected slots:
    void setRegime(int index);
    void setState(int index);
    void setDopByte(int index);
    void updateClock();
    void SlotReadPackageFinished();
};

#endif  // BSP_H
