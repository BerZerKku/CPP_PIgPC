#ifndef BSP_H
#define BSP_H

#include <QDateTime>
#include <QObject>

#include "PIg/src/protocols/standart/protocolBspS.h"

typedef QVector<uint8_t> pkg_t;

class Bsp : public QObject
{
    Q_OBJECT

    const QString msgSizeError = "��������� ���������� ���� ������ %1";
    const QString msgTimeSourceError = "��������� �������� ������ %1";

public:
    explicit Bsp(QObject *parent = nullptr);

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

signals:
    void debug(QString msg);

private:
    pkg_t pkgTx;        ///< ������ ��� ��������.
    QDateTime dt;       ///< ���� � �����.
    stGBparam params;   ///< ���������.

    /// ������� ����������� �����.
    uint8_t calcCrc(pkg_t &pkg);
    /// �������� ������.
    eGB_COM checkPkg(pkg_t &pkg);
    /// ������������� �������.
    void initClock();
    /// ������������� ����������.
    void initParam();
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

private slots:
    void updateClock();
};

#endif // BSP_H
