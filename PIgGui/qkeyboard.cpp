#include "qkeyboard.h"
#include "ui_qkeyboard.h"

#include <QDebug>
#include <QKeyEvent>
#include <QPalette>
#include <QTextCodec>

QKeyboard::QKeyboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QKeyboard)
{
    ui->setupUi(this);

    btns.resize(NUM_KEY_IN_LAYOUT);
    btnLayout.resize(2*btns.size());

    btns.replace(0, ui->btn1);
    btns.replace(1, ui->btn2);
    btns.replace(2, ui->btn3);
    btns.replace(3, ui->btn4);
    btns.replace(4, ui->btn5);
    btns.replace(5, ui->btn6);
    btns.replace(6, ui->btn7);
    btns.replace(7, ui->btn8);
    btns.replace(8, ui->btn9);

    QFont f = ui->btn1->font();
    f.setPointSize(15);

    for(int i = 0; i < btns.size(); i++) {
        QPushButton *btn = btns.at(i);
        btn->setFont(f);
        btn->setFocusPolicy(Qt::NoFocus);
        connect(btn, &QPushButton::clicked, [=]() {btnPressed(i);});
    }

    reset();
    refresh();

    connect(this, &QKeyboard::clicked, this, &QKeyboard::keyPressed);

    reset();
}

//
QKeyboard::~QKeyboard()
{
    delete ui;
}

//
void QKeyboard::btnPressed(int value)
{
    Q_ASSERT(value < btns.size());

    if (alt) {
        value += btns.size();
    }
    eKEY ekey = btnLayout.at(value);

    if (ekey != KEY_NO) {
        if (ekey == KEY_FUNC) {
            switchAlt();
        } else {
            key = static_cast<uint8_t> (value + 1);
        }
    }

//    emit debug(QString("Button pressed: %1").arg(name));
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
            name = QString::fromLocal8Bit("Фн");
        } break;
        case KEY_ENTER: {
            name = QChar(0x23CE);
        } break;
        case KEY_CANCEL: {
            name = QString::fromLocal8Bit("Отмена");
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
            name = QString::fromLocal8Bit("Меню");
        } break;
        case KEY_RESET: {
            name = QString::fromLocal8Bit("Сброс");
        } break;
        case KEY_PUSK: {
            name = QString::fromLocal8Bit("Пуск");
        } break;
        case KEY_PUSK_UD: {
            name = QString::fromLocal8Bit("Удал.\nПуск\nПРД");
        } break;
        case KEY_PUSK_NALAD: {
            name = QString::fromLocal8Bit("н. Пуск");
        } break;
        case KEY_RESET_IND: {
            name = QString::fromLocal8Bit("Сброс\nинд.");
        } break;
        case KEY_CALL: {
            name = QString::fromLocal8Bit("Вызов");
        } break;
        case KEY_AC_PUSK: {
            name = QString::fromLocal8Bit("Пуск\nАК");
        } break;
        case KEY_AC_PUSK_UD: {
            name = QString::fromLocal8Bit("Удал.\nПуск\nАК");
        } break;
        case KEY_AC_RESET: {
            name = QString::fromLocal8Bit("Сброс\nАК");
        } break;
        case KEY_AC_REGIME: {
            name = QString::fromLocal8Bit("Режим\nАК");
        } break;
    }

    Q_ASSERT(!name.isEmpty());

    return name;
}

//
uint8_t QKeyboard::getKey()
{
    uint8_t tkey = key;
    key = 0;
    return tkey;
}

//
void QKeyboard::keyPressed(int value)
{
    eKEY ekey = KEY_NO;

//    emit debug(QString("keyPressed: 0x%1").arg(value, 8, 16, QLatin1Char('0')));

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

    if (btnLayout.count(ekey) > 0) {
        int start = alt ? btns.size() : 0;
        int keynumber = btnLayout.indexOf(ekey, start);

        if ((keynumber >= 0) && ((keynumber - start) < btns.size())) {
            // Анимация нажатия кнопки, если она есть на экране
            btns.at(keynumber - start)->animateClick();
        } else {
            key = static_cast<uint8_t> (keynumber + 1);
        }
    }
}

//
void QKeyboard::refresh()
{
    QColor color;

    color = alt ? kBtnColorSecondary : kBtnColorPrimary;

    Q_ASSERT(btns.size() == NUM_KEY_IN_LAYOUT);
    Q_ASSERT(btnLayout.size() == 2*btns.size());

    for(quint8 i = 0; i < btns.size(); i++) {
        QPushButton *btn = btns.at(i);
        int keynumber = alt ? i + btns.size() : i;
        btn->setText(getButtonName(btnLayout.at(keynumber)));
        btn->setEnabled(btnLayout.at(keynumber) != KEY_EMPTY);
    }

    setButtonTextColor(color);
}

//
void QKeyboard::reset()
{
    for(auto &btn: btnLayout) {
        btn = KEY_NO;
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

void QKeyboard::setLayoutButton(uint8_t number, eKEY key)
{
    Q_ASSERT(number > 0);
    Q_ASSERT(number <= 2*NUM_KEY_IN_LAYOUT);

    if ((number > 0) && (number <= (2*NUM_KEY_IN_LAYOUT))) {
        btnLayout.replace(number - 1, key);
    }
    refresh();
}

//
void QKeyboard::switchAlt()
{
    alt = !alt;
    refresh();
}
