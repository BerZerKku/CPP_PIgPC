/*
 * ks0108.h
 *
 *  Created on: 06.04.2012
 *      Author: Хозяин
 */

#ifndef KS0108_H_
#define KS0108_H_

#include <avr/io.h>

/// Управление подсветкой
enum eLCD_LED
{
	/// Выключение подсветки на постоянку
	LED_OFF,
	/// Включение подсветки на постоянку
	LED_ON,
	/// Выключение подсветки, при отсутствии активности
	LED_SWITCH
};

/// Время работы кратковременной подсветки * 10мс
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

// Управление подсветкой, 0 - выкл
#define PORT_LED	PORTD
#define PIN_LED		(1 << PD5)

// размер массива данных
#define SIZE_BUF_STRING 120
// кол-во точек по горизонтали
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
