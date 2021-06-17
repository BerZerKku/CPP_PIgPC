/*
 * deviceDef.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMDEF_H_
#define PARAMDEF_H_

// #include "glbDefine.h"

/// Минимальные, максимальные значения параметров приемника и их дискретность
/// ЗАЩИТА

/// Тип автоконтроля
enum eGB_TYPE_AC {
	GB_TYPE_AC_MIN = 1, 		// минимальное значение
	GB_TYPE_AC_AUTO_NORM = 1,	// АК нормальный / АК включен
	GB_TYPE_AC_AUTO_REPEAT = 2,	// АК повторный
	GB_TYPE_AC_FAST = 3,		// АК ускоренный
	GB_TYPE_AC_OFF = 4,			// АК выключен
	GB_TYPE_AC_CHECK = 5,		// АК беглый
	GB_TYPE_AC_PUSK_SELF = 6,	// Пуск АК свой / АК испытания / АК контр.пров.
	GB_TYPE_AC_PUSK = 7,		// АК пуск
	GB_TYPE_AC_ONE_SIDE = 8,	// АК односторонний
	GB_TYPE_AC_MAX,				// максимальное принимаемое значение
	GB_TYPE_AC_AUTO,			// подмена  GB_TYPE_AC_AUTO_NORM в Р400м->ЛинияР
	GB_TYPE_AC_CHECK_1			// подмена GB_TYPE_AC_CHECK в Р400м->АВЗК-80 и ПВЗ-90
};

/// класс для параметров и настроек защиты
class TDeviceDef {
public:
	TDeviceDef() {
		numDevices_ = GB_NUM_DEVICES_MAX;
		typeAc_ = GB_TYPE_AC_AUTO_NORM;
		timeToAc_ = 0;
		numJrnEntry_ = 0;
		maxNumJrnEntry_ = 0;
		overflow_ = false;
	}

	TDeviceStatus status;

	// тип линии (кол-во аппаратов в линии)
	bool setNumDevices(eGB_NUM_DEVICES val) {
		bool stat = false;
		if (val >= GB_NUM_DEVICES_MIN) {
			if (val < GB_NUM_DEVICES_MAX) {
				numDevices_ = val;
				stat = true;
			}
		}

		if (!stat)
			val = GB_NUM_DEVICES_MAX;
		numDevices_ = val;
		return stat;
	}
	eGB_NUM_DEVICES getNumDevices() const {
		return numDevices_;
	}

	// тип автоконтроля
	bool setTypeAC(eGB_TYPE_AC val) {
		bool stat = false;
		if ((val >= GB_TYPE_AC_MIN) && (val <= GB_TYPE_AC_MAX)) {
			typeAc_ = val;
			stat = true;
		}

		return stat;
	}
	eGB_TYPE_AC getTypeAC() const {
		return typeAc_;
	}

	// время до автоконтроля
	bool setTimeToAC(uint32_t val) {
		bool stat = false;

		val /= 1000;

		if (val <= 60000) {
			timeToAc_ = val;
			stat = true;
		}

		return stat;
	}
	uint64_t getTimeToAC() const {
		return timeToAc_;
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
			stat = true;
			maxNumJrnEntry_ = max;
		}
		return stat;
	}
	uint16_t getMaxNumJrnEntry() const {
		return maxNumJrnEntry_;
	}

private:
		// тип линии (кол-во аппаратов)
	eGB_NUM_DEVICES numDevices_;

	// тип автоконтроля
	eGB_TYPE_AC typeAc_;

	// время до автоконтроля в секундах
	uint16_t timeToAc_;

	// кол-во записей в журнале
	uint16_t numJrnEntry_;

	// максимальное кол-во записей в журнале
	uint16_t maxNumJrnEntry_;

	// флаг переполнения журнала
	bool overflow_;
};


#endif /* PARAMDEF_H_ */
