/*
 * control.cpp
 *
 *  Created on: 11 ���. 2021 �.
 *      Author: Shcheblykin
 */

#include "control.h"
#include "paramDef.h"

const TControl::data_t TControl::mData[TControl::CTRL_MAX] PROGMEM = {
    // 12345678901234567890
    { "�����", GB_COM_SET_CONTROL, true, GB_CONTROL_CALL },
    { "���� ��", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK },
    { "���� �� ����", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK_SELF },
    { "����� ��", GB_COM_SET_CONTROL, true, GB_CONTROL_REG_AC },
    { "����� ��", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_AC },
    { "����� ���������", GB_COM_PRM_RES_IND, false, 0U },
    { "���� ������ ���.", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_ON },
    { "���� ������ ����.", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_OFF },
    { "���� ���������", GB_COM_PRM_ENTER, false, 0U },
    { "���� �� ����������", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_AC_UD },
    { "���� ���������� 1", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_1 },
    { "���� ���������", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_ALL },
    { "�����", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_SELF }
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
