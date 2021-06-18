/*
 * bspParam.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMBSP_H_
#define PARAMBSP_H_

#include "glbDefine.h"
#include "paramPrm.h"
#include "paramPrd.h"
#include "paramDef.h"
#include "paramGlb.h"
#include "paramUart.h"
#include "parameter/LocalParams.h"
#include "menu/measure.h"
#include "menu/password.h"
#include "menu/test.h"
#include "menu/txCom.h"


/// Структура параметров БСП
struct stGBparam {
	// false - означает что надо настроить меню под текущий тип аппарата
	bool device;

	bool connectionBsp;
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
	TMeasure measParam;

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
