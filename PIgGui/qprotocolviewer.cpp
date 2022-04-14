#include "qprotocolviewer.h"
#include <QDateTime>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QValidator>

/**
 * *****************************************************************************
 *
 * @brief QProtocolViewer::QProtocolViewer
 * @param parent
 *
 * @todo Перенести работу протокола в отдельный поток.
 *
 * *****************************************************************************
 */
QProtocolViewer::QProtocolViewer(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    QHBoxLayout *hlayout = new QHBoxLayout();

    hlayout->addWidget(&mCommandFilterEdit);
    hlayout->addWidget(&mButtonClear);
    hlayout->addWidget(&mButtonStart);

    vlayout->addWidget(&mTextEdit);
    vlayout->addLayout(hlayout);

    connect(&mButtonClear, &QPushButton::clicked, this, &QProtocolViewer::SlotClear);
    connect(&mButtonStart, &QPushButton::clicked, this, &QProtocolViewer::SlotStart);
    connect(&mCommandFilterEdit,
            &QLineEdit::textChanged,
            this,
            &QProtocolViewer::SlotUpdatePattern);

    mButtonClear.setText("Clear");

    // HEX строка
    QRegExp hex_reg("^[A-Fa-f0-9]{2}( [A-Fa-f0-9]{2})*");
    mCommandFilterEdit.setValidator(new QRegExpValidator(hex_reg));

    QFont font = mTextEdit.font();
    font.setFamily("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    mTextEdit.setFont(font);

    // Программа не запускается (Windows) если mTimer сделать не укзаателем
    mTimer.setInterval(25);
    mTimer.setSingleShot(true);
    mTimer.connect(&mTimer, &QTimer::timeout, this, &QProtocolViewer::SlotTimerTimeout);

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
        UpdateText();
    }

    if (!mRxByte || mData.size() == 0)
    {
        mRxByte = true;

        QString time = QTime::currentTime().toString(kTimeFormat);
        mData.append(QString("[%1] RX:").arg(time));
    }

    AppendByte(byte);
    mTimer.start();
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

    if (mRxByte)
    {
        mRxByte = false;
        UpdateText();
    }

    if (!mTxByte || mData.size() == 0)
    {
        mTxByte = true;

        QString time = QTime::currentTime().toString(kTimeFormat);
        mData.append(QString("[%1] TX:").arg(time));
    }

    AppendByte(byte);
    mTimer.start();
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
    mCommandFilterEdit.setText(text);
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
    mTimer.stop();

    if (mStart)
    {
        mTxByte = false;
        mRxByte = false;
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
    mData.last().append(" " + ConvertByte(byte));
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
 * @brief QProtocolViewer::UpdateText
 *
 * *****************************************************************************
 */
void QProtocolViewer::UpdateText()
{
    int length = mData.length();

    QStringList text = mData.mid(mDataLengthLast).filter(mCommandRegExp);
    mDataLengthLast  = length;

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
 * @brief QProtocolViewer::SlotClear
 *
 * @fixme Возможны проблемы при очистке в то время как туда осуществляется запись.
 * *****************************************************************************
 */
void QProtocolViewer::SlotClear()
{
    mData.clear();
    mTextEdit.clear();
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
    QString pattern;

    pattern.append("[RT]X: 55 AA (");
    pattern.append(text.simplified().split(' ').join('|'));
    pattern.append(")");

    mCommandRegExp.setPattern(pattern);

    // при смене фильтра обновить информацию на экране
    mTextEdit.clear();
    mDataLengthLast = -1;
    UpdateText();
}

void QProtocolViewer::SlotTimerTimeout()
{
    mTxByte = false;
    mRxByte = false;

    UpdateText();
}
