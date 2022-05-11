/*
 * menu.cpp
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */
#include <stdio.h>

#include "debug/debug.hpp"
#include "menu.h"
#include "version.hpp"

char clMenu::vLCDbuf[SIZE_BUF_STRING + 1];

static const char s_text_no_connect[] PROGMEM = "Нет связи с БСП!!!";
static const char s_text_version[] PROGMEM    = "Версии ПО";
static const char s_text_version_pi[] PROGMEM = "  ПИ  МК  : %.2X.%.2X";


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
clMenu::clMenu() : m_connection(false), m_key(0)
{
    m_top_lines = 1;
    m_blink     = false;

    m_blink_counter    = 0;
    m_lcd_init_counter = 0;

    m_cursor_on  = false;
    m_cursor_pos = 0;

    vLCDsetLed(LED_ON);
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

    m_key = eKEYget();

    if (!m_connection)
    {
        vLCDsetLed(LED_ON);

        clearTextBuf();

        m_top_lines = 1;
        snprintf_P(&vLCDbuf[0], 21, (m_blink) ? s_text_no_connect : s_text_version);
        snprintf_P(&vLCDbuf[20], 21, s_text_version_pi, PROJECT_VER_MAJOR, PROJECT_VER_MINOR);
    }
    else
    {
        if (m_cursor_on && m_blink)
        {
            uint8_t pos = m_cursor_pos - 1;

            if (pos < (SIZE_BUF_STRING - 1))
            {
                vLCDbuf[pos] = '_';
            }
        }

        vLCDsetLed((m_led_on) ? (LED_ON) : (LED_OFF));
    }

    vLCDclear();
    vLCDdrawBoard(m_top_lines);
    vLCDputchar(vLCDbuf, m_top_lines);
    vLCDrefresh();
}


/**
 * *****************************************************************************
 *
 * @brief Очистка буфера текста.
 *
 * *****************************************************************************
 */
void clMenu::clearTextBuf()
{
    for (uint_fast8_t i = 0; i < sizeof(vLCDbuf); i++)
        vLCDbuf[i] = ' ';
}

/* ******************************** E N D *********************************** */
