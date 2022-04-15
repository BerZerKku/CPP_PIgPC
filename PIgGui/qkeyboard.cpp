#include "qkeyboard.h"
#include "ui_qkeyboard.h"

#include <QDebug>
#include <QKeyEvent>
#include <QPalette>
#include <QTextCodec>

QKeyboard::QKeyboard(QWidget *parent) : QWidget(parent), ui(new Ui::QKeyboard)
{
    ui->setupUi(this);

    btns.resize(NUM_KEY_IN_LAYOUT);
    btnLayout.resize(btns.size());

    btns.replace(0, ui->btn1);
    btns.replace(1, ui->btn2);
    btns.replace(2, ui->btn3);
    btns.replace(3, ui->btn4);
    btns.replace(4, ui->btn5);
    btns.replace(5, ui->btn6);
    btns.replace(6, ui->btn7);
    btns.replace(7, ui->btn8);
    btns.replace(8, ui->btn9);
    btns.replace(9, ui->btn10);

    QFont f = ui->btn1->font();
    f.setPointSize(15);

    for (int i = 0; i < btns.size(); i++)
    {
        QPushButton *btn = btns.at(i);
        btn->setFont(f);
        btn->setFocusPolicy(Qt::NoFocus);
        connect(btn, &QPushButton::pressed, this, &QKeyboard::btnPressed);
        connect(btn, &QPushButton::released, this, &QKeyboard::btnPressed);
    }

    connect(this, &QKeyboard::clicked, this, &QKeyboard::keyPressed);

    SetLayout();
}

//
QKeyboard::~QKeyboard()
{
    delete ui;
}


//
void QKeyboard::reset()
{
    for (auto &btn : btnLayout)
    {
        btn = KEY_NO;
    }

    alt = false;
}


//
uint16_t QKeyboard::getKey()
{
    return m_key;
}


//
void QKeyboard::btnPressed()
{

    uint16_t key = 0;
    for (int i = 0; i < btns.size(); i++)
    {
        key |= ((btns.at(i)->isDown()) ? (1) : (0)) << i;
    }

    m_key = key;
}


//
void QKeyboard::keyPressed(int value)
{
    const QMap<Qt::Key, eKEY> key_map = {
        { Qt::Key_Backspace, KEY_CANCEL },  //
        { Qt::Key_C, KEY_CANCEL },          //
        { Qt::Key_Down, KEY_DOWN },         //
        { Qt::Key_Left, KEY_LEFT },         //
        { Qt::Key_F, KEY_FUNC },            //
        { Qt::Key_M, KEY_MENU },            //
        { Qt::Key_Return, KEY_ENTER },      //
        { Qt::Key_Right, KEY_RIGHT },       //
        { Qt::Key_Up, KEY_UP }              //
    };


    eKEY ekey = key_map.value(static_cast<Qt::Key>(value), KEY_NO);

    if ((ekey != KEY_NO) && (btnLayout.count(ekey) > 0))
    {
        int start     = alt ? btns.size() : 0;
        int keynumber = btnLayout.indexOf(ekey, start);

        if ((keynumber >= 0) && ((keynumber - start) < btns.size()))
        {
            // Анимация нажатия кнопки, если она есть на экране
            btns.at(keynumber - start)->animateClick();
        }
        else
        {
            m_key = static_cast<uint8_t>(keynumber + 1);
        }
    }
}


//
void QKeyboard::refresh()
{
    QColor color;

    color = alt ? kBtnColorSecondary : kBtnColorPrimary;

    Q_ASSERT(btns.size() == NUM_KEY_IN_LAYOUT);
    Q_ASSERT(btnLayout.size() == btns.size());

    for (quint8 i = 0; i < btns.size(); i++)
    {
        QPushButton *btn = btns.at(i);

        btn->setText(getButtonName(btnLayout.at(i)));
        btn->setEnabled(btnLayout.at(i) != KEY_EMPTY);
    }

    setButtonTextColor(color);
}


//
void QKeyboard::setButtonColor(QColor color)
{
    if (color.isValid())
    {
        QString qss = QString("background-color: %1").arg(color.name());
        ui->btn1->setStyleSheet(qss);
        for (int i = 0; i < btns.size(); i++)
        {
            btns.at(i)->setStyleSheet(qss);
        }
    }
}


//
void QKeyboard::setButtonTextColor(QColor color)
{
    if (color.isValid())
    {
        QPalette palette = ui->btn1->palette();
        palette.setColor(QPalette::ButtonText, color);
        for (int i = 0; i < btns.size(); i++)
        {
            btns.at(i)->setPalette(palette);
        }
    }
}


//
QString QKeyboard::getButtonName(eKEY ekey) const
{
    QString name;

    switch (ekey)
    {
    case KEY_NO:
        {
            name = "KEY_NO";
        }
        break;
    case KEY_EMPTY:
        {
            name = " ";
        }
        break;
    case KEY_FUNC:
        {
            name = QString::fromLocal8Bit("Фн");
        }
        break;
    case KEY_ENTER:
        {
            name = QChar(0x23CE);
        }
        break;
    case KEY_CANCEL:
        {
            name = QString::fromLocal8Bit("Отмена");
        }
        break;
    case KEY_DOWN:
        {
            name = QChar(0x25BC);
        }
        break;
    case KEY_LEFT:
        {
            name = QChar(0x25C0);
        }
        break;
    case KEY_RIGHT:
        {
            name = QChar(0x25B6);
        }
        break;
    case KEY_UP:
        {
            name = QChar(0x25B2);
        }
        break;
    case KEY_MENU:
        {
            name = QString::fromLocal8Bit("Меню");
        }
        break;
    case KEY_RESET:
        {
            name = QString::fromLocal8Bit("Сброс");
        }
        break;
    case KEY_PUSK:
        {
            name = QString::fromLocal8Bit("Пуск");
        }
        break;
    case KEY_PUSK_UD:
        {
            name = QString::fromLocal8Bit("Удал.\nПуск\nПРД");
        }
        break;
    case KEY_PUSK_NALAD:
        {
            name = QString::fromLocal8Bit("н. Пуск");
        }
        break;
    case KEY_RESET_IND:
        {
            name = QString::fromLocal8Bit("Сброс\nинд.");
        }
        break;
    case KEY_CALL:
        {
            name = QString::fromLocal8Bit("Вызов");
        }
        break;
    case KEY_AC_PUSK:
        {
            name = QString::fromLocal8Bit("Пуск\nАК");
        }
        break;
    case KEY_AC_PUSK_UD:
        {
            name = QString::fromLocal8Bit("Удал.\nПуск\nАК");
        }
        break;
    case KEY_AC_RESET:
        {
            name = QString::fromLocal8Bit("Сброс\nАК");
        }
        break;
    case KEY_AC_REGIME:
        {
            name = QString::fromLocal8Bit("Режим\nАК");
        }
        break;
    case KEY_TALK:
        {
            name = QString::fromLocal8Bit("Речь");
        }
        break;
    case KEY_MAX: break;
    }

    Q_ASSERT(!name.isEmpty());

    return name;
}


//
void QKeyboard::SetLayout()
{
    SetLayoutK400();

    refresh();
}


//
void QKeyboard::SetLayoutK400()
{
    btnLayout.replace(0, KEY_FUNC);
    btnLayout.replace(1, KEY_UP);
    btnLayout.replace(2, KEY_PUSK_UD);
    btnLayout.replace(3, KEY_LEFT);
    btnLayout.replace(4, KEY_MENU);
    btnLayout.replace(5, KEY_RIGHT);
    btnLayout.replace(6, KEY_AC_RESET);
    btnLayout.replace(7, KEY_DOWN);
    btnLayout.replace(8, KEY_CANCEL);
    btnLayout.replace(9, KEY_TALK);
}
