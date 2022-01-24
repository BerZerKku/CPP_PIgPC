#ifndef QHEX_SPINBOX_H
#define QHEX_SPINBOX_H

#include <QDebug>
#include <QSpinBox>

class QHexSpinBox : public QSpinBox
{
public:
    QHexSpinBox(bool bit8, QWidget *parent = 0) : QSpinBox(parent), m_bit8(bit8)
    {
        setDisplayIntegerBase(16);
        setRange(INT16_MIN, INT16_MAX);
    }

protected:
    QString textFromValue(int value) const override
    {
        QString text = QString::number(static_cast<unsigned int>(value), 16).toUpper();

        if (m_bit8)
        {
            text = text.mid(text.size() - 2);
        }
        else
        {
            text = text.mid(text.size() - 4);
        }

        return text;
    }

    int valueFromText(const QString &text) const override
    {
        int value = static_cast<int>(text.toUInt(0, 16));
        return value;
    }

    QValidator::State validate(QString &input, int &pos) const override
    {
        QString copy(input);

        if (copy.startsWith("0x"))
            copy.remove(0, 2);
        pos -= copy.size() - copy.trimmed().size();
        copy = copy.trimmed();

        if (copy.isEmpty())
            return QValidator::Intermediate;

        input = copy.toUpper();
        bool         okay;
        unsigned int val = copy.toUInt(&okay, 16);

        if (!okay || (val > 0xFFFF))
            return QValidator::Invalid;

        return QValidator::Acceptable;
    }

private:
    bool m_bit8;
};

#endif  // QHEX_SPINBOX_H
