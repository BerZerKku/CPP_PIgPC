#ifndef BSP_H
#define BSP_H

#include <QComboBox>

#include <QDateTime>
#include <QLineEdit>
#include <QMap>
#include <QRegExp>
#include <QSpinBox>
#include <QString>
#include <QStringList>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "PIg/src/glbDefine.h"
#include "PIg/src/security/user.h"
#include "PIg/src/security/securityevent.h"

typedef QVector<uint8_t> pkg_t;

class Bsp : public QTreeWidget
{
    Q_OBJECT

    static const QString msgSizeError;
    static const QString msgTimeSourceError;

    struct state_t {
        QComboBox *regime = nullptr;
        QSpinBox *state = nullptr;
        QSpinBox *dopByte = nullptr;
        QLineEdit *warning = nullptr;
        QLineEdit *fault = nullptr;
    };

    struct device_t {
        QComboBox *isDef = nullptr;
        QComboBox *typeDevice = nullptr;
        QComboBox *typeLine = nullptr;
        QComboBox *typeOpto = nullptr;
    };

    struct jrn_t {
        quint16 cntSecurity = 0;
        quint16 posSecurity = SIZE_OF_SECURITY_JRN - 1;
        QTreeWidgetItem *security = nullptr;
        QTreeWidget *securityTw = nullptr;
    };

    struct test_t {
        QLineEdit* byte1;
        QLineEdit* byte2;
        QLineEdit* byte3;
        QLineEdit* byte4;
        QLineEdit* byte5;
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

    void crtTreeDevice();
    void crtTreeGlb();
    void crtTreeInterface();
    void crtTreePrd();
    void crtTreePrm();
    void crtTreeState();
    void crtTreeState(QTreeWidgetItem *top, std::string name, state_t &state);
    void crtTreeUser();
    void crtJournals();
    void crtTest();

    static QMap<eGB_PARAM, QVector<QComboBox*>> mapCombobox;
    static QMap<eGB_PARAM, QLineEdit*> mapLineEdit;
    static QMap<eGB_PARAM, QVector<QSpinBox*>> mapSpinBox;

    static QTextCodec *codec;  ///< �����.
    static QString getParamName(eGB_PARAM param);

    static pkg_t pkgRx; ///< ������ �� ������.
    static pkg_t pkgTx; ///< ������ ��� ��������.

    static quint8 getComboBoxValue(eGB_PARAM param, uint8_t number=1);
    static quint8 getComboBoxValue(QComboBox *combobox);
    static void setComboBoxValue(eGB_PARAM param, quint8 value, uint8_t number=1);
    static void setComboBoxValueBits(eGB_PARAM param, quint8 value, uint8_t number=1);
    static int setComboBoxValue(QComboBox *combobox, quint8 value);

    static QString getLineEditValue(eGB_PARAM param);
    static QString getLineEditValue(QLineEdit *lineedit);
    static void setLineEditValue(eGB_PARAM param, std::string value);
    static int setLineEditValue(QLineEdit *lineedit, std::string value);

    static qint16 getSpinBoxValue(eGB_PARAM param, uint8_t number=1);
    static qint16 getSpinBoxValue(QSpinBox *spinbox);
    static void setSpinBoxValue(eGB_PARAM param, qint16 value, uint8_t number=1);
    static int setSpinBoxValue(QSpinBox *spinbox, qint16 value);

    static device_t device;
    static state_t stateDef;
    static state_t stateGlb;
    static state_t statePrm;
    static state_t statePrd;
    static jrn_t journals;
    static test_t test;

    /// ��������� �������.
    static void procCommand(eGB_COM com, pkg_t &data);

public slots:

signals:
    void writeByte(int value);

private:
    static QDateTime *dt;   ///< ���� � �����.

    QRegExp pwdRegExp;
    QRegExpValidator pwdValidator;
    QRegExp errRegExp;
    QRegExpValidator errValidator;

    static void addJSEntry(user_t user, userSrc_t src, TSecurityEvent::event_t event);
    QTreeWidgetItem* crtJSEntry(quint16 index);

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

    static void hdlrComGetVers(eGB_COM com, pkg_t &data);
    static void hdlrComNetAdrGet(eGB_COM com, pkg_t &data);
    static void hdlrComNetAdrSet(eGB_COM com, pkg_t &data);
    static void hdlrComSetControl(eGB_COM com, pkg_t &data);

    static void hdlrComJrnIsClr(eGB_COM com, pkg_t &data);
    static void hdlrComGetJrnIsCnt(eGB_COM com, pkg_t &data);
    static void hdlrComGetJrnIsEntry(eGB_COM com, pkg_t &data);
    static void hdlrComGetJrnIsSetEntry(eGB_COM com, pkg_t &data);

    static void hdlrComGetTest(eGB_COM com, pkg_t &data);

private slots:
    static void setRegime(int index);
    static void setState(int index);
    static void setDopByte(int index);
    static void updateClock();
};

#endif // BSP_H
