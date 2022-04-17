#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wrapper.h"

#include "PIg/src/drivers/keyboard.h"
#include "PIg/src/drivers/ks0108.h"
#include "PIg/src/menu/base.h"

#include <QTextCodec>

static uint16_t uLedTimeOn = LCD_TIME_LED_ON;
static eLCD_LED eLed       = LED_OFF;
static QString  text;

void vKEYmain(void)
{
}

uint16_t eKEYget(void)
{
    return w->ui->kbd->getKey();
}

uint8_t timePressKey()
{
    // FIXME В оригинале имеется еще обработка длительного нажатия кнопки.
    return 0;
}

void vLCDclear(void)
{
    text.clear();
    w->ui->textEdit->clear();
}

void vLCDinit(void)
{
}

void vLCDmain(void)
{
}

void vLCDrefresh(void)
{
    w->ui->textEdit->clear();
    w->ui->textEdit->setText(text);
}

bool vLCDdrawBoard(uint8_t num)
{
    Q_UNUSED(num)
    return true;
}

bool vLCDputchar(const char* buf, uint8_t num)
{
    QTextCodec* codec = QTextCodec::codecForName("CP1251");

    text.clear();
    for (quint8 row = 0; row < 7; row++)
    {
        for (quint16 col = 0; col < 20; col++)
        {
            char symbol = (row == num) ? '=' : *buf++;
            if (symbol == '\0')
            {
                symbol = ' ';
            }
            text += codec->toUnicode(&symbol, 1);
        }
        if (row != 6)
        {
            text += "\n";
        }
    }

    return true;
}

void vLCDsetLed(eLCD_LED val)
{
    Q_ASSERT(val == LED_OFF || val == LED_SWITCH || val == LED_ON);

    eLed       = val;
    uLedTimeOn = (eLed == LED_SWITCH) ? (LCD_TIME_LED_ON) : 0;
}

void vLCDled()
{
    const uint16_t step   = 20;  // период вызова/10мс
    bool           enable = false;

    if (eLed == LED_ON)
    {
        enable = true;
    }
    else if (uLedTimeOn > 0)
    {
        uLedTimeOn = (uLedTimeOn > step) ? uLedTimeOn - step : 0;
        enable     = uLedTimeOn > 0;
    }

    w->setBacklight(enable);
}
