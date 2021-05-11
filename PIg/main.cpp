/*
 * main.cpp
 *
 *  Created on: 03.04.2012
 *      Author: Хозяин
 */
#include <stdlib.h>
#include <stdio.h>
#include "src/glbDefine.h"
#include "src/drivers/keyboard.h"
#include "src/drivers/ks0108.h"
#include "src/drivers/tmp75.h"
#include "src/drivers/uart.h"
#include "src/menu/base.hpp"
#include "src/menu/menu.h"
#include "src/protocols/standart/protocolPcS.h"
#include "src/protocols/standart/protocolBspS.h"
#include "src/protocols/modbus/protocolPcM.h"
#include "src/protocols/iec101/protocolPcI.h"

/// Время работы одного цикла (зависит от настройки таймеров), мс
#define TIME_CYLCE 100

/// адрес датчика температуры
#define TEMP_IC_ADR 0x48

/// Флаг, устанавливается каждые 100мс
static volatile bool b100ms = false;

// Датчики температуры
TTmp75 tmp75(TEMP_IC_ADR);
/// Класс последовательного порта работающего с ПК
TUart uartPC(TUart::PORT_UART1, uBufUartPc, BUFF_SIZE_PC);
/// Класc последовательного порта работающего с БСП
TUart uartBSP(TUart::PORT_UART0, uBufUartBsp, BUFF_SIZE_BSP);

void setupUart(TInterface::INTERFACE intf, uint16_t baudrate,
		TDataBits::DATA_BITS dbits, TParity::PARITY parity,
		TStopBits::STOP_BITS sbits) {

	uartPC.open(baudrate, dbits, parity, sbits);

	if (intf != TInterface::USB) {
		PORTD |= (1 << PD4);
	} else {
		PORTD &= ~(1 << PD4);
	}
}

/** main.c
 *
 *  @param Нет
 *  @return Нет
 */
int __attribute__ ((OS_main))
main(void) {
	uint8_t cnt_wdt = 0;

	// запуск последовательного порта для связи с БСП
	// все настройки фиксированы
	uartBSP.open(4800, TDataBits::_8, TParity::NONE, TStopBits::TWO);

	mainInit();
	sei();

	while (1) {
		if (b100ms) {
			b100ms = false;

			bspRead();
			pcRead();

			mainCycle();

			uartPC.trData(pcWrite());
			uartBSP.trData(bspWrite());

			wdt_reset();

			// запуск процедуры считывания температуры c датчика
			menu.sParam.measParam.setTemperature(tmp75.getTemperature());
			tmp75.readTemp();
		}
	}
}

/// Прерывание по совпадению Таймер0. Срабатывает раз в 50 мкс.
ISR(TIMER0_COMP_vect) {
	// Обработчик ЖКИ
	vLCDmain();
	// подсветка ЖКИ
	vLCDled();

	timer50us();
}

/// Прерывание по совпадению А Таймер1. Срабатывает раз в 10 мс.
ISR(TIMER1_COMPA_vect) {
	static uint_fast8_t cnt = 0;

	// обработчик клавиатуры вызываем раз в 10мс
	vKEYmain();

	// установка флага раз в 100мс
	if (cnt > 0) {
		cnt--;
	}

	if (cnt == 0) {
		b100ms = true;
		cnt = 10;
	}
}

///Прерывание по опустошению передающего буфера UART1
ISR(USART1_UDRE_vect) {
	uartPC.isrUDR();
}

/// Прерывание по окончанию передачи данных UART1
ISR(USART1_TX_vect) {
	uartPC.isrTX();
	pcTxEnd();
}

/// Прерывание по получению данных UART1
ISR(USART1_RX_vect) {
	uint8_t state = UCSR1A;
	uint8_t byte = UDR1;

	bool error = (state & ((1 << FE) | (1 << DOR) | (1 << UPE)));
	pcPushByteFrom(byte, error);
}

/// Прерывание по опустошению передающего буфера UART0
ISR(USART0_UDRE_vect) {
	uartBSP.isrUDR();
}

/// Прерывание по окончанию передачи данных UART0
ISR(USART0_TX_vect) {
	uartBSP.isrTX();
	bspTxEnd();
}

/// Прерывание по получению данных UART0
ISR(USART0_RX_vect) {
	uint8_t state = UCSR0A;
	uint8_t byte = UDR0;

	bool error = state & ((1 << FE) | (1 << DOR) | (1 << UPE));
	bspPushByteFrom(byte, error);
}

/// Прерывание по получению данных TWI
ISR(TWI_vect) {
	uint8_t state = TWSR & 0xF8;

	tmp75.isr(state);

//  TWCR |= (1 << TWINT);
}
