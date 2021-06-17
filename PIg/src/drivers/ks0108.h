/*
 * ks0108.h
 *
 *  Created on: 06.04.2012
 *      Author: ������
 */

#ifndef KS0108_H_
#define KS0108_H_

#include <avr/io.h>

/// ���������� ����������
enum eLCD_LED
{
	/// ���������� ��������� �� ���������
	LED_OFF,
	/// ��������� ��������� �� ���������
	LED_ON,
	/// ���������� ���������, ��� ���������� ����������
	LED_SWITCH
};

/// ����� ������ ��������������� ��������� * 10��
#ifdef DEBUG
#define LCD_TIME_LED_ON 1000
#else
#define LCD_TIME_LED_ON 30000
#endif

#define PORT_RST 	PORTC
#define PIN_RST 	(1 << 5)
#define PORT_CS 	PORTC
#define PIN_CS2 	(1 << 6)
#define PIN_CS1 	(1 << 7)
#define PIN_CS  	(PIN_CS1 | PIN_CS2)

#define PORT_E 		PORTF
#define PIN_E   	(1 << 3)
#define PORT_RW 	PORTF
#define PIN_RW  	(1 << 2)
#define PORT_RS 	PORTF
#define PIN_RS  	(1 << 1)

// ���������� ����������, 0 - ����
#define PORT_LED	PORTD
#define PIN_LED		(1 << PD5)

// ������ ������� ������
#define SIZE_BUF_STRING 120
// ���-�� ����� �� �����������
#define NUM_POINT_HOR 128
#define NUM_POINT_VERT 64
#define NUM_LINE (NUM_PINT_VERT / 8)

void vLCDclear		(void);
void vLCDinit		(void);
void vLCDmain		(void);
void vLCDrefresh	(void);
bool vLCDdrawBoard	(uint8_t num);
bool vLCDputchar	(const char* buf, uint8_t num);
void vLCDsetLed		(eLCD_LED val);
void vLCDled		();

#endif /* KS0108_H_ */
