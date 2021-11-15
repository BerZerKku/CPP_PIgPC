#include <math.h>
#include <util/delay.h>
#include <stdio.h>
#include "ks0108.h"
#include "symbols.h"

bool vLCDcheckBusy	(void);
void vLCDsetXY		(uint8_t x, uint8_t y);
void vLCDcom		(uint8_t com, uint8_t cs);
void vLCDdata		(uint8_t data, uint8_t cs);

/// буфер инф-ии выводимой на ЖКИ
static uint8_t uBuf[1024];
/// текущее положение в буфере ввода
static uint16_t uCnt = 0;
/// время до выключения подсветки * 100 мкс
static uint16_t uLedTimeOn = LCD_TIME_LED_ON;
/// состояние подсветки
static eLCD_LED eLed = LED_OFF;

/// обновление информации
static volatile bool bRefresh = false;

static void vLCDdrawSymb(uint16_t poz, uint8_t val);

/**	Проверка флага занятости ЖКИ
 * 	@param Нет
 * 	@return false - занято.
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

/** Запись команды в ЖКИ
 * 	@param com Команда
 * 	@param cs Выбор кристалла
 * 	@return Нет
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

/** Очистка дисплея
 * 	@param Нет
 * 	@return Нет
 */
void vLCDclear(void) {
	for (uint_fast16_t i = 0; i < 1024; i++) {
		uBuf[i] = 0;
	}
}

/** Запись данных в ЖКИ
 * 	@param data Байт данных, передаваемый в ЖКИ
 * 	@param cs Выбор кристалла
 * 	@return Нет
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

/** Запись данных в ЖКИ
 * 	@param data Байт данных, помещаемый в буфер
 * 	@return Нет
 */
void vLCDwriteData(uint8_t data) {
	uBuf[uCnt++] = data;
}

/** Инициализация дисплея
 * 	@param Нет
 * 	@return Нет
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

/**	Установка текущей координаты ЖКИ
 * 	@param x Номер столбца
 * 	@param y Номер строки
 * 	@return Нет
 */
void vLCDsetXY(uint8_t x, uint8_t y) {
	uCnt = (y * NUM_POINT_HOR) + x - 1;
}

/** Старт обновления содержимого ЖКИ
 * 	@param Нет
 * 	@return Нет
 */
void vLCDrefresh(void) {
	bRefresh = true;
}

/** Обновление ЖКИ
 * 	@param Нет
 * 	@return Нет
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

/** Преобразование массива символов, в данные для вывода на жки
 * 	@param buf Указатель на буфер данных ЖКИ
 *	@param num Кол-во линий для параметров
 *	@return true - в случае удачного вывода
 */
bool vLCDputchar(const char* buf, uint8_t num) {
	uint_fast16_t poz = 0;

	if (num > 7)
		return false;

	// вывод параметров
	// начало : вторая строка дисплея + смещение от края на 4 точек
	poz = NUM_POINT_HOR + 4;
	for (uint_fast8_t i = 0; i < 20 * num; i++, poz += 6) {
		vLCDdrawSymb(poz, *buf++);

		// если достигли конца строки, сделаем сдвижку на 8 пунктов
		if ((i % 20) == 19)
			poz += 8;
	}

	// вывод основного меню
	// начало : пятая строка дисплея + смещение от края на 4 точеки
	poz = (num + 2) * NUM_POINT_HOR + 4;
	for (uint_fast8_t i = 20 * num; i < SIZE_BUF_STRING; i++, poz += 6) {
		vLCDdrawSymb(poz, *buf++);

		// если достигли конца строки. сделаем сдвижку на 10 пунктов
		if ((i % 20) == 19)
			poz += 8;
	}

	return true;
}

/**	Установка режима работы подсветкой
 * 	@param val Одно из значений:
 * 	@arg LED_ON Включить постоянную подсветку
 * 	@arg LED_SWITCH Включить временную подсветку
 * 	@arg LED_OFF Выключить подсветку
 * 	@return Нет
 */
void vLCDsetLed(eLCD_LED val) {
	if (val == LED_SWITCH)
		uLedTimeOn = LCD_TIME_LED_ON;
	else
		eLed = val;
}

/** Управление подсветкой (формирование ШИМ).
 * 	Частота задается частотой вызова данной функции
 * 	@param Нет
 * 	@return Нет
 */
void vLCDled(void) {
	// счетчик 200 циклов , т.е. период 10мс
	static uint8_t cnt = 200 - 1;

	// Работа с подсветкой
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

/** Рисование рамки
 * 	@param num Кол-во линий параметров 1..3
 * 	@return true - в случае успешной отрисовки
 */
bool vLCDdrawBoard(uint8_t num) {
	// задано ошибочное кол-во строк для отображения параметров
	if (num > 6)
		return false;

	uint16_t poz = 0;

	// нарисуем основную рамку

	// горизонтальные границы
	for (uint8_t i = 3; i < 125; i++) {
		uBuf[i] = 0x5A;
	}

	// вертикальные границы
	poz = 128;
	for (uint8_t i = 0; i < 7; i++, poz += NUM_POINT_HOR) {
		uBuf[poz] = 0xff;
		uBuf[poz + 2] = 0xff;
		uBuf[poz + 125] = 0xff;
		uBuf[poz + 127] = 0xff;
	}

	// углы
	uBuf[0] = uBuf[127] = 0xff;
	uBuf[1] = uBuf[126] = 0x18;
	uBuf[2] = uBuf[125] = 0xDB;

	// нарисуем разделение на два поля, если надо

	if (num < 7) {
		// горизонтальная граница
		poz = (num + 1) * NUM_POINT_HOR + 3;
		for (uint8_t i = 3; i < 125; i++) {
			uBuf[poz++] = 0x5A;
		}

		// углы
		poz = (num + 1) * NUM_POINT_HOR;
		uBuf[poz + 1] = uBuf[poz + 127 - 1] = 0x18;
		uBuf[poz + 2] = uBuf[poz + 127 - 2] = 0xDB;
	}

	return true;
}

/** Преобразивание символа в данные для вывода на экран
 * 	@param poz Начальная позиция в буфере
 * 	@param val символ
 */
static void vLCDdrawSymb(uint16_t poz, uint8_t val) {
	// установим соответствие между символом и имеющейся таблицей данных
	if (val < 0x20)
		val = 0;
	else if (val < 0x80)
		val -= 0x20;
	else if (val >= 0xB0)
		val -= 0x50;
	else
		val = 0;

	// копируем значения из таблицы в буфер
	for (uint_fast8_t i = 0; i < 5; i++) {
		uBuf[poz++] = pgm_read_byte(&sym[val] [i]);
	}
	uBuf[poz] = 0;
}
