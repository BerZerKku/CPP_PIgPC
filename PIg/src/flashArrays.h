#ifndef FLASH_ARRAYS_H
#define FLASH_ARRAYS_H

/**
 * *****************************************************************************
 *
 *  @file flashArrays.h
 *
 *  VERSION_1v52:
 *      - Вновь.
 *
 * *****************************************************************************
 */

#include "avr.h"
#include "glbTypes.h"

extern const char fcCompR400m[GB_COMP_R400M_MAX][STRING_LENGHT] PROGMEM;
extern const char fcCompK400[GB_COMP_K400_MAX][STRING_LENGHT] PROGMEM;
extern const char fcCompRZSK[GB_COMP_RZSK_MAX][STRING_LENGHT] PROGMEM;

#endif  // FLASH_ARRAYS_H
