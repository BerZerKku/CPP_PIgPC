#include "kbdLayout.h"

/// Массив кнопок Р400м
static const uint8_t fcKeyR400M[2*NUM_KEY_IN_LAYOUT] PROGMEM = {
    //		основные функции
    KEY_FUNC, 		KEY_UP, 		KEY_EMPTY, 		//
    KEY_LEFT, 		KEY_ENTER, 		KEY_RIGHT, 		//
    KEY_EMPTY, 		KEY_DOWN, 		KEY_CANCEL,		//
    //		дополнительные функции
    KEY_FUNC,		KEY_CALL,		KEY_PUSK_UD,	//
    KEY_AC_PUSK, 	KEY_MENU,		KEY_AC_PUSK_UD,	//
    KEY_AC_RESET,	KEY_AC_REGIME,	KEY_PUSK_NALAD	//
};

/// Массив кнопок К400
static const uint8_t fcKeyK400[2*NUM_KEY_IN_LAYOUT] PROGMEM = {
    //		основные функции
    KEY_FUNC, 		KEY_UP, 		KEY_EMPTY, 		//
    KEY_LEFT, 		KEY_ENTER, 		KEY_RIGHT, 		//
    KEY_CANCEL, 	KEY_DOWN, 		KEY_EMPTY,		//
    //		дополнительные функции
    KEY_FUNC,		KEY_EMPTY,		KEY_RESET_IND,	//
    KEY_PUSK, 		KEY_MENU,		KEY_EMPTY,		//
    KEY_EMPTY,		KEY_EMPTY,		KEY_RESET		//
};

/// Массив кнопок РЗСК
static const uint8_t fcKeyRZSK[2*NUM_KEY_IN_LAYOUT] PROGMEM = {
    //		основные функции
    KEY_FUNC, 		KEY_UP, 		KEY_EMPTY, 		//
    KEY_LEFT, 		KEY_ENTER, 		KEY_RIGHT, 		//
    KEY_EMPTY, 		KEY_DOWN, 		KEY_CANCEL,		//
    //		дополнительные функции
    KEY_FUNC,		KEY_CALL,		KEY_PUSK_UD,	//
    KEY_PUSK, 		KEY_MENU,		KEY_PUSK_NALAD,	//
    KEY_RESET_IND,	KEY_EMPTY,		KEY_EMPTY		//
};

/// текущая раскладка клаавиатуры
static PGM_P keyboard = (PGM_P) fcKeyR400M;

/**	Устанавливает текущуюю раскладку клавиатуры.
 * 	@param type Тип устройства.
 * 	@return Нет
 */
void vKEYset(eGB_TYPE_DEVICE type)
{
    if (type == AVANT_K400) {
        keyboard = (PGM_P) fcKeyK400;
    } else if (type == AVANT_R400M) {
        keyboard = (PGM_P) fcKeyR400M;
    } else if (type == AVANT_RZSK) {
        keyboard = (PGM_P) fcKeyRZSK;
    } else if (type == AVANT_OPTO) {
        Q_ASSERT("Unknown keyboard loyaut");
    }
}

eKEY vKEYgetButtonLayout(uint8_t number) {
    eKEY key = KEY_NO;

    if ((number > 0) && (number <= 2*NUM_KEY_IN_LAYOUT)) {
        key = static_cast<eKEY> (pgm_read_byte(((PGM_P) keyboard) + number - 1));
    }

     return key;
}

eKEY vKEYgetButton(uint8_t number)
{
    eKEY key = KEY_NO;

    if ((number > 0) && (number <= 2*NUM_KEY_IN_LAYOUT)) {
        key = static_cast<eKEY> (pgm_read_byte(((PGM_P) keyboard) + number - 1));
    }

    if (key == KEY_EMPTY) {
        key = KEY_NO;
    }

    return key;
}
