/*
 * keyboard.cpp
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */

#include "keyboard.h"

/// ����� ������������ * 10��
#define TIME_DELAY 10
/// ����� �� ���������� �����������
#define TIME_DELAY_REPEAT 100

//
#define PRESS_TIME_STEP 5
#define PRESS_TIME_MAX  10
/// ����� ������� �� ������ (0..10)
static uint_fast8_t timePress = 0;

// ���� ������
// 1R1C - ����� ������ (1..3) � ������� (1..3)
// F - �������������� �������
enum eBUT
{
    // ���� ������, ��������� �������
    BUT_NO = 0x00,
    BUT_1R1C,
    BUT_1R2C,
    BUT_1R3C,
    BUT_2R1C,
    BUT_2R2C,
    BUT_2R3C,
    BUT_3R1C,
    BUT_3R2C,
    BUT_3R3C,

    // ���� ������, ��� ������� ������ �������
    BUT_F = 0x80,
    BUT_F_1R1C,
    BUT_F_1R2C,
    BUT_F_1R3C,
    BUT_F_2R1C,
    BUT_F_2R2C,
    BUT_F_2R3C,
    BUT_F_3R1C,
    BUT_F_3R2C,
    BUT_F_3R3C
};

/// ��� ������� ������
static eBUT keyPressed;


/**	���������� �������� ������� ������
 * 	@param ���
 * 	@return eKEY ��� ������� ������
 */
uint8_t eKEYget(void)
{
    uint_fast8_t but = keyPressed;

    if ((but >= BUT_1R1C) && (but <= BUT_3R3C))
    {
        but = 1 + (but - BUT_1R1C);
    }
    else if ((but >= BUT_F_1R1C) && (but <= BUT_F_3R3C))
    {
        but = 10 + (but - BUT_F_1R1C);
    }
    else
    {
        but = 0;
    }

    keyPressed = BUT_NO;
    return but;
}

/** ���������� ���� ����������� �������.
 *  ����� ������, ���� ����������.
 *  @retval True - ���������� ������� ������.
 *  @retval False - �� ���������� ������� ������
 *
 */
uint8_t timePressKey()
{
    return (timePress / PRESS_TIME_STEP);
}

/**	������� ����������� ������� ������
 *	�������� ���� ��� � 10��
 *	������� ����� ������ "�������" ������������
 *	��������� ������������ ������ TIME_DELAY_REPEAT * (������ ������ �-��)
 *	����� ������������ TIME_DELAY * (������ ������ �-��)
 * 	@param ���
 * 	@return ���
 */
void vKEYmain(void)
{
    static uint_fast8_t delay   = TIME_DELAY;  // ������� ������������
    static uint_fast8_t keyPrev = BUT_NO;      // ���������� �������� ������
    uint_fast8_t        tmp;                   // ��������� ����������
    uint_fast8_t        tmpKey = BUT_NO;       // ��������� ����������

    tmp = (~PIND) & 0xC0;
    // ������� �������� ������� ������ "�������"
    if (tmp & 0x80)
        tmpKey = BUT_F;

    // ��������� ����������, �� ����������� ������ ������� ������
    if (tmp & 0x40)
        tmpKey |= BUT_3R1C;
    else
    {
        tmp = (~PING) & 0x03;
        if (tmp)
        {
            if (tmp & 0x01)
                tmpKey |= BUT_2R1C;
            else if (tmp & 0x02)
                tmpKey |= BUT_1R2C;
        }
        else
        {
            tmp = ~(PINC) &0x1F;
            if (tmp)
            {
                if (tmp & 0x01)
                    tmpKey |= BUT_1R3C;
                else if (tmp & 0x02)
                    tmpKey |= BUT_2R2C;
                else if (tmp & 0x04)
                    tmpKey |= BUT_2R3C;
                else if (tmp & 0x08)
                    tmpKey |= BUT_3R2C;
                else if (tmp & 0x10)
                    tmpKey |= BUT_3R3C;
            }
        }
    }

    // �������� �� ���������� � ���������� ���������
    // ���� �� �������, �������� ������� ������������
    // ���� ������ �� ������, ����������� �� ���������
    // ���� ������ ������������ ���������� �����,
    // ������������ ������������ � �������� TIME_DELAY_REPEAT
    if (tmpKey != keyPrev)
    {
        keyPrev   = tmpKey;
        tmpKey    = BUT_NO;
        delay     = TIME_DELAY;
        timePress = 0;
    }
    else
    {
        if (delay > 0)
        {
            if (tmpKey != BUT_NO)
            {
                tmpKey = BUT_NO;
                delay--;
            }
        }
        else
        {
            delay = TIME_DELAY_REPEAT;
            if (timePress < PRESS_TIME_MAX)
            {
                timePress++;
            }
        }
    }

    // ���� ���������� ������� ��� ����������, ������ ����� �������
    // ������� ����� ������ ������� - ������������
    if (keyPressed == BUT_NO)
    {
        keyPressed = (eBUT) tmpKey;
    }
}
