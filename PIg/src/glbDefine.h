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

#include "avr.h"
#include "debug/debug.hpp"
#include <stdint.h>

/// размер массива
#define SIZE_OF(mas) (sizeof(mas) / sizeof(mas[0]))

/// преобразование двух CHAR в INT
#define TO_INT16(high, low)  ((static_cast<int16_t>(high) << 8) + (low))
#define TO_UINT16(high, low) ((static_cast<uint16_t>(high) << 8) + (low))

/// преобразование двоично-десятичного числа в целое
#define BCD_TO_BIN(val) (static_cast<uint8_t>((val >> 4) * 10 + (val & 0x0F)))

/// преобразование целого числа в двоично-десятичный вид
#define BIN_TO_BCD(val) (static_cast<uint8_t>(((val / 10) << 4) + (val % 10)))

/// Количество символов в строке ЖКИ
#define DISPLAY_ROW_SIZE 20

/// время вывода доп.сообщения на экран, мс
#define TIME_MESSAGE (3000 / MENU_TIME_CYLCE)


/// Команды
enum eGB_COM
{
    // + означает что команда задукоментирована
    GB_COM_NO                   = 0,
    GB_COM_DEF_GET_DEF_TYPE     = 0x01,  // +
    GB_COM_DEF_GET_LINE_TYPE    = 0x02,  // +
    GB_COM_DEF_GET_T_NO_MAN     = 0x03,  // +
    GB_COM_DEF_GET_DELAY        = 0x04,  // +
    GB_COM_DEF_GET_OVERLAP      = 0x05,  // +
    GB_COM_DEF_GET_RZ_DEC       = 0x06,  // +
    GB_COM_DEF_GET_PRM_TYPE     = 0x07,  // + ! в Р400М это снижение уровня АК
    GB_COM_DEF_GET_FREQ_PRD     = 0x08,  // +
    GB_COM_DEF_GET_RZ_THRESH    = 0x09,  // + ! в Р400М это частота ПРМ
    GB_COM_DEF_GET_TYPE_AC      = 0x0A,  // +
    GB_COM_DEF_GET_LIMIT_PRD    = 0x0B,  // +
    GB_COM_PRM_GET_TIME_ON      = 0x11,  // +
    GB_COM_PRM_GET_TEST_COM     = 0x12,  // +
    GB_COM_PRM_GET_TIME_OFF     = 0x13,  // +
    GB_COM_PRM_GET_BLOCK_COM    = 0x14,  // +
    GB_COM_PRM_GET_FREQ_CORR    = 0x15,  // +
    GB_COM_PRM_GET_INC_SAFETY   = 0x16,  // +
    GB_COM_PRM_GET_DR_STATE     = 0x17,  // +
    GB_COM_PRM_GET_DR_BLOCK     = 0x18,  // +
    GB_COM_PRM_GET_DR_COM       = 0x19,  // +
    GB_COM_PRM_GET_RING_COM_REC = 0x1B,  // +
    GB_COM_PRM_GET_COM          = 0x1C,  // +
    GB_COM_PRM_GET_COM_SIGN     = 0x1D,  // +
    GB_COM_PRD_GET_TIME_ON      = 0x21,  // +
    GB_COM_PRD_GET_DURATION     = 0x22,  // +
    GB_COM_PRD_GET_FREQ_CORR    = 0x23,  // +
    GB_COM_PRD_GET_BLOCK_COM    = 0x24,  // +
    GB_COM_PRD_GET_LONG_COM     = 0x25,  // +
    GB_COM_PRD_GET_TEST_COM     = 0x26,  // +
    GB_COM_PRD_GET_DR_STATE     = 0x27,  // +
    GB_COM_PRD_GET_DR_BLOCK     = 0x28,  // +
    GB_COM_PRD_GET_COM_A        = 0x29,  // +
    GB_COM_PRD_GET_RING_COM_TR  = 0x2B,  // +
    GB_COM_PRD_GET_COM          = 0x2C,  // +
    GB_COM_PRD_GET_CF_TM        = 0x2D,  // +
    GB_COM_PRD_GET_COM_SIGN     = 0x2E,  // +
    GB_COM_GET_SOST             = 0x30,  // +
    GB_COM_GET_FAULT            = 0x31,  // +
    GB_COM_GET_TIME             = 0x32,  // +
    GB_COM_GET_COR_U_I          = 0x33,  // + ! в ОПТИКЕ это Резервирование
    GB_COM_GET_MEAS             = 0x34,  // +
    GB_COM_GET_TIME_SINCHR      = 0x35,  // +
    GB_COM_GET_COM_PRM_KEEP     = 0x36,  // + ! в Р400М это Uвых номинальное
    GB_COM_GET_COM_PRD_KEEP     = 0x37,  // + ! дополнительно тип удаленного аппарата, телемеханика,
                                     // предупреждение и авария по D в К400 и т.д.
    GB_COM_GET_NET_ADR = 0x38,  // +
    GB_COM_GET_TIME_RERUN = 0x39,  // + ! в Р400М это параметры для совместимостей
    GB_COM_GET_FREQ       = 0x3A,  // +
    GB_COM_GET_DEVICE_NUM = 0x3B,  // +
    GB_COM_GET_CF_THRESHOLD = 0x3C,  // + ! порог предупреждения и загрубления
    GB_COM_GET_OUT_CHECK        = 0x3D,  // +
    GB_COM_GET_TEST             = 0x3E,  // +
    GB_COM_GET_VERS             = 0x3F,  // +
    GB_COM_PRM_ENTER            = 0x51,  // +
    GB_COM_SET_REG_DISABLED     = 0x70,  // +
    GB_COM_SET_REG_ENABLED      = 0x71,  // +
    GB_COM_SET_CONTROL          = 0x72,  // +
    GB_COM_SET_PASSWORD         = 0x73,  // + ! только с ПК
    GB_COM_GET_PASSWORD         = 0x74,  // + ! только с ПК
    GB_COM_SET_REG_TEST_2       = 0x7D,  // +
    GB_COM_SET_REG_TEST_1       = 0x7E,  // +
    GB_COM_DEF_SET_DEF_TYPE     = 0x81,  // +
    GB_COM_DEF_SET_LINE_TYPE    = 0x82,  // +
    GB_COM_DEF_SET_T_NO_MAN     = 0x83,  // +
    GB_COM_DEF_SET_DELAY        = 0x84,  // +
    GB_COM_DEF_SET_OVERLAP      = 0x85,  // +
    GB_COM_DEF_SET_RZ_DEC       = 0x86,  // +
    GB_COM_DEF_SET_PRM_TYPE     = 0x87,  // + ! в Р400М это снижение уровня АК
    GB_COM_DEF_SET_FREQ_PRD     = 0x88,  // +
    GB_COM_DEF_SET_RZ_THRESH    = 0x89,  // + ! в Р400М это частота ПРМ
    GB_COM_DEF_SET_TYPE_AC      = 0x8A,  // +
    GB_COM_DEF_SET_LIMIT_PRD    = 0x8B,  //
    GB_COM_PRM_SET_TIME_ON      = 0x91,  // +
    GB_COM_PRM_SET_TEST_COM     = 0x92,  // +
    GB_COM_PRM_SET_TIME_OFF     = 0x93,  // +
    GB_COM_PRM_SET_BLOCK_COM    = 0x94,  // +
    GB_COM_PRM_SET_FREQ_CORR    = 0x95,  // +
    GB_COM_PRM_SET_INC_SAFETY   = 0x96,  // +
    GB_COM_PRM_SET_DR_STATE     = 0x97,  // +
    GB_COM_PRM_SET_DR_BLOCK     = 0x98,  // +
    GB_COM_PRM_SET_DR_COM       = 0x99,  // +
    GB_COM_PRM_RES_IND          = 0x9A,  // +
    GB_COM_PRM_SET_RING_COM_REC = 0x9B,  // +
    GB_COM_PRM_SET_COM          = 0x9C,  // +
    GB_COM_PRM_SET_COM_SIGN     = 0x9D,  // +
    GB_COM_PRD_SET_TIME_ON      = 0xA1,  // +
    GB_COM_PRD_SET_DURATION     = 0xA2,  // +
    GB_COM_PRD_SET_FREQ_CORR    = 0xA3,  // +
    GB_COM_PRD_SET_BLOCK_COM    = 0xA4,  // +
    GB_COM_PRD_SET_LONG_COM     = 0xA5,  // +
    GB_COM_PRD_SET_TEST_COM     = 0xA6,  // +
    GB_COM_PRD_SET_DR_STATE     = 0xA7,  // +
    GB_COM_PRD_SET_DR_BLOCK     = 0xA8,  // +
    GB_COM_PRD_SET_COM_A        = 0xA9,  // +
    GB_COM_PRD_RES_IND          = 0xAA,  // +
    GB_COM_PRD_SET_RING_COM_TR  = 0xAB,  // +
    GB_COM_PRD_SET_COM          = 0xAC,  // +
    GB_COM_PRD_SET_CF_TM        = 0xAD,  // +
    GB_COM_PRD_SET_COM_SIGN     = 0xAE,  // +
    GB_COM_SET_TIME             = 0xB2,  // +
    GB_COM_SET_COR_U_I          = 0xB3,  // +
    GB_COM_SET_TIME_SINCHR      = 0xB5,  // +
    GB_COM_SET_COM_PRM_KEEP     = 0xB6,  // + ! в Р400М это Uвых номинальное
    GB_COM_SET_COM_PRD_KEEP     = 0xB7,  // + ! дополнительно тип удаленного аппарата, телемеханика
    GB_COM_SET_NET_ADR          = 0xB8,  // +
    GB_COM_SET_TIME_RERUN       = 0xB9,  // +
    GB_COM_SET_FREQ             = 0xBA,  // +
    GB_COM_SET_DEVICE_NUM       = 0xBB,  // +
    GB_COM_SET_CF_THRESHOLD     = 0xBC,  // +
    GB_COM_SET_OUT_CHECK        = 0xBD,  // +
    GB_COM_DEF_GET_JRN_CNT      = 0xC1,  // +
    GB_COM_DEF_GET_JRN_ENTRY    = 0xC2,  // +
    GB_COM_DEF_JRN_CLR = 0xCA,  // ! стирание журнала ЗАЩ, только с ПК
    GB_COM_PRM_GET_JRN_CNT   = 0xD1,  // +
    GB_COM_PRM_GET_JRN_ENTRY = 0xD2,  // +
    GB_COM_PRM_JRN_CLR = 0xDA,  // ! стирание журнала ПРМ, только с ПК
    GB_COM_PRD_GET_JRN_CNT   = 0xE1,  // +
    GB_COM_PRD_GET_JRN_ENTRY = 0xE2,  // +
    GB_COM_PRD_JRN_CLR   = 0xEA,  // ! стирание журнала ПРД, только с ПК
    GB_COM_GET_JRN_CNT   = 0xF1,  // +
    GB_COM_GET_JRN_ENTRY = 0xF2,  // +
    GB_COM_JRN_CLR = 0xFA  // ! стирание журнала событий, только с ПК
};


/// Маски команд
enum eGB_COM_MASK
{
    // Тип устройства
    GB_COM_MASK_DEVICE     = 0x30,
    GB_COM_MASK_DEVICE_DEF = 0x00,
    GB_COM_MASK_DEVICE_PRM = 0x10,
    GB_COM_MASK_DEVICE_PRD = 0x20,
    GB_COM_MASK_DEVICE_GLB = 0x30,

    // группа команды
    GB_COM_MASK_GROUP              = 0xC0,
    GB_COM_MASK_GROUP_READ_PARAM   = 0x00,
    GB_COM_MASK_GROUP_WRITE_REGIME = 0x40,
    GB_COM_MASK_GROUP_WRITE_PARAM  = 0x80,
    GB_COM_MASK_GROUP_READ_JOURNAL = 0xC0
};

#endif /* GLBDEF_H_ */
