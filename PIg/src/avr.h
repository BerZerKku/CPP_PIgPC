/*
 * paramUart.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef AVR_H_
#define AVR_H_

#if defined(AVR)

#include <avr/eeprom.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include <limits.h>
#include <stdint.h>

#define pgm_read_ptr(x) (PGM_P) pgm_read_word(x)
#define nullptr NULL

#else

#include <climits>
#include <cstdint>
#include <cstddef>

#if defined(QT_CORE_LIB) && !defined(MY_TESTS)
#include "wrapper.hpp"
#endif

#define PGM_P const char *
#define PSTR
#define PROGMEM
#define snprintf_P snprintf
#define pgm_read_byte *(uint8_t *)
#define pgm_read_word *(uint16_t *)
#define pgm_read_ptr(x) *(x)
#define prog_uint8_t uint8_t

#endif


#endif /* PARAMIS_H_ */
