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
    { "Вызов", GB_COM_SET_CONTROL, true, GB_CONTROL_CALL },
    { "Пуск АК", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK },
    { "Пуск АК свой", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK_SELF },
    { "Режим АК", GB_COM_SET_CONTROL, true, GB_CONTROL_REG_AC },
    { "Сброс АК", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_AC },
    { "Сброс индикации", GB_COM_PRM_RES_IND, false, 0U },
    { "Пуск своего вкл.", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_ON },
    { "Пуск своего выкл.", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_OFF },
    { "Пуск приемника", GB_COM_PRM_ENTER, false, 0U },
    { "Пуск АК удаленного", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_AC_UD },
    { "Пуск удаленного 1", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_1 },
    { "Пуск удаленных", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_ALL },
    { "Сброс", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_SELF }
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
