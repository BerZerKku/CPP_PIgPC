/*
 * protocolBspM.h
 *
 *  Created on: 05.09.2014
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLPCM_H_
#define PROTOCOLPCM_H_

#include "protocolModbus.h"
#include "glbDefine.h"
#include "paramBsp.h"

class TProtocolPcM : public TProtocolModbus
{
	/// Адреса регистров и флагов устройства
	enum ADR_REG{
		ADR_REG_MIN			= 0,
		// Дата и время
		ADR_YEAR 			= 1,	///< Год.
		ADR_MONTH,					///< Месяц.
		ADR_DAY,					///< День.
		ADR_HOUR,					///< Часы.
		ADR_MINUTE,					///< Минуты.
		ADR_SECOND,					///< Секунды.
		// Пароли
		ADR_PASSWORD 		= 7,	///< Текущий пароль.
		ADR_NEW_PASSWORD,			///< Новый пароль.
		// Текущее состояние
		ADR_GLB_FAULT		= 10,	///< Код неисправности Общий.
		ADR_GLB_WARNING,			///< Код предупреждения Общий.
		ADR_PRM_FAULT,				///< Код неисправности Приемника.
		ADR_PRM_WARNING,			///< Код предупреждения Приемника.
		ADR_PRM_REGIME,				///< Режим Приемника.
		ADR_PRM_STATE,				///< Состояние Приемника.
		ADR_PRM_DOP,				///< Дополнительный байт Приемника.
		ADR_PRD_FAULT,				///< Код неисправности Передатчика.
		ADR_PRD_WARNING,			///< Код предупреждения Передатчика.
		ADR_PRD_REGIME,				///< Режим Передатчика.
		ADR_PRD_STATE,				///< Состояние Передатчика.
		ADR_PRD_DOP,				///< Дополнительный байт Передатчика.
		ADR_DEF_FAULT,				///< Код неисправности Защиты.
		ADR_DEF_WARNING,			///< Код предупреждения Защиты.
		ADR_DEF_REGIME,				///< Режим Защиты.
		ADR_DEF_STATE,				///< Состояние Защиты.
		ADR_DEF_DOP,				///< Дополнительный байт Защиты.
		// Журнал событий
		ADR_JRN_EVT_CNT_PWR = 27,	///< Кол-во сделанных записей с момента включения.
		ADR_JRN_EVT_CNT,			///< Кол-во записей в журнале.
		ADR_JRN_EVT_NUM,			///< Номер текущей записи в журнале.
		ADR_JRN_EVT_REZ1,			///< Резерв.
		ADR_JRN_EVT_REZ2,			///< Резерв.
		ADR_JRN_EVT_DEV,			///< Имя устройства.
		ADR_JRN_EVT_TYPE,			///< Тип события.
		ADR_JRN_EVT_REG,			///< Режим работы.
		ADR_JRN_EVT_REZ3,			///< Резерв.
		ADR_JRN_EVT_REZ4,			///< Резерв.
		ADR_JRN_EVT_REZ5,			///< Резерв.
		ADR_JRN_EVT_REZ6,			///< Резерв.
		ADR_JRN_EVT_MSECOND,		///< Миллисекунды.
		ADR_JRN_EVT_SECOND,			///< Секунды.
		ADR_JRN_EVT_MINUTE,			///< Минуты.
		ADR_JRN_EVT_HOUR,			///< Час.
		ADR_JRN_EVT_WDAY,			///< День недели.
		ADR_JRN_EVT_DAY,			///< День.
		ADR_JRN_EVT_MONTH,			///< Месяц.
		ADR_JRN_EVT_YEAR,			///< Год.
		// Журнал приемника
		ADR_JRN_PRM_CNT_PWR = 50,	///< Кол-во сделанных записей с момента включения.
		ADR_JRN_PRM_CNT,			///< Кол-во записей в журнале.
		ADR_JRN_PRM_NUM,			///< Номер текущей записи в журнале.
		ADR_JRN_PRM_REZ1,			///< Резерв.
		ADR_JRN_PRM_REZ2,			///< Резерв.
		ADR_JRN_PRM_DEV,			///< Имя устройства.
		ADR_JRN_PRM_NUM_COM,		///< Номер команды.
		ADR_JRN_PRM_EVENT,			///< Событие.
		ADR_JRN_PRM_REZ3,			///< Резерв.
		ADR_JRN_PRM_REZ4,			///< Резерв.
		ADR_JRN_PRM_REZ5,			///< Резерв.
		ADR_JRN_PRM_REZ6,			///< Резерв.
		ADR_JRN_PRM_MSECOND,		///< Миллисекунды.
		ADR_JRN_PRM_SECOND,			///< Секунды.
		ADR_JRN_PRM_MINUTE,			///< Минуты.
		ADR_JRN_PRM_HOUR,			///< Час.
		ADR_JRN_PRM_WDAY,			///< День недели.
		ADR_JRN_PRM_DAY,			///< День.
		ADR_JRN_PRM_MONTH,			///< Месяц.
		ADR_JRN_PRM_YEAR,			///< Год.
		// Журнал передатчика
		ADR_JRN_PRD_CNT_PWR = 74,	///< Кол-во сделанных записей с момента включения.
		ADR_JRN_PRD_CNT,			///< Кол-во записей в журнале.
		ADR_JRN_PRD_NUM,			///< Номер текущей записи в журнале.
		ADR_JRN_PRD_REZ1,			///< Резерв.
		ADR_JRN_PRD_REZ2,			///< Резерв.
		ADR_JRN_PRD_DEV,			///< Имя устройства.
		ADR_JRN_PRD_NUM_COM,		///< Номер команды.
		ADR_JRN_PRD_EVENT,			///< Событие.
		ADR_JRN_PRD_SOURCE,			///< Источник команды.
		ADR_JRN_PRD_REZ4,			///< Резерв.
		ADR_JRN_PRD_REZ5,			///< Резерв.
		ADR_JRN_PRD_REZ6,			///< Резерв.
		ADR_JRN_PRD_MSECOND,		///< Миллисекунды.
		ADR_JRN_PRD_SECOND,			///< Секунды.
		ADR_JRN_PRD_MINUTE,			///< Минуты.
		ADR_JRN_PRD_HOUR,			///< Час.
		ADR_JRN_PRD_WDAY,			///< День недели.
		ADR_JRN_PRD_DAY,			///< День.
		ADR_JRN_PRD_MONTH,			///< Месяц.
		ADR_JRN_PRD_YEAR,			///< Год.
		// Журнал защиты
		ADR_JRN_DEF_CNT_PWR = 98,	///< Кол-во сделанных записей с момента включения.
		ADR_JRN_DEF_CNT,			///< Кол-во записей в журнале.
		ADR_JRN_DEF_NUM,			///< Номер текущей записи в журнале.
		ADR_JRN_DEF_REZ1,			///< Резерв.
		ADR_JRN_DEF_REZ2,			///< Резерв.
		ADR_JRN_DEF_DEV,			///< Имя устройства.
		ADR_JRN_DEF_TYPE,			///< Состояние сигналов.
		ADR_JRN_DEF_STATE,			///< Состояние.
		ADR_JRN_DEF_REZ3,			///< Резерв.
		ADR_JRN_DEF_REZ4,			///< Резерв.
		ADR_JRN_DEF_REZ5,			///< Резерв.
		ADR_JRN_DEF_REZ6,			///< Резерв.
		ADR_JRN_DEF_MSECOND,		///< Миллисекунды.
		ADR_JRN_DEF_SECOND,			///< Секунды.
		ADR_JRN_DEF_MINUTE,			///< Минуты.
		ADR_JRN_DEF_HOUR,			///< Час.
		ADR_JRN_DEF_WDAY,			///< День недели.
		ADR_JRN_DEF_DAY,			///< День.
		ADR_JRN_DEF_MONTH,			///< Месяц.
		ADR_JRN_DEF_YEAR,			///< Год.
		// Измрения
		ADR_MEAS_U_OUT 		= 123,	///< Выходное напряжение.
		ADR_MEAS_I_OUT,				///< Выходной ток.
		ADR_MEAS_UC1,				///< Запас по Uк (Uк1).
		ADR_MEAS_UC2,				///< Запас по Uк2.
		ADR_MEAS_UN1,				///< Уровень шумов Uш (Uш1).
		ADR_MEAS_UN2,				///< Уровень шумов Uш2.
		ADR_MEAS_UD1,				///< Запас по Uз (Uз1).
		ADR_MEAS_UD2,				///< Запас по Uз2.
		ADR_MEAS_PW,				///< Длительность импульсов ВЧ блокировки.
		// Индикация команд
		ADR_IND_COM_PRM_16	= 140,	///< Индикация команд приемника 1-16.
		ADR_IND_COM_PRM_32,			///< Индикация команд приемника 17-32.
		ADR_IND_COM_PRD_16,			///< Индикация команд передатчика 1-16.
		ADR_IND_COM_PRD_32,			///< Индикация команд передатчика 16-32.
		// Версии прошивок
		ADR_IC_BSP_MCU 		= 156,	///< Версия прошивки МК платы БСП.
		ADR_IC_BSP_DSP,				///< Версия прошивки DSP платы БСП.
		ADR_IC_PI_MCU,				///< Версия прошивки МК платы ПИ.
		ADR_IC_BSK_PRD1,			///< Версия прошивки ПЛИС ПРД блока БСК1.
		ADR_IC_BSK_PRM1,			///< Версия прошивки ПЛИС ПРМ блока БСК1.
		ADR_IC_BSK_PRD2,			///< Версия прошивки ПЛИС ПРД блока БСК2.
		ADR_IC_BSK_PRM2,			///< Версия прошивки ПЛИС ПРМ блока БСК2.
		ADR_IC_BSZ,					///< Версия прошивки ПЛИС блока БСЗ.
		// Максимальное кол-во используемых адресов регистров
		ADR_REG_MAX
	};

	enum ADR_COIL {
		ADR_C_MIN	 		= 200,
		// Флаги текущего состояния
		ADR_C_FAULT 		= 201,	///< Наличие неисправности.
		ADR_C_WARNING,				///< Наличие предупреждения.
		ADR_C_IND_PRD, 				///< Наличие индикации команд передатчика.
		ADR_C_IND_PRM,				///< Наличие индикации команд приемника.
		// Флаги неисправностей общих, начиная с кода 0х0001 по код 0x8000
		ADR_C_GLB_FAULT_1	= 301,	///< Неисправность общая с кодом 0х0001.
		ADR_C_GLB_FAULT_16	= 316,	///< Неисправность общая с кодом 0х8000.
		// Флаги предупреждений общих, начиная с кода 0х0001 по код 0x8000
		ADR_C_GLB_WARNING_1	= 317,	///< Предупреждение общее с кодом 0х0001.
		ADR_C_GLB_WARNING_16= 332,	///< Предупреждение общее с кодом 0х8000.

		// Передатчик
		ADR_C_PRD_IS		= 400,	///< Передатчик подключен.
		// Флаги неисправностей передатчика, начиная с кода 0х0001 по код 0x8000
		ADR_C_PRD_FAULT_1	= 401,	///< Неисправность передатчика с кодом 0x0001.
		ADR_C_PRD_FAULT_16	= 416,	///< Неисправность передатчика с кодом 0x8000.
		// Флаги предупреждений передатчика, начиная с кода 0х0001 по код 0x8000
		ADR_C_PRD_WARNING_1	= 417,	///< Предупреждение приемника с кодом 0х0001.
		ADR_C_PRD_WARNING_16= 432,	///< Предупреждение приемника с кодом 0х8000.
		// Индикация команд передатчика, начиная с 1 по 32
		ADR_C_PRD_IND_1		= 450,	///< Индикация 1 команды передатчика.
		ADR_C_PRD_IND_32	= 481,	///< Индикация 32 команды передатчика.

		// Приемника
		ADR_C_PRM_IS		= 500,	///< Приемник подключен.
		// Флаги неисправностей приемника, начиная с кода 0х0001 по код 0x8000
		ADR_C_PRM_FAULT_1	= 501,	///< Неисправность приемника с кодом 0x0001.
		ADR_C_PRM_FAULT_16	= 516,	///< Неисправность приемника с кодом 0x8000.
		// Флаги предупреждений приемника, начиная с кода 0х0001 по код 0x8000
		ADR_C_PRM_WARNING_1	= 517,	///< Предупреждение приемника с кодом 0х0001.
		ADR_C_PRM_WARNING_16= 532,	///< Предупреждение приемника с кодом 0х8000.
		// Индикация команд передатчика, начиная с 1 по 32
		ADR_C_PRM_IND_1		= 550,	///< Индикация 1 команды применика.
		ADR_C_PRM_IND_32	= 581,	///< Индикация 32 команды применика.

		// Защита
		ADR_C_DEF_IS		= 600,	///< Защита подключена.
		// Флаги неисправностей приемника, начиная с кода 0х0001 по код 0x8000
		ADR_C_DEF_FAULT_1	= 601,	///< Неисправность защиты с кодом 0x0001.
		ADR_C_DEF_FAULT_16	= 616,	///< Неисправность защиты с кодом 0x8000.
		// Флаги предупреждений приемника, начиная с кода 0х0001 по код 0x8000
		ADR_C_DEF_WARNING_1	= 617,	///< Предупреждение защиты с кодом 0х0001.
		ADR_C_DEF_WARNING_16= 632,	///< Предупреждение защиты с кодом 0x8000.
		// Максимальное кол-во используемых флагов
		ADR_C_MAX
	};


public:
	/**	Конструктор.
	 *
	 * 	@param sParam Указатель на структуру параметров.
	 * 	@param buf Указатель на буфер данных.
	 * 	@param size Размер буфера данных.
	 */
	TProtocolPcM(stGBparam *sParam, uint8_t *buf, uint8_t size);

	/**	Деструктор.
	 *
	 */
//	~TProtocolPcM() {};

	/**	Функция отправки сообщения.
	 *
	 * 	@return Кол-во байт данных для передачи.
	 */
	uint8_t send();

private:

	stGBparam * const sParam_;	///< Структура параметров

	// Чтение флагов.
	TProtocolModbus::CHECK_ERR readCoil(uint16_t adr, bool &val);

	// Запись флагов.
	TProtocolModbus::CHECK_ERR writeCoil(uint16_t adr, bool val);

	// Чтение регистров.
	TProtocolModbus::CHECK_ERR readRegister(uint16_t adr, uint16_t &val);

	// Запись регистров
	TProtocolModbus::CHECK_ERR writeRegister(uint16_t adr, uint16_t val);

	// Обработка принятой команды чтения ID.
	TProtocolModbus::CHECK_ERR readID(char *buf, uint8_t &size);

	// Считывание флагов общих
	bool readCoilGlb(uint16_t adr);

	// Считывание флагов защиты
	bool readCoilDef(uint16_t adr);

	// Считывание флагов приемника
	bool readCoilPrm(uint16_t adr);

	// Считывание флагов передатчика
	bool readCoilPrd(uint16_t adr);

	// Считывание из регистров даты и времени.
	uint16_t readRegDateTime(uint16_t adr);

	// Считывание из регистров состояния.
	uint16_t readRegState(uint16_t adr);

	// Cчитывание из регистров измеряемых параметров.
	uint16_t readRegMeasure(uint16_t adr);

	// Cчитывание из регистров версий прошивок.
	uint16_t readRegVersionIC(uint16_t adr);

	// Считывание журнала событий.
	uint16_t readJournalEvent(uint16_t adr);

	// Считывание журнала приемника.
	uint16_t readJournalPrm(uint16_t adr);

	// Считывание журнала передатчика.
	uint16_t readJournalPrd(uint16_t adr);

	// Считывание журнала защиты.
	uint16_t readJournalDef(uint16_t adr);

};


#endif /* PROTOCOLPM_H_ */
