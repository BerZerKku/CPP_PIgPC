/*
 * setup.cpp
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */

#include "src/avr.h"
#include "src/debug/debug.hpp"

void low_level_init() __attribute__((__naked__)) __attribute__((section(".init3")));

/**	������������� ���������
 * 	�������������� ����� �� ��������� ��������� �� ���� � ��������� � +
 * 	@param ���
 * 	@return ���
 */
void low_level_init()
{
	// ��������� ��������
	SFIOR &= ~(1 << PUD);

	// ���� �
	// ��������������� ����
	DDRA = 0xFF;
	PORTA= 0x00;

	// ���� B
	DDRB = 0x00;
	PORTB = ~0x00;

	// ���� �
	// PORTC.0 = KEY5	- ���� +
	// PORTC.1 = KEY6	- ���� +
	// PORTC.2 = KEY7	- ���� +
	// PORTC.3 = KEY8	- ���� +
	// PORTC.4 = KEY9	- ���� +
	// PORTC.5 = RST	- ����� 0
	// PORTC.6 = CS2	- ����� 0
	// PORTC.7 = CS1	- ����� 0
	DDRC = (1 << DDC5) | (1 << DDC6) | (1 << DDC7);
	PORTC= (char) ~((1 << PC7) | (1 << PC6) | (1 << PC5));

	// ���� D
	// PORTD.0 = SCL	- ������.
	// PORTD.1 = SDA 	- ������.
	// PORTD.2 = RXD1	- ������. ����
	// PORTD.3 = TXD1	- ������. �����
	// PORTD.4 = MUX	- ����� 0 (������������ ����������� 0-USB, 1-485)
	// PORTD.5 = LED	- ����� 0
	// PORTD.6 = KEY1	- ���� +
	// PORTD.7 = KEY2	- ���� +
	DDRD = (1 << DDD5) | (1 << DDD4);
	PORTD= ~((1 << PD5) | (1 << PD4));

	// ���� �
	// PORTE.0 = RXD0	- ������. ����
	// PORTE.1 = TXD0	- ������. �����
	// PORTE.6 = TP1 	- ����� 1
	// PORTE.7 = TP2 	- ����� 1
	DDRE = (1 << PE7) | (1 << PE6);
	PORTE = ~0x00;

	// ���� F
	// PORTF.1 = RS		- ����� 0
	// PORTF.2 = RW		- ����� 0
	// PORTF.3 = E_STR	- ����� 0
	DDRF = (1 << DDF3) | (1 << DDF2) | (1 << DDF1);
	PORTF = ~((1 << PF3) | (1 << PF2) | (1 << PF1));

	// ���� G
	// PORTG.0 = KEY3	- ���� +
	// PORTG.1 = KEY4	- ���� +
	DDRG = 0x00;
	PORTG= ~0x00;

#ifdef DEBUG
	DDR_DBG |= (PIN_TP1 | PIN_TP2);
#endif

	// ��������� ��������
	TIMSK = 0;

	// ������ 0
	// ����� ����� �� ����������
	// �������� N = 32, ���� �� OCR = 24
	// ������� = F_CPU / (2 * N * (1 + OCR)
	// ��� F_CPU = 16��� ������� 10000��
	TCCR0 = (1 << WGM01) | (0 << WGM00) |
			(0 << CS02) | (1 << CS01) | (1 << CS00);
	OCR0  = 24;
	TIMSK |= (1 << OCIE0);

	// ������ 1
	// ����� ����� �� ����������
	// �������� N = 256, ���� �� OCR = 624
	// ������� = F_CPU / (2 * N * (1 + OCR)
	// ��� F_CPU = 16��� ������� 50��
	TCCR1A 	= (0 << WGM11) | (0 << WGM10);
	TCCR1B 	= (0 << WGM13) | (1 << WGM12) |
				(1 << CS12) | (0 << CS11) | (0 << CS10);
	TCCR1C 	= 0;
	TCNT1 	= 0;
	OCR1A 	= 624;
	TIMSK 	|= (1 << OCIE1A);

	wdt_enable(WDTO_250MS);
}



