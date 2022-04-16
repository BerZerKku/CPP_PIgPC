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

/// режим подсветки по умолчанию
#define LED_REGIME LED_SWITCH

char clMenu::vLCDbuf[SIZE_BUF_STRING + 1];

static const char s_text_no_connect[] PROGMEM = " Нет связи с БСП!!! ";
static const char s_text_version[] PROGMEM    = " Версия ПИ: %.2u.%.2u";


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
    if (m_key != 0)
    {
        vLCDsetLed(LED_SWITCH);
    }

    if (!m_connection)
    {
        lineParam_ = 1;

        clearTextBuf();
        if (m_blink)
        {
            snprintf_P(&vLCDbuf[0], 21, s_text_no_connect);
        }
        else
        {
            snprintf_P(&vLCDbuf[0], 21, "!23123");
        }

        snprintf_P(&vLCDbuf[40], 20, s_text_version, PROJECT_VER_MAJOR, PROJECT_VER_MINOR);
    }

    vLCDputchar(vLCDbuf, lineParam_);
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
