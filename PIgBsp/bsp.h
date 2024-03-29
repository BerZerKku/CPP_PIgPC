#ifndef BSP_H
#define BSP_H

#include <QComboBox>

#include "PIg/src/glbDefine.h"
#include <QDateTime>
#include <QLineEdit>
#include <QMap>
#include <QRegExp>
#include <QSpinBox>
#include <QString>
#include <QStringList>
#include <QTreeWidget>
#include <QTreeWidgetItem>

typedef QVector<uint8_t> pkg_t;

class Bsp : public QTreeWidget
{
    Q_OBJECT

    static const QString msgSizeError;
    static const QString msgTimeSourceError;

    struct state_t
    {
        QComboBox *regime  = nullptr;
        QSpinBox * state   = nullptr;
        QSpinBox * dopByte = nullptr;
        QLineEdit *warning = nullptr;
        QLineEdit *fault   = nullptr;
    };

    struct device_t
    {
        QComboBox *isDef      = nullptr;
        QComboBox *typeDevice = nullptr;
        QComboBox *typeLine   = nullptr;
        QComboBox *typeOpto   = nullptr;
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

    static const QString kTimeFormat;

public:
    explicit Bsp(QWidget *parent = nullptr);

    /// ������������� ����������.
    void initParam();
    /// ������������� �������.
    static void initClock();

    /// ������� ����������� �����.
    static uint8_t calcCrc(pkg_t &pkg);
    /// �������� ������.
    static bool checkPkg(pkg_t &pkg, eGB_COM &com);

    void crtTreeDef();
    void crtTreeDevice();
    void crtTreeDevieVersions(QTreeWidgetItem *top);
    void crtTreeGlb();
    void crtTreeInterface();
    void crtTreeMeasure();
    void crtTreePrd();
    void crtTreePrm();
    void crtTreeState();
    void crtTreeState(QTreeWidgetItem *top, std::string name, state_t &state);
    void crtJournals();
    void crtTest();

    static QMap<eGB_PARAM, QVector<QComboBox *>> mapCombobox;
    static QMap<eGB_PARAM, QLineEdit *>          mapLineEdit;
    static QMap<eGB_PARAM, QVector<QSpinBox *>>  mapSpinBox;

    static QTextCodec *codec;  ///< �����.
    static QString     getParamName(eGB_PARAM param);

    static pkg_t pkgRx;  ///< ������ �� ������.
    static pkg_t pkgTx;  ///< ������ ��� ��������.

    static quint8 getComboBoxValue(eGB_PARAM param, uint8_t number = 1);
    static quint8 getComboBoxValue(QComboBox *combobox);
    static void   setComboBoxValue(eGB_PARAM param, quint8 value, uint8_t number = 1);
    static void   setComboBoxValueBits(eGB_PARAM param, quint8 value, uint8_t number = 1);
    static int    setComboBoxValue(QComboBox *combobox, quint8 value);

    static QString getLineEditValue(eGB_PARAM param);
    static QString getLineEditValue(QLineEdit *lineedit);
    static void    setLineEditValue(eGB_PARAM param, std::string value);
    static int     setLineEditValue(QLineEdit *lineedit, std::string value);

    static qint16 getSpinBoxValue(eGB_PARAM param, uint8_t number = 1);
    static qint16 getSpinBoxValue(QSpinBox *spinbox);
    static void   setSpinBoxValue(eGB_PARAM param, qint16 value, uint8_t number = 1);
    static int    setSpinBoxValue(QSpinBox *spinbox, qint16 value);

    static device_t  device;
    static state_t   stateDef;
    static state_t   stateGlb;
    static state_t   statePrm;
    static state_t   statePrd;
    static test_t    test;
    static measure_t m_measure;

    /// ��������� �������.
    static void procCommand(eGB_COM com, pkg_t &data);

public slots:

signals:
    void writeByte(int value);

private:
    static QDateTime *dt;  ///< ���� � �����.

    QRegExp          pwdRegExp;
    QRegExpValidator pwdValidator;
    QRegExp          errRegExp;
    QRegExpValidator errValidator;

    void crtComboBox(eGB_PARAM param);
    void fillComboboxList(QComboBox *combobox, eGB_PARAM param);
    void fillComboboxListOnOff(QComboBox *combobox);
    void fillComboboxListRegime(QComboBox *combobox);
    void fillComboboxListTypeDevice(QComboBox *combobox);
    void fillComboboxListTypeLine(QComboBox *combobox);
    void fillComboboxListTypeOpto(QComboBox *combobox);

    void crtLineEdit(eGB_PARAM param, std::string);

    void crtSpinBox(eGB_PARAM param);

    void keyPressEvent(QKeyEvent *event) override;

    /// �������������� bcd ���� � �����.
    static quint8 bcd2int(quint8 bcd, bool &ok);
    /// �������������� ������ ����� � bcd ���.
    static quint8 int2bcd(quint8 val, bool &ok);

    /// ��������� ������ ������ ��������.
    static void procCommandReadJournal(eGB_COM com, pkg_t &data);
    /// ��������� ������ ������ ����������.
    static void procCommandReadParam(eGB_COM com, pkg_t &data);
    /// ��������� ������ ������ ����������.
    static void procCommandWriteParam(eGB_COM com, pkg_t &data);
    /// ��������� ������ ������ ������.
    static void procCommandWriteRegime(eGB_COM com, pkg_t &data);

    static void hdlrComDefTypeAcGet(eGB_COM com, pkg_t &data);
    static void hdlrComDefTypeAcSet(eGB_COM com, pkg_t &data);
    static void hdlrComDeviceNumGet(eGB_COM com, pkg_t &data);
    static void hdlrComDeviceNumSet(eGB_COM com, pkg_t &data);
    static void hdlrComGetVers(eGB_COM com, pkg_t &data);
    static void hdlrComMeasGet(eGB_COM com, pkg_t &data);
    static void hdlrComNetAdrGet(eGB_COM com, pkg_t &data);
    static void hdlrComNetAdrSet(eGB_COM com, pkg_t &data);
    static void hdlrComSetControl(eGB_COM com, pkg_t &data);

    static void hdlrComGetTest(eGB_COM com, pkg_t &data);

    static uint8_t getCompatibility(eGB_TYPE_DEVICE typedevice);

private slots:
    static void setRegime(int index);
    static void setState(int index);
    static void setDopByte(int index);
    static void updateClock();
    static void updateCompatibility(int index);
};

#endif  // BSP_H
