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
    };

    struct test_t
    {
        QLineEdit *byte1;
        QLineEdit *byte2;
        QLineEdit *byte3;
        QLineEdit *byte4;
        QLineEdit *byte5;
    };

    static const QString kTimeFormat;

public:
    explicit Bsp(QWidget *parent = nullptr);

    /// Инициализация параметров.
    void initParam();
    /// Инициализация времени.
    static void initClock();

    /// Подсчет контрольной суммы.
    static uint8_t calcCrc(pkg_t &pkg);
    /// Проверка пакета.
    static bool checkPkg(pkg_t &pkg, eGB_COM &com);

    void crtTreeDevice();
    void crtTreeGlb();
    void crtTreeInterface();
    void crtTreePrd();
    void crtTreePrm();
    void crtTreeState();
    void crtTreeState(QTreeWidgetItem *top, std::string name, state_t &state);
    void crtJournals();
    void crtTest();

    static QMap<eGB_PARAM, QVector<QComboBox *>> mapCombobox;
    static QMap<eGB_PARAM, QLineEdit *>          mapLineEdit;
    static QMap<eGB_PARAM, QVector<QSpinBox *>>  mapSpinBox;

    static QTextCodec *codec;  ///< Кодек.
    static QString     getParamName(eGB_PARAM param);

    static pkg_t pkgRx;  ///< Данные на приеме.
    static pkg_t pkgTx;  ///< Данные для передачи.

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

    static device_t device;
    static state_t  stateDef;
    static state_t  stateGlb;
    static state_t  statePrm;
    static state_t  statePrd;
    static test_t   test;

    /// Обработка команды.
    static void procCommand(eGB_COM com, pkg_t &data);

public slots:

signals:
    void writeByte(int value);

private:
    static QDateTime *dt;  ///< Дата и время.

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

    /// Преобразование bcd кода в целое.
    static quint8 bcd2int(quint8 bcd, bool &ok);
    /// Преобразование целого числа в bcd код.
    static quint8 int2bcd(quint8 val, bool &ok);

    /// Обработка команд чтения журналов.
    static void procCommandReadJournal(eGB_COM com, pkg_t &data);
    /// Обработка команд чтения параметров.
    static void procCommandReadParam(eGB_COM com, pkg_t &data);
    /// Обработка команд записи параметров.
    static void procCommandWriteParam(eGB_COM com, pkg_t &data);
    /// Обработка команд записи режима.
    static void procCommandWriteRegime(eGB_COM com, pkg_t &data);

    static void hdlrComControlSet(eGB_COM com, pkg_t &data);
    static void hdlrComFreqGet(eGB_COM com, pkg_t &data);
    static void hdlrComFreqSet(eGB_COM com, pkg_t &data);
    static void hdlrComNetAdrGet(eGB_COM com, pkg_t &data);
    static void hdlrComNetAdrSet(eGB_COM com, pkg_t &data);
    static void hdlrComPrdKeepGet(eGB_COM com, pkg_t &data);
    static void hdlrComPrdKeepSet(eGB_COM com, pkg_t &data);
    static void hdlrComTestGet(eGB_COM com, pkg_t &data);
    static void hdlrComVersGet(eGB_COM com, pkg_t &data);

    static uint8_t getCompatibility(eGB_TYPE_DEVICE typedevice);

private slots:
    static void setRegime(int index);
    static void setState(int index);
    static void setDopByte(int index);
    static void updateClock();
    static void updateCompatibility(int index);
};

#endif  // BSP_H
