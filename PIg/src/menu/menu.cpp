/*
 * menu.cpp
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */
#include <stdio.h>

#include "debug/debug.hpp"
#include "drivers/ks0108.h"
#include "menu.h"
#include "txCom.h"
#include "version.hpp"

/// ����� ��������� �� ���������
#define LED_REGIME LED_SWITCH

/// ����� ������ ���������� �� ���
static char vLCDbuf[SIZE_BUF_STRING + 1];

/// ���-�� ����� ������ ������������ �� ������
#define NUM_TEXT_LINES (SIZE_BUF_STRING / 20)


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
clMenu::clMenu() : m_connection(false), m_key(KEY_NO)
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

    // ������� ��� � ����������
    // ���� ������ ����� ������ - ��������� ��������������� ���������
    uint8_t key_code = eKEYget();
    eKEY    key      = vKEYgetButton(key_code);
    if (key != KEY_NO)
    {
        if (key == KEY_EMPTY)
            key = KEY_NO;
        m_key = key;

        vLCDsetLed(LED_SWITCH);
    }

    // ����� � ����� ����������� �������� ����
    // ���� ��������� ��� ��� �������� �� ���������
    //    (this->*lvlMenu)();
    m_key = KEY_NO;

    // ����� ��������� � ������ ���������� ����� � ���
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

    // �������������� ������ �������� � ������ ��� ������ �� �����
    vLCDputchar(vLCDbuf, lineParam_);
    // ������ ���������� ���-�� �� ���
    vLCDrefresh();
}
