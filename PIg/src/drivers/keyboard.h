/*
 * keyboard.h
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "glbDefine.h"

/// Время антидребезга * 10мс
#define TIME_DELAY 10
/// Время до повторного определения
#define TIME_DELAY_REPEAT 100

/// Функции кнопок
enum eKEY {
	KEY_NO = 0x00,	// нет нажатых кнопок
	KEY_EMPTY,		// была нажата кнопка, но без какой-либо функции
	KEY_FUNC,		// Функция
	KEY_ENTER,		// Меню
	KEY_CANCEL,		// Отмена
	KEY_DOWN,		// Вниз
	KEY_LEFT,		// Влево
	KEY_RIGHT,		// Вправо
	KEY_UP,			// Вверх

	KEY_MENU,		// Вход в Меню
	KEY_RESET,		// Сброс
	KEY_PUSK,		// Пуск	(ввод приемника в работу)
	KEY_PUSK_UD,	// Удал. Пуск ПРД
	KEY_PUSK_NALAD,	// н.Пуск / н.Пуск ПРД (наладочный пуск)
	KEY_RESET_IND,	// Сброс инд.
	KEY_CALL,		// Вызов

	KEY_AC_PUSK,	// Пуск АК
	KEY_AC_PUSK_UD,	// Удал. Пуск АК
	KEY_AC_RESET,	// Сброс АК
	KEY_AC_REGIME	// Режим АК
};

void vKEYmain	(void);
void vKEYset	(eGB_TYPE_DEVICE type);
eKEY eKEYget	(void);
uint8_t timePressKey();

#endif /* KEYBOARD_H_ */
