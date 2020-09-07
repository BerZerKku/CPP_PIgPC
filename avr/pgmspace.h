#ifndef PGMSPACE_H
#define PGMSPACE_H

#define PGM_P const char *
#define PSTR
#define PROGMEM
#define snprintf_P snprintf
#define pgm_read_byte *(uint8_t *)
#define pgm_read_word *(uint16_t *)
#define prog_uint8_t uint8_t

#endif // PGMSPACE_H
