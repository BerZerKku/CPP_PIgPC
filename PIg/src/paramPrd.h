/*
 * devicePrd.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMPRD_H_
#define PARAMPRD_H_

#include "glbDefine.h"

/// класс для параметров и настроек передатчика
class TDevicePrd {
public:
	TDevicePrd() {
		numCom_ = 0;
		for (uint_fast8_t i = 0; i < (MAX_NUM_COM_PRD / 8); i++) {
			indCom_[i] = 0;
		}

		numJrnEntry_ = 0;
		maxNumJrnEntry_ = 0;
		overflow_ = false;
	}

	TDeviceStatus status;

	/**	Установка текущего кол-ва команд в ПРД.
	 *
	 * 	Если новое значение выходит за диапазон допустимых значений, устанавли
	 * 	вается 0. Далее если команд нет, ПРД отключается. Иначе включается.
	 *
	 * 	@param val Количество команд на ПРД.
	 * 	@return Статус установки (eGB_ACT - побитные значения).
	 */
	uint8_t setNumCom(uint8_t val) {
		uint8_t act = GB_ACT_NO;

		if (val > MAX_NUM_COM_PRD) {
			val = 0;
			act = GB_ACT_ERROR;
		}

		if (numCom_ == val) {
			act |= GB_ACT_OLD;
		} else {
			numCom_ = val;
			// если кол-во команд не равно 0, то включается ПРМ
			this->status.setEnable(val != 0);
			act |= GB_ACT_NEW;
		}


		return act;
	}

	/**	Возвращает текущее кол-во команд в ПРД.
	 *
	 * 	@return Кол-во команд ПРД.
	 */
	uint8_t getNumCom() const {
		return numCom_;
	}

		/** Установка текущего стостояния светодиодов на блоке БСК
	 *
	 *	@param num Номер восьмерки команд (0 - 8..1 команды, 1 - 16..9 и т.д.).
	 *	@param val Состояние светодиодов.
	 *	@return True - в случае успешной записи, False - иначе.
	 */
	bool setIndCom8(uint8_t num, uint8_t val) {
		bool stat = false;
		if (num < (MAX_NUM_COM_PRD / 8)) {
			indCom_[num] = val;
			stat = true;
		}
		return stat;
	}

	/**	Возвращает текущее состояние светодиода комадны на блоке БСК.
	 *
	 * 	@param num Номер команды.
	 * 	@return True- светодиод горит, False - иначе.
	 */
	bool getIndCom(uint8_t num) const {
		// номер восьмерки
		uint8_t pl = num / 8;
		// номер внутри восьмерки
		num = num % 8;
		return (indCom_[pl] & (1 << num)) != 0;
	}

	/**	Устанавливает текущее состояние светодиодов команд на блоке БСК.
	 *
	 * 	Каждый бит является флагом для соответствующей команды, младший бит -
	 * 	младшая команда. Установленный бит означает что светодиод горит.
	 *
	 * 	@param num Номер восьмерки команд (0 - 8..1 команды, 1 - 16..9 и т.д.).
	 * 	@return Текущее состояние флагов блокировки команд.
	 */
	uint8_t getIndCom8(uint8_t num) const {
		uint8_t val = 0;
		if (num < (MAX_NUM_COM_PRD / 8))
			val = indCom_[num];
		return val;
	}

	/**	Провекра наличия горящих светодиодов индикации команд на блоке БСК,
	 *
	 * 	@retval True - если горит светодиорд хотя бы одной команда.
	 * 	@retval False - горящих светодиодов команд нет.
	 */
	bool isIndCom() const {
		uint8_t ind = 0;

		for(uint8_t i = 0; i < (MAX_NUM_COM_PRD / 8); i++) {
			ind |= indCom_[i];
		}

		return (ind != 0);
	}

	// количество записей в журнале
	// количество записей в журнале
	bool setNumJrnEntry(uint16_t val) {
		bool stat = false;

		overflow_ = (val & 0xC000) != 0;
		val &= 0x3FFF;

		if (val <= maxNumJrnEntry_) {
			numJrnEntry_ = val;
			stat = true;
		}
		return stat;
	}
	uint16_t getNumJrnEntry() const {
		return numJrnEntry_;
	}

	// максимальное кол-во записей в журнале
	bool setMaxNumJrnEntry(uint16_t max) {
		bool stat = false;
		if (max <= 1024) {
			maxNumJrnEntry_ = max;
			stat = true;
		}
		return stat;
	}
	uint16_t getMaxNumJrnEntry() const {
		return maxNumJrnEntry_;
	}

private:
	// текущее кол-во команд
	uint8_t numCom_;

	// состояние индикации светодиодов на блоке БСК
	uint8_t indCom_[MAX_NUM_COM_PRD / 8];

	// кол-во записей в журнале
	uint16_t numJrnEntry_;

	// максималбное кол-во записей в журнале
	uint16_t maxNumJrnEntry_;

	// флаг переполнения журнала
	bool overflow_;
};

#endif /* PARAMPRD_H_ */
