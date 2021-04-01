/*
 * setup.cpp
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */

#include "src/avr.h"
#include "src/debug/debug.hpp"

void low_level_init() __attribute__((__naked__)) __attribute__((section(".init3")));

/**	Инициализация периферии
 * 	Неиспользуемые порты по умолчанию настроены на вход с подтяжкой к +
 * 	@param Нет
 * 	@return Нет
 */
void low_level_init()
{
	// Включение подтяжки
	SFIOR &= ~(1 << PUD);

	// Порт А
	// Двунаправленная шина
	DDRA = 0xFF;
	PORTA= 0x00;

	// Порт B
	DDRB = 0x00;
	PORTB = ~0x00;

	// Порт С
	// PORTC.0 = KEY5	- вход +
	// PORTC.1 = KEY6	- вход +
	// PORTC.2 = KEY7	- вход +
	// PORTC.3 = KEY8	- вход +
	// PORTC.4 = KEY9	- вход +
	// PORTC.5 = RST	- выход 0
	// PORTC.6 = CS2	- выход 0
	// PORTC.7 = CS1	- выход 0
	DDRC = (1 << DDC5) | (1 << DDC6) | (1 << DDC7);
	PORTC= (char) ~((1 << PC7) | (1 << PC6) | (1 << PC5));

	// Порт D
	// PORTD.0 = SCL	- альтер.
	// PORTD.1 = SDA 	- альтер.
	// PORTD.2 = RXD1	- альтер. вход
	// PORTD.3 = TXD1	- альтер. выход
	// PORTD.4 = MUX	- выход 0 (переключение интерфейсов 0-USB, 1-485)
	// PORTD.5 = LED	- выход 0
	// PORTD.6 = KEY1	- вход +
	// PORTD.7 = KEY2	- вход +
	DDRD = (1 << DDD5) | (1 << DDD4);
	PORTD= ~((1 << PD5) | (1 << PD4));

	// Порт Е
	// PORTE.0 = RXD0	- альтер. вход
	// PORTE.1 = TXD0	- альтер. выход
	// PORTE.6 = TP1 	- выход 1
	// PORTE.7 = TP2 	- выход 1
	DDRE = (1 << PE7) | (1 << PE6);
	PORTE = ~0x00;

	// Порт F
	// PORTF.1 = RS		- выход 0
	// PORTF.2 = RW		- выход 0
	// PORTF.3 = E_STR	- выход 0
	DDRF = (1 << DDF3) | (1 << DDF2) | (1 << DDF1);
	PORTF = ~((1 << PF3) | (1 << PF2) | (1 << PF1));

	// Порт G
	// PORTG.0 = KEY3	- вход +
	// PORTG.1 = KEY4	- вход +
	DDRG = 0x00;
	PORTG= ~0x00;

#ifdef DEBUG
	DDR_DBG |= (PIN_TP1 | PIN_TP2);
#endif

	// Обнуление регистра
	TIMSK = 0;

	// Таймер 0
	// режим сброс по совпадению
	// делитель N = 32, счет до OCR = 24
	// частота = F_CPU / (2 * N * (1 + OCR)
	// при F_CPU = 16МГц получим 10000Гц
	TCCR0 = (1 << WGM01) | (0 << WGM00) |
			(0 << CS02) | (1 << CS01) | (1 << CS00);
	OCR0  = 24;
	TIMSK |= (1 << OCIE0);

	// Таймер 1
	// режим сброс по совпадению
	// делитель N = 256, счет до OCR = 624
	// частота = F_CPU / (2 * N * (1 + OCR)
	// при F_CPU = 16МГц получим 50Гц
	TCCR1A 	= (0 << WGM11) | (0 << WGM10);
	TCCR1B 	= (0 << WGM13) | (1 << WGM12) |
				(1 << CS12) | (0 << CS11) | (0 << CS10);
	TCCR1C 	= 0;
	TCNT1 	= 0;
	OCR1A 	= 624;
	TIMSK 	|= (1 << OCIE1A);

	wdt_enable(WDTO_250MS);
}



