/*
 * bspParam.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMBSP_H_
#define PARAMBSP_H_

#include "src/glbDefine.h"
#include "src/paramDef.h"
#include "src/paramGlb.h"
#include "src/paramPrm.h"
#include "src/paramPrd.h"
#include "src/paramUart.h"
#include "src/menu/dateTime.hpp"
#include "src/menu/journalEntry.hpp"
#include "src/menu/jrnScada.hpp"
#include "src/menu/txCom.hpp"
#include "src/parameter/LocalParams.h"

/// Структура параметров БСП
struct stGBparam {
	// false - означает что надо настроить меню под текущий тип аппарата
	bool device;

    // Флаг наличия связи с БСП.
    bool connectionBsp;
    // Флаг наличия связи с ПК.
    bool connectionPc;

	// тип аппарата
	eGB_TYPE_DEVICE typeDevice;

	// тип оптического аппарата
	eGB_TYPE_OPTO typeOpto;

	// пароль
	TPassword password;

	// дата/время
    TDateTime DateTime;

	// дата/время БСП в момент установки новго времени
    TDateTime DateTimeReq;

	// дата/время

	// измеряемые параметры
	TMeasuredParameters measParam;

	// текущее состояние устройств
	TDeviceDef def;
	TDevicePrm prm;
	TDevicePrd prd;
	TDeviceGlb glb;

	// локальные параметры
	LocalParams local;

	// параметры для работы с последовательным портом
	TUartData Uart;

	// буфер команд
	TTxCom txComBuf;

	// запись в журнале
	TJournalEntry jrnEntry;

	// запись для SCADA
	TJrnSCADA jrnScada;

	// тесты
	TTest test;
};

#endif /* PARAMBSP_H_ */
