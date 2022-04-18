/*
 * main.cpp
 *
 *  Created on: 03.04.2012
 *      Author: ������
 */
#include "avr.h"
#include "drivers/keyboard.h"
#include "drivers/ks0108.h"
#include "drivers/tmp75.h"
#include "drivers/uart.h"
#include "menu/base.h"
#include "menu/menu.h"
#include "protocols/standart/protocolS.h"
#include <stdio.h>
#include <stdlib.h>

/// ����� ������ ������ ����� (������� �� ��������� ��������), ��
#define TIME_CYLCE 100

/// ����� ������� �����������
#define TEMP_IC_ADR 0x48

// ����, ��������������� ������ 100��
static volatile bool b100ms = false;

// ������� �����������
static TTmp75 tmp75(TEMP_IC_ADR);
/// ����c ����������������� ����� ����������� � ���
static TUart uartBSP(TUart::PORT_UART0, uBufUartBsp, BUFF_SIZE_BSP);


/** main.c
 *
 *  @param ���
 *  @return ���
 */
int main(void)
{
    // ������ ����������������� ����� ��� ����� � ���
    // ��� ��������� �����������
    uartBSP.open(19200, TDataBits::_8, TParity::EVEN, TStopBits::ONE);

    mainInit();

    sei();

    while (1)
    {
        if (b100ms)
        {
            b100ms = false;

            bspRead();

            mainCycle();

            uartBSP.trData(bspWrite());


            // ������ ��������� ���������� ����������� c �������
            // menu.sParam.measParam.setTemperature(tmp75.getTemperature());
            tmp75.readTemp();
        }

        wdt_reset();
    }
}

/// ���������� �� ���������� ������0. ����������� ��� � 50 ���.
ISR(TIMER0_COMP_vect)
{
    // ���������� ���
    vLCDmain();
    // ��������� ���
    vLCDled();
}

/// ���������� �� ���������� � ������1. ����������� ��� � 10 ��.
ISR(TIMER1_COMPA_vect)
{
    static uint_fast8_t cnt = 0;

    // ���������� ���������� �������� ��� � 10��
    vKEYmain();

    // ��������� ����� ��� � 100��
    if (cnt > 0)
    {
        cnt--;
    }

    if (cnt == 0)
    {
        b100ms = true;
        cnt    = 10;
    }
}


/// ���������� �� ����������� ����������� ������ UART0
ISR(USART0_UDRE_vect)
{
    uartBSP.isrUDR();
}

/// ���������� �� ��������� �������� ������ UART0
ISR(USART0_TX_vect)
{
    uartBSP.isrTX();
    bspTxEnd();
}

/// ���������� �� ��������� ������ UART0
ISR(USART0_RX_vect)
{
    uint8_t state = UCSR0A;
    uint8_t byte  = UDR0;

    bool error = state & ((1 << FE0) | (1 << DOR0) | (1 << UPE0));
    bspPushByteFrom(byte, error);
}

/// ���������� �� ��������� ������ TWI
ISR(TWI_vect)
{
    uint8_t state = TWSR & 0xF8;

    tmp75.isr(state);

    TWCR |= (1 << TWINT);
}

/* ******************************** E N D *********************************** */
