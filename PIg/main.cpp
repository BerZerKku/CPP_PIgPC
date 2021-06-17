/*
 * main.cpp
 *
 *  Created on: 03.04.2012
 *      Author: ������
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <stdio.h>
#include "glbDefine.h"
#include "ks0108.h"
#include "menu.h"
#include "keyboard.h"
#include "uart.h"
#include "tmp75.h"
#include "protocolPcS.h"
#include "protocolBspS.h"
#include "protocolPcM.h"
#include "protocolPcI.h"

/// ����� ������ ������ ����� (������� �� ��������� ��������), ��
#define TIME_CYLCE 100

/// ������ ������ ��� ������� � ���
#define BUFF_SIZE_BSP 128

/// ������ ������ ��� ������� � ��
#define BUFF_SIZE_PC 128

/// ������������ ���-�� ������������ ��������� �� ��� ��� ������ �����
#define MAX_LOST_COM_FROM_BSP 10

/// ����� ������ ������������
#define EEPROM_START_ADDRESS 0x10

/// ����� ������� �����������
#define TEMP_IC_ADR 0x48

/// ��������� ���������� ���������� � EEPROM
struct sEeprom {
	uint16_t 				password;	/// ������.
	TInterface::INTERFACE 	interface;	/// ���������.
	TProtocol::PROTOCOL		protocol;	/// ��������.
	TBaudRate::BAUD_RATE 	baudRate;	/// ��������.
	TDataBits::DATA_BITS 	dataBits;	/// ���-�� ��� ������.
	TParity::PARITY 	 	parity;		/// ��������.
	TStopBits::STOP_BITS 	stopBits;	/// ���-�� ����-���.
};

// ��������� �������� ��������� �� ���������������� ������
static bool uartRead();

// ������ �������� ��������� �� ���������������� ������
static bool uartWrite();

/// ����, ��������������� ������ 100��
static volatile bool b100ms = false;

/// ����� ��� ����� � �� �� ����������������� �����
uint8_t uBufUartPc[BUFF_SIZE_PC];
/// ����� ��� ����� � ��� �� ����������������� �����
uint8_t uBufUartBsp[BUFF_SIZE_BSP];

// ��������� �������� � ������
static sEeprom eeprom;

// ������� �����������

TTmp75 tmp75(TEMP_IC_ADR);

/// ����� ����
clMenu menu;
/// ����� ����������������� ����� ����������� � ��
TUart uartPC(TUart::PORT_UART1, uBufUartPc, BUFF_SIZE_PC);
/// ����c ����������������� ����� ����������� � ���
TUart uartBSP(TUart::PORT_UART0, uBufUartBsp, BUFF_SIZE_BSP);
/// ����� ������������ ��������� ����������� � ��
clProtocolPcS protPCs(uBufUartPc, BUFF_SIZE_PC, &menu.sParam);
/// ����� ������ ��������� MODBUS
TProtocolPcM protPCm(&menu.sParam, uBufUartPc, BUFF_SIZE_PC);
/// ����� ��������� ��� 101 ����������� � ��
TProtocolPcI protPCi(&menu.sParam, uBufUartPc, BUFF_SIZE_PC);
/// ����� ������������ ��������� ����������� � ���
clProtocolBspS protBSPs(uBufUartBsp, BUFF_SIZE_BSP, &menu.sParam);


static bool uartRead();
static bool uartWrite();
static void setInterface(TUartData *uart);
static bool isUartPcReinit(sEeprom *current, TUartData *newparam);
static void setProtocol(TProtocol::PROTOCOL protocol, uint16_t baud);

/**	������ � ��������� ������� �� UART
 *
 * 	@param ���
 * 	@return False - � ������ 5-� ������������ ��������� �� ��� ������
 */
static bool uartRead() {
	bool stat = true;
	// ��� ������������� � �� �������
	static uint8_t lastPcCom = 0;
	// ���-�� ������������ ��������� � ���
	static uint8_t cntLostCom = 0;

	// ����� ������� �������� ������ �� ���������
	protBSPs.checkStat();
	// �������� ������� ��������� � ��� � �� ���������
	if (protBSPs.getCurrentStatus() == PRTS_STATUS_READ_OK) {
		// �������� ����������� ����� ����������� ��������� �
		// ��������� ������ ���� ��� ������������� ����������
		if (protBSPs.checkReadData()) {
			// ��������� ��������� ���������
			protBSPs.getData(lastPcCom == protBSPs.getCurrentCom());

			// �������� ������������ ������� ����������� � �� � �������
			// ���������� �� ��� � ���� ��������� ��������� ��������� �� ��
			// ��� ������� GB_COM_GET_VERS ���������� ���������� ������ ���-��
			if (lastPcCom == protBSPs.getCurrentCom()) {
				if (protPCs.isEnable()) {
					if (protPCs.copyCommandFrom(protBSPs.buf))
						protPCs.update();
				}
			}
		}

		// ����� �������� � ��������� ��������� ������� ��� ����������
		// ������������� �������
		lastPcCom = 0;
		// ����� �������� ���������� ��������� � ���
		cntLostCom = 0;
	} else {
		// � ������ ���������� ������ ���������� ��������� ��� ������ � ���
		// ���� ��������� ������������ � False
		if (cntLostCom < MAX_LOST_COM_FROM_BSP)
			cntLostCom++;
		else
			stat = false;
	}

	if (protPCs.isEnable()) {
		// ����� ������� �������� ������ �� ���������
		protPCs.checkStat();
		// �������� ������� ������� � �� � �� ���������
		if (protPCs.getCurrentStatus() == PRTS_STATUS_READ_OK) {
			// �������� ����������� ����� ����������� ��������� �
			// ��������� ������ ���� ��� ������������� ����������
			if (protPCs.checkReadData()) {
				// ��������� ��������� ���������
				// ���� ��������� ������ ����������, �������� ��� � ���
				// (�.�. ���� ��� �� ������/��������� ������)
				if (!protPCs.getData()) {
					// ���������� ������������� �� �������
					lastPcCom = protPCs.getCurrentCom();

					// ��������� ��������� � ���
					if (protBSPs.getCurrentStatus() == PRTS_STATUS_NO) {
						protBSPs.copyCommandFrom(protPCs.buf);
						protPCs.setCurrentStatus(PRTS_STATUS_WAIT_ANSWER);
					}
				}
			}
		}
	} else if (protPCm.isEnable()) {
		if (protPCm.isReadData()) {
			protPCm.readData();
		}
	} else if (protPCi.isEnable()) {
		if (protPCi.isReadData()) {
			protPCi.readData();
		}
	}

	return stat;
}

/**	�������� ������ �� UART.
 *
 * 	@retval True - ������.
 */
static bool uartWrite() {

	if (protPCs.isEnable()) {
		// ����� ��������� �������� ������ ��������� �� ���������.
		protPCs.checkStat();
		// �������� ������������� �������� ������� �� �� � �� ��������
		ePRTS_STATUS stat = protPCs.getCurrentStatus();
		if (stat == PRTS_STATUS_WRITE_PC) {
			// ��������� ������ ���
			uartPC.trData(protPCs.trCom());
		} else if (stat == PRTS_STATUS_WRITE) {
			// �������� ������ ��
			uartPC.trData(protPCs.trCom());
		}
	} else if (protPCm.isEnable()) {
		uartPC.trData(protPCm.sendData());
	} else if (protPCi.isEnable()) {
		uartPC.trData(protPCi.sendData());
	}

	// ����� ��������� �������� ������ ��������� �� ���������.
	protBSPs.checkStat();
	// �������� ��� �� ������������� �������� ������� � ��
	// ���� ���, �� ������� ������� � ����
	ePRTS_STATUS stat = protBSPs.getCurrentStatus();
	if (stat == PRTS_STATUS_WRITE_PC) {
		// ��������� ������� ��
		uartBSP.trData(protBSPs.trCom());
	} else if (stat == PRTS_STATUS_NO) {
		// �������� ������� ���
		eGB_COM com = menu.getTxCommand();
		// ���� ���� �������, ���������� �� � ���
		if (com != GB_COM_NO) {
			uint8_t num = protBSPs.sendData(com);
			uartBSP.trData(num);
		}
	}

	return true;
}

/**	��������� ���������� ����� � �������.
 *
 * 	@param val ������� ���������
 * 	@return ���
 */
static void setInterface(TUartData *uart) {
	TInterface::INTERFACE val = uart->Interface.get();

	uartPC.close();
	// ���� ���� ����� � ��, �� ��������� �������������
	// ���� ���� ����� �� ���.����, �� ��������� ������������
	switch (val) {
		case TInterface::USB:
			// �� ����� ������� ��������� USB ����� �����������
			setProtocol(TProtocol::STANDART, 19200);
			uartPC.open(19200, TDataBits::_8, TParity::NONE,
					TStopBits::TWO);
			break;
		case TInterface::RS485:
			setProtocol(uart->Protocol.get(), uart->BaudRate.getValue());
			uartPC.open(uart->BaudRate.getValue(),
					uart->DataBits.get(),
					uart->Parity.get(),
					uart->StopBits.get());
			break;
		case TInterface::MAX:		// ��������
			break;
	}

	if (val == TInterface::USB) {
		PORTD &= ~(1 << PD4);
	} else {
		PORTD |= (1 << PD4);
	}
}

/** �������� ������������� ����������������� �����.
 *
 * 	������������ ������� ���������, � ������������. � ���� ��� �� ���������
 * 	���������� ���������� ����� ��������.
 *
 * 	@param *current ������� ��������� �����.
 * 	@param *newparam ����������� ��������� �����.
 * 	@retval True, ���� ��������� ����������.
 * 	@retval False, ���� ��������� ���������.
 */
static bool isUartPcReinit(sEeprom *current, TUartData *newparam) {
	bool stat = false;

	if (current->interface != newparam->Interface.get()) {
		current->interface = newparam->Interface.get();
		stat = true;
	}

	if (current->baudRate != newparam->BaudRate.get()) {
		current->baudRate = newparam->BaudRate.get();
		stat = true;
	}

	if (current->dataBits != newparam->DataBits.get()) {
		current->dataBits = newparam->DataBits.get();
		stat = true;
	}

	if (current->parity != newparam->Parity.get()) {
		current->parity = newparam->Parity.get();
		stat = true;
	}

	if (current->stopBits != newparam->StopBits.get()) {
		current->stopBits = newparam->StopBits.get();
		stat = true;
	}

	if (current->protocol != newparam->Protocol.get()) {
		current->protocol = newparam->Protocol.get();
		stat = true;
	}

	return stat;
}

static void setProtocol(TProtocol::PROTOCOL protocol, uint16_t baud) {

	protPCs.setDisable();
	protPCi.setDisable();
	protPCm.setDisable();

	switch(protocol) {
		case TProtocol::STANDART:
			protPCs.setEnable(PRTS_STATUS_NO);
			menu.sParam.jrnScada.setIdle();
//			protPCi.setDisable();
//			protPCm.setDisable();
			break;
		case TProtocol::MODBUS:
			protPCm.setTick(baud, 50);
			protPCm.setEnable();
			menu.sParam.jrnScada.setIdle();
//			protPCi.setDisable();
//			protPCs.setDisable();
			break;
		case TProtocol::IEC_101:
			protPCi.setTick(baud, 50);
			protPCi.setEnable();
			menu.sParam.jrnScada.setReadyToRead();
//			protPCm.setDisable();
//			protPCs.setDisable();
			break;
		case TProtocol::MAX:		// ��������
			break;
	}
}

/**	main.c
 *
 * 	@param ���
 * 	@return ���
 */
int __attribute__ ((OS_main))
main(void) {
	// ������� ��� ���������� ���
	uint8_t cnt_lcd = 0;
	uint8_t cnt_1s = 0;
	uint8_t cnt_wdt = 0;

	// ��������� �������� ������������, ��������� �� ������
	// ���������� �� ��������� ����������, ����� ������ �� ������
	eeprom_read_block(&eeprom, (sEeprom*) EEPROM_START_ADDRESS, sizeof(eeprom));
	EEAR = 0;	// ����� ������ ������ � 0, ��� ������ ������
	menu.sParam.password.init(eeprom.password);
	menu.sParam.Uart.Interface.set(eeprom.interface);
	menu.sParam.Uart.Protocol.set(eeprom.protocol);
	menu.sParam.Uart.BaudRate.set(eeprom.baudRate);
	menu.sParam.Uart.DataBits.set(eeprom.dataBits);
	menu.sParam.Uart.Parity.set(eeprom.parity);
	menu.sParam.Uart.StopBits.set(eeprom.stopBits);

	// ������ ����������������� ����� ��� ����� � ���
	// ��� ��������� �����������
	uartBSP.open(4800,TDataBits::_8,TParity::NONE,TStopBits::TWO);
	protBSPs.setEnable(PRTS_STATUS_NO);

	// ������ ����������������� ����� ��� ����� � ��/��������� ����
	setInterface(&menu.sParam.Uart);

	sei();

	// ��������� ���
	vLCDinit();
	vLCDclear();

	// ������� ��� ��������
	// menu.setTypeDevice(AVANT_NO);

	while (1) {
		if (b100ms) {
			b100ms = false;

			cnt_wdt++;
			// ��������� �������� ��������� � ���/��
			// �������� � ���� �������� ���������� ����� � ���
			bool connect = uartRead();
			menu.setConnectionBsp(connect);

			cnt_wdt++;
			// TODO ���� ����� ����� menu.main(); �� ����� ����� ���������� ��� �������� �����!!!
			// ������ ����������� ��� � 1�
			if (++cnt_1s >= 10) {
				cnt_1s = 0;

				//uint16_t password = menu.sParam.password.get();
				//eeprom_update_word((uint16_t*) EEPROM_PASSWORD, password);

				// ��������� �������� ���������� � ������ �
				// �������� ����������� �����
				// ���� ��� ���������� - ���� ��������������������
				// , � ����� ��������� ���������������� � ������ � ������
				if (isUartPcReinit(&eeprom, &menu.sParam.Uart)) {
					// ��������� ���������� ����� � �������
					setInterface(&menu.sParam.Uart);
				}

				// ���������� �������� ������ � ����� ������
				eeprom.password = menu.sParam.password.get();

				// ��������� �������� ������
				uint8_t nadr = menu.sParam.glb.getNetAddress();
				if (protPCm.getAddressLan() != nadr) {
					protPCm.setAddressLan(nadr);
				} else if (protPCi.getAddressLan() != nadr) {
					protPCi.setAddressLan(nadr);
				}

				// ���������� �������� ������������ � ������
				eeprom_update_block(&eeprom, (sEeprom*) EEPROM_START_ADDRESS,
						sizeof(eeprom));
				EEAR = 0;	// ����� ������ ������ � 0, ��� ������ ������
			}

			cnt_wdt++;
			// ���������� ������
			// ��� 100 - ����� �������� �����
			if (++cnt_lcd >= (MENU_TIME_CYLCE / TIME_CYLCE)) {
				cnt_lcd = 0;
				menu.main();
			}

			cnt_wdt++;
			// �������� ��������� � ���/��
			uartWrite();

			// ����� wdt, ���� ��� ���� ����� ���� ��������
			if (cnt_wdt == 4)
				wdt_reset();
			cnt_wdt = 0;

			// ������ ��������� ���������� ����������� c �������
			menu.sParam.measParam.setTemperature(tmp75.getTemperature());
			tmp75.readTemp();
		}
	}
}

///	���������� �� ���������� ������0. ����������� ��� � 50 ���.
ISR(TIMER0_COMP_vect) {
	// ���������� ���
	vLCDmain();
	// ��������� ���
	vLCDled();

	// ��� ������ ��������� MODBUS
	if (protPCm.isEnable()) {
		protPCm.tick();
	} else if (protPCi.isEnable()) {
		protPCi.tick();
	}
}

///	���������� �� ���������� � ������1. ����������� ��� � 10 ��.
ISR(TIMER1_COMPA_vect) {
	static uint_fast8_t cnt = 0;

	// ���������� ���������� �������� ��� � 10��
	vKEYmain();

	// ��������� ����� ��� � 100��
	cnt = (cnt > 0) ? cnt - 1 : (b100ms = true, 10 - 1);
}

///���������� �� ����������� ����������� ������ UART1
ISR(USART1_UDRE_vect) {
	uartPC.isrUDR();
}

/// ���������� �� ��������� �������� ������ UART1
ISR(USART1_TX_vect) {
	uartPC.isrTX();
	if (protPCs.isEnable()) {
		protPCs.setCurrentStatus(PRTS_STATUS_NO);
	} else if (protPCm.isEnable()) {
		protPCm.setReadState();
	} else if (protPCi.isEnable()) {
		protPCi.setReadState();
	}
}

/// ���������� �� ��������� ������ UART1
ISR(USART1_RX_vect) {
	uint8_t state = UCSR1A;
	uint8_t byte = UDR1;

	if (state & ((1 << FE) | (1 << DOR) | (1 << UPE))) {
		// � ������ ������ ����� �������� �������� ������
		// � �������� ������� ������ ���������
		protPCs.setCurrentStatus(PRTS_STATUS_NO);
	} else {
		if (protPCs.isEnable()) {
			// �������� "�����������"
			protPCs.checkByte(byte);
		} else if (protPCm.isEnable()) {
			// �������� MODBUS
			protPCm.push(byte);
		} else if (protPCi.isEnable()) {
			protPCi.push(byte);
		}
	}
}

///	���������� �� ����������� ����������� ������ UART0
ISR(USART0_UDRE_vect) {
	uartBSP.isrUDR();
}

/// ���������� �� ��������� �������� ������ UART0
ISR(USART0_TX_vect) {
	uartBSP.isrTX();
	protBSPs.setCurrentStatus(PRTS_STATUS_NO);
}

/// ���������� �� ��������� ������ UART0
ISR(USART0_RX_vect) {
	uint8_t state = UCSR0A;
	uint8_t byte = UDR0;

	// � ������ ����������� ������
	if (state & ((1 << FE) | (1 << DOR) | (1 << UPE))) {
		// � ������ ������ ����� �������� �������� ������
		// � �������� ������� ������ ���������
		protBSPs.setCurrentStatus(PRTS_STATUS_NO);
	} else {
		// ���������� ��������� "�����������"
		protBSPs.checkByte(byte);
	}
}

/// ���������� �� ��������� ������ TWI
ISR(TWI_vect) {
	uint8_t state = TWSR & 0xF8;

	tmp75.isr(state);

//	TWCR |= (1 << TWINT);
}
