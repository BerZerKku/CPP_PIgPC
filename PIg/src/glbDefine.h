/*
 * glb_def.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

/**
 *  Максимальное значение для строковых параметров на 1 больше возможного,
 *  для хранения в массиве ошибочного значения и удобства пользования
 *  перечислениями.
 *  Так что для ввода строкового значения максимум должен быть меньше на 1 !!
 */

#ifndef GLBDEF_H_
#define GLBDEF_H_

#include <stdint.h>
#include "avr.h"
#include "menu/dateTime.h"
#include "debug/debug.hpp"

/// размер массива
#define SIZE_OF(mas) (sizeof(mas) / sizeof(mas[0]))

/// версия текущей прошивки
#define VERS 0x0149

/// максимально кол-во команд на прием (должно быть кратно 8)
#define MAX_NUM_COM_PRM 32

/// максимальное кол-во команд на передачу (должно быть кратно 8)
#define MAX_NUM_COM_PRD 32

/// максимальное кол-во транзитных команд (+1 - это 0 команда)
#define MAX_NUM_COM_RING 96

/// преобразование двух CHAR в INT
#define TO_INT16(high, low) (((uint16_t) (high) << 8) + (low))

/// преобразование двоично-десятичного числа в целое
#define BCD_TO_BIN(val) ((val >> 4) * 10 + (val & 0x0F))

/// преобразование целого числа в двоично-десятичный вид
#define BIN_TO_BCD(val) (((val / 10) << 4) + (val % 10))

/// максимально возможное кол-во состояний устройств
#define MAX_NUM_DEVICE_STATE 14

/// максимальное кол-во неисправностей для любого устройства
#define MAX_NUM_FAULTS 16

/// максимальное кол-во предупреждений для любого устройства
#define MAX_NUM_WARNINGS 16

/// минимальное кол-во команд передаваемых за одну секунду
#define MIN_NUM_COM_SEND_IN_1_SEK 8

/// максимальное кол-во сигналов в тестах
#define MAX_NUM_TEST_SIGNAL 100

// длина половины строки (+1 - символ конца строки)
#define STRING_LENGHT (11 + 1)

// длина строки (+1 - символ конца строки)
#define NAME_PARAM_LENGHT (20 + 1)

/// максимальное и минимальный код типа событий в журнале событий
#define MIN_JRN_EVENT_VALUE 1
#define MAX_JRN_EVENT_VALUE 38

#define GLB_JRN_EVENT_K400_MAX  512     /// кол-во записей в журнале событый К400
#define GLB_JRN_EVENT_R400M_MAX 512     /// кол-во записей в журнале событый Р400м
#define GLB_JRN_EVENT_RZSK_MAX  256     /// кол-во записей в журнале событый РЗСК
#define GLB_JRN_EVENT_OPTO_MAX  256     /// кол-во записей в журнале событый ОПТИКА

#define GLB_JRN_DEF_K400_MAX    0       ///< кол-во записей в журнале защиты К400
#define GLB_JRN_DEF_R400M_MAX   1024    ///< кол-во записей в журнале защиты Р400м
#define GLB_JRN_DEF_RZSK_MAX    256     ///< кол-во записей в журнале защиты РЗСК
#define GLB_JRN_DEF_OPTO_MAX    2048    ///< кол-во записей в журнале защиты ОПТИКА

#define GLB_JRN_PRM_K400_MAX    512     ///< кол-во записей в журнале приемника К400
#define GLB_JRN_PRM_RZSK_MAX    256     ///< кол-во записей в журнале приемника РЗСК
#define GLB_JRN_PRM_OPTO_MAX    256     ///< кол-во записей в журнале приемника ОПТИКА

#define GLB_JRN_PRD_K400_MAX    512     /// кол-во записей в журнале передатчика К400
#define GLB_JRN_PRD_RZSK_MAX    256     /// кол-во записей в журнале передатчика РЗСК
#define GLB_JRN_PRD_OPTO_MAX    256     /// кол-во записей в журнале передатчика ОПТИКА

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
    MENU_ENTER_PARAM_NO,        ///< отмена изменения параметра
    MENU_ENTER_PARAM_INT,       ///< изменение параметра, целое значение
    MENU_ENTER_PARAM_LIST,      ///< изменение параметра, выбор из списка
    MENU_ENTER_PARAM_LIST_2,    ///< изменение параметра, выбор из списка значений
    MENU_ENTER_PARAM_U_COR,     ///< изменение параметра, коррекция напряжения
    MENU_ENTER_PASSWORD,        ///< ввод пароля
    MENU_ENTER_PASSWORD_NEW,    ///< ввод нового пароля
    MENU_ENTER_PASSWORD_READY,  ///< введен верный пароль
    MENU_ENTER_PASSWORD_N_READY,///< новый пароль введен корректно
    MENU_ENTER_PARAM_READY,     ///< необходимо изменить параметр
    MENU_ENTER_PARAM_MESSAGE,   ///< вывод сообщения на экран при вводе параметра
};

/// Тип аппарата
enum eGB_TYPE_DEVICE {
    AVANT_NO = 0,   // ошибочное значение
    AVANT_R400,     //
    AVANT_RZSK,     //
    AVANT_K400,     //
    AVANT_R400M,    //
    AVANT_OPTO,     //
    AVANT_MAX
};

/// Тип оптического аппарата
enum eGB_TYPE_OPTO {
    TYPE_OPTO_STANDART = 0, // стандартная оптика
    TYPE_OPTO_RING_UNI,     // оптическое однонаправленное кольцо
    TYPE_OPTO_RING_BI,      // оптиеское двунаправленное кольцо
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
    GB_SEND_NO,         ///< Команды стандартного протокола нет
    GB_SEND_INT8,       ///< Передается один байт данных.
    GB_SEND_INT8_DOP,   ///< Передается два байта данных (значение, доп.байт).
    GB_SEND_DOP_INT8,   ///< Передается два байта данных (доп.байт, значение).
    GB_SEND_INT16_BE,   ///< Передается два байта данных (in16>>8, int16&0xFF).
    GB_SEND_DOP_BITES,  ///< Передается битовая переменная (доп.байт, значение).
    GB_SEND_BITES_DOP,  ///< Передается битовая переменная (значение, доп.байт).
    GB_SEND_COR_U,      ///< Передается коррекция напряжения.
    GB_SEND_COR_I       ///< Передается коррекция тока.
};

/// Кол-во аппаратов в линии
//  Учесть что 1 и 2 используются для параметров в 3-х концевой. Не менять !!!
enum eGB_NUM_DEVICES {
    GB_NUM_DEVICES_MIN = 1,
    GB_NUM_DEVICES_2 = 1,
    GB_NUM_DEVICES_3 = 2,
    GB_NUM_DEVICES_MAX
};

/// Режимы работы
enum eGB_REGIME {
    GB_REGIME_MIN = 0,              //
    GB_REGIME_DISABLED = 0,         // выведен
    GB_REGIME_READY,                // готов
    GB_REGIME_ENABLED,              // введен
    GB_REGIME_TALK,                 // речь
    GB_REGIME_TEST_1,               // тест ПРД
    GB_REGIME_TEST_2,               // тест ПРМ
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
                                        // + означает что команда задукоментирована
    GB_COM_NO = 0,
    GB_COM_DEF_GET_DEF_TYPE     = 0x01, // +
    GB_COM_DEF_GET_LINE_TYPE    = 0x02, // +
    GB_COM_DEF_GET_T_NO_MAN     = 0x03, // +
    GB_COM_DEF_GET_DELAY        = 0x04, // +
    GB_COM_DEF_GET_OVERLAP      = 0x05, // +
    GB_COM_DEF_GET_RZ_DEC       = 0x06, // +
    GB_COM_DEF_GET_PRM_TYPE     = 0x07, // + ! в Р400М это снижение уровня АК
    GB_COM_DEF_GET_FREQ_PRD     = 0x08, // +
    GB_COM_DEF_GET_RZ_THRESH    = 0x09, // + ! в Р400М это частота ПРМ
    GB_COM_DEF_GET_TYPE_AC      = 0x0A, // +
    GB_COM_DEF_GET_LIMIT_PRD    = 0x0B, // +
    GB_COM_PRM_GET_TIME_ON      = 0x11, // +
    GB_COM_PRM_GET_TEST_COM     = 0x12, // +
    GB_COM_PRM_GET_TIME_OFF     = 0x13, // +
    GB_COM_PRM_GET_BLOCK_COM    = 0x14, // +
    GB_COM_PRM_GET_FREQ_CORR    = 0x15, // +
    GB_COM_PRM_GET_INC_SAFETY   = 0x16, // +
    GB_COM_PRM_GET_DR_STATE     = 0x17, // +
    GB_COM_PRM_GET_DR_BLOCK     = 0x18, // +
    GB_COM_PRM_GET_DR_COM       = 0x19, // +
    GB_COM_PRM_GET_RING_COM_REC = 0x1B, // +
    GB_COM_PRM_GET_COM          = 0x1C, // +
    GB_COM_PRM_GET_COM_SIGN     = 0x1D, // +
    GB_COM_PRD_GET_TIME_ON      = 0x21, // +
    GB_COM_PRD_GET_DURATION     = 0x22, // +
    GB_COM_PRD_GET_FREQ_CORR    = 0x23, // +
    GB_COM_PRD_GET_BLOCK_COM    = 0x24, // +
    GB_COM_PRD_GET_LONG_COM     = 0x25, // +
    GB_COM_PRD_GET_TEST_COM     = 0x26, // +
    GB_COM_PRD_GET_DR_STATE     = 0x27, // +
    GB_COM_PRD_GET_DR_BLOCK     = 0x28, // +
    GB_COM_PRD_GET_COM_A        = 0x29, // +
    GB_COM_PRD_GET_RING_COM_TR  = 0x2B, // +
    GB_COM_PRD_GET_COM          = 0x2C, // +
    GB_COM_PRD_GET_CF_TM        = 0x2D, // +
    GB_COM_PRD_GET_COM_SIGN     = 0x2E, // +
    GB_COM_GET_SOST             = 0x30, // +
    GB_COM_GET_FAULT            = 0x31, // +
    GB_COM_GET_TIME             = 0x32, // +
    GB_COM_GET_COR_U_I          = 0x33, // + ! в ОПТИКЕ это Резервирование
    GB_COM_GET_MEAS             = 0x34, // +
    GB_COM_GET_TIME_SINCHR      = 0x35, // +
    GB_COM_GET_COM_PRM_KEEP     = 0x36, // + ! в Р400М это Uвых номинальное
    GB_COM_GET_COM_PRD_KEEP     = 0x37, // + ! дополнительно тип удаленного аппарата, телемеханика, предупреждение и авария по D в К400 и т.д.
    GB_COM_GET_NET_ADR          = 0x38, // +
    GB_COM_GET_TIME_RERUN       = 0x39, // + ! в Р400М это параметры для совместимостей
    GB_COM_GET_FREQ             = 0x3A, // +
    GB_COM_GET_DEVICE_NUM       = 0x3B, // +
    GB_COM_GET_CF_THRESHOLD     = 0x3C, // + ! порог предупреждения и загрубления
    GB_COM_GET_OUT_CHECK        = 0x3D, // +
    GB_COM_GET_TEST             = 0x3E, // +
    GB_COM_GET_VERS             = 0x3F, // +
    GB_COM_PRM_ENTER            = 0x51, // +
    GB_COM_SET_REG_DISABLED     = 0x70, // +
    GB_COM_SET_REG_ENABLED      = 0x71, // +
    GB_COM_SET_CONTROL          = 0x72, // +
    GB_COM_SET_PASSWORD         = 0x73, // + ! только с ПК
    GB_COM_GET_PASSWORD         = 0x74, // + ! только с ПК
    GB_COM_SET_REG_TEST_2       = 0x7D, // +
    GB_COM_SET_REG_TEST_1       = 0x7E, // +
    GB_COM_DEF_SET_DEF_TYPE     = 0x81, // +
    GB_COM_DEF_SET_LINE_TYPE    = 0x82, // +
    GB_COM_DEF_SET_T_NO_MAN     = 0x83, // +
    GB_COM_DEF_SET_DELAY        = 0x84, // +
    GB_COM_DEF_SET_OVERLAP      = 0x85, // +
    GB_COM_DEF_SET_RZ_DEC       = 0x86, // +
    GB_COM_DEF_SET_PRM_TYPE     = 0x87, // + ! в Р400М это снижение уровня АК
    GB_COM_DEF_SET_FREQ_PRD     = 0x88, // +
    GB_COM_DEF_SET_RZ_THRESH    = 0x89, // + ! в Р400М это частота ПРМ
    GB_COM_DEF_SET_TYPE_AC      = 0x8A, // +
    GB_COM_DEF_SET_LIMIT_PRD    = 0x8B, //
    GB_COM_PRM_SET_TIME_ON      = 0x91, // +
    GB_COM_PRM_SET_TEST_COM     = 0x92, // +
    GB_COM_PRM_SET_TIME_OFF     = 0x93, // +
    GB_COM_PRM_SET_BLOCK_COM    = 0x94, // +
    GB_COM_PRM_SET_FREQ_CORR    = 0x95, // +
    GB_COM_PRM_SET_INC_SAFETY   = 0x16, // +
    GB_COM_PRM_SET_DR_STATE     = 0x97, // +
    GB_COM_PRM_SET_DR_BLOCK     = 0x98, // +
    GB_COM_PRM_SET_DR_COM       = 0x99, // +
    GB_COM_PRM_RES_IND          = 0x9A, // +
    GB_COM_PRM_SET_RING_COM_REC = 0x9B, // +
    GB_COM_PRM_SET_COM          = 0x9C, // +
    GB_COM_PRM_SET_COM_SIGN     = 0x9D, // +
    GB_COM_PRD_SET_TIME_ON      = 0xA1, // +
    GB_COM_PRD_SET_DURATION     = 0xA2, // +
    GB_COM_PRD_SET_FREQ_CORR    = 0xA3, // +
    GB_COM_PRD_SET_BLOCK_COM    = 0xA4, // +
    GB_COM_PRD_SET_LONG_COM     = 0xA5, // +
    GB_COM_PRD_SET_TEST_COM     = 0xA6, // +
    GB_COM_PRD_SET_DR_STATE     = 0xA7, // +
    GB_COM_PRD_SET_DR_BLOCK     = 0xA8, // +
    GB_COM_PRD_SET_COM_A        = 0xA9, // +
    GB_COM_PRD_RES_IND          = 0xAA, // +
    GB_COM_PRD_SET_RING_COM_TR  = 0xAB, // +
    GB_COM_PRD_SET_COM          = 0xAC, // +
    GB_COM_PRD_SET_CF_TM        = 0xAD, // +
    GB_COM_PRD_SET_COM_SIGN     = 0xAE, // +
    GB_COM_SET_TIME             = 0xB2, // +
    GB_COM_SET_COR_U_I          = 0xB3, // +
    GB_COM_SET_TIME_SINCHR      = 0xB5, // +
    GB_COM_SET_COM_PRM_KEEP     = 0xB6, // + ! в Р400М это Uвых номинальное
    GB_COM_SET_COM_PRD_KEEP     = 0xB7, // + ! дополнительно тип удаленного аппарата, телемеханика
    GB_COM_SET_NET_ADR          = 0xB8, // +
    GB_COM_SET_TIME_RERUN       = 0xB9, // +
    GB_COM_SET_FREQ             = 0xBA, // +
    GB_COM_SET_DEVICE_NUM       = 0xBB, // +
    GB_COM_SET_CF_THRESHOLD     = 0xBC, // +
    GB_COM_SET_OUT_CHECK        = 0xBD, // +
    GB_COM_DEF_GET_JRN_CNT      = 0xC1, // +
    GB_COM_DEF_GET_JRN_ENTRY    = 0xC2, // +
    GB_COM_DEF_JRN_CLR          = 0xCA, // ! стирание журнала ЗАЩ, только с ПК
    GB_COM_PRM_GET_JRN_CNT      = 0xD1, // +
    GB_COM_PRM_GET_JRN_ENTRY    = 0xD2, // +
    GB_COM_PRM_JRN_CLR          = 0xDA, // ! стирание журнала ПРМ, только с ПК
    GB_COM_PRD_GET_JRN_CNT      = 0xE1, // +
    GB_COM_PRD_GET_JRN_ENTRY    = 0xE2, // +
    GB_COM_PRD_JRN_CLR          = 0xEA, // ! стирание журнала ПРД, только с ПК
    GB_COM_GET_JRN_CNT          = 0xF1, // +
    GB_COM_GET_JRN_ENTRY        = 0xF2, // +
    GB_COM_JRN_CLR              = 0xFA  // ! стирание журнала событий, только с ПК
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
    POS_COM_NET_ADR_netAdr = 1
};

/// Последовательность параметров в команде COM_PRD_KEEP (0x37)
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
    POS_COM_PRD_KEEP_ringComTransit
};

/// Параметры (связаны с fParams)
typedef enum {
    GB_PARAM_NULL_PARAM = 0,    ///< параметр заглушка
    // общие параметры
    GB_PARAM_TIME_SYNCH,        ///< синхронизация часов
	GB_PARAM_TIME_SYNCH_SRC,	///< синхронизация часов (выбор источника)
    GB_PARAM_NUM_OF_DEVICE,     ///< номер аппарата
    GB_PARAM_NUM_OF_DEVICE_RING,///< номер аппарата в оптическом кольце
    GB_PARAM_OUT_CHECK,         ///< контроль выходного сигнала
    GB_PARAM_WARN_THD,          ///< порог предупреждения
    GB_PARAM_WARN_THD_CF,       ///< порог предупреждения по КЧ (для РЗСК)
    GB_PARAM_TIME_RERUN,        ///< время перезапуска
    GB_PARAM_COM_PRD_KEEP,      ///< удержание реле команд ПРД
    GB_PARAM_COM_PRM_KEEP,      ///< удержание реле команд ПРМ
    GB_PARAM_IN_DEC,            ///< загрубление чувствительности ПРМ
    GB_PARAM_NET_ADDRESS,       ///< адрес в локальной сети
    GB_PARAM_U_OUT_NOM,         ///< номинальноые выходное напряжение
    GB_PARAM_FREQ,              ///< частота
    GB_PARAM_COMP_P400,         ///< совместимость (Р400, Р400м)
    GB_PARAM_IN_DEC_AC_ANSWER,  ///< снижение ответа АК (ПВЗЛ)
    GB_PARAM_DETECTOR,          ///< тип детектора
    GB_PARAM_COR_U,             ///< коррекция напряжения
    GB_PARAM_COR_I,             ///< коррекция тока
    GB_PARAM_PVZUE_PROTOCOL,    ///< протокол обмена (ПВЗУ-Е)
    GB_PARAM_PVZUE_PARITY,      ///< признак четности (ПВЗУ-Е)
    GB_PARAM_PVZUE_FAIL,        ///< допустимые провалы (ПВЗУ-Е)
    GB_PARAM_PVZUE_NOISE_THD,   ///< порог по помехе (ПВЗУ-Е)
    GB_PARAM_PVZUE_NOISE_LVL,   ///< допустимая помеха (ПВЗУ-Е)
    GB_PARAM_PVZUE_AC_TYPE,     ///< тип автоконтроля (ПВЗУ-Е)
    GB_PARAM_PVZUE_AC_PERIOD,   ///< период беглого режима АК (ПВЗУ-Е)
    GB_PARAM_PVZUE_AC_PER_RE,   ///< период повтора беглого режима АК (ПЗВУ-Е)
    GB_PARAM_BACKUP,            ///< резервирование
    GB_PARAM_COMP_K400,         ///< совместимость К400
    GB_PARAM_NUM_OF_DEVICES,    ///< тип линии (кол-во аппаратов в линии)
    GB_PARAM_TM_K400,           ///< телемеханика
    GB_PARAM_WARN_D,            ///< уровень срабатывания предупредительной сигнализации по D
    GB_PARAM_ALARM_D,           ///< уровень срабатывания аварийной сигнализации по D
    GB_PARAM_TEMP_MONITOR,      ///< контроль температуры
    GB_PARAM_TEMP_THR_HI,       ///< верхнее значение температуры
    GB_PARAM_TEMP_THR_LOW,      ///< нижнее значение температуры
    GB_PARAM_TM_SPEED,          ///< скорость ТМ
    GB_PARAM_COMP_RZSK,         ///< совместимость (РЗСК)
    // параметры защиты
    GB_PARAM_DEF_TYPE,          ///< тип защиты
    GB_PARAM_TIME_NO_MAN,       ///< дополнительное время без манипуляции
    GB_PARAM_OVERLAP,           ///< перекрытие импульсов (кроме Р400/Р400м)
    GB_PARAM_OVERLAP_P400,      ///< перекрытие импульсов (Р400/Р400м)
    GB_PARAM_OVERLAP_OPTO,      ///< перекрытие импульсов (Р400 Оптика)
    GB_PARAM_DELAY,             ///< компенсация задержки
    GB_PARAM_DELAY_OPTO,        ///< компенсация задержки
    GB_PARAM_WARN_THD_RZ,       ///< порог предупреждения по РЗ (для РЗСК)
    GB_PARAM_SENS_DEC,          ///< загрубленеи чувствительности
    GB_PARAM_SENS_DEC_RZ,       ///< загрубленеи чувствительности по РЗ
    GB_PARAM_PRM_TYPE,          ///< тип приемника
    GB_PARAM_AC_IN_DEC,         ///< снижение уровня АК
    GB_PARAM_FREQ_PRD,          ///< частота ПРД
    GB_PARAM_FREQ_PRM,          ///< частота ПРМ
    GB_PARAM_SHIFT_FRONT,       ///< сдвиг переднего фронта ПРД
    GB_PARAM_SHIFT_BACK,        ///< сдвиг заднего фронта ПРД
    GB_PARAM_SHIFT_PRM,         ///< сдвиг ПРМ
    GB_PARAM_SHIFT_PRD,         ///< сдвиг ВЧ ПРД от ПУСК
    GB_PARAM_LIMIT_PRD,         ///< ограничение полосы передатчика
    GB_PARAM_DELAY_OFF_PRM,     ///< задержка выключения ПРМ
    GB_PARAM_DELAY_ON_PRM,      ///< задержка включения ПРМ
    GB_PARAM_DELAY_ON_PRD,      ///< задержка включения ПРД
    GB_PARAM_MIN_TIME_PRD,      ///< минимальная длительность ПРД
    // параметры передатчика
    GB_PARAM_PRD_IN_DELAY,      ///< время включения (задержка срабатывания дискретного входа)
    GB_PARAM_PRD_DURATION_L,    ///< длительность команды ВЧ
    GB_PARAM_PRD_DURATION_O,    ///< длительность команды ОПТИКА
    GB_PARAM_PRD_TEST_COM,      ///< тестовая команда
    GB_PARAM_PRD_COM_LONG,      ///< следящие команды
    GB_PARAM_PRD_COM_BLOCK,     ///< блокированные команды
    GB_PARAM_PRD_DR_ENABLE,     ///< трансляция ЦП
    GB_PARAM_PRD_DR_COM_BLOCK,  ///< блокированные команды ЦП
    GB_PARAM_PRD_COM_NUMS,      ///< количество команд
    GB_PARAM_PRD_COM_NUMS_A,    ///< количество команд группы А
    GB_PARAM_PRD_FREQ_CORR,     ///< коррекция частоты ПРД
    GB_PARAM_PRD_DEC_CF,        ///< снижение уровня КС
    GB_PARAM_PRD_DEC_TM,        ///< снижение уровня ТМ
    GB_PARAM_PRD_DEFAULT_CF,    ///< КС по умолчанию
    GB_PARAM_PRD_COM_SIGNAL,    ///< сигнализация команд ПРД
    // параметры приемника
    GB_PARAM_PRM_TIME_ON,       ///< задержка на фиксацию команды (время включения)
    GB_PARAM_PRM_COM_BLOCK,     ///< блокированные команды
    GB_PARAM_PRM_TIME_OFF,      ///< задержка на выключение
    GB_PARAM_PRM_DR_ENABLE,     ///< трансляция ЦП
    GB_PARAM_PRM_DR_COM_BLOCK,  ///< блокированные команды ЦП
    GB_PARAM_PRM_DR_COM_TO_HF,  ///< команда ВЧ в ЦП
    GB_PARAM_PRM_COM_NUMS,      ///< количество команд
    GB_PARAM_PRM_TEST_COM,      ///< прием тестовой команды
    GB_PARAM_PRM_FREQ_CORR,     ///< коррекция частоты ПРМ
    GB_PARAM_PRM_COM_SIGNAL,    ///< сигнализация команд ПРМ
    GB_PARAM_PRM_INC_SAFETY,    ///< Повышение безопасности
    // параметры интерфейса
    GB_PARAM_INTF_INTERFACE,    ///< интерфейс связи
    GB_PARAM_INTF_PROTOCOL,     ///< протокол
    GB_PARAM_INTF_BAUDRATE,     ///< скорость передачи
    GB_PARAM_INTF_DATA_BITS,    ///< биты данных
    GB_PARAM_INTF_PARITY,       ///< четность
    GB_PARAM_INTF_STOP_BITS,    ///< стоповые биты
    // параметры кольца
    GB_PARAM_RING_TIME_WAIT,    ///< время ожидания команд
    GB_PARAM_RING_COM_TRANSIT,  ///< транзитные команды
    GB_PARAM_RING_COM_REC,      ///< переназначение команд приемника
    GB_PARAM_RING_COM_TR,       ///< переназначение команд дискретных входов
    // другое
    GB_PARAM_OTHER_TEST_SIGNAL, ///< Тестовый сигнал.
    //
    GB_PARAM_MAX
} eGB_PARAM;

/// Значения команд управления
enum eGB_CONTROL {
    GB_CONTROL_RESET_SELF   = 1,    // сброс своего
    GB_CONTROL_RESET_UD     = 2,    // сброс удаленного (-ых)
    GB_CONTROL_RESET_AC     = 3,    // сброс АК
    GB_CONTROL_PUSK_UD_1    = 4,    // пуск удаленного (1)
    GB_CONTROL_PUSK_UD_2    = 5,    // пуск удаленного 2
    GB_CONTROL_PUSK_UD_ALL  = 6,    // пуск удаленных
    GB_CONTROL_CALL         = 7,    // вызов
    GB_CONTROL_PUSK_ON      = 8,    // пуск наладочный вкл.
    GB_CONTROL_PUSK_OFF     = 9,    // пуск наладочный выкл.
    GB_CONTROL_PUSK_AC_UD   = 10,   // пуск АК удаленный
    GB_CONTROL_RESET_UD_1   = 11,   // сброс удаленного 1
    GB_CONTROL_RESET_UD_2   = 12,   // сброс удаленного 2
    GB_CONTROL_PUSK_UD_3    = 13,   // пуск удаленного 3
    GB_CONTROL_REG_AC       = 14,   // режим АК
    GB_CONTROL_RESET_UD_3   = 15,   // сброс удаленного 3
    GB_CONTROL_MAN_1        = 16,   // пуск МАН удаленного (1)
    GB_CONTROL_MAN_2        = 17,   // пуск МАН удаленного 2
    GB_CONTROL_MAN_3        = 18,   // пуск МАН удаленного 3
    GB_CONTROL_MAN_ALL      = 19,   // пуск МАН удаленных
    GB_CONTROL_MAX                  //
};

/// События журнала передатчика/приемника - конец и начало команды
enum eGB_STATE_COM {
    GB_STATE_COM_MIN    = 0,        ///< Минимальное значение
    GB_STATE_COM_END    = 0,        ///< Окончание команды
    GB_STATE_COM_START,             ///< Начало команды
    GB_STATE_COM_MAX                ///< Максимальное значение
};

/// Источник передаваемой команды
enum eGB_SOURCE_COM {
    GB_SOURCE_COM_DI    = 0,        ///< Команда поступила с дискретного входа
    GB_SOURCE_COM_DR    = 1,        ///< Команда поступила с цифрового стыка
    GB_SOURCE_COM_MAX               ///< Ошибочное значение
};

/// сигналы в тест1 и тест2
enum eGB_TEST_SIGNAL {
    GB_SIGNAL_OFF = 0,              //
    GB_SIGNAL_CF,                   // РЗСК / Р400М
    GB_SIGNAL_CF1,
    GB_SIGNAL_CF2,
    GB_SIGNAL_CF3,
    GB_SIGNAL_CF4,
    GB_SIGNAL_CF_NO_RZ,             // РЗСК
    GB_SIGNAL_CF_RZ,                // РЗСК
    GB_SIGNAL_CF2_NO_RZ,            // РЗСК
    GB_SIGNAL_CF2_RZ,               // РЗСК
    GB_SIGNAL_RZ,
    GB_SIGNAL_COM1_NO_RZ,           // РЗСК
    GB_SIGNAL_COM2_NO_RZ,           // РЗСК
    GB_SIGNAL_COM3_NO_RZ,           // РЗСК
    GB_SIGNAL_COM4_NO_RZ,           // РЗСК
    GB_SIGNAL_COM5_NO_RZ,           // РЗСК
    GB_SIGNAL_COM6_NO_RZ,           // РЗСК
    GB_SIGNAL_COM7_NO_RZ,           // РЗСК
    GB_SIGNAL_COM8_NO_RZ,           // РЗСК
    GB_SIGNAL_COM1_RZ,              // РЗСК
    GB_SIGNAL_COM2_RZ,              // РЗСК
    GB_SIGNAL_COM3_RZ,              // РЗСК
    GB_SIGNAL_COM4_RZ,              // РЗСК
	GB_SIGNAL_COM5_RZ,              // РЗСК
	GB_SIGNAL_COM6_RZ,              // РЗСК
	GB_SIGNAL_COM7_RZ,              // РЗСК
	GB_SIGNAL_COM8_RZ,              // РЗСК
    GB_SIGNAL_COM1,                 // vvvvvvvvvvvvvvvvvvvvvvvvvv
    GB_SIGNAL_COM2,                 // сигналы команд должны идти
    GB_SIGNAL_COM3,                 // подряд для заполнения К400
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
    GB_SIGNAL_COM30,                // сигналы команд должны идти
    GB_SIGNAL_COM31,                // подряд для заполнения К400
    GB_SIGNAL_COM32,                // ^^^^^^^^^^^^^^^^^^^^^^^^^^
    GB_SIGNAL_CF_RZ_R400M,
    GB_SIGNAL_CF1_RZ_R400M,
    GB_SIGNAL_CF2_RZ_R400M,
    GB_SIGNAL_CF3_RZ_R400M,
    GB_SIGNAL_CF4_RZ_R400M,
    GB_SIGNAL_CS,                   // в оптике вместо КЧ
    GB_SIGNAL_COM1A,                // оптика однонаправленная vvv
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
    GB_SIGNAL_COM32C,   // оптика однонаправленная ^^^
    GB_SIGNAL_MAX
};



/// Состояние обработки принятого значения параметра.
enum eGB_ACT {
    GB_ACT_NO       = 0,        // значение по умолчанию
    GB_ACT_OLD      = 0x01,     // новое значение совпадает с предыдущим
    GB_ACT_NEW      = 0x02,     // установлено новое значение
    GB_ACT_ERROR    = 0x04      // ошибочное значение
};


/// Количество команда в К400
enum eGB_K400_NUM_COM {
    GB_K400_NUM_COM_MIN = 0,    ///< минимальное значение
    GB_K400_NUM_COM_0   = 0,    ///<
    GB_K400_NUM_COM_4   = 1,    ///<
    GB_K400_NUM_COM_8   = 2,    ///<
    GB_K400_NUM_COM_12  = 3,    ///<
    GB_K400_NUM_COM_16  = 4,    ///<
    GB_K400_NUM_COM_24  = 5,    ///<
    GB_K400_NUM_COM_32  = 6,    ///<
    GB_K400_NUM_COM_MAX         ///< кол-во пунктов в списке
};

/// Класс для текущего состояния аппарата
class TDeviceStatus {
public:
    /// Максимальное кол-во вариантов для номеров удаленных аппаратов
    // т.е. используются 3 бита, 0 бит - 1 аппарат, ... 2 биит - 3 аппарат.
    // если установлены все три - ошибка.
    static const uint8_t GB_MAX_REM_NUM = 7;

    TDeviceStatus() {
        // присваивание иемени по умолчанию
        static const char nameDev[] PROGMEM = "НЕТ";
        name = nameDev;
        enable_ = false;
        fault_ = 0;
        faults_ = 0;
        numFaults_ = 0;
        warning_ = 0;
        warnings_ = 0;
        numWarnings_ = 0;
        remoteNumber_ = 0;
        regime_ = GB_REGIME_MAX;
        state_ = MAX_NUM_DEVICE_STATE;
        dopByte_ = 0;
    }

    /** Установка номера удаленного аппарата.
     *
     *  Используется для вывода номеров удаленных аппаратов, для которых
     *  сформировалась ошибка (АВАНТ Р400(М) совместимость с ПВЗУ-Е).
     *
     *  @param val Номер аппарата(ов). Три младших бита отвечают за соответсвую-
     *  щий аппарат.
     *  @retval True - в случае успешной записи параметра.
     *  @retval False - если была обнаружена ошибка данных
     */
    bool setRemoteNumber(uint8_t val) {
        bool state = false;

        if (val > GB_MAX_REM_NUM) {
            remoteNumber_ = GB_MAX_REM_NUM;
        } else {
            remoteNumber_ = val;
            state = true;
        }

        return state;
    }

    /** Возвращает номер удаленного аппарата.
     *
     *  Используется для вывода номеров удаленных аппаратов, для которых
     *  сформировалась ошибка (АВАНТ Р400(М) совместимость с ПВЗУ-Е).
     *
     *  @param Номер аппарата(ов), а точнее строка в массиве  fcRemoteNum.
     */
    uint8_t getRemoteNumber() const {
        return remoteNumber_;
    }

    // аварии
    bool setFault(uint16_t faults) {
        fault_ = getFirstSetBit(faults);
        faults_ = faults;
        numFaults_ = getNumSetBits(faults);
        return true;
    }
    uint8_t getFault() const {
        return fault_;
    }
    uint16_t getFaults() const {
        return faults_;
    }
    uint8_t getNumFaults() const {
        return numFaults_;
    }

    // неисправности
    bool setWarning(uint16_t warnings) {
        warning_ = getFirstSetBit(warnings);
        warnings_ = warnings;
        numWarnings_ = getNumSetBits(warnings);
        return true;
    }
    uint8_t getWarning() const {
        return warning_;
    }
    uint16_t getWarnings() const {
        return warnings_;
    }
    uint8_t getNumWarnings() const {
        return numWarnings_;
    }

    /** Возвращает текущее состояние указанного номера предупреждения.
     *
     *  При значении аргумента 0xFF возвращает true если есть хотя бы одно
     *  предупреждение. При значениях от 0
     *
     *  @param shift Номер предупреждения.
     *  @return Состояние предупреждения.
     */
    bool isWarning(uint8_t shift=0xFF) const {
        bool s = false;
        if (shift == 0xFF) {
            s = (warnings_ != 0);
        } else if (shift < 16) {
            s = ((warnings_ & (1 << shift)) != 0);
        }
        return s;
    }

    /** Возвращает текущее состояние указанного номера неисправности.
     *
     *  При значении аргумента 0xFF возвращает true если есть хотя бы одно
     *  предупреждение. При значениях от 0
     *
     *  @param shift Номер неисправности.
     *  @return Состояние неисправности.
     */
    bool isFault(uint8_t shift=0xFF) const {
        bool s = false;
        if (shift == 0xFF) {
            s = (faults_ != 0);
        } else if (shift < 16) {
            s = ((faults_ & (1 << shift)) != 0);
        }
        return s;
    }

    // режим работы
    // для GLB возвращается GB_REGIME_DISABLED, в случае если все имеющиеся
    // устройства выведены, иначе GB_REGIME_MAX
    bool setRegime(eGB_REGIME regime) {
        bool stat = false;
        if ((regime >= GB_REGIME_MIN) && (regime <= GB_REGIME_MAX)) {
            stat = true;
        } else
            regime = GB_REGIME_MAX;
        regime_ = regime;
        return stat;
    }
    eGB_REGIME getRegime() const {
        return regime_;
    }

    //состояние
    bool setState(uint8_t state) {
        bool stat = false;
        if (state < MAX_NUM_DEVICE_STATE) {
            stat = true;
        } else {
            state = MAX_NUM_DEVICE_STATE;
        }
        state_ = state;
        return stat;
    }
    uint8_t getState() const {
        return state_;
    }

    uint8_t getDopByte() const {
        return dopByte_;
    }
    bool setDopByte(uint8_t byte) {
        dopByte_ = byte;
        return true;
    }

    // работа с флагом наличия устройства
    // возвращает true если новое значение отличается от текущего
    bool setEnable(bool val) {
        uint8_t act = GB_ACT_NO;

        if (enable_ == val) {
            act |= GB_ACT_OLD;
        } else {
            act |= GB_ACT_NEW;
        }
        enable_ = val;

        return act;
    }
    bool isEnable() const {
        return enable_;
    }

    // массивы расшифровок аварий и предупреждений
    PGM_P faultText[MAX_NUM_FAULTS];
    PGM_P warningText[MAX_NUM_WARNINGS];
    PGM_P stateText[MAX_NUM_DEVICE_STATE + 1];
    PGM_P name;

private:
    // текущая приоритетная неисправность, неисправности и кол-во неисправностей
    uint8_t fault_;
    uint16_t faults_;
    uint8_t numFaults_;

    // текущее приоритетное предупреждение, предупреждения и кол-во предупрежд.
    uint8_t warning_;
    uint16_t warnings_;
    uint8_t numWarnings_;

    // Номер удаленного аппарата для которого была сформирована ошибка/предупр.
    // используется в Р400(М) в совместимости с ПВЗУ-Е
    uint8_t remoteNumber_;

    eGB_REGIME regime_;
    uint8_t state_;
    uint8_t dopByte_;

    // True - означает наличие данного устройства в текущем исполнении АВАНТа
    bool enable_;


    // возвращает кол-во установленных в 1 бит
    uint8_t getNumSetBits(uint16_t val)
    {
        uint8_t cnt = 0;
        for(; val > 0; val >>= 1)
        {
            if (val & 0x0001)
            cnt++;
        }
        return cnt;
    }

    // возвращает номер младшего установленного бита
    uint8_t getFirstSetBit(uint16_t val)
    {
        uint8_t cnt = 0;
        for(; val > 0; val >>= 1)
        {
            if (val & 0x0001)
            break;
            cnt++;
        }
        return cnt;
    }
};


class TJournalEntry {
public:
    TJournalEntry() {
        clear();
    }

    void clear() {
        currentDevice_ = GB_DEVICE_MAX;
        deviceJrn_ = GB_DEVICE_K400_MAX;
        eventType_ = MAX_JRN_EVENT_VALUE - MIN_JRN_EVENT_VALUE + 1;
        regime_ = GB_REGIME_MAX;
        numCom_ = 0;
        sourceCom_ = GB_SOURCE_COM_MAX;
        srcCom_ = 0;
        signalPusk_ = false;
        signalStop_ = false;
        signalMan_ = false;
        signalPrm_ = false;
        signalPrd_ = false;
        signalOut_ = false;
        signals_ = 0;

        numJrnEntries_ = 0;
        maxNumJrnEntry_ = 0;
        overflow_ = false;
        addressFirstEntry_ = 0;

        currentEntry_ = 1;
        ready_ = false;

        val = false;
    }

    // время для записи журнала
    TDateTime dateTime;

    // время для передачи по протоколам
    TDateTime dateTimeTr;

    // значение для передачи по протоколу
    bool val;

    bool setCurrentDevice(eGB_DEVICE device) {
        bool stat = false;
        if ((device >= GB_DEVICE_MIN) && (device < GB_DEVICE_MAX)) {
            currentDevice_ = device;
            stat = true;
        } else
            currentDevice_ = GB_DEVICE_MAX;
        return stat;
    }
    eGB_DEVICE getCurrentDevice() const {
        return currentDevice_;
    }

    // запись\считывание устройства для которого сделана запись
    bool setDeviceJrn(eGB_DEVICE_K400 device) {
        bool stat = false;
        if ((device >= GB_DEVICE_K400_MIN) && (device < GB_DEVICE_K400_MAX)) {
            deviceJrn_ = device;
            stat = true;
        } else
            deviceJrn_ = GB_DEVICE_K400_MAX;
        return stat;
    }
    eGB_DEVICE_K400 getDeviceJrn() const {
        return deviceJrn_;
    }

    // тип события
    bool setEventType(uint8_t val) {
        bool state = false;

        uint8_t min = 255;
        uint8_t max = 0;
        // установка мин/макс значения события, в зависимости от
        // текущего журнала
        if (currentDevice_ == GB_DEVICE_GLB) {
            min = MIN_JRN_EVENT_VALUE;
            max = MAX_JRN_EVENT_VALUE;
        } else if (currentDevice_ == GB_DEVICE_PRD) {
            min = GB_STATE_COM_MIN;
            max = GB_STATE_COM_MAX;
        } else if (currentDevice_ == GB_DEVICE_PRM) {
            min = GB_STATE_COM_MIN;
            max = GB_STATE_COM_MAX;
        } else if (currentDevice_ == GB_DEVICE_DEF) {
            min = 0;
            max = MAX_NUM_DEVICE_STATE;
        }

        if ((val >= min) && (val <= max)) {
            eventType_ = val;
            state = true;
        } else
            eventType_ = max;

        return state;
    }
    uint8_t getEventType() const {
        return eventType_;
    }

    // источник команды
    bool setSrcCom(uint8_t val) {
        srcCom_ = val & 0x07;
        return true;
    }
    uint8_t getSrcCom() const {
        return srcCom_;
    }


    /** Установка записей для журналов ОПТИКИ.
     *  Считываются 4 байта, старшим вперед.
     *  Каждый установленный бит в них отвечает за свое событие.
     *  @param buf Указатель на массив из 4 байт данных.
     *  @retval True - всегда.
     */
    bool setOpticEntry(uint8_t *buf) {
        uint8_t num = 0;

        // В каждом байте записи считается кол-во установленных битов
        for(uint_fast8_t i = 0; i <= 3; i++) {
            uint8_t byte = *buf++;
            opticEntry_[i] = byte;
            for(uint_fast8_t j = 1; j > 0; j <<= 1) {
                if (byte & j) {
                    num++;
                }
            }
        }
        numOpticEntries_ = num;

        return true;
    }

    /** Возвращает кол-во активных событий в журнале ОПТИКИ.
     *  Для журнала событий это кол-во событий, для команд - команд.
     *  @return Кол-во активных собыйтив журнале ОПТИКИ
     */
    uint8_t getNumOpticsEntries() const {
        return numOpticEntries_;
    }

    /** Ищет и возвращает код события, исходя из номера события.
     *  Т.е. если у нас в записи 5 активных событий, для каждого из них
     *  можно получить код записи (например номер команды).
     *
     *  @param num Номер события, начиная с 1.
     *  @return Код события (0 - в случае ошибки).
     */
    uint8_t getOpticEntry(uint8_t num) {
        uint8_t val = 0;

        // проверка на допустимое значение
        if ((num >= 1) && (num <= numOpticEntries_)) {
            for(uint_fast8_t i = 0; i <= 3; i++) {
                // перебор 4-х байт, начиная с младшего
                uint8_t byte = opticEntry_[3 - i];
                for(uint_fast8_t j = 0; j < 8; j++) {
                    if (byte & (1 << j)) {
                        if (--num == 0) {
                            // номер установленного бита, от 1 до 32
                            val = 1 + ((i * 8) + j);
                        }
                    }
                }
            }
        }
        return val;
    }

    /** Установка записей для журналов ОПТИКИ ЦПП.
     *
     *  Считываются 4 байта, СТАРШИМ вперед.
     *  Каждый установленный бит в них отвечает за свое событие.
     *
     *  @param buf Указатель на массив из 4 байт данных.
     *  @retval True - всегда.
     */
    bool setOpticEntryDR(uint8_t *buf) {
        uint8_t num = 0;

        // В каждом байте записи считается кол-во установленных битов
        for(uint_fast8_t i = 0; i <= 3; i++) {
            uint8_t byte = *buf++;
            opticEntryDR_[i] = byte;
            for(uint_fast8_t j = 1; j > 0; j <<= 1) {
                if (byte & j) {
                    num++;
                }
            }
        }
        numOpticEntriesDR_ = num;

        return true;
    }

    /** Возвращает флаг команды ЦПП.
     *
     *  @param com Номер команды для которой проверяется флаг 1..32.
     *  @return 1 - если команда передана по ЦПП, 0 - с дискретного входа.
     */
    uint8_t getOpticEntryDR(uint8_t com) {
        eGB_SOURCE_COM val = GB_SOURCE_COM_DI;

        if ((com > 0) && (com < MAX_NUM_COM_PRD)) {
            com--;

            uint8_t i = (MAX_NUM_COM_PRD - 1) / 8;
            while(com > 8) {
                com -= 8;
                i--;
            }

            if (opticEntryDR_[i] & (1 << com)) {
                val = GB_SOURCE_COM_DR;
            }
        }

        return val;
    }

    // режим
    bool setRegime(eGB_REGIME regime) {
        bool state = false;
        if ((regime >= GB_REGIME_MIN) && (regime <= GB_REGIME_MAX)) {
            regime_ = regime;
            state = true;
        } else
            regime_ = GB_REGIME_MAX;
        return state;
    }
    eGB_REGIME getRegime() const {
        return regime_;
    }

    // сигналы для журнала защиты
    bool setSignalDef(uint8_t val) {
        bool state = true;

        signalPusk_ = (val & 0x10) ? 1 : 0;
        signalStop_ = (val & 0x20) ? 1 : 0;
        signalMan_ = (val & 0x40) ? 1 : 0;
        signalPrm_ = (val & 0x01) ? 1 : 0;
        signalPrd_ = (val & 0x02) ? 1 : 0;
        signalOut_ = (val & 0x04) ? 1 : 0;
        signals_ = val;

        return state;
    }
    uint8_t getSignalPusk() const {
        return signalPusk_;
    }
    uint8_t getSignalStop() const {
        return signalStop_;
    }
    uint8_t getSignalMan() const {
        return signalMan_;
    }
    uint8_t getSignalPrm() const {
        return signalPrm_;
    }
    uint8_t getSignalPrd() const {
        return signalPrd_;
    }
    uint8_t getSignalOut() const {
        return signalOut_;
    }
    uint8_t getSignals() const {
        return signals_;
    }

    // номер команды
    bool setNumCom(uint8_t num) {
        bool stat = false;
        if ((num > 0) && (num <= MAX_NUM_COM_PRD)) {
            numCom_ = num;
            stat = true;
        }
        return stat;
    }
    uint8_t getNumCom() const {
        return numCom_;
    }

    /** Установка источника передаваемой команды.
     *  В случае не корректного значения, в параметр будет записано значение
     *  \a GB_SOURCE_COM_MAX.
     *
     *  @see eGB_SOURCE_COM
     *  @param source Источник передаваемой команды.
     *  @return True - в случае корректного значения, False - иначе.
     */
    bool setSourceCom(eGB_SOURCE_COM source) {
        bool stat = false;

        switch(source) {
            case GB_SOURCE_COM_DI:
            case GB_SOURCE_COM_DR:
            case GB_SOURCE_COM_MAX: {
                stat = true;
            }
            break;
        }

        if (!stat) {
            source = GB_SOURCE_COM_MAX;
        }

        sourceCom_ = source;
        return stat;
    }

    /** Считывание источника передаваемой команды.
     *
     *  @return Источник передаваемой команды.
     */
    eGB_SOURCE_COM getSourceCom() const {
        return sourceCom_;
    }

    // количество записей в журнале
    bool setNumJrnEntry(uint16_t val) {
        bool stat = false;

        overflow_ = (val & 0xC000) != 0;

        val &= 0x3FFF;

        if (val <= maxNumJrnEntry_) {
            if (overflow_) {
                numJrnEntries_ = maxNumJrnEntry_;
                addressFirstEntry_ = val;
            } else {
                numJrnEntries_ = val;
                addressFirstEntry_ = 0;
            }
            stat = true;
        }
        return stat;
    }
    uint16_t getNumJrnEntries() const {
        return numJrnEntries_;
    }

    // максимальное кол-во записей в журнале
    bool setMaxNumJrnEntries(uint16_t max) {
        bool stat = false;
        if (max <= GLB_JRN_MAX) {
            stat = true;
            maxNumJrnEntry_ = max;
        }
        return stat;
    }
    uint16_t getMaxNumJrnEntry() const {
        return maxNumJrnEntry_;
    }

    // переполнение журнала
    bool isOverflow() const {
        return overflow_;
    }

    // номер адреса текущей записи в журнале
    uint16_t getEntryAdress() const {
        uint16_t address = 0;

        if (numJrnEntries_ > 0) {
            address= (currentEntry_ + addressFirstEntry_ - 1) % numJrnEntries_;
        }

        return address;
    }

    // текущая запись
    uint16_t getCurrentEntry() const {
        return currentEntry_;
    }
    // следующая/предыдущая запись возвращает true если новое значение
    // отличается от предыдущего
    bool setNextEntry() {
        bool stat = false;
        uint16_t tmp = currentEntry_;
        tmp = (tmp < numJrnEntries_) ? currentEntry_ + 1 : 1;
        if (tmp != currentEntry_) {
            currentEntry_ = tmp;
            ready_ = false;
            stat = true;
        }
        return stat;
    }
    bool setPreviousEntry() {
        bool stat = false;
        uint16_t tmp = currentEntry_;
        tmp = (tmp > 1) ? tmp - 1 : numJrnEntries_;
        if (tmp != currentEntry_) {
            currentEntry_ = tmp;
            ready_ = false;
            stat = true;
        }
        return stat;
    }

    // Установка номера записи
    bool setNumEntry(uint16_t num) {
        bool stat = false;
        if (num <= numJrnEntries_) {
            currentEntry_ = num;
            ready_ = false;
            stat = true;
        }
        return stat;
    }

    // утстановка и считывание флага получения информации о текущей записи
    bool setReady() {
        return (ready_ = true);
    }
    bool isReady() const {
        return ready_;
    }

private:
    // текущий журнал
    eGB_DEVICE currentDevice_;

    // устройство
    eGB_DEVICE_K400 deviceJrn_;

    // тип события
    uint8_t eventType_;

    // источник команды (номер аппарата с которого пришла команда)
    uint8_t srcCom_;

    // режим
    eGB_REGIME regime_;

    // сигналы для журнала защиты
    bool signalPusk_;
    bool signalStop_;
    bool signalMan_;
    bool signalPrm_;
    bool signalPrd_;
    bool signalOut_;    // Выход приемника (ПРМ2 или РЗвых)
    uint8_t signals_;

    // буфер записи для журналов ОПТИКИ
    uint8_t opticEntry_[4];

    // буфер записи для журналов ОПТИКИ ЦПП
    uint8_t opticEntryDR_[4];

    // кол-во событий в одной записи журнала ОПТИКИ
    uint8_t numOpticEntries_;

    // кол-во событий в одной записи журнала ОПТИКИ ЦПП
    uint8_t numOpticEntriesDR_;

    // кол-во записей в журнале
    uint16_t numJrnEntries_;

    // максимальное кол-во записей в журнале
    uint16_t maxNumJrnEntry_;

    // переполнение журнала
    bool overflow_;

    // адрес первой записи
    uint16_t addressFirstEntry_;

    // адрес текущей записи (отображаемой на экране)
    uint16_t currentEntry_;

    // номер команды в текущей записи
    uint16_t numCom_;

    // источник передаваемой команды
    eGB_SOURCE_COM sourceCom_;

    // флаг получения информации о текущей записи
    bool ready_;
};


class TJrnSCADA {
public:

    /// Состояние команды.
    typedef enum __attribute__ ((__packed__)) {
        COM_OFF = 0,    ///< Окончена.
        COM_ON,         ///< Началась.
        COM_MAX         ///< Максимальное значение.
    } com_t;

    /// Текущее состояние записи.
    typedef enum __attribute__ ((__packed__)) {
        STATE_IDLE = 0, ///< Ожидание.
        STATE_REC,      ///< Считать новую запись журнала.
        STATE_TR_OK,    ///< Текущая запись передана в SCADA, считать новую запись журнала.
        STATE_REC_OK,   ///< Запись журнала считана и ждет отправки в SCADA.
        STATE_MAX       ///< Максимальное значение.
    } state_t;

    /// Шаги проверки сигналов защиты.
    enum __attribute__ ((__packed__)) defSignal_t {
        DEF_SIGNAL_PUSK = 0,    ///< ПУСК.
        DEF_SIGNAL_STOP,        ///< СТОП.
        DEF_SIGNAL_MAN,         ///< МАН.
        DEF_SIGNAL_PRM,         ///< Осц. ПРМ.
        DEF_SIGNAL_PRD,         ///< Осц. ПРД.
        DEF_SIGNAL_OUT,         ///< Выход приемника.
        //
        DEF_SIGNAL_MAX          ///< Количество сигналов в списке.
    };

    TJrnSCADA() {
        m_u8Com = 0;
        m_u8ComSource = 0;
        m_u8Event = 0;
        m_eJrn = GB_DEVICE_K400_MAX;

        m_eState = STATE_IDLE;
        m_step = 0;
    }

    TDateTime dtime;

    /// Установка события.
    void setEvent(uint8_t val) {
        m_u8Event= val;
    }

    /// Возвращает событие.
    uint8_t getEvent() const {
        return m_u8Event;
    }

    /** Возвращает событие для журнала защиты.
     *
     *  @param[out] val Значение события.
     *  @return Событие для журнала защиты.
     *  @retval 0 если событий больше нет.
     */
    uint8_t getDefEvent(bool &val) {
        uint8_t ev = 0;

        switch(m_step) {
            case DEF_SIGNAL_PUSK:
                ev = ++m_step;
                val = m_u8Com & 0x01 ? true : false;
                break;
            case DEF_SIGNAL_STOP:
                ev = ++m_step;
                val = m_u8Com & 0x02 ? true : false;
                break;
            case DEF_SIGNAL_MAN:
                ev = ++m_step;
                val = m_u8Com & 0x04 ? true : false;
                break;
            case DEF_SIGNAL_PRM:
                ev = ++m_step;
                val = m_u8ComSource & 0x01 ? true : false;
                break;
            case DEF_SIGNAL_PRD:
                ev = ++m_step;
                val = m_u8ComSource & 0x02 ? true : false;
                break;
            case DEF_SIGNAL_OUT:
                ev = ++m_step;
                val = m_u8ComSource & 0x04 ? true : false;
                break;
        }


        return ev;
    }

    /// Установка номера команды.
    void setCom(uint8_t val) {
        m_u8Com = val;
    }

    /// Возвращает номер команды.
    uint8_t getCom() const {
        return m_u8Com;
    }

    /// Установка источника сигнала
    void setComSource(uint8_t source) {
        m_u8ComSource = source;
    }

    /** Возвращает источник сигнала
     *
     *  @retval 0 - дискретный вход.
     *  @retval 1 - цифровой переприем.
     */
    uint8_t getComSource() const {
        return m_u8ComSource;
    }

    /// Установка текущего журнала.
    void setJrn(uint8_t val) {
        if ((val >= GB_DEVICE_K400_MIN) && (val <GB_DEVICE_K400_MAX)) {
            m_eJrn = static_cast<eGB_DEVICE_K400> (val);
        } else {
            m_eJrn = GB_DEVICE_K400_MAX;
        }
        m_step = 0;
    }

    /// Проверка текущего журнала на журнал защиты.
    bool isJrnDef() const {
        return (m_eJrn == GB_DEVICE_K400_DEF);
    }

    /// Проверка текущего журнала на журнал событий.
    bool isJrnEvent() const {
        return (m_eJrn == GB_DEVICE_K400_GLB);
    }

    /// Проверка текущего журнала на журнал приемника.
    bool isJrnPrm() const {
        // TODO проверка на все остальные журналы приемника
        return (m_eJrn == GB_DEVICE_K400_PRM1);
    }

    /// Проверка текущего журнала на журнал передатчика.
    bool isJrnPrd() const {
        return (m_eJrn == GB_DEVICE_K400_PRD);
    }

    /// Установка текущего состояния в наличие новой записи.
    void setReadyToSend() {
        m_eState = STATE_REC_OK;
    }

    /// Уситановка текущего состояния в готовность к новой записи.
    void setReadyToRead() {
        m_eState = STATE_REC;
    }

    /// Установка текущего состояния в готовность к новой записи + предыдущая была передана.
    void setReadyToEvent() {
        m_eState = STATE_TR_OK;
    }

    /// Установка текущего состояния в ожидание.
    void setIdle() {
        m_eState = STATE_IDLE;
    }

    /// Проверка текущего состояния на запись нового сообщения.
    bool isReadyToWrite() const {
        return ((m_eState == STATE_REC) || (m_eState == STATE_TR_OK));
    }

    /** Проверка текущего состояния на наличие нового сообщения.
     *
     *  В случае ошибки, состояние будет установлено в ожидание нового события.
     *
     *  @return true - имеется новое сообщение.
     */
    bool isReadyToSend() {
        if (m_eState >= STATE_MAX) {
            m_eState = STATE_REC;
        }

        return (m_eState == STATE_REC_OK);
    }

    /** Возвращает текущее состояние.
     *
     *  @retval STATE_IDLE
     *  @retval STATE_REC
     *  @retval STATE_TR_OK
     */
    uint8_t getState() const {
        return (m_eState >= STATE_REC_OK) ? STATE_IDLE : m_eState;
    }

private:
    eGB_DEVICE_K400 m_eJrn; /// Журнал.
    uint8_t m_u8Event;      /// Cобытие.
    uint8_t m_u8Com;        /// Номер команды.
    uint8_t m_u8ComSource;  /// Источник команды (0 - ДВ, 1 - ЦПП).

    state_t m_eState;       /// Текущее состояние.
    uint8_t m_step;         /// Счетчик опроса.
};

#endif /* GLBDEF_H_ */
