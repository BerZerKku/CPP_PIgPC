/*
 * menu.cpp
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */
#include <stdio.h>

#include "debug/debug.hpp"
#include "menu.h"
#include "version.hpp"

/// ����� ��������� �� ���������
#define LED_REGIME LED_SWITCH

char clMenu::vLCDbuf[SIZE_BUF_STRING + 1];

/**
 * *************************************************************************
 *
 * @brief �����������.
 *
 * ���������������� �� ��������� �������������� ��� ��������.
 * ����������� ����������� ��� ������ �������.
 * ������������ ������� �� ��������� ������� ����.
 *
 * *************************************************************************
 */
clMenu::clMenu() : m_connection(false), m_key(0)
{
    lineParam_ = 3;
    m_blink    = false;

    m_blink_counter    = 0;
    m_lcd_init_counter = 0;

    // ������ ���������
    cursorEnable_ = false;
    cursorLine_   = 0;

    // ��������� ������ ������ ���������
    vLCDsetLed(LED_REGIME);
}


/**
 * *************************************************************************
 *
 * @brief ������ � ����.
 *
 * *************************************************************************
 */
void clMenu::proc(void)
{

    static const char fcNoConnectBsp[] PROGMEM = " ��� ����� � ���!!! ";

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

    vLCDputchar(vLCDbuf, lineParam_);
    vLCDrefresh();
}
