/*
 * control.cpp
 *
 *  Created on: 11 июн. 2021 г.
 *      Author: Shcheblykin
 */

#include "control.h"
#include "paramDef.h"

static const char CallText[] PROGMEM           = "Вызов";
static const char AcAutoText[] PROGMEM         = "АК автоматическ.";
static const char AcAcceleratedText[] PROGMEM  = "АК ускоренный";
static const char AcNormalText[] PROGMEM       = "АК нормальный";
static const char AcOffText[] PROGMEM          = "АК выключен";
static const char AcOnText[] PROGMEM           = "АК включен";
static const char AcPuskText[] PROGMEM         = "АК пуск";
static const char AcPuskSelfText[] PROGMEM     = "Пуск АК свой";
static const char AcQuickText[] PROGMEM        = "АК беглый";
static const char AcRegimeText[] PROGMEM       = "Режим АК";
static const char AcRequestText[] PROGMEM      = "АК запрос";
static const char AcResetText[] PROGMEM        = "Сброс АК";
static const char AcTestText[] PROGMEM         = "АК испытания";
static const char AcCtrlCheckText[] PROGMEM    = "АК контр.провер.";
static const char IndResetText[] PROGMEM       = "Сброс индикации";
static const char PuskAdjOnText[] PROGMEM      = "Пуск налад. вкл.";
static const char PuskAdjOffText[] PROGMEM     = "Пуск налад. выкл";
static const char PuskPrdText[] PROGMEM        = "Пуск ПРД";
static const char PuskPrmText[] PROGMEM        = "Пуск приемника";
static const char RemoteAcPuskText[] PROGMEM   = "Пуск АК удаленн.";
static const char RemoteManText[] PROGMEM      = "Пуск удален. МАН";
static const char RemoteMan1Text[] PROGMEM     = "Пуск удал. МАН 1";
static const char RemoteMan2Text[] PROGMEM     = "Пуск удал. МАН 2";
static const char RemoteMan3Text[] PROGMEM     = "Пуск удал. МАН 3";
static const char RemoteManAllText[] PROGMEM   = "Пуск удал-ых МАН";
static const char RemotePuskText[] PROGMEM     = "Пуск удаленного";
static const char RemotePusk1Text[] PROGMEM    = "Пуск удаленн. 1";
static const char RemotePusk2Text[] PROGMEM    = "Пуск удаленн. 2";
static const char RemotePusk3Text[] PROGMEM    = "Пуск удаленн. 3";
static const char RemotePuskAllText[] PROGMEM  = "Пуск удаленных";
static const char RemoteResetText[] PROGMEM    = "Сброс удаленного";
static const char RemoteReset1Text[] PROGMEM   = "Сброс удален. 1";
static const char RemoteReset2Text[] PROGMEM   = "Сброс удален. 2";
static const char RemoteReset3Text[] PROGMEM   = "Сброс удален. 3";
static const char RemoteResetAllText[] PROGMEM = "Сброс удаленных";
static const char ResetText[] PROGMEM          = "Сброс своего";
static const char ResetAllText[] PROGMEM       = "Сброс всех";
static const char SingleOffText[] PROGMEM      = "Одност.реж. выкл";
static const char SingleOnText[] PROGMEM       = "Одност.реж. вкл";

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
 * @brief Возвращает текст для сигнала управления.
 * @param[in] ctrl Сигнал управления.
 * @return Текст.
 * @return "" в случе ошибки.
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
