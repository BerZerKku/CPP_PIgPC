/*
 * keyboard.cpp
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */

#include "keyboard.h"

/// Время антидребезга * 10мс
#define TIME_DELAY 10
/// Время до повторного определения
#define TIME_DELAY_REPEAT 100

// коды кнопок
// 1R1C - Номер строки (1..3) и стоблца (1..3)
// F - дополнительная функция
enum eBUT
{
	// коды кнопок, одиночное нажатие
	BUT_NO = 0x00,
	BUT_1R1C,
	BUT_1R2C,
	BUT_1R3C,
	BUT_2R1C,
	BUT_2R2C,
	BUT_2R3C,
	BUT_3R1C,
	BUT_3R2C,
	BUT_3R3C,

	// коды кнопок, при нажатой кнопке ФУНКЦИЯ
	BUT_F = 0x80,
	BUT_F_1R1C,
	BUT_F_1R2C,
	BUT_F_1R3C,
	BUT_F_2R1C,
	BUT_F_2R2C,
	BUT_F_2R3C,
	BUT_F_3R1C,
	BUT_F_3R2C,
	BUT_F_3R3C
};

/// код нажатой кнопки
static eBUT keyPressed;


/**	Возвращает значение нажатой кнопки
 * 	@param Нет
 * 	@return eKEY Код нажатой кнопки
 */
uint8_t eKEYget(void) {
	uint_fast8_t but = keyPressed;

	if ((but >= BUT_1R1C) && (but <= BUT_3R3C)) {
		but = 1 + (but - BUT_1R1C);
	} else if ((but >= BUT_F_1R1C) && (but <= BUT_F_3R3C)) {
		but = 10 + (but - BUT_F_1R1C);
	} else {
		but = 0;
	}

	keyPressed = BUT_NO;
	return but;
}

/**	Функция определения нажатой кнопки
 *	Вызывать надо раз в 10мс
 *	Нажатие одной кнопки "Функция" игнорируется
 *	Повторное срабатывание кнопки TIME_DELAY_REPEAT * (период вызова ф-ии)
 *	Время антидребезга TIME_DELAY * (период вызова ф-ии)
 * 	@param Нет
 * 	@return Нет
 */
void vKEYmain(void) {
	static uint_fast8_t delay = TIME_DELAY;	// счетчик антидребезга
	static uint_fast8_t keyPrev = BUT_NO;	// предыдущее значение кнопки
	uint_fast8_t tmp;						// временная переменная
	uint_fast8_t tmpKey = BUT_NO;			// временная переменная

	tmp = (~PIND ) & 0xC0;
	// сначала проверим нажатие кнопки "Функция"
	if (tmp & 0x80)
		tmpKey = BUT_F;

	// сканируем клавиатуру, до определения первой нажатой кнопки
	if (tmp & 0x40)
		tmpKey |= BUT_3R1C;
	else {
		tmp = (~PING ) & 0x03;
		if (tmp) {
			if (tmp & 0x01)
				tmpKey |= BUT_2R1C;
			else if (tmp & 0x02)
				tmpKey |= BUT_1R2C;
		} else {
			tmp = ~(PINC ) & 0x1F;
			if (tmp) {
				if (tmp & 0x01)
					tmpKey |= BUT_1R3C;
				else if (tmp & 0x02)
					tmpKey |= BUT_2R2C;
				else if (tmp & 0x04)
					tmpKey |= BUT_2R3C;
				else if (tmp & 0x08)
					tmpKey |= BUT_3R2C;
				else if (tmp & 0x10)
					tmpKey |= BUT_3R3C;
			}
		}
	}

	// Проверим на совпадение с предыдущим значением
	// Если не совпало, запустим счетчик антидребезга
	// Если кнопки не нажаты, антидребезг не считается
	// Если кнопка удерживается длительное время,
	// срабатывание определяется с периодом TIME_DELAY_REPEAT
	// NOTE Периодический повтор нажатия убран!
	if (tmpKey != keyPrev) {
		keyPrev = tmpKey;
		tmpKey = BUT_NO;
		delay = TIME_DELAY;
	} else {
		if ((tmpKey != BUT_NO) && (delay > 0)) {
			delay--;

			if (delay > 0) {
				tmpKey = BUT_NO;
			}
		} else {
			tmpKey = BUT_NO;
		}
	}

	// Если предыдущее нажатие уже обработано, вернем новое нажатие
	// Нажатие одной кнопки функция - игнорируется
	if (keyPressed == BUT_NO) {
		keyPressed = (eBUT) tmpKey;
	}
}
