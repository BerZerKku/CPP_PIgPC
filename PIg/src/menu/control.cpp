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
static const char AcResetText[] PROGMEM        = "Сброс АК";
static const char AcTestText[] PROGMEM         = "АК испытания";
static const char CtrlCheckText[] PROGMEM      = "АК контр.провер.";
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
