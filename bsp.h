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

    const QString msgSizeError = "Wrong size of data: %1";
    const QString msgTimeSourceError = "Wrong source of time: %1";

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

signals:
    void debug(QString msg);

private:
    pkg_t pkgTx;        ///< Данные для передачи.
    QDateTime dt;       ///< Дата и время.
    stGBparam params;   ///< Параметры.
    QTextCodec *codec;  ///< Кодек.

    LocalParams lp;     ///< Локальный параметр.

    QRegExp pwdRegExp;
    QRegExpValidator pwdValidator;

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

    QMap<eGB_PARAM, QComboBox*> mapCombobox;
    QMap<eGB_PARAM, QLineEdit*> mapLineEdit;
    QMap<eGB_PARAM, QSpinBox*> mapSpinBox;

    void crtTree();
    void crtTreeUser();

    void crtComboBox(eGB_PARAM param, qint16 value);
    void crtLineEdit(eGB_PARAM param, std::string);
    void crtSpinBox(eGB_PARAM param, qint16 value);

    quint8 getComboBoxValue(eGB_PARAM param);
    QString getLineEditValue(eGB_PARAM param);

    void setComboBoxValue(eGB_PARAM param, quint8 value);
    void setLineEditValue(eGB_PARAM param, std::string value);

    QString getParamName(eGB_PARAM param);

    void fillComboboxList(QComboBox *combobox, eGB_PARAM param);
    void setComboboxValue(QComboBox *combobox, quint8 value);

    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateClock();

};

#endif // BSP_H
