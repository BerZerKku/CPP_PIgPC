/*
 * flashParams.h
 *
 *  Created on: 27.03.2015
 *      Author: Shcheblykin
 */

#ifndef FLASHPARAMS_H_
#define FLASHPARAMS_H_

#include "glbDefine.h"
#include "parameter/param.h"
#include "paramDef.h"
#include "paramUart.h"
#include "paramGlb.h"

extern size_t getSizeOfParam();

/// массив строк размерностей, связан с Param::DIMENSION
static const char fcDimension[] [5] PROGMEM = {
    "",     // Param::DIM_NO
    "мА",   // Param::DIM_MA
    "А",    // Param::DIM_A
    "кА",   // Param::DIM_KA
    "В",    // Param::DIM_V
    "кВ",   // Param::DIM_KV
    "мс",   // Param::MSEC
    "c",    // Param::DIM_SEC
    "час",  // Param::DIM_HOUR
    "дБ",   // Param::DIM_DB
    "Гц",   // Param::DIM_HZ
    "кГц",  // Param::DIM_KHZ
    "град"  // Param::DIM_DEG
};

/// Возможные значения параметра.
static const char fcOnOff[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "выкл.",        ///< 0
    "вкл."          ///< 1
};

/// Тип защиты (параметр Защиты).
static const char fcDefType[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "ДФЗ-ПрПд",     ///< 0
    "ДФЗ-МК1",      ///< 1
    "ДФЗ-МК2",      ///< 2
    "ДФЗ-L60",      ///< 3
    "НЗ-ПрПд",      ///< 4
    "НЗ-МК",        ///< 5
    "ППЗ-ПрПд",     ///< 6
    "ППЗ-МК"        ///< 7
};

/// Тип линии (параметр Общий).
static const char fcNumDevices[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "2 концевая",   ///< 1
    "3 концевая"    ///< 2
};

/// Тип детектора (параметр Общий).
static const char fcPrmType[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "акт+пасс",     ///< 0
    "активный",     ///< 1
    "пассивный"     ///< 2
};

/// Тип совместимости (параметр Защиты). Связан с eGB_COMPATIBILITY.
static const char fcCompatibility[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "АВАНТ Р400",   ///< 0
    "ПВЗ-90",       ///< 1
    "АВЗК-80",      ///< 2
    "ПВЗУ-Е",       ///< 3
    "ПВЗЛ",         ///< 4
    "ЛИНИЯ-Р",      ///< 5
    "ПВЗК"          ///< 6
};

/// Тип совместимости (параметр К400). Связан с eGB_COMP_K400.
static const char fcCompK400[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "АВАНТ",        ///< 0
    "АВАНТ ПРД",    ///< 1
    "АВАНТ ПРМ",    ///< 2
    "АКПА ПРД",     ///< 3
    "АКПА ПРМ",     ///< 4
    "КЕДР ПРД",     ///< 5
    "КЕДР ПРМ",     ///< 6
    "УПК-Ц ПРД",    ///< 7
    "УПК-Ц ПРМ",    ///< 8
    "ВЧТО ПРД",     ///< 9
    "ВЧТО ПРМ",     ///< 10
    "АНКА ПРД",     ///< 11
    "АНКА ПРМ"      ///< 12
};

/// Тип совместимости (параметр РЗСК). Связан с eGB_COMP_RZSK.
static const char fcCompRZSK[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "РЗСК",         ///< 0
    "РЗСКм",        ///< 1
    "РЗСК-3 8к"     ///< 2
};

/// Частота ПРД / ПРМ (параметр Защиты).
static const char fcPvzlFreq[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "-1000Гц",      ///< 0
    "-500Гц",       ///< 1
    "0Гц",          ///< 2
    "+500Гц",       ///< 3
    "+1000Гц"       ///< 4
};

/// Протокол обмена (параметр Общий).
static const char fcPvzueProtocol[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "быстрый",      ///< 1
    "медленный"     ///< 2
};

/// Тип атоконтроля (параметр Общий).
static char fcPvzueTypeAC[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "тревожный",    ///< 1
    "нормальный",   ///< 2
    "спокойный"     ///< 3
};

/// Признак четности (параметр Общий).
static char fcPvzueParity[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "вкл.",         ///< 1
    "выкл."         ///< 2
};

/// Интерфейс связи (параметр Интерфейс).
static char fcInterface[TInterface::MAX][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "USB",          ///< TInterface::USB
    "RS485"         ///< TInterface::RS485
};

// Протокол (параметр Интерфейс).
static char fcProtocol[TProtocol::MAX ][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "СТАНДАРТ",     ///< TProtocol::STANDART
    "MODBUS",       ///< TProtocol::MODBUS
    "МЭК-101"       ///< TProtocol::IEC_101
};

// Скорость, бит/с (параметр Интерфейс).
static char fcBaudRate[TBaudRate::MAX][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "600",          ///< TBaudRate::_600
    "1200",         ///< TBaudRate::_1200
    "2400",         ///< TBaudRate::_2400
    "4800",         ///< TBaudRate::_4800
    "9600",         ///< TBaudRate::_9600
    "19200"         ///< TBaudRate::_19200
};

/// Количество бит данных (параметр Интерфейс).
static char fcDataBits[TDataBits::MAX][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "8"             ///< TDataBits::_8
};

/// Четность (параметр Интерфейс).
static char fcParity[TParity::MAX][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "нет",          ///< TParity::NONE
    "чет",          ///< TParity::EVEN
    "нечет"         ///< TParity::ODD
};

/// Стоповые биты (параметр Интерфейс).
static char fcStopBits[TStopBits::MAX][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "1",            ///< TStopBits::ONE
    "2"             ///< TStopBits::TWO
};

/// Скорость ТМ
static char fcTMSpeed[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "200",
    "400"
};

/// Сигналы КС
static char fcCF[][STRING_LENGHT] PROGMEM = {
    "КС1",
    "КС2"
};

/// Переназначение команд в кольце
static char fcRingRenumber[][STRING_LENGHT] PROGMEM = {
    "0",
    "1A","2A","3A","4A","5A","6A","7A","8A","9A","10A",
    "11A","12A","13A","14A","15A","16A","17A","18A","19A","20A",
    "21A","22A","23A","24A","25A","26A","27A","28A","29A","30A","31A","32A",
    "1B","2B","3B","4B","5B","6B","7B","8B","9B","10B",
    "11B","12B","13B","14B","15B","16B","17B","18B","19B","20B",
    "21B","22B","23B","24B","25B","26B","27B","28B","29B","30B","31B","32B",
    "1C","2C","3C","4C","5C","6C","7C","8C","9C","10C",
    "11C","12C","13C","14C","15C","16C","17C","18C","19C","20C",
    "21C","22C","23C","24C","25C","26C","27C","28C","29C","30C","31C","32C"
};

// Синхронизация времени
static char fcTymeSynchSrc[][STRING_LENGHT] PROGMEM = {
    //   1234567890
    "выкл.",
    "канал св.",
    "АСУ ТП"
};

/// Сигналы в Тестах ПРМ/ПРД.
static const char fcTest1K400[GB_SIGNAL_MAX + 1][STRING_LENGHT] PROGMEM = {
    //234567890
    "выкл.",        ///<
    "КЧ",           ///<
    "КЧ1",          ///<
    "КЧ2",          ///<
    "КЧ3",          ///<
    "КЧ4",          ///<
    "КЧ1",          ///< GB_SIGNAL_CF_NO_RZ
    "КЧ1 с блок",   ///< GB_SIGNAL_CF_RZ
    "КЧ2",          ///< GB_SIGNAL_CF2_NO_RZ
    "КЧ2 с блок",   ///< GB_SIGNAL_CF2_RZ
    "РЗ",           ///<
    "Команда1",     ///< GB_SIGNAL_COM1_NO_RZ
    "Команда2",     ///< GB_SIGNAL_COM2_NO_RZ
    "Команда3",     ///< GB_SIGNAL_COM3_NO_RZ
    "Команда4",     ///< GB_SIGNAL_COM4_NO_RZ
    "Команда5",     ///< GB_SIGNAL_COM5_NO_RZ
    "Команда6",     ///< GB_SIGNAL_COM6_NO_RZ
    "Команда7",     ///< GB_SIGNAL_COM7_NO_RZ
    "Команда8",     ///< GB_SIGNAL_COM8_NO_RZ
    "Ком1+блок",    ///< GB_SIGNAL_COM1_RZ
    "Ком2+блок",    ///< GB_SIGNAL_COM2_RZ
    "Ком3+блок",    ///< GB_SIGNAL_COM3_RZ
    "Ком4+блок",    ///< GB_SIGNAL_COM4_RZ
    "Ком5+блок",    ///< GB_SIGNAL_COM5_RZ
    "Ком6+блок",    ///< GB_SIGNAL_COM6_RZ
    "Ком7+блок",    ///< GB_SIGNAL_COM7_RZ
    "Ком8+блок",    ///< GB_SIGNAL_COM8_RZ
    "Команда1",     ///< GB_SIGNAL_COM1
    "Команда2",
    "Команда3",
    "Команда4",
    "Команда5",
    "Команда6",
    "Команда7",
    "Команда8",
    "Команда9",
    "Команда10",
    "Команда11",
    "Команда12",
    "Команда13",
    "Команда14",
    "Команда15",
    "Команда16",
    "Команда17",
    "Команда18",
    "Команда19",
    "Команда20",
    "Команда21",
    "Команда22",
    "Команда23",
    "Команда24",
    "Команда25",
    "Команда26",
    "Команда27",
    "Команда28",
    "Команда29",
    "Команда30",
    "Команда31",
    "Команда32",
    "КЧ и РЗ",      ///< GB_SIGNAL_CF_RZ_R400M
    "КЧ1 и РЗ",     ///< GB_SIGNAL_CF1_RZ_R400M
    "КЧ2 и РЗ",     ///< GB_SIGNAL_CF2_RZ_R400M
    "КЧ3 и РЗ",     ///< GB_SIGNAL_CF3_RZ_R400M
    "КЧ4 и РЗ",     ///< GB_SIGNAL_CF4_RZ_R400M
    "КС",           ///< в оптике вместо КЧ
    "Команда1A",    ///< Оптика однонаправленная vvv
    "Команда2A",
    "Команда3A",
    "Команда4A",
    "Команда5A",
    "Команда6A",
    "Команда7A",
    "Команда8A",
    "Команда9A",
    "Команда10A",
    "Команда11A",
    "Команда12A",
    "Команда13A",
    "Команда14A",
    "Команда15A",
    "Команда16A",
    "Команда17A",
    "Команда18A",
    "Команда19A",
    "Команда20A",
    "Команда21A",
    "Команда22A",
    "Команда23A",
    "Команда24A",
    "Команда25A",
    "Команда26A",
    "Команда27A",
    "Команда28A",
    "Команда29A",
    "Команда30A",
    "Команда31A",
    "Команда32A",
    "Команда1B",
    "Команда2B",
    "Команда3B",
    "Команда4B",
    "Команда5B",
    "Команда6B",
    "Команда7B",
    "Команда8B",
    "Команда9B",
    "Команда10B",
    "Команда11B",
    "Команда12B",
    "Команда13B",
    "Команда14B",
    "Команда15B",
    "Команда16B",
    "Команда17B",
    "Команда18B",
    "Команда19B",
    "Команда20B",
    "Команда21B",
    "Команда22B",
    "Команда23B",
    "Команда24B",
    "Команда25B",
    "Команда26B",
    "Команда27B",
    "Команда28B",
    "Команда29B",
    "Команда30B",
    "Команда31B",
    "Команда32B",
    "Команда1C",
    "Команда2C",
    "Команда3C",
    "Команда4C",
    "Команда5C",
    "Команда6C",
    "Команда7C",
    "Команда8C",
    "Команда9C",
    "Команда10C",
    "Команда11C",
    "Команда12C",
    "Команда13C",
    "Команда14C",
    "Команда15C",
    "Команда16C",
    "Команда17C",
    "Команда18C",
    "Команда19C",
    "Команда20C",
    "Команда21C",
    "Команда22C",
    "Команда23C",
    "Команда24C",
    "Команда25C",
    "Команда26C",
    "Команда27C",
    "Команда28C",
    "Команда29C",
    "Команда30C",
    "Команда31C",
    "Команда32C",   ///< Оптика однонаправленная ^^^
    "ошибка"        ///< ошибочное значение
};

#endif /* FLASHPARAMS_H_ */
