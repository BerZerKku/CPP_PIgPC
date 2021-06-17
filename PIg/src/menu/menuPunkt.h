/*
 * menuPunkt.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef MENUPUNKT_H_
#define MENUPUNKT_H_

#include "glbDefine.h"

/// Структура пункта меню
class TMenuPunkt {

	/// максимальное кол-во пунктов в меню
	static const uint8_t  MAX_NUM_PUNKTS = 20;

public:
	/**	Конструктор.
	 */
	TMenuPunkt() {
		clear();

		for (uint_fast8_t i = 0; i < MAX_NUM_PUNKTS; i++)
			com_[i] = GB_COM_NO;
	}

	/**	Добавление пункта меню, через указатель.
	 *
	 * 	@param name Имя пункта.
	 * 	@param com Команда для запроса из БСП. По умолчанию - нет.
	 * 	@retval True В случае успешного добавления.
	 * 	@retval False В случае ошибки добавления. Например, переполнен массив.
	 */
	bool add(PGM_P name, eGB_COM com=GB_COM_NO) {
		bool stat = false;
		if (cnt_ < MAX_NUM_PUNKTS) {
			name_[cnt_] = name;
			com_[cnt_] = com;
			cnt_++;
			stat = true;
		}
		return stat;
	}

	/**	Добавление пункта меню, через номер элемента массива.
	 *
	 * 	@param name	Номер пункта (имя).
	 * 	@param com Команда для запроса из БСП. По умолчанию - нет.
	 * 	@retval True В случае успешного добавления.
	 * 	@retval False В случае ошибки добавления. Например, переполнен массив.
	 */
	bool add(uint8_t name, eGB_COM com=GB_COM_NO) {
		bool stat = false;
		if (cnt_ < MAX_NUM_PUNKTS) {
			number_[cnt_] = name;
			com_[cnt_] = com;
			cnt_++;
			stat = true;
		}
		return stat;
	}

	/** Очистка текущего списка пунктов меню.
	 */
	void clear() {
		cnt_ = 0;
	}

	/** Замена указанного номера пункта меню.
	 *
	 * 	@param name Имя пунка.
	 * 	@param com Команда для запроса из БСП.
	 * 	@param num Номер пункта.
	 * 	@retval True В случае успешного добавления.
	 * 	@retval False В случае ошибки добавления. Например, не верный номер.
	 */
	bool change(PGM_P name, eGB_COM com, uint8_t num) {
		bool stat = false;
		if (num < cnt_) {
			name_[num] = name;
			com_[num] = com;
			stat = true;
		}
		return stat;
	}

	/** Возвращает название укзанного пункта меню.
	 *
	 *	@param num	Номер пункта.
	 * 	@return Название пункта.
	 */
	PGM_P getName(uint8_t num) {
//		if (num >= cnt_)
//			num = 0;
//		return name_[num];
		return ((num < cnt_) ? name_[num] : 0);
	}

	/**	Возвращает номер эелемента массива с именем указанного пункта меню.
	 *
	 *	@param num	Номер пункта.
	 * 	@return Название пункта.
	 */
	uint8_t getNumber(uint8_t num) {
		return ((num < cnt_) ? number_[num] : 0);
	}

	/** Возвращает команду указанного пункта меню.
	 *
	 * 	@param num Номер пункта.
	 * 	@return Команда для запроса из БСП.
	 */
	eGB_COM getCom(uint8_t num) {
		if (num >= cnt_)
			num = 0;
		return com_[num];
	}

	/** Возвращает текущее кол-во пункто меню.
	 *
	 * 	@return Текущее кол-во пунктов меню.
	 * 	@retval 0 - Пусто.
	 */
	uint8_t getMaxNumPunkts() {
		return cnt_;
	}

private:
	/// текущее кол-во пунктов
	uint8_t cnt_;

	/// указатель на имя пункта
	PGM_P name_[MAX_NUM_PUNKTS];

	/// номер пункта, используется с массивами
	uint8_t number_[MAX_NUM_PUNKTS];

	/// команда для запроса из БСП, необходимая для данного пункта меню
	eGB_COM com_[MAX_NUM_PUNKTS];
};


#endif /* MENUPUNKT_H_ */
