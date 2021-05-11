/*
 * protocolPcI.cpp
 *
 *  Created on: 14.05.2015
 *      Author: Shcheblykin
 */

#include "protocolPcI.h"


// Адреса элементов информации (EInfoElement)
const uint16_t TProtocolPcI::c_adrIE2[] PROGMEM = {
		201,	// IE2_ERROR
		202, 	// IE2_WARNING
		203, 	// IE2_PRD_COM
		204,	// IE2_PRM_COM
		205,	// IE2_DI_PUSK_PRM
		206,	// IE2_DI_RESET_IND
		207,	// IE2_DI_TM_CONTROL
		301,	// IE2_GLB_ERROR_H0001
		302,	// IE2_GLB_ERROR_H0002
		303,	// IE2_GLB_ERROR_H0004
		304,	// IE2_GLB_ERROR_H0008
		305,	// IE2_GLB_ERROR_H0010
		306,	// IE2_GLB_ERROR_H0020
		307,	// IE2_GLB_ERROR_H0040
		308,	// IE2_GLB_ERROR_H0080
		309,	// IE2_GLB_ERROR_H0100
		310,	// IE2_GLB_ERROR_H0200
		311,	// IE2_GLB_ERROR_H0400
		312,	// IE2_GLB_ERROR_H0800
		313,	// IE2_GLB_ERROR_H1000
		314,	// IE2_GLB_ERROR_H2000
		315,	// IE2_GLB_ERROR_H4000
		316,	// IE2_GLB_ERROR_H8000
		317,	// IE2_GLB_WARNING_H0001
		318,	// IE2_GLB_WARNING_H0002
		319,	// IE2_GLB_WARNING_H0004
		320,	// IE2_GLB_WARNING_H0008
		321,	// IE2_GLB_WARNING_H0010
		322,	// IE2_GLB_WARNING_H0020
		323,	// IE2_GLB_WARNING_H0040
		324,	// IE2_GLB_WARNING_H0080
		325,	// IE2_GLB_WARNING_H0100
		326,	// IE2_GLB_WARNING_H0200
		327,	// IE2_GLB_WARNING_H0400
		328,	// IE2_GLB_WARNING_H0800
		329,	// IE2_GLB_WARNING_H1000
		330,	// IE2_GLB_WARNING_H2000
		331,	// IE2_GLB_WARNING_H4000
		332,	// IE2_GLB_WARNING_H8000
		400,	// IE2_PRD_ON
		401,	// IE2_PRD_ERROR_H0001
		402,	// IE2_PRD_ERROR_H0002
		403,	// IE2_PRD_ERROR_H0004
		404,	// IE2_PRD_ERROR_H0008
		405,	// IE2_PRD_ERROR_H0010
		406,	// IE2_PRD_ERROR_H0020
		407,	// IE2_PRD_ERROR_H0040
		408,	// IE2_PRD_ERROR_H0080
		409,	// IE2_PRD_ERROR_H0100
		410,	// IE2_PRD_ERROR_H0200
		411,	// IE2_PRD_ERROR_H0400
		412,	// IE2_PRD_ERROR_H0800
		413,	// IE2_PRD_ERROR_H1000
		414,	// IE2_PRD_ERROR_H2000
		415,	// IE2_PRD_ERROR_H4000
		416,	// IE2_PRD_ERROR_H8000
		417,	// IE2_PRD_WARNING_H0001
		418,	// IE2_PRD_WARNING_H0002
		419,	// IE2_PRD_WARNING_H0004
		420,	// IE2_PRD_WARNING_H0008
		421,	// IE2_PRD_WARNING_H0010
		422,	// IE2_PRD_WARNING_H0020
		423,	// IE2_PRD_WARNING_H0040
		424,	// IE2_PRD_WARNING_H0080
		425,	// IE2_PRD_WARNING_H0100
		426,	// IE2_PRD_WARNING_H0200
		427,	// IE2_PRD_WARNING_H0400
		428,	// IE2_PRD_WARNING_H0800
		429,	// IE2_PRD_WARNING_H1000
		430,	// IE2_PRD_WARNING_H2000
		431,	// IE2_PRD_WARNING_H4000
		432,	// IE2_PRD_WARNING_H8000
		450,	// IE2_PRD_COM_01
		451,	// IE2_PRD_COM_02
		452,	// IE2_PRD_COM_03
		453,	// IE2_PRD_COM_04
		454,	// IE2_PRD_COM_05
		455,	// IE2_PRD_COM_06
		456,	// IE2_PRD_COM_07
		457,	// IE2_PRD_COM_08
		458,	// IE2_PRD_COM_09
		459,	// IE2_PRD_COM_10
		460,	// IE2_PRD_COM_11
		461,	// IE2_PRD_COM_12
		462,	// IE2_PRD_COM_13
		463,	// IE2_PRD_COM_14
		464,	// IE2_PRD_COM_15
		465,	// IE2_PRD_COM_16
		466,	// IE2_PRD_COM_17
		467,	// IE2_PRD_COM_18
		468,	// IE2_PRD_COM_19
		469,	// IE2_PRD_COM_20
		470,	// IE2_PRD_COM_21
		471,	// IE2_PRD_COM_22
		472,	// IE2_PRD_COM_23
		473,	// IE2_PRD_COM_24
		474,	// IE2_PRD_COM_25
		475,	// IE2_PRD_COM_26
		476,	// IE2_PRD_COM_27
		477,	// IE2_PRD_COM_28
		478,	// IE2_PRD_COM_29
		479,	// IE2_PRD_COM_30
		480,	// IE2_PRD_COM_31
		481,	// IE2_PRD_COM_32
		485,	// IE2_PRD_DISABLED
		486,	// IE2_PRD_ENABLED
		487,	// IE2_PRD_TEST
		500,	// IE2_PRM_ON
		501,	// IE2_PRM_ERROR_H0001
		502,	// IE2_PRM_ERROR_H0002
		503,	// IE2_PRM_ERROR_H0004
		504,	// IE2_PRM_ERROR_H0008
		505,	// IE2_PRM_ERROR_H0010
		506,	// IE2_PRM_ERROR_H0020
		507,	// IE2_PRM_ERROR_H0040
		508,	// IE2_PRM_ERROR_H0080
		509,	// IE2_PRM_ERROR_H0100
		510,	// IE2_PRM_ERROR_H0200
		511,	// IE2_PRM_ERROR_H0400
		512,	// IE2_PRM_ERROR_H0800
		513,	// IE2_PRM_ERROR_H1000
		514,	// IE2_PRM_ERROR_H2000
		515,	// IE2_PRM_ERROR_H4000
		516,	// IE2_PRM_ERROR_H8000
		517,	// IE2_PRM_WARNING_H0001
		518,	// IE2_PRM_WARNING_H0002
		519,	// IE2_PRM_WARNING_H0004
		520,	// IE2_PRM_WARNING_H0008
		521,	// IE2_PRM_WARNING_H0010
		522,	// IE2_PRM_WARNING_H0020
		523,	// IE2_PRM_WARNING_H0040
		524,	// IE2_PRM_WARNING_H0080
		525,	// IE2_PRM_WARNING_H0100
		526,	// IE2_PRM_WARNING_H0200
		527,	// IE2_PRM_WARNING_H0400
		528,	// IE2_PRM_WARNING_H0800
		529,	// IE2_PRM_WARNING_H1000
		530,	// IE2_PRM_WARNING_H2000
		531,	// IE2_PRM_WARNING_H4000
		532,	// IE2_PRM_WARNING_H8000
		550,	// IE2_PRM_COM_01
		551,	// IE2_PRM_COM_02
		552,	// IE2_PRM_COM_03
		553,	// IE2_PRM_COM_04
		554,	// IE2_PRM_COM_05
		555,	// IE2_PRM_COM_06
		556,	// IE2_PRM_COM_07
		557,	// IE2_PRM_COM_08
		558,	// IE2_PRM_COM_09
		559,	// IE2_PRM_COM_10
		560,	// IE2_PRM_COM_11
		561,	// IE2_PRM_COM_12
		562,	// IE2_PRM_COM_13
		563,	// IE2_PRM_COM_14
		564,	// IE2_PRM_COM_15
		565,	// IE2_PRM_COM_16
		566,	// IE2_PRM_COM_17
		567,	// IE2_PRM_COM_18
		568,	// IE2_PRM_COM_19
		569,	// IE2_PRM_COM_20
		570,	// IE2_PRM_COM_21
		571,	// IE2_PRM_COM_22
		572,	// IE2_PRM_COM_23
		573,	// IE2_PRM_COM_24
		574,	// IE2_PRM_COM_25
		575,	// IE2_PRM_COM_26
		576,	// IE2_PRM_COM_27
		577,	// IE2_PRM_COM_28
		578,	// IE2_PRM_COM_29
		579,	// IE2_PRM_COM_30
		580,	// IE2_PRM_COM_31
		581,	// IE2_PRM_COM_32
		585,	// IE2_PRM_DISABLED
		586,	// IE2_PRM_READY
		587,	// IE2_PRM_ENABLED
		588,	// IE2_PRM_TEST
		600,	// IE2_DEF_ON
		601,	// IE2_DEF_ERROR_H0001
		602,	// IE2_DEF_ERROR_H0002
		603,	// IE2_DEF_ERROR_H0004
		604,	// IE2_DEF_ERROR_H0008
		605,	// IE2_DEF_ERROR_H0010
		606,	// IE2_DEF_ERROR_H0020
		607,	// IE2_DEF_ERROR_H0040
		608,	// IE2_DEF_ERROR_H0080
		609,	// IE2_DEF_ERROR_H0100
		610,	// IE2_DEF_ERROR_H0200
		611,	// IE2_DEF_ERROR_H0400
		612,	// IE2_DEF_ERROR_H0800
		613,	// IE2_DEF_ERROR_H1000
		614,	// IE2_DEF_ERROR_H2000
		615,	// IE2_DEF_ERROR_H4000
		616,	// IE2_DEF_ERROR_H8000
		617,	// IE2_DEF_WARNING_H0001
		618,	// IE2_DEF_WARNING_H0002
		619,	// IE2_DEF_WARNING_H0004
		620,	// IE2_DEF_WARNING_H0008
		621,	// IE2_DEF_WARNING_H0010
		622,	// IE2_DEF_WARNING_H0020
		623,	// IE2_DEF_WARNING_H0040
		624,	// IE2_DEF_WARNING_H0080
		625,	// IE2_DEF_WARNING_H0100
		626,	// IE2_DEF_WARNING_H0200
		627,	// IE2_DEF_WARNING_H0400
		628,	// IE2_DEF_WARNING_H0800
		629,	// IE2_DEF_WARNING_H1000
		630,	// IE2_DEF_WARNING_H2000
		631,	// IE2_DEF_WARNING_H4000
		632		// IE2_DEF_WARNING_H8000
};

// Конструктор
TProtocolPcI::TProtocolPcI(stGBparam *sParam, uint8_t *buf, uint8_t size) :
sParam_(sParam), CIec101(buf, size) {

	COMPILE_TIME_ASSERT(SIZE_OF(c_adrIE2) == IE2_MAX);

	for(uint16_t i = 0; i < IE2_MAX; i++) {
		m_flags[i] = getValue(static_cast<EInfoElement2> (i));
	}
}

// Функция отправки сообщения.
uint8_t TProtocolPcI::send() {
	return sendData();
}

// Проверка наличия данных класса 1 на передачу.
bool TProtocolPcI::checkEventClass1(uint16_t &adr, bool &val, SCp56Time2a &time) {
	static uint16_t ladr;	// Последний переданный адрес.

	if (!sParam_->jrnScada.isReadyToSend())
		return false;

	TJrnSCADA *jrn = &sParam_->jrnScada;

	uint16_t ms = jrn->dtime.getMsSecond();

	if (jrn->isJrnEvent()) {
		adr = c_adrIe1Event1 + jrn->getEvent() - 1;
		// Для событий формируются сигналы начала и окончания с разницей 1 мс.
		if (ladr != adr) {
			ladr = adr;
			val = true;
			ms = (ms < 999) ? ms : ms - 1;
		} else {
			ladr = 0;
			val = false;
			ms = (ms < 999) ? ms + 1 : ms;
			sParam_->jrnScada.setReadyToEvent();
		}
	} else if (jrn->isJrnPrm()) {
		val = jrn->getEvent();
		adr = c_adrIe1PrmCom1 + jrn->getCom() - 1;
		sParam_->jrnScada.setReadyToEvent();
	} else if (jrn->isJrnPrd()) {
		val = jrn->getEvent();
		adr = (jrn->getComSource()) ? c_adrIe1PrdCCom1 : c_adrIe1PrdDCom1;
		adr += jrn->getCom() - 1;
		sParam_->jrnScada.setReadyToEvent();
	}
	else if (jrn->isJrnDef()) {
		adr = jrn->getDefEvent(val);
		if (adr == 0) {
			sParam_->jrnScada.setReadyToEvent();
			return false;
		}
		adr += c_adrIe1DefSignal1 - 1;
	}
	else {
		sParam_->jrnScada.setReadyToEvent();
		return false;
	}

    time.years 			= jrn->dtime.getYear();
    time.months 		= jrn->dtime.getMonth();
    time.dayOfMonth 	= jrn->dtime.getDay();
    time.hours 			= jrn->dtime.getHour();
    time.minutes 		= jrn->dtime.getMinute();
    time.milliseconds 	= jrn->dtime.getSecond() * 1000;
	time.milliseconds  += ms;

	return true;
}

// Проверка наличия данных класса 2 на передачу.
bool TProtocolPcI::checkEventClass2(uint16_t &adr, bool &val, SCp56Time2a &time) {
	for(uint8_t i = 0; i < IE2_MAX; i++) {
		bool t = getValue(static_cast<EInfoElement2> (i));

		if (m_flags[i] != t) {
			m_flags[i] = t;

			val = t;
			adr = pgm_read_word(&c_adrIE2[i]);

			time.years 			= sParam_->DateTime.getYear();
			time.months 		= sParam_->DateTime.getMonth();
			time.dayOfMonth 	= sParam_->DateTime.getDay();
			time.hours 			= sParam_->DateTime.getHour();
			time.minutes 		= sParam_->DateTime.getMinute();
			time.milliseconds 	= sParam_->DateTime.getSecond()*1000;
			time.milliseconds  += sParam_->DateTime.getMsSecond();

			return true;
		}
	}

	return false;
}

// Обработка ответа на команду опроса.
bool TProtocolPcI::procInterrog(uint16_t &adr, bool &val) {
	static EInfoElement2 e = IE2_ERROR;

	if (e >= IE2_MAX) {
		e = IE2_ERROR;
		return false;
	}

	adr = pgm_read_word(&c_adrIE2[e]);
	val = getValue(e);

	e = static_cast<EInfoElement2>(e + 1);

	return true;
}

// Установка времени
bool TProtocolPcI::procSetTime() {
	// Заполнение команды для передачи нового времени в БСП
	uint8_t i = 0;
    sParam_->txComBuf.setInt8(stTime.years, i++);
    sParam_->txComBuf.setInt8(stTime.months, i++);
    sParam_->txComBuf.setInt8(stTime.dayOfMonth, i++);
    sParam_->txComBuf.setInt8(stTime.hours, i++);
    sParam_->txComBuf.setInt8(stTime.minutes, i++);
    sParam_->txComBuf.setInt8((stTime.milliseconds / 1000), i++);

	uint16_t ms = stTime.milliseconds % 1000 + getDelay();
    sParam_->txComBuf.setInt8(static_cast<uint8_t> (ms), i++);
    sParam_->txComBuf.setInt8(static_cast<uint8_t> (ms >> 8), i++);
	sParam_->txComBuf.setInt8(1, i++);
	sParam_->txComBuf.addFastCom(GB_COM_SET_TIME);

	// сброс флага наличия ответа на команду установки времени в БСП
	sParam_->DateTimeReq.setTimeBsp_ = false;

	return true;
}

// Установка времени, сообщение об окончании.
bool TProtocolPcI::procSetTimeEnd() {
	if (!sParam_->DateTimeReq.setTimeBsp_)
		return false;

	// очистка метки времени
	uint8_t *ptr = (uint8_t *) &stTime;
	for(uint8_t i = 0; i < sizeof(SCp56Time2a); i++) {
		*ptr++ = 0;
	}

    stTime.years = sParam_->DateTimeReq.getYear();
    stTime.months = sParam_->DateTimeReq.getMonth();
    stTime.dayOfMonth = sParam_->DateTimeReq.getDay();
    stTime.hours = sParam_->DateTimeReq.getHour();
    stTime.minutes = sParam_->DateTimeReq.getMinute();
    stTime.milliseconds = 1000 * sParam_->DateTimeReq.getSecond();
    stTime.milliseconds +=	sParam_->DateTimeReq.getMsSecond();

	// сброс флага наличия принятых даты/времени в момент синхронизации БСП
	sParam_->DateTimeReq.setTimeBsp_ = false;

	return true;
}

// Возвращает состояние элемента информации.
bool TProtocolPcI::getValue(EInfoElement2 ei) const {
	bool val = false;

	if (ei >= IE2_DEF_ON) {
		val = getDef(ei);
	} else if (ei >= IE2_PRM_ON) {
		val = getPrm(ei);
	} else if (ei >= IE2_PRD_ON) {
		val = getPrd(ei);
	} else if (ei >= IE2_GLB_ERROR_H0001) {
		val = getGlb(ei);
	} else if (ei >= IE2_ERROR) {
		val = getDevice(ei);
	}

	return val;
}

// Возвращает состояние флага информации аппарата.
bool TProtocolPcI::getDevice(EInfoElement2 ei) const {
	bool val = false;

	if (ei == IE2_ERROR) {
		val = sParam_->glb.status.isFault();
		val |= sParam_->def.status.isEnable()&& sParam_->def.status.isFault();
		val |= sParam_->prm.status.isEnable()&& sParam_->prm.status.isFault();
		val |= sParam_->prd.status.isEnable()&& sParam_->prd.status.isFault();
	} else if (ei == IE2_WARNING) {
		val = sParam_->glb.status.isWarning();
		val |= sParam_->def.status.isEnable()&& sParam_->def.status.isWarning();
		val |= sParam_->prm.status.isEnable()&& sParam_->prm.status.isWarning();
		val |= sParam_->prd.status.isEnable()&& sParam_->prd.status.isWarning();
	} else if (ei == IE2_PRD_COM) {
		val = sParam_->prd.isIndCom();
	} else if (ei == IE2_PRM_COM) {
		val = sParam_->prm.isIndCom();
	} else if (ei == IE2_DI_PUSK_PRM) {
		val = sParam_->glb.getDInputPuskPrm();
	} else if (ei == IE2_DI_RESET_IND) {
		val = sParam_->glb.getDInputResetInd();
	} else if (ei == IE2_DI_TM_CONTROL) {
		val = sParam_->glb.getDInputTmControl();
	}

	return val;
}

// Возвращает состояние флага общей информации.
bool TProtocolPcI::getGlb(EInfoElement2 ei) const {
	bool val = false;

	if (ei >= IE2_GLB_WARNING_H0001) {
		if (ei <= IE2_GLB_WARNING_H8000) {
			val = sParam_->glb.status.isWarning(ei - IE2_GLB_WARNING_H0001);
		}
	} else if (ei >= IE2_GLB_ERROR_H0001) {
		if (ei <= IE2_GLB_ERROR_H8000) {
			val = sParam_->glb.status.isFault(ei - IE2_GLB_ERROR_H0001);
		}
	}

	return val;
}

// Возвращает состояние флага передатчика.
bool TProtocolPcI::getPrd(EInfoElement2 ei) const {
	bool val = false;

	if (ei == IE2_PRD_TEST) {
		val = (sParam_->prd.status.getRegime() == GB_REGIME_TEST_1) ||
				(sParam_->prd.status.getRegime() == GB_REGIME_TEST_2);
	} else if (ei == IE2_PRD_ENABLED) {
		val = (sParam_->prd.status.getRegime() == GB_REGIME_ENABLED);
	} else if (ei == IE2_PRD_DISABLED) {
		val = (sParam_->prd.status.getRegime() == GB_REGIME_DISABLED);
	} if ((ei >= IE2_PRD_COM_01) && (ei <= IE2_PRD_COM_32)) {
		// TODO сделать зависимость от количества команд
		val = sParam_->prd.getIndCom(ei - IE2_PRD_COM_01);
	} else if ((ei >= IE2_PRD_WARNING_H0001) && (ei <= IE2_PRD_WARNING_H8000)) {
		val = sParam_->prd.status.isWarning(ei - IE2_PRD_WARNING_H0001);
	} else if ((ei >= IE2_PRD_ERROR_H0001) && (ei <= IE2_PRD_ERROR_H8000)) {
		val = sParam_->prd.status.isFault(ei - IE2_PRD_ERROR_H0001);
	} else if (ei == IE2_PRD_ON) {
		val = sParam_->prd.status.isEnable();
	}

	return val;
}

// Возвращает состояние флага приемника.
bool TProtocolPcI::getPrm(EInfoElement2 ei) const {
	bool val = false;

	if (ei == IE2_PRM_TEST) {
		val = (sParam_->prm.status.getRegime() == GB_REGIME_TEST_1) ||
				(sParam_->prm.status.getRegime() == GB_REGIME_TEST_2);
	} else if (ei == IE2_PRM_ENABLED) {
		val = (sParam_->prm.status.getRegime() == GB_REGIME_ENABLED);
	} else if (ei == IE2_PRM_READY) {
		val = (sParam_->prm.status.getRegime() == GB_REGIME_READY);
	} else if (ei == IE2_PRM_DISABLED) {
		val = (sParam_->prm.status.getRegime() == GB_REGIME_DISABLED);
	} else if ((ei >= IE2_PRM_COM_01) && (ei <= IE2_PRM_COM_32)) {
		// TODO сделать зависимость от количества команд
		val = sParam_->prm.getIndCom(ei - IE2_PRM_COM_01);
	} else if ((ei >= IE2_PRM_WARNING_H0001) && (ei <= IE2_PRM_WARNING_H8000)) {
		val = sParam_->prm.status.isWarning(ei - IE2_PRM_WARNING_H0001);
	} else if ((ei >= IE2_PRM_ERROR_H0001) && (ei <= IE2_PRM_ERROR_H8000)) {
		val = sParam_->prm.status.isFault(ei - IE2_PRM_ERROR_H0001);
	} else if (ei == IE2_PRM_ON) {
		val = sParam_->prm.status.isEnable();
	}

	return val;
}

// Возвращает состояние флага защиты.
bool TProtocolPcI::getDef(EInfoElement2 ei) const {
	bool val = false;

	if ((ei >= IE2_DEF_WARNING_H0001) && (ei <= IE2_DEF_WARNING_H8000)) {
		val = sParam_->def.status.isWarning(ei - IE2_DEF_WARNING_H0001);
	} else if ((ei >= IE2_DEF_ERROR_H0001) && (ei <= IE2_DEF_ERROR_H8000)) {
		val = sParam_->def.status.isFault(ei - IE2_DEF_ERROR_H0001);
	} else if (ei == IE2_DEF_ON) {
		val = sParam_->def.status.isEnable();
	}

	return val;
}
