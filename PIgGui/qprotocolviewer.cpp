#include <QDateTime>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "qprotocolviewer.h"

/**
 * *****************************************************************************
 *
 * @brief QProtocolViewer::QProtocolViewer
 * @param parent
 *
 * *****************************************************************************
 */
QProtocolViewer::QProtocolViewer(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    QHBoxLayout *hlayout = new QHBoxLayout();

    hlayout->addWidget(&mCommandFilterEdit);
    hlayout->addWidget(&mButtonStart);

    vlayout->addWidget(&mTextEdit);
    vlayout->addLayout(hlayout);

    connect(&mButtonStart, &QPushButton::clicked, this, &QProtocolViewer::SlotStart);
    connect(&mCommandFilterEdit,
            &QLineEdit::textChanged,
            this,
            &QProtocolViewer::SlotUpdatePattern);

    QFont font = mTextEdit.font();
    font.setFamily("Monospace");

    Start(false);
}

/**
 * *****************************************************************************
 *
 * @brief QProtocolViewer::AddRxByte
 * @param[in] byte
 *
 * *****************************************************************************
 */
void QProtocolViewer::AddRxByte(uint8_t byte)
{
    if (!mStart)
    {
        return;
    }

    if (mTxByte)
    {
        mTxByte = false;
        FilterData(mCommandRegExp);

        QString time = QTime::currentTime().toString(kTimeFormat);
        mTextEdit.append(QString("[%1] RX:").arg(time));
    }

    AppendByte(byte);
}


/**
 * *****************************************************************************
 *
 * @brief QProtocolViewer::AddTxByte
 * @param[in] byte
 *
 * *****************************************************************************
 */
void QProtocolViewer::AddTxByte(uint8_t byte)
{
    if (!mStart)
        return;

    if (!mTxByte)
    {
        mTxByte = true;
        FilterData(mCommandRegExp);

        QString time = QTime::currentTime().toString(kTimeFormat);
        mTextEdit.append(QString("\n[%1] TX:").arg(time));
    }

    AppendByte(byte);
}

/**
 * *****************************************************************************
 *
 * @brief QProtocolViewer::SetFilter
 * @param text
 *
 * *****************************************************************************
 */
void QProtocolViewer::SetPattern(const QString &text)
{
    QString pattern;

    pattern.append("[RT]X: 55 AA (");
    pattern.append(text.simplified().split(' ').join('|'));
    pattern.append(")");

    mCommandRegExp.setPattern(pattern);
}


/**
 * *****************************************************************************
 *
 * @brief QProtocolViewer::SetViewEnabled
 * @param[in] start
 *
 * *****************************************************************************
 */
void QProtocolViewer::Start(bool start)
{
    mStart = start;
    mButtonStart.setText(mStart ? "Stop" : "Start");

    if (mStart)
    {
        mTextEdit.clear();
    }
}


/**
 * *****************************************************************************
 *
 * @brief QProtocolViewer::GetTextFromByte
 * @param[in] byte
 *
 * *****************************************************************************
 */
void QProtocolViewer::AppendByte(uint8_t byte)
{
    mTextEdit.insertPlainText(" " + ConvertByte(byte));
}


/**
 * *****************************************************************************
 *
 * @brief QProtocolViewer::ConvertByte
 * @param[in] byte
 * @return
 *
 * *****************************************************************************
 */
QString QProtocolViewer::ConvertByte(uint8_t byte)
{
    return QString("%1").arg(byte, 2, 16, QLatin1Char('0')).toUpper();
}


/**
 * *****************************************************************************
 *
 * @brief QProtocolViewer::FilterData
 * @param[in] commands
 *
 * *****************************************************************************
 */
void QProtocolViewer::FilterData(const QRegExp &reg_exp)
{
    QStringList text = mTextEdit.toPlainText().split("\n").filter(reg_exp);
    text.removeAll("");

    mTextEdit.clear();
    for (auto &line : text)
    {
        mTextEdit.append(line);

        if (line.count("RX:") == 1)
        {
            mTextEdit.append("");
        }
    }
}


/**
 * *****************************************************************************
 *
 * @brief QProtocolViewer::SlotViewEnabled
 *
 * *****************************************************************************
 */
void QProtocolViewer::SlotStart()
{
    Start(!mStart);
}


/**
 * *****************************************************************************
 *
 * @brief QProtocolViewer::SlotGetFilterString
 * @param text
 *
 * *****************************************************************************
 */
void QProtocolViewer::SlotUpdatePattern(const QString &text)
{
    SetPattern(text);
}
