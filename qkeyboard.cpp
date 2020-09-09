#include "qkeyboard.h"
#include "ui_qkeyboard.h"

#include <QDebug>
#include <QKeyEvent>
#include <QPalette>

QKeyboard::QKeyboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QKeyboard)
{
    ui->setupUi(this);

    signalMapper = new QSignalMapper(this);

    btns.append(ui->btn1);
    btns.append(ui->btn2);
    btns.append(ui->btn3);
    btns.append(ui->btn4);
    btns.append(ui->btn5);
    btns.append(ui->btn6);
    btns.append(ui->btn7);
    btns.append(ui->btn8);
    btns.append(ui->btn9);

    QFont f = ui->btn1->font();
    f.setPointSize(15);

    for(QPushButton *btn: btns) {
        btn->setFont(f);
        connect(btn, &QPushButton::clicked,
                this->signalMapper, QOverload<>::of(&QSignalMapper::map));
    }
    reset();

    connect(signalMapper, QOverload<int>::of(&QSignalMapper::mapped),
            this, &QKeyboard::clicked);

    connect(this, &QKeyboard::clicked, this, &QKeyboard::btnPressed);
}

//
QKeyboard::~QKeyboard()
{
    delete ui;
}

//
void QKeyboard::btnPressed(int value)
{
    if (value != KEY_NO) {
        if (value == KEY_FUNC) {
            switchAlt();
        } else {
            key = static_cast<eKEY> (value);
        }
    }

    qDebug() << "Button pressed: " << getButtonName(static_cast<eKEY> (value));
}

//
QString QKeyboard::getButtonName(eKEY ekey) const
{
    QString name;

    switch(ekey) {
        case KEY_NO: {
            name = "KEY_NO";
        } break;
        case KEY_EMPTY: {
            name = " ";
        } break;
        case KEY_FUNC: {
            name = "Фн";
        } break;
        case KEY_ENTER: {
            name = QChar(0x23CE);
        } break;
        case KEY_CANCEL: {
            name = "Отмена";
        } break;
        case KEY_DOWN: {
            name = QChar(0x25BC);
        } break;
        case KEY_LEFT: {
            name = QChar(0x25C0);
        } break;
        case KEY_RIGHT: {
            name = QChar(0x25B6);
        } break;
        case KEY_UP: {
            name = QChar(0x25B2);
        } break;
        case KEY_MENU:{
            name = "Меню";
        } break;
        case KEY_RESET: {
            name = "Сброс";
        } break;
        case KEY_PUSK: {
            name = "Пуск";
        } break;
        case KEY_PUSK_UD: {
            name = "KEY_PUSK_UD";
        } break;
        case KEY_PUSK_NALAD: {
            name = "KEY_PUSK_NALAD";
        } break;
        case KEY_RESET_IND: {
            name = "Сброс\nинд.";
        } break;
        case KEY_CALL: {
            name = "KEY_CALL";
        } break;
        case KEY_AC_PUSK: {
            name = "KEY_AC_PUSK";
        } break;
        case KEY_AC_PUSK_UD: {
            name = "KEY_AC_PUSK_UD";
        } break;
        case KEY_AC_RESET: {
            name = "KEY_AC_RESET";
        } break;
        case KEY_AC_REGIME: {
            name = "KEY_AC_REGIME";
        } break;
    }

    Q_ASSERT(!name.isEmpty());

    return name;
}

//
eKEY QKeyboard::getKey()
{
    eKEY tkey = key;
    key = KEY_NO;
    return tkey;
}

//
void QKeyboard::keyPressed(int value)
{
    eKEY ekey = KEY_NO;

    switch(static_cast<Qt::Key> (value)) {
        case Qt::Key_Backspace: // DOWN
        case Qt::Key_C: {
            ekey = KEY_CANCEL;
        } break;
        case Qt::Key_Down: {
            ekey = KEY_DOWN;
        } break;
        case Qt::Key_Left: {
            ekey = KEY_LEFT;
        } break;
        case Qt::Key_F: {
            ekey = KEY_FUNC;
        } break;
        case Qt::Key_M: {
            ekey = KEY_MENU;
        } break;
        case Qt::Key_Return: {
            ekey = KEY_ENTER;
        } break;
        case Qt::Key_Right: {
            ekey = KEY_RIGHT;
        } break;
        case Qt::Key_Up: {
            ekey = KEY_UP;
        } break;
        default: {
        }
    }

    // Анмимация нажатия кнопки, если она есть на экране
    QMap<QPushButton*, eKEY> *m = alt ? &secondary : &primary;
    if (m->values().contains(ekey)) {
        m->key(ekey)->animateClick();
    } else {
        btnPressed(ekey);
    }
}

//
void QKeyboard::refresh()
{
    QColor color;
    QMap<QPushButton*, eKEY> *m;

    m = alt ? &secondary : &primary;
    color = alt ? kBtnColorSecondary : kBtnColorPrimary;

    for(QPushButton *btn: btns) {
        btn->setText(getButtonName(m->value(btn)));
        btn->setEnabled(m->value(btn) != KEY_EMPTY);
        signalMapper->setMapping(btn, m->value(btn));
    }

    setButtonTextColor(color);
}

//
void QKeyboard::reset()
{
    for(QPushButton *btn: btns) {
        primary[btn] = KEY_NO;
        secondary[btn] = KEY_NO;
        signalMapper->setMapping(btn, KEY_NO);
    }

    alt = false;
}

//
void QKeyboard::setButtonColor(QColor color)
{
   if (color.isValid()) {
        QString qss = QString("background-color: %1").arg(color.name());
        ui->btn1->setStyleSheet(qss);
        for(QPushButton *btn: btns) {
            btn->setStyleSheet(qss);
        }
    }
}

void QKeyboard::setButtonTextColor(QColor color)
{
    if (color.isValid()) {
        QPalette palette = ui->btn1->palette();
        palette.setColor(QPalette::ButtonText, color);
        for(QPushButton *btn: btns) {
            btn->setPalette(palette);
        }
    }
}

//
void QKeyboard::setType(eGB_TYPE_DEVICE device) {
    reset();

    switch(device) {
        case AVANT_K400: {
            setTypeK400();
        } break;

        default: {
            Q_ASSERT(false);
        }
    }

    refresh();
}

//
void QKeyboard::setTypeK400()
{
    primary[ui->btn1] = KEY_FUNC;
    primary[ui->btn2] = KEY_UP;
    primary[ui->btn3] = KEY_EMPTY;
    primary[ui->btn4] = KEY_LEFT;
    primary[ui->btn5] = KEY_ENTER;
    primary[ui->btn6] = KEY_RIGHT;
    primary[ui->btn7] = KEY_CANCEL;
    primary[ui->btn8] = KEY_DOWN;
    primary[ui->btn9] = KEY_EMPTY;

    secondary[ui->btn1] = KEY_FUNC;
    secondary[ui->btn2] = KEY_EMPTY;
    secondary[ui->btn3] = KEY_RESET_IND;
    secondary[ui->btn4] = KEY_PUSK;
    secondary[ui->btn5] = KEY_MENU;
    secondary[ui->btn6] = KEY_EMPTY;
    secondary[ui->btn7] = KEY_EMPTY;
    secondary[ui->btn8] = KEY_EMPTY;
    secondary[ui->btn9] = KEY_RESET;
}

//
void QKeyboard::switchAlt()
{
    alt = !alt;
    refresh();
}
