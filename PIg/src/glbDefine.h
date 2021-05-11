/*
 * glb_def.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

/**
 * 	Максимальное значение для строковых параметров на 1 больше возможного,
 * 	для хранения в массиве ошибочного значения и удобства пользования
 * 	перечислениями.
 * 	Так что для ввода строкового значения максимум должен быть меньше на 1 !!
 */

#ifndef GLBDEF_H_
#define GLBDEF_H_

#include <stdint.h>
#include "avr.h"
#include "src/debug/debug.hpp"

/// размер массива
#define SIZE_OF(mas) (sizeof(mas) / sizeof(mas[0]))

/// пароль администратора
#define PASSWORD_ADMIN 6352

/// пароль пользователя по умолчанию
#define PASSWORD_USER 0

/// версия текущей прошивки
#define VERS 0x6401

/// максимально кол-во команд на прием (должно быть кратно 8)
#define MAX_NUM_COM_PRM 32

/// максимальное кол-во команд на передачу (должно быть кратно 8)
#define MAX_NUM_COM_PRD 32

/// максимальное кол-во транзитных команд (+1 - это 0 команда)
#define MAX_NUM_COM_RING 96

/// преобразование двух CHAR в INT
#define TO_INT16(high, low) (((uint16_t) (high) << 8) + (low))

/// максимально возможное кол-во состояний устройств
#define MAX_NUM_DEVICE_STATE 14

/// максимальное кол-во неисправностей для любого устройства
#define MAX_NUM_FAULTS 16

/// максимальное кол-во предупреждений для любого устройства
#define MAX_NUM_WARNINGS 16

/// минимальное кол-во команд передаваемых за одну секунду
#define MIN_NUM_COM_SEND_IN_1_SEK 8

///	максимальное кол-во быстрых команды
// на данный момент надо только для ТЕСТов РЗСК, т.к. там посылаются 2 команды
// а так же возможно появление быстрой команды при работе с АСУ ТП
#define MAX_NUM_FAST_COM 3

/// максимальное кол-во команд в первом буфере
#define MAX_NUM_COM_BUF1 10

/// максимальное кол-во команд во втором буфере
#define MAX_NUM_COM_BUF2 4

// не учитываем быстрые команды, т.к. они возникают достаточно редко
#if (MIN_NUM_COM_SEND_IN_1_SEK - MAX_NUM_COM_BUF2 < 4)
#error Слишком мало второстепенных команд отправляется в БСП за один период!!!
#endif

/// максимальное кол-во сигналов в тестах
#define MAX_NUM_TEST_SIGNAL 100

// длина половины строки (+1 - символ конца строки)
#define STRING_LENGHT (11 + 1)

// длина строки (+1 - символ конца строки)
#define NAME_PARAM_LENGHT (20 + 1)

/// максимальное и минимальный код типа событий в журнале событий
#define MIN_JRN_EVENT_VALUE 1
#define MAX_JRN_EVENT_VALUE 38

#define GLB_JRN_EVENT_K400_MAX 	512		/// кол-во записей в журнале событый К400
#define GLB_JRN_EVENT_R400M_MAX 512		/// кол-во записей в журнале событый Р400м
#define GLB_JRN_EVENT_RZSK_MAX 	256		/// кол-во записей в журнале событый РЗСК
#define GLB_JRN_EVENT_OPTO_MAX 	256		/// кол-во записей в журнале событый ОПТИКА

#define GLB_JRN_DEF_K400_MAX 	0		///< кол-во записей в журнале защиты К400
#define GLB_JRN_DEF_R400M_MAX 	1024	///< кол-во записей в журнале защиты Р400м
#define GLB_JRN_DEF_RZSK_MAX 	256		///< кол-во записей в журнале защиты РЗСК
#define GLB_JRN_DEF_OPTO_MAX 	2048	///< кол-во записей в журнале защиты ОПТИКА

#define GLB_JRN_PRM_K400_MAX 	512		///< кол-во записей в журнале приемника К400
#define GLB_JRN_PRM_RZSK_MAX 	256		///< кол-во записей в журнале приемника РЗСК
#define GLB_JRN_PRM_OPTO_MAX	256		///< кол-во записей в журнале приемника ОПТИКА

#define GLB_JRN_PRD_K400_MAX 	512		/// кол-во записей в журнале передатчика К400
#define GLB_JRN_PRD_RZSK_MAX 	256		/// кол-во записей в журнале передатчика РЗСК
#define GLB_JRN_PRD_OPTO_MAX	256		/// кол-во записей в журнале передатчика ОПТИКА

/// максимально возможное кол-во записей в одном архиве
#define GLB_JRN_MAX 2048

/// Максимальное знаачение доп.байта в архиве событий К400/РЗСК
#define GLB_JRN_DOP_MAX 6

/// время одно цикла работы с меню, мс
#define MENU_TIME_CYLCE 200

/// время вывода доп.сообщения на экран, мс
#define TIME_MESSAGE (3000 / MENU_TIME_CYLCE)

/// Режим работы функции ввода параметров
enum eMENU_ENTER_PARAM {
	MENU_ENTER_PARAM_NO,		///< отмена изменения параметра
	MENU_ENTER_PARAM_INT,		///< изменение параметра, целое значение
	MENU_ENTER_PARAM_LIST,		///< изменение параметра, выбор из списка
	MENU_ENTER_PARAM_LIST_2,	///< изменение параметра, выбор из списка значений
	MENU_ENTER_PARAM_U_COR,		///< изменение параметра, коррекция напряжения
	MENU_ENTER_PASSWORD,		///< ввод пароля
	MENU_ENTER_PASSWORD_NEW,	///< ввод нового пароля
	MENU_ENTER_PASSWORD_READY,	///< введен верный пароль
    MENU_ENTER_PASSWORD_N_READY,///< новый пароль введен корректно
	MENU_ENTER_PARAM_READY,		///< необходимо изменить параметр
	MENU_ENTER_PARAM_MESSAGE,	///< вывод сообщения на экран при вводе параметра
};

/// Тип аппарата
enum eGB_TYPE_DEVICE {
	AVANT_NO = 0,	// ошибочное значение
	AVANT_R400,		//
	AVANT_RZSK,		//
	AVANT_K400,		//
	AVANT_R400M,	//
	AVANT_OPTO,		//
	AVANT_MAX
};

/// Тип оптического аппарата
enum eGB_TYPE_OPTO {
	TYPE_OPTO_STANDART = 0,	// стандартная оптика
	TYPE_OPTO_RING_UNI,		// оптическое однонаправленное кольцо
	TYPE_OPTO_RING_BI,		// оптиеское двунаправленное кольцо
	TYPE_OPTO_MAX
};

/// Устройство
enum eGB_DEVICE {
	GB_DEVICE_MIN = 0,
	GB_DEVICE_DEF = 0,
	GB_DEVICE_PRM,
	GB_DEVICE_PRD,
	GB_DEVICE_GLB,
	GB_DEVICE_MAX
};

/// Устройство в журнале событий К400
enum eGB_DEVICE_K400 {
	GB_DEVICE_K400_MIN = 0,
	GB_DEVICE_K400_DEF = 0,
	GB_DEVICE_K400_PRM1,
	GB_DEVICE_K400_PRM2,
	GB_DEVICE_K400_PRD,
	GB_DEVICE_K400_GLB,
	GB_DEVICE_K400_PRM1_PRM2,
	GB_DEVICE_K400_MAX
};

/// Тип канала связи
enum eGB_TYPE_LINE {
	GB_TYPE_LINE_MIN = 1,
	GB_TYPE_LINE_UM = 1,
	GB_TYPE_LINE_OPTO = 2,
	GB_TYPE_LINE_E1 = 3,
	GB_TYPE_LINE_MAX
};

/// Тип параметра, в плане сохранения нового значения.
enum eGB_SEND_TYPE {
    GB_SEND_NO,             ///< Команды стандартного протокола нет
    GB_SEND_INT8,           ///< Передается один байт данных.
    GB_SEND_INT8_DOP,       ///< Передается два байта данных (значение, доп.байт).
    GB_SEND_DOP_INT8,       ///< Передается два байта данных (доп.байт, значение).
    GB_SEND_DOP_INT16_LE,   ///< Передается три байта данных (доп.байт, int16&0xFF, in16>>8).
    GB_SEND_INT16_BE,       ///< Передается два байта данных (in16>>8, int16&0xFF).
    GB_SEND_DOP_BITES,      ///< Передается битовая переменная (доп.байт, значение).
    GB_SEND_BITES_DOP,      ///< Передается битовая переменная (значение, доп.байт).
    GB_SEND_COR_U,          ///< Передается коррекция напряжения.
    GB_SEND_COR_I           ///< Передается коррекция тока.
};

/// Кол-во аппаратов в линии
// 	Учесть что 1 и 2 используются для параметров в 3-х концевой. Не менять !!!
enum eGB_NUM_DEVICES {
	GB_NUM_DEVICES_MIN = 1,
	GB_NUM_DEVICES_2 = 1,
	GB_NUM_DEVICES_3 = 2,
	GB_NUM_DEVICES_MAX
};

/// Режимы работы
enum eGB_REGIME {
	GB_REGIME_MIN = 0, 				//
	GB_REGIME_DISABLED = 0,			// выведен
	GB_REGIME_READY,				// готов
	GB_REGIME_ENABLED,				// введен
	GB_REGIME_TALK,					// речь
	GB_REGIME_TEST_1,				// тест ПРД
	GB_REGIME_TEST_2,				// тест ПРМ
	GB_REGIME_MAX,
};

// Режимы работы для ввода с клавиатуры
// порядок не совпадает с eGB_REGIME
enum eGB_REGIME_ENTER {
	GB_REGIME_ENTER_MIN = 0,
	GB_REGIME_ENTER_DISABLED = 0,
	GB_REGIME_ENTER_ENABLED = 1,
	GB_REGIME_ENTER_TEST_1,
	GB_REGIME_ENTER_TEST_2,
	GB_REGIME_ENTER_MAX
};

/// Команды
enum eGB_COM {
    // + означает что команда задокументирована
	GB_COM_NO = 0,
	GB_COM_DEF_GET_DEF_TYPE 	= 0x01,	// +
	GB_COM_DEF_GET_LINE_TYPE 	= 0x02,	// +
	GB_COM_DEF_GET_T_NO_MAN 	= 0x03,	// +
	GB_COM_DEF_GET_DELAY 		= 0x04,	// +
	GB_COM_DEF_GET_OVERLAP 		= 0x05,	// +
	GB_COM_DEF_GET_RZ_DEC 		= 0x06,	// +
	GB_COM_DEF_GET_PRM_TYPE 	= 0x07,	// + ! в Р400М это снижение уровня АК
	GB_COM_DEF_GET_FREQ_PRD 	= 0x08,	// +
	GB_COM_DEF_GET_RZ_THRESH 	= 0x09,	// + ! в Р400М это частота ПРМ
	GB_COM_DEF_GET_TYPE_AC 		= 0x0A,	// +
	GB_COM_DEF_GET_LIMIT_PRD	= 0x0B,	// +
	GB_COM_PRM_GET_TIME_ON 		= 0x11,	// +
	GB_COM_PRM_GET_TEST_COM		= 0x12,	// +
	GB_COM_PRM_GET_TIME_OFF 	= 0x13,	// +
	GB_COM_PRM_GET_BLOCK_COM 	= 0x14,	// +
	GB_COM_PRM_GET_FREQ_CORR	= 0x15,	// +
	GB_COM_PRM_GET_INC_SAFETY	= 0x16,	// +
	GB_COM_PRM_GET_DR_STATE 	= 0x17, // +
	GB_COM_PRM_GET_DR_BLOCK		= 0x18,	// +
	GB_COM_PRM_GET_DR_COM		= 0x19,	// +
	GB_COM_PRM_GET_RING_COM_REC	= 0x1B,	// +
	GB_COM_PRM_GET_COM			= 0x1C, // +
	GB_COM_PRM_GET_COM_SIGN		= 0x1D,	// +
    GB_COM_PRM_GET_BLOCK_ALL    = 0x1E, //
	GB_COM_PRD_GET_TIME_ON 		= 0x21,	// +
	GB_COM_PRD_GET_DURATION 	= 0x22,	// +
	GB_COM_PRD_GET_FREQ_CORR	= 0x23,	// +
	GB_COM_PRD_GET_BLOCK_COM 	= 0x24,	// +
	GB_COM_PRD_GET_LONG_COM 	= 0x25,	// +
	GB_COM_PRD_GET_TEST_COM 	= 0x26,	// +
	GB_COM_PRD_GET_DR_STATE 	= 0x27, // +
	GB_COM_PRD_GET_DR_BLOCK		= 0x28,	// +
	GB_COM_PRD_GET_COM_A		= 0x29,	// +
	GB_COM_PRD_GET_RING_COM_TR	= 0x2B,	// +
	GB_COM_PRD_GET_COM			= 0x2C,	// +
	GB_COM_PRD_GET_CF_TM		= 0x2D,	// +
	GB_COM_PRD_GET_COM_SIGN		= 0x2E, // +
	GB_COM_GET_SOST 			= 0x30,	// +
	GB_COM_GET_FAULT 			= 0x31,	// +
	GB_COM_GET_TIME 			= 0x32,	// +
	GB_COM_GET_COR_U_I 			= 0x33,	// + ! в ОПТИКЕ это Резервирование
	GB_COM_GET_MEAS 			= 0x34,	// +
	GB_COM_GET_TIME_SINCHR 		= 0x35,	// +
	GB_COM_GET_COM_PRM_KEEP 	= 0x36, // + ! в Р400М это Uвых номинальное
	GB_COM_GET_COM_PRD_KEEP 	= 0x37,	// + ! дополнительно тип удаленного аппарата, телемеханика, предупреждение и авария по D в К400 и т.д.
	GB_COM_GET_NET_ADR 			= 0x38,	// +
	GB_COM_GET_TIME_RERUN 		= 0x39,	// + ! в Р400М это параметры для совместимостей
	GB_COM_GET_FREQ 			= 0x3A,	// +
	GB_COM_GET_DEVICE_NUM 		= 0x3B,	// +
	GB_COM_GET_CF_THRESHOLD 	= 0x3C,	// + ! порог предупреждения и загрубления
	GB_COM_GET_OUT_CHECK 		= 0x3D,	// +
	GB_COM_GET_TEST 			= 0x3E,	// +
	GB_COM_GET_VERS 			= 0x3F,	// +
	GB_COM_PRM_ENTER 			= 0x51,	// +
	GB_COM_SET_REG_DISABLED 	= 0x70,	// +
	GB_COM_SET_REG_ENABLED 		= 0x71,	// +
	GB_COM_SET_CONTROL 			= 0x72,	// +
	GB_COM_SET_PASSWORD 		= 0x73,	// + ! только с ПК
	GB_COM_GET_PASSWORD 		= 0x74,	// + ! только с ПК
	GB_COM_SET_REG_TEST_2 		= 0x7D,	// +
	GB_COM_SET_REG_TEST_1 		= 0x7E,	// +
	GB_COM_DEF_SET_DEF_TYPE 	= 0x81,	// +
	GB_COM_DEF_SET_LINE_TYPE 	= 0x82,	// +
	GB_COM_DEF_SET_T_NO_MAN 	= 0x83,	// +
	GB_COM_DEF_SET_DELAY 		= 0x84,	// +
	GB_COM_DEF_SET_OVERLAP 		= 0x85,	// +
	GB_COM_DEF_SET_RZ_DEC 		= 0x86,	// +
	GB_COM_DEF_SET_PRM_TYPE 	= 0x87,	// + ! в Р400М это снижение уровня АК
	GB_COM_DEF_SET_FREQ_PRD 	= 0x88,	// +
	GB_COM_DEF_SET_RZ_THRESH 	= 0x89,	// + ! в Р400М это частота ПРМ
	GB_COM_DEF_SET_TYPE_AC 		= 0x8A,	// +
	GB_COM_DEF_SET_LIMIT_PRD	= 0x8B,	//
	GB_COM_PRM_SET_TIME_ON 		= 0x91,	// +
	GB_COM_PRM_SET_TEST_COM		= 0x92,	// +
	GB_COM_PRM_SET_TIME_OFF 	= 0x93,	// +
	GB_COM_PRM_SET_BLOCK_COM 	= 0x94,	// +
	GB_COM_PRM_SET_FREQ_CORR	= 0x95,	// +
	GB_COM_PRM_SET_INC_SAFETY	= 0x16,	// +
	GB_COM_PRM_SET_DR_STATE 	= 0x97, // +
	GB_COM_PRM_SET_DR_BLOCK		= 0x98,	// +
	GB_COM_PRM_SET_DR_COM		= 0x99,	// +
	GB_COM_PRM_RES_IND			= 0x9A,	// +
	GB_COM_PRM_SET_RING_COM_REC	= 0x9B,	// +
	GB_COM_PRM_SET_COM			= 0x9C,	// +
	GB_COM_PRM_SET_COM_SIGN		= 0x9D,	// +
    GB_COM_PRM_SET_BLOCK_ALL    = 0x9E, //
	GB_COM_PRD_SET_TIME_ON 		= 0xA1,	// +
	GB_COM_PRD_SET_DURATION 	= 0xA2,	// +
	GB_COM_PRD_SET_FREQ_CORR	= 0xA3,	// +
	GB_COM_PRD_SET_BLOCK_COM 	= 0xA4,	// +
	GB_COM_PRD_SET_LONG_COM 	= 0xA5,	// +
	GB_COM_PRD_SET_TEST_COM 	= 0xA6,	// +
	GB_COM_PRD_SET_DR_STATE 	= 0xA7, // +
	GB_COM_PRD_SET_DR_BLOCK		= 0xA8,	// +
	GB_COM_PRD_SET_COM_A		= 0xA9,	// +
	GB_COM_PRD_RES_IND 			= 0xAA,	// +
	GB_COM_PRD_SET_RING_COM_TR	= 0xAB,	// +
	GB_COM_PRD_SET_COM			= 0xAC,	// +
	GB_COM_PRD_SET_CF_TM		= 0xAD,	// +
	GB_COM_PRD_SET_COM_SIGN		= 0xAE, // +
	GB_COM_SET_TIME 			= 0xB2,	// +
	GB_COM_SET_COR_U_I 			= 0xB3,	// +
	GB_COM_SET_TIME_SINCHR 		= 0xB5,	// +
	GB_COM_SET_COM_PRM_KEEP 	= 0xB6, // + ! в Р400М это Uвых номинальное
	GB_COM_SET_COM_PRD_KEEP 	= 0xB7, // + ! дополнительно тип удаленного аппарата, телемеханика
	GB_COM_SET_NET_ADR 			= 0xB8,	// +
	GB_COM_SET_TIME_RERUN 		= 0xB9,	// +
	GB_COM_SET_FREQ 			= 0xBA,	// +
	GB_COM_SET_DEVICE_NUM 		= 0xBB,	// +
	GB_COM_SET_CF_THRESHOLD 	= 0xBC,	// +
	GB_COM_SET_OUT_CHECK 		= 0xBD,	// +
	GB_COM_DEF_GET_JRN_CNT 		= 0xC1,	// +
	GB_COM_DEF_GET_JRN_ENTRY 	= 0xC2,	// +
	GB_COM_DEF_JRN_CLR 			= 0xCA,	// ! стирание журнала ЗАЩ, только с ПК
	GB_COM_PRM_GET_JRN_CNT 		= 0xD1,	// +
	GB_COM_PRM_GET_JRN_ENTRY 	= 0xD2,	// +
	GB_COM_PRM_JRN_CLR 			= 0xDA,	// ! стирание журнала ПРМ, только с ПК
	GB_COM_PRD_GET_JRN_CNT 		= 0xE1,	// +
	GB_COM_PRD_GET_JRN_ENTRY 	= 0xE2,	// +
	GB_COM_PRD_JRN_CLR 			= 0xEA,	// ! стирание журнала ПРД, только с ПК
	GB_COM_GET_JRN_CNT 			= 0xF1,	// +
	GB_COM_GET_JRN_ENTRY 		= 0xF2,	// +
	GB_COM_JRN_CLR 				= 0xFA	// ! стирание журнала событий, только с ПК
};

/// Маски команд
enum eGB_COM_MASK {
	// Тип устройства
	GB_COM_MASK_DEVICE = 0x30,
	GB_COM_MASK_DEVICE_DEF = 0x00,
	GB_COM_MASK_DEVICE_PRM = 0x10,
	GB_COM_MASK_DEVICE_PRD = 0x20,
	GB_COM_MASK_DEVICE_GLB = 0x30,

	// группа команды
	GB_COM_MASK_GROUP = 0xC0,
	GB_COM_MASK_GROUP_READ_PARAM = 0x00,
	GB_COM_MASK_GROUP_WRITE_REGIME = 0x40,
	GB_COM_MASK_GROUP_WRITE_PARAM = 0x80,
	GB_COM_MASK_GROUP_READ_JOURNAL = 0xC0
};

/// Последовательность параметров в команде Сетевой адрес (0х38)
enum posComNetAdr_t {
    POS_COM_NET_ADR_netAdr = 1,
    POS_COM_NET_ADR_protocol,
    POS_COM_NET_ADR_baudrate,
    POS_COM_NET_ADR_dataBits,
    POS_COM_NET_ADR_parity,
    POS_COM_NET_ADR_stopBits,
    POS_COM_NET_ADR_password,
    POS_COM_NET_ADR_vpSac2 = POS_COM_NET_ADR_password + 2,
    POS_COM_NET_ADR_vpSam
};

enum posComPrdKeep_t {
    POS_COM_PRD_KEEP_prdKeep = 1,
    POS_COM_PRD_KEEP_compK400,
    POS_COM_PRD_KEEP_tmK400,
    POS_COM_PRD_KEEP_warnD,
    POS_COM_PRD_KEEP_alarmD,
    POS_COM_PRD_KEEP_tempMonitor,
    POS_COM_PRD_KEEP_tempThrHi,
    POS_COM_PRD_KEEP_tempThrLow,
    POS_COM_PRD_KEEP_tmSpeed,
    POS_COM_PRD_KEEP_ringTimeWait,
    POS_COM_PRD_KEEP_ringComTransit,
    POS_COM_PRD_KEEP_alarmResetMode = POS_COM_PRD_KEEP_ringComTransit + (MAX_NUM_COM_RING / 8)
};

enum posComPrmBlockAll_t {
    POS_COM_PRM_BLOCK_ALL_vpSac1 = 1
};

/// Параметры (связаны с fParams)
typedef enum {
	GB_PARAM_NULL_PARAM = 0,	///< параметр заглушка
	// общие параметры
	GB_PARAM_TIME_SYNCH,		///< синхронизация часов
	GB_PARAM_NUM_OF_DEVICE,		///< номер аппарата
	GB_PARAM_NUM_OF_DEVICE_RING,///< номер аппарата в оптическом кольце
	GB_PARAM_OUT_CHECK,			///< контроль выходного сигнала
	GB_PARAM_WARN_THD, 			///< порог предупреждения
	GB_PARAM_WARN_THD_CF,		///< порог предупреждения по КЧ (для РЗСК)
	GB_PARAM_TIME_RERUN,		///< время перезапуска
	GB_PARAM_COM_PRD_KEEP,		///< удержание реле команд ПРД
	GB_PARAM_COM_PRM_KEEP,		///< удержание реле команд ПРМ
	GB_PARAM_IN_DEC,			///< загрубление чувствительности ПРМ
	GB_PARAM_NET_ADDRESS,		///< адрес в локальной сети
	GB_PARAM_U_OUT_NOM,			///< номинальноые выходное напряжение
	GB_PARAM_FREQ,				///< частота
	GB_PARAM_COMP_P400,			///< совместимость (Р400, Р400м)
	GB_PARAM_IN_DEC_AC_ANSWER,	///< снижение ответа АК (ПВЗЛ)
	GB_PARAM_DETECTOR,			///< тип детектора
	GB_PARAM_COR_U,				///< коррекция напряжения
	GB_PARAM_COR_I,				///< коррекция тока
	GB_PARAM_PVZUE_PROTOCOL,	///< протокол обмена (ПВЗУ-Е)
	GB_PARAM_PVZUE_PARITY,		///< признак четности (ПВЗУ-Е)
	GB_PARAM_PVZUE_FAIL,		///< допустимые провалы (ПВЗУ-Е)
	GB_PARAM_PVZUE_NOISE_THD,	///< порог по помехе (ПВЗУ-Е)
	GB_PARAM_PVZUE_NOISE_LVL,	///< допустимая помеха (ПВЗУ-Е)
	GB_PARAM_PVZUE_AC_TYPE,		///< тип автоконтроля (ПВЗУ-Е)
	GB_PARAM_PVZUE_AC_PERIOD,	///< период беглого режима АК (ПВЗУ-Е)
	GB_PARAM_PVZUE_AC_PER_RE,	///< период повтора беглого режима АК (ПЗВУ-Е)
	GB_PARAM_BACKUP,			///< резервирование
	GB_PARAM_COMP_K400,			///< совместимость К400
	GB_PARAM_NUM_OF_DEVICES,	///< тип линии (кол-во аппаратов в линии)
	GB_PARAM_TM_K400,			///< телемеханика
	GB_PARAM_WARN_D,			///< уровень срабатывания предупредительной сигнализации по D
	GB_PARAM_ALARM_D,			///< уровень срабатывания аварийной сигнализации по D
	GB_PARAM_TEMP_MONITOR,		///< контроль температуры
	GB_PARAM_TEMP_THR_HI,		///< верхнее значение температуры
	GB_PARAM_TEMP_THR_LOW,		///< нижнее значение температуры
	GB_PARAM_TM_SPEED,			///< скорость ТМ
    GB_PARAM_ALARM_RESET_MODE,  ///< режим работы сигнализации
	// параметры защиты
	GB_PARAM_DEF_TYPE,			///< тип защиты
	GB_PARAM_TIME_NO_MAN,		///< дополнительное время без манипуляции
	GB_PARAM_OVERLAP,			///< перекрытие импульсов (кроме Р400/Р400м)
	GB_PARAM_OVERLAP_P400,		///< перекрытие импульсов (Р400/Р400м)
	GB_PARAM_OVERLAP_OPTO,		///< перекрытие импульсов (Р400 Оптика)
	GB_PARAM_DELAY,				///< компенсация задержки
	GB_PARAM_DELAY_OPTO,		///< компенсация задержки
	GB_PARAM_WARN_THD_RZ,		///< порог предупреждения по РЗ (для РЗСК)
	GB_PARAM_SENS_DEC,			///< загрубленеи чувствительности
	GB_PARAM_SENS_DEC_RZ,		///< загрубленеи чувствительности по РЗ
	GB_PARAM_PRM_TYPE,			///< тип приемника
	GB_PARAM_AC_IN_DEC,			///< снижение уровня АК
	GB_PARAM_FREQ_PRD,			///< частота ПРД
	GB_PARAM_FREQ_PRM,			///< частота ПРМ
	GB_PARAM_SHIFT_FRONT,		///< сдвиг переднего фронта ПРД
	GB_PARAM_SHIFT_BACK,		///< сдвиг заднего фронта ПРД
	GB_PARAM_SHIFT_PRM,			///< сдвиг ПРМ
	GB_PARAM_SHIFT_PRD,			///< сдвиг ВЧ ПРД от ПУСК
	GB_PARAM_LIMIT_PRD,			///< ограничение полосы передатчика
	GB_PARAM_DELAY_OFF_PRM,		///< задержка выключения ПРМ
	GB_PARAM_DELAY_ON_PRM,		///< задержка включения ПРМ
	GB_PARAM_DELAY_ON_PRD,		///< задержка включения ПРД
	GB_PARAM_MIN_TIME_PRD,		///< минимальная длительность ПРД
	// параметры передатчика
	GB_PARAM_PRD_IN_DELAY,		///< время включения (задержка срабатывания дискретного входа)
	GB_PARAM_PRD_DURATION_L,	///< длительность команды ВЧ
	GB_PARAM_PRD_DURATION_O,	///< длительность команды ОПТИКА
	GB_PARAM_PRD_TEST_COM,		///< тестовая команда
	GB_PARAM_PRD_COM_LONG,		///< следящие команды
	GB_PARAM_PRD_COM_BLOCK,		///< блокированные команды
	GB_PARAM_PRD_DR_ENABLE,		///< трансляция ЦП
	GB_PARAM_PRD_DR_COM_BLOCK,	///< блокированные команды ЦП
	GB_PARAM_PRD_COM_NUMS,		///< количество команд
	GB_PARAM_PRD_COM_NUMS_A,	///< количество команд группы А
	GB_PARAM_PRD_FREQ_CORR,		///< коррекция частоты ПРД
	GB_PARAM_PRD_DEC_CF,		///< снижение уровня КС
	GB_PARAM_PRD_DEC_TM,		///< снижение уровня ТМ
	GB_PARAM_PRD_DEFAULT_CF,	///< КС по умолчанию
	GB_PARAM_PRD_COM_SIGNAL,	///< сигнализация команд ПРД
	// параметры приемника
	GB_PARAM_PRM_TIME_ON,		///< задержка на фиксацию команды (время включения)
	GB_PARAM_PRM_TIME_ON_K400,	///< задержка на фиксацию команды (время включения) в К400
    GB_PARAM_PRM_COM_BLOCK,		///< блокированные команды
	GB_PARAM_PRM_TIME_OFF,		///< задержка на выключение
	GB_PARAM_PRM_DR_ENABLE,		///< трансляция ЦП
	GB_PARAM_PRM_DR_COM_BLOCK,	///< блокированные команды ЦП
	GB_PARAM_PRM_DR_COM_TO_HF,	///< команда ВЧ в ЦП
	GB_PARAM_PRM_COM_NUMS,		///< количество команд
	GB_PARAM_PRM_TEST_COM,		///< прием тестовой команды
	GB_PARAM_PRM_FREQ_CORR,		///< коррекция частоты ПРМ
	GB_PARAM_PRM_COM_SIGNAL,	///< сигнализация команд ПРМ
	GB_PARAM_PRM_INC_SAFETY,	///< Повышение безопасности
	// параметры интерфейса
	GB_PARAM_INTF_PROTOCOL,		///< протокол
	GB_PARAM_INTF_BAUDRATE,		///< скорость передачи
	GB_PARAM_INTF_DATA_BITS,	///< биты данных
	GB_PARAM_INTF_PARITY,		///< четность
	GB_PARAM_INTF_STOP_BITS,	///< стоповые биты
    GB_PARAM_INTF_INTERFACE,	///< интерфейс
	// параметры кольца
	GB_PARAM_RING_TIME_WAIT,	///< время ожидания команд
	GB_PARAM_RING_COM_TRANSIT,	///< транзитные команды
	GB_PARAM_RING_COM_REC,		///< переназначение команд приемника
    GB_PARAM_RING_COM_TR,		///< переназначение команд дискретных входов
    // виртуальная панель
    GB_PARAM_VP_SAC1,           ///< Вывод ПРМ (SAC1)
    GB_PARAM_VP_SAC2,           ///< Управление (SAC2)
    GB_PARAM_VP_SAm,            ///< Управления направлением одного выходв (SAnn.x / SA.m)
    // другое
    GB_PARAM_USER_PASSWORD,     ///< Пароль пользователя.
    //
    GB_PARAM_MAX
} eGB_PARAM;

/// Значения команд управления
enum eGB_CONTROL {
	GB_CONTROL_RESET_SELF 	= 1,	// сброс своего
	GB_CONTROL_RESET_UD 	= 2,	// сброс удаленного (-ых)
	GB_CONTROL_RESET_AC		= 3,	// сброс АК
	GB_CONTROL_PUSK_UD_1 	= 4,	// пуск удаленного (1)
	GB_CONTROL_PUSK_UD_2 	= 5,	// пуск удаленного 2
	GB_CONTROL_PUSK_UD_ALL 	= 6,	// пуск удаленных
	GB_CONTROL_CALL 		= 7,	// вызов
	GB_CONTROL_PUSK_ON 		= 8,	// пуск наладочный вкл.
	GB_CONTROL_PUSK_OFF 	= 9,	// пуск наладочный выкл.
	GB_CONTROL_PUSK_AC_UD 	= 10,	// пуск АК удаленный
	GB_CONTROL_RESET_UD_1	= 11,	// сброс удаленного 1
	GB_CONTROL_RESET_UD_2	= 12,	// сброс удаленного 2
	GB_CONTROL_PUSK_UD_3 	= 13,	// пуск удаленного 3
	GB_CONTROL_REG_AC 		= 14,	// режим АК
	GB_CONTROL_RESET_UD_3	= 15,	// сброс удаленного 3
	GB_CONTROL_MAN_1 		= 16,	// пуск МАН удаленного (1)
	GB_CONTROL_MAN_2 		= 17,	// пуск МАН удаленного 2
	GB_CONTROL_MAN_3 		= 18,	// пуск МАН удаленного 3
	GB_CONTROL_MAN_ALL 		= 19,	// пуск МАН удаленных
	GB_CONTROL_MAX					//
};

/// События журнала передатчика/приемника - конец и начало команды
enum eGB_STATE_COM {
	GB_STATE_COM_MIN 	= 0,		///< Минимальное значение
	GB_STATE_COM_END 	= 0,		///< Окончание команды
	GB_STATE_COM_START,				///< Начало команды
	GB_STATE_COM_MAX				///< Максимальное значение
};

/// Источник передаваемой команды
enum eGB_SOURCE_COM {
	GB_SOURCE_COM_DI 	= 0, 		///< Команда поступила с дискретного входа
	GB_SOURCE_COM_DR	= 1,		///< Команда поступила с цифрового стыка
	GB_SOURCE_COM_MAX				///< Ошибочное значение
};

/// сигналы в тест1 и тест2
enum eGB_TEST_SIGNAL {
	GB_SIGNAL_OFF = 0, 				//
	GB_SIGNAL_CF,					// РЗСК / Р400М
	GB_SIGNAL_CF1,
	GB_SIGNAL_CF2,
	GB_SIGNAL_CF3,
	GB_SIGNAL_CF4,
	GB_SIGNAL_CF_NO_RZ,				// РЗСК
	GB_SIGNAL_CF_RZ,				// РЗСК
	GB_SIGNAL_CF2_NO_RZ,			// РЗСК
	GB_SIGNAL_CF2_RZ,				// РЗСК
	GB_SIGNAL_RZ,
	GB_SIGNAL_COM1_NO_RZ,			// РЗСК
	GB_SIGNAL_COM2_NO_RZ,			// РЗСК
	GB_SIGNAL_COM3_NO_RZ,			// РЗСК
	GB_SIGNAL_COM4_NO_RZ,			// РЗСК
	GB_SIGNAL_COM1_RZ,				// РЗСК
	GB_SIGNAL_COM2_RZ,				// РЗСК
	GB_SIGNAL_COM3_RZ,				// РЗСК
	GB_SIGNAL_COM4_RZ,				// РЗСК
	GB_SIGNAL_COM1,					// vvvvvvvvvvvvvvvvvvvvvvvvvv
	GB_SIGNAL_COM2,					// сигналы команд должны идти
	GB_SIGNAL_COM3,					// подряд для заполнения К400
	GB_SIGNAL_COM4,
	GB_SIGNAL_COM5,
	GB_SIGNAL_COM6,
	GB_SIGNAL_COM7,
	GB_SIGNAL_COM8,
	GB_SIGNAL_COM9,
	GB_SIGNAL_COM10,
	GB_SIGNAL_COM11,
	GB_SIGNAL_COM12,
	GB_SIGNAL_COM13,
	GB_SIGNAL_COM14,
	GB_SIGNAL_COM15,
	GB_SIGNAL_COM16,
	GB_SIGNAL_COM17,
	GB_SIGNAL_COM18,
	GB_SIGNAL_COM19,
	GB_SIGNAL_COM20,
	GB_SIGNAL_COM21,
	GB_SIGNAL_COM22,
	GB_SIGNAL_COM23,
	GB_SIGNAL_COM24,
	GB_SIGNAL_COM25,
	GB_SIGNAL_COM26,
	GB_SIGNAL_COM27,
	GB_SIGNAL_COM28,
	GB_SIGNAL_COM29,
	GB_SIGNAL_COM30,				// сигналы команд должны идти
	GB_SIGNAL_COM31,				// подряд для заполнения К400
	GB_SIGNAL_COM32,				// ^^^^^^^^^^^^^^^^^^^^^^^^^^
	GB_SIGNAL_CF_RZ_R400M,
	GB_SIGNAL_CF1_RZ_R400M,
	GB_SIGNAL_CF2_RZ_R400M,
	GB_SIGNAL_CF3_RZ_R400M,
	GB_SIGNAL_CF4_RZ_R400M,
	GB_SIGNAL_CS,					// в оптике вместо КЧ
	GB_SIGNAL_COM1A,				// оптика однонаправленная vvv
	GB_SIGNAL_COM2A,
	GB_SIGNAL_COM3A,
	GB_SIGNAL_COM4A,
	GB_SIGNAL_COM5A,
	GB_SIGNAL_COM6A,
	GB_SIGNAL_COM7A,
	GB_SIGNAL_COM8A,
	GB_SIGNAL_COM9A,
	GB_SIGNAL_COM10A,
	GB_SIGNAL_COM11A,
	GB_SIGNAL_COM12A,
	GB_SIGNAL_COM13A,
	GB_SIGNAL_COM14A,
	GB_SIGNAL_COM15A,
	GB_SIGNAL_COM16A,
	GB_SIGNAL_COM17A,
	GB_SIGNAL_COM18A,
	GB_SIGNAL_COM19A,
	GB_SIGNAL_COM20A,
	GB_SIGNAL_COM21A,
	GB_SIGNAL_COM22A,
	GB_SIGNAL_COM23A,
	GB_SIGNAL_COM24A,
	GB_SIGNAL_COM25A,
	GB_SIGNAL_COM26A,
	GB_SIGNAL_COM27A,
	GB_SIGNAL_COM28A,
	GB_SIGNAL_COM29A,
	GB_SIGNAL_COM30A,
	GB_SIGNAL_COM31A,
	GB_SIGNAL_COM32A,
	GB_SIGNAL_COM1B,
	GB_SIGNAL_COM2B,
	GB_SIGNAL_COM3B,
	GB_SIGNAL_COM4B,
	GB_SIGNAL_COM5B,
	GB_SIGNAL_COM6B,
	GB_SIGNAL_COM7B,
	GB_SIGNAL_COM8B,
	GB_SIGNAL_COM9B,
	GB_SIGNAL_COM10B,
	GB_SIGNAL_COM11B,
	GB_SIGNAL_COM12B,
	GB_SIGNAL_COM13B,
	GB_SIGNAL_COM14B,
	GB_SIGNAL_COM15B,
	GB_SIGNAL_COM16B,
	GB_SIGNAL_COM17B,
	GB_SIGNAL_COM18B,
	GB_SIGNAL_COM19B,
	GB_SIGNAL_COM20B,
	GB_SIGNAL_COM21B,
	GB_SIGNAL_COM22B,
	GB_SIGNAL_COM23B,
	GB_SIGNAL_COM24B,
	GB_SIGNAL_COM25B,
	GB_SIGNAL_COM26B,
	GB_SIGNAL_COM27B,
	GB_SIGNAL_COM28B,
	GB_SIGNAL_COM29B,
	GB_SIGNAL_COM30B,
	GB_SIGNAL_COM31B,
	GB_SIGNAL_COM32B,
	GB_SIGNAL_COM1C,
	GB_SIGNAL_COM2C,
	GB_SIGNAL_COM3C,
	GB_SIGNAL_COM4C,
	GB_SIGNAL_COM5C,
	GB_SIGNAL_COM6C,
	GB_SIGNAL_COM7C,
	GB_SIGNAL_COM8C,
	GB_SIGNAL_COM9C,
	GB_SIGNAL_COM10C,
	GB_SIGNAL_COM11C,
	GB_SIGNAL_COM12C,
	GB_SIGNAL_COM13C,
	GB_SIGNAL_COM14C,
	GB_SIGNAL_COM15C,
	GB_SIGNAL_COM16C,
	GB_SIGNAL_COM17C,
	GB_SIGNAL_COM18C,
	GB_SIGNAL_COM19C,
	GB_SIGNAL_COM20C,
	GB_SIGNAL_COM21C,
	GB_SIGNAL_COM22C,
	GB_SIGNAL_COM23C,
	GB_SIGNAL_COM24C,
	GB_SIGNAL_COM25C,
	GB_SIGNAL_COM26C,
	GB_SIGNAL_COM27C,
	GB_SIGNAL_COM28C,
	GB_SIGNAL_COM29C,
	GB_SIGNAL_COM30C,
	GB_SIGNAL_COM31C,
	GB_SIGNAL_COM32C,	// оптика однонаправленная ^^^
	GB_SIGNAL_MAX
};



/// Состояние обработки принятого значения параметра.
enum eGB_ACT {
	GB_ACT_NO 		= 0,		// значение по умолчанию
	GB_ACT_OLD		= 0x01,		// новое значение совпадает с предыдущим
	GB_ACT_NEW		= 0x02,		// установлено новое значение
	GB_ACT_ERROR	= 0x04		// ошибочное значение
};


/// Количество команда в К400
enum eGB_K400_NUM_COM {
	GB_K400_NUM_COM_MIN = 0,	///< минимальное значение
	GB_K400_NUM_COM_0	= 0,	///<
	GB_K400_NUM_COM_4	= 1,	///<
	GB_K400_NUM_COM_8	= 2,	///<
	GB_K400_NUM_COM_12	= 3,	///<
	GB_K400_NUM_COM_16	= 4,	///<
	GB_K400_NUM_COM_24	= 5,	///<
	GB_K400_NUM_COM_32	= 6,	///<
	GB_K400_NUM_COM_MAX			///< кол-во пунктов в списке
};

/// Класс для пароля
class TPassword {

public:
	TPassword() {
        password_ = 10000;
		admin_ = PASSWORD_ADMIN;
	}

	// возвращает текущий пароль пользователя
	uint16_t get() const {
		return password_;
	}

	// устанавливает пароль пользователя
	bool set(uint16_t pas) {
		bool stat = false;
        if (pas < 10000) {
			password_ = pas;
			stat = true;
		}
		return stat;
	}

	// начальная инициализация пароля пользователя
	// если введено неверное значение, пароль будет PASSWORD_USER
	void init(uint16_t pas) {
		if (!set(pas))
			set(PASSWORD_USER);
	}

	// проверка значения на совпадение с паролем пользователя и администратора
	bool check(uint16_t pas) const {
		return ((pas == password_) || (pas == admin_));
	}

private:
	// пароль пользователя
	uint16_t password_;

	// пароль администратора
	uint16_t admin_;
};

/// класс для измеряемых параметров
class TMeasuredParameters {
public:
	TMeasuredParameters() {
		voltDef_ = 0;
		voltDef2_ = 0;
		voltCf_ = 0;
		voltCf2_ = 0;
		voltNoise_ = 0;
		voltNoise2_ = 0;
		voltOut_ = 0;
		curOut_ = 0;
		resistOut_ = 0;
		pulseWidth_ = 0;
		d_ = 0;
		temperature_ = -100;
		freqDev_ = 0;
	}

	// запас по защите
	int8_t getVoltageDef() const {
		return voltDef_;
	}
	bool setVoltageDef(int8_t voltDef) {
		bool stat = false;
		if ((voltDef > -100) && (voltDef < 100)) {
			voltDef_ = voltDef;
			stat = true;
		}
		return stat;
	}

	// запас по защите второй
	int8_t getVoltageDef2() const {
		return voltDef2_;
	}
	bool setVoltageDef2(int8_t voltDef) {
		bool stat = false;
		if ((voltDef > -100) && (voltDef < 100)) {
			voltDef2_ = voltDef;
			stat = true;
		}
		return stat;
	}

	// запас по КЧ
	int8_t getVoltageCf() const {
		return voltCf_;
	}
	bool setVoltageCf(int8_t voltCf) {
		bool stat = false;
		if ((voltCf > -100) && (voltCf < 100)) {
			voltCf_ = voltCf;
			stat = true;
		}
		return stat;
	}

	// запас по КЧ кторой
	int8_t getVoltageCf2() const {
		return voltCf2_;
	}
	bool setVoltageCf2(int8_t voltCf) {
		bool stat = false;
		if ((voltCf > -100) && (voltCf < 100)) {
			voltCf2_ = voltCf;
			stat = true;
		}
		return stat;
	}

	// напряжение выхода
	uint8_t getVoltageOutInt() const {
		return (voltOut_ / 10);
	}
	uint8_t getVoltageOutFract() const {
		return (voltOut_ % 10);
	}
	uint16_t getVoltageOut() const {
		return voltOut_;
	}
	bool setVoltageOut(uint8_t voltOutInt, uint8_t voltOutFract) {
		bool stat = false;
		if ((voltOutInt < 100) && (voltOutFract < 10)) {
			voltOut_ = (((uint16_t) voltOutInt) * 10) + voltOutFract;
			stat = true;
		}
		return stat;
	}

	// ток выхода
	uint16_t getCurrentOut() const {
		return curOut_;
	}
	bool setCurrentOut(uint16_t curOut) {
		bool stat = false;
		if (curOut <= 999) {
			curOut_ = curOut;
			stat = true;
		}
		return stat;
	}

	// сопротивление выхода
	uint16_t getResistOut() const {
		return resistOut_;
	}
	bool setResistOut(uint16_t resistOut) {
		bool stat = false;
		if (resistOut <= 999) {
			resistOut_ = resistOut;
			stat = true;
		}
		return stat;
	}

	// уровень шума
	int8_t getVoltageNoise() const {
		return voltNoise_;
	}
	bool setVoltageNoise(int8_t voltNoise) {
		bool stat = false;
		if ((voltNoise > -100) && (voltNoise < 100)) {
			voltNoise_ = voltNoise;
			stat = true;
		}
		return stat;
	}

	// уровень шума 2
	int8_t getVoltageNoise2() const {
		return voltNoise2_;
	}
	bool setVoltageNoise2(int8_t val) {
		bool stat = false;
		if ((val > -100) && (val < 100)) {
			voltNoise2_ = val;
			stat = true;
		}
		return stat;
	}

	// длительность импульсов ВЧ блокировки на выходе применика
	uint16_t getPulseWidth() const {
		return pulseWidth_;
	}
	bool setPulseWidth(uint16_t val) {
		bool stat = false;
		if (val <= 360) {
			pulseWidth_ = val;
			stat = true;
		}
		return stat;
	}

	// Запас по тест.команде (двухчаст) или Отношение сигнал/помеха (одночаст)
	int8_t getD() const {
		return d_;
	}
	bool setD(int8_t val) {
		bool stat = false;
		if ((val >= -64) && (val <= 64)) {
			d_ = val;
			stat = true;
		}
		return stat;
	}


	int8_t getTemperature() const {
		return temperature_;
	}
	bool setTemperature(int8_t val){
		bool stat = false;
		if ((val >= -100) && (val <= 125)) {
			temperature_ = val;
			stat = true;
		}
		return stat;
	}

	// отклонение частоты КС На ПРМ
	int8_t getFreqDev() const {
		return freqDev_;
	}
	bool setFreqDev(int8_t val) {
		bool stat = false;
		if ((val >= -100) && (val <= 100)) {
			freqDev_ = val;
			stat = true;
		}
		return stat;
	}


private:
	// запас по защите (-99 .. 99)дБ
	int8_t voltDef_;
	// запас по защите второго канала (-99..99)дБ
	int8_t voltDef2_;
	// запас по КЧ (-99 .. 99)дБ
	int8_t voltCf_;
	// запас по КЧ второго канала (-99..99)дБ
	int8_t voltCf2_;
	// уровень шумов (-99 .. 99)дБ
	int8_t voltNoise_;
	// уровень шумов 2 (-99 .. 99)дБ
	int8_t voltNoise2_;
	// выходное напряжение умноженное на 10 (0 .. 999)В
	uint16_t voltOut_;
	// выходной ток (0 .. 999)мА
	uint16_t curOut_;
	// выходное сопротивление (0 .. 999)Ом
	uint16_t resistOut_;
	// длительность импульсов ВЧ блокировки на выходе применика (0..360)°
	uint16_t pulseWidth_;
	// запас по тест.команде (двухчаст) или Отношение сигнал/помеха (одночаст) [-64..64]дБ
	int8_t d_;
	// температуры (-100..125), -100 - ошибка
	int8_t temperature_;
	// отклонение частоты КС на ПРМ (-100..100)Гц
	int8_t freqDev_;
};

//
class TTest {
public:
	TTest() {
		clear();
	}
	// очистка списка сигналов
	void clear() {
		for(uint_fast8_t i = 0; i < MAX_NUM_TEST_SIGNAL; i++)
			signalList[i] = GB_SIGNAL_OFF;
		num_ = 1;
		currentSignal_ = GB_SIGNAL_OFF;
		currentSignal2_ = GB_SIGNAL_OFF;
	}

	bool addSignalToList(eGB_TEST_SIGNAL signal) {
		bool stat = false;
		if ((num_ < MAX_NUM_TEST_SIGNAL) && (signal < GB_SIGNAL_MAX)) {
			signalList[num_++] = signal;
			stat = true;
		}
		return stat;
	}

	/**	Преобразование тестового сигнала в значения для передачи в БСП.
	 * 	@param [out] cf	Код сигнала КЧ/Команды.
	 * 	@param [out] rz	Код сигнала РЗ.
	 *	@param sig Тестовый сигнал.
	 *
	 */
	void getBytes(uint8_t &cf, uint8_t &rz, eGB_TEST_SIGNAL sig) {
		if ((sig >= GB_SIGNAL_COM1) && (sig <= GB_SIGNAL_COM32)) {
			rz = 0;
			cf = 3 + sig - GB_SIGNAL_COM1; // 3 - кол-во кч ?!
		} else if ((sig >= GB_SIGNAL_CF1) && (sig <= GB_SIGNAL_CF4)) {
			rz = 0;
			cf = 1 + sig - GB_SIGNAL_CF1;
		} else if (sig == GB_SIGNAL_CF_NO_RZ) {
			rz = 1;
			cf = 1;
		} else if (sig == GB_SIGNAL_CF_RZ) {
			rz = 2;
			cf = 1;
		} else if (sig == GB_SIGNAL_CF2_NO_RZ) {
			rz = 1;
			cf = 2;
		} else if (sig == GB_SIGNAL_CF2_RZ) {
			rz = 2;
			cf = 2;
		} else if (sig == GB_SIGNAL_RZ) {
			rz = 1;
			cf = 0;
		} else if ((sig >= GB_SIGNAL_COM1_RZ) && (sig <= GB_SIGNAL_COM4_RZ)) {
			rz = 2;
			cf = 3 + sig - GB_SIGNAL_COM1_RZ; // 3 - кол-во кч ?!
		} else if ((sig>=GB_SIGNAL_COM1_NO_RZ)&&(sig<=GB_SIGNAL_COM4_NO_RZ)) {
			rz = 1;
			cf = 3 + sig - GB_SIGNAL_COM1_NO_RZ; // 3 - кол-во кч ?!
		} else if ((sig == GB_SIGNAL_CF) || (sig == GB_SIGNAL_CS)) {
			rz = 0;
			cf = 1;
		} else if ((sig >= GB_SIGNAL_COM1A) && (sig <= GB_SIGNAL_COM32C)) {
			rz = 0;
			cf = 3 + sig - GB_SIGNAL_COM1A;
		} else {
			rz = 0;
			cf = 0;
		}
	}

	/** Возвращает максимальное кол-во сигналов в Тесте.
	 *	@return Максимальное кол-во сигналов в Тесте.
	 */
	uint8_t getNumSignals() const {
		return num_;
	}

	/** Определенеи текущего сигнала передаваемого / примнимаемого в Тестах.
	 *
	 * 	В Тесте приемника всегда принимается 6 байт данных.
	 * 	В Тесте передатчика: в РЗСК/Р400м 3 байта, в К400/ОПТИКА 5 байт.
	 *
	 * 	В 3-х концевом варианте, кол-во байт данных удваивается.
	 *
	 * 	@param s Указатель на начало массива данных.
	 * 	@param type Тип аппарата.
	 * 	@param numBytes Максимальное количество команд.
	 */
	void setCurrentSignal(uint8_t *s, eGB_TYPE_DEVICE type, eGB_TYPE_OPTO opto) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_MAX;
		eGB_TEST_SIGNAL signal2 = GB_SIGNAL_MAX;

		if (type == AVANT_K400) {
			signal = getCurrentSignalK400(s);
			signal2 = getCurrentSignalK400((s + 5));
		} else if (type == AVANT_RZSK) {
			signal = getCurrentSignalRZSK(s);
			signal2 = getCurrentSignalRZSK((s + 3));
		} else if (type == AVANT_R400M) {
			signal = getCurrentSignalR400M(s);
		} else if (type == AVANT_OPTO) {
			if (opto == TYPE_OPTO_RING_UNI) {
				signal = getCurrentSignalOptoRingUni(s);
			} else {
				signal = getCurrentSignalOpto(s);
			}
		}
		currentSignal_ = signal;
		currentSignal2_ = signal2;
	}

	/** Возвращает текущий сигнал в Тесте для первого приемника.
	 * 	@return Текущий сигнал в Тесте.
	 */
	eGB_TEST_SIGNAL getCurrentSignal() const {
		return currentSignal_;
	}

	/** Возвращает текущий сигнал в Тесте для второго приемника.
	 * 	@return Текущий сигнал в Тесте.
	 */
	eGB_TEST_SIGNAL getCurrentSignal2() const {
		return currentSignal2_;
	}

	// список сигналов
	uint8_t signalList[MAX_NUM_TEST_SIGNAL];

private:
	// кол-во возможных сигналов в тесте
	uint8_t num_;

	// текущий сигнал
	eGB_TEST_SIGNAL currentSignal_;

	// текущий сигнал второго приемника
	eGB_TEST_SIGNAL currentSignal2_;

	// возвращает номер первого установленного бита 1..8, либо 0
	// проверка начинается с 0-ого бита
	uint8_t getSetBit(uint8_t byte) {
		uint8_t bit = 0;

		if (byte) {
			uint8_t b = 0;
			for (uint_fast8_t i = 1; i > 0; i <<= 1) {
				b++;
				if (byte & i) {
					bit = b;
					break;
				}
			}
		}
		return bit;
	}

	/** Добавление сигнала в список для К400.
	 * 	бит: 7		6		5		4		3		2		1		0		;
	 * 	b1 : x 		x 		x 		x 		x		x 		[кч2]	[кч1]	;
	 * 	b2 : [ком8] [ком7]	[ком6] 	[ком5] 	[ком4] 	[ком3] 	[ком2] 	[ком1]	;
	 * 	b3 : [ком16][ком15] [ком14] [ком13] [ком12] [ком11] [ком10] [ком9]	;
	 * 	b4 : [ком24][ком23] [ком22] [ком21] [ком20] [ком19] [ком18] [ком17]	;
	 * 	b5 : [ком32][ком31] [ком30] [ком29] [ком28] [ком27] [ком26] [ком25]	;
	 * 	Установленный бит означает наличие данного сигнала на передачу.
	 * 	Поиск ведется до первого установленного бита.
	 * 	@param *s Указатель на массив данных.
	 * 	@return Текущий тестовый сигнал.
	 */
	eGB_TEST_SIGNAL getCurrentSignalK400(uint8_t *s) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

		uint8_t t = *s;
		// сначала проверяется наличие КЧ1-КЧ2
		if (t & 0x01)
			signal = GB_SIGNAL_CF1;
		else if (t & 0x02)
			signal = GB_SIGNAL_CF2;
		else {
			// проверяется начичие команд с 1 по 8
			t = getSetBit(*(++s));
			if (t) {
				t = GB_SIGNAL_COM1 + t - 1;
				signal = (eGB_TEST_SIGNAL) t;
			} else {
				// проверяется начичие команд с 9 по 16
				t = getSetBit(*(++s));
				if (t) {
					t = GB_SIGNAL_COM9 + t - 1;
					signal = (eGB_TEST_SIGNAL) t;
				} else {
					// проверяется начичие команд с 17 по 24
					t = getSetBit(*(++s));
					if (t) {
						t = GB_SIGNAL_COM17 + t - 1;
						signal = (eGB_TEST_SIGNAL) t;
					} else {
						// проверяется начичие команд с 25 по 32
						t = getSetBit(*(++s));
						if (t) {
							t = GB_SIGNAL_COM25 + t - 1;
							signal = (eGB_TEST_SIGNAL) t;
						}
					}
				}
			}
		}
		return signal;
	}

	/** Добавление сигнала в список для РЗСК.
	 * 	бит: 7	6	5	4	3		2		1		0		;
	 * 	b1 : 0 	0 	0	0	[рз2] 	[рз1] 	[кч2] 	[кч1]	;
	 * 	b2 : 0	0	0 	0 	[ком4] 	[ком3] 	[ком2] 	[ком1]	;
	 * 	Установленный бит означает наличие данного сигнала на передачу.
	 * 	Если есть сигнал РЗ, но нет ни команд ни КЧ. Считаем что КЧ1 есть!
	 * 	рз1 + комN = команда без блок;
	 * 	рз2 + комN = команда с блок;
	 * 	рз1 + кч2  = кч2 без блок;
	 * 	рз2 + кч2  = кч2 с блок;
	 * 	рз1 + кч1  = кч без блок;
	 * 	рз2 + кч1  = кч с блок.
	 * 	@param *s Указатель на массив данных.
	 * 	@return Текущий тестовый сигнал.
	 */
	eGB_TEST_SIGNAL getCurrentSignalRZSK(uint8_t *s) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

		uint8_t b1 = (*s) & 0x0F;
		uint8_t b2 = (*(++s)) & 0x0F;

		if (b2) {
			b2 = getSetBit(b2) - 1;
			if (b1 & 0x04) {
				signal = (eGB_TEST_SIGNAL) (b2 + GB_SIGNAL_COM1_NO_RZ);
			} else if (b1 & 0x08) {
				signal = (eGB_TEST_SIGNAL) (b2 + GB_SIGNAL_COM1_RZ);
			}
		} else if (b1) {
			// если есть РЗ, то есть и КЧ
			if ((b1 & 0x06) == 0x06) {
				signal = GB_SIGNAL_CF2_NO_RZ;
			} else if ((b1 & 0x0A) == 0x0A) {
				signal = GB_SIGNAL_CF2_RZ;
			} else if ((b1 & 0x05) == 0x05) {
				signal = GB_SIGNAL_CF_NO_RZ;
			} else if ((b1 & 0x09) == 0x09)  {
				signal = GB_SIGNAL_CF_RZ;
			}
		}

		return signal;
	}

	/** Добавление сигнала в список для Р400м.
	 * 	бит: 7		6		5		4		3		2		1		0		;
	 * 	b1 : x 		x 		x 		[рз]	[кч4]	[кч3]	[кч2]	[кч1]	;
	 * 	Установленный бит означает наличие данного сигнала на передачу.
	 * 	Одновременно могут присутствовать оба сигнала.
	 * 	@param *s Указатель на массив данных.
	 * 	@return Текущий тестовый сигнал.
	 */
	eGB_TEST_SIGNAL getCurrentSignalR400M(uint8_t *s) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

		// TODO Р400М 3-х концевая вывод сигналов теста
		// подумать как выводить при наличии РЗ + КЧ1, КЧ2 и т.д.

		uint8_t t = *s;
		if (t & 0x10) {
			signal = GB_SIGNAL_RZ;

			// выводится "КЧ и РЗ" при их одновременно наличии
			if (t & 0x01) {
				signal = GB_SIGNAL_CF1_RZ_R400M;
			} else if (t & 0x02) {
				signal = GB_SIGNAL_CF2_RZ_R400M;
			} else if (t & 0x04) {
				signal = GB_SIGNAL_CF3_RZ_R400M;
			} else if (t & 0x08) {
				signal = GB_SIGNAL_CF4_RZ_R400M;
			}
		} else {
			if (t & 0x01) {
				signal = GB_SIGNAL_CF1;
			} else if (t & 0x02) {
				signal = GB_SIGNAL_CF2;
			} else if (t & 0x04) {
				signal = GB_SIGNAL_CF3;
			} else if (t & 0x08) {
				signal = GB_SIGNAL_CF4;
			}
		}

		return signal;
	}

	/** Добавление сигнала в список для ОПТИКИ.
	 * 	бит: 7		6		5		4		3		2		1		0		;
	 * 	b1 : x 		x 		x 		[рз]	x		x 		x 		[кч]	;
	 * 	b2 : [ком8] [ком7]	[ком6] 	[ком5] 	[ком4] 	[ком3] 	[ком2] 	[ком1]	;
	 * 	b3 : [ком16][ком15] [ком14] [ком13] [ком12] [ком11] [ком10] [ком9]	;
	 * 	b4 : [ком24][ком23] [ком22] [ком21] [ком20] [ком19] [ком18] [ком17]	;
	 * 	b5 : [ком32][ком31] [ком30] [ком29] [ком28] [ком27] [ком26] [ком25]	;
	 * 	Установленный бит означает наличие данного сигнала на передачу.
	 *	Поиск ведется до первого установленного бита.
	 * 	@param *s Указатель на массив данных.
	 * 	@return Текущий тестовый сигнал.
	 */
	eGB_TEST_SIGNAL getCurrentSignalOpto(uint8_t *s) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

		uint8_t t = *s;
		if (t & 0x10) {
			signal = GB_SIGNAL_RZ;
		} else if (t & 0x01) {
			signal = GB_SIGNAL_CS;
		} else {
			t = getSetBit(*(++s));
			if (t != 0) {
				t = (t - 1) + GB_SIGNAL_COM1;
				signal = (eGB_TEST_SIGNAL) t;
			} else {
				// проверяется начичие команд с 9 по 16
				t = getSetBit(*(++s));
				if (t) {
					t = GB_SIGNAL_COM9 + t - 1;
					signal = (eGB_TEST_SIGNAL) t;
				} else {
					// проверяется начичие команд с 17 по 24
					t = getSetBit(*(++s));
					if (t) {
						t = GB_SIGNAL_COM17 + t - 1;
						signal = (eGB_TEST_SIGNAL) t;
					} else {
						// проверяется начичие команд с 25 по 32
						t = getSetBit(*(++s));
						if (t) {
							t = GB_SIGNAL_COM25 + t - 1;
							signal = (eGB_TEST_SIGNAL) t;
						}
					}
				}
			}
		}
		return signal;
	}

	/** Добавление сигнала в список для ОПТИКИ.
		 * 	бит: 7		6		5		4		3		2		1		0		;
		 * 	b1 : x 		x 		x 		[рз]	x		x 		x 		[кч]	;
		 * 	b2 : [к8A]	[к7A]	[к6A] 	[к5A] 	[к4A] 	[к3A] 	[к2A] 	[к1A]	;
		 * 	b3 : [к16A]	[к15A] 	[к14A] 	[к13A] 	[к12A] 	[к11A] 	[к10A] 	[к9A]	;
		 * 	b4 : [к24A]	[к23A] 	[к22A] 	[к21A] 	[к20A] 	[к19A] 	[к18A] 	[к17A]	;
		 * 	b5 : [к32A]	[к31A]	[к30A] 	[к29A] 	[к28A] 	[к27A] 	[к26A] 	[к25A]	;
		 * 	b6 : [к8B]	[к7B]	[к6B] 	[к5B] 	[к4B] 	[к3B] 	[к2B] 	[к1B]	;
		 * 	b7 : [к16B]	[к15B] 	[к14B] 	[к13B] 	[к12B] 	[к11B] 	[к10B] 	[к9B]	;
		 * 	b8 : [к24B]	[к23B] 	[к22B] 	[к21B] 	[к20B] 	[к19B] 	[к18B] 	[к17B]	;
		 * 	b9 : [к32B]	[к31B]	[к30B] 	[к29B] 	[к28B] 	[к27B] 	[к26B] 	[к25B]	;
		 * 	b10: [к8C]	[к7C]	[к6C] 	[к5C] 	[к4C] 	[к3C] 	[к2C] 	[к1C]	;
		 * 	b11: [к16C]	[к15C] 	[к14C] 	[к13C] 	[к12C] 	[к11C] 	[к10C] 	[к9C]	;
		 * 	b12: [к24C]	[к23C] 	[к22C] 	[к21C] 	[к20C] 	[к19C] 	[к18C] 	[к17C]	;
		 * 	b13: [к32C]	[к31C]	[к30C] 	[к29C] 	[к28C] 	[к27C] 	[к26C] 	[к25C]	;
		 * 	Установленный бит означает наличие данного сигнала на передачу.
		 *	Поиск ведется до первого установленного бита.
		 * 	@param *s Указатель на массив данных.
		 * 	@return Текущий тестовый сигнал.
		 */
	eGB_TEST_SIGNAL getCurrentSignalOptoRingUni(uint8_t *s) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

		uint8_t t = *s;
		if (t & 0x10) {
			signal = GB_SIGNAL_RZ;
		} else if (t & 0x01) {
			signal = GB_SIGNAL_CS;
		} else {
			for(uint8_t i = 0; i < (MAX_NUM_COM_RING / 8); i++) {
				t = getSetBit(*(++s));
				if (t != 0) {
					t = (t - 1) + GB_SIGNAL_COM1A + i*8;
					signal = (eGB_TEST_SIGNAL) t;
				}
			}
		}
		return signal;
	}
};

#endif /* GLBDEF_H_ */
