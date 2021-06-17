/*
 * main.cpp
 *
 *  Created on: 03.04.2012
 *      Author: Хозяин
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

/// Время работы одного цикла (зависит от настройки таймеров), мс
#define TIME_CYLCE 100

/// Размер буфера для общения с БСП
#define BUFF_SIZE_BSP 128

/// Размер буфера для общения с ПК
#define BUFF_SIZE_PC 128

/// Максимальное кол-во неполученных сообщений от БСП для ошибки связи
#define MAX_LOST_COM_FROM_BSP 10

/// адрес пароля пользователя
#define EEPROM_START_ADDRESS 0x10

/// адрес датчика температуры
#define TEMP_IC_ADR 0x48

/// Структура параметров хранящихся в EEPROM
struct sEeprom {
	uint16_t 				password;	/// Пароль.
	TInterface::INTERFACE 	interface;	/// Интерфейс.
	TProtocol::PROTOCOL		protocol;	/// Протокол.
	TBaudRate::BAUD_RATE 	baudRate;	/// Скорость.
	TDataBits::DATA_BITS 	dataBits;	/// Кол-во бит данных.
	TParity::PARITY 	 	parity;		/// Четность.
	TStopBits::STOP_BITS 	stopBits;	/// Кол-во стоп-бит.
};

// Обработка принятых сообщений по последовательным портам
static bool uartRead();

// Запуск передачи сообщений по последовательным портам
static bool uartWrite();

/// Флаг, устанавливается каждые 100мс
static volatile bool b100ms = false;

/// Буфер для связи с ПК по последовательному порту
uint8_t uBufUartPc[BUFF_SIZE_PC];
/// Буфер для свящи с БСП по последовательному порту
uint8_t uBufUartBsp[BUFF_SIZE_BSP];

// параметры хранимые в ЕЕПРОМ
static sEeprom eeprom;

// Датчики температуры

TTmp75 tmp75(TEMP_IC_ADR);

/// Класс меню
clMenu menu;
/// Класс последовательного порта работающего с ПК
TUart uartPC(TUart::PORT_UART1, uBufUartPc, BUFF_SIZE_PC);
/// Класc последовательного порта работающего с БСП
TUart uartBSP(TUart::PORT_UART0, uBufUartBsp, BUFF_SIZE_BSP);
/// Класс стандартного протокола работающего с ПК
clProtocolPcS protPCs(uBufUartPc, BUFF_SIZE_PC, &menu.sParam);
/// Класс работы протокола MODBUS
TProtocolPcM protPCm(&menu.sParam, uBufUartPc, BUFF_SIZE_PC);
/// Класс протокола МЭК 101 работающего с ПК
TProtocolPcI protPCi(&menu.sParam, uBufUartPc, BUFF_SIZE_PC);
/// Класс стандартного протокола работающего с БСП
clProtocolBspS protBSPs(uBufUartBsp, BUFF_SIZE_BSP, &menu.sParam);


static bool uartRead();
static bool uartWrite();
static void setInterface(TUartData *uart);
static bool isUartPcReinit(sEeprom *current, TUartData *newparam);
static void setProtocol(TProtocol::PROTOCOL protocol, uint16_t baud);

/**	Работа с принятыми данными по UART
 *
 * 	@param Нет
 * 	@return False - в случае 5-и неполученных сообщений от БСП подряд
 */
static bool uartRead() {
	bool stat = true;
	// код запрашиваемой с ПК команды
	static uint8_t lastPcCom = 0;
	// кол-во неполученных сообщений с БСП
	static uint8_t cntLostCom = 0;

	// перед приемом проверим статус на залипание
	protBSPs.checkStat();
	// Проверка наличия сообщения с БСП и ее обработка
	if (protBSPs.getCurrentStatus() == PRTS_STATUS_READ_OK) {
		// проверка контрольной суммы полученного сообщения и
		// обработка данных если она соответствует полученной
		if (protBSPs.checkReadData()) {
			// обработка принятого сообщения
			protBSPs.getData(lastPcCom == protBSPs.getCurrentCom());

			// проверка соответствия команды запрошенной с ПК и команды
			// полученной от БСП и если совпадают пересылка сообщения на ПК
			// для команды GB_COM_GET_VERS происходит добавление версии БСП-ПИ
			if (lastPcCom == protBSPs.getCurrentCom()) {
				if (protPCs.isEnable()) {
					if (protPCs.copyCommandFrom(protBSPs.buf))
						protPCs.update();
				}
			}
		}

		// после принятия и обработки сообщения сбросим код предыдущей
		// запрашиваемой команды
		lastPcCom = 0;
		// сброс счетчика потерянных сообщений с БСП
		cntLostCom = 0;
	} else {
		// в случае превышения порога потерянных сообщений при обмене с БСП
		// флаг состояния сбрасывается в False
		if (cntLostCom < MAX_LOST_COM_FROM_BSP)
			cntLostCom++;
		else
			stat = false;
	}

	if (protPCs.isEnable()) {
		// перед приемом проверим статус на залипание
		protPCs.checkStat();
		// проверка наличия команды с ПК и ее обработка
		if (protPCs.getCurrentStatus() == PRTS_STATUS_READ_OK) {
			// проверка контрольной суммы полученного сообщения и
			// обработка данных если она соответствует полученной
			if (protPCs.checkReadData()) {
				// обработка принятого сообщения
				// если сообщение небыло обработано, перешлем его в БСП
				// (т.е. если это не запрос/изменение пароля)
				if (!protPCs.getData()) {
					// сохранение запрашиваемой ПК команды
					lastPcCom = protPCs.getCurrentCom();

					// пересылка сообщения в БСП
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

/**	Передача данных по UART.
 *
 * 	@retval True - всегда.
 */
static bool uartWrite() {

	if (protPCs.isEnable()) {
		// Перед передачей проверим статус протокола на залипание.
		protPCs.checkStat();
		// проверка необходимости передачи команды на ПК и ее отправка
		ePRTS_STATUS stat = protPCs.getCurrentStatus();
		if (stat == PRTS_STATUS_WRITE_PC) {
			// пересылка ответа БСП
			uartPC.trData(protPCs.trCom());
		} else if (stat == PRTS_STATUS_WRITE) {
			// отправка ответа ПИ
			uartPC.trData(protPCs.trCom());
		}
	} else if (protPCm.isEnable()) {
		uartPC.trData(protPCm.sendData());
	} else if (protPCi.isEnable()) {
		uartPC.trData(protPCi.sendData());
	}

	// Перед передачей проверим статус протокола на залипание.
	protBSPs.checkStat();
	// проверим нет ли необходимости передачи команды с ПК
	// если нет, то возьмем команду с МЕНЮ
	ePRTS_STATUS stat = protBSPs.getCurrentStatus();
	if (stat == PRTS_STATUS_WRITE_PC) {
		// пересылка запроса ПК
		uartBSP.trData(protBSPs.trCom());
	} else if (stat == PRTS_STATUS_NO) {
		// отправка запроса БСП
		eGB_COM com = menu.getTxCommand();
		// если есть команда, отправляем ее в БСП
		if (com != GB_COM_NO) {
			uint8_t num = protBSPs.sendData(com);
			uartBSP.trData(num);
		}
	}

	return true;
}

/**	Установка интерфейса связи с АВАНТом.
 *
 * 	@param val Текущий интерфейс
 * 	@return Нет
 */
static void setInterface(TUartData *uart) {
	TInterface::INTERFACE val = uart->Interface.get();

	uartPC.close();
	// если идет связь с ПК, то настройки фиксированные
	// если идет связь по Лок.сети, то настройки пользователя
	switch (val) {
		case TInterface::USB:
			// во время отладки интерфейс USB можно настраивать
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
		case TInterface::MAX:		// заглушка
			break;
	}

	if (val == TInterface::USB) {
		PORTD &= ~(1 << PD4);
	} else {
		PORTD |= (1 << PD4);
	}
}

/** Проверка необходимости переинициализации порта.
 *
 * 	Сравниваются текущие настройки, с необходимыми. И если они не совпадают
 * 	происходит сохранение новых значений.
 *
 * 	@param *current Текущие настройки порта.
 * 	@param *newparam Необходимые настройки порта.
 * 	@retval True, если настройки отличаются.
 * 	@retval False, если настройки совпадают.
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
		case TProtocol::MAX:		// заглушка
			break;
	}
}

/**	main.c
 *
 * 	@param Нет
 * 	@return Нет
 */
int __attribute__ ((OS_main))
main(void) {
	// счетчик для обновления ЖКИ
	uint8_t cnt_lcd = 0;
	uint8_t cnt_1s = 0;
	uint8_t cnt_wdt = 0;

	// установка настроек пользователя, считанных из ЕЕПРОМ
	// проводится до включения прерываний, чтобы ничего не мешало
	eeprom_read_block(&eeprom, (sEeprom*) EEPROM_START_ADDRESS, sizeof(eeprom));
	EEAR = 0;	// сброс адреса ЕЕПРОМ в 0, для защиты данных
	menu.sParam.password.init(eeprom.password);
	menu.sParam.Uart.Interface.set(eeprom.interface);
	menu.sParam.Uart.Protocol.set(eeprom.protocol);
	menu.sParam.Uart.BaudRate.set(eeprom.baudRate);
	menu.sParam.Uart.DataBits.set(eeprom.dataBits);
	menu.sParam.Uart.Parity.set(eeprom.parity);
	menu.sParam.Uart.StopBits.set(eeprom.stopBits);

	// запуск последовательного порта для связи с БСП
	// все настройки фиксированы
	uartBSP.open(4800,TDataBits::_8,TParity::NONE,TStopBits::TWO);
	protBSPs.setEnable(PRTS_STATUS_NO);

	// запуск последовательного порта для связи с ПК/Локальной сети
	setInterface(&menu.sParam.Uart);

	sei();

	// настройка ЖКИ
	vLCDinit();
	vLCDclear();

	// зададим тип аппарата
	// menu.setTypeDevice(AVANT_NO);

	while (1) {
		if (b100ms) {
			b100ms = false;

			cnt_wdt++;
			// обработка принятых сообщений с БСП/ПК
			// передача в МЕНЮ текущего сосотояния связи с БСП
			bool connect = uartRead();
			menu.setConnectionBsp(connect);

			cnt_wdt++;
			// TODO Если стоит после menu.main(); то сбоит смена интерфейса или настроек порта!!!
			// задачи выполняемые раз в 1с
			if (++cnt_1s >= 10) {
				cnt_1s = 0;

				//uint16_t password = menu.sParam.password.get();
				//eeprom_update_word((uint16_t*) EEPROM_PASSWORD, password);

				// сравнение занчений записанных в ЕЕПРОМ с
				// текущими настройками порта
				// если они отличаются - порт переинициализируется
				// , а новые настройки подготавливаются к записи в ЕЕПРОМ
				if (isUartPcReinit(&eeprom, &menu.sParam.Uart)) {
					// установка интерфейса связи с АВАНТом
					setInterface(&menu.sParam.Uart);
				}

				// считывание текущего пароля в буфер ЕЕПРОМ
				eeprom.password = menu.sParam.password.get();

				// установка сетевого адреса
				uint8_t nadr = menu.sParam.glb.getNetAddress();
				if (protPCm.getAddressLan() != nadr) {
					protPCm.setAddressLan(nadr);
				} else if (protPCi.getAddressLan() != nadr) {
					protPCi.setAddressLan(nadr);
				}

				// обновление настроек пользователя в ЕЕПРОМ
				eeprom_update_block(&eeprom, (sEeprom*) EEPROM_START_ADDRESS,
						sizeof(eeprom));
				EEAR = 0;	// сброс адреса ЕЕПРОМ в 0, для защиты данных
			}

			cnt_wdt++;
			// обновление экрана
			// где 100 - время рабочего цикла
			if (++cnt_lcd >= (MENU_TIME_CYLCE / TIME_CYLCE)) {
				cnt_lcd = 0;
				menu.main();
			}

			cnt_wdt++;
			// отправка сообщений в БСП/ПК
			uartWrite();

			// сброс wdt, если все шаги цикла были пройдены
			if (cnt_wdt == 4)
				wdt_reset();
			cnt_wdt = 0;

			// запуск процедуры считывания температуры c датчика
			menu.sParam.measParam.setTemperature(tmp75.getTemperature());
			tmp75.readTemp();
		}
	}
}

///	Прерывание по совпадению Таймер0. Срабатывает раз в 50 мкс.
ISR(TIMER0_COMP_vect) {
	// Обработчик ЖКИ
	vLCDmain();
	// подсветка ЖКИ
	vLCDled();

	// для работы протокола MODBUS
	if (protPCm.isEnable()) {
		protPCm.tick();
	} else if (protPCi.isEnable()) {
		protPCi.tick();
	}
}

///	Прерывание по совпадению А Таймер1. Срабатывает раз в 10 мс.
ISR(TIMER1_COMPA_vect) {
	static uint_fast8_t cnt = 0;

	// обработчик клавиатуры вызываем раз в 10мс
	vKEYmain();

	// установка флага раз в 100мс
	cnt = (cnt > 0) ? cnt - 1 : (b100ms = true, 10 - 1);
}

///Прерывание по опустошению передающего буфера UART1
ISR(USART1_UDRE_vect) {
	uartPC.isrUDR();
}

/// Прерывание по окончанию передачи данных UART1
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

/// Прерывание по получению данных UART1
ISR(USART1_RX_vect) {
	uint8_t state = UCSR1A;
	uint8_t byte = UDR1;

	if (state & ((1 << FE) | (1 << DOR) | (1 << UPE))) {
		// в случае ошибки сброс счетчика принятых данных
		// и текущего статуса работы протокола
		protPCs.setCurrentStatus(PRTS_STATUS_NO);
	} else {
		if (protPCs.isEnable()) {
			// протокол "Стандартный"
			protPCs.checkByte(byte);
		} else if (protPCm.isEnable()) {
			// протокол MODBUS
			protPCm.push(byte);
		} else if (protPCi.isEnable()) {
			protPCi.push(byte);
		}
	}
}

///	Прерывание по опустошению передающего буфера UART0
ISR(USART0_UDRE_vect) {
	uartBSP.isrUDR();
}

/// Прерывание по окончанию передачи данных UART0
ISR(USART0_TX_vect) {
	uartBSP.isrTX();
	protBSPs.setCurrentStatus(PRTS_STATUS_NO);
}

/// Прерывание по получению данных UART0
ISR(USART0_RX_vect) {
	uint8_t state = UCSR0A;
	uint8_t byte = UDR0;

	// В случае обнаружения ошибки
	if (state & ((1 << FE) | (1 << DOR) | (1 << UPE))) {
		// в случае ошибки сброс счетчика принятых данных
		// и текущего статуса работы протокола
		protBSPs.setCurrentStatus(PRTS_STATUS_NO);
	} else {
		// обработчик протокола "Стандартный"
		protBSPs.checkByte(byte);
	}
}

/// Прерывание по получению данных TWI
ISR(TWI_vect) {
	uint8_t state = TWSR & 0xF8;

	tmp75.isr(state);

//	TWCR |= (1 << TWINT);
}
