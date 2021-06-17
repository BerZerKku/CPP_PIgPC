#include <math.h>
#include <util/delay.h>
#include <stdio.h>
#include "ks0108.h"
#include "symbols.h"

bool vLCDcheckBusy	(void);
void vLCDsetXY		(uint8_t x, uint8_t y);
void vLCDcom		(uint8_t com, uint8_t cs);
void vLCDdata		(uint8_t data, uint8_t cs);

/// ����� ���-�� ��������� �� ���
static uint8_t uBuf[1024];
/// ������� ��������� � ������ �����
static uint16_t uCnt = 0;
/// ����� �� ���������� ��������� * 100 ���
static uint16_t uLedTimeOn = LCD_TIME_LED_ON;
/// ��������� ���������
static eLCD_LED eLed = LED_OFF;

/// ���������� ����������
static volatile bool bRefresh = false;

static void vLCDdrawSymb(uint16_t poz, uint8_t val);

/**	�������� ����� ��������� ���
 * 	@param ���
 * 	@return false - ������.
 */
bool vLCDcheckBusy(void) {
	uint8_t data = 0;

	PORT_RS &= ~PIN_RS;
	DDRA = 0;
	PORT_CS |= PIN_CS;
	PORT_RW |= PIN_RW;
	asm volatile ( "nop\n\t""nop\n\t"::);
	PORT_E |= PIN_E;
	asm volatile ( "nop\n\t""nop\n\t"::);
	data = PINA;
	PORT_E &= ~PIN_E;
	_delay_us(4);
	PORT_CS &= ~PIN_CS;
	PORT_E |= PIN_E;
	DDRA = 0xFF;

	if (data & 0x80)
		return false;
	else
		return true;
}

/** ������ ������� � ���
 * 	@param com �������
 * 	@param cs ����� ���������
 * 	@return ���
 */
void vLCDcom(uint8_t com, uint8_t cs) {
	PORT_CS |= cs;
	PORT_RS &= ~PIN_RS;
	PORT_RW &= ~PIN_RW;
	asm volatile ( "nop\n\t""nop\n\t"::);
	PORT_E |= PIN_E;
	PORTA = com;
	asm volatile ( "nop\n\t""nop\n\t"::);
	PORT_E &= ~PIN_E;
	_delay_us(1);
	PORT_CS &= ~cs;
}

/** ������� �������
 * 	@param ���
 * 	@return ���
 */
void vLCDclear(void) {
	for (uint_fast16_t i = 0; i < 1024; i++) {
		uBuf[i] = 0;
	}
}

/** ������ ������ � ���
 * 	@param data ���� ������, ������������ � ���
 * 	@param cs ����� ���������
 * 	@return ���
 */
void vLCDdata(uint8_t data, uint8_t cs) {
	PORT_CS |= cs;
	PORT_RS |= PIN_RS;
	PORT_RW &= ~PIN_RW;
	asm volatile ( "nop\n\t""nop\n\t"::);
	PORTA = data;
	PORT_E |= PIN_E;
	asm volatile ( "nop\n\t""nop\n\t"::);
	PORT_E &= ~PIN_E;
	_delay_us(2);
	PORT_CS &= ~cs;
}

/** ������ ������ � ���
 * 	@param data ���� ������, ���������� � �����
 * 	@return ���
 */
void vLCDwriteData(uint8_t data) {
	uBuf[uCnt++] = data;
}

/** ������������� �������
 * 	@param ���
 * 	@return ���
 */
void vLCDinit(void) {
	if (!(PORT_RST & PIN_RST)) {
		PORT_RST |= PIN_RST;
		_delay_ms(10);
	}
	vLCDsetXY(0, 0);
	_delay_us(2);
	vLCDcom(0xc4, PIN_CS);
	_delay_us(2);
	vLCDcom(0x3f, PIN_CS);
	_delay_us(5);
}

/**	��������� ������� ���������� ���
 * 	@param x ����� �������
 * 	@param y ����� ������
 * 	@return ���
 */
void vLCDsetXY(uint8_t x, uint8_t y) {
	uCnt = (y * NUM_POINT_HOR) + x - 1;
}

/** ����� ���������� ����������� ���
 * 	@param ���
 * 	@return ���
 */
void vLCDrefresh(void) {
	bRefresh = true;
}

/** ���������� ���
 * 	@param ���
 * 	@return ���
 */
void vLCDmain(void) {
	static uint8_t x = 0;
	static uint8_t y = 0;

	if (bRefresh) {
		if (x == 0) {
			vLCDcom(0xb8 + y, PIN_CS);
			_delay_us(5);
			vLCDcom(0x40, PIN_CS);
			_delay_us(5);
		}

		if (x < 64) {
			vLCDdata(uBuf[x + y * NUM_POINT_HOR], PIN_CS1);
			vLCDdata(uBuf[x + 64 + y * NUM_POINT_HOR], PIN_CS2);
			x++;
		} else if (y < 7) {
			y++;
			x = 0;
		} else {
			y = 0;
			x = 0;
			bRefresh = false;
		}
	}
}

/** �������������� ������� ��������, � ������ ��� ������ �� ���
 * 	@param buf ��������� �� ����� ������ ���
 *	@param num ���-�� ����� ��� ����������
 *	@return true - � ������ �������� ������
 */
bool vLCDputchar(const char* buf, uint8_t num) {
	uint_fast16_t poz = 0;

	if (num > 7)
		return false;

	// ����� ����������
	// ������ : ������ ������ ������� + �������� �� ���� �� 4 �����
	poz = NUM_POINT_HOR + 4;
	for (uint_fast8_t i = 0; i < 20 * num; i++, poz += 6) {
		vLCDdrawSymb(poz, *buf++);

		// ���� �������� ����� ������, ������� ������� �� 8 �������
		if ((i % 20) == 19)
			poz += 8;
	}

	// ����� ��������� ����
	// ������ : ����� ������ ������� + �������� �� ���� �� 4 ������
	poz = (num + 2) * NUM_POINT_HOR + 4;
	for (uint_fast8_t i = 20 * num; i < SIZE_BUF_STRING; i++, poz += 6) {
		vLCDdrawSymb(poz, *buf++);

		// ���� �������� ����� ������. ������� ������� �� 10 �������
		if ((i % 20) == 19)
			poz += 8;
	}

	return true;
}

/**	��������� ������ ������ ����������
 * 	@param val ���� �� ��������:
 * 	@arg LED_ON �������� ���������� ���������
 * 	@arg LED_SWITCH �������� ��������� ���������
 * 	@arg LED_OFF ��������� ���������
 * 	@return ���
 */
void vLCDsetLed(eLCD_LED val) {
	if (val == LED_SWITCH)
		uLedTimeOn = LCD_TIME_LED_ON;
	else
		eLed = val;
}

/** ���������� ���������� (������������ ���).
 * 	������� �������� �������� ������ ������ �������
 * 	@param ���
 * 	@return ���
 */
void vLCDled(void) {
	// ������� 200 ������ , �.�. ������ 10��
	static uint8_t cnt = 200 - 1;

	// ������ � ����������
	if (eLed == LED_ON)
		PORT_LED ^= PIN_LED;
	else if (uLedTimeOn > 0) {
		PORT_LED ^= PIN_LED;
		if (cnt > 0)
			cnt--;
		else {
			cnt = 200 - 1;
			uLedTimeOn--;
		}
	} else
		PORT_LED &= ~PIN_LED;
}

/** ��������� �����
 * 	@param num ���-�� ����� ���������� 1..3
 * 	@return true - � ������ �������� ���������
 */
bool vLCDdrawBoard(uint8_t num) {
	// ������ ��������� ���-�� ����� ��� ����������� ����������
	if (num > 6)
		return false;

	uint16_t poz = 0;

	// �������� �������� �����

	// �������������� �������
	for (uint8_t i = 3; i < 125; i++) {
		uBuf[i] = 0x5A;
	}

	// ������������ �������
	poz = 128;
	for (uint8_t i = 0; i < 7; i++, poz += NUM_POINT_HOR) {
		uBuf[poz] = 0xff;
		uBuf[poz + 2] = 0xff;
		uBuf[poz + 125] = 0xff;
		uBuf[poz + 127] = 0xff;
	}

	// ����
	uBuf[0] = uBuf[127] = 0xff;
	uBuf[1] = uBuf[126] = 0x18;
	uBuf[2] = uBuf[125] = 0xDB;

	// �������� ���������� �� ��� ����, ���� ����

	if (num < 7) {
		// �������������� �������
		poz = (num + 1) * NUM_POINT_HOR + 3;
		for (uint8_t i = 3; i < 125; i++) {
			uBuf[poz++] = 0x5A;
		}

		// ����
		poz = (num + 1) * NUM_POINT_HOR;
		uBuf[poz + 1] = uBuf[poz + 127 - 1] = 0x18;
		uBuf[poz + 2] = uBuf[poz + 127 - 2] = 0xDB;
	}

	return true;
}

/** �������������� ������� � ������ ��� ������ �� �����
 * 	@param poz ��������� ������� � ������
 * 	@param val ������
 */
static void vLCDdrawSymb(uint16_t poz, uint8_t val) {
	// ��������� ������������ ����� �������� � ��������� �������� ������
	if (val < 0x20)
		val = 0;
	else if (val < 0x80)
		val -= 0x20;
	else if (val >= 0xB0)
		val -= 0x50;
	else
		val = 0;

	// �������� �������� �� ������� � �����
	for (uint_fast8_t i = 0; i < 5; i++) {
		uBuf[poz++] = pgm_read_byte(&sym[val] [i]);
	}
	uBuf[poz] = 0;
}
