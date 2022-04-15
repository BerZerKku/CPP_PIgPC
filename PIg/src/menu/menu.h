/*
 * menu.h
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */

#ifndef MENU_H_
#define MENU_H_

#include "drivers/keyboard.h"
#include "drivers/ks0108.h"

/// ����� ������ ������ ����� (������� �� ��������� ��������), ��
#define TIME_CYLCE 100

// ����� ����
class clMenu
{
    static constexpr uint8_t k_lcd_init_counter_max = (200 / TIME_CYLCE);
    static constexpr uint8_t k_blink_counter_max    = (1000 / TIME_CYLCE);

public:
    clMenu();
    void proc();

    static char vLCDbuf[SIZE_BUF_STRING + 1];

    /**
     * *************************************************************************
     *
     * @brief ��������� ��������� ����������.
     * @param[in] enabled True ���� ���� ����������, ����� false.
     *
     * *************************************************************************
     */
    void SetConnection(bool enabled) { m_connection = enabled; }

    /**
     * *************************************************************************
     *
     * @brief ��������� ������� ����� � ���.
     * @return True ���� ���� ����������, ����� false.
     *
     * *************************************************************************
     */
    bool IsConnection() const { return m_connection; }


    /**
     * *************************************************************************
     *
     * @brief ���������� ���� ������� ������.
     * @return ���� ������� ������.
     *
     * *************************************************************************
     */
    uint16_t GetKeys() const { return m_key; }

private:
    bool     m_connection;    ///< ���� ������� ����� � ���.
    uint16_t m_key;           ///< ���� ������� ������
    bool     m_blink;         ///< ���� ������� ��������
    uint8_t m_blink_counter;  ///< ������� ������ ��� ������� ��������
    uint8_t m_lcd_init_counter;  ///< ������� ������ ��� ������������� �������

    // true - ���������� ������� �� ����� ������
    bool cursorEnable_;

    // ������� ��������� ������� (����� ������)
    uint8_t cursorLine_;

    // ���-�� ������������ ����������
    uint8_t lineParam_;


#ifdef TEST_FRIENDS
    TEST_FRIENDS
#endif
};

#endif /* MENU_H_ */
