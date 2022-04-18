/*
 * main.cpp
 *
 *  Created on: 03.04.2012
 *      Author: Хозяин
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

/// Время работы одного цикла (зависит от настройки таймеров), мс
#define TIME_CYLCE 100

/// адрес датчика температуры
#define TEMP_IC_ADR 0x48

// Флаг, устанавливается каждые 100мс
static volatile bool b100ms = false;

// Датчики температуры
static TTmp75 tmp75(TEMP_IC_ADR);
/// Класc последовательного порта работающего с БСП
static TUart uartBSP(TUart::PORT_UART0, uBufUartBsp, BUFF_SIZE_BSP);


/** main.c
 *
 *  @param Нет
 *  @return Нет
 */
int main(void)
{
    // запуск последовательного порта для связи с БСП
    // все настройки фиксированы
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


            // запуск процедуры считывания температуры c датчика
            // menu.sParam.measParam.setTemperature(tmp75.getTemperature());
            tmp75.readTemp();
        }

        wdt_reset();
    }
}

/// Прерывание по совпадению Таймер0. Срабатывает раз в 50 мкс.
ISR(TIMER0_COMP_vect)
{
    // Обработчик ЖКИ
    vLCDmain();
    // подсветка ЖКИ
    vLCDled();
}

/// Прерывание по совпадению А Таймер1. Срабатывает раз в 10 мс.
ISR(TIMER1_COMPA_vect)
{
    static uint_fast8_t cnt = 0;

    // обработчик клавиатуры вызываем раз в 10мс
    vKEYmain();

    // установка флага раз в 100мс
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


/// Прерывание по опустошению передающего буфера UART0
ISR(USART0_UDRE_vect)
{
    uartBSP.isrUDR();
}

/// Прерывание по окончанию передачи данных UART0
ISR(USART0_TX_vect)
{
    uartBSP.isrTX();
    bspTxEnd();
}

/// Прерывание по получению данных UART0
ISR(USART0_RX_vect)
{
    uint8_t state = UCSR0A;
    uint8_t byte  = UDR0;

    bool error = state & ((1 << FE0) | (1 << DOR0) | (1 << UPE0));
    bspPushByteFrom(byte, error);
}

/// Прерывание по получению данных TWI
ISR(TWI_vect)
{
    uint8_t state = TWSR & 0xF8;

    tmp75.isr(state);

    TWCR |= (1 << TWINT);
}

/* ******************************** E N D *********************************** */
