/*
 * control.cpp
 *
 *  Created on: 11 июн. 2021 г.
 *      Author: Shcheblykin
 */

#include "control.h"
#include "paramDef.h"

const TControl::data_t TControl::m_data[TControl::CTRL_MAX] PROGMEM = {
    // 12345678901234567890
    { "Вызов", GB_COM_SET_CONTROL, true, GB_CONTROL_CALL },                      // punkt05
    { "АК автоматическ.", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_AUTO_NORM },  // punkt15
    { "АК ускоренный", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_ACCEL },         // punkt16
    { "АК нормальный", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_AUTO_NORM },     // punkt19
    { "АК выключен", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_OFF },             // punkt17
    { "АК включен", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_AUTO_NORM },        // punkt31
    { "АК пуск", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK },                // punkt08
    { "Пуск АК свой", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK_SELF },      // punkt12
    { "АК беглый", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_CHECK },             // punkt20
    { "Режим АК", GB_COM_SET_CONTROL, true, GB_CONTROL_REG_AC },                 //
    { "Сброс АК", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_AC },               // punkt11
    { "АК испытания", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK_SELF },      // punkt18
    { "АК контр.провер.", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK_SELF },  // punkt10
    { "Сброс индикации", GB_COM_PRM_RES_IND, false, 0U },                        // punkt35
    { "Пуск налад. вкл.", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_ON },        // punkt06
    { "Пуск налад. выкл", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_OFF },       // punkt07
    { "Пуск ПРД", GB_COM_SET_CONTROL, false, GB_CONTROL_PUSK_UD_1 },             // punkt14
    { "Пуск приемника", GB_COM_PRM_ENTER, false, 0U },                           //
    { "Пуск АК удаленн.", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_AC_UD },     // punkt13
    { "Пуск удален. МАН", GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_1 },          // punkt09
    { "Пуск удал. МАН 1", GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_1 },          // punkt27
    { "Пуск удал. МАН 2", GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_2 },          // punkt28
    { "Пуск удал. МАН 3", GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_3 },          // punkt29
    { "Пуск удал-ых МАН", GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_ALL },        // punkt30
    { "Пуск удаленного", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_1 },       // punkt02
    { "Пуск удаленн. 1", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_1 },       // punkt23
    { "Пуск удаленн. 2", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_1 },       // punkt24
    { "Пуск удаленн. 3", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_1 },       // punkt25
    { "Пуск удаленных", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_ALL },      // punkt26
    { "Сброс удаленного", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD },       // punkt04
    { "Сброс удален. 1", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD_1 },      // punkt32
    { "Сброс удален. 2", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD_2 },      // punkt33
    { "Сброс удален. 3", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD_3 },      // punkt34
    { "Сброс удаленных", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD },        // punkt22
    { "Сброс своего", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_SELF },         // punkt03
    { "Сброс всех", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD },             // punkt36
    { "Одност.реж. выкл", GB_COM_DEF_SET_TYPE_AC, true, 0 },                     // punkt37
    { "Одност.реж. вкл", GB_COM_DEF_SET_TYPE_AC, true, 1 }                       // punkt38
};


/**
 * *****************************************************************************
 *
 * @brief Возвращает текст для сигнала управления.
 * @param[in] ctrl Сигнал управления.
 * @return Текст.
 * @return "" в случе ошибки.
 *
 * *****************************************************************************
 */
PGM_P TControl::getText(TControl::ctrl_t ctrl) const
{

    return checkCtrl(ctrl) ? m_data[ctrl].text : PSTR("");
}


/**
 * *****************************************************************************
 *
 * @brief Возвращает данные для сигнала управления.
 * @param[in] ctrl Сигнал управления.
 * @param[out] com Команда.
 * @param[out] isbyte Наличие байта данных.
 * @param[out] byte Байт данных.
 * @return true если сигнал корректный, иначе false.
 *
 * *****************************************************************************
 */
bool TControl::getData(TControl::ctrl_t ctrl, eGB_COM& com, bool& isbyte, uint8_t& byte) const
{
    bool check = checkCtrl(ctrl);

    if (check)
    {
        const data_t& data = m_data[ctrl];

        com    = static_cast<eGB_COM>(pgm_read_byte(&data.com));
        isbyte = pgm_read_byte(&data.isbyte);
        byte   = pgm_read_byte(&data.byte);
    }

    return check;
}


/**
 * *****************************************************************************
 *
 * @brief Проверяет корректность сигнала управления.
 * @param[in] ctrl Сигнал управления.
 * @return true если сигнал корректный, иначе false.
 *
 * *****************************************************************************
 */
bool TControl::checkCtrl(TControl::ctrl_t ctrl) const
{
    return (ctrl > CTRL_NO) && (ctrl < CTRL_MAX);
}
