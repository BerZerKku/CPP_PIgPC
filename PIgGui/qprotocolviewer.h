#ifndef QPROTOCOLVIEWER_H
#define QPROTOCOLVIEWER_H

#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>

class QProtocolViewer : public QWidget
{
    Q_OBJECT

    using pkg_t = QVector<uint8_t>;

    const QString kHexRegEx   = "^[A-Fa-f0-9]{2}( [A-Fa-f0-9]{2})*";  // например "12 f1 8F"
    const QString kTimeFormat = "HH:mm:ss.zzz";

public:
    explicit QProtocolViewer(QWidget *parent = nullptr);

    void AddRxByte(uint8_t byte);
    void AddTxByte(uint8_t byte);
    void SetPattern(const QString &text);

private:
    bool mStart  = false;
    bool mTxByte = true;

    QTextEdit   mTextEdit;
    QPushButton mButtonStart;
    QRegExp     mCommandRegExp;
    QLineEdit   mCommandFilterEdit;

    void    Start(bool start);
    void    AppendByte(uint8_t byte);
    QString ConvertByte(uint8_t byte);
    void    FilterData(const QRegExp &reg_exp);

private slots:
    void SlotStart();
    void SlotUpdatePattern(const QString &text);
};

#endif  // QPROTOCOLVIEWER_H
