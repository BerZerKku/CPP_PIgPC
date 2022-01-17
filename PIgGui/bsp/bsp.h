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
#include <QTimer>
#include <QTreeWidget>
#include <QTreeWidgetItem>

typedef QVector<uint8_t> pkg_t;

class Bsp : public QTreeWidget
{
    Q_OBJECT

    static const QString     kMsgSizeError;
    static const QString     kMsgTimeSourceError;
    static const QTextCodec *kCodec;
    static const QString     kTimeFormat;

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

public:
    explicit Bsp(QWidget *parent = nullptr);

    /// Инициализация параметров.
    void initParam();
    /// Инициализация времени.
    void initClock();

    /// Подсчет контрольной суммы.
    uint8_t calcCrc(pkg_t &pkg);
    /// Проверка пакета.
    bool checkPkg(pkg_t &pkg, eGB_COM &com);

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

    QMap<eGB_PARAM, QVector<QComboBox *>> mapCombobox;
    QMap<eGB_PARAM, QLineEdit *>          mapLineEdit;
    QMap<eGB_PARAM, QVector<QSpinBox *>>  mapSpinBox;

    QString getParamName(eGB_PARAM param);

    pkg_t mPkgRx;  ///< Данные на приеме.
    pkg_t mPkgTx;  ///< Данные для передачи.

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
    qint16 getSpinBoxValue(QSpinBox *spinbox);
    void   setSpinBoxValue(eGB_PARAM param, qint16 value, uint8_t number = 1);
    int    setSpinBoxValue(QSpinBox *spinbox, qint16 value);

    device_t  mDevice;
    state_t   stateDef;
    state_t   stateGlb;
    state_t   statePrm;
    state_t   statePrd;
    test_t    test;
    measure_t m_measure;

    /// Обработка команды.
    void procCommand(eGB_COM com, pkg_t &data);

public slots:
    void SlotReadByte(int value);

signals:
    void SignalWriteByte(int value);
    void SignalSendFinished();

private:
    QDateTime dt;  ///< Дата и время.

    QRegExp          pwdRegExp;
    QRegExpValidator pwdValidator;
    QRegExp          errRegExp;
    QRegExpValidator errValidator;
    QTimer           mTimerSignalSendFinished;

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

    /// Преобразование bcd кода в целое.
    quint8 bcd2int(quint8 bcd, bool &ok);
    /// Преобразование целого числа в bcd код.
    quint8 int2bcd(quint8 val, bool &ok);

    /// Обработка команд чтения журналов.
    void procCommandReadJournal(eGB_COM com, pkg_t &data);
    /// Обработка команд чтения параметров.
    void procCommandReadParam(eGB_COM com, pkg_t &data);
    /// Обработка команд записи параметров.
    void procCommandWriteParam(eGB_COM com, pkg_t &data);
    /// Обработка команд записи режима.
    void procCommandWriteRegime(eGB_COM com, pkg_t &data);

    void hdlrComDefTypeAcGet(eGB_COM com, pkg_t &data);
    void hdlrComDefTypeAcSet(eGB_COM com, pkg_t &data);
    void hdlrComDeviceNumGet(eGB_COM com, pkg_t &data);
    void hdlrComDeviceNumSet(eGB_COM com, pkg_t &data);
    void hdlrComGetVers(eGB_COM com, pkg_t &data);
    void hdlrComMeasGet(eGB_COM com, pkg_t &data);
    void hdlrComNetAdrGet(eGB_COM com, pkg_t &data);
    void hdlrComNetAdrSet(eGB_COM com, pkg_t &data);
    void hdlrComSetControl(eGB_COM com, pkg_t &data);

    void hdlrComGetTest(eGB_COM com, pkg_t &data);

    uint8_t getCompatibility(eGB_TYPE_DEVICE typedevice);

private slots:
    void setRegime(int index);
    void setState(int index);
    void setDopByte(int index);
    void updateClock();
    void updateCompatibility(int index);
    void SlotReadPackageFinished();
};

#endif  // BSP_H
