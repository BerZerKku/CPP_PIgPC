#include "kbdLayout.h"

/// ������ ������ �400�
static const uint8_t fcKeyR400M[2 * NUM_KEY_IN_LAYOUT] PROGMEM = {
    //      �������� �������
    KEY_FUNC,
    KEY_UP,
    KEY_EMPTY,  //
    KEY_LEFT,
    KEY_ENTER,
    KEY_RIGHT,  //
    KEY_EMPTY,
    KEY_DOWN,
    KEY_CANCEL,  //
    //      �������������� �������
    KEY_FUNC,
    KEY_CALL,
    KEY_PUSK_UD,  //
    KEY_AC_PUSK,
    KEY_MENU,
    KEY_AC_PUSK_UD,  //
    KEY_AC_RESET,
    KEY_AC_REGIME,
    KEY_PUSK_NALAD  //
};

/// ������ ������ �400
static const uint8_t fcKeyK400[2 * NUM_KEY_IN_LAYOUT] PROGMEM = {
    //      �������� �������
    KEY_FUNC,
    KEY_UP,
    KEY_EMPTY,  //
    KEY_LEFT,
    KEY_ENTER,
    KEY_RIGHT,  //
    KEY_CANCEL,
    KEY_DOWN,
    KEY_EMPTY,  //
    //      �������������� �������
    KEY_FUNC,
    KEY_EMPTY,
    KEY_RESET_IND,  //
    KEY_PUSK,
    KEY_MENU,
    KEY_EMPTY,  //
    KEY_EMPTY,
    KEY_EMPTY,
    KEY_RESET  //
};

/// ������ ������ ����
static const uint8_t fcKeyRZSK[2 * NUM_KEY_IN_LAYOUT] PROGMEM = {
    //      �������� �������
    KEY_FUNC,
    KEY_UP,
    KEY_EMPTY,  //
    KEY_LEFT,
    KEY_ENTER,
    KEY_RIGHT,  //
    KEY_EMPTY,
    KEY_DOWN,
    KEY_CANCEL,  //
    //      �������������� �������
    KEY_FUNC,
    KEY_CALL,
    KEY_PUSK_UD,  //
    KEY_PUSK,
    KEY_MENU,
    KEY_PUSK_NALAD,  //
    KEY_RESET_IND,
    KEY_EMPTY,
    KEY_EMPTY  //
};

/// ������� ��������� �����������
static PGM_P keyboard = (PGM_P) fcKeyR400M;

/** ������������� �������� ��������� ����������.
 *  @param type ��� ����������.
 *  @return ���
 */
void vKEYset(eGB_TYPE_DEVICE type)
{
    if (type == AVANT_K400)
    {
        keyboard = (PGM_P) fcKeyK400;
    }
    else if (type == AVANT_R400M)
    {
        keyboard = (PGM_P) fcKeyR400M;
    }
    else if (type == AVANT_RZSK)
    {
        keyboard = (PGM_P) fcKeyRZSK;
    }
    else
    {
        Q_ASSERT("Unknown keyboard loyaut");
    }
}

eKEY vKEYgetButtonLayout(uint8_t number)
{
    eKEY key = KEY_NO;

    if ((number > 0) && (number <= 2 * NUM_KEY_IN_LAYOUT))
    {
        key = static_cast<eKEY>(pgm_read_byte(((PGM_P) keyboard) + number - 1));
    }

    return key;
}

eKEY vKEYgetButton(uint8_t number)
{
    eKEY key = KEY_NO;

    if ((number > 0) && (number <= 2 * NUM_KEY_IN_LAYOUT))
    {
        key = static_cast<eKEY>(pgm_read_byte(((PGM_P) keyboard) + number - 1));
    }

    if (key == KEY_EMPTY)
    {
        key = KEY_NO;
    }

    return key;
}
