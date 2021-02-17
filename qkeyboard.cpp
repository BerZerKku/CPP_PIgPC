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
        btn->setText(getButtonName(KEY_EMPTY));
        btn->setEnabled(false);
        btn->setFont(f);
        connect(btn, &QPushButton::clicked,
                this->signalMapper, QOverload<>::of(&QSignalMapper::map));
    }
    reset();

    connect(signalMapper, QOverload<int>::of(&QSignalMapper::mappedInt),
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

    QString name = getButtonName(static_cast<eKEY> (value));
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
            name = "KEY_PUSK_UD";
        } break;
        case KEY_PUSK_NALAD: {
            name = "KEY_PUSK_NALAD";
        } break;
        case KEY_RESET_IND: {
            name = QString::fromLocal8Bit("Сброс\nинд.");
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

    // Анмимация нажатия кнопки, если она есть на экране
    QMap<uint8_t, eKEY> *m = alt ? &secondary : &primary;
    if (m->values().contains(ekey)) {
        btns.at(m->key(ekey))->animateClick();
    } else {
        btnPressed(ekey);
    }
}

//
void QKeyboard::refresh()
{
    QColor color;
    QMap<uint8_t, eKEY> *m;

    m = alt ? &secondary : &primary;
    color = alt ? kBtnColorSecondary : kBtnColorPrimary;

    for(quint8 i = 1; i <= btns.size(); i++) {
        QPushButton *btn = btns.at(i - 1);
        btn->setText(getButtonName(m->value(i)));
        btn->setEnabled(m->value(i) != KEY_EMPTY);
        signalMapper->setMapping(btn, m->value(i - 1));
    }

    setButtonTextColor(color);
}

//
void QKeyboard::reset()
{
    for(uint8_t i = 1; i <= NUM_KEY_IN_LAYOUT; i++) {
        primary[i] = KEY_NO;
        secondary[i] = KEY_NO;
        signalMapper->setMapping(btns.at(i - 1), KEY_NO);
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
        if (number <= NUM_KEY_IN_LAYOUT) {
            primary.insert(number, key);
        } else {
            secondary.insert(number, key);
        }
    }
    refresh();

    qDebug() << "number = " << (quint16) number << ", key = " << (quint16) key;
}

//
void QKeyboard::switchAlt()
{
    alt = !alt;
    refresh();
}
