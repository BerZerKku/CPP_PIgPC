/*
 * ks0108.h
 *
 *  Created on: 06.04.2012
 *      Author: ������
 */

#ifndef KS0108_H_
#define KS0108_H_

#include <stdint.h>

/// ����� ������ ��������������� ��������� * 10��
#ifdef DEBUG
#define LCD_TIME_LED_ON 1000
#else
#define LCD_TIME_LED_ON 30000
#endif

// ������ ������� ������
#define SIZE_BUF_STRING 120
#define ROW_LEN 20
// ���-�� ����� �� �����������
#define NUM_POINT_HOR 128
#define NUM_POINT_VERT 64
#define NUM_LINE (NUM_PINT_VERT / 8)

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

void vLCDclear		(void);
void vLCDinit		(void);
void vLCDmain		(void);
void vLCDrefresh	(void);
bool vLCDdrawBoard	(uint8_t num);
bool vLCDputchar	(const char* buf, uint8_t num);
void vLCDsetLed		(eLCD_LED val);
void vLCDled		();

#endif /* KS0108_H_ */
