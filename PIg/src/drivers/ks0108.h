/*
 * ks0108.h
 *
 *  Created on: 06.04.2012
 *      Author: Хозяин
 */

#ifndef KS0108_H_
#define KS0108_H_

#include <stdint.h>

/// Время работы кратковременной подсветки * 10мс
#ifdef DEBUG
#define LCD_TIME_LED_ON 1000
#else
#define LCD_TIME_LED_ON 30000
#endif

// размер массива данных
#define SIZE_BUF_STRING 120
#define ROW_LEN 20
// кол-во точек по горизонтали
#define NUM_POINT_HOR 128
#define NUM_POINT_VERT 64
#define NUM_LINE (NUM_PINT_VERT / 8)

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

void vLCDclear		(void);
void vLCDinit		(void);
void vLCDmain		(void);
void vLCDrefresh	(void);
bool vLCDdrawBoard	(uint8_t num);
bool vLCDputchar	(const char* buf, uint8_t num);
void vLCDsetLed		(eLCD_LED val);
void vLCDled		();

#endif /* KS0108_H_ */
