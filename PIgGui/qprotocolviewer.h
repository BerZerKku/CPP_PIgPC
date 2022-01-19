#ifndef QPROTOCOLVIEWER_H
#define QPROTOCOLVIEWER_H

#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>
#include <QWidget>

class QProtocolViewer : public QWidget
{
    Q_OBJECT

    const QString kHexRegEx   = "^[A-Fa-f0-9]{2}( [A-Fa-f0-9]{2})*";  // например "12 f1 8F"
    const QString kTimeFormat = "HH:mm:ss.zzz";

public:
    explicit QProtocolViewer(QWidget *parent = nullptr);

    void AddRxByte(uint8_t byte);
    void AddTxByte(uint8_t byte);
    void SetPattern(const QString &text);

private:
    bool mStart  = false;
    bool mTxByte = false;
    bool mRxByte = false;

    int         mDataLengthLast = -1;
    QStringList mData;

    QTextEdit   mTextEdit;
    QPushButton mButtonStart;
    QPushButton mButtonClear;
    QRegExp     mCommandRegExp;
    QLineEdit   mCommandFilterEdit;
    QTimer      mTimer;

    void    Start(bool start);
    void    AppendByte(uint8_t byte);
    QString ConvertByte(uint8_t byte);
    void    UpdateText();

private slots:
    void SlotClear();
    void SlotStart();
    void SlotUpdatePattern(const QString &text);
    void SlotTimerTimeout();
};

#endif  // QPROTOCOLVIEWER_H
