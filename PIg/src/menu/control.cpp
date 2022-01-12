/*
 * control.cpp
 *
 *  Created on: 11 ���. 2021 �.
 *      Author: Shcheblykin
 */

#include "control.h"
#include "paramDef.h"

const TControl::data_t TControl::m_data[TControl::CTRL_MAX] PROGMEM = {
    // 12345678901234567890
    { "�����", GB_COM_SET_CONTROL, true, GB_CONTROL_CALL },                      // punkt05
    { "�� ������������.", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_AUTO_NORM },  // punkt15
    { "�� ����������", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_ACCEL },         // punkt16
    { "�� ����������", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_AUTO_NORM },     // punkt19
    { "�� ��������", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_OFF },             // punkt17
    { "�� �������", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_AUTO_NORM },        // punkt31
    { "�� ����", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK },                // punkt08
    { "���� �� ����", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK_SELF },      // punkt12
    { "�� ������", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_CHECK },             // punkt20
    { "����� ��", GB_COM_SET_CONTROL, true, GB_CONTROL_REG_AC },                 //
    { "����� ��", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_AC },               // punkt11
    { "�� ���������", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK_SELF },      // punkt18
    { "�� �����.������.", GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK_SELF },  // punkt10
    { "����� ���������", GB_COM_PRM_RES_IND, false, 0U },                        // punkt35
    { "���� �����. ���.", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_ON },        // punkt06
    { "���� �����. ����", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_OFF },       // punkt07
    { "���� ���", GB_COM_SET_CONTROL, false, GB_CONTROL_PUSK_UD_1 },             // punkt14
    { "���� ���������", GB_COM_PRM_ENTER, false, 0U },                           //
    { "���� �� �������.", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_AC_UD },     // punkt13
    { "���� ������. ���", GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_1 },          // punkt09
    { "���� ����. ��� 1", GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_1 },          // punkt27
    { "���� ����. ��� 2", GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_2 },          // punkt28
    { "���� ����. ��� 3", GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_3 },          // punkt29
    { "���� ����-�� ���", GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_ALL },        // punkt30
    { "���� ����������", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_1 },       // punkt02
    { "���� �������. 1", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_1 },       // punkt23
    { "���� �������. 2", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_1 },       // punkt24
    { "���� �������. 3", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_1 },       // punkt25
    { "���� ���������", GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_ALL },      // punkt26
    { "����� ����������", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD },       // punkt04
    { "����� ������. 1", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD_1 },      // punkt32
    { "����� ������. 2", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD_2 },      // punkt33
    { "����� ������. 3", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD_3 },      // punkt34
    { "����� ���������", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD },        // punkt22
    { "����� ������", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_SELF },         // punkt03
    { "����� ����", GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD },             // punkt36
    { "������.���. ����", GB_COM_DEF_SET_TYPE_AC, true, 0 },                     // punkt37
    { "������.���. ���", GB_COM_DEF_SET_TYPE_AC, true, 1 }                       // punkt38
};


/**
 * *****************************************************************************
 *
 * @brief ���������� ����� ��� ������� ����������.
 * @param[in] ctrl ������ ����������.
 * @return �����.
 * @return "" � ����� ������.
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
 * @brief ���������� ������ ��� ������� ����������.
 * @param[in] ctrl ������ ����������.
 * @param[out] com �������.
 * @param[out] isbyte ������� ����� ������.
 * @param[out] byte ���� ������.
 * @return true ���� ������ ����������, ����� false.
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
 * @brief ��������� ������������ ������� ����������.
 * @param[in] ctrl ������ ����������.
 * @return true ���� ������ ����������, ����� false.
 *
 * *****************************************************************************
 */
bool TControl::checkCtrl(TControl::ctrl_t ctrl) const
{
    return (ctrl > CTRL_NO) && (ctrl < CTRL_MAX);
}
