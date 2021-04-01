/*
 * main.cpp
 *
 *  Created on: 03.04.2012
 *      Author: ������
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

/// ����� ������ ������ ����� (������� �� ��������� ��������), ��
#define TIME_CYLCE 100

/// ����� ������� �����������
#define TEMP_IC_ADR 0x48

/// ����, ��������������� ������ 100��
static volatile bool b100ms = false;

// ������� �����������
TTmp75 tmp75(TEMP_IC_ADR);
/// ����� ����������������� ����� ����������� � ��
TUart uartPC(TUart::PORT_UART1, uBufUartPc, BUFF_SIZE_PC);
/// ����c ����������������� ����� ����������� � ���
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
 *  @param ���
 *  @return ���
 */
int __attribute__ ((OS_main))
main(void) {
	uint8_t cnt_wdt = 0;

	// ������ ����������������� ����� ��� ����� � ���
	// ��� ��������� �����������
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

			// ������ ��������� ���������� ����������� c �������
			menu.sParam.measParam.setTemperature(tmp75.getTemperature());
			tmp75.readTemp();
		}
	}
}

/// ���������� �� ���������� ������0. ����������� ��� � 50 ���.
ISR(TIMER0_COMP_vect) {
	// ���������� ���
	vLCDmain();
	// ��������� ���
	vLCDled();

	timer50us();
}

/// ���������� �� ���������� � ������1. ����������� ��� � 10 ��.
ISR(TIMER1_COMPA_vect) {
	static uint_fast8_t cnt = 0;

	// ���������� ���������� �������� ��� � 10��
	vKEYmain();

	// ��������� ����� ��� � 100��
	if (cnt > 0) {
		cnt--;
	}

	if (cnt == 0) {
		b100ms = true;
		cnt = 10;
	}
}

///���������� �� ����������� ����������� ������ UART1
ISR(USART1_UDRE_vect) {
	uartPC.isrUDR();
}

/// ���������� �� ��������� �������� ������ UART1
ISR(USART1_TX_vect) {
	uartPC.isrTX();
	pcTxEnd();
}

/// ���������� �� ��������� ������ UART1
ISR(USART1_RX_vect) {
	uint8_t state = UCSR1A;
	uint8_t byte = UDR1;

	bool error = (state & ((1 << FE) | (1 << DOR) | (1 << UPE)));
	pcPushByteFrom(byte, error);
}

/// ���������� �� ����������� ����������� ������ UART0
ISR(USART0_UDRE_vect) {
	uartBSP.isrUDR();
}

/// ���������� �� ��������� �������� ������ UART0
ISR(USART0_TX_vect) {
	uartBSP.isrTX();
	bspTxEnd();
}

/// ���������� �� ��������� ������ UART0
ISR(USART0_RX_vect) {
	uint8_t state = UCSR0A;
	uint8_t byte = UDR0;

	bool error = state & ((1 << FE) | (1 << DOR) | (1 << UPE));
	bspPushByteFrom(byte, error);
}

/// ���������� �� ��������� ������ TWI
ISR(TWI_vect) {
	uint8_t state = TWSR & 0xF8;

	tmp75.isr(state);

//  TWCR |= (1 << TWINT);
}
