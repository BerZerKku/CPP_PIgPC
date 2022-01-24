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
static const char AcRequestText[] PROGMEM      = "�� ������";
static const char AcResetText[] PROGMEM        = "����� ��";
static const char AcTestText[] PROGMEM         = "�� ���������";
static const char AcCtrlCheckText[] PROGMEM    = "�� �����.������.";
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
    // 2345678901234567890
    { CallText, GB_COM_SET_CONTROL, true, GB_CONTROL_CALL },                  //
    { AcAutoText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_AUTO_NORM },       //
    { AcAcceleratedText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_ACCEL },    //
    { AcCtrlCheckText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK_SELF },  //
    { AcNormalText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_AUTO_NORM },     //
    { AcOffText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_OFF },              //
    { AcOnText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_AUTO_NORM },         //
    { AcPuskText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK },            //
    { AcPuskSelfText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK_SELF },   //
    { AcQuickText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_CHECK },          //
    { AcRegimeText, GB_COM_SET_CONTROL, true, GB_CONTROL_REG_AC },            //
    { AcRequestText, GB_COM_DEF_SET_TYPE_AC, true, 6U },                      //
    { AcResetText, GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_AC },           //
    { AcTestText, GB_COM_DEF_SET_TYPE_AC, true, GB_TYPE_AC_PUSK_SELF },       //
    { IndResetText, GB_COM_PRM_RES_IND, false, 0U },                          //
    { PuskAdjOnText, GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_ON },          //
    { PuskAdjOffText, GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_OFF },        //
    { PuskPrdText, GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_ALL },        //
    { PuskPrmText, GB_COM_PRM_ENTER, false, 0U },                             //
    { RemoteAcPuskText, GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_AC_UD },    //
    { RemoteManText, GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_1 },            //
    { RemoteMan1Text, GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_1 },           //
    { RemoteMan2Text, GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_2 },           //
    { RemoteMan3Text, GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_3 },           //
    { RemoteManAllText, GB_COM_SET_CONTROL, true, GB_CONTROL_MAN_ALL },       //
    { RemotePuskText, GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_1 },       //
    { RemotePusk1Text, GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_1 },      //
    { RemotePusk2Text, GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_2 },      //
    { RemotePusk3Text, GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_3 },      //
    { RemotePuskAllText, GB_COM_SET_CONTROL, true, GB_CONTROL_PUSK_UD_ALL },  //
    { RemoteResetText, GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD },       //
    { RemoteReset1Text, GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD_1 },    //
    { RemoteReset2Text, GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD_2 },    //
    { RemoteReset3Text, GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD_3 },    //
    { RemoteResetAllText, GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD },    //
    { ResetText, GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_SELF },           //
    { ResetAllText, GB_COM_SET_CONTROL, true, GB_CONTROL_RESET_UD },          //
    { SingleOffText, GB_COM_DEF_SET_TYPE_AC, true, 0 },                       //
    { SingleOnText, GB_COM_DEF_SET_TYPE_AC, true, 1 }                         //
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
    PGM_P text = static_cast<PGM_P>(pgm_read_ptr(&m_data[ctrl].text));
    return checkCtrl(ctrl) ? text : PSTR("");
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
