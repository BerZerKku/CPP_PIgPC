/*
 * control.cpp
 *
 *  Created on: 11 ���. 2021 �.
 *      Author: Shcheblykin
 */

#include "control.h"
#include "paramDef.h"

static const char CallText[] PROGMEM           = "�����";
static const char AcAutoText[] PROGMEM         = "�� ������������.";
static const char AcAcceleratedText[] PROGMEM  = "�� ����������";
static const char AcNormalText[] PROGMEM       = "�� ����������";
static const char AcOffText[] PROGMEM          = "�� ��������";
static const char AcOnText[] PROGMEM           = "�� �������";
static const char AcPuskText[] PROGMEM         = "�� ����";
static const char AcPuskSelfText[] PROGMEM     = "���� �� ����";
static const char AcQuickText[] PROGMEM        = "�� ������";
static const char AcRegimeText[] PROGMEM       = "����� ��";
static const char AcResetText[] PROGMEM        = "����� ��";
static const char AcTestText[] PROGMEM         = "�� ���������";
static const char CtrlCheckText[] PROGMEM      = "�� �����.������.";
static const char IndResetText[] PROGMEM       = "����� ���������";
static const char PuskAdjOnText[] PROGMEM      = "���� �����. ���.";
static const char PuskAdjOffText[] PROGMEM     = "���� �����. ����";
static const char PuskPrdText[] PROGMEM        = "���� ���";
static const char PuskPrmText[] PROGMEM        = "���� ���������";
static const char RemoteAcPuskText[] PROGMEM   = "���� �� �������.";
static const char RemoteManText[] PROGMEM      = "���� ������. ���";
static const char RemoteMan1Text[] PROGMEM     = "���� ����. ��� 1";
static const char RemoteMan2Text[] PROGMEM     = "���� ����. ��� 2";
static const char RemoteMan3Text[] PROGMEM     = "���� ����. ��� 3";
static const char RemoteManAllText[] PROGMEM   = "���� ����-�� ���";
static const char RemotePuskText[] PROGMEM     = "���� ����������";
static const char RemotePusk1Text[] PROGMEM    = "���� �������. 1";
static const char RemotePusk2Text[] PROGMEM    = "���� �������. 2";
static const char RemotePusk3Text[] PROGMEM    = "���� �������. 3";
static const char RemotePuskAllText[] PROGMEM  = "���� ���������";
static const char RemoteResetText[] PROGMEM    = "����� ����������";
static const char RemoteReset1Text[] PROGMEM   = "����� ������. 1";
static const char RemoteReset2Text[] PROGMEM   = "����� ������. 2";
static const char RemoteReset3Text[] PROGMEM   = "����� ������. 3";
static const char RemoteResetAllText[] PROGMEM = "����� ���������";
static const char ResetText[] PROGMEM          = "����� ������";
static const char ResetAllText[] PROGMEM       = "����� ����";
static const char SingleOffText[] PROGMEM      = "������.���. ����";
static const char SingleOnText[] PROGMEM       = "������.���. ���";

const TControl::data_t TControl::m_data[TControl::CTRL_MAX] PROGMEM = {
    // 12345678901234567890
    { CallText, GB_COM_SET_CONTROL, true, GB_CONTROL_CALL },                  // punkt05
    { AcAutoText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_AUTO_NORM },       // punkt15
    { AcAcceleratedText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_ACCEL },    // punkt16
    { AcNormalText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_AUTO_NORM },     // punkt19
    { AcOffText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_OFF },              // punkt17
    { AcOnText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_AUTO_NORM },         // punkt31
    { AcPuskText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK },            // punkt08
    { AcPuskSelfText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK_SELF },   // punkt12
    { AcQuickText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_CHECK },          // punkt20
    { AcRegimeText, GB_COM_SET_CONTROL, true, GB_CONTROL_REG_AC },            //
    { AcResetText, GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_AC },           // punkt11
    { AcTestText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK_SELF },       // punkt18
    { CtrlCheckText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK_SELF },    // punkt10
    { IndResetText, GB_COM_PRM_RES_IND, false, 0U },                          // punkt35
    { PuskAdjOnText, GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_ON },          // punkt06
    { PuskAdjOffText, GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_OFF },        // punkt07
    { PuskPrdText, GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_1 },          // punkt14
    { PuskPrmText, GB_COM_PRM_ENTER, false, 0U },                             //
    { RemoteAcPuskText, GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_AC_UD },    // punkt13
    { RemoteManText, GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_1 },            // punkt09
    { RemoteMan1Text, GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_1 },           // punkt27
    { RemoteMan2Text, GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_2 },           // punkt28
    { RemoteMan3Text, GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_3 },           // punkt29
    { RemoteManAllText, GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_ALL },       // punkt30
    { RemotePuskText, GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_1 },       // punkt02
    { RemotePusk1Text, GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_1 },      // punkt23
    { RemotePusk2Text, GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_2 },      // punkt24
    { RemotePusk3Text, GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_3 },      // punkt25
    { RemotePuskAllText, GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_ALL },  // punkt26
    { RemoteResetText, GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD },       // punkt04
    { RemoteReset1Text, GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD_1 },    // punkt32
    { RemoteReset2Text, GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD_2 },    // punkt33
    { RemoteReset3Text, GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD_3 },    // punkt34
    { RemoteResetAllText, GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD },    // punkt22
    { ResetText, GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_SELF },           // punkt03
    { ResetAllText, GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD },          // punkt36
    { SingleOffText, GB_COM_DEF_SET_TYPE_AC, true, 0 },                       // punkt37
    { SingleOnText, GB_COM_DEF_SET_TYPE_AC, true, 1 }                         // punkt38
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
