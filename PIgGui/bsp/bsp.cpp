#include <QDebug>
#include <QHeaderView>
#include <QTextCodec>
#include <cmath>

#include "bsp.h"
#include "menu/menu.h"
#include "unicode.hpp"

const QString Bsp::kMsgSizeError       = "Wrong size of data in command %1: %2";
const QString Bsp::kMsgTimeSourceError = "Wrong source of time: %1";
const QString Bsp::kTimeFormat         = "dd.MM.yyyy hh:mm:ss.zzz";

const QString Bsp::kVersion = "%1.%2";


/**
 * *****************************************************************************
 *
 * @brief �����������.
 * @param[in] tree ������.
 * @param[in] parent ��������.
 *
 * *****************************************************************************
 */
Bsp::Bsp(QTreeWidget *tree, QWidget *parent) : QWidget(parent), m_tree(tree)
{
    // ��� ������ �� ������ �� ���������� ���������, �� ������ �� resize ����.
    m_tree->headerItem()->setText(0, ToUnicode("Parameter"));
    m_tree->headerItem()->setText(1, ToUnicode("Value"));

    //    expandAll();
    //    header()->resizeSections(QHeaderView::ResizeToContents);
    //    header()->setSectionResizeMode(0, QHeaderView::Fixed);
    //    header()->resizeSection(0, header()->sectionSize(0) + 5);
    //    setFixedWidth(1.75*header()->sectionSize(0));

    m_tree->header()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_tree->header()->resizeSection(0, 220);
    m_tree->setFixedWidth(400);

    m_tree->setSelectionMode(QAbstractItemView::NoSelection);
    m_tree->setFocusPolicy(Qt::NoFocus);

    m_tree->clear();
}


/**
 * *****************************************************************************
 *
 * @brief ������� ����� ���������.
 *
 * *****************************************************************************
 */
void Bsp::crtTreeParam()
{
    QTreeWidgetItem *top;

    // ������

    top = new QTreeWidgetItem(m_tree);
    top->setText(0, ToUnicode("������"));
    m_tree->addTopLevelItem(top);

    m_key.setRange(0, 0xFFFF);
    m_key.setPrefix("0x");
    m_key.setDisplayIntegerBase(16);
    m_key.setReadOnly(true);
    m_key.setFocusPolicy(Qt::NoFocus);
    m_key.findChild<QLineEdit *>()->setReadOnly(true);
    QLineEdit *line = m_key.findChild<QLineEdit *>();
    connect(line, &QLineEdit::selectionChanged, line, &QLineEdit::deselect);
    AddWidget(top, ToUnicode("����������"), &m_key);

    m_version.setReadOnly(true);
    m_version.setFocusPolicy(Qt::NoFocus);
    connect(&m_version, &QLineEdit::selectionChanged, &m_version, &QLineEdit::deselect);
    AddWidget(top, ToUnicode("������"), &m_version);

    // ������

    top = new QTreeWidgetItem(m_tree);
    top->setText(0, ToUnicode("������"));
    m_tree->addTopLevelItem(top);

    for (int i = 0; i < (SIZE_BUF_STRING / DISPLAY_ROW_SIZE); i++)
    {
        QLineEdit *line = new QLineEdit;
        QFont      font = line->font();

        font.setFamily("Monospace");
        font.setStyleHint(QFont::TypeWriter);

        line->setFont(font);
        line->setMaxLength(DISPLAY_ROW_SIZE);

        m_lines.append(line);
        AddWidget(top, ToUnicode("����� %1").arg(i + 1), line);
    }

    m_top_lines.setRange(1, m_lines.size() - 1);
    AddWidget(top, ToUnicode("���������� top �����"), &m_top_lines);

    m_led_on.addItem(ToUnicode("0 - ����"), 0);
    m_led_on.addItem(ToUnicode("1 - ���"), 1);
    m_led_on.setEditable(false);
    AddWidget(top, ToUnicode("���������"), &m_led_on);

    m_cursor_on.addItem(ToUnicode("0 - ����"), 0);
    m_cursor_on.addItem(ToUnicode("1 - ���"), 1);
    m_cursor_on.setEditable(false);
    AddWidget(top, ToUnicode("������"), &m_cursor_on);

    m_cursor_pos.setRange(1, SIZE_BUF_STRING);
    AddWidget(top, ToUnicode("������� �������"), &m_cursor_pos);

    m_tree->expandAll();
}


//
uint8_t Bsp::calcCrc(QVector<uint8_t> &pkg)
{
    uint8_t crc = 0;

    for (uint8_t byte : pkg)
    {
        crc += byte;
    }

    return crc;
}

//
bool Bsp::checkPkg(QVector<uint8_t> &pkg, uint8_t &com)
{
    uint8_t byte;

    byte = pkg.takeFirst();
    if (byte != 0x55)
    {
        return false;
    }

    byte = pkg.takeFirst();
    if (byte != 0xAA)
    {
        return false;
    }

    byte = pkg.takeLast();
    if (byte != calcCrc(pkg))
    {
        return false;
    }

    com = pkg.takeFirst();

    byte = pkg.takeFirst();
    if (byte != pkg.size())
    {
        return false;
    }

    return true;
}


/**
 * *****************************************************************************
 *
 * @brief ������� ������ ���������� � ���������� ��������� ���������.
 *
 * *****************************************************************************
 */
void Bsp::Init()
{
    crtTreeParam();

    m_map_com_rx.insert(0x01, &Bsp::HdlrComRx01);

    m_map_com_tx.insert(0x11, &Bsp::HdlrComTx11);
}


/**
 * *****************************************************************************
 *
 * @brief �������� ������.
 * @param[in] top ��������.
 * @param[in] name �������� ���������
 * @param[in] widget ������ ���������.
 *
 * *****************************************************************************
 */
void Bsp::AddWidget(QTreeWidgetItem *top, const QString &name, QWidget *widget)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(top);
    item->setText(0, name);
    top->addChild(item);

    m_tree->setItemWidget(item, 1, widget);
}


/**
 * *****************************************************************************
 *
 * @brief �������� ���������� ��� �������� �������.
 * @param[in] com �������.
 * @param[in] data ������.
 *
 * *****************************************************************************
 */
void Bsp::procCommand(uint8_t com, pkg_t &data)
{
    HdlrCom_t hdlr;

    m_pkg_tx.clear();

    hdlr = m_map_com_rx.value(com, &Bsp::HdlrComDummy);
    (this->*hdlr)(com, data);

    if (com == 0x01)
    {
        com  = 0x11;
        hdlr = m_map_com_tx.value(com, &Bsp::HdlrComDummy);
        (this->*hdlr)(com, data);
        procCommandWrite(com, data);
    }
}


/**
 * *****************************************************************************
 *
 * @brief �������� ������.
 * @param[in] com �������.
 * @param[in] data ������.
 *
 * *****************************************************************************
 */
void Bsp::procCommandWrite(uint8_t com, pkg_t &data)
{
    m_pkg_tx.clear();

    if (com == 0x11)
    {
        m_pkg_tx.append(data.takeFirst());
        m_pkg_tx.append(static_cast<uint8_t>(data.size()));
        m_pkg_tx.append(data);
        m_pkg_tx.append(calcCrc(m_pkg_tx));

        m_pkg_tx.prepend(0xAA);
        m_pkg_tx.prepend(0x55);

        for (int i = 0; i < m_pkg_tx.size(); i++)
        {
            emit SignalWriteByte(m_pkg_tx.at(i));
        }
    }
}


/**
 * *****************************************************************************
 *
 * @brief ���� ��������� ��������� �����.
 * @param[in] value �������� ���� ������.
 *
 * *****************************************************************************
 */
void Bsp::SlotReadByte(int value)
{
    static uint8_t len = 0;

    // @fixme �������� ����� �� �������� ����.
    // @fixme ����� ������� ��������� �������� ������ �� ��� ����� ���������� �����!
    emit SignalSendFinished();

    switch (m_pkg_rx.size())
    {
    case 0:
        {
            if (value == 0x55)
            {
                m_pkg_rx.clear();
                m_pkg_rx.append(static_cast<uint8_t>(value));
            }
        }
        break;
    case 1:
        {
            if (value == 0xAA)
            {
                m_pkg_rx.append(static_cast<uint8_t>(value));
            }
            else
            {
                m_pkg_rx.clear();
            }
        }
        break;
    case 2:
        {
            m_pkg_rx.append(static_cast<uint8_t>(value));
        }
        break;
    case 3:
        {
            m_pkg_rx.append(static_cast<uint8_t>(value));
            len = static_cast<uint8_t>(value);
        }
        break;
    default:
        {
            m_pkg_rx.append(static_cast<uint8_t>(value));
            if (len == 0)
            {
                uint8_t com;

                if (checkPkg(m_pkg_rx, com))
                {
                    // if (viewCom.count(com) != 0)
                    // {
                    //   qDebug() << "comRx <<< " << showbase << hex << mPkgRx << ", com = " << com;
                    // }

                    procCommand(com, m_pkg_rx);
                }
                m_pkg_rx.clear();
            }
            else
            {
                len--;
            }
        }
    }
}

/**
 * *****************************************************************************
 *
 * @brief ���� ������� ������ ���.
 * @param[in] staet True ��� ������ ������ ���, ����� False.
 *
 * *****************************************************************************
 */
void Bsp::SlotStart(bool start)
{
    (start) ? (m_timer.start(k_timer_tick_ms)) : (m_timer.stop());
}


//
void Bsp::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}


//
quint8 Bsp::bcd2int(quint8 bcd, bool *ok)
{
    quint8 value = 0;
    bool   t_ok  = ((bcd & 0x0F) < 0x0A) && ((bcd & 0xF0) < 0xA0);

    if (t_ok)
    {
        value = bcd & 0x0F;
        value += ((bcd >> 4) & 0x0F) * 10;
    }
    else
    {
        qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << " Error: " << bcd;
    }

    if (ok != nullptr)
    {
        *ok = t_ok;
    }


    return value;
}

//
quint8 Bsp::int2bcd(quint8 val, bool *ok)
{
    quint8 bcd  = 0;
    bool   t_ok = (val < 100);

    if (t_ok)
    {
        bcd = (val % 10);
        bcd += (val / 10) << 4;
    }
    else
    {
        qWarning("Can't convert value 0x%.2X to BCD code!", val);
    }

    if (ok != nullptr)
    {
        *ok = t_ok;
    }

    return bcd;
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ���������� ���� ������ � ���������
 * @param[in] com �������.
 * @param[in] size ���������� ���� ������ � ���������.
 * @param[in] size_allowed ��������� ���������� ������ � ���������.
 * @return True ���� ���������� ���������� ����, ����� false.
 *
 * *****************************************************************************
 */
bool Bsp::CheckSize(uint8_t com, int size, QVector<int> size_allowed)
{
    Q_ASSERT(size_allowed.size() > 0);

    bool check = (size_allowed.count(size) != 0);
    if (!check)
    {
        QString message = "Wrong size of data in command %1: %2";
        qWarning() << message.arg(com, 2, 16, QLatin1Char('0')).arg(size);
    }

    return check;
}


/**
 * *****************************************************************************
 *
 * @brief �������� ��� �������. ������� ��������������.
 * @param[in] �������.
 * @param[in] ������.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComDummy(uint8_t com, pkg_t &data)
{
    Q_UNUSED(data)

    qWarning() << QString("Command handler missing: 0x%1").arg(com, 2, 16, QLatin1Char('0'));
}


/**
 * *****************************************************************************
 *
 * @brief ������������ ������� ������ ���������� � ��.
 * @param[in] �������.
 * @param[in] ������.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComRx01(uint8_t com, pkg_t &data)
{
    Q_ASSERT(com == 0x01);

    if (com == 0x01)
    {
        if (!CheckSize(com, data.size(), { 4 }))
        {
            return;
        }

        uint8_t version_major = data.at(0);
        uint8_t version_minor = data.at(1);

        m_version.setText(kVersion.arg(version_major, 2, 16, QLatin1Char('0'))
                              .arg(version_minor, 2, 16, QLatin1Char('0')));

        uint16_t key = data.at(3);
        key          = static_cast<uint16_t>((key << 8) + data.at(2));
        m_key.setValue(key);
    }
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ������ ��� ������� �������� ����������� �� ��.
 * @param[in] �������.
 * @param[in] ������.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComTx11(uint8_t com, pkg_t &data)
{
    uint8_t value;
    Q_ASSERT(com == 0x11);

    data.clear();

    if (com == 0x11)
    {
        data.append(com);

        for (int i = 0; i < m_lines.size(); i++)
        {
            QLineEdit *line  = m_lines.at(i);
            QByteArray chars = line->text().toLocal8Bit();

            for (int j = 0; j < line->maxLength(); j++)
            {
                (j < chars.size()) ? data.append(chars.at(j)) : data.append(' ');
            }
        }

        value = m_top_lines.value() & 0x07;
        value |= (m_led_on.currentData().toUInt() == 0) ? (0) : (0x80);
        data.append(value);
        value = (m_cursor_pos.value() & 0x7F);
        value |= (m_cursor_on.currentData().toUInt() == 0) ? (0) : (0x80);
        data.append(value);
        data.append(0);
        data.append(0);
        data.append(0);
        data.append(0);
        data.append(0);
        data.append(0);
    }

    Q_ASSERT(data.size() == (128 + 1));
}


/**
 * *****************************************************************************
 *
 * @brief ���� ��������� ��������� ������������� ���������.
 *
 * *****************************************************************************
 */
void Bsp::SlotReadPackageFinished()
{
}
