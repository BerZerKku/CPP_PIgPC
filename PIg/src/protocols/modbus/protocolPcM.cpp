/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include <stdio.h>
#include "protocolPcM.h"
#include "src/glbDefine.h"

// Конструктор
TProtocolPcM::TProtocolPcM(stGBparam *sParam, uint8_t *buf, uint8_t size) :
sParam_(sParam), TProtocolModbus(buf, size) {
	// NONE
}

//	Функция отправки сообщения.
uint8_t TProtocolPcM::send() {

	return sendData();
}

/**	Чтение флагов.
 *
 *	Проводится проверка корректности адреса. Если он находится в допустимом
 *	диапазоне, возвращается текущее значение регистра.
 *
 *	C неиспользуемых флагов будет считано false.
 *
 *	@param adr Адрес флага.
 *	@param val [out] Состояние флага.
 *	@retval CHECK_ERR_NO Ошибок при считывании флага не возникло.
 *	@retval CHECK_ERR_ADR Недопустимый адрес флага.
 */
TProtocolModbus::CHECK_ERR TProtocolPcM::readCoil(uint16_t adr, bool &val) {
	//  проверка адресов
	if ((adr <= ADR_C_MIN) || (adr >= ADR_C_MAX))
		return CHECK_ERR_ADR;

	// по умолчанию будет возвращено false
	val = false;

	if (adr >= ADR_C_DEF_IS) {
		val = readCoilDef(adr);
	} else if (adr >= ADR_C_PRM_IS) {
		val = readCoilPrm(adr);
	} else if (adr >= ADR_C_PRD_IS) {
		val = readCoilPrd(adr);
	} else if (adr >= ADR_C_FAULT) {
		val = readCoilGlb(adr);
	}

	return CHECK_ERR_NO;
}

/**	Запись флагов.
 *
 *	Проводится проверка корректности адреса. Выход за максимальный или
 *	минимальный адреса считается ошибкой.
 *
 *	Доступные адреса для записи:
 *	- ADR_C_IND_PRD;
 *	- ADR_C_IND_PRM.
 *	При записи 0 (т.е. false) в любой из этих флагов, происходит сброс индикации
 *	команд приемника и передатчика.
 *
 *	@param adr Адрес флага.
 *	@param val Состояние флага.
 * 	@retval CHECK_ERR_NO Ошибок при записи флага не возникло.
 * 	@retval CHECK_ERR_ADR Недопустимый адрес флага.
 *	@retval CHECK_ERR_DEVICE Возникла внутренняя ошибка.
 */
TProtocolModbus::CHECK_ERR TProtocolPcM::writeCoil(uint16_t adr, bool val) {

	if ((adr <= ADR_C_MIN) || (adr >= ADR_C_MAX))
		return CHECK_ERR_ADR;

	if ((adr == ADR_C_IND_PRD) || (adr == ADR_C_IND_PRM))  {
		if (!val)
			sParam_->txComBuf.addFastCom(GB_COM_PRM_RES_IND);
	}

	return CHECK_ERR_NO;
}


/**	Чтение регистров.
 *
 *	Проводится проверка корректности адреса. Если он находится в допустимом
 *	диапазоне, возвращается текущее значение регистра.
 *
 *	C неиспользуемых адресов будет считано 0xFFFF.
 *
 *	@param adr Адрес регистра.
 *	@param val [out] Значение регистра.
 *	@retval CHECK_ERR_NO Ошибок при считывании регистра не возникло.
 *	@retval CHECK_ERR_ADR Недопустимый адрес регистра.
 */
TProtocolModbus::CHECK_ERR TProtocolPcM::readRegister(uint16_t adr, uint16_t &val) {
	//  проверка адресов
	if ((adr <= ADR_REG_MIN) || (adr >= ADR_REG_MAX))
		return CHECK_ERR_ADR;

	// по умолчанию будет возвращено 0xFFFF
	val = 0xFFFF;
	if (adr >= ADR_IC_BSP_MCU) {
		val = readRegVersionIC(adr);
	} else if (adr >= ADR_IND_COM_PRM_16) {
		if (adr == ADR_IND_COM_PRM_16) {
			val = ((uint16_t) sParam_->prm.getIndCom8(1)) << 8;
			val += sParam_->prm.getIndCom8(0);
		} else if (adr == ADR_IND_COM_PRM_32) {
			val = ((uint16_t) sParam_->prm.getIndCom8(3)) << 8;
			val += sParam_->prm.getIndCom8(2);
		} else if (adr == ADR_IND_COM_PRD_16) {
			val = ((uint16_t) sParam_->prd.getIndCom8(1)) << 8;
			val += sParam_->prd.getIndCom8(0);
		} else if (adr == ADR_IND_COM_PRD_32) {
			val = ((uint16_t) sParam_->prd.getIndCom8(3)) << 8;
			val += sParam_->prd.getIndCom8(2);
		}
	} else if (adr >= ADR_MEAS_U_OUT) {
		val = readRegMeasure(adr);
	} else if (adr >= ADR_JRN_DEF_CNT_PWR) {
		val = readJournalDef(adr);
	} else if (adr >= ADR_JRN_PRD_CNT_PWR) {
		val = readJournalPrd(adr);
	} else if (adr >= ADR_JRN_PRM_CNT_PWR) {
		val = readJournalPrm(adr);
	} else if (adr >= ADR_JRN_EVT_CNT_PWR) {
		val = readJournalEvent(adr);
	} else if (adr >= ADR_GLB_FAULT) {
		val = readRegState(adr);
	} else if (adr >= ADR_PASSWORD) {
		// пароли не считываются
	} else if (adr >= ADR_YEAR ) {
		val = readRegDateTime(adr);
	}

	return CHECK_ERR_NO;
}

/**	Запись внутренних регистров.
 *
 * 	Проводится проверка корректности адреса. Выход за максимальный или
 *	минимальный адреса считается ошибкой.
 *
 *	Доступные адреса для записи:
 *	- ADR_YEAR Установка года.
 *	- ADR_MONTH Установка месяца.
 *	- ADR_DAY Установка дня.
 *	- ADR_HOUR Установка часа.
 *	- ADR_MINUTE Установка минут.
 *	- ADR_SECOND Установка секунд.
 *	- ADR_JRN_EVT_NUM Выбор номера записи журнала событий.
 *	- ADR_JRN_PRM_NUM Выбор номера записи журнала приемника.
 *	- ADR JRN_PRD_NUM Выбор номера записи журнала передатчика.
 *	- ADR_JRN_DEF_NUM Выбор номера записи журнала защиты.
 *	- ADR_IND_COM_PRM_16 Сброс индикации всех команд приемника и передатчика.
 *	- ADR_IND_COM_PRM_32 Сброс индикации всех команд приемника и передатчика.
 *	- ADR_IND_COM_PRD_16 Сброс индикации всех команд приемника и передатчика.
 *	- ADR_IND_COM_PRD_32 Сброс индикации всех команд приемника и передатчика.
 *
 *	Сброс индикации осуществляется записью 0 в любой из регистров индикации.
 *	Запись всех регистров времени должна осуществляется одной командой.
 *	В момент записи регистра ADR_SECOND команда на изменение времени помещается
 *	в буфер отправки в БСП.
 *
 *
 *	@param adr Адрес регистра.
 *	@param val Состояние регистра.
 * 	@retval CHECK_ERR_NO Ошибок при считывании регистра не возникло.
 * 	@retval CHECK_ERR_ADR Недопустимый адрес регистра.
 *	@retval CHECK_ERR_DEVICE Возникла внутренняя ошибка.
 */
TProtocolModbus::CHECK_ERR TProtocolPcM::writeRegister(uint16_t adr,
                                                       uint16_t val)
{

	if ((adr <= ADR_REG_MIN	) || (adr >= ADR_REG_MAX))
		return CHECK_ERR_ADR;

	if (adr == ADR_JRN_EVT_NUM) {
		sParam_->jrnEntry.setNumEntry(val);
		sParam_->txComBuf.setInt16(sParam_->jrnEntry.getEntryAdress());
		sParam_->txComBuf.addFastCom(GB_COM_GET_JRN_ENTRY);
	} else if (adr == ADR_JRN_PRM_NUM) {
		sParam_->jrnEntry.setNumEntry(val);
		sParam_->txComBuf.setInt16(sParam_->jrnEntry.getEntryAdress());
		sParam_->txComBuf.addFastCom(GB_COM_PRM_GET_JRN_ENTRY);
	} else if (adr == ADR_JRN_PRD_NUM) {
		sParam_->jrnEntry.setNumEntry(val);
		sParam_->txComBuf.setInt16(sParam_->jrnEntry.getEntryAdress());
		sParam_->txComBuf.addFastCom(GB_COM_PRD_GET_JRN_ENTRY);
	} else if (adr == ADR_JRN_DEF_NUM) {
		sParam_->jrnEntry.setNumEntry(val);
		sParam_->txComBuf.setInt16(sParam_->jrnEntry.getEntryAdress());
		sParam_->txComBuf.addFastCom(GB_COM_DEF_GET_JRN_ENTRY);
	} else if ((adr >= ADR_IND_COM_PRM_16) && (adr <= ADR_IND_COM_PRD_32)) {
		if (val == 0) {
			sParam_->txComBuf.addFastCom(GB_COM_PRM_RES_IND);
		}
	} else if ((adr >= ADR_YEAR) && (adr <= ADR_SECOND)) {
        if (adr == ADR_MONTH) {
            val = ((val >= 1) && (val <= 12)) ? val : 1;
		} else if (adr ==  ADR_DAY) {
            val = ((val >= 1) && (val <= 31)) ? val : 1;
		} else if (adr == ADR_HOUR) {
            val = (val <= 23) ? val : 1;
		} else if (adr == ADR_MINUTE) {
            val = (val <= 59) ? val : 1;
		} else if (adr == ADR_SECOND) {
            val = (val <= 59) ? val : 1;
			sParam_->txComBuf.addFastCom(GB_COM_SET_TIME);
			sParam_->txComBuf.setInt8(1, 8);
		}
        sParam_->txComBuf.setInt8(val, static_cast<uint8_t> (adr - ADR_YEAR));
	}

	return CHECK_ERR_NO;
}

/**	Чтение ID.
 *
 * 	В посылку для ответа помещаются необходимые данные. Последним байтом
 * 	помещается индикатор состояния: ON - 0xFF, OFF- 0x00.
 *
 * 	Будет передано назавание изделия и последним байтом ON если аппарат не
 * 	находится в режиме "Выведен".
 *
 * 	Пример названия изделия: "АВАНТ Р400м-100-В".
 *
 *	@param buf Указатель на буфер, для записи данных.
 *	@param size [out] Передается максимальный размер сообщения, не включая
 *	байт для индикатора состояния. Возвращается кол-во байт данных
 *	записанных в буфер.
 * 	@retval CHECK_ERR_NO Ошибок при считывании  не возникло.
 *	@retval CHECK_ERR_DEVICE Возникла внутренняя ошибка.
 */
TProtocolModbus::CHECK_ERR TProtocolPcM::readID(char *buf, uint8_t &size) {

	// В Р400м вариантов нет
	static const char r400m[] PROGMEM = "АВАНТ Р400м-100-В";
	// В РЗСК выводится наличие поста, кол-во команд на прд, кол-во команд на
	// прм, тип линии (вч/оптика/е1).
	static const char rzsk[]  PROGMEM = "АВАНТ РЗСК-%d%d%d-В";
	// В К400 выводится кол-во команд на прд, кол-во команд на прм
	// и по идее должен выводится тип совместимости и тип линии ?!
	static const char k400[]  PROGMEM = "АВАНТ К400-%d%dВ";
	// TODO В Оптике, надо выводить реалльный тип аппарата !!!
	static const char opto[]  PROGMEM = "АВАНТ ОПТО-%d%d%d-О";
	// Неизвестное устройство
	static const char unknown[]  PROGMEM = "АВАНТ -%d%d%d-X";

	 eGB_TYPE_DEVICE device = sParam_->typeDevice;

	if (device == AVANT_R400M) {
		size = snprintf(buf, size - 1, r400m);
	} else if (device == AVANT_RZSK) {
		uint8_t def = sParam_->def.status.isEnable();
		uint8_t prd = sParam_->prd.getNumCom() / 4;
		uint8_t prm = sParam_->prm.getNumCom() / 4;
		size = snprintf_P(buf, size - 1, rzsk, def, prd, prm);
	} else if (device == AVANT_K400) {
		uint8_t prd = sParam_->prd.getNumCom() / 4;
		uint8_t prm = sParam_->prm.getNumCom() / 4;
		size = snprintf_P(buf, size - 1, k400, prd, prm);
	} else if (device == AVANT_OPTO) {
		uint8_t def = sParam_->def.status.isEnable();
		uint8_t prd = sParam_->prd.getNumCom() / 4;
		uint8_t prm = sParam_->prm.getNumCom() / 4;
		size = snprintf_P(buf, size - 1, opto, def, prd, prm);
	} else {
		uint8_t def = sParam_->def.status.isEnable();
		uint8_t prd = sParam_->prd.getNumCom() / 4;
		uint8_t prm = sParam_->prm.getNumCom() / 4;
		size = snprintf_P(buf, size - 1, rzsk, def, prd, prm);
	}

	eGB_REGIME reg = sParam_->glb.status.getRegime();
	buf[size++] = (reg == GB_REGIME_DISABLED) ? 0x00 : 0xFF;

	return CHECK_ERR_NO;
}

/**	Считывание флагов общих.
 *
 *	@param adr Адрес флага.
 * 	@return Состояние флага.
 */
bool TProtocolPcM::readCoilGlb(uint16_t adr) {
	bool val = false;

	if (adr >= ADR_C_GLB_WARNING_1) {
		if (adr <= ADR_C_GLB_WARNING_16) {
			val = sParam_->glb.status.isWarning(adr - ADR_C_GLB_WARNING_1);
		}
	} else if (adr >= ADR_C_GLB_FAULT_1) {
		if (adr <= ADR_C_GLB_FAULT_16) {
			val = sParam_->glb.status.isFault(adr - ADR_C_GLB_FAULT_1);
		}
	} else if (adr == ADR_C_IND_PRM) {
		val = sParam_->prm.isIndCom();
	} else if (adr == ADR_C_IND_PRD) {
		val = sParam_->prd.isIndCom();
	} else if (adr == ADR_C_WARNING) {
		val = sParam_->glb.status.isWarning();
		val |= sParam_->def.status.isEnable()&& sParam_->def.status.isWarning();
		val |= sParam_->prm.status.isEnable()&& sParam_->prm.status.isWarning();
		val |= sParam_->prd.status.isEnable()&& sParam_->prd.status.isWarning();
	} else if (adr == ADR_C_FAULT) {
		val = sParam_->glb.status.isFault();
		val |= sParam_->def.status.isEnable()&& sParam_->def.status.isFault();
		val |= sParam_->prm.status.isEnable()&& sParam_->prm.status.isFault();
		val |= sParam_->prd.status.isEnable()&& sParam_->prd.status.isFault();
	}

	return val;
}

/**	Считывание флагов передатчика.
 *
 *	@param adr Адрес флага.
 * 	@return Состояние флага.
 */
bool TProtocolPcM::readCoilPrd(uint16_t adr) {
	bool val = false;

	if (adr >= ADR_C_PRD_IND_1) {
		if (adr <= ADR_C_PRD_IND_32) {
			val = sParam_->prd.getIndCom(adr - ADR_C_PRD_IND_1);
		}
	} else if (adr >= ADR_C_PRD_WARNING_1) {
		if (adr <= ADR_C_PRD_WARNING_16) {
			val = sParam_->prd.status.isWarning(adr - ADR_C_PRD_WARNING_1);
		}
	} else if (adr >= ADR_C_PRD_FAULT_1) {
		if (adr <= ADR_C_PRD_FAULT_16) {
			val = sParam_->prd.status.isFault(adr - ADR_C_PRD_FAULT_1);
		}
	} else if (adr == ADR_C_PRD_IS) {
		val = sParam_->prd.status.isEnable();
	}

	return val;
}

/**	Считывание флагов приемника.
 *
 *	@param adr Адрес флага.
 * 	@return Состояние флага.
 */
bool TProtocolPcM::readCoilPrm(uint16_t adr) {
	bool val = false;

	if (adr >= ADR_C_PRM_IND_1) {
		if (adr <= ADR_C_PRM_IND_32) {
			val = sParam_->prm.getIndCom(adr - ADR_C_PRM_IND_1);
		}
	} else if (adr >= ADR_C_PRM_WARNING_1) {
		if (adr <= ADR_C_PRM_WARNING_16) {
			val = sParam_->prm.status.isWarning(adr - ADR_C_PRM_WARNING_1);
		}
	} else if (adr >= ADR_C_PRM_FAULT_1) {
		if (adr <= ADR_C_PRM_FAULT_16) {
			val = sParam_->prm.status.isFault(adr - ADR_C_PRM_FAULT_1);
		}
	} else if (adr == ADR_C_PRM_IS) {
		val = sParam_->prm.status.isEnable();
	}

	return val;
}

/**	Считывание флагов защиты.
 *
 *	@param adr Адрес флага.
 * 	@return Состояние флага.
 */
bool TProtocolPcM::readCoilDef(uint16_t adr) {
	bool val = false;

	if (adr >= ADR_C_DEF_WARNING_1) {
		if (adr <= ADR_C_DEF_WARNING_16) {
			val = sParam_->def.status.isWarning(adr - ADR_C_DEF_WARNING_1);
		}
	} else if (adr >= ADR_C_DEF_FAULT_1) {
		if (adr <= ADR_C_DEF_FAULT_16) {
			val = sParam_->def.status.isFault(adr - ADR_C_DEF_FAULT_1);
		}
	} else if (adr == ADR_C_DEF_IS) {
		val = sParam_->def.status.isEnable();
	}

	return val;
}

/**	Считывание из регистров даты и времени.
 *
 *	@param adr Адрес регистра.
 * 	@return Значение регистра.
 */
uint16_t TProtocolPcM::readRegDateTime(uint16_t adr) {
	uint16_t val = 0xFFFF;

	if (adr == ADR_YEAR) {
		val = sParam_->DateTime.getYear();
	} else if (adr == ADR_MONTH) {
		val = sParam_->DateTime.getMonth();
	} else if (adr == ADR_DAY) {
		val = sParam_->DateTime.getDay();
	} else if (adr == ADR_HOUR) {
		val = sParam_->DateTime.getHour();
	} else if (adr == ADR_MINUTE) {
		val = sParam_->DateTime.getMinute();
	} else if (adr == ADR_SECOND) {
		val = sParam_->DateTime.getSecond();
	}

	return val;
}

/**	Считывание из регистров состояния.
 *
 *	@param adr Адрес регистра.
 * 	@return Значение регистра.
 */
uint16_t TProtocolPcM::readRegState(uint16_t adr) {
	uint16_t val = 0xFFFF;

	if (adr == ADR_GLB_FAULT) {
		val = sParam_->glb.status.getFaults();
	} else if (adr == ADR_GLB_WARNING) {
		val = sParam_->glb.status.getWarnings();
	} else if (adr == ADR_PRM_FAULT) {
		val = sParam_->prm.status.getFaults();
	} else if (adr == ADR_PRM_WARNING) {
		val = sParam_->prm.status.getWarnings();
	} else if (adr == ADR_PRM_REGIME) {
		val = sParam_->prm.status.getRegime();
	} else if (adr == ADR_PRM_STATE) {
		val = sParam_->prm.status.getState();
	} else if (adr == ADR_PRM_DOP) {
		val = sParam_->prm.status.getDopByte();
	} else if (adr == ADR_PRD_FAULT) {
		val = sParam_->prd.status.getFaults();
	} else if (adr == ADR_PRD_WARNING) {
		val = sParam_->prd.status.getWarnings();
	} else if (adr == ADR_PRD_REGIME) {
		val = sParam_->prd.status.getRegime();
	} else if (adr == ADR_PRD_STATE) {
		val = sParam_->prd.status.getState();
	} else if (adr == ADR_PRD_DOP) {
		val = sParam_->prd.status.getDopByte();
	} else if (adr == ADR_DEF_FAULT) {
		val = sParam_->def.status.getFaults();
	} else if (adr == ADR_DEF_WARNING) {
		val = sParam_->def.status.getWarnings();
	} else if (adr == ADR_DEF_REGIME) {
		val = sParam_->def.status.getRegime();
	} else if (adr == ADR_DEF_STATE) {
		val = sParam_->def.status.getState();
	} else if (adr == ADR_DEF_DOP) {
		val = sParam_->def.status.getDopByte();
	}

	return val;
}


/**	Cчитывание из регистров измеряемых параметров.
 *
 * 	@param adr Адрес регистра.
 * 	@return Значение регистра.
 */
uint16_t TProtocolPcM::readRegMeasure(uint16_t adr) {
	uint16_t val = 0xFFFF;

	if (adr == ADR_MEAS_U_OUT) {
		val = ((uint16_t) sParam_->measParam.getVoltageOutInt() * 10);
		val += (uint16_t) sParam_->measParam.getVoltageOutFract();
	} else if (adr == ADR_MEAS_I_OUT) {
		val = sParam_->measParam.getCurrentOut();
	} else if (adr == ADR_MEAS_UC1) {
		val = sParam_->measParam.getVoltageCf();
	} else if (adr == ADR_MEAS_UC2) {
		val = sParam_->measParam.getVoltageCf2();
	} else if (adr == ADR_MEAS_UN1) {
		val = sParam_->measParam.getVoltageNoise();
	} else if (adr == ADR_MEAS_UN2) {
		val = sParam_->measParam.getVoltageNoise2();
	} else if (adr == ADR_MEAS_UD1) {
		val = sParam_->measParam.getVoltageDef();
	} else if (adr == ADR_MEAS_UD2) {
		val = sParam_->measParam.getVoltageDef2();
	} else if (adr == ADR_MEAS_PW) {
		val = sParam_->measParam.getPulseWidth();
	}

	return val;
}

/**	Cчитывание из регистров версий прошивок.
 *
 * 	@param adr Адрес регистра.
 * 	@return Значение регистра.
 */
uint16_t TProtocolPcM::readRegVersionIC(uint16_t adr) {
	uint16_t val = 0xFFFF;

	if (adr == ADR_IC_BSP_MCU) {
		val = sParam_->glb.getVersProgIC(GB_IC_BSP_MCU);
	} else if (adr == ADR_IC_BSP_DSP) {
		val = sParam_->glb.getVersProgIC(GB_IC_BSP_DSP);
	} else if (adr == ADR_IC_PI_MCU) {
		val = sParam_->glb.getVersProgIC(GB_IC_PI_MCU);
	} else if (adr == ADR_IC_BSK_PRD1) {
		val = sParam_->glb.getVersProgIC(GB_IC_BSK_PLIS_PRD1);
	} else if (adr == ADR_IC_BSK_PRM1) {
		val = sParam_->glb.getVersProgIC(GB_IC_BSK_PLIS_PRM1);
	} else if (adr == ADR_IC_BSK_PRD2) {
		val = sParam_->glb.getVersProgIC(GB_IC_BSK_PLIS_PRD2);
	} else if (adr == ADR_IC_BSK_PRM2) {
		val = sParam_->glb.getVersProgIC(GB_IC_BSK_PLIS_PRM2);
	} else if (adr == ADR_IC_BSZ) {
		val = sParam_->glb.getVersProgIC(GB_IC_BSZ_PLIS);
	}

	return val;
}

/**	Считывание журнала событий.
 *
 * 	@param adr Адрес регистра.
 * 	@return Значние регистра.
 */
uint16_t TProtocolPcM::readJournalEvent(uint16_t adr) {
	uint16_t val = 0xFFFF;
	TJournalEntry *jrn = &sParam_->jrnEntry;

	if (sParam_->jrnEntry.getCurrentDevice() != GB_DEVICE_GLB) {
		sParam_->jrnEntry.clear();
		sParam_->jrnEntry.setCurrentDevice(GB_DEVICE_GLB);
		uint16_t t = 0;
		eGB_TYPE_DEVICE device = sParam_->typeDevice;
		if (device == AVANT_K400) {
			t = GLB_JRN_EVENT_K400_MAX;
		} else if (device == AVANT_R400M) {
			t = GLB_JRN_EVENT_R400M_MAX;
		} else if (device == AVANT_RZSK) {
			t = GLB_JRN_EVENT_RZSK_MAX;
		} else if (device == AVANT_OPTO) {
			t = GLB_JRN_EVENT_OPTO_MAX;
		}
		sParam_->jrnEntry.setMaxNumJrnEntries(t);
		sParam_->txComBuf.addFastCom(GB_COM_GET_JRN_CNT);
	} else {
		if (adr == ADR_JRN_EVT_CNT_PWR) {
			sParam_->txComBuf.addFastCom(GB_COM_GET_JRN_CNT);
		} else if (adr == ADR_JRN_EVT_CNT) {
			val = jrn->getNumJrnEntries();
			sParam_->txComBuf.addFastCom(GB_COM_GET_JRN_CNT);
		} else {
			if (jrn->isReady()) {
				if (adr == ADR_JRN_EVT_NUM) {
					val = jrn->getCurrentEntry();
				} else if (adr == ADR_JRN_EVT_DEV) {
					val = jrn->getDeviceJrn();
				} else if (adr == ADR_JRN_EVT_TYPE) {
					val = jrn->getEventType();
				} else if (adr == ADR_JRN_EVT_REG) {
					val = jrn->getRegime();
				} else if (adr == ADR_JRN_EVT_MSECOND) {
					val = jrn->dateTime.getMsSecond();
				} else if (adr == ADR_JRN_EVT_SECOND) {
					val = jrn->dateTime.getSecond();
				} else if (adr == ADR_JRN_EVT_MINUTE) {
					val = jrn->dateTime.getMinute();
				} else if (adr == ADR_JRN_EVT_HOUR) {
					val = jrn->dateTime.getHour();
				} else if (adr == ADR_JRN_EVT_WDAY) {
					val = jrn->dateTime.getDayOfWeek();
				} else if (adr == ADR_JRN_EVT_DAY) {
					val = jrn->dateTime.getDay();
				} else if (adr == ADR_JRN_EVT_MONTH) {
					val = jrn->dateTime.getMonth();
				} else if (adr == ADR_JRN_EVT_YEAR) {
					val = jrn->dateTime.getYear();
				}
			}
		}
	}

	return val;
}

/**	Считывание журнала приемника.
 *
 * 	@param adr Адрес регистра.
 * 	@return Значние регистра.
 */
uint16_t TProtocolPcM::readJournalPrm(uint16_t adr) {
	uint16_t val = 0xFFFF;
	TJournalEntry *jrn = &sParam_->jrnEntry;

	if (sParam_->jrnEntry.getCurrentDevice() != GB_DEVICE_PRM) {
		sParam_->jrnEntry.clear();
		sParam_->jrnEntry.setCurrentDevice(GB_DEVICE_PRM);
		uint16_t t = 0;
		eGB_TYPE_DEVICE device = sParam_->typeDevice;
		if (device == AVANT_K400) {
			t = GLB_JRN_PRM_K400_MAX;
		} else if (device == AVANT_R400M) {
			t = 0;
		} else if (device == AVANT_RZSK) {
			t = GLB_JRN_PRM_RZSK_MAX;
		} else if (device == AVANT_OPTO) {
			t = GLB_JRN_PRM_OPTO_MAX;
		}
		sParam_->jrnEntry.setMaxNumJrnEntries(t);
		sParam_->txComBuf.addFastCom(GB_COM_PRM_GET_JRN_CNT);
	} else {
		if (adr == ADR_JRN_PRM_CNT_PWR) {
			sParam_->txComBuf.addFastCom(GB_COM_PRM_GET_JRN_CNT );
		} else if (adr == ADR_JRN_PRM_CNT) {
			val = jrn->getNumJrnEntries();
			sParam_->txComBuf.addFastCom(GB_COM_PRM_GET_JRN_CNT );
		} else {
			if (jrn->isReady()) {
				if (adr == ADR_JRN_PRM_NUM) {
					val = jrn->getCurrentEntry();
				} else if (adr == ADR_JRN_PRM_DEV) {
					val = jrn->getDeviceJrn();
				} else if (adr == ADR_JRN_PRD_NUM_COM) {
					val = jrn->getNumCom();
				} else if (adr == ADR_JRN_PRM_EVENT) {
					val = jrn->getEventType();
				} else if (adr == ADR_JRN_PRM_MSECOND) {
					val = jrn->dateTime.getMsSecond();
				} else if (adr == ADR_JRN_PRM_SECOND) {
					val = jrn->dateTime.getSecond();
				} else if (adr == ADR_JRN_PRM_MINUTE) {
					val = jrn->dateTime.getMinute();
				} else if (adr == ADR_JRN_PRM_HOUR) {
					val = jrn->dateTime.getHour();
				} else if (adr == ADR_JRN_PRM_WDAY) {
					val = jrn->dateTime.getDayOfWeek();
				} else if (adr == ADR_JRN_PRM_DAY) {
					val = jrn->dateTime.getDay();
				} else if (adr == ADR_JRN_PRM_MONTH) {
					val = jrn->dateTime.getMonth();
				} else if (adr == ADR_JRN_PRM_YEAR) {
					val = jrn->dateTime.getYear();
				}
			}
		}
	}

	return val;
}

/**	Считывание журнала передатчика.
 *
 * 	@param adr Адрес регистра.
 * 	@return Значние регистра.
 */
uint16_t TProtocolPcM::readJournalPrd(uint16_t adr) {
	uint16_t val = 0xFFFF;
	TJournalEntry *jrn = &sParam_->jrnEntry;

	if (sParam_->jrnEntry.getCurrentDevice() != GB_DEVICE_PRD) {
		sParam_->jrnEntry.clear();
		sParam_->jrnEntry.setCurrentDevice(GB_DEVICE_PRD);
		uint16_t t = 0;
		eGB_TYPE_DEVICE device = sParam_->typeDevice;
		if (device == AVANT_K400) {
			t = GLB_JRN_PRD_K400_MAX;
		} else if (device == AVANT_R400M) {
			t = 0;
		} else if (device == AVANT_RZSK) {
			t = GLB_JRN_PRD_RZSK_MAX;
		} else if (device == AVANT_OPTO) {
			t = GLB_JRN_PRD_OPTO_MAX;
		}
		sParam_->jrnEntry.setMaxNumJrnEntries(t);
		sParam_->txComBuf.addFastCom(GB_COM_PRD_GET_JRN_CNT);
	} else {
		if (adr == ADR_JRN_PRD_CNT_PWR) {
			sParam_->txComBuf.addFastCom(GB_COM_PRD_GET_JRN_CNT );
		} else if (adr == ADR_JRN_PRD_CNT) {
			val = jrn->getNumJrnEntries();
			sParam_->txComBuf.addFastCom(GB_COM_PRD_GET_JRN_CNT );
		} else {
			if (jrn->isReady()) {
				if (adr == ADR_JRN_PRD_NUM) {
					val = jrn->getCurrentEntry();
				} else if (adr == ADR_JRN_PRD_DEV) {
					val = jrn->getDeviceJrn();
				} else if (adr == ADR_JRN_PRD_NUM_COM) {
					val = jrn->getNumCom();
				} else if (adr == ADR_JRN_PRD_EVENT) {
					val = jrn->getEventType();
				} else if (adr == ADR_JRN_PRD_SOURCE) {
					val = jrn->getSourceCom();
				} else if (adr == ADR_JRN_PRD_MSECOND) {
					val = jrn->dateTime.getMsSecond();
				} else if (adr == ADR_JRN_PRD_SECOND) {
					val = jrn->dateTime.getSecond();
				} else if (adr == ADR_JRN_PRD_MINUTE) {
					val = jrn->dateTime.getMinute();
				} else if (adr == ADR_JRN_PRD_HOUR) {
					val = jrn->dateTime.getHour();
				} else if (adr == ADR_JRN_PRD_WDAY) {
					val = jrn->dateTime.getDayOfWeek();
				} else if (adr == ADR_JRN_PRD_DAY) {
					val = jrn->dateTime.getDay();
				} else if (adr == ADR_JRN_PRD_MONTH) {
					val = jrn->dateTime.getMonth();
				} else if (adr == ADR_JRN_PRD_YEAR) {
					val = jrn->dateTime.getYear();
				}
			}
		}
	}

	return val;
}

/**	Считывание журнала защиты.
 *
 * 	@param adr Адрес регистра.
 * 	@return Значние регистра.
 */
uint16_t TProtocolPcM::readJournalDef(uint16_t adr) {
	uint16_t val = 0xFFFF;
	TJournalEntry *jrn = &sParam_->jrnEntry;

	if (sParam_->jrnEntry.getCurrentDevice() != GB_DEVICE_DEF) {
		sParam_->jrnEntry.clear();
		sParam_->jrnEntry.setCurrentDevice(GB_DEVICE_DEF);
		uint16_t t = 0;
		eGB_TYPE_DEVICE device = sParam_->typeDevice;
		if (device == AVANT_K400) {
			t =  GLB_JRN_DEF_K400_MAX;
		} else if (device == AVANT_R400M) {
			t = GLB_JRN_DEF_R400M_MAX;
		} else if (device == AVANT_RZSK) {
			t = GLB_JRN_DEF_RZSK_MAX;
		} else if (device == AVANT_OPTO) {
			t = GLB_JRN_DEF_OPTO_MAX;
		}
		sParam_->jrnEntry.setMaxNumJrnEntries(t);
		sParam_->txComBuf.addFastCom(GB_COM_DEF_GET_JRN_CNT);
	} else {
		if (adr == ADR_JRN_DEF_CNT_PWR) {
			sParam_->txComBuf.addFastCom(GB_COM_DEF_GET_JRN_CNT );
		} else if (adr == ADR_JRN_DEF_CNT) {
			val = jrn->getNumJrnEntries();
			sParam_->txComBuf.addFastCom(GB_COM_DEF_GET_JRN_CNT );
		} else {
			if (jrn->isReady()) {
				if (adr == ADR_JRN_DEF_NUM) {
					val = jrn->getCurrentEntry();
				} else if (adr == ADR_JRN_DEF_DEV) {
					val = jrn->getDeviceJrn();
				} else if (adr == ADR_JRN_DEF_TYPE) {
					val = jrn->getSignals();
				} else if (adr == ADR_JRN_DEF_STATE) {
					val = jrn->getEventType();
				} else if (adr == ADR_JRN_DEF_MSECOND) {
					val = jrn->dateTime.getMsSecond();
				} else if (adr == ADR_JRN_DEF_SECOND) {
					val = jrn->dateTime.getSecond();
				} else if (adr == ADR_JRN_DEF_MINUTE) {
					val = jrn->dateTime.getMinute();
				} else if (adr == ADR_JRN_DEF_HOUR) {
					val = jrn->dateTime.getHour();
				} else if (adr == ADR_JRN_DEF_WDAY) {
					val = jrn->dateTime.getDayOfWeek();
				} else if (adr == ADR_JRN_DEF_DAY) {
					val = jrn->dateTime.getDay();
				} else if (adr == ADR_JRN_DEF_MONTH) {
					val = jrn->dateTime.getMonth();
				} else if (adr == ADR_JRN_DEF_YEAR) {
					val = jrn->dateTime.getYear();
				}
			}
		}
	}

	return val;
}
