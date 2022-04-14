/*
 * menu.cpp
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */
#include <stdio.h>

#include "debug/debug.hpp"
#include "drivers/ks0108.h"
#include "menu.h"
#include "txCom.h"
#include "version.hpp"

/// режим подсветки по умолчанию
#define LED_REGIME LED_SWITCH

/// буфер текста выводимого на ЖКИ
static char vLCDbuf[SIZE_BUF_STRING + 1];

/// кол-во строк данных отображаемых на экране
#define NUM_TEXT_LINES (SIZE_BUF_STRING / 20)


/**
 * *************************************************************************
 *
 * @brief Конструктор.
 *
 * Устнанавливается по умолчанию неопределеныый тип аппарата.
 * Заполняются необходимые для работы массивы.
 * Производится переход на стартовый уровень меню.
 *
 * *************************************************************************
 */
clMenu::clMenu() : m_connection(false), m_key(KEY_NO)
{
    lineParam_ = 3;
    m_blink    = false;

    m_blink_counter    = 0;
    m_lcd_init_counter = 0;

    // курсор неактивен
    cursorEnable_ = false;
    cursorLine_   = 0;

    // установка режима работы подсветки
    vLCDsetLed(LED_REGIME);
}


/**
 * *************************************************************************
 *
 * @brief Работа с меню.
 *
 * *************************************************************************
 */
void clMenu::proc(void)
{

    static const char fcNoConnectBsp[] PROGMEM = " Нет связи с БСП!!! ";

    if (++m_lcd_init_counter >= k_lcd_init_counter_max)
    {
        vLCDinit();
        m_lcd_init_counter = 0;
    }

    if (++m_blink_counter >= k_blink_counter_max)
    {
        m_blink         = !m_blink;
        m_blink_counter = 0;
    }

    // Считаем код с клавиатуры
    // Если нажата любая кнопка - включится кратковременная подсветка
    uint8_t key_code = eKEYget();
    eKEY    key      = vKEYgetButton(key_code);
    if (key != KEY_NO)
    {
        if (key == KEY_EMPTY)
            key = KEY_NO;
        m_key = key;

        vLCDsetLed(LED_SWITCH);
    }

    // вывод в буфер содержимого текущего меню
    // либо сообщения что тип аппарата не определен
    //    (this->*lvlMenu)();
    m_key = KEY_NO;

    // вывод сообщения в случае отсутствия связи с БСП
    if (!m_connection)
    {
        if (m_blink)
        {
            lineParam_ = 1;
            snprintf_P(&vLCDbuf[0], 20, fcNoConnectBsp);
            snprintf_P(&vLCDbuf[20], 20, fcNoConnectBsp);
            snprintf_P(&vLCDbuf[40], 20, fcNoConnectBsp);
            snprintf_P(&vLCDbuf[60], 20, fcNoConnectBsp);
            snprintf_P(&vLCDbuf[80], 20, fcNoConnectBsp);
            snprintf_P(&vLCDbuf[100], 20, fcNoConnectBsp);
        }
    }

    // преобразование строки символов в данные для вывода на экран
    vLCDputchar(vLCDbuf, lineParam_);
    // запуск обновления инф-ии на ЖКИ
    vLCDrefresh();
}
