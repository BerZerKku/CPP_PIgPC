#include <QDebug>
#include <QHeaderView>
#include <QTextCodec>
#include <cmath>

#include "bsp.h"

const QString Bsp::kMsgSizeError       = "Wrong size of data in command %1: %2";
const QString Bsp::kMsgTimeSourceError = "Wrong source of time: %1";

const QTextCodec *Bsp::kCodec      = QTextCodec::codecForName("CP1251");
const QString     Bsp::kTimeFormat = "dd.MM.yyyy hh:mm:ss.zzz";


/**
 * *****************************************************************************
 *
 * @brief Конструктор.
 * @param[in] tree Дерево.
 * @param[in] parent Родитель.
 *
 * *****************************************************************************
 */
Bsp::Bsp(QTreeWidget *tree, QWidget *parent) : QWidget(parent), m_tree(tree)
{
    // Эти строки не влияют на содержимое заголовка, но влияют на resize ниже.
    m_tree->headerItem()->setText(0, kCodec->toUnicode("Parameter"));
    m_tree->headerItem()->setText(1, kCodec->toUnicode("Value"));

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
 * @brief Создает ветку параметрв.
 *
 * *****************************************************************************
 */
void Bsp::crtTreeParam()
{
    QTreeWidgetItem *top = new QTreeWidgetItem(m_tree);
    top->setText(0, kCodec->toUnicode("Параметры"));
    m_tree->addTopLevelItem(top);

    m_key.setRange(0, 0xFFFF);
    m_key.setPrefix("0x");
    m_key.setDisplayIntegerBase(16);
    AddSpinBox(top, kCodec->toUnicode("Клавиатура"), &m_key);
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
bool Bsp::checkPkg(QVector<uint8_t> &pkg, eGB_COM &com)
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

    com = static_cast<eGB_COM>(pkg.takeFirst());

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
 * @brief Создает дерево параметров и производит начальные настройки.
 *
 * *****************************************************************************
 */
void Bsp::Init()
{
    crtTreeParam();

    m_map_com_rx.insert(0x01, &Bsp::HdlrComRx01);

    m_map_com_tx.insert(0x11, &Bsp::HdlrComTx11);

    connect(&m_timer, &QTimer::timeout, this, &Bsp::SlotHandleTick);
}


/**
 * *****************************************************************************
 *
 * @brief Добавить SpinBox.
 * @param[in] top
 * @param[in] widget Dbl;tn
 *
 * *****************************************************************************
 */
void Bsp::AddSpinBox(QTreeWidgetItem *top, const QString &name, QSpinBox *spinbox)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(top);
    item->setText(0, name);
    top->addChild(item);

    m_tree->setItemWidget(item, 1, spinbox);
}


/**
 * *****************************************************************************
 *
 * @brief Вызывает обработчик для принятой команды.
 * @param[in] com Команда.
 * @param[in] data Данные.
 *
 * *****************************************************************************
 */
void Bsp::procCommand(eGB_COM com, pkg_t &data)
{
    m_pkg_tx.clear();

    HdlrCom_t hdlr = m_map_com_rx.value(com, &Bsp::HdlrComDummy);
    (this->*hdlr)(com, data);
}

/**
 * *****************************************************************************
 *
 * @brief Слот обработки принятого байта.
 * @param[in] value Принятый байт данных.
 *
 * *****************************************************************************
 */
void Bsp::SlotReadByte(int value)
{
    static uint8_t len = 0;

    // @fixme Добавить время на передачу байт.
    // @fixme Найти сигналу окончания передачи данных из БСП более подходящее место!
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
                eGB_COM com;

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
 * @brief Слот запуска работы БСП.
 * @param[in] staet True для начала работы БСП, иначе False.
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
 * @brief Проверяет количество байт данных в сообщении
 * @param[in] com Команда.
 * @param[in] size Количество байт данных в сообщении.
 * @param[in] size_allowed Возможное количество данных в сообщении.
 * @return True если корректное количество байт, иначе false.
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
 * @brief Заглушка для команды. Выводит предупреждение.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComDummy(eGB_COM com, pkg_t &data)
{
    Q_UNUSED(data)

    qWarning() << QString("Command handler missing: 0x%1").arg(com, 2, 16, QLatin1Char('0'));
}


/**
 * *****************************************************************************
 *
 * @brief Обрабатывает команду чтения информации с ПИ.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComRx01(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == 0x01);

    if (com == 0x01)
    {
        if (!CheckSize(com, data.size(), { 2 }))
        {
            return;
        }

        uint16_t value = data.at(1);
        value          = static_cast<uint16_t>((value << 8) + data.at(0));

        m_key.setValue(value);
    }
}


/**
 * *****************************************************************************
 *
 * @brief Формирует данные для команды передачи изображения на ПИ.
 * @param[in] Команда.
 * @param[in] Данные.
 *
 * *****************************************************************************
 */
void Bsp::HdlrComTx11(eGB_COM com, pkg_t &data)
{
    Q_ASSERT(com == 0x11);

    data.clear();

    if (com == 0x11)
    {
        data.append(com);

        for (int i = 0; i < 128; i++)
        {
            data.append(static_cast<uint8_t>(i));
        }
    }


    Q_ASSERT(data.size() == (128 + 1));
}


/**
 * *****************************************************************************
 *
 * @brief Слот обработки окончания передаваемого сообщения.
 *
 * *****************************************************************************
 */
void Bsp::SlotReadPackageFinished()
{
}


/**
 * *****************************************************************************
 *
 * @brief Слот обработки периодических событий.
 *
 * *****************************************************************************
 */
void Bsp::SlotHandleTick()
{
    eGB_COM com = static_cast<eGB_COM>(0x11);

    HdlrCom_t hdlr = m_map_com_tx.value(com, &Bsp::HdlrComDummy);
    (this->*hdlr)(com, m_pkg_tx);

    if (!m_pkg_tx.isEmpty())
    {
        m_pkg_tx.insert(1, static_cast<uint8_t>(m_pkg_tx.size() - 1));
        m_pkg_tx.append(calcCrc(m_pkg_tx));
        m_pkg_tx.insert(0, 0x55);
        m_pkg_tx.insert(1, 0xAA);

        com = static_cast<eGB_COM>(m_pkg_tx.at(2));

        // if (viewCom.count(com) != 0)
        // {
        //     qDebug() << "comTx >>> " << showbase << hex << Bsp::pkgTx;
        // }

        for (uint8_t byte : m_pkg_tx)
        {
            emit SignalWriteByte(byte);
        }


        m_pkg_tx.clear();
    }
}
