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

    /** �������� ����� ������ � ���.
     *
     *  @param[in] pkg ������.
     */
    void sendToBsp(pkg_t pkg);

    /** �������� ����� ������ �� ���
     *
     *  @return ������.
     */
    pkg_t receiveFromBsp();

    /// ������������� ����������.
    void initParam();

public slots:
    void setUser(int value);

signals:
    void debug(QString msg);
    void userChanged(int value);

private:
    pkg_t pkgTx;        ///< ������ ��� ��������.
    QDateTime dt;       ///< ���� � �����.
    stGBparam params;   ///< ���������.
    QTextCodec *codec;  ///< �����.

    QRegExp pwdRegExp;
    QRegExpValidator pwdValidator;
    QRegExp errRegExp;
    QRegExpValidator errValidator;

    QMap<eGB_PARAM, QVector<QComboBox*>> mapCombobox;
    QMap<eGB_PARAM, QLineEdit*> mapLineEdit;
    QMap<eGB_PARAM, QVector<QSpinBox*>> mapSpinBox;

    state_t stateDef;
    state_t stateGlb;
    state_t statePrm;
    state_t statePrd;

    QVector<eGB_COM> viewCom;

    QComboBox regime;

    /// ������� ����������� �����.
    uint8_t calcCrc(pkg_t &pkg);
    /// �������� ������.
    eGB_COM checkPkg(pkg_t &pkg);
    /// ������������� �������.
    void initClock();
    /// ������������� �������.
    void initDebug();

    /// ��������� �������.
    void procCommand(eGB_COM com, pkg_t &data);
    /// ��������� ������ ������ ��������.
    void procCommandReadJournal(eGB_COM com, pkg_t &data);
    /// ��������� ������ ������ ����������.
    void procCommandReadParam(eGB_COM com, pkg_t &data);
    /// ��������� ������ ������ ����������.
    void procCommandWriteParam(eGB_COM com, pkg_t &data);
    /// ��������� ������ ������ ������.
    void procCommandWriteRegime(eGB_COM com, pkg_t &data);

    /// �������������� bcd ���� � �����.
    quint8 bcd2int(quint8 bcd, bool &ok) const;
    /// �������������� ������ ����� � bcd ���.
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
    quint8 getComboBoxValue(eGB_PARAM param, uint8_t number=1);
    quint8 getComboBoxValue(QComboBox *combobox);
    void setComboBoxValue(eGB_PARAM param, quint8 value, uint8_t number=1);
    void setComboBoxValueBits(eGB_PARAM param, quint8 value, uint8_t number=1);
    int setComboBoxValue(QComboBox *combobox, quint8 value);

    void crtLineEdit(eGB_PARAM param, std::string);
    QString getLineEditValue(eGB_PARAM param);
    QString getLineEditValue(QLineEdit *lineedit);
    void setLineEditValue(eGB_PARAM param, std::string value);
    int setLineEditValue(QLineEdit *lineedit, std::string value);

    void crtSpinBox(eGB_PARAM param);
    qint16 getSpinBoxValue(eGB_PARAM param, uint8_t number=1);
    qint16 getSpinBoxValue(QSpinBox *spinbox);
    void setSpinBoxValue(eGB_PARAM param, qint16 value, uint8_t number=1);
    int setSpinBoxValue(QSpinBox *spinbox, qint16 value);

    QString getParamName(eGB_PARAM param);

    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateClock();
    void setRegime(int index);
    void setState(int index);
    void setDopByte(int index);
    void setLocalValues(int value);

    void hdlrComNetAdrGet(eGB_COM com, pkg_t data);
    void hdlrComNetAdrSet(eGB_COM com, pkg_t data);
};

#endif // BSP_H
