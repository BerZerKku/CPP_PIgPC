/*
 * control.cpp
 *
 *  Created on: 11 июн. 2021 г.
 *      Author: Shcheblykin
 */

#include "control.h"
#include "paramDef.h"

const TControl::data_t TControl::mData[TControl::CTRL_MAX] PROGMEM = {
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

PGM_P TControl::getText(TControl::ctrl_t ctrl) const
{

    return checkCtrl(ctrl) ? mData[ctrl].text : PSTR("");
}

bool TControl::getData(TControl::ctrl_t ctrl, eGB_COM& com, bool& isbyte, uint8_t& byte) const
{
    bool check = checkCtrl(ctrl);

    if (check)
    {
        const data_t& data = mData[ctrl];

        com    = data.com;
        isbyte = data.isbyte;
        byte   = data.byte;
    }

    return check;
}

bool TControl::checkCtrl(TControl::ctrl_t ctrl) const
{
    return (ctrl > CTRL_NO) && (ctrl < CTRL_MAX);
}
