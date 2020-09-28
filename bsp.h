#ifndef BSP_H
#define BSP_H

#include "PIg/src/protocols/standart/protocolBspS.h"
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

typedef QVector<uint8_t> pkg_t;

class Bsp : public QTreeWidget
{
    Q_OBJECT

    const QString msgSizeError = "Wrong size of data in command %1: %2";
    const QString msgTimeSourceError = "Wrong source of time: %1";

    struct state_t {
        QComboBox regime;
        QSpinBox state;
        QSpinBox dopByte;
        QLineEdit warning;
        QLineEdit fault;
    };

public:
    explicit Bsp(QWidget *parent = nullptr);

    /** Передает пакет данных в БСП.
     *
     *  @param[in] pkg Данные.
     */
    void sendToBsp(pkg_t pkg);

    /** Получает пакет данных из БСП
     *
     *  @return Данные.
     */
    pkg_t receiveFromBsp();

    /// Инициализация параметров.
    void initParam();

public slots:
    void setUser(int value);

signals:
    void debug(QString msg);
    void userChanged(int value);

private:
    pkg_t pkgTx;        ///< Данные для передачи.
    QDateTime dt;       ///< Дата и время.
    stGBparam params;   ///< Параметры.
    QTextCodec *codec;  ///< Кодек.

    QRegExp pwdRegExp;
    QRegExpValidator pwdValidator;
    QRegExp errRegExp;
    QRegExpValidator errValidator;

    QMap<eGB_PARAM, QComboBox*> mapCombobox;
    QMap<eGB_PARAM, QLineEdit*> mapLineEdit;
    QMap<eGB_PARAM, QSpinBox*> mapSpinBox;

    state_t stateDef;
    state_t stateGlb;
    state_t statePrm;
    state_t statePrd;

    QComboBox regime;

    /// Подсчет контрольной суммы.
    uint8_t calcCrc(pkg_t &pkg);
    /// Проверка пакета.
    eGB_COM checkPkg(pkg_t &pkg);
    /// Инициализация времени.
    void initClock();

    /// Обработка команды.
    void procCommand(eGB_COM com, pkg_t &data);
    /// Обработка команд чтения журналов.
    void procCommandReadJournal(eGB_COM com, pkg_t &data);
    /// Обработка команд чтения параметров.
    void procCommandReadParam(eGB_COM com, pkg_t &data);
    /// Обработка команд записи параметров.
    void procCommandWriteParam(eGB_COM com, pkg_t &data);
    /// Обработка команд записи режима.
    void procCommandWriteRegime(eGB_COM com, pkg_t &data);

    /// Преобразование bcd кода в целое.
    quint8 bcd2int(quint8 bcd, bool &ok) const;
    /// Преобразование целого числа в bcd код.
    quint8 int2bcd(quint8 val, bool &ok) const;

    void crtTree();
    void crtTreeGlb();
    void crtTreeInterface();
    void crtTreePrd();
    void crtTreePrm();
    void crtTreeState();
    void crtTreeState(QTreeWidgetItem *top, std::string name, state_t &state);
    void crtTreeUser();

    void crtComboBox(eGB_PARAM param);
    void fillComboboxList(QComboBox *combobox, eGB_PARAM param);
    void fillComboboxListRegime(QComboBox *combobox);
    quint8 getComboBoxValue(eGB_PARAM param);
    quint8 getComboBoxValue(QComboBox *combobox);
    void setComboBoxValue(eGB_PARAM param, quint8 value);
    int setComboBoxValue(QComboBox *combobox, quint8 value);

    void crtLineEdit(eGB_PARAM param, std::string);
    QString getLineEditValue(eGB_PARAM param);
    QString getLineEditValue(QLineEdit *lineedit);
    void setLineEditValue(eGB_PARAM param, std::string value);
    int setLineEditValue(QLineEdit *lineedit, std::string value);

    void crtSpinBox(eGB_PARAM param);
    qint16 getSpinBoxValue(eGB_PARAM param);
    qint16 getSpinBoxValue(QSpinBox *spinbox);
    void setSpinBoxValue(eGB_PARAM param, qint16 value);
    int setSpinBoxValue(QSpinBox *spinbox, qint16 value);

    QString getParamName(eGB_PARAM param);

    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateClock();
    void setRegime(int index);
    void setState(int index);
    void setDopByte(int index);
    void setLocalValues(int value);
};

#endif // BSP_H
