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

    const int k_timer_tick_ms = 100;

protected:
    typedef void (Bsp::*HdlrCom_t)(uint8_t com, pkg_t &data);

    static const QString kMsgSizeError;
    static const QString kMsgTimeSourceError;
    static const QString kTimeFormat;
    static const QString kVersion;

public:
    explicit Bsp(QTreeWidget *tree, QWidget *parent = nullptr);

    /// Подсчет контрольной суммы.
    uint8_t calcCrc(pkg_t &pkg);
    /// Проверка пакета.
    bool checkPkg(pkg_t &pkg, uint8_t &com);

    void Init();

    pkg_t m_pkg_rx;  ///< Данные на приеме.
    pkg_t m_pkg_tx;  ///< Данные для передачи.

    void AddWidget(QTreeWidgetItem *top, const QString &name, QWidget *widget);


public slots:
    void SlotReadByte(int value);
    void SlotStart(bool start);

signals:
    void SignalWriteByte(int value);
    void SignalSendFinished();

protected:
    QSpinBox             m_key;
    QLineEdit            m_version;
    QVector<QLineEdit *> m_lines;
    QSpinBox             m_top_lines;
    QComboBox            m_led_on;
    QComboBox            m_cursor_on;
    QSpinBox             m_cursor_pos;

    QTreeWidget *const m_tree;

    QMap<uint8_t, HdlrCom_t> m_map_com_rx;
    QMap<uint8_t, HdlrCom_t> m_map_com_tx;

    QDateTime dt;  ///< Дата и время.

    QRegExp          pwdRegExp;
    QRegExpValidator pwdValidator;
    QRegExp          errRegExp;
    QRegExpValidator errValidator;
    QTimer           m_timer;

    void crtTreeParam();

    void keyPressEvent(QKeyEvent *event) override;

    /// Преобразование bcd кода в целое.
    quint8 bcd2int(quint8 bcd, bool *ok = nullptr);
    /// Преобразование целого числа в bcd код.
    quint8 int2bcd(quint8 val, bool *ok = nullptr);

    void procCommand(uint8_t com, pkg_t &data);
    void procCommandWrite(uint8_t com, pkg_t &data);

    bool CheckSize(uint8_t com, int size, QVector<int> size_allowed);

    void HdlrComDummy(uint8_t com, pkg_t &data);

    void HdlrComRx01(uint8_t com, pkg_t &data);
    void HdlrComTx11(uint8_t com, pkg_t &data);

protected slots:
    void SlotReadPackageFinished();
};

#endif  // BSP_H
