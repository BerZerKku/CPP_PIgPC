/*
 * deviceGlb.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMGLB_H_
#define PARAMGLB_H_

/// ОБЩИЕ
/// номер аппарата
#define GLB_DEV_NUM_MIN		1
#define GLB_DEV_NUM_MAX 	3
#define GLB_DEV_NUM_DISC 	1
#define GLB_DEV_NUM_FRACT	1
#define GLB_DEV_NUM_MIN_F	(GLB_DEV_NUM_MIN / GLB_DEV_NUM_FRACT)
#define GLB_DEV_NUM_MAX_F 	(GLB_DEV_NUM_MAX / GLB_DEV_NUM_FRACT)
#define GLB_DEV_NUM_DISC_F	(GLB_DEV_NUM_DISC / GLB_DEV_NUM_FRACT)
/// сетевой адрес
#define GLB_NET_ADR_MIN		1
#define GLB_NET_ADR_MAX		247
#define GLB_NET_ADR_DISC 	1
#define GLB_NET_ADR_FRACT	1
#define GLB_NET_ADR_MIN_F	(GLB_NET_ADR_MIN / GLB_NET_ADR_FRACT)
#define GLB_NET_ADR_MAX_F	(GLB_NET_ADR_MAX / GLB_NET_ADR_FRACT)
#define GLB_NET_ADR_DISC_F	(GLB_NET_ADR_DISC / GLB_NET_ADR_FRACT)

/// Микросхемы имеющие прошивку
enum eGB_IC {
	GB_IC_MIN = 0,
	GB_IC_BSP_MCU = 0,
	GB_IC_BSP_DSP,
	GB_IC_BSP_DSP_PLIS,
	GB_IC_PI_MCU,
	GB_IC_BSK_PLIS_PRD1,
	GB_IC_BSK_PLIS_PRD2,
	GB_IC_BSK_PLIS_PRM1,
	GB_IC_BSK_PLIS_PRM2,
	GB_IC_BSZ_PLIS,
	GB_IC_MAX
};

/// Совместимость
enum eGB_COMP_R400M {
	GB_COMP_R400M_MIN    = 0,
	GB_COMP_R400M_AVANT  = 0,
	GB_COMP_R400M_PVZ90  = 1,
	GB_COMP_R400M_AVZK80 = 2,
	GB_COMP_R400M_PVZUE  = 3,
	GB_COMP_R400M_PVZL   = 4,
	GB_COMP_R400M_LINER  = 5,
	GB_COMP_R400M_PVZK   = 6, // TODO задел на будущее. Ничего по нему не сделано.
	GB_COMP_R400M_MAX
};

/// Совместимость для К400
enum eGB_COMP_K400 {
	GB_COMP_K400_MIN 		= 0,
	GB_COMP_K400_AVANT 		= 0,
	GB_COMP_K400_AVANT_PRD 	= 1,
	GB_COMP_K400_AVANT_PRM	= 2,
	GB_COMP_K400_AKPA_PRD 	= 3,
	GB_COMP_K400_AKPA_PRM 	= 4,
	GB_COMP_K400_KEDR_PRD	= 5,
	GB_COMP_K400_KEDR_PRM 	= 6,
	GB_COMP_K400_UPKC_PRD	= 7,
	GB_COMP_K400_UPKC_PRM	= 8,
	GB_COMP_K400_VCTO_PRD	= 9,
	GB_COMP_K400_VCTO_PRM	= 10,
	GB_COMP_K400_ANKA_PRD	= 11,	///< АНКА-АВПА
	GB_COMP_K400_ANKA_PRM	= 12,	///< АНКА-АВПА
	GB_COMP_K400_MAX
};

/// Совместимость для РЗСК
enum eGB_COMP_RZSK {
    GB_COMP_RZSK_MIN    = 0,
    GB_COMP_RZSK        = GB_COMP_RZSK_MIN,
    GB_COMP_RZSK_M,
	GB_COMP_RZSK_3E8,
    //
    GB_COMP_RZSK_MAX
};

/// класс для общих параметров и настроек
class TDeviceGlb {
	/// Дискретные входы
	enum __attribute__((__packed__)) dInput_t {
		DINPUT_PUSK_PRM 	= 0x01,	///< Пуск ПРМ
		DINPUT_RESET_IND 	= 0x02,	///< Сброс инд
		DINPUT_TM_CONTROL 	= 0x04	///< Упр ТМ
	};

public:
	TDeviceGlb() {
		typeDevice_ = AVANT_NO;
		typeOpto_ = TYPE_OPTO_MAX;
		numDevices_ = GB_NUM_DEVICES_MAX;
		typeLine_ = GB_TYPE_LINE_MAX;
		compR400m_ = GB_COMP_R400M_MAX;
		compK400_ = GB_COMP_K400_MAX;
		compRZSK_ = GB_COMP_RZSK_MAX;

		deviceNum_ = GLB_DEV_NUM_MIN_F;
		netAdr_ = 0;
		ledOn = false;

		for(uint_fast8_t i = 0; i < GB_IC_MAX; i++) {
			versProgIC_[i] = 0;
		}

		dInputState = 0x00;
	}

	TDeviceStatus status;

	/**	Возвращает тип аппарата.
	 *
	 * 	@return Тип аппарата.
	 */
	eGB_TYPE_DEVICE getTypeDevice() const {
		return typeDevice_;
	}

	/**	Установка типа аппарата.
	 *
	 * 	В случае ошибки, значение не поменяется.
	 *
	 * 	@param Тип аппарата.
	 * 	@return Статус установки.
	 */
	uint8_t setTypeDevice(eGB_TYPE_DEVICE val) {
		uint8_t act = GB_ACT_NO;

		if ((val < AVANT_NO) || (val > AVANT_MAX)) {
			act |= GB_ACT_ERROR;
			val = typeDevice_;
		}

		// NOTE Р400м не выпускается оптическим
		if (typeLine_ == GB_TYPE_LINE_OPTO) {
			val = AVANT_OPTO;
		}

		act |= (typeDevice_ == val) ? GB_ACT_OLD : GB_ACT_NEW;
		typeDevice_ = val;

		return act;
	}

	/**	Возвращает тип оптического аппарата.
	 *
	 * 	@return Тип аппарата.
	 */
	eGB_TYPE_OPTO getTypeOpto() const {
		return typeOpto_;
	}

	/**	Установка типа аппарата.
		 *
		 * 	В случае ошибки, значение не поменяется.
		 *
		 * 	@param Тип аппарата.
		 * 	@return Статус установки.
		 */
		uint8_t setTypeOpto(eGB_TYPE_OPTO val) {
			uint8_t act = GB_ACT_NO;

			if (val > TYPE_OPTO_MAX) {
				act |= GB_ACT_ERROR;
			} else {
				if (typeLine_ == GB_TYPE_LINE_OPTO) {
					if (typeOpto_ == val) {
						act = GB_ACT_OLD;
					} else {
						typeOpto_ = val;
						act = GB_ACT_NEW;
					}
				}
			}

			return act;
		}



	/**	Возвращает максимальное кол-во аппаратов в линии.
	 *
	 * 	@retval 3 В случае 3-х концевой версии
	 * 	@retval 2 Во всех остальных случаях.
	 */
	uint8_t getMaxNumDevices() const {
		return (numDevices_ == GB_NUM_DEVICES_3) ? 3 : 2;
	}

	/**	Возвращает кол-во аппаратов в линии (2-х, 3-х концевая).
	 *
	 * 	@return Кол-во аппаратов в линии.
	 */
	eGB_NUM_DEVICES getNumDevices() const {
		return numDevices_;
	}

	/** Установка кол-ва аппаратов в линии (2-х, 3-х концевая.).
	 *
	 * 	В случае ошибки, значение не поменяется.
	 * 	@param val Кол-во аппаратов в линии.
	 * 	@return Статус установки (eGB_ACT - побитные значения).
	 */
	uint8_t setNumDevices(eGB_NUM_DEVICES val) {
		uint8_t act = GB_ACT_NO;

		if ((val < GB_NUM_DEVICES_MIN) || (val >= GB_NUM_DEVICES_MAX)) {
			act = GB_ACT_ERROR;
		} else {
			if (numDevices_ == val) {
				act = GB_ACT_OLD;
			} else {
				numDevices_ = val;
				act = GB_ACT_NEW;
			}
		}

		return act;
	}

	/**	Возвращает текущий тип линии (вч/оптика и т.д.).
	 *
	 * 	@return Тип линии.
	 */
	eGB_TYPE_LINE getTypeLine() const {
		return typeLine_;
	}

	/** Установка типа линии (вч/оптика и т.д.).
	 *
	 * 	В случае ошибки, значение не поменяется.
	 *
	 * 	@param val Тип линии.
	 * 	@return Статус установки (eGB_ACT - побитные значения).
	 */
	uint8_t setTypeLine(eGB_TYPE_LINE val) {
		uint8_t act = GB_ACT_NO;

		if ((val < GB_TYPE_LINE_MIN) || (val >= GB_TYPE_LINE_MAX)) {
			act = GB_ACT_ERROR;
		} else {
			if (typeLine_ == val) {
				act = GB_ACT_OLD;
			} else {
				typeLine_ = val;
				act = GB_ACT_NEW;
			}
		}

		return act;
	}

	/**	Запись версии прошивки для микросхем АВАНТа.
	 * 	Данные хранятся в переменной int16_t.
	 * 	Старший байт - версия прошивки. Младший - ревизия.
	 * 	Например: 2.3, где 2 - версия, 3 - ревизия.
	 * 	@param vers Версия прошивки.
	 * 	@param ic	Микросхема.
	 */
	void setVersProgIC16(uint16_t vers, eGB_IC ic) {
		if (ic < GB_IC_MAX) {
			versProgIC_[ic] = vers;
		}
	}

	/**	Запись версии прошивки для микросхема АВАНТа.
	  * Данные хранятся в переменной int16_t.
	 * 	Старший байт - версия прошивки. Младший - ревизия.
	 * 	Например: 2.3, где 2 - версия, 3 - ревизия.
	 * 	@param vers Версия прошивки.
	 * 	@param ic	Микросхема.
	 */
	void setVersProgIC8(uint8_t vers, eGB_IC ic) {
		if (ic < GB_IC_MAX) {
			uint8_t hi = vers >> 4;
			uint8_t low = vers & 0x0F;
			versProgIC_[ic] = TO_INT16(hi, low);
		}
	}

	/**	Возвращает версию прошивки для указанной микросхемы АВАНТа.
	 * 	Старший байт int16_t - версия прошивки, младший - ревизия.
	 * 	Например: 2.3, где 2 - версия, 3 - ревизия.
	 * 	@return Версия прошивки микросхемы.
	 * 	@retval 0 В случае ошибочного номера микросхемы.
	 */
	uint16_t getVersProgIC(eGB_IC ic) {
		return ((ic < GB_IC_MAX) ? versProgIC_[ic] : 0);
	}


	/** Установка совместимости.
	 *  @param[in] val Совместимость.
	 *  @return Статус установки (eGB_ACT - побитные значения).
	 *  @retval GB_ACT_OLD
	 *  @retval GB_ACT_NEW
	 */
	uint8_t setCompatibility(uint8_t compatibility) {
		uint8_t act = GB_ACT_NO;

		switch(typeDevice_) {
			case AVANT_R400: // DOWN
			case AVANT_R400M: {
				act = setCompR400m(eGB_COMP_R400M(compatibility));
			} break;
			case AVANT_RZSK: {
				act = setCompRZSK(eGB_COMP_RZSK(compatibility));
			} break;
			case AVANT_K400: {
				act = setCompK400(eGB_COMP_K400(compatibility));
			} break;
			case AVANT_OPTO: {

			} break;
			case AVANT_NO: // DOWN
			case AVANT_MAX: {
				act |= setCompR400m(eGB_COMP_R400M(compatibility));
				act |= setCompRZSK(eGB_COMP_RZSK(compatibility));
				act |= setCompK400(eGB_COMP_K400(compatibility));
			} break;
		}

		return act;
	}

	/**	Возвращает текущую совместимость.
	 * 	@return Совместимость.
	 */
	eGB_COMP_R400M getCompR400m() const {
		return compR400m_;
	}

	/**	Возвращает текущую совместимость в К400ю
	 *
	 * 	@return Совместимость в К400.
	 */
	eGB_COMP_K400 getCompK400() const {
		return compK400_;
	}

	/** Возвращает текущую совместимость в К400ю
	 *
	 *  @return Совместимость в К400.
	 */
	eGB_COMP_RZSK getCompRZSK() const {
	    return compRZSK_;
	}

	// номер аппарата
	bool setDeviceNum(uint8_t val) {
		bool stat = false;
		val = (val / GLB_DEV_NUM_DISC_F) * GLB_DEV_NUM_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_DEV_NUM_MIN_F) {
			if (val <= getMaxNumDevices()) {
				deviceNum_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getDeviceNum() const {
		return (deviceNum_ * GLB_DEV_NUM_FRACT);
	}

	// сетевой адрес
	bool setNetAddress(uint8_t val) {
		bool stat = false;
		val = (val / GLB_NET_ADR_DISC_F) * GLB_NET_ADR_DISC_F;

		// записано в таком виде т.к. иначе портится автоформат в Eclipse
		if (val >= GLB_NET_ADR_MIN_F) {
			if (val <= GLB_NET_ADR_MAX_F) {
				netAdr_ = val;
				stat = true;
			}

		}
		return stat;
	}
	uint8_t getNetAddress() const {
		return (netAdr_ * GLB_NET_ADR_FRACT);
	}

	// проверка флага включения подсветки
	bool isLedOn() const {
		return ledOn;
	}

	// установка значения флага включения подсветки
	void setLedOn(bool ledOn) {
		this->ledOn = ledOn;
	}

	// установка флагов текущих состояний дискретных входов
	void setDInputState(uint8_t val) {
		dInputState = val;
	}

	// Возвращает текущее состояние ДВ "Пуск ПРМ"
	bool getDInputPuskPrm() const {
		return ((dInputState & DINPUT_PUSK_PRM) > 0) ? true : false;
	}
	// Возвращает текущее состояние ДВ "Сброс инд"
	bool getDInputResetInd() const {
		return ((dInputState & DINPUT_RESET_IND) > 0) ? true : false;
	}

	// Возвращает текущее состояние ДВ "Упр ТМ"
	bool getDInputTmControl() const {
		return ((dInputState & DINPUT_TM_CONTROL) > 0) ? true : false;
	}

private:
	// версии прошивок микросхем
	uint16_t versProgIC_[GB_IC_MAX];

	// тип аппарата
	eGB_TYPE_DEVICE typeDevice_;

	// тип оптического аппарата
	eGB_TYPE_OPTO typeOpto_;

	// кол-во аппаратов в линии 2 или 3
	eGB_NUM_DEVICES numDevices_;

	// тип линии
	eGB_TYPE_LINE typeLine_;

	// совместимость
	eGB_COMP_R400M compR400m_;

	// совместимость в К400
	eGB_COMP_K400 compK400_;

	// совместимость в РЗСК
	eGB_COMP_RZSK compRZSK_;

	// номер аппарата
	uint8_t deviceNum_;

	// сетевой адрес
	uint8_t netAdr_;

	// флаг включения подсветки
	bool ledOn;

	// состояние дискретных входов (побитно, выкл(0)/вкл(1)).
	uint8_t dInputState;

	/**	Установка совместимости (тип удаленного аппарата).
	 * 	@param val Совместимость.
	 * 	@return Статус установки (eGB_ACT - побитные значения).
	 */
	uint8_t setCompR400m(eGB_COMP_R400M val) {
		uint8_t act = GB_ACT_NO;

		if ((val < GB_COMP_R400M_MIN) || (val >= GB_COMP_R400M_MAX)) {
			val = GB_COMP_R400M_MAX;
			act = GB_ACT_ERROR;
		}

		act |= (compR400m_ == val) ? GB_ACT_OLD : GB_ACT_NEW;
		compR400m_ = val;

		return act;
	}

	/** Установка совместимости для RZSK (тип удаленного аппарата).
	 *  @param val Совместимость.
	 *  @return Статус установки (eGB_ACT - побитные значения).
	 */
	uint8_t setCompRZSK(eGB_COMP_RZSK val) {
		uint8_t act = GB_ACT_NO;

		if ((val < GB_COMP_RZSK_MIN) || (val >= GB_COMP_RZSK_MAX)) {
			val = GB_COMP_RZSK_MAX;
			act = GB_ACT_ERROR;
		}

		act |= (compRZSK_ == val) ? GB_ACT_OLD : GB_ACT_NEW;
		compRZSK_ = val;

		return act;
	}

	/**	Установка совместимости для К400 (тип удаленного аппарата).
	 * 	@param val Совместимость.
	 * 	@return Статус установки (eGB_ACT - побитные значения).
	 */
	uint8_t setCompK400(eGB_COMP_K400 val) {
		uint8_t act = GB_ACT_NO;

		if ((val < GB_COMP_K400_MIN) || (val >= GB_COMP_K400_MAX)) {
			val = GB_COMP_K400_MAX;
			act = GB_ACT_ERROR;
		}

		act |= (compK400_ == val) ? GB_ACT_OLD : GB_ACT_NEW;
		compK400_ = val;

		return act;
	}
};


#endif /* PARAMGLB_H_ */
