/*
 * menu.cpp
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */
#include <stdio.h>

#include "debug/debug.hpp"
#include "drivers/ks0108.h"
#include "flash.h"
#include "menu.h"
#include "txCom.h"
#include "version.hpp"

/// ����� ��������� �� ���������
#define LED_REGIME LED_SWITCH

/// ����� ������ ���������� �� ���
static char vLCDbuf[SIZE_BUF_STRING + 1];

/// ���-�� ����� ������ ������������ �� ������
#define NUM_TEXT_LINES (SIZE_BUF_STRING / 20)

clMenu::clMenu()
{
    // ��������� ������� ������� ������ �������������� � ������������� ���-�� ���������� ������
    COMPILE_TIME_ASSERT(SIZE_OF(fcRingRenumber) > MAX_NUM_COM_RING);

    lvlMenu         = &clMenu::lvlStart;
    lineParam_      = 3;
    lvlCreate_      = true;
    blink_          = false;
    blinkMeasParam_ = false;
    curCom_         = 0;
    delay_          = TIME_MESSAGE;

    // ������ ���������
    cursorEnable_ = false;
    cursorLine_   = 0;

    // ������� ������ ��� ���
    key_ = KEY_NO;

    // ��� ���������� ��� �� ��������
    setDevice(AVANT_NO);

    // ����� ��� ���
    sParam.connectionBsp = false;
    sParam.connectionPc  = false;

    // �������� ������� ��������� ������ ��� ���� ���������
    // ������ ������ ���� �������� ��������� � ��������� ������ ������
    // ���� fcUnknownSost
    sParam.def.status.stateText[0]                    = fcDefSost00;
    sParam.def.status.stateText[1]                    = fcDefSost01;
    sParam.def.status.stateText[2]                    = fcDefSost02;
    sParam.def.status.stateText[3]                    = fcDefSost03;
    sParam.def.status.stateText[4]                    = fcDefSost04;
    sParam.def.status.stateText[5]                    = fcDefSost05;
    sParam.def.status.stateText[6]                    = fcDefSost06;
    sParam.def.status.stateText[7]                    = fcDefSost07;
    sParam.def.status.stateText[8]                    = fcDefSost08;
    sParam.def.status.stateText[9]                    = fcDefSost09;
    sParam.def.status.stateText[10]                   = fcDefSost10;
    sParam.def.status.stateText[11]                   = fcDefSost11;
    sParam.def.status.stateText[12]                   = fcDefSost12;
    sParam.def.status.stateText[13]                   = fcDefSost13;
    sParam.def.status.stateText[14]                   = fcDefSost14;
    sParam.def.status.stateText[MAX_NUM_DEVICE_STATE] = fcUnknownSost;

    sParam.prm.status.stateText[0]                    = fcPrmSost00;
    sParam.prm.status.stateText[1]                    = fcPrmSost01;
    sParam.prm.status.stateText[2]                    = fcPrmSost02;
    sParam.prm.status.stateText[3]                    = fcPrmSost03;
    sParam.prm.status.stateText[4]                    = fcPrmSost04;
    sParam.prm.status.stateText[5]                    = fcPrmSost05;
    sParam.prm.status.stateText[6]                    = fcPrmSost06;
    sParam.prm.status.stateText[7]                    = fcPrmSost07;
    sParam.prm.status.stateText[8]                    = fcPrmSost08;
    sParam.prm.status.stateText[9]                    = fcPrmSost09;
    sParam.prm.status.stateText[10]                   = fcPrmSost10;
    sParam.prm.status.stateText[11]                   = fcPrmSost11;
    sParam.prm.status.stateText[12]                   = fcPrmSost12;
    sParam.prm.status.stateText[13]                   = fcPrmSost13;
    sParam.prm.status.stateText[14]                   = fcPrmSost14;
    sParam.prm.status.stateText[MAX_NUM_DEVICE_STATE] = fcUnknownSost;

    sParam.prd.status.stateText[0]                    = fcPrdSost00;
    sParam.prd.status.stateText[1]                    = fcPrdSost01;
    sParam.prd.status.stateText[2]                    = fcPrdSost02;
    sParam.prd.status.stateText[3]                    = fcPrdSost03;
    sParam.prd.status.stateText[4]                    = fcPrdSost04;
    sParam.prd.status.stateText[5]                    = fcPrdSost05;
    sParam.prd.status.stateText[6]                    = fcPrdSost06;
    sParam.prd.status.stateText[7]                    = fcPrdSost07;
    sParam.prd.status.stateText[8]                    = fcPrdSost08;
    sParam.prd.status.stateText[9]                    = fcPrdSost09;
    sParam.prd.status.stateText[10]                   = fcPrdSost10;
    sParam.prd.status.stateText[11]                   = fcPrdSost11;
    sParam.prd.status.stateText[12]                   = fcPrdSost12;
    sParam.prd.status.stateText[13]                   = fcPrdSost13;
    sParam.prd.status.stateText[14]                   = fcPrdSost14;
    sParam.prd.status.stateText[MAX_NUM_DEVICE_STATE] = fcUnknownSost;

    // �������� ����� �����������
    sParam.def.status.name = PSTR("���");  // fcDeviceName00;
    sParam.prm.status.name = PSTR("���");  // fcDeviceName01;
    sParam.prd.status.name = PSTR("���");  // fcDeviceName02;

#ifdef DEBUG
    // � ������ ������� �������� ���������� ���������
    vLCDsetLed(LED_ON);
#else
    // ��������� ������ ������ ���������
    vLCDsetLed(LED_REGIME);
#endif

    uint16_t version = TO_UINT16(PROJECT_VER_MAJOR, PROJECT_VER_MINOR);
    sParam.glb.setVersProgIC16(version, GB_IC_PI_MCU);
}

void clMenu::proc(void)
{

    static const char fcNoConnectBsp[] PROGMEM = " ��� ����� � ���!!! ";

    static uint8_t cntInitLcd = 0;  // ������� ������� �� ����������������� ���
    static uint8_t cntBlinkMeas = 0;  // ������� ������� ����� ���������
    static uint8_t cntBlinkText = 0;  // ������� ������� ����� ��������
    static uint16_t cntReturn = 0;  // ������� ������� �� �������� �� ��������� �������
    // ���������� ��������� ����� ������� ����� � ���
    static bool lastConnection = false;

    if (++cntBlinkMeas >= TIME_MEAS_PARAM)
    {
        blinkMeasParam_ = !blinkMeasParam_;
        cntBlinkMeas    = 0;
    }

    if (++cntBlinkText >= TIME_TEXT)
    {
        blink_       = !blink_;
        cntBlinkText = 0;
    }

    if (++cntInitLcd >= TIME_TO_INIT_LCD)
    {
        vLCDinit();
        cntInitLcd = 0;
    }

    // ��������� �������� ��� ������ ��� � ������ ����� ������ ����������
    // ����� ��������� ��� �������� �������� ��� ���� ������ ������
    if (!sParam.device)
    {
        setDevice(sParam.typeDevice);
        sParam.txComBuf.addFastCom(GB_COM_GET_VERS);
    }

    // ������� ��� � ����������
    // ���� ������ ����� ������ - ��������� ��������������� ���������
    uint8_t key_code = eKEYget();
    eKEY    key      = vKEYgetButton(key_code);
    if (key != KEY_NO)
    {
        if (key == KEY_EMPTY)
            key = KEY_NO;
        key_ = key;

        vLCDsetLed(LED_SWITCH);
        cntReturn = 0;
    }

    // ����� �� ��������� �������:
    // - ���� ����� ������;
    // - ��� ���������� (������� ������) � ������� ��������� �������.
    eGB_REGIME regime = sParam.glb.status.getRegime();
    if ((regime == GB_REGIME_ENABLED) && (lvlMenu != &clMenu::lvlStart))
    {
        if (cntReturn < TIME_RETURN_LVL_START)
        {
            cntReturn++;
        }

        if (cntReturn >= TIME_RETURN_LVL_START)
        {
            key_ = KEY_MENU;
        }
    }
    else
    {
        cntReturn = 0;
    }

    // ��������� ������� ������ + "��", ���� �� ���� ����� ������ ��������
    if ((key_ != KEY_NO) && !EnterParam.isEnable())
    {
        TControl::ctrl_t ctrl = onFnButton(key_);
        AddControlToSend(ctrl);
    }

    if (checkLedOn())
    {
        vLCDsetLed(LED_SWITCH);
    }

    // ������� ������ ���������
    if (delay_ < TIME_MESSAGE)
    {
        delay_++;
    }

    // ����� � ����� ����������� �������� ����
    // ���� ��������� ��� ��� �������� �� ���������
    clearTextBuf();
    (this->*lvlMenu)();
    key_ = KEY_NO;

#if defined(VIEW_DEBUG_PARAM)
    // ����� ���������� ����������
    if (this->lvlMenu == &clMenu::lvlStart)
    {
        snprintf(&vLCDbuf[20], 5, "1*%02X", sDebug.byte1);
        snprintf(&vLCDbuf[25], 5, "2*%02X", sDebug.byte2);
        snprintf(&vLCDbuf[30], 5, "3*%02X", sDebug.byte3);
        snprintf(&vLCDbuf[35], 5, "4*%02X", sDebug.byte4);
        snprintf(&vLCDbuf[40], 5, "5*%02X", sDebug.byte5);
        snprintf(&vLCDbuf[45], 5, "6*%02X", sDebug.byte6);
        snprintf(&vLCDbuf[50], 5, "7*%02X", sDebug.byte7);
        snprintf(&vLCDbuf[55], 5, "8*%02X", sDebug.byte8);
    }
    else
    {
        snprintf(&vLCDbuf[10], 5, "1*%02X", sDebug.byte1);
        snprintf(&vLCDbuf[15], 5, "2*%02X", sDebug.byte2);
    }
#endif

    // ����� ��������� � ������ ���������� ����� � ���
    bool connection = sParam.connectionBsp;
    if (!connection)
    {
        if (blink_)
        {
            // ���� ����� ���, ������������ ������ ���������� ����������
            // ��������� ��������������� �������
            snprintf_P(&vLCDbuf[0], 20, fcNoConnectBsp);
        }
    }
    else if (!lastConnection)
    {
        // ���� ����� � ��� ������ �������������� ������ ������� ������ ������
        sParam.txComBuf.addFastCom(GB_COM_GET_VERS);
    }
    lastConnection = connection;

    // �������������� ������ �������� � ������ ��� ������ �� �����
    vLCDputchar(vLCDbuf, lineParam_);
    // ������ ���������� ���-�� �� ���
    vLCDrefresh();
}

/** ���������� �������� ��� �400.
 *  @retval True - ������.
 */
bool clMenu::setDeviceK400()
{
    sParam.typeDevice = AVANT_K400;
    sParam.glb.setTypeDevice(AVANT_K400);

    // ���������/���������� ���������� � ����������� �� ������� �������������
    eGB_COMP_K400 comp = sParam.glb.getCompK400();
    bool          prm  = false;
    bool          prd  = false;
    switch (comp)
    {
    case GB_COMP_K400_AVANT:
        prm = true;
        prd = true;
        break;
    case GB_COMP_K400_AVANT_PRD: prd = true; break;
    case GB_COMP_K400_AVANT_PRM: prm = true; break;
    case GB_COMP_K400_AKPA_PRD: prd = true; break;
    case GB_COMP_K400_AKPA_PRM: prm = true; break;
    case GB_COMP_K400_KEDR_PRD: prd = true; break;
    case GB_COMP_K400_KEDR_PRM: prm = true; break;
    case GB_COMP_K400_UPKC_PRD: prd = true; break;
    case GB_COMP_K400_UPKC_PRM: prm = true; break;
    case GB_COMP_K400_VCTO_PRD: prd = true; break;
    case GB_COMP_K400_VCTO_PRM: prm = true; break;
    case GB_COMP_K400_ANKA_PRD: prd = true; break;
    case GB_COMP_K400_ANKA_PRM: prm = true; break;
    case GB_COMP_K400_MAX:  // �������� !!!
        break;
    }
    sParam.prd.status.setEnable(prd);
    sParam.prm.status.setEnable(prm);

    // ���� � ����� ��������� �� ���� ���������
    measParam[0]                      = MENU_MEAS_PARAM_TIME;
    measParam[MAX_NUM_MEAS_PARAM]     = MENU_MEAS_PARAM_TIME;
    measParam[1]                      = MENU_MEAS_PARAM_DATE;
    measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;

    if (!sParam.prd.status.isEnable())
    {
        // ���� ������ ��������, �� ����� U � I
        // � � ���� �������� ��� ���� ���� �������� U�1, Uk2, U�1, U�2
        if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3)
        {
            measParam[2]                      = MENU_MEAS_PARAM_UC1;
            measParam[2 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC1;
            measParam[3]                      = MENU_MEAS_PARAM_UC2;
            measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC2;
            measParam[4]                      = MENU_MEAS_PARAM_UN1;
            measParam[4 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN1;
            measParam[5]                      = MENU_MEAS_PARAM_UN2;
            measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN2;
        }
        else
        {
            measParam[3]                      = MENU_MEAS_PARAM_UC;
            measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC;
            measParam[5]                      = MENU_MEAS_PARAM_UN;
            measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_D;
        }
    }
    else if (!sParam.prm.status.isEnable())
    {
        // ���� ������ ����������, �� ����� Uk/U�
        measParam[2]                      = MENU_MEAS_PARAM_UOUT;
        measParam[2 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UOUT;
        measParam[4]                      = MENU_MEAS_PARAM_IOUT;
        measParam[4 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_IOUT;
    }
    else
    {
        // ���� ���� �������� � ���������� ������� U,I,Uk,U�
        // � 3-� �������� ��� ���� ��������� U�1/2, U�1/2
        measParam[2]                      = MENU_MEAS_PARAM_UOUT;
        measParam[2 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UOUT;
        measParam[4]                      = MENU_MEAS_PARAM_IOUT;
        measParam[4 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_IOUT;
        if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3)
        {
            measParam[3]                      = MENU_MEAS_PARAM_UC1;
            measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC2;
            measParam[5]                      = MENU_MEAS_PARAM_UN1;
            measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN2;
        }
        else
        {
            measParam[3]                      = MENU_MEAS_PARAM_UC;
            measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC;
            measParam[5]                      = MENU_MEAS_PARAM_UN;
            measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_D;
        }
    }

    // ���������� ������� ���������� ��� ���� "���������"
    uint8_t cnt = 0;
    if (sParam.prd.status.isEnable())
    {
        measParamLvl[cnt++] = MENU_MEAS_PARAM_UOUT;  // 1
        measParamLvl[cnt++] = MENU_MEAS_PARAM_IOUT;  // 2
        measParamLvl[cnt++] = MENU_MEAS_PARAM_ROUT;  // 3
    }
    if (sParam.prm.status.isEnable())
    {
        if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3)
        {
            measParamLvl[cnt++] = MENU_MEAS_PARAM_UC1;  // 4
            measParamLvl[cnt++] = MENU_MEAS_PARAM_UC2;  // 5
            measParamLvl[cnt++] = MENU_MEAS_PARAM_UN1;  // 6
            measParamLvl[cnt++] = MENU_MEAS_PARAM_UN2;  // 7
        }
        else
        {
            measParamLvl[cnt++] = MENU_MEAS_PARAM_UC;  // 4
            measParamLvl[cnt++] = MENU_MEAS_PARAM_UN;  // 5
            measParamLvl[cnt++] = MENU_MEAS_PARAM_D;   // 6
        }
        measParamLvl[cnt++] = MENU_MEAS_PARAM_DF;  // 7(8)
    }
    measParamLvl[cnt++] = MENU_MEAS_PARAM_TEMPERATURE;  // 8(9)

    // ���������� ������� ����� ��������������
    sParam.glb.status.faultText[0] = fcGlbFault0001;
    sParam.glb.status.faultText[1] = fcGlbFault0002;
    sParam.glb.status.faultText[2] = fcGlbFault0004;
    sParam.glb.status.faultText[3] = fcGlbFault0008;
    sParam.glb.status.faultText[4] = fcGlbFault0010;
    // 5-7 ���
    sParam.glb.status.faultText[8] = fcGlbFault0100;
    sParam.glb.status.faultText[9] = fcGlbFault0200;
    // 10 ���
    sParam.glb.status.faultText[11] = fcGlbFault0800;
    sParam.glb.status.faultText[12] = fcGlbFault1000;
    // 13-15 ���
    // ���������� ������� ����� ��������������
    sParam.glb.status.warningText[0] = fcGlbWarning01;
    // 1 - ���
    sParam.glb.status.warningText[2] = fcGlbWarning04k400;
    sParam.glb.status.warningText[3] = fcGlbWarning08k400;
    sParam.glb.status.warningText[4] = fcGlbWarning10k400;
    // 5-15 ���

    // ���������� ������
    sParam.def.status.setEnable(false);

    // ��������
    // ���������� ������� �������������� ���������
    sParam.prm.status.faultText[0] = fcPrmFault0001rzsk;
    sParam.prm.status.faultText[1] = fcPrmFault0002rzsk;
    sParam.prm.status.faultText[2] = fcPrmFault0004rzsk;
    sParam.prm.status.faultText[3] = fcPrmFault0008k400;
    // 4-7 ���
    sParam.prm.status.faultText[8]  = fcPrmFault0100rzsk;
    sParam.prm.status.faultText[9]  = fcPrmFault0200rzsk;
    sParam.prm.status.faultText[10] = fcPrmFault0400rzsk;
    sParam.prm.status.faultText[11] = fcPrmFault0800rzsk;
    // 12-15 ���
    // ���������� ������� �������������� ���������
    sParam.prm.status.warningText[0] = fcPrmWarning01rzsk;
    sParam.prm.status.warningText[1] = fcPrmWarning02k400;
    sParam.prm.status.warningText[2] = fcPrmWarning04k400;
    sParam.prm.status.warningText[3] = fcPrmWarning08k400;
    // 4-15 ���

    // ����������
    // ���������� ������� �������������� �����������
    sParam.prd.status.faultText[0] = fcPrdFault0001rzsk;
    sParam.prd.status.faultText[1] = fcPrdFault0002rzsk;
    // 2-7 ���
    sParam.prd.status.faultText[8]  = fcPrdFault0100rzsk;
    sParam.prd.status.faultText[9]  = fcPrdFault0200rzsk;
    sParam.prd.status.faultText[10] = fcPrdFault0400rzsk;
    sParam.prd.status.faultText[11] = fcPrdFault0800rzsk;
    // 12-15 ���
    // ���������� ������� �������������� �����������
    // 0 ���
    sParam.prd.status.warningText[1] = fcPrdWarning02k400;
    sParam.prd.status.warningText[2] = fcPrdWarning04k400;
    // 3-15 ���

    return true;
}

/** ���������� �������� ��� ����.
 *  @retval True - ������.
 */
bool clMenu::setDeviceRZSK()
{
    sParam.typeDevice = AVANT_RZSK;
    sParam.glb.setTypeDevice(AVANT_RZSK);

    sParam.prm.status.setEnable(sParam.prm.getNumCom() != 0);
    sParam.prd.status.setEnable(sParam.prd.getNumCom() != 0);

    // ���������
    sParam.def.status.stateText[14] = fcDefSost14rzsk;
    sParam.prm.status.stateText[14] = fcPrmSost14rzsk;

    if (sParam.def.status.isEnable())
    {
        measParam[0]                  = MENU_MEAS_PARAM_TIME;  // ���� <-> �����
        measParam[MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;
        // ������ ������� ����������
        if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3)
        {
            measParam[1]                      = MENU_MEAS_PARAM_UZ1;
            measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UZ2;
        }
        else
        {
            measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UZ;
        }
    }
    else
    {
        measParam[0] = measParam[MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_TIME;
        measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;
    }

    if (sParam.prd.status.isEnable() || sParam.def.status.isEnable())
    {
        measParam[2] = measParam[2 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UOUT;
        measParam[4] = measParam[4 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_IOUT;
    }

    if (sParam.prm.status.isEnable() || sParam.def.status.isEnable())
    {
        // ������ ������� ����������
        if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3)
        {
            measParam[3]                      = MENU_MEAS_PARAM_UC1;
            measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC2;
            measParam[5]                      = MENU_MEAS_PARAM_UN1;
            measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN2;
        }
        else
        {
            measParam[3] = measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC;
            measParam[5] = measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN;
        }
    }

    // ���������� ������� ���������� ��� ���� "���������"
    uint8_t cnt = 0;
    if (sParam.prd.status.isEnable() || sParam.def.status.isEnable())
    {
        measParamLvl[cnt++] = MENU_MEAS_PARAM_UOUT;  // 1
        measParamLvl[cnt++] = MENU_MEAS_PARAM_IOUT;  // 2
    }
    if (sParam.def.status.isEnable())
    {
        if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3)
        {
            measParamLvl[cnt++] = MENU_MEAS_PARAM_UZ1;  // 3
            measParamLvl[cnt++] = MENU_MEAS_PARAM_UZ2;  // 4
        }
        else
        {
            measParamLvl[cnt++] = MENU_MEAS_PARAM_UZ;  // 3
        }
    }
    if (sParam.prm.status.isEnable() || sParam.def.status.isEnable())
    {
        if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3)
        {
            measParamLvl[cnt++] = MENU_MEAS_PARAM_UC1;  // 5
            measParamLvl[cnt++] = MENU_MEAS_PARAM_UC2;  // 6
            measParamLvl[cnt++] = MENU_MEAS_PARAM_UN1;  // 7
            measParamLvl[cnt++] = MENU_MEAS_PARAM_UN2;  // 8
        }
        else
        {
            measParamLvl[cnt++] = MENU_MEAS_PARAM_UC;  // 4
            measParamLvl[cnt++] = MENU_MEAS_PARAM_UN;  // 5
        }
    }
    measParamLvl[cnt++] = MENU_MEAS_PARAM_TEMPERATURE;  // 9 ��� 6

    // ���������� ������� ����� ��������������
    sParam.glb.status.faultText[0] = fcGlbFault0001;
    sParam.glb.status.faultText[1] = fcGlbFault0002;
    sParam.glb.status.faultText[2] = fcGlbFault0004;
    sParam.glb.status.faultText[3] = fcGlbFault0008;
    sParam.glb.status.faultText[4] = fcGlbFault0010;
    // 5-7 ���
    sParam.glb.status.faultText[8] = fcGlbFault0100;
    sParam.glb.status.faultText[9] = fcGlbFault0200;
    // 10 ���
    sParam.glb.status.faultText[11] = fcGlbFault0800;
    sParam.glb.status.faultText[12] = fcGlbFault1000;
    // 13-15 ���
    // ���������� ������� ����� ��������������
    // 0-9 ���
    sParam.glb.status.warningText[10] = fcGlbWarning01;
    // 11-15 ���

    // ������
    // ���������� ������� �������������� ������
    sParam.def.status.faultText[0] = fcDefFault0001;
    sParam.def.status.faultText[1] = fcDefFault0002;
    sParam.def.status.faultText[2] = fcDefFault0004;
    sParam.def.status.faultText[3] = fcDefFault0008rzskm;
    // 4-7
    sParam.def.status.faultText[8] = fcDefFault0100;
    sParam.def.status.faultText[9] = fcDefFault0200;
    // 10 ���
    sParam.def.status.faultText[11] = fcDefFault0800;
    // 12 ���
    sParam.def.status.faultText[13] = fcDefFault2000;
    sParam.def.status.faultText[14] = fcDefFault4000rzsk;
    sParam.def.status.faultText[15] = fcDefFault8000rzsk;

    // ���������� ������� �������������� ������
    sParam.def.status.warningText[0] = fcDefWarning01rzsk;
    sParam.def.status.warningText[1] = fcDefWarning02;
    sParam.def.status.warningText[2] = fcDefWarning04rzskm;
    // 3-15 ���

    // ��������
    // ���������� ������� �������������� ���������
    sParam.prm.status.faultText[0] = fcPrmFault0001rzsk;
    sParam.prm.status.faultText[1] = fcPrmFault0002rzsk;
    sParam.prm.status.faultText[2] = fcPrmFault0004rzsk;
    // 3-7 ���
    sParam.prm.status.faultText[8]  = fcPrmFault0100rzsk;
    sParam.prm.status.faultText[9]  = fcPrmFault0200rzsk;
    sParam.prm.status.faultText[10] = fcPrmFault0400rzsk;
    sParam.prm.status.faultText[11] = fcPrmFault0800rzsk;
    // 12-15 ���

    // ���������� ������� �������������� ���������
    sParam.prm.status.warningText[0] = fcPrmWarning01rzsk;
    // 1-15 ���

    // ����������
    // ���������� ������� �������������� �����������
    sParam.prd.status.faultText[0] = fcPrdFault0001rzsk;
    sParam.prd.status.faultText[1] = fcPrdFault0002rzsk;
    // 2-7 ���
    sParam.prd.status.faultText[8]  = fcPrdFault0100rzsk;
    sParam.prd.status.faultText[9]  = fcPrdFault0200rzsk;
    sParam.prd.status.faultText[10] = fcPrdFault0400rzsk;
    sParam.prd.status.faultText[11] = fcPrdFault0800rzsk;
    // 12-15 ���

    // ���������� ������� �������������� �����������
    // 0-15 ���

    return true;
}

/** ���������� �������� ��� �400�.
 *  @retval True - ������.
 */
bool clMenu::setDeviceR400M()
{

    sParam.typeDevice = AVANT_R400M;
    sParam.glb.setTypeDevice(AVANT_R400M);

    // ������ ������� ����������
    measParam[0]                  = MENU_MEAS_PARAM_TIME;  // ���� <-> �����
    measParam[MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;
    measParam[2] = measParam[2 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UOUT;
    measParam[4] = measParam[4 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_IOUT;

    // ������ ������� ����������
    measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UZ;
    if (sParam.glb.getNumDevices() == GB_NUM_DEVICES_3)
    {
        measParam[3]                      = MENU_MEAS_PARAM_UC1;
        measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC2;
    }
    else
    {
        measParam[3] = measParam[3 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UC;
    }
    measParam[5]                      = MENU_MEAS_PARAM_SD;
    measParam[5 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_UN;

    // ���������� ������� ���������� ��� ���� "���������"
    uint8_t cnt         = 0;
    measParamLvl[cnt++] = MENU_MEAS_PARAM_UOUT;  // 1
    measParamLvl[cnt++] = MENU_MEAS_PARAM_IOUT;  // 2
    measParamLvl[cnt++] = MENU_MEAS_PARAM_UZ;    // 3
    if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3)
    {
        measParamLvl[cnt++] = MENU_MEAS_PARAM_UC1;  // 4
        measParamLvl[cnt++] = MENU_MEAS_PARAM_UC2;  // 5
    }
    else
    {
        measParamLvl[cnt++] = MENU_MEAS_PARAM_UC;  // 4
    }
    measParamLvl[cnt++] = MENU_MEAS_PARAM_SD;           // 6 ��� 5
    measParamLvl[cnt++] = MENU_MEAS_PARAM_UN;           // 7 ��� 6
    measParamLvl[cnt++] = MENU_MEAS_PARAM_TEMPERATURE;  // 8 ��� 7

    // ���������� ������� ����� ��������������
    sParam.glb.status.faultText[0]  = fcGlbFault0001;
    sParam.glb.status.faultText[1]  = fcGlbFault0002;
    sParam.glb.status.faultText[2]  = fcGlbFault0004;
    sParam.glb.status.faultText[3]  = fcGlbFault0008;
    sParam.glb.status.faultText[4]  = fcGlbFault0010;
    sParam.glb.status.faultText[5]  = fcGlbFault0020;
    sParam.glb.status.faultText[6]  = fcGlbFault0040;
    sParam.glb.status.faultText[7]  = fcGlbFault0080;
    sParam.glb.status.faultText[8]  = fcGlbFault0100;
    sParam.glb.status.faultText[9]  = fcGlbFault0200;
    sParam.glb.status.faultText[10] = fcGlbFault0400;
    sParam.glb.status.faultText[11] = fcGlbFault0800;
    sParam.glb.status.faultText[12] = fcGlbFault1000;
    sParam.glb.status.faultText[13] = fcGlbFault2000;
    sParam.glb.status.faultText[14] = fcGlbFault4000;
    // 15 ���
    // ���������� ������� ����� ��������������
    sParam.glb.status.warningText[0] = fcGlbWarning01;
    // 1-15 - ���

    // ������
    // ���������� ������� �������������� ������
    sParam.def.status.faultText[0] = fcDefFault0001;
    sParam.def.status.faultText[1] = fcDefFault0002;
    sParam.def.status.faultText[2] = fcDefFault0004;
    sParam.def.status.faultText[3] = fcDefFault0008r400m;
    sParam.def.status.faultText[4] = fcDefFault0010;
    // 5 ���
    sParam.def.status.faultText[6]  = fcDefFault0040;
    sParam.def.status.faultText[7]  = fcDefFault0080;
    sParam.def.status.faultText[8]  = fcDefFault0100;
    sParam.def.status.faultText[9]  = fcDefFault0200;
    sParam.def.status.faultText[10] = fcDefFault0400;
    sParam.def.status.faultText[11] = fcDefFault0800;
    sParam.def.status.faultText[12] = fcDefFault1000;
    sParam.def.status.faultText[13] = fcDefFault2000;
    sParam.def.status.faultText[14] = fcDefFault4000;
    sParam.def.status.faultText[15] = fcDefFault8000;
    // ���������� ������� �������������� ������
    sParam.def.status.warningText[0] = fcDefWarning01;
    sParam.def.status.warningText[1] = fcDefWarning02;
    sParam.def.status.warningText[2] = fcDefWarning04;
    sParam.def.status.warningText[3] = fcDefWarning08;
    sParam.def.status.warningText[4] = fcDefWarning10r400m;
    sParam.def.status.warningText[5] = fcDefWarning20r400m;
    // 6-15 ���

    // ���������� ���������
    sParam.prm.status.setEnable(false);
    // ���������� �����������
    sParam.prd.status.setEnable(false);

    return true;
}

/** ���������� �������� ��� ������.
 *  @retval True - ������.
 */
bool clMenu::setDeviceOPTO()
{

    sParam.typeDevice = AVANT_OPTO;
    sParam.glb.setTypeDevice(AVANT_OPTO);

    sParam.prm.status.setEnable(sParam.prm.getNumCom() != 0);
    sParam.prd.status.setEnable(sParam.prd.getNumCom() != 0);

    // ���������
    sParam.def.status.stateText[9] = fcDefSost09opto;

    sParam.prd.status.stateText[1] = fcPrdSost01opto;
    sParam.prd.status.stateText[3] = fcPrdSost03opto;

    sParam.prm.status.stateText[1] = fcPrmSost01opto;
    sParam.prm.status.stateText[3] = fcPrmSost03opto;

    measParam[0] = measParam[0 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_TIME;
    measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;

    // ���������� ������� ����� ��������������
    sParam.glb.status.faultText[0] = fcGlbFault0001;
    sParam.glb.status.faultText[1] = fcGlbFault0002;
    sParam.glb.status.faultText[2] = fcGlbFault0004;
    sParam.glb.status.faultText[3] = fcGlbFault0008;
    sParam.glb.status.faultText[4] = fcGlbFault0010;
    // 5-8 ���
    sParam.glb.status.faultText[9] = fcGlbFault0200;
    // 10-15 ���
    // ���������� ������� ����� ��������������
    sParam.glb.status.warningText[0] = fcGlbWarning01;
    sParam.glb.status.warningText[1] = fcGlbWarning02;
    if (sParam.glb.getTypeOpto() == TYPE_OPTO_RING_UNI)
    {                                                            // �400 ������
        sParam.glb.status.warningText[2] = fcGlbWarning04ring1;  // ������ ��������
        sParam.glb.status.warningText[3] = fcGlbWarning08ring1;  // ������ ��������
    }
    else
    {
        sParam.glb.status.warningText[2] = fcGlbWarning04;
        sParam.glb.status.warningText[3] = fcGlbWarning08;
    }
    sParam.glb.status.warningText[4] = fcGlbWarning10;
    sParam.glb.status.warningText[5] = fcGlbWarning20;
    sParam.glb.status.warningText[6] = fcGlbWarning40;
    // 7 ���
    if (sParam.glb.getTypeOpto() == TYPE_OPTO_RING_UNI)
    {
        sParam.glb.status.warningText[8] = fcGlbWarning100ring1;
        sParam.glb.status.warningText[9] = fcGlbWarning200ring1;
        // 10-15 ���
    }
    else
    {
        sParam.glb.status.warningText[8] = fcGlbWarning100;
        // 9-15 ���
    }

    // ������
    // ���������� ������� �������������� ������
    sParam.def.status.faultText[0] = fcDefFault0001;
    sParam.def.status.faultText[1] = fcDefFault0002;
    sParam.def.status.faultText[2] = fcDefFault0004;
    // 3
    sParam.def.status.faultText[4] = fcDefFault0010opto;
    // 5-7 ���
    sParam.def.status.faultText[8] = fcDefFault0100;
    sParam.def.status.faultText[9] = fcDefFault0200;
    // 10 ���
    sParam.def.status.faultText[11] = fcDefFault0800;
    // 12 ���
    sParam.def.status.faultText[13] = fcDefFault2000;
    // 14-15 ���
    // ���������� ������� �������������� ������
    sParam.def.status.warningText[0] = fcDefWarning01opto;
    sParam.def.status.warningText[1] = fcDefWarning02;
    // 2-15 ���

    // ��������
    // ���������� ������� �������������� ���������
    sParam.prm.status.faultText[0] = fcPrmFault0001rzsk;
    sParam.prm.status.faultText[1] = fcPrmFault0002rzsk;
    sParam.prm.status.faultText[2] = fcPrmFault0004opto;
    // 3-7 ���
    sParam.prm.status.faultText[8]  = fcPrmFault0100rzsk;
    sParam.prm.status.faultText[9]  = fcPrmFault0200rzsk;
    sParam.prm.status.faultText[10] = fcPrmFault0400rzsk;
    sParam.prm.status.faultText[11] = fcPrmFault0800rzsk;
    // 12-15 ���
    // ���������� ������� �������������� ������
    sParam.prm.status.warningText[0] = fcPrmWarning01opto;
    // 1-15 ���

    // ����������
    // ���������� ������� �������������� �����������
    sParam.prd.status.faultText[0] = fcPrdFault0001rzsk;
    sParam.prd.status.faultText[1] = fcPrdFault0002rzsk;
    // 2-7 ���
    sParam.prd.status.faultText[8]  = fcPrdFault0100rzsk;
    sParam.prd.status.faultText[9]  = fcPrdFault0200rzsk;
    sParam.prd.status.faultText[10] = fcPrdFault0400rzsk;
    sParam.prd.status.faultText[11] = fcPrdFault0800rzsk;
    // 12-15 ���
    // ���������� ������� �������������� �����������
    // 0-15 ���

    return true;
}


bool clMenu::setDevice(eGB_TYPE_DEVICE device)
{
    static const char fcUnknownFault[] PROGMEM   = "�������������";
    static const char fcUnknownWarning[] PROGMEM = "��������������";

    bool status = false;

    sParam.glb.status.setEnable(true);

    // ���� ����������� ��� �������� ����� �������, ������� ����� ������
    // �� ������� ��������.
    //  if (device == AVANT_NO) {
    device = sParam.glb.getTypeDevice();
    if (device == AVANT_NO)
    {
        bool isPrm = sParam.prm.status.isEnable();
        bool isPrd = sParam.prd.status.isEnable();

        if (sParam.glb.getTypeLine() == GB_TYPE_LINE_UM)
        {
            // �� �������
            // ���� ���� ������ + ������� (��� �/��� ���) - ����
            // ���� ���� ������ ������� (��� �/��� ���) - �400
            // ���� ���� ������ � (������ �������� & 0xF000) = 0xF000 - �400�

            if (sParam.def.status.isEnable())
            {
                if (isPrm || isPrd)
                {
                    device = AVANT_RZSK;
                }
                else
                {
                    uint16_t vers = sParam.glb.getVersProgIC(GB_IC_BSP_MCU);
                    if ((vers & 0xF000) == 0xF000)
                    {
                        device = AVANT_R400M;
                    }
                }
            }
            else
            {
                if (isPrm || isPrd)
                {
                    device = AVANT_K400;
                }
            }
        }
        else if (sParam.glb.getTypeLine() == GB_TYPE_LINE_OPTO)
        {
            device = AVANT_OPTO;
        }
    }

    // ���� ������� ���������� ��������� � �����, �� ������ �� ������
    // ����� ������� �� ��������� �������
    if (device == sParam.typeDevice)
    {
        status = true;
    }
    else
    {
        sParam.typeDevice = device;
        lvlMenu           = &clMenu::lvlStart;
        lvlCreate_        = true;
    }

    //  if (!status) {
    // ��������������� "�������" �������� ��������������
    for (uint_fast8_t i = 0; i < MAX_NUM_FAULTS; i++)
        sParam.glb.status.faultText[i] = fcUnknownFault;
    for (uint_fast8_t i = 0; i < MAX_NUM_FAULTS; i++)
        sParam.def.status.faultText[i] = fcUnknownFault;
    for (uint_fast8_t i = 0; i < MAX_NUM_FAULTS; i++)
        sParam.prm.status.faultText[i] = fcUnknownFault;
    for (uint_fast8_t i = 0; i < MAX_NUM_FAULTS; i++)
        sParam.prd.status.faultText[i] = fcUnknownFault;

    // ��������������� "�������" �������� ��������������
    for (uint_fast8_t i = 0; i < MAX_NUM_WARNINGS; i++)
        sParam.glb.status.warningText[i] = fcUnknownWarning;
    for (uint_fast8_t i = 0; i < MAX_NUM_WARNINGS; i++)
        sParam.def.status.warningText[i] = fcUnknownWarning;
    for (uint_fast8_t i = 0; i < MAX_NUM_WARNINGS; i++)
        sParam.prm.status.warningText[i] = fcUnknownWarning;
    for (uint_fast8_t i = 0; i < MAX_NUM_WARNINGS; i++)
        sParam.prd.status.warningText[i] = fcUnknownWarning;

    // ��������������� ������� ������� ������������ ����������
    for (uint_fast8_t i = 0; i < (MAX_NUM_MEAS_PARAM * 2); i++)
        measParam[i] = MENU_MEAS_PARAM_NO;

    // ��������������� ������� ������� ������������ ���������� � "���������"
    for (uint_fast8_t i = 0; i < MAX_NUM_MEAS_PARAM_LVL; i++)
        measParamLvl[i] = MENU_MEAS_PARAM_NO;

    // ��������� ������� ����� ���������� � �����������
    sParam.def.status.stateText[9]  = fcDefSost09;
    sParam.def.status.stateText[14] = fcDefSost14;

    sParam.prd.status.stateText[1] = fcPrdSost01;
    sParam.prd.status.stateText[3] = fcPrdSost03;

    sParam.prm.status.stateText[1]  = fcPrmSost01;
    sParam.prm.status.stateText[3]  = fcPrmSost03;
    sParam.prm.status.stateText[14] = fcPrmSost14;

    if (device == AVANT_K400)
    {
        status = setDeviceK400();
    }
    else if (device == AVANT_RZSK)
    {
        status = setDeviceRZSK();
    }
    else if (device == AVANT_R400M)
    {
        status = setDeviceR400M();
    }
    else if (device == AVANT_OPTO)
    {
        status = setDeviceOPTO();
    }
    else
    {
        //  if ((!status) || (device == AVANT_NO)) {
        // ���� ���������� ������ �� �������� �� ��� ���� ��������� ���
        // �� ������ ������������ ������
        sParam.typeDevice = AVANT_NO;
        // � ������ ������������ ���� ����������, �������� ���
        for (uint_fast8_t i = 0; i < (MAX_NUM_MEAS_PARAM * 2); i++)
            measParam[i] = MENU_MEAS_PARAM_NO;
        measParam[0] = measParam[0 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_TIME;
        measParam[1] = measParam[1 + MAX_NUM_MEAS_PARAM] = MENU_MEAS_PARAM_DATE;

        sParam.def.status.setEnable(false);
        sParam.prm.status.setEnable(false);
        sParam.prd.status.setEnable(false);

        lvlMenu = &clMenu::lvlError;
    }

    vKEYset(getKeyboardLayout());

    // "�����" ����� ������������� �������� ���� ��������
    sParam.device = true;
    // ���������� �������� ������ ����
    lvlCreate_ = true;

    return status;
}

// ���������� ��������� ������� �� ����������.
eGB_COM clMenu::getTxCommand()
{
    static uint8_t cnt = 0;

    // ������� ������� ���� � ����� ������� �����������
    eGB_COM com = sParam.txComBuf.getFastCom();

    while (com == GB_COM_NO)
    {
        if (cnt == 0)
        {
            com = GB_COM_GET_SOST;
        }
        else if (cnt == 1)
        {
            com = GB_COM_GET_TIME;
        }
        else
        {
            // ������� ������� ������ ������������ ������� ��� � �������
            com = sParam.txComBuf.getCom2();
        }

        if (com == GB_COM_NO)
        {
            // ������� ������� ������ ������������ ������������
            com = sParam.txComBuf.getCom1();
            cnt = 0;
        }
        else
        {
            cnt++;
        }
    }

    return com;
}

/** ������� ���������� ������
 *  @param ���
 *  @return ���
 */
void clMenu::clearTextBuf()
{
    for (uint_fast8_t i = 0; i < sizeof(vLCDbuf); i++)
        vLCDbuf[i] = ' ';
}

/** ������� ������
 *  @param line ����� ������ 1..NUM_TEXT_LINES
 *  @retrun ���
 */
void clMenu::clearLine(uint8_t line)
{
    if ((line > 0) && (line <= NUM_TEXT_LINES))
    {
        line = (line - 1) * 20;
        for (uint_fast8_t i = 0; i < 20; i++)
            vLCDbuf[line++] = ' ';
    }
}

/** ������� "��������� ��� ��������"
 *  ���������� �� ����� ��������� ������� "�������������". �� ���� ��� ��������
 *  �� ����� ��������� n-�� ���-�� �������, �� ������� �������� ��
 *  "��� �������� �� ���������!!!"
 *  @param ���
 *  @return ���
 */
void clMenu::lvlError()
{
    static uint8_t time = 0;

    static const char fcNoTypeDevice0[] PROGMEM = "    ��� ��������    ";
    static const char fcNoTypeDevice1[] PROGMEM = "   �� ���������!!!  ";
    static const char fcNoTypeDevice3[] PROGMEM = "    �������������   ";

    if (lvlCreate_)
    {
        lvlCreate_    = false;
        cursorEnable_ = false;
        vLCDclear();
        // ������ ���� ������ ��������� ��� ����� ����������
        lineParam_ = 1;
        vLCDclear();
        vLCDdrawBoard(lineParam_);
        sParam.txComBuf.clear();
        sParam.txComBuf.addCom2(GB_COM_GET_VERS);
    }

    // ����� �� ����� ���������� ����������
    printMeasParam(0, measParam[0]);
    printMeasParam(1, measParam[1]);

    // �������� ������� ���������� � ����������� ��������� ���� ��������
    if (time >= 25)
    {
        snprintf_P(&vLCDbuf[40], 21, fcNoTypeDevice0);
        snprintf_P(&vLCDbuf[60], 21, fcNoTypeDevice1);
    }
    else
    {
        snprintf_P(&vLCDbuf[40], 21, fcNoTypeDevice3);
    }
    time++;

    if (sParam.typeDevice != AVANT_NO)
    {
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        time       = 0;
    }
}

/** ������� ���������
 *  @param ���
 *  @return ���
 */
void clMenu::lvlStart()
{

    if (lvlCreate_)
    {
        lvlCreate_ = false;

        cursorEnable_ = false;
        lineParam_    = 3;
        vLCDclear();
        vLCDdrawBoard(lineParam_);

        sParam.txComBuf.clear();
        // ����� 2
        // �������������� �������
        // ����� ���������� ��������� (� ��)
        sParam.txComBuf.addCom2(GB_COM_GET_FAULT);
        if (sParam.glb.getTypeLine() == GB_TYPE_LINE_UM)
            sParam.txComBuf.addCom2(GB_COM_GET_MEAS);
        if (sParam.typeDevice == AVANT_R400M)
        {
            sParam.txComBuf.addCom2(GB_COM_DEF_GET_TYPE_AC);
        }
        // ����� 1
        // ������������� + ���-�� ��������� � ����� + ����� ��������
        // + ������� ����� + �������
        // � �400� + �� + �������������
        // � �400 + �������������
        // � ���� + �������������
        sParam.txComBuf.addCom1(GB_COM_DEF_GET_LINE_TYPE);
        sParam.txComBuf.addCom1(GB_COM_GET_DEVICE_NUM);
        sParam.txComBuf.addCom1(GB_COM_GET_NET_ADR);
        if (sParam.typeDevice != AVANT_OPTO)
        {
            sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);
            sParam.txComBuf.addCom2(GB_COM_GET_TIME);
        }
        sParam.txComBuf.addCom1(GB_COM_GET_JRN_CNT);
        if (sParam.def.status.isEnable())
        {
            sParam.txComBuf.addCom1(GB_COM_DEF_GET_JRN_CNT);
        }
        if (sParam.prd.status.isEnable())
        {
            sParam.txComBuf.addCom1(GB_COM_PRD_GET_JRN_CNT);
        }
        if (sParam.prm.status.isEnable())
        {
            sParam.txComBuf.addCom1(GB_COM_PRM_GET_JRN_CNT);
        }
    }

    // ����� �� ����� ���������� ����������
    for (uint_fast8_t i = 0; i < (lineParam_ * 2); i++)
    {
        if (i >= MAX_NUM_MEAS_PARAM)
            continue;

        if (blinkMeasParam_)
            printMeasParam(i, measParam[i]);
        else
            printMeasParam(i, measParam[i + MAX_NUM_MEAS_PARAM]);
    }

    //  uint16_t val = sParam.glb.status.getWarnings();

    uint8_t poz = lineParam_ * 20;
    if (sParam.def.status.isEnable())
    {
        printDevicesStatus(poz, &sParam.def.status);
        poz += 20;

        // � �400� ��������� �� � ����� �� ��, � ����� ��� �������������
        if (sParam.typeDevice == AVANT_R400M)
        {
            eGB_COMP_R400M comp = sParam.glb.getCompR400m();

            // ���� �������� � �������������, ������� ��� �� �����
            if (comp != GB_COMP_R400M_AVANT)
            {
                uint8_t len = snprintf_P(&vLCDbuf[poz], 21, PSTR("���������. "));

                if (comp < GB_COMP_R400M_MAX)
                {
                    snprintf_P(&vLCDbuf[poz + len], 21 - len, fcCompR400m[comp]);
                }
                else
                {
                    snprintf_P(&vLCDbuf[poz + len], 21 - len, PSTR("������"));
                }
            }

            printAc(poz + 20);
        }
    }

    if (sParam.prm.status.isEnable())
    {
        printDevicesStatus(poz, &sParam.prm.status);
        poz += 20;
    }

    if (sParam.prd.status.isEnable())
    {
        printDevicesStatus(poz, &sParam.prd.status);
    }

    if (key_ != KEY_NO)
    {
        // TODO ������������ ����� ����� �������������� �������� "�� + ������"!
        // �������������� �������� ������� ������ + "��" � ��������� ����
        TControl::ctrl_t ctrl = onFnButton(key_);
        AddControlToSend(ctrl);

        if (key_ == KEY_MENU)
        {
            lvlMenu    = &clMenu::lvlFirst;
            lvlCreate_ = true;
        }
    }
}

/** ������� ���� ������
 *  @param ���
 *  @return ���
 */
void clMenu::lvlFirst()
{
    static const char title[] PROGMEM  = "����";
    static const char punkt1[] PROGMEM = "������";
    static const char punkt2[] PROGMEM = "����������";
    static const char punkt3[] PROGMEM = "���������";
    static const char punkt4[] PROGMEM = "�����";
    static const char punkt5[] PROGMEM = "����������";
    static const char punkt6[] PROGMEM = "���������";

    if (lvlCreate_)
    {
        lvlCreate_ = false;

        cursorLine_   = 1;
        lineParam_    = 1;
        cursorEnable_ = true;

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        Punkts_.Clear();
        Punkts_.Add(punkt1);
        Punkts_.Add(punkt2);
        Punkts_.Add(punkt3);
        Punkts_.Add(punkt4);
        Punkts_.Add(punkt5);

        if (sParam.glb.getTypeDevice() != AVANT_OPTO)
        {
            Punkts_.Add(punkt6);
        }

        // ������������� �������
        sParam.txComBuf.clear();

        if (sParam.typeDevice == AVANT_RZSK)
        {
            sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);
        }
    }

    snprintf_P(&vLCDbuf[0], 21, title);

    PGM_P name = Punkts_.GetName(cursorLine_ - 1);
    printPunkts();

    switch (key_)
    {
    case KEY_UP: cursorLineUp(); break;
    case KEY_DOWN: cursorLineDown(); break;

    case KEY_ENTER:
        if (name == punkt1)
        {
            lvlMenu    = &clMenu::lvlJournal;
            lvlCreate_ = true;
        }
        else if (name == punkt2)
        {
            lvlMenu    = &clMenu::lvlControl;
            lvlCreate_ = true;
        }
        else if (name == punkt3)
        {
            lvlMenu    = &clMenu::lvlSetup;
            lvlCreate_ = true;
        }
        else if (name == punkt4)
        {
            lvlMenu    = &clMenu::lvlTest;
            lvlCreate_ = true;
        }
        else if (name == punkt5)
        {
            lvlMenu    = &clMenu::lvlInfo;
            lvlCreate_ = true;
        }
        else if (name == punkt6)
        {
            lvlMenu    = &clMenu::lvlMeasure;
            lvlCreate_ = true;
        }
        break;

    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    default: break;
    }
}

/** ������� ����. ���������� �� ��������.
 *  @param ���
 *  @return ���
 */
void clMenu::lvlInfo()
{
    static const char title[] PROGMEM = "����\\����������";

    if (lvlCreate_)
    {
        lvlCreate_  = false;
        lineParam_  = 1;
        cursorLine_ = 0;

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        Punkts_.Clear();
        Punkts_.Add(nullptr, GB_IC_BSP_MCU);
        if (sParam.typeDevice != AVANT_OPTO)
        {
            Punkts_.Add(nullptr, GB_IC_BSP_DSP);  // ������ � ������ ��� DSP

            if (sParam.typeDevice == AVANT_K400)
            {  // � �400 �������� �������� DSP
                Punkts_.Add(nullptr, GB_IC_BSP_DSP_PLIS);
            }
        }
        Punkts_.Add(nullptr, GB_IC_PI_MCU);
        if (sParam.prd.status.isEnable())
        {
            Punkts_.Add(nullptr, GB_IC_BSK_PLIS_PRD1);
            if (sParam.prd.getNumCom() > 16)
            {
                Punkts_.Add(nullptr, GB_IC_BSK_PLIS_PRD2);
            }
        }
        if (sParam.prm.status.isEnable())
        {
            Punkts_.Add(nullptr, GB_IC_BSK_PLIS_PRM1);
            if (sParam.prm.getNumCom() > 16)
            {
                Punkts_.Add(nullptr, GB_IC_BSK_PLIS_PRM2);
            }
        }
        if (sParam.def.status.isEnable())
        {
            Punkts_.Add(nullptr, GB_IC_BSZ_PLIS);
        }

        // ������������� �������
        // ����������� ������ �������� (�� ������ ������������)
        sParam.txComBuf.clear();
        sParam.txComBuf.addCom2(GB_COM_GET_VERS);
    }

    snprintf_P(&vLCDbuf[0], 21, title);

    uint8_t numLines  = NUM_TEXT_LINES - lineParam_;
    uint8_t cntPunkts = cursorLine_;
    for (uint_fast8_t line = lineParam_; line < NUM_TEXT_LINES; line++)
    {
        uint8_t pos = 20 * line;
        eGB_IC  ic  = static_cast<eGB_IC>(Punkts_.GetData(cntPunkts));

        uint8_t  len  = snprintf_P(&vLCDbuf[pos], DISPLAY_ROW_SIZE + 1, fcIC[ic]);
        uint16_t vers = sParam.glb.getVersProgIC(ic);
        snprintf_P(&vLCDbuf[pos + len],
                   DISPLAY_ROW_SIZE - len + 1,
                   PSTR(": %02X.%02X"),
                   static_cast<uint8_t>(vers >> 8),
                   static_cast<uint8_t>(vers));

        if (++cntPunkts >= Punkts_.GetLen())
        {
            break;
        }
    }

    switch (key_)
    {
    case KEY_UP:
        if (cursorLine_ > 0)
        {
            cursorLine_--;
        }
        break;
    case KEY_DOWN:
        if ((cursorLine_ + numLines) < Punkts_.GetLen())
        {
            cursorLine_++;
        }
        break;

    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlFirst;
        lvlCreate_ = true;
        break;

    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    default: break;
    }
}

/** ������� ����. �������.
 *  @param ���
 *  @return ���
 */
void clMenu::lvlJournal()
{
    static const char title[] PROGMEM  = "����\\������";
    static const char punkt1[] PROGMEM = "�������";
    static const char punkt2[] PROGMEM = "������";
    static const char punkt3[] PROGMEM = "��������";
    static const char punkt4[] PROGMEM = "����������";

    if (lvlCreate_)
    {
        lvlCreate_    = false;
        cursorLine_   = 1;
        cursorEnable_ = true;
        lineParam_    = 1;

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        sParam.txComBuf.clear();

        // ��������� ����������� ������� ���� � ��������������� �� ������
        Punkts_.Clear();
        Punkts_.Add(punkt1);

        if (sParam.def.status.isEnable())
        {
            Punkts_.Add(punkt2);
        }

        if (sParam.prm.status.isEnable())
        {
            Punkts_.Add(punkt3);
        }

        if (sParam.prd.status.isEnable())
        {
            Punkts_.Add(punkt4);
        }
    }

    PGM_P name = Punkts_.GetName(cursorLine_ - 1);

    snprintf_P(&vLCDbuf[0], 21, title);
    printPunkts();

    switch (key_)
    {
    case KEY_UP: cursorLineUp(); break;
    case KEY_DOWN: cursorLineDown(); break;

    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlFirst;
        lvlCreate_ = true;
        break;
    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    case KEY_ENTER:
        if (name == punkt1)
        {
            lvlMenu    = &clMenu::lvlJournalEvent;
            lvlCreate_ = true;
        }
        else if (name == punkt2)
        {
            lvlMenu    = &clMenu::lvlJournalDef;
            lvlCreate_ = true;
        }
        else if (name == punkt3)
        {
            lvlMenu    = &clMenu::lvlJournalPrm;
            lvlCreate_ = true;
        }
        else if (name == punkt4)
        {
            lvlMenu    = &clMenu::lvlJournalPrd;
            lvlCreate_ = true;
        }
        break;

    default: break;
    }
}

/** ������� ����. ������ �������.
 *  @param ���
 *  @return ���
 */
void clMenu::lvlJournalEvent()
{
    static const char title[] PROGMEM = "������\\�������";

    eGB_TYPE_DEVICE device = sParam.typeDevice;

    if (lvlCreate_)
    {
        lvlCreate_    = false;
        cursorEnable_ = false;
        lineParam_    = 1;
        curCom_       = 1;

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        // ��������� �������� ������� � ������������� ���-�� ������� � ���
        sParam.jrnEntry.clear();
        sParam.jrnEntry.setCurrentDevice(GB_DEVICE_GLB);
        uint16_t t = 0;

        if (device == AVANT_K400)
        {
            t = GLB_JRN_EVENT_K400_MAX;
        }
        else if (device == AVANT_R400M)
        {
            t = GLB_JRN_EVENT_R400M_MAX;
        }
        else if (device == AVANT_RZSK)
        {
            t = GLB_JRN_EVENT_RZSK_MAX;
        }
        else if (device == AVANT_OPTO)
        {
            t = GLB_JRN_EVENT_OPTO_MAX;
        }
        sParam.jrnEntry.setMaxNumJrnEntries(t);

        // ������������� �������
        sParam.txComBuf.clear();
        sParam.txComBuf.addCom1(GB_COM_GET_JRN_CNT);
        sParam.txComBuf.addCom2(GB_COM_GET_JRN_ENTRY);
        sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());

        if (device == AVANT_R400M)
        {
            // � �400� ������ � ������� ������� ������� �� �������������
            sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);
        }
    }

    // ����� ������� ������ � ������ � ������������ ���-�� �������
    uint16_t cur_entry   = sParam.jrnEntry.getCurrentEntry();
    uint16_t num_entries = sParam.jrnEntry.getNumJrnEntries();

    uint8_t poz = 0;
    // ����� �������� �������� ������ ����
    snprintf_P(&vLCDbuf[poz], 21, title);
    poz += 20;

    // ����� ����� ������� ������ � �� ���-��
    if (num_entries != 0)
    {
        if (device == AVANT_OPTO)
        {
            // � ������ ������������� ��������� ���-�� ������� � ����� ������
            snprintf_P(&vLCDbuf[poz],
                       21,
                       fcJrnNumEntriesOpto,
                       cur_entry,
                       num_entries,
                       sParam.jrnEntry.getNumOpticsEntries());
        }
        else
        {
            snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntries, cur_entry, num_entries);
        }
    }
    poz += 20;

    if (num_entries == 0)
    {
        // ����� ��������� �� ���������� ������� � �������
        snprintf_P(&vLCDbuf[poz + 24], 12, fcJrnEmpty);
    }
    else if (!sParam.jrnEntry.isReady())
    {
        // ��������� � ������� ������ ��� �� ��������
        snprintf_P(&vLCDbuf[poz + 21], 20, fcJrnNotReady);
    }
    else
    {
        // ����� ������
        snprintf_P(&vLCDbuf[poz], 21, fcRegimeJrn);
        snprintf_P(&vLCDbuf[poz + 7], 13, fcRegime[sParam.jrnEntry.getRegime()]);
        poz += 20;
        // ����� ����
        snprintf_P(&vLCDbuf[poz],
                   21,
                   fcDateJrn,
                   sParam.jrnEntry.dateTime.getDay(),
                   sParam.jrnEntry.dateTime.getMonth(),
                   sParam.jrnEntry.dateTime.getYear());
        poz += 20;
        //      snprintf_P(&vLCDbuf[poz],4,fcDevices[sParam.journalEntry.getDevice()]);
        // ����� �������
        snprintf_P(&vLCDbuf[poz],
                   21,
                   fcTimeJrn,
                   sParam.jrnEntry.dateTime.getHour(),
                   sParam.jrnEntry.dateTime.getMinute(),
                   sParam.jrnEntry.dateTime.getSecond(),
                   sParam.jrnEntry.dateTime.getMsSecond());
        poz += 20;

        // ����� �������
        // � ������ � ����� ������ ����� ���� ����� �������, �������
        // ����������� ��� ������� � ������
        uint8_t event = 0;
        if (device == AVANT_OPTO)
        {
            // �������� �������� ������ ������� � ���-��� ������� � ������
            if (curCom_ > sParam.jrnEntry.getNumOpticsEntries())
                curCom_ = 1;
            event = sParam.jrnEntry.getOpticEntry(curCom_);
        }
        else
        {
            event = sParam.jrnEntry.getEventType();
        }

        eGB_TYPE_DEVICE device = sParam.typeDevice;
        if (device == AVANT_R400M)
        {
            eGB_COMP_R400M comp           = sParam.glb.getCompR400m();
            uint8_t        remote_numbers = sParam.jrnEntry.getRemoteNumbers();
            printEventTextR400m(poz, event, remote_numbers, comp);
        }
        else if (device == AVANT_K400)
        {
            if (event <= MAX_JRN_EVENT_VALUE)
            {
                uint8_t dev = static_cast<uint8_t>(sParam.jrnEntry.getDeviceJrn());

                char tmp[SIZE_OF(fcDevicesK400[0])] = "";
                strncpy_P(tmp, fcDevicesK400[dev], SIZE_OF(tmp) - 1);
                snprintf_P(&vLCDbuf[poz], 21, fcJrnEventK400[event], tmp);
            }
            else
            {
                snprintf_P(&vLCDbuf[poz], 21, fcJrnEventK400[MAX_JRN_EVENT_VALUE], event);
            }
        }
        else if (device == AVANT_RZSK)
        {
            if (event < MAX_JRN_EVENT_VALUE)
            {
                uint8_t dev = static_cast<uint8_t>(sParam.jrnEntry.getDeviceJrn());

                char tmp[SIZE_OF(fcDevicesK400[0])] = "";
                strncpy_P(tmp, fcDevicesK400[dev], SIZE_OF(tmp) - 1);
                snprintf_P(&vLCDbuf[poz], 21, fcJrnEventRZSK[event], tmp);
            }
            else
            {
                snprintf_P(&vLCDbuf[poz], 21, fcJrnEventRZSK[MAX_JRN_EVENT_VALUE], event);
            }
        }
        else if (device == AVANT_OPTO)
        {
            if (sParam.glb.getTypeOpto() == TYPE_OPTO_RING_UNI)
            {
                snprintf_P(&vLCDbuf[poz], 21, fcJrnEventOPTOring[event], event);
            }
            else
            {
                snprintf_P(&vLCDbuf[poz], 21, fcJrnEventOPTO[event], event);
            }
        }
    }

    switch (key_)
    {
    case KEY_UP:
        if (sParam.jrnEntry.setPreviousEntry())
        {
            sParam.txComBuf.addFastCom(GB_COM_GET_JRN_ENTRY);
            curCom_ = 1;
        }
        break;
    case KEY_DOWN:
        if (sParam.jrnEntry.setNextEntry())
        {
            sParam.txComBuf.addFastCom(GB_COM_GET_JRN_ENTRY);
            curCom_ = 1;
        }
        break;

    case KEY_LEFT:
        if (device == AVANT_OPTO)
        {
            if (curCom_ > 1)
            {
                curCom_--;
            }
            else
            {
                curCom_ = sParam.jrnEntry.getNumOpticsEntries();
            }
        }
        break;
    case KEY_RIGHT:
        if (device == AVANT_OPTO)
        {
            if (curCom_ < sParam.jrnEntry.getNumOpticsEntries())
            {
                curCom_++;
            }
            else
            {
                curCom_ = 1;
            }
        }
        break;

    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlJournal;
        lvlCreate_ = true;
        break;
    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    default: break;
    }

    // �������� � ��������� ��� ��� ����� ����������� ������
    // �������� � �����, ����� �� ������ ����� �� ���������� ��������� �
    // ������� ������ ����
    sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
}

/** ������� ����. ������ ������.
 *  @param ���
 *  @return ���
 */
void clMenu::lvlJournalDef()
{
    static const char title[] PROGMEM = "������\\������";

    if (lvlCreate_)
    {
        lvlCreate_    = false;
        cursorEnable_ = false;
        lineParam_    = 1;

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        // ��������� �������� ������� � ������������� ���-�� ������� � ���
        sParam.jrnEntry.clear();
        sParam.jrnEntry.setCurrentDevice(GB_DEVICE_DEF);
        uint16_t        t      = 0;
        eGB_TYPE_DEVICE device = sParam.typeDevice;
        if (device == AVANT_R400M)
        {
            t = GLB_JRN_DEF_R400M_MAX;
        }
        else if (device == AVANT_RZSK)
        {
            t = GLB_JRN_DEF_RZSK_MAX;
        }
        else if (device == AVANT_OPTO)
        {
            t = GLB_JRN_DEF_OPTO_MAX;
        }
        sParam.jrnEntry.setMaxNumJrnEntries(t);

        // ������������� �������
        sParam.txComBuf.clear();
        sParam.txComBuf.addCom1(GB_COM_DEF_GET_JRN_CNT);
        sParam.txComBuf.addCom2(GB_COM_DEF_GET_JRN_ENTRY);
        sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
    }

    // ����� ������� ������ � ������ � ������������ ���-�� �������
    uint16_t cur_entry   = sParam.jrnEntry.getCurrentEntry();
    uint16_t num_entries = sParam.jrnEntry.getNumJrnEntries();

    uint8_t poz = 0;
    // ����� �������� �������� ������ ����
    snprintf_P(&vLCDbuf[poz], 21, title);
    poz += 20;
    // ����� ����� ������� ������ � �� ���-��
    if (num_entries != 0)
    {
        snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntries, cur_entry, num_entries);
    }
    poz += 20;

    if (num_entries == 0)
    {
        // ����� ��������� �� ���������� ������� � �������
        snprintf_P(&vLCDbuf[poz + 24], 12, fcJrnEmpty);
    }
    else if (!sParam.jrnEntry.isReady())
    {
        // ��������� � ������� ������ ��� �� ��������
        snprintf_P(&vLCDbuf[poz + 21], 20, fcJrnNotReady);
    }
    else
    {
        // ����� ���������
        snprintf_P(&vLCDbuf[poz], 21, fcStateJrn);
        if (sParam.glb.getTypeLine() != GB_TYPE_LINE_OPTO)
        {
            snprintf_P(&vLCDbuf[poz + 11],
                       10,
                       sParam.def.status.stateText[sParam.jrnEntry.getEventType()]);
        }
        poz += 20;
        // ����� ����
        snprintf_P(&vLCDbuf[poz],
                   21,
                   fcDateJrn,
                   sParam.jrnEntry.dateTime.getDay(),
                   sParam.jrnEntry.dateTime.getMonth(),
                   sParam.jrnEntry.dateTime.getYear());
        poz += 20;
        //      snprintf_P(&vLCDbuf[poz],4,fcDevices[sParam.journalEntry.getDevice()]);
        // ����� �������
        snprintf_P(&vLCDbuf[poz],
                   21,
                   fcTimeJrn,
                   sParam.jrnEntry.dateTime.getHour(),
                   sParam.jrnEntry.dateTime.getMinute(),
                   sParam.jrnEntry.dateTime.getSecond(),
                   sParam.jrnEntry.dateTime.getMsSecond());

        poz += 20;
        //      uint8_t signals = sParam.jrnEntry.getSignalDef();
        //      snprintf(&vLCDbuf[poz], 21, "%x",signals);
        snprintf_P(&vLCDbuf[poz],
                   21,
                   fcSignalDefJrn,
                   sParam.jrnEntry.getSignalPusk(),
                   sParam.jrnEntry.getSignalStop(),
                   sParam.jrnEntry.getSignalMan(),
                   sParam.jrnEntry.getSignalPrd(),
                   sParam.jrnEntry.getSignalPrm(),
                   sParam.jrnEntry.getSignalOut());
    }

    switch (key_)
    {
    case KEY_UP:
        if (sParam.jrnEntry.setPreviousEntry())
            sParam.txComBuf.addFastCom(GB_COM_DEF_GET_JRN_ENTRY);
        break;
    case KEY_DOWN:
        if (sParam.jrnEntry.setNextEntry())
            sParam.txComBuf.addFastCom(GB_COM_DEF_GET_JRN_ENTRY);
        break;

    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlJournal;
        lvlCreate_ = true;
        break;
    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;
    default: break;
    }

    // �������� � ��������� ��� ��� ����� ����������� ������
    // �������� � �����, ����� �� ������ ����� �� ���������� ��������� �
    // ������� ������ ����
    sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
}

/** ������� ����. ������ ���������.
 *  @param ���
 *  @return ���
 */
void clMenu::lvlJournalPrm()
{
    static const char title[] PROGMEM = "������\\��������";
    if (lvlCreate_)
    {
        lvlCreate_    = false;
        cursorEnable_ = false;
        lineParam_    = 1;
        curCom_       = 1;

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        // ��������� �������� ������� � ������������� ���-�� ������� � ���
        sParam.jrnEntry.clear();
        sParam.jrnEntry.setCurrentDevice(GB_DEVICE_PRM);
        uint16_t        t      = 0;
        eGB_TYPE_DEVICE device = sParam.typeDevice;
        if (device == AVANT_K400)
        {
            t = GLB_JRN_PRM_K400_MAX;
        }
        else if (device == AVANT_RZSK)
        {
            t = GLB_JRN_PRM_RZSK_MAX;
        }
        else if (device == AVANT_OPTO)
        {
            t = GLB_JRN_PRM_OPTO_MAX;
        }
        sParam.jrnEntry.setMaxNumJrnEntries(t);

        // ������������� �������
        sParam.txComBuf.clear();
        sParam.txComBuf.addCom1(GB_COM_PRM_GET_JRN_CNT);
        sParam.txComBuf.addCom2(GB_COM_PRM_GET_JRN_ENTRY);
        sParam.txComBuf.addCom2(GB_COM_GET_DEVICE_NUM);
        sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
    }

    eGB_TYPE_DEVICE device = sParam.typeDevice;

    // ����� ������� ������ � ������ � ������������ ���-�� �������
    uint16_t cur_entry   = sParam.jrnEntry.getCurrentEntry();
    uint16_t num_entries = sParam.jrnEntry.getNumJrnEntries();

    uint8_t poz = 0;
    // ����� �������� �������� ������ ����
    snprintf_P(&vLCDbuf[poz], 21, title);
    poz += 20;
    // ����� ����� ������� ������ � �� ���-��
    if (num_entries != 0)
    {
        if (device == AVANT_OPTO)
        {
            // � ������ ������������� ��������� ���-�� ������� � ����� ������
            snprintf_P(&vLCDbuf[poz],
                       21,
                       fcJrnNumEntriesOpto,
                       cur_entry,
                       num_entries,
                       sParam.jrnEntry.getNumOpticsEntries());
        }
        else
        {
            snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntries, cur_entry, num_entries);
        }
    }

    poz += 20;
    if (num_entries == 0)
    {
        // ����� ��������� �� ���������� ������� � �������
        snprintf_P(&vLCDbuf[poz + 24], 12, fcJrnEmpty);
    }
    else if (!sParam.jrnEntry.isReady())
    {
        // ��������� � ������� ������ ��� �� ��������
        snprintf_P(&vLCDbuf[poz + 21], 20, fcJrnNotReady);
    }
    else
    {
        // ����� ������ �������
        uint8_t com = 0;
        if (device == AVANT_OPTO)
        {
            // � ������ � ������ ������ ������ ��� �������� �� ���� �������
            // ���� 1 - ����������, 0 - ���.
            if (curCom_ > sParam.jrnEntry.getNumOpticsEntries())
                curCom_ = 1;
            com = sParam.jrnEntry.getOpticEntry(curCom_);
        }
        else
        {
            com = sParam.jrnEntry.getNumCom();
        }

        if ((device == AVANT_RZSK) && (sParam.glb.getMaxNumDevices() == 3))
        {
            uint8_t src    = sParam.jrnEntry.getSrcCom();
            uint8_t devnum = sParam.glb.getDeviceNum();

            if (devnum == 1)
            {
                char tmp[SIZE_OF(fcSrcPrm1[0])] = "";
                strncpy_P(tmp, fcSrcPrm1[src], SIZE_OF(tmp) - 1);
                snprintf_P(&vLCDbuf[poz], 21, fcNumComJrnPrm, com, tmp);
            }
            else if (devnum == 2)
            {
                char tmp[SIZE_OF(fcSrcPrm2[0])] = "";
                strncpy_P(tmp, fcSrcPrm2[src], SIZE_OF(tmp) - 1);
                snprintf_P(&vLCDbuf[poz], 21, fcNumComJrnPrm, com, tmp);
            }
            else if (devnum == 3)
            {
                char tmp[SIZE_OF(fcSrcPrm3[0])] = "";
                strncpy_P(tmp, fcSrcPrm3[src], SIZE_OF(tmp) - 1);
                snprintf_P(&vLCDbuf[poz], 21, fcNumComJrnPrm, com, tmp);
            }
            else
            {
                snprintf_P(&vLCDbuf[poz], 21, fcNumComJrn, com);
            }
        }
        else
        {
            uint8_t t = snprintf_P(&vLCDbuf[poz], 21, fcNumComJrn, com);

            if (device == AVANT_K400)
            {
                uint8_t s = sParam.jrnEntry.getSourceCom();
                snprintf_P(&vLCDbuf[poz + t + 1], 5, fcJrnSourcePrd[s]);
            }
        }

        poz += 20;
        // ����� ����
        snprintf_P(&vLCDbuf[poz],
                   21,
                   fcDateJrn,
                   sParam.jrnEntry.dateTime.getDay(),
                   sParam.jrnEntry.dateTime.getMonth(),
                   sParam.jrnEntry.dateTime.getYear());
        poz += 20;
        // ����� �������
        snprintf_P(&vLCDbuf[poz],
                   21,
                   fcTimeJrn,
                   sParam.jrnEntry.dateTime.getHour(),
                   sParam.jrnEntry.dateTime.getMinute(),
                   sParam.jrnEntry.dateTime.getSecond(),
                   sParam.jrnEntry.dateTime.getMsSecond());
        poz += 20;
        // ����� �������
        if (device == AVANT_OPTO)
        {
            // � ������ ���� ���� ������ - �� ��� ������� �������
            // ����� - ������ �� �������� ���
            if (com != 0)
            {
                snprintf_P(&vLCDbuf[poz], 21, fcJrnPrmOptoComYes);
            }
            else
            {
                snprintf_P(&vLCDbuf[poz], 21, fcJrnPrdOptoComNo);
            }
        }
        else
        {
            uint8_t event = sParam.jrnEntry.getEventType();
            snprintf_P(&vLCDbuf[poz], 21, fcJrnPrd[event], event);
        }
    }

    switch (key_)
    {
    case KEY_UP:
        if (sParam.jrnEntry.setPreviousEntry())
        {
            sParam.txComBuf.addFastCom(GB_COM_PRM_GET_JRN_ENTRY);
            curCom_ = 1;
        }
        break;
    case KEY_DOWN:
        if (sParam.jrnEntry.setNextEntry())
        {
            sParam.txComBuf.addFastCom(GB_COM_PRM_GET_JRN_ENTRY);
            curCom_ = 1;
        }
        break;

    case KEY_LEFT:
        if (device == AVANT_OPTO)
        {
            if (curCom_ > 1)
            {
                curCom_--;
            }
            else
            {
                curCom_ = sParam.jrnEntry.getNumOpticsEntries();
            }
        }
        break;
    case KEY_RIGHT:
        if (device == AVANT_OPTO)
        {
            if (curCom_ < sParam.jrnEntry.getNumOpticsEntries())
            {
                curCom_++;
            }
            else
            {
                curCom_ = 1;
            }
        }
        break;

    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlJournal;
        lvlCreate_ = true;
        break;
    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    default: break;
    }

    // �������� � ��������� ��� ��� ����� ����������� ������
    // �������� � �����, ����� �� ������ ����� �� ���������� ��������� �
    // ������� ������ ����
    sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
}

/** ������� ����. ������ �����������.
 *  @param ���
 *  @return ���
 */
void clMenu::lvlJournalPrd()
{
    static const char title[] PROGMEM = "������\\����������";

    if (lvlCreate_)
    {
        lvlCreate_    = false;
        cursorEnable_ = false;
        lineParam_    = 1;
        curCom_       = 1;

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        // ��������� �������� ������� � ������������� ���-�� ������� � ���
        sParam.jrnEntry.clear();
        sParam.jrnEntry.setCurrentDevice(GB_DEVICE_PRD);
        uint16_t        t      = 0;
        eGB_TYPE_DEVICE device = sParam.typeDevice;
        if (device == AVANT_K400)
        {
            t = GLB_JRN_PRD_K400_MAX;
        }
        else if (device == AVANT_RZSK)
        {
            t = GLB_JRN_PRD_RZSK_MAX;
        }
        else if (device == AVANT_OPTO)
        {
            t = GLB_JRN_PRD_OPTO_MAX;
        }
        sParam.jrnEntry.setMaxNumJrnEntries(t);

        // ������������� �������
        sParam.txComBuf.clear();
        sParam.txComBuf.addCom1(GB_COM_PRD_GET_JRN_CNT);
        sParam.txComBuf.addCom2(GB_COM_PRD_GET_JRN_ENTRY);
        sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
    }

    eGB_TYPE_DEVICE device = sParam.typeDevice;

    // ����� ������� ������ � ������ � ������������ ���-�� �������
    uint16_t cur_entry   = sParam.jrnEntry.getCurrentEntry();
    uint16_t num_entries = sParam.jrnEntry.getNumJrnEntries();

    uint8_t poz = 0;
    // ����� �������� �������� ������ ����
    snprintf_P(&vLCDbuf[poz], 21, title);
    poz += 20;

    uint8_t numSubEntries = sParam.jrnEntry.getNumOpticsEntries();

    // ����� ����� ������� ������ � �� ���-��
    if (num_entries != 0)
    {
        if (device == AVANT_OPTO)
        {
            // � ������ ������������� ��������� ���-�� ������� � ����� ������
            snprintf_P(&vLCDbuf[poz],
                       21,
                       fcJrnNumEntriesOpto,
                       cur_entry,
                       num_entries,
                       numSubEntries);
        }
        else
        {
            snprintf_P(&vLCDbuf[poz], 21, fcJrnNumEntries, cur_entry, num_entries);
        }
    }

    poz += 20;
    if (num_entries == 0)
    {
        // ����� ��������� �� ���������� ������� � �������
        snprintf_P(&vLCDbuf[poz + 24], 12, fcJrnEmpty);
    }
    else if (!sParam.jrnEntry.isReady())
    {
        // ��������� � ������� ������ ��� �� ��������
        snprintf_P(&vLCDbuf[poz + 21], 20, fcJrnNotReady);
    }
    else
    {
        // ����� ������ �������
        uint8_t com = 0;
        if (device == AVANT_OPTO)
        {
            // � ������ � ����� ������ ����� ���� ����� ������
            if (curCom_ > numSubEntries)
            {
                curCom_ = 1;
            }

            com = sParam.jrnEntry.getOpticEntry(curCom_);
        }
        else
        {
            com = sParam.jrnEntry.getNumCom();
        }
        uint8_t t = snprintf_P(&vLCDbuf[poz], 21, fcNumComJrn, com);

        // ��� ���������� ��-����������, ������� �������� ������������ �������
        if ((device == AVANT_K400) || (device == AVANT_RZSK))
        {
            uint8_t s = sParam.jrnEntry.getSourceCom();
            snprintf_P(&vLCDbuf[poz + t + 1], 5, fcJrnSourcePrd[s]);
        }
        else if (device == AVANT_OPTO)
        {
            uint8_t s = sParam.jrnEntry.getOpticEntryDR(com);
            snprintf_P(&vLCDbuf[poz + t + 1], 5, fcJrnSourcePrd[s]);
        }

        poz += 20;
        // ����� ����
        snprintf_P(&vLCDbuf[poz],
                   21,
                   fcDateJrn,
                   sParam.jrnEntry.dateTime.getDay(),
                   sParam.jrnEntry.dateTime.getMonth(),
                   sParam.jrnEntry.dateTime.getYear());
        poz += 20;
        // ����� �������
        snprintf_P(&vLCDbuf[poz],
                   21,
                   fcTimeJrn,
                   sParam.jrnEntry.dateTime.getHour(),
                   sParam.jrnEntry.dateTime.getMinute(),
                   sParam.jrnEntry.dateTime.getSecond(),
                   sParam.jrnEntry.dateTime.getMsSecond());
        poz += 20;
        // ����� �������
        if (device == AVANT_OPTO)
        {
            // � ������ ���� ���� ������ - �� ��� ������� �������
            // ����� - ������ �� �������� ���
            if (com != 0)
            {
                snprintf_P(&vLCDbuf[poz], 21, fcJrnPrdOptoComYes);
            }
            else
            {
                snprintf_P(&vLCDbuf[poz], 21, fcJrnPrdOptoComNo);
            }
        }
        else
        {
            uint8_t event = sParam.jrnEntry.getEventType();
            snprintf_P(&vLCDbuf[poz], 21, fcJrnPrd[event], event);
        }
    }

    switch (key_)
    {
    case KEY_UP:
        if (sParam.jrnEntry.setPreviousEntry())
        {
            sParam.txComBuf.addFastCom(GB_COM_PRD_GET_JRN_ENTRY);
            curCom_ = 1;
        }
        break;
    case KEY_DOWN:
        if (sParam.jrnEntry.setNextEntry())
        {
            sParam.txComBuf.addFastCom(GB_COM_PRD_GET_JRN_ENTRY);
            curCom_ = 1;
        }
        break;

    case KEY_LEFT:
        if (device == AVANT_OPTO)
        {
            if (curCom_ > 1)
            {
                curCom_--;
            }
            else
            {
                curCom_ = sParam.jrnEntry.getNumOpticsEntries();
            }
        }
        break;
    case KEY_RIGHT:
        if (device == AVANT_OPTO)
        {
            if (curCom_ < sParam.jrnEntry.getNumOpticsEntries())
            {
                curCom_++;
            }
            else
            {
                curCom_ = 1;
            }
        }
        break;

    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlJournal;
        lvlCreate_ = true;
        break;
    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    default: break;
    }

    // �������� � ��������� ��� ��� ����� ����������� ������
    // �������� � �����, ����� �� ������ ����� �� ���������� ��������� �
    // ������� ������ ����
    sParam.txComBuf.setInt16(sParam.jrnEntry.getEntryAdress());
}

/** ������� ����. ����������.
 *  @param ���
 *  @return ���
 */
void clMenu::lvlControl()
{

    static const char title[] PROGMEM = "����\\����������";

    eGB_TYPE_DEVICE device = sParam.typeDevice;

    if (lvlCreate_)
    {
        lvlCreate_    = false;
        cursorLine_   = 1;
        cursorEnable_ = true;
        lineParam_    = 1;

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        Punkts_.Clear();
        sParam.local.clearParams();

        fillLvlControl(device);

        // ������������� �������
        sParam.txComBuf.clear();
        if (sParam.typeDevice != AVANT_OPTO)
        {
            // ��� �����, ������������� � ����� �������� ��� ����������������
            sParam.txComBuf.addCom1(GB_COM_DEF_GET_LINE_TYPE);
            sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);
            sParam.txComBuf.addCom1(GB_COM_GET_DEVICE_NUM);

            if (isRzskM())
            {
                // ����� �������������
                sParam.txComBuf.addCom1(GB_COM_DEF_GET_TYPE_AC);
                sParam.local.addParam(GB_PARAM_DEF_ONE_SIDE);
            }
        }
    }

    snprintf_P(&vLCDbuf[0], 21, title);

    ChangeControlPunkts();
    printPunkts();

    switch (key_)
    {
    case KEY_UP: cursorLineUp(); break;
    case KEY_DOWN: cursorLineDown(); break;

    case KEY_CANCEL:
        {
            lvlMenu    = &clMenu::lvlFirst;
            lvlCreate_ = true;
            break;
        }

    case KEY_MENU:
        {
            lvlMenu    = &clMenu::lvlStart;
            lvlCreate_ = true;
            break;
        }

    case KEY_ENTER:
        {
            uint8_t ctrl = Punkts_.GetData(cursorLine_ - 1);
            AddControlToSend(static_cast<TControl::ctrl_t>(ctrl));
            break;
        }


    default: break;
    }
}

/** ������� ����. ���������.
 *  @param ���
 *  @return ���
 */
void clMenu::lvlSetup()
{
    static const char title[] PROGMEM  = "����\\���������";
    static const char punkt1[] PROGMEM = "�����";
    static const char punkt2[] PROGMEM = "����� � ����";
    static const char punkt3[] PROGMEM = "���������";
    static const char punkt4[] PROGMEM = "������";
    static const char punkt5[] PROGMEM = "���������";

    if (lvlCreate_)
    {
        lvlCreate_    = false;
        cursorLine_   = 1;
        cursorEnable_ = true;
        lineParam_    = 1;

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        Punkts_.Clear();
        Punkts_.Add(punkt1);
        Punkts_.Add(punkt2);
        Punkts_.Add(punkt3);
        Punkts_.Add(punkt4);
        Punkts_.Add(punkt5);

        // ������������� �������
        sParam.txComBuf.clear();
    }

    snprintf_P(&vLCDbuf[0], 21, title);

    PGM_P name = Punkts_.GetName(cursorLine_ - 1);
    printPunkts();
    if (EnterParam.isEnable())
    {
        // ���� ������ �������� ���������
        eMENU_ENTER_PARAM stat = EnterParam.getStatus();

        // ����� ������� ����� : ������ ��� ��������
        (this->*enterFunc)();

        if (stat == MENU_ENTER_PASSWORD_READY)
        {
            uint16_t val = static_cast<uint16_t>(EnterParam.getValue());

            if (sParam.password.check(val))
            {
                EnterParam.setEnable(MENU_ENTER_PASSWORD_NEW);
            }
            else
            {
                EnterParam.setDisable();
            }
        }
        else if (stat == MENU_ENTER_PASSWORD_N_READY)
        {
            uint16_t val = static_cast<uint16_t>(EnterParam.getValue());

            sParam.password.set(val);
        }
    }

    switch (key_)
    {
    case KEY_UP: cursorLineUp(); break;
    case KEY_DOWN: cursorLineDown(); break;

    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlFirst;
        lvlCreate_ = true;
        break;
    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    case KEY_ENTER:
        {
            if (name == punkt1)
            {
                lvlMenu    = &clMenu::lvlRegime;
                lvlCreate_ = true;
            }
            else if (name == punkt2)
            {
                lvlMenu    = &clMenu::lvlSetupDT;
                lvlCreate_ = true;
            }
            else if (name == punkt3)
            {
                lvlMenu    = &clMenu::lvlSetupParam;
                lvlCreate_ = true;
            }
            else if (name == punkt4)
            {
                enterFunc = &clMenu::enterPassword;
                EnterParam.setEnable(MENU_ENTER_PASSWORD);
                EnterParam.com = GB_COM_NO;
            }
            else if (name == punkt5)
            {
                lvlMenu    = &clMenu::lvlSetupInterface;
                lvlCreate_ = true;
            }
        }
        break;

    default: break;
    }
}

void clMenu::lvlRegime()
{
    static const char title[] PROGMEM = "���������\\�����";
    eGB_REGIME        reg             = sParam.glb.status.getRegime();

    if (lvlCreate_)
    {
        lvlCreate_    = false;
        cursorLine_   = 1;
        cursorEnable_ = true;
        lineParam_    = 1;

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        // ������������� �������
        sParam.txComBuf.clear();
        // ���-�� ��������� � �����
        sParam.txComBuf.addCom2(GB_COM_GET_DEVICE_NUM);
    }

    snprintf_P(&vLCDbuf[0], 21, title);

    uint8_t poz = lineParam_ * 20;
    if (sParam.def.status.isEnable())
    {
        printDevicesRegime(poz, &sParam.def.status);
        poz += 20;
    }
    if (sParam.prm.status.isEnable())
    {
        printDevicesRegime(poz, &sParam.prm.status);
        poz += 20;
    }
    if (sParam.prd.status.isEnable())
    {
        printDevicesRegime(poz, &sParam.prd.status);
    }

    // ���� ������ �������� ���������.
    // ������� ���������� ��������� ����� ������.
    // ��� ������� ������� � ����� "�������" �� "������" ��� "�����",
    // ���������� ������ ������. ��� ��������� ������ ��������� ���������.
    if (EnterParam.isEnable())
    {
        eMENU_ENTER_PARAM stat = EnterParam.getStatus();
        eGB_REGIME_ENTER  val  = GB_REGIME_ENTER_MAX;

        // ����� ������� ����� : ������ ��� ��������
        (this->*enterFunc)();

        if (stat == MENU_ENTER_PARAM_READY)
        {
            // �������� ������, ���� �������� ������� � ����� "�������"
            // �� ������� "������" � "�����"
            val = static_cast<eGB_REGIME_ENTER>(EnterParam.getValueEnter());

            if ((reg == GB_REGIME_ENABLED) || (reg == GB_REGIME_READY))
            {
                if (val == GB_REGIME_ENTER_DISABLED)
                {
                    enterFunc = &clMenu::enterPassword;
                    EnterParam.setEnable(MENU_ENTER_PASSWORD);
                    EnterParam.setDopValue(static_cast<uint16_t>(val));
                    val = GB_REGIME_ENTER_MAX;
                }
            }
        }

        if (stat == MENU_ENTER_PASSWORD_READY)
        {
            // �������� ��������� ������

            if (sParam.password.check(static_cast<uint16_t>(EnterParam.getValue())))
            {
                val = static_cast<eGB_REGIME_ENTER>(EnterParam.getDopValue());
            }
            else
            {
                EnterParam.printMessage();
            }
        }

        if (val != GB_REGIME_ENTER_MAX)
        {
            eGB_COM com = GB_COM_NO;

            if (val == GB_REGIME_ENTER_DISABLED)
                com = GB_COM_SET_REG_DISABLED;
            else if (val == GB_REGIME_ENTER_ENABLED)
                com = GB_COM_SET_REG_ENABLED;
            sParam.txComBuf.addFastCom(com);
            EnterParam.setDisable();
        }
    }

    switch (key_)
    {
    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlSetup;
        lvlCreate_ = true;
        break;
    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    case KEY_ENTER:
        {
            uint8_t min = GB_REGIME_ENTER_DISABLED;
            uint8_t max = GB_REGIME_ENTER_DISABLED;
            uint8_t val = GB_REGIME_ENTER_DISABLED;

            // "������"     -> "�������"
            // "�������"    -> "������"
            // "�����"      -> "������"  / "�������"
            // ���������    -> "�������" / "������"
            enterFunc = &clMenu::enterValue;
            EnterParam.setParam(GB_PARAM_OTHER_REGIME);
            EnterParam.setEnable(MENU_ENTER_PARAM_LIST);
            switch (reg)
            {
            case GB_REGIME_ENABLED:
                min = GB_REGIME_ENTER_DISABLED;
                max = GB_REGIME_ENTER_DISABLED;
                val = GB_REGIME_ENTER_DISABLED;
                break;
            case GB_REGIME_DISABLED:
                min = GB_REGIME_ENTER_ENABLED;
                max = GB_REGIME_ENTER_ENABLED;
                val = GB_REGIME_ENTER_ENABLED;
                break;
            case GB_REGIME_READY:
                min = GB_REGIME_ENTER_DISABLED;
                max = GB_REGIME_ENTER_ENABLED;
                val = GB_REGIME_ENTER_ENABLED;
                break;
            default:
                min = GB_REGIME_ENTER_DISABLED;
                max = GB_REGIME_ENTER_ENABLED;
                val = GB_REGIME_ENTER_DISABLED;
                break;
            }
            EnterParam.setValueRange(min, max);
            EnterParam.setValue(val);
            EnterParam.com = GB_COM_NO;
        }
        break;

    default:
        {
        }
        break;
    }
}

/** ������� ����. ��������� ����������.
 *  @param ���
 *  @return ���
 */
void clMenu::lvlSetupParam()
{
    static const char title[] PROGMEM  = "���������\\���������";
    static const char punkt1[] PROGMEM = "������";
    static const char punkt2[] PROGMEM = "��������";
    static const char punkt3[] PROGMEM = "����������";
    static const char punkt4[] PROGMEM = "�����";
    static const char punkt5[] PROGMEM = "������";

    if (lvlCreate_)
    {
        lvlCreate_    = false;
        cursorLine_   = 1;
        cursorEnable_ = true;
        lineParam_    = 1;

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        // ��������� ����, � ����������� �� �������� ����������
        Punkts_.Clear();
        if (sParam.def.status.isEnable())
        {
            Punkts_.Add(punkt1);
        }
        if (sParam.prm.status.isEnable())
        {
            Punkts_.Add(punkt2);
        }
        if (sParam.prd.status.isEnable())
        {
            Punkts_.Add(punkt3);
        }
        Punkts_.Add(punkt4);

        // � ������ ��� ������ ����������� ����� ���������
        if (sParam.glb.getTypeDevice() == AVANT_OPTO)
        {
            if (sParam.glb.getTypeOpto() != TYPE_OPTO_STANDART)
            {
                Punkts_.Add(punkt5);
            }
        }

        // ������������� �������
        sParam.txComBuf.clear();
    }

    PGM_P name = Punkts_.GetName(cursorLine_ - 1);

    snprintf_P(&vLCDbuf[0], 20, title);
    printPunkts();

    switch (key_)
    {
    case KEY_UP: cursorLineUp(); break;
    case KEY_DOWN: cursorLineDown(); break;

    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlSetup;
        lvlCreate_ = true;
        break;
    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    case KEY_ENTER:
        if (name == punkt1)
        {
            lvlMenu    = &clMenu::lvlSetupParamDef;
            lvlCreate_ = true;
        }
        else if (name == punkt2)
        {
            lvlMenu    = &clMenu::lvlSetupParamPrm;
            lvlCreate_ = true;
        }
        else if (name == punkt3)
        {
            lvlMenu    = &clMenu::lvlSetupParamPrd;
            lvlCreate_ = true;
        }
        else if (name == punkt4)
        {
            lvlMenu    = &clMenu::lvlSetupParamGlb;
            lvlCreate_ = true;
        }
        else if (name == punkt5)
        {
            lvlMenu    = &clMenu::lvlSetupParamRing;
            lvlCreate_ = true;
        }
        break;

    default: break;
    }
}

/** ������� ����. ��������� ���������� ������.
 *  @param ���
 *  @return ���
 */
void clMenu::lvlSetupParamDef()
{
    static const char title[] PROGMEM = "���������\\������";

    if (lvlCreate_)
    {
        lvlCreate_ = false;
        EnterParam.setDisable();

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        // ���������� �������� ���������� � ������
        eGB_TYPE_DEVICE device = sParam.typeDevice;
        sParam.txComBuf.clear();

        fillLvlSetupParamDef(device);

        // ��� ���������������� ���� ����������� ������� ������:
        // ���-�� ��������� � �����
        sParam.txComBuf.addCom2(GB_COM_DEF_GET_LINE_TYPE);

        if (device == AVANT_RZSK)
        {
            sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);
        }
        else if (device == AVANT_R400M)
        {
            // ��� ���������������� ���� ����������� ������� ������:
            // �������������
            sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);
        }
    }

    snprintf_P(&vLCDbuf[0], 21, title);

    setupParam();

    if (!sParam.def.status.isEnable())
    {
        key_ = KEY_CANCEL;
    }

    switch (key_)
    {
    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlSetupParam;
        lvlCreate_ = true;
        break;
    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    default: break;
    }
}

/** ������� ����. ��������� ���������� ���������.
 *  @param ���
 *  @return ���
 */
void clMenu::lvlSetupParamPrm()
{
    static const char title[] PROGMEM = "���������\\��������";

    if (lvlCreate_)
    {
        lvlCreate_ = false;
        EnterParam.setDisable();

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        // ���������� �������� ���������� � ������
        eGB_TYPE_DEVICE device = sParam.typeDevice;
        sParam.txComBuf.clear();

        sParam.local.clearParams();

        uint8_t numcom = sParam.prm.getNumCom();
        sParam.txComBuf.addCom2(GB_COM_PRM_GET_COM);

        if (device == AVANT_K400)
        {
            sParam.local.addParam(GB_PARAM_PRM_COM_NUMS);
            sParam.local.addParam(GB_PARAM_PRM_TIME_ON);
            if (numcom != 0)
            {
                sParam.local.addParam(GB_PARAM_PRM_COM_BLOCK);
                sParam.local.addParam(GB_PARAM_PRM_TIME_OFF);
            }
            sParam.local.addParam(GB_PARAM_PRM_DR_ENABLE);
            if (numcom != 0)
            {
                sParam.local.addParam(GB_PARAM_PRM_DR_COM_BLOCK);
                sParam.local.addParam(GB_PARAM_PRM_DR_COM_TO_HF);
            }
            sParam.local.addParam(GB_PARAM_PRM_TEST_COM);
            sParam.local.addParam(GB_PARAM_PRM_FREQ_CORR);
            sParam.local.addParam(GB_PARAM_PRM_COM_SIGNAL);
        }
        else if (device == AVANT_RZSK)
        {
            sParam.local.addParam(GB_PARAM_PRM_TIME_ON);
            if (numcom != 0)
            {
                sParam.local.addParam(GB_PARAM_PRM_COM_BLOCK);
                sParam.local.addParam(GB_PARAM_PRM_TIME_OFF);
                sParam.local.addParam(GB_PARAM_PRM_INC_SAFETY);
            }
        }
        else if (device == AVANT_OPTO)
        {
            sParam.local.addParam(GB_PARAM_PRM_TIME_ON);
            if (numcom != 0)
            {
                sParam.local.addParam(GB_PARAM_PRM_COM_BLOCK);
                sParam.local.addParam(GB_PARAM_PRM_TIME_OFF);
            }

            if (sParam.glb.getTypeOpto() == TYPE_OPTO_STANDART)
            {
                sParam.local.addParam(GB_PARAM_PRM_DR_ENABLE);
                if (numcom != 0)
                {
                    sParam.local.addParam(GB_PARAM_PRM_DR_COM_BLOCK);
                    sParam.local.addParam(GB_PARAM_PRM_DR_COM_TO_HF);
                }
                sParam.local.addParam(GB_PARAM_PRM_COM_SIGNAL);
            }
        }
    }

    snprintf_P(&vLCDbuf[0], 21, title);

    setupParam();

    if (!sParam.prm.status.isEnable())
    {
        key_ = KEY_CANCEL;
    }

    switch (key_)
    {
    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlSetupParam;
        lvlCreate_ = true;
        break;
    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    default: break;
    }
}

/** ������� ����. ��������� ���������� �����������.
 *  @param ���
 *  @return ���
 */
void clMenu::lvlSetupParamPrd()
{
    static const char title[] PROGMEM = "���������\\����������";

    if (lvlCreate_)
    {
        lvlCreate_ = false;
        EnterParam.setDisable();

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        // ���������� �������� ���������� � ������
        eGB_TYPE_DEVICE device = sParam.typeDevice;
        sParam.txComBuf.clear();

        sParam.local.clearParams();
        uint8_t numcom = sParam.prd.getNumCom();
        sParam.txComBuf.addCom2(GB_COM_PRD_GET_COM);

        if (device == AVANT_K400)
        {  // �������������
            sParam.txComBuf.addCom2(GB_COM_GET_COM_PRD_KEEP);

            sParam.local.addParam(GB_PARAM_PRD_COM_NUMS);
            sParam.local.addParam(GB_PARAM_PRD_IN_DELAY);
            sParam.local.addParam(GB_PARAM_PRD_DURATION_L);
            sParam.local.addParam(GB_PARAM_PRD_TEST_COM);
            if (numcom != 0)
            {
                sParam.local.addParam(GB_PARAM_PRD_COM_LONG);
                sParam.local.addParam(GB_PARAM_PRD_COM_BLOCK);
                sParam.local.addParam(GB_PARAM_PRD_COM_NUMS_A);
            }
            sParam.local.addParam(GB_PARAM_PRD_DR_ENABLE);
            if (numcom != 0)
            {
                sParam.local.addParam(GB_PARAM_PRD_DR_COM_BLOCK);
            }
            sParam.local.addParam(GB_PARAM_PRD_FREQ_CORR);

            sParam.local.addParam(GB_PARAM_PRD_DEC_CF);
            sParam.local.addParam(GB_PARAM_PRD_DEC_TM);
            //          sParam.local.addParam(GB_PARAM_PRD_DEFAULT_CF); ����� �� �������������
            sParam.local.addParam(GB_PARAM_PRD_COM_SIGNAL);
        }
        else if (device == AVANT_RZSK)
        {
            sParam.local.addParam(GB_PARAM_PRD_IN_DELAY);
            sParam.local.addParam(GB_PARAM_PRD_DURATION_L);
            sParam.local.addParam(GB_PARAM_PRD_COM_LONG);
            sParam.local.addParam(GB_PARAM_PRD_COM_BLOCK);
        }
        else if (device == AVANT_OPTO)
        {
            sParam.local.addParam(GB_PARAM_PRD_IN_DELAY);
            sParam.local.addParam(GB_PARAM_PRD_DURATION_O);
            sParam.local.addParam(GB_PARAM_PRD_COM_LONG);
            sParam.local.addParam(GB_PARAM_PRD_COM_BLOCK);

            if (sParam.glb.getTypeOpto() == TYPE_OPTO_STANDART)
            {
                sParam.local.addParam(GB_PARAM_PRD_DR_ENABLE);
                if (numcom != 0)
                {
                    sParam.local.addParam(GB_PARAM_PRD_DR_COM_BLOCK);
                }
                sParam.local.addParam(GB_PARAM_PRD_COM_SIGNAL);
            }
        }
    }

    snprintf_P(&vLCDbuf[0], 21, title);

    setupParam();

    if (!sParam.prd.status.isEnable())
    {
        key_ = KEY_CANCEL;
    }

    switch (key_)
    {
    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlSetupParam;
        lvlCreate_ = true;
        break;
    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    default: break;
    }
}

/** ������� ����. ��������� ���������� �����.
 *  @param ���
 *  @return ���
 */
void clMenu::lvlSetupParamGlb()
{
    static const char title[] PROGMEM = "���������\\�����";

    if (lvlCreate_)
    {
        lvlCreate_ = false;
        EnterParam.setDisable();

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        // ���������� �������� ���������� � ������
        eGB_TYPE_DEVICE device = sParam.typeDevice;

        sParam.txComBuf.clear();
        fillLvlSetupParamGlb(device);

        if (device == AVANT_K400)
        {
            sParam.txComBuf.addCom2(GB_COM_GET_MEAS);
            sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);
        }
        else if (device == AVANT_RZSK)
        {
            sParam.txComBuf.addCom2(GB_COM_GET_MEAS);
            sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);
        }
        else if (device == AVANT_R400M)
        {
            // ��� ���������������� ���� ����������� ������� ������:
            // ������������� � ���-�� ��������� � �����
            // ���������� ��������� ��� ���������
            sParam.txComBuf.addCom2(GB_COM_GET_MEAS);
            sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);
            sParam.txComBuf.addCom1(GB_COM_DEF_GET_LINE_TYPE);
        }
        else if (device == AVANT_OPTO)
        {
        }
    }

    snprintf_P(&vLCDbuf[0], 21, title);

    setupParam();

    switch (key_)
    {
    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlSetupParam;
        lvlCreate_ = true;
        break;
    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    default: break;
    }
}

/** ������� ����. ��������� ���������� ������.
 *  @param ���
 *  @return ���
 */
void clMenu::lvlSetupParamRing()
{
    static const char title[] PROGMEM = "���������\\������";

    if (lvlCreate_)
    {
        lvlCreate_ = false;
        EnterParam.setDisable();

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        // ���������� �������� ���������� � ������
        eGB_TYPE_DEVICE device = sParam.typeDevice;
        eGB_TYPE_OPTO   topto  = sParam.glb.getTypeOpto();
        sParam.txComBuf.clear();

        sParam.local.clearParams();
        if (device == AVANT_OPTO)
        {
            if (topto == TYPE_OPTO_RING_UNI)
            {
                sParam.local.addParam(GB_PARAM_RING_TIME_WAIT);
                sParam.local.addParam(GB_PARAM_RING_COM_TRANSIT);
                sParam.local.addParam(GB_PARAM_RING_COM_REC);
                sParam.local.addParam(GB_PARAM_RING_COM_TR);
            }
        }
    }

    snprintf_P(&vLCDbuf[0], 21, title);

    setupParam();

    // ����� �� ������� ������ ����, ���� ��� �� ������ ������
    if (sParam.glb.getTypeDevice() != AVANT_OPTO)
    {
        key_ = KEY_CANCEL;
    }
    else if (sParam.glb.getTypeOpto() == TYPE_OPTO_STANDART)
    {
        key_ = KEY_CANCEL;
    }

    switch (key_)
    {
    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlSetupParam;
        lvlCreate_ = true;
        break;
    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    default: break;
    }
}

/** ������� ����. ���������
 *  @param ���
 *  @return ���
 */
void clMenu::lvlSetupInterface()
{
    static const char title[] PROGMEM = "���������\\���������";

    if (lvlCreate_)
    {
        lvlCreate_ = false;
        EnterParam.setDisable();

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        //      eGB_TYPE_DEVICE device = sParam.typeDevice;
        sParam.txComBuf.clear();

        sParam.local.clearParams();
        // ���� ����������� ����� �� ��������� ����
        // ���������� ��������� ������
        // � USB ������: 19200 ���/�, 8 ���, 2 ����-����, ��������-���
        sParam.local.addParam(GB_PARAM_INTF_INTERFACE);
        if (sParam.Uart.Interface.get() == TInterface::RS485)
        {
            sParam.local.addParam(GB_PARAM_INTF_PROTOCOL);
            // � ������ ������� ����� �������� � ����� ������,
            // ����� ������ � ��������
            sParam.local.addParam(GB_PARAM_NET_ADDRESS);
            sParam.local.addParam(GB_PARAM_INTF_BAUDRATE);
            sParam.local.addParam(GB_PARAM_INTF_DATA_BITS);
            sParam.local.addParam(GB_PARAM_INTF_PARITY);
            sParam.local.addParam(GB_PARAM_INTF_STOP_BITS);
        }
    }

    snprintf_P(&vLCDbuf[0], 21, title);

    LocalParams* lp = &sParam.local;
    if (getCom(lp->getParam()) == GB_COM_NO)
    {
        // ��� ���������� �������� �������� ��������� ����������� � ������
        eGB_PARAM param = lp->getParam();
        if (param == GB_PARAM_INTF_INTERFACE)
        {
            lp->setValue(sParam.Uart.Interface.get());
        }
        else if (param == GB_PARAM_INTF_PROTOCOL)
        {
            lp->setValue(sParam.Uart.Protocol.get());
        }
        else if (param == GB_PARAM_INTF_BAUDRATE)
        {
            lp->setValue(sParam.Uart.BaudRate.get());
        }
        else if (param == GB_PARAM_INTF_DATA_BITS)
        {
            lp->setValue(sParam.Uart.DataBits.get());
        }
        else if (param == GB_PARAM_INTF_PARITY)
        {
            lp->setValue(sParam.Uart.Parity.get());
        }
        else if (param == GB_PARAM_INTF_STOP_BITS)
        {
            lp->setValue(sParam.Uart.StopBits.get());
        }
    }

    setupParam();

    switch (key_)
    {
    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlSetup;
        lvlCreate_ = true;
        break;
    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    default: break;
    }
}

/** ������� ����. ��������� ����/�����.
 *  @param ���
 *  @return ���
 */
void clMenu::lvlSetupDT()
{
    static const char title[] PROGMEM  = "���������\\�����&����";
    static const char punkt1[] PROGMEM = "���";
    static const char punkt2[] PROGMEM = "�����";
    static const char punkt3[] PROGMEM = "����";
    static const char punkt4[] PROGMEM = "����";
    static const char punkt5[] PROGMEM = "������";
    static const char punkt6[] PROGMEM = "�������";

    if (lvlCreate_)
    {
        lvlCreate_    = false;
        cursorLine_   = 1;
        cursorEnable_ = true;
        lineParam_    = 2;

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        Punkts_.Clear();
        Punkts_.Add(punkt1);
        Punkts_.Add(punkt2);
        Punkts_.Add(punkt3);
        Punkts_.Add(punkt4);
        Punkts_.Add(punkt5);
        Punkts_.Add(punkt6);

        // ������������� �������
        sParam.txComBuf.clear();
    }

    PGM_P name = Punkts_.GetName(cursorLine_ - 1);

    snprintf_P(&vLCDbuf[20], 21, title);

    printMeasParam(0, MENU_MEAS_PARAM_DATE);
    printMeasParam(1, MENU_MEAS_PARAM_TIME);

    if (EnterParam.isEnable())
    {
        // ����� �������� ������
        snprintf_P(&vLCDbuf[20 * lineParam_], 21, name, cursorLine_);
        eMENU_ENTER_PARAM stat = enterValue();

        if (stat == MENU_ENTER_PARAM_READY)
        {
            // ����������� ��������� �������� �� ���� �����
            //          sParam.txComBuf.setInt8(BIN_TO_BCD(EnterParam.getValueEnter()),
            //                  EnterParam.getDopValue());
            uint8_t t   = static_cast<uint8_t>(EnterParam.getDopValue());
            uint8_t val = static_cast<uint8_t>(EnterParam.getValueEnter());
            if (t <= 2)
            {
                // ���� ����
                // �������� ����������� ����� �� �������
                if (t == 0)
                {
                    // ���� ����, �������� ����, �.�. ����� ���� ����������
                    uint8_t month = BCD_TO_BIN(sParam.txComBuf.getInt8(1));
                    uint8_t day   = BCD_TO_BIN(sParam.txComBuf.getInt8(2));
                    if (day > sParam.DateTime.getNumDaysInMonth(month, val))
                    {
                        sParam.txComBuf.setInt8(0x01, 2);
                    }
                }
                else if (t == 1)
                {
                    // ���� ������, �������� ���-�� ������������� ����
                    uint8_t day = BCD_TO_BIN(sParam.txComBuf.getInt8(2));
                    if (day > sParam.DateTime.getNumDaysInMonth(val))
                    {
                        sParam.txComBuf.setInt8(0x01, 2);
                    }
                }
                sParam.txComBuf.setInt8(BIN_TO_BCD(val), t);
                sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.DateTime.getHour()), 3);
                sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.DateTime.getMinute()), 4);
                sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.DateTime.getSecond()), 5);
                sParam.txComBuf.addFastCom(EnterParam.com);
            }
            else if (t <= 5)
            {
                // ���� �������
                // �������� ����������� ���� �� �������
                sParam.txComBuf.setInt8(BIN_TO_BCD(val), t);
                sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.DateTime.getYear()), 0);
                sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.DateTime.getMonth()), 1);
                sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.DateTime.getDay()), 2);
                sParam.txComBuf.addFastCom(EnterParam.com);
            }
            sParam.txComBuf.setInt8(0, 6);  // �� ������ 0
            sParam.txComBuf.setInt8(0, 7);  //
            EnterParam.setDisable();
        }
    }
    else
        printPunkts();

    switch (key_)
    {
    case KEY_UP: cursorLineUp(); break;
    case KEY_DOWN: cursorLineDown(); break;

    case KEY_ENTER:
        enterFunc = &clMenu::enterValue;
        if (name == punkt1)
        {
            EnterParam.setEnable();
            EnterParam.setValueRange(0, 99);
            EnterParam.setValue(sParam.DateTime.getYear());
            EnterParam.setDopValue(0);
        }
        else if (name == punkt2)
        {
            EnterParam.setEnable();
            EnterParam.setValueRange(1, 12);
            EnterParam.setValue(sParam.DateTime.getMonth());
            EnterParam.setDopValue(1);
        }
        else if (name == punkt3)
        {
            EnterParam.setEnable();
            uint8_t max = sParam.DateTime.getNumDaysInMonth();
            EnterParam.setValueRange(1, max);
            EnterParam.setValue(sParam.DateTime.getDay());
            EnterParam.setDopValue(2);
        }
        else if (name == punkt4)
        {
            EnterParam.setEnable();
            EnterParam.setValueRange(0, 23);
            EnterParam.setValue(sParam.DateTime.getHour());
            EnterParam.setDopValue(3);
        }
        else if (name == punkt5)
        {
            EnterParam.setEnable();
            EnterParam.setValueRange(0, 59);
            EnterParam.setValue(sParam.DateTime.getMinute());
            EnterParam.setDopValue(4);
        }
        else if (name == punkt6)
        {
            EnterParam.setEnable();
            EnterParam.setValueRange(0, 59);
            EnterParam.setValue(sParam.DateTime.getSecond());
            EnterParam.setDopValue(5);
        }
        EnterParam.com = GB_COM_SET_TIME;
        EnterParam.setDisc(1);
        EnterParam.setFract(1);
        // �������� ������� �������� ���� � �������
        // ����� ����������� � ������� �������� � ���
        sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.DateTime.getYear()), 0);
        sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.DateTime.getMonth()), 1);
        sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.DateTime.getDay()), 2);
        sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.DateTime.getHour()), 3);
        sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.DateTime.getMinute()), 4);
        sParam.txComBuf.setInt8(BIN_TO_BCD(sParam.DateTime.getSecond()), 5);
        sParam.txComBuf.setInt8(0, 8);  //
        break;

    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlSetup;
        lvlCreate_ = true;
        break;
    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    default: break;
    }
}

/** ������� ����. ���������.
 *
 *  @param ���
 *  @return ���
 */
void clMenu::lvlMeasure()
{
    static const char title[] PROGMEM = "����\\���������";

    if (lvlCreate_)
    {
        lvlCreate_  = false;
        lineParam_  = 1;
        cursorLine_ = 0;

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        // ������������� �������
        // ����������� ������ �������� (�� ������ ������������)
        sParam.txComBuf.clear();
        sParam.txComBuf.addCom2(GB_COM_GET_MEAS);
        // 2 ������� ��������� ��� ���������� ������� ������ ���������� ����������
        sParam.txComBuf.addCom2(GB_COM_GET_FAULT);

        // ��� �����, ������������� � ����� �������� ��� ����������������
        sParam.txComBuf.addCom1(GB_COM_DEF_GET_LINE_TYPE);
        sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);
        sParam.txComBuf.addCom1(GB_COM_GET_DEVICE_NUM);
    }

    snprintf_P(&vLCDbuf[0], 21, title);

    for (uint_fast8_t i = 0; i < MAX_NUM_MEAS_PARAM_LVL; i++)
    {
        printMeasParam(lineParam_ * 2 + i, measParamLvl[i]);
    }

    if (sParam.typeDevice == AVANT_OPTO)
    {
        key_ = KEY_CANCEL;
    }

    switch (key_)
    {
    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlFirst;
        lvlCreate_ = true;
        break;

    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    default: break;
    }
}

/** ������� ����. ���� 1.
 *  @param ���
 *  @return ���
 */
void clMenu::lvlTest()
{
    static const char title[] PROGMEM       = "���������\\�����";
    static const char punkt1[] PROGMEM      = "���� �����������";
    static const char punkt2[] PROGMEM      = "���� ���������";
    static const char message[][21] PROGMEM = {
        // 12345678901234567890
        "    ��������� �     ",  //
        "   ����� �������    "   //
    };

    if (lvlCreate_)
    {
        lvlCreate_    = false;
        cursorLine_   = 1;
        cursorEnable_ = true;
        lineParam_    = 1;

        vLCDclear();
        vLCDdrawBoard(lineParam_);

        Punkts_.Clear();
        // ���� ����������� ��� ������� ����������� ��� ������
        if ((sParam.prd.status.isEnable()) || (sParam.def.status.isEnable()))
        {
            Punkts_.Add(punkt1);
        }
        // ���� ��������� ��� ������� ��������� ��� ������
        if ((sParam.prm.status.isEnable()) || (sParam.def.status.isEnable()))
        {
            Punkts_.Add(punkt2);
        }

        // �������������� �������
        sParam.txComBuf.clear();
        if (sParam.typeDevice != AVANT_OPTO)
        {
            // �������������
            sParam.txComBuf.addCom1(GB_COM_GET_COM_PRD_KEEP);
        }
    }

    PGM_P name = Punkts_.GetName(cursorLine_ - 1);

    snprintf_P(&vLCDbuf[0], 20, title);
    if (isMessage())
    {
        for (uint_fast8_t i = lineParam_ + 1; i <= NUM_TEXT_LINES; i++)
            clearLine(i);

        uint8_t poz = 40;
        for (uint_fast8_t i = 0; i < 2; i++, poz += 20)
            snprintf_P(&vLCDbuf[poz], 21, message[i]);
    }
    else
        printPunkts();

    eGB_REGIME reg = sParam.glb.status.getRegime();
    switch (key_)
    {
    case KEY_UP: cursorLineUp(); break;
    case KEY_DOWN: cursorLineDown(); break;

    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlFirst;
        lvlCreate_ = true;
        break;
    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    case KEY_ENTER:
        if (name == punkt1)
        {
            if ((reg == GB_REGIME_DISABLED) || (reg == GB_REGIME_TEST_2))
            {
                //
                sParam.txComBuf.setInt8(0, 0);
                sParam.txComBuf.setInt8(0, 0);
                sParam.txComBuf.addFastCom(GB_COM_SET_REG_TEST_1);
                lvlMenu    = &clMenu::lvlTest1;
                lvlCreate_ = true;
            }
            else if (reg == GB_REGIME_TEST_1)
            {
                lvlMenu    = &clMenu::lvlTest1;
                lvlCreate_ = true;
            }
        }
        else if (name == punkt2)
        {
            if ((reg == GB_REGIME_DISABLED) || (reg == GB_REGIME_TEST_1))
            {
                sParam.txComBuf.addFastCom(GB_COM_SET_REG_TEST_2);
                lvlMenu    = &clMenu::lvlTest2;
                lvlCreate_ = true;
            }
            else if (reg == GB_REGIME_TEST_2)
            {
                lvlMenu    = &clMenu::lvlTest2;
                lvlCreate_ = true;
            }
        }
        // ���� ������� ���� (�.�. ����� �������� �����) �� ���������,
        // ������� ���������
        if (lvlMenu == &clMenu::lvlTest)
            printMessage();
        break;

    default: break;
    }
}

/** ������� ����. ���� 1.
 *  ���� � ������� 1 ������� ����� ������� ����� ����������� � ������ ������
 *  ��������� ����� � ���������� ����� ����.
 *  @param ���
 *  @return ���
 */
void clMenu::lvlTest1()
{
    static const char title[] PROGMEM  = "�����\\����������";
    static const char punkt1[] PROGMEM = "������� �����������";
    static uint8_t cnt = 0;  // ������� �� ������ ��� ��������� ������
    eGB_TYPE_DEVICE device = sParam.typeDevice;

    if (lvlCreate_)
    {

        lvlCreate_    = false;
        cursorLine_   = 1;
        cursorEnable_ = true;
        // � ������ �� ���� �������� ���.���������
        lineParam_ = (device == AVANT_OPTO) ? 1 : 2;

        vLCDclear();
        vLCDdrawBoard(lineParam_);
        cnt = 0;

        // �������������� �������
        sParam.txComBuf.clear();
        sParam.txComBuf.addCom1(GB_COM_GET_MEAS);  // ���������
        sParam.txComBuf.addCom2(GB_COM_GET_TEST);  // �������

        // ������� ��� ������
        sParam.test.clear();
        if (device == AVANT_R400M)
        {
            sParam.test.addSignalToList(GB_SIGNAL_RZ);
            eGB_COMP_R400M comp = sParam.glb.getCompR400m();
            if (comp == GB_COMP_R400M_AVANT)
            {
                sParam.test.addSignalToList(GB_SIGNAL_CF);
            }
            else if (comp == GB_COMP_R400M_LINER)
            {
                sParam.test.addSignalToList(GB_SIGNAL_CF1);
                sParam.test.addSignalToList(GB_SIGNAL_CF2);
                sParam.test.addSignalToList(GB_SIGNAL_CF3);
                sParam.test.addSignalToList(GB_SIGNAL_CF4);
            }
        }
        else if (device == AVANT_RZSK)
        {
            uint8_t num_com = sParam.prd.getNumCom();
            // ����������� ���������� ������
            num_com = num_com > 8 ? 8 : num_com;

            if (sParam.def.status.isEnable())
            {
                sParam.test.addSignalToList(GB_SIGNAL_CF_NO_RZ);
                sParam.test.addSignalToList(GB_SIGNAL_CF_RZ);
                sParam.test.addSignalToList(GB_SIGNAL_CF2_NO_RZ);
                sParam.test.addSignalToList(GB_SIGNAL_CF2_RZ);
                for (uint8_t i = 0; i < num_com; i++)
                {
                    uint8_t signal = GB_SIGNAL_COM1_NO_RZ + i;
                    sParam.test.addSignalToList(eGB_TEST_SIGNAL(signal));
                }
                for (uint8_t i = 0; i < num_com; i++)
                {
                    uint8_t signal = GB_SIGNAL_COM1_RZ + i;
                    sParam.test.addSignalToList(eGB_TEST_SIGNAL(signal));
                }
            }
            else
            {
                sParam.test.addSignalToList(GB_SIGNAL_CF1);
                sParam.test.addSignalToList(GB_SIGNAL_CF2);
                for (uint8_t i = 0; i < num_com; i++)
                {
                    uint8_t signal = GB_SIGNAL_COM1 + i;
                    sParam.test.addSignalToList(eGB_TEST_SIGNAL(signal));
                }
            }
        }
        else if (device == AVANT_K400)
        {
            sParam.test.addSignalToList(GB_SIGNAL_CF1);
            sParam.test.addSignalToList(GB_SIGNAL_CF2);
            uint8_t num_com = sParam.prd.getNumCom();
            for (uint_fast8_t i = 0; i < num_com; i++)
            {
                eGB_TEST_SIGNAL signal = static_cast<eGB_TEST_SIGNAL>(GB_SIGNAL_COM1 + i);
                sParam.test.addSignalToList(signal);
            }
        }
        else if (device == AVANT_OPTO)
        {
            sParam.test.addSignalToList(GB_SIGNAL_CS);
            if (sParam.def.status.isEnable())
            {
                sParam.test.addSignalToList(GB_SIGNAL_RZ);
            }

            if (sParam.glb.getTypeOpto() == TYPE_OPTO_RING_UNI)
            {
                for (uint_fast8_t i = 0; i < MAX_NUM_COM_RING; i++)
                {
                    eGB_TEST_SIGNAL signal = static_cast<eGB_TEST_SIGNAL>(GB_SIGNAL_COM1A + i);
                    sParam.test.addSignalToList(signal);
                }
            }
            else
            {
                uint8_t num_com = sParam.prd.getNumCom();
                for (uint_fast8_t i = 0; i < num_com; i++)
                {
                    eGB_TEST_SIGNAL signal = static_cast<eGB_TEST_SIGNAL>(GB_SIGNAL_COM1 + i);
                    sParam.test.addSignalToList(signal);
                }
            }
        }
    }

    // ����� �� ����� ���������� ����������, ���� ��� �� ������
    if (device != AVANT_OPTO)
    {
        printMeasParam(2, MENU_MEAS_PARAM_UOUT);
        printMeasParam(3, MENU_MEAS_PARAM_IOUT);
    }

    snprintf_P(&vLCDbuf[0], 21, title);
    snprintf_P(&vLCDbuf[lineParam_ * 20], 21, punkt1);

    // ����� �������� �����, ��� ������ �������� ������� �������
    // ����� �� ����� ���� ������� ��� ������� ���������
    sParam.txComBuf.setInt8(0, 0);
    if (EnterParam.isEnable())
    {
        // ���� ������ �������� ���������
        eMENU_ENTER_PARAM stat = enterValue();

        if (stat == MENU_ENTER_PARAM_READY)
        {
            // ������� ������(�)
            uint8_t sig = EnterParam.listValue[EnterParam.getValue()];
            uint8_t rz  = 0;
            uint8_t cf  = 0;

            sParam.test.getBytes(cf, rz, static_cast<eGB_TEST_SIGNAL>(sig));
            // �.�. � ��� ��� ��������� �������� ���� ��� ������ ������
            // ��� ������ �� ������� ��������� �������� ����� �������
            // ������� � ������ ������� ��� ������ �� �����
            // !!! ��� �������� ������� ���� ��������� ������ � ������
            // ��
            sParam.txComBuf.setInt8(2, 0);   // ������ ��
            sParam.txComBuf.setInt8(rz, 1);  // ������� ������ ��
            sParam.txComBuf.addFastCom(EnterParam.com);
            // ��
            sParam.txComBuf.setInt8(1, 0);   // ������ ��
            sParam.txComBuf.setInt8(cf, 1);  // ������� ������ ��
            sParam.txComBuf.addFastCom(EnterParam.com);

            EnterParam.setDisable();
        }
    }
    else
    {
        uint8_t poz   = 100;
        uint8_t value = sParam.test.getCurrentSignal();

        poz += snprintf_P(&vLCDbuf[poz], 21, fcValue);
        snprintf_P(&vLCDbuf[poz],
                   VALUE_STRING_SIZE,
                   getTextValue(GB_PARAM_OTHER_TEST_SIGNAL, value));
    }

    // ����� �� ����� ��� �������������� ������
    if (sParam.glb.status.getRegime() != GB_REGIME_TEST_1)
    {
        if (++cnt >= TIME_TEST_EXIT)
        {
            key_ = KEY_CANCEL;
        }
    }
    else
    {
        cnt = 0;
    }

    switch (key_)
    {
    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlTest;
        lvlCreate_ = true;
        break;
    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    case KEY_ENTER:
        EnterParam.setParam(GB_PARAM_OTHER_TEST_SIGNAL);
        EnterParam.setValue(0);
        EnterParam.setEnable(MENU_ENTER_PARAM_LIST_2);
        EnterParam.setValueRange(0, sParam.test.getNumSignals() - 1);
        EnterParam.listValue = sParam.test.signalList;
        EnterParam.com       = GB_COM_SET_REG_TEST_1;
        break;

    default: break;
    }

    // ����� ������ ������������ ��� ������ ������ ���� ������� ����� ����1
    if (lvlMenu != &clMenu::lvlTest1)
    {
        if (sParam.glb.status.getRegime() == GB_REGIME_TEST_1)
        {
            sParam.txComBuf.addFastCom(GB_COM_SET_REG_DISABLED);
        }
    }
}

/** ������� ����. ���� 2.
 *  ���� � ������� 1 ������� ����� ������� ����� ����������� � ������ ������
 *  ��������� ����� � ���������� ����� ����.
 *  @param ���
 *  @return ���
 */
void clMenu::lvlTest2()
{
    static const char title[] PROGMEM  = "�����\\��������";
    static const char punkt1[] PROGMEM = "������� ���������";
    static const char prm1[] PROGMEM   = "���1: ";
    static const char prm2[] PROGMEM   = "���2: ";

    static uint8_t cnt = 0;  // ������� �� ������ ��� ��������� ������
    eGB_TYPE_DEVICE device = sParam.typeDevice;

    if (lvlCreate_)
    {
        lvlCreate_ = false;

        cursorLine_   = 1;
        cursorEnable_ = true;

        if (device == AVANT_OPTO)
        {
            // � ������ ���������� ��������� �������� �� ����
            lineParam_ = 1;
        }
        else if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3)
        {
            // � ���� �������� ����� 2 ������ ����������
            lineParam_ = 3;
        }
        else
        {
            // ����� ���� ������ ����������
            lineParam_ = 2;
        }
        vLCDclear();
        vLCDdrawBoard(lineParam_);
        cnt = 0;

        // �������������� �������
        sParam.txComBuf.clear();
        if (device != AVANT_OPTO)
        {
            sParam.txComBuf.addCom1(GB_COM_GET_MEAS);  // ���������
        }
        sParam.txComBuf.addCom2(GB_COM_GET_TEST);  // �������
    }

    // ����� �� ����� ���������� ����������, ���� ��� �� ������
    if (device != AVANT_OPTO)
    {
        if (sParam.def.getNumDevices() == GB_NUM_DEVICES_3)
        {
            if (sParam.typeDevice == AVANT_R400M)
            {
                printMeasParam(2, MENU_MEAS_PARAM_UC1);
                printMeasParam(3, MENU_MEAS_PARAM_UZ);
                printMeasParam(4, MENU_MEAS_PARAM_UC2);
            }
            else if (sParam.def.status.isEnable())
            {
                printMeasParam(2, MENU_MEAS_PARAM_UC1);
                printMeasParam(3, MENU_MEAS_PARAM_UZ1);
                printMeasParam(4, MENU_MEAS_PARAM_UC2);
                printMeasParam(5, MENU_MEAS_PARAM_UZ2);
            }
            else
            {
                printMeasParam(2, MENU_MEAS_PARAM_UC1);
                printMeasParam(3, MENU_MEAS_PARAM_UN1);
                printMeasParam(4, MENU_MEAS_PARAM_UC2);
                printMeasParam(5, MENU_MEAS_PARAM_UN2);
            }
        }
        else
        {
            printMeasParam(2, MENU_MEAS_PARAM_UC);
            if (sParam.def.status.isEnable())
            {
                printMeasParam(3, MENU_MEAS_PARAM_UZ);
            }
            else
            {
                printMeasParam(3, MENU_MEAS_PARAM_UN);
            }
        }
    }

    snprintf_P(&vLCDbuf[0], 21, title);
    snprintf_P(&vLCDbuf[lineParam_ * 20], 21, punkt1);

    uint8_t poz = 100;
    // � �400� ������ ���� ��������, �� �������� �� ���-�� ��������� � �����
    if ((sParam.def.getNumDevices() == GB_NUM_DEVICES_3) && (sParam.typeDevice != AVANT_R400M))
    {

        poz = 80;
        poz += snprintf_P(&vLCDbuf[poz], VALUE_STRING_SIZE, prm1);

        uint8_t value = sParam.test.getCurrentSignal();
        snprintf_P(&vLCDbuf[poz],
                   VALUE_STRING_SIZE,
                   getTextValue(GB_PARAM_OTHER_TEST_SIGNAL, value));

        poz = 100;
        poz += snprintf_P(&vLCDbuf[poz], VALUE_STRING_SIZE, prm2);

        value = sParam.test.getCurrentSignal2();
        snprintf_P(&vLCDbuf[poz],
                   VALUE_STRING_SIZE,
                   getTextValue(GB_PARAM_OTHER_TEST_SIGNAL, value));
    }
    else
    {
        poz = 100;
        poz += snprintf_P(&vLCDbuf[poz], VALUE_STRING_SIZE, fcValue);

        uint8_t value = sParam.test.getCurrentSignal();
        snprintf_P(&vLCDbuf[poz],
                   VALUE_STRING_SIZE,
                   getTextValue(GB_PARAM_OTHER_TEST_SIGNAL, value));
    }

    // ����� �� ����� ��� ��������������� ������
    if (sParam.glb.status.getRegime() != GB_REGIME_TEST_2)
    {
        if (++cnt >= TIME_TEST_EXIT)
        {
            key_ = KEY_CANCEL;
        }
    }
    else
    {
        cnt = 0;
    }

    switch (key_)
    {
    case KEY_CANCEL:
        lvlMenu    = &clMenu::lvlTest;
        lvlCreate_ = true;
        break;
    case KEY_MENU:
        lvlMenu    = &clMenu::lvlStart;
        lvlCreate_ = true;
        break;

    default: break;
    }

    // ����� ������ ������������ ��� ������ ������ ���� ������� ����� ����2
    if (lvlMenu != &clMenu::lvlTest2)
    {
        if (sParam.glb.status.getRegime() == GB_REGIME_TEST_2)
        {
            sParam.txComBuf.addFastCom(GB_COM_SET_REG_DISABLED);
        }
    }
}

/** ���� ���������.
 *  @param ���
 *  @return True - �� ���������
 */
eMENU_ENTER_PARAM clMenu::enterValue()
{
    static const char enterList[] PROGMEM = "����: ";
    static const char enterInt[] PROGMEM  = "����: %01d";
    static const char enterUcor[] PROGMEM = "����: %01u.%01u";

    uint8_t len = 0;
    uint8_t pos = 100;
    clearLine(NUM_TEXT_LINES);
    eMENU_ENTER_PARAM status = EnterParam.getStatus();
    if (status == MENU_ENTER_PARAM_INT)
    {
        int16_t val = EnterParam.getValue();
        snprintf_P(&vLCDbuf[pos], NAME_PARAM_LENGHT, enterInt, val);
    }
    else if (status == MENU_ENTER_PARAM_U_COR)
    {
        uint16_t val = static_cast<uint16_t>(EnterParam.getValue());
        snprintf_P(&vLCDbuf[pos], NAME_PARAM_LENGHT, enterUcor, val / 10, val % 10);
    }
    else if (status == MENU_ENTER_PARAM_LIST)
    {
        uint8_t val = static_cast<uint8_t>(EnterParam.getValue());
        len         = snprintf_P(&vLCDbuf[pos], NAME_PARAM_LENGHT, enterList);
        snprintf_P(&vLCDbuf[pos + len],
                   NAME_PARAM_LENGHT - len,
                   getTextValue(EnterParam.getParam(), val));
    }
    else if (status == MENU_ENTER_PARAM_LIST_2)
    {
        uint8_t val = EnterParam.listValue[EnterParam.getValue()];
        len         = snprintf_P(&vLCDbuf[pos], NAME_PARAM_LENGHT, enterList);
        snprintf_P(&vLCDbuf[pos + len],
                   NAME_PARAM_LENGHT - len,
                   getTextValue(EnterParam.getParam(), val));
    }
    else
    {
        key_ = KEY_CANCEL;
    }

    eKEY tkey = key_;
    key_      = KEY_NO;
    switch (tkey)
    {
    case KEY_CANCEL: EnterParam.setDisable(); break;

    case KEY_MENU:
        EnterParam.setDisable();
        key_ = KEY_MENU;
        break;

    case KEY_UP: EnterParam.incValue(timePressKey()); break;
    case KEY_DOWN: EnterParam.decValue(timePressKey()); break;

    case KEY_ENTER: EnterParam.setEnterValueReady(); break;

    default: break;
    }

    return EnterParam.getStatus();
}

/** ���� ������
 *  @param ���
 *  @return True - �� ���������
 */
eMENU_ENTER_PARAM clMenu::enterPassword()
{
    eMENU_ENTER_PARAM status = EnterParam.getStatus();
    if (status == MENU_ENTER_PARAM_MESSAGE)
    {
        for (uint_fast8_t i = lineParam_ + 1; i <= NUM_TEXT_LINES; i++)
            clearLine(i);

        // ����� ��������� �� ��� ���, ���� ������� ������� �� ���������
        // ����� ������� � �������� ����� ����
        if (EnterParam.cnt_ < TIME_MESSAGE)
        {
            static const char message[3][21] PROGMEM = { //       12345678901234567890
                                                         "       ������       ",  //
                                                         "    ������������    ",  //
                                                         "       ������       "
            };

            EnterParam.cnt_++;
            key_ = KEY_NO;

            uint8_t poz = 40;
            for (uint_fast8_t i = 0; i < 3; i++, poz += 20)
                snprintf_P(&vLCDbuf[poz], 21, message[i]);
        }
        else
        {
            key_ = KEY_CANCEL;
        }
    }
    else
    {

        uint8_t poz = 100;
        clearLine(NUM_TEXT_LINES);

        uint16_t val = static_cast<uint16_t>(EnterParam.getValue());

        if (status == MENU_ENTER_PASSWORD)
        {
            static const char enter[] PROGMEM = "������: %04u";
            snprintf_P(&vLCDbuf[poz], 21, enter, val);
        }
        else if (status == MENU_ENTER_PASSWORD_NEW)
        {
            static const char enterNew[] PROGMEM = "����� ������: %04u";
            snprintf_P(&vLCDbuf[poz], 21, enterNew, val);
        }
        else
        {
            key_ = KEY_CANCEL;
        }
    }

    eKEY tkey = key_;
    key_      = KEY_NO;
    switch (tkey)
    {
    case KEY_CANCEL: EnterParam.setDisable(); break;

    case KEY_MENU:
        EnterParam.setDisable();
        key_ = KEY_MENU;
        break;

    case KEY_ENTER:
        if (status == MENU_ENTER_PASSWORD)
            EnterParam.setEnterValueReady(MENU_ENTER_PASSWORD_READY);
        else if (status == MENU_ENTER_PASSWORD_NEW)
            EnterParam.setEnterValueReady(MENU_ENTER_PASSWORD_N_READY);
        else
            EnterParam.setDisable();
        break;

    case KEY_UP:
        EnterParam.setDisc(1000);
        EnterParam.incValue();
        break;
    case KEY_RIGHT:
        EnterParam.setDisc(100);
        EnterParam.incValue();
        break;
    case KEY_DOWN:
        EnterParam.setDisc(10);
        EnterParam.incValue();
        break;
    case KEY_LEFT:
        EnterParam.setDisc(1);
        EnterParam.incValue();
        break;

    default: break;
    }

    return EnterParam.getStatus();
}

/** ����� �� ����� ������� ������� ���� � �������
 *
 *  @param ���
 *  @return ���
 */
void clMenu::printPunkts()
{
    // ����� �� ����� ������� �������� ����
    // ����� ������������� �� ��������� ������� ������,
    // ���� ��������� ������

    // ���-�� ������������ ����� �� ������
    uint8_t numLines = NUM_TEXT_LINES - lineParam_;
    // ���� ����� ������� ������ ������, ��� ��������� ����� �� ������
    // �� �������� �� ����� �������� � (������� ����� - ���.�� �����)
    // ����� � ������
    uint8_t punkt_index = (cursorLine_ > numLines) ? cursorLine_ - numLines : 0;

    for (uint_fast8_t line = lineParam_; line < NUM_TEXT_LINES; line++)
    {
        char* position = &vLCDbuf[20 * line];

        uint8_t len = snprintf_P(position, DISPLAY_ROW_SIZE + 1, PSTR("%u. "), punkt_index + 1);

        if (cursorEnable_ && (cursorLine_ == (punkt_index + 1)))
        {
            *(position + len - 1) = '*';
        }

        PGM_P text = Punkts_.GetName(punkt_index);
        if (text != nullptr)
        {
            snprintf_P(position + len, DISPLAY_ROW_SIZE - len + 1, text);
        }

        if (++punkt_index >= Punkts_.GetLen())
            break;
    }
}

/** ����� � ��������� ����� ������������� ���������.
 *  � ����� ������ ��������� ��� ���������.
 *  @param poz ������� �������
 *  @arg [0, 12), 0 ������ ������ �����, 11 - ������� ������
 *  @param par ������������ ��������
 *  @return ���
 */
void clMenu::printMeasParam(uint8_t poz, eMENU_MEAS_PARAM par)
{
    static const char fcUout[] PROGMEM = "U=%02u.%01u�";  // ���������� ������.
    static const char fcIout[] PROGMEM = "I=%03u��";      // ��� ������.
    static const char fcRout[] PROGMEM = "R=%03u��";      // ������������� �����.
    static const char fcUz[] PROGMEM   = "U�=%02d��";     // ����� �� ������.
    static const char fcUzx[] PROGMEM = "U�%u=%02d��";  // ����� �� ������ � �������.
    static const char fcUcf[] PROGMEM  = "U�=%02d��";    // ����� �� �C.
    static const char fcUcfx[] PROGMEM = "U�%u=%02d��";  // ����� �� �C � �������.
    static const char fcUn[] PROGMEM   = "U�=%02d��";    // ������� �����.
    static const char fcUnx[] PROGMEM = "U�%u=%02d��";  // ������� ����� � �������.
    static const char fcSd[] PROGMEM   = "S�=%02u�";        // �������� � �������.
    static const char fcDate[] PROGMEM = "%02u.%02u.%02u";  // ����.
    static const char fcTime[] PROGMEM = "%02u:%02u:%02u";  // �����.
    static const char fcD[] PROGMEM =
        "D=%02d��";  // ����� �� ����.������� (��������) ��� ��������� ������/������ (��������)
    static const char fcTemper[] PROGMEM = "T=%02d�C";  // �����������
    static const char fcFreqDev[] PROGMEM = "dF=%02d��";  // ���������� ������ �� �� ���

    // � ������������ �������� ���� ������ � ��������� ������� ��
    // �������� ������ ��������:
    // - ��� �������� 1 ���������: 2 � 3
    // - ��� �������� 2 ���������: 1 � 3
    // - ��� �������� 3 ���������: 1 � 2
    uint8_t ndevice1 = (sParam.glb.getDeviceNum() == 1) ? 2 : 1;
    uint8_t ndevice2 = (sParam.glb.getDeviceNum() == 3) ? 2 : 3;

    // �������� �� ������������ �������
    // 10 - ���-�� �������� ���������� �� ������ ��� 1 ��������
    if (poz < 12)
    {
        poz = (poz * 10) + 1;

        switch (par)
        {
        case MENU_MEAS_PARAM_DATE:
            snprintf_P(&vLCDbuf[poz],
                       11,
                       fcDate,
                       sParam.DateTime.getDay(),
                       sParam.DateTime.getMonth(),
                       sParam.DateTime.getYear());
            break;
        case MENU_MEAS_PARAM_TIME:
            snprintf_P(&vLCDbuf[poz],
                       11,
                       fcTime,
                       sParam.DateTime.getHour(),
                       sParam.DateTime.getMinute(),
                       sParam.DateTime.getSecond());
            break;
        case MENU_MEAS_PARAM_UZ:
            snprintf_P(&vLCDbuf[poz], 11, fcUz, sParam.measParam.getVoltageDef());
            break;

            // � 3-� �������� ����� ���� Uz1 == Uz, Uz2
        case MENU_MEAS_PARAM_UZ1:
            snprintf_P(&vLCDbuf[poz], 11, fcUzx, ndevice1, sParam.measParam.getVoltageDef());
            break;
        case MENU_MEAS_PARAM_UZ2:
            snprintf_P(&vLCDbuf[poz], 11, fcUzx, ndevice2, sParam.measParam.getVoltageDef2());
            break;

        case MENU_MEAS_PARAM_UC:
            snprintf_P(&vLCDbuf[poz], 11, fcUcf, sParam.measParam.getVoltageCf());
            break;

            // � 3-� �������� ����� ���� Uk1 == Uk, Uk2
        case MENU_MEAS_PARAM_UC1:
            snprintf_P(&vLCDbuf[poz], 11, fcUcfx, ndevice1, sParam.measParam.getVoltageCf());
            break;
        case MENU_MEAS_PARAM_UC2:
            snprintf_P(&vLCDbuf[poz], 11, fcUcfx, ndevice2, sParam.measParam.getVoltageCf2());
            break;

        case MENU_MEAS_PARAM_UOUT:
            snprintf_P(&vLCDbuf[poz],
                       11,
                       fcUout,
                       sParam.measParam.getVoltageOutInt(),
                       sParam.measParam.getVoltageOutFract());
            break;

        case MENU_MEAS_PARAM_IOUT:
            snprintf_P(&vLCDbuf[poz], 11, fcIout, sParam.measParam.getCurrentOut());
            break;

        case MENU_MEAS_PARAM_ROUT:
            snprintf_P(&vLCDbuf[poz], 11, fcRout, sParam.measParam.getResistOut());
            break;

        case MENU_MEAS_PARAM_UN:
            snprintf_P(&vLCDbuf[poz], 11, fcUn, sParam.measParam.getVoltageNoise());
            break;
            // � 3-� �������� ����� ���� U�1 == U�, U�2
        case MENU_MEAS_PARAM_UN1:
            snprintf_P(&vLCDbuf[poz], 11, fcUnx, ndevice1, sParam.measParam.getVoltageNoise());
            break;
        case MENU_MEAS_PARAM_UN2:
            snprintf_P(&vLCDbuf[poz], 11, fcUnx, ndevice2, sParam.measParam.getVoltageNoise2());
            break;

        case MENU_MEAS_PARAM_SD:
            snprintf_P(&vLCDbuf[poz], 11, fcSd, sParam.measParam.getPulseWidth());
            break;

        case MENU_MEAS_PARAM_D: snprintf_P(&vLCDbuf[poz], 11, fcD, sParam.measParam.getD()); break;

        case MENU_MEAS_PARAM_TEMPERATURE:
            snprintf_P(&vLCDbuf[poz], 11, fcTemper, sParam.measParam.getTemperature());
            break;

        case MENU_MEAS_PARAM_DF:
            snprintf_P(&vLCDbuf[poz], 11, fcFreqDev, sParam.measParam.getFreqDev());
            break;

        case MENU_MEAS_PARAM_NO:
            // ������ �� ������
            break;
        }
    }
}

/** ����� �� ����� �������� ��������� ����������.
 *  ���� ���� ����� ������������� - ��������� ���������� ����������� �����
 *  ������������ � ��� (���� ���� ����).
 *  ����� ����������� ������� ������������� ���������� � ���� ����, ���������
 *  �� ���� �� ���������.
 *
 *  ���� ���� ����� �������������� - ��������� ��������� ��������� � �����������
 *  ������� ��������������. ���� �������������� ���������, ��������� ������ ���.
 *  ����� ����������� ������� �������������� ��� ���������� � ���� ����,
 *  �������� �� ���� �� ���������.
 *
 *  ���� ������ ������� ��������������/��������������, �� ��������� �������
 *  ��������� � ����� ������ ����������.
 *
 *  ��� ���� � ������������� ����� � ������ ��������� ������ "��������",
 *  �� ����� ��������� ������� ����� ��.
 *
 *  @param poz ��������� ������� � ������ ������ ���
 *  @param device ������ ��� �������� ����������
 *  @return ���
 */
void clMenu::printDevicesStatus(uint8_t poz, TDeviceStatus* device)
{
    static const char fcFaults[] PROGMEM   = "������. %c-%04X";
    static const char fcWarnings[] PROGMEM = "�������. %c-%04X";

    PGM_P*        text;
    uint_fast8_t  x                            = 0;
    uint_fast16_t y                            = 0;
    char          tmp[SIZE_OF(fcRemoteNum[0])] = "";

    snprintf_P(&vLCDbuf[poz], 4, device->name);
    poz += 3;
    snprintf(&vLCDbuf[poz], 2, ":");
    poz += 1;

    if (sParam.glb.status.getNumFaults() != 0)
    {
        if (blink_)
        {
            text = sParam.glb.status.faultText;
            x    = sParam.glb.status.getFault();
            snprintf_P(&vLCDbuf[poz], 17, text[x]);
        }
        else
        {
            y = sParam.glb.status.getFaults();
            snprintf_P(&vLCDbuf[poz], 17, fcFaults, 'g', y);
        }
    }
    else if (device->getNumFaults() != 0)
    {
        if (blink_)
        {
            text = device->faultText;
            x    = device->getFault();
            y    = device->getRemoteNumber();
            strncpy_P(tmp, fcRemoteNum[y], SIZE_OF(tmp) - 1);
            snprintf_P(&vLCDbuf[poz], 17, text[x], tmp);
        }
        else
        {
            y = device->getFaults();
            snprintf_P(&vLCDbuf[poz], 17, fcFaults, 'l', y);
        }
    }
    else if ((sParam.glb.status.getNumWarnings() != 0) && (blink_))
    {
        if (sParam.glb.status.getNumWarnings() == 1)
        {
            text = sParam.glb.status.warningText;
            x    = sParam.glb.status.getWarning();
            snprintf_P(&vLCDbuf[poz], 17, text[x]);
        }
        else
        {
            y = sParam.glb.status.getWarnings();
            snprintf_P(&vLCDbuf[poz], 17, fcWarnings, 'g', y);
        }
    }
    else if ((device->getNumWarnings() != 0) && (blink_))
    {
        if (device->getNumWarnings() == 1)
        {
            text = device->warningText;
            x    = device->getWarning();
            y    = device->getRemoteNumber();
            strncpy_P(tmp, fcRemoteNum[y], SIZE_OF(tmp) - 1);
            snprintf_P(&vLCDbuf[poz], 17, text[x], tmp);
        }
        else
        {
            y = device->getWarnings();
            snprintf_P(&vLCDbuf[poz], 17, fcWarnings, 'l', y);
        }
    }
    else
    {
        text = device->stateText;
        poz += 1 + snprintf_P(&vLCDbuf[poz], 9, fcRegime[device->getRegime()]);

        uint8_t state = device->getState();
        snprintf_P(&vLCDbuf[poz], 9, text[state], device->getDopByte());
    }
}

/** ����� � ������ ���� "�����" �������� ������ ���������
 *  @param poz ��������� ������� � ������ ������ ���
 *  @param device ������ ��� �������� ����������
 *  @return ���
 */
void clMenu::printDevicesRegime(uint8_t poz, TDeviceStatus* device)
{
    snprintf_P(&vLCDbuf[poz], 4, device->name);
    poz += 3;
    snprintf(&vLCDbuf[poz], 2, ":");
    poz += 1;
    snprintf_P(&vLCDbuf[poz], 9, fcRegime[device->getRegime()]);
}

// ����� �� ����� �������� ������ � �� ���������� ��� ���������� ���-��.
void clMenu::printParam()
{
    const uint8_t MAX_CHARS = 21;

    snprintf_P(&vLCDbuf[20],
               MAX_CHARS,
               PSTR("��������:%u �����:%u"),
               sParam.local.getNumOfCurrParam(),
               sParam.local.getNumOfParams());

    snprintf_P(&vLCDbuf[40], MAX_CHARS, getNameOfParam(sParam.local.getParam()));

    printSameNumber(60);
    printRange(80);
    printValue(100);
}

// ����� �� ����� �������� ������ � �� ���������� ��� ���������� ���-��.
void clMenu::printSameNumber(uint8_t pos)
{
    const uint8_t MAX_CHARS = 21;

    if (sParam.local.getNumOfSameParams() > 1)
    {
        uint8_t   len = 0;
        eGB_PARAM p   = sParam.local.getParam();
        uint8_t   val = sParam.local.getNumOfCurrSameParam();
        uint8_t   max = sParam.local.getNumOfSameParams();

        if (p == GB_PARAM_RING_COM_TRANSIT)
        {
            // ��� ���������� ������ ������ ������ ��������� �������� ���� 16A/32C
            len = snprintf_P(&vLCDbuf[pos], VALUE_STRING_SIZE, PSTR("�����: "));
            len += snprintf_P(&vLCDbuf[pos + len], 4, getTextValue(GB_PARAM_RING_COM_REC, val));
            vLCDbuf[pos + len++] = '/';
            snprintf_P(&vLCDbuf[pos + len], 4, getTextValue(GB_PARAM_RING_COM_REC, max));
        }
        else
        {
            snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("�����: %u/%u"), val, max);
        }
    }
}

//  ����� �� ����� ��������� �������� ���������.
void clMenu::printRange(uint8_t pos)
{
    const uint8_t MAX_CHARS = 11;

    LocalParams* lp    = &sParam.local;
    eGB_PARAM    param = lp->getParam();
    int16_t      min   = lp->getMin();
    int16_t      max   = lp->getMax();

    uint8_t len       = 0;
    bool    dimension = false;
    pos += snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("��������: "));

    switch (getRangeType(param))
    {
    case Param::RANGE_LIST: snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("������")); break;

    case Param::RANGE_ON_OFF: snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("���./����.")); break;

    case Param::RANGE_INT:  // DOWN
        dimension = true;
    case Param::RANGE_INT_NO_DIM:
        len = snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("%d..%d"), min, max);
        break;

    case Param::RANGE_U_COR:  // DOWN
        max /= 10;
    case Param::RANGE_INT_PM:
        min       = 0;
        dimension = true;
        len       = snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("%d..�%d"), min, max);
        break;
    }

    if (dimension)
    {
        printDimension(pos + len, MAX_CHARS - len, param);
    }
}

// ����� �� ����� �������� �������� ���������.
void clMenu::printValue(uint8_t pos)
{
    const uint8_t MAX_CHARS = 11;

    int16_t   val   = sParam.local.getValue();
    eGB_PARAM param = sParam.local.getParam();

    pos += snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("��������: "));

    LocalParams::STATE state = sParam.local.getState();
    if (state == LocalParams::STATE_ERROR)
    {
        // ����� ���������� ��������
        if (blink_)
        {
            snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("������!!!"));
        }
        else
        {
            snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("%d"), val);
        }
    }
    else if (state == LocalParams::STATE_READ_PARAM)
    {
        if (blink_)
        {
            snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("������."));
        }
        else
        {
            snprintf_P(&vLCDbuf[pos], MAX_CHARS, PSTR("������.."));
        }
    }
    else
    {
        // ����� ����������� ��������
        uint8_t len       = 0;
        bool    dimension = false;
        switch (getParamType(param))
        {
        case Param::PARAM_BITES:  // DOWN
        case Param::PARAM_LIST:
            len = snprintf_P(&vLCDbuf[pos], MAX_CHARS, getTextValue(param, val));
            break;
        case Param::PARAM_I_COR:  // DOWN
        case Param::PARAM_INT:
            dimension = true;
            len       = snprintf(&vLCDbuf[pos], MAX_CHARS, "%d", val);
            break;
        case Param::PARAM_U_COR:
            dimension = true;
            if (val >= 0)
            {
                len = snprintf(&vLCDbuf[pos], MAX_CHARS, "%d.%d", val / 10, val % 10);
            }
            else
            {
                val = -val;
                len = snprintf(&vLCDbuf[pos], MAX_CHARS, "-%d.%d", val / 10, val % 10);
            }
            break;
        case Param::PARAM_NO: break;
        }

        if (dimension)
        {
            printDimension(pos + len, MAX_CHARS - len, param);
        }
    }
}

//
uint8_t clMenu::printDimension(uint8_t pos, uint8_t size, eGB_PARAM pn)
{
    uint8_t len = 0;

    if (pn < GB_PARAM_MAX)
    {
        Param::DIMENSION dimension = getDim(pn);
        if ((dimension > Param::DIM_NO) && (dimension < Param::DIM_MAX))
        {
            len = snprintf_P(&vLCDbuf[pos], size, fcDimension[dimension]);
        }
    }

    return len;
}

// ����� �� ����� ������ �� � ������� �� ��������� ��������.
void clMenu::printAc(uint8_t pos)
{
    eGB_COMP_R400M comp = sParam.glb.getCompR400m();
    eGB_TYPE_AC    ac   = sParam.def.getTypeAC();

    if (sParam.typeDevice == AVANT_R400M)
    {
        if (comp == GB_COMP_R400M_LINER)
        {
            // � �400� ������������� ������ ��������� �������� ""��-����"
            // �� "��-����"
            if (ac == GB_TYPE_AC_AUTO_NORM)
            {
                ac = GB_TYPE_AC_AUTO;
            }
        }
        else if (comp == GB_COMP_R400M_AVZK80)
        {
            // � �400� ������������� ����-80 ��������� �������� ""��-����"
            // �� "��-����"
            if (ac == GB_TYPE_AC_CHECK)
            {
                ac = GB_TYPE_AC_CHECK_1;
            }
        }
        else if (comp == GB_COMP_R400M_PVZ90)
        {
            if (ac == GB_TYPE_AC_CHECK)
            {
                ac = GB_TYPE_AC_CHECK_1;
            }
        }
    }

    pos += snprintf_P(&vLCDbuf[pos], 9, fcAcType[static_cast<uint8_t>(ac)]);

    if (ac != GB_TYPE_AC_OFF)
    {
        if (sParam.def.status.getRegime() == GB_REGIME_ENABLED)
        {
            uint16_t time = GetTimeAc(comp);
            uint8_t  hour = time / 3600;
            uint8_t  min  = (time % 3600) / 60;
            uint8_t  sec  = time % 60;
            snprintf_P(&vLCDbuf[pos + 1], 11, fcTimeToAc, hour, min, sec);
        }
    }
}

// ��������� ���������� ��� ����� �������� � ����������.
void clMenu::enterParameter()
{
    LocalParams* lp    = &sParam.local;
    eGB_PARAM    param = lp->getParam();

    // �������� ������ ��� ������
    if (lp->isReadOnly())
        return;

    if ((getChangeReg(param) == Param::CHANGE_REG_DISABLE)
        && (sParam.glb.status.getRegime() != GB_REGIME_DISABLED))
    {
        printMessage();
    }
    else
    {

        switch (getParamType(param))
        {
        case Param::PARAM_BITES:  // DOWN
        case Param::PARAM_LIST: EnterParam.setEnable(MENU_ENTER_PARAM_LIST); break;
        case Param::PARAM_I_COR:  // DOWN
        case Param::PARAM_INT: EnterParam.setEnable(MENU_ENTER_PARAM_INT); break;
        case Param::PARAM_U_COR: EnterParam.setEnable(MENU_ENTER_PARAM_U_COR); break;
        case Param::PARAM_NO: break;
        }

        if (EnterParam.isEnable())
        {

            int16_t min = lp->getMin();
            int16_t val = lp->getValue();
            int16_t max = lp->getMax();

            // ��� ����� �������� ��������� ���� � ����������
            // ����������� �������� ������ 0 , � ��������� ��������
            // ������������ ������ �������� ����������/����, � �� ���������.
            // ���� ��� ���� ��������� ��������, �� �������� ���� ������ 0.
            // ����� ��� ������ ���� ���� - ������� ������� ���������.
            //
            // ��� ��������� ���������� � ������ ������ �������� ��������,
            // ��������������� �������.
            if (getParamType(param) == Param::PARAM_I_COR)
            {
                min = 0;
                val = static_cast<int16_t>(sParam.measParam.getCurrentOut());
                if ((val < min) || (val > max))
                {
                    val = 0;
                }
                if (lp->getState() != LocalParams::STATE_NO_ERROR)
                {
                    val = 0;
                    max = 0;
                }
            }
            else if (getParamType(param) == Param::PARAM_U_COR)
            {
                min = 0;
                val = static_cast<int16_t>(sParam.measParam.getVoltageOut());
                if ((val < min) || (val > max))
                {
                    val = 0;
                }
                if (lp->getState() != LocalParams::STATE_NO_ERROR)
                {
                    val = 0;
                    max = 0;
                }
            }
            else if (lp->getState() != LocalParams::STATE_NO_ERROR)
            {
                val = min;
            }

            enterFunc = &clMenu::enterValue;
            EnterParam.setParam(lp->getParam());
            EnterParam.setValueRange(min, max);
            EnterParam.setValue(val);
            //            EnterParam.list = getListOfValues(param);
            EnterParam.setFract(getFract(param));
            EnterParam.setDisc(getDisc(param));
        }
    }
}

// ������ � ���� ��������� ����������.
void clMenu::setupParam()
{

    if (isMessage())
    {
        // ����� �� ����� ��������� � ������������� ��������� ���������,
        // ��� ���� ������� ������ ����� ���������������.
        static const char message[3][21] PROGMEM = { // 2345678901234567890
                                                     " �������� ��������  ",
                                                     "  ����� ������ �    ",
                                                     "  ������ �������    "
        };

        for (uint8_t i = 0, pos = 40; i < SIZE_OF(message); i++, pos += 20)
        {
            snprintf_P(&vLCDbuf[pos], 21, message[i]);
        }

        key_ = KEY_NO;
    }
    else
    {
        // ����� �� ����� ���������� � ������� ���������
        printParam();
    }

    if (EnterParam.isEnable())
    {
        eMENU_ENTER_PARAM stat = enterValue();

        if (stat == MENU_ENTER_PARAM_READY)
        {
            eGB_PARAM param = sParam.local.getParam();
            eGB_COM   com   = getCom(param);

            // ���� � ��������� ���� ������� ������ � ������ ���, ����
            // ������ �� ������ � ���.
            // ����� ���� ������ � ������.
            if (com)
            {
                // ���������� ������ ��� ������ � ���.
                uint8_t dop = getSendDop(param);
                uint8_t pos = sParam.local.getNumOfCurrSameParam() - 1;

                switch (getSendType(param))
                {
                case GB_SEND_INT8:
                    sParam.txComBuf.setInt8(static_cast<uint8_t>(EnterParam.getValueEnter()));
                    break;

                case GB_SEND_INT8_DOP:  // DOWN
                case GB_SEND_DOP_INT8:
                    sParam.txComBuf.setInt8(static_cast<uint8_t>(EnterParam.getValueEnter()), 0);
                    sParam.txComBuf.setInt8(pos + dop, 1);
                    break;

                case GB_SEND_INT16_BE:
                    sParam.txComBuf.setInt8(static_cast<uint8_t>(EnterParam.getValue() >> 8), 0);
                    sParam.txComBuf.setInt8(static_cast<uint8_t>(EnterParam.getValue()), 1);
                    break;

                case GB_SEND_BITES_DOP:  // DOWN
                case GB_SEND_DOP_BITES:
                    {
                        uint8_t val = sParam.local.getValueB();
                        if (EnterParam.getValue())
                        {
                            val |= (1 << (pos % 8));
                        }
                        else
                        {
                            val &= ~(1 << (pos % 8));
                        }
                        sParam.txComBuf.setInt8(val, 0);
                        sParam.txComBuf.setInt8(pos / 8 + dop, 1);
                    }
                    break;

                case GB_SEND_COR_U:
                    {
                        // ���� ������� �������� ��������� ���� ����� 0
                        // �� ���������� ��������� � ���.������ ������ 4
                        // ���������� ����� ���������
                        int16_t t = EnterParam.getValue();
                        if (t == 0)
                            dop = 4;
                        else
                        {
                            // ����� ��������� =
                            // ���������� ������� - (���������� � ��� - ���������)
                            t -= static_cast<int16_t>(sParam.measParam.getVoltageOut());
                            t += sParam.local.getValue();
                        }
                        sParam.txComBuf.setInt8(dop, 0);
                        sParam.txComBuf.setInt8(static_cast<uint8_t>(t / 10), 1);
                        sParam.txComBuf.setInt8(static_cast<uint8_t>(t % 10) * 10, 2);
                    }
                    break;

                case GB_SEND_COR_I:
                    {
                        // ���� ������� �������� ��������� ���� ����� 0
                        // �� ���������� ��������� � ���.������ ������ 5
                        // ���������� ����� ���������
                        int16_t t = static_cast<int16_t>(EnterParam.getValue());
                        if (t == 0)
                            dop = 5;
                        else
                        {
                            // ����� ��������� = ��� ������� - (��� � ��� - ���������)
                            t -= static_cast<int16_t>(sParam.measParam.getCurrentOut());
                            t += sParam.local.getValue();
                        }
                        sParam.txComBuf.setInt8(dop, 0);
                        sParam.txComBuf.setInt8(static_cast<uint8_t>(t >> 8), 1);
                        sParam.txComBuf.setInt8(static_cast<uint8_t>(t), 2);
                    }
                    break;

                case GB_SEND_NO: com = GB_COM_NO; break;
                }

                if (com != GB_COM_NO)
                {
                    com = static_cast<eGB_COM>(static_cast<uint8_t>(com)
                                               + GB_COM_MASK_GROUP_WRITE_PARAM);
                    sParam.txComBuf.addFastCom(com);
                    sParam.txComBuf.setSendType(getSendType(param));
                }
            }
            else
            {
                eGB_PARAM param = sParam.local.getParam();
                if (param != GB_PARAM_NULL_PARAM)
                {
                    // ������ ���������� � ������.
                    uint8_t tmp = static_cast<uint8_t>(EnterParam.getValueEnter());
                    if (param == GB_PARAM_INTF_INTERFACE)
                    {
                        TInterface::INTERFACE val;
                        val = static_cast<TInterface::INTERFACE>(tmp);
                        // ���� ��������� ��������, ������� ����
                        if (val != sParam.Uart.Interface.get())
                        {
                            sParam.Uart.Interface.set(val);
                            lvlCreate_ = true;
                        }
                    }
                    else if (param == GB_PARAM_INTF_PROTOCOL)
                    {
                        sParam.Uart.Protocol.set(static_cast<TProtocol::PROTOCOL>(tmp));
                    }
                    else if (param == GB_PARAM_INTF_BAUDRATE)
                    {
                        sParam.Uart.BaudRate.set(static_cast<TBaudRate::BAUD_RATE>(tmp));
                    }
                    else if (param == GB_PARAM_INTF_DATA_BITS)
                    {
                        sParam.Uart.DataBits.set(static_cast<TDataBits::DATA_BITS>(tmp));
                    }
                    else if (param == GB_PARAM_INTF_PARITY)
                    {
                        sParam.Uart.Parity.set(static_cast<TParity::PARITY>(tmp));
                    }
                    else if (param == GB_PARAM_INTF_STOP_BITS)
                    {
                        sParam.Uart.StopBits.set(static_cast<TStopBits::STOP_BITS>(tmp));
                    }
                }
            }
            EnterParam.setDisable();
        }
    }

    switch (key_)
    {
    case KEY_UP: sParam.local.prevParam(); break;
    case KEY_DOWN: sParam.local.nextParam(); break;

    case KEY_LEFT: sParam.local.prevSameParam(); break;
    case KEY_RIGHT: sParam.local.nextSameParam(); break;

    case KEY_ENTER: enterParameter(); break;

    default: break;
    }

    // ������� �������, �� ������� �������� ������ ���� + ������� �������
    if (sParam.local.getState() == LocalParams::STATE_READ_PARAM)
    {

        eGB_COM com = getCom(sParam.local.getParam());
        sParam.txComBuf.addFastCom(com);
        sParam.txComBuf.addCom1(com, 1);
    }

    // ����� �� �������� ������ ����, ���� ���-�� ���������� ����� 0
    if (sParam.local.getNumOfParams() == 0)
    {
        key_ = KEY_CANCEL;
    }
}

// �������� ������������� � ���������.
bool clMenu::checkLedOn()
{
    bool ledOn = false;

    if (sParam.glb.status.getRegime() != GB_REGIME_ENABLED)
    {
        ledOn = true;
    }

    if (sParam.glb.status.isFault() || sParam.glb.status.isWarning())
    {
        ledOn = true;
    }

    if (sParam.def.status.isEnable())
    {
        if (sParam.def.status.getState() != 1)
        {
            ledOn = true;
        }
        else if (sParam.def.status.isFault())
        {
            ledOn = true;
        }
        else if (sParam.def.status.isWarning())
        {
            ledOn = true;
        }
    }

    if (sParam.prm.status.isEnable())
    {
        if (sParam.prm.status.getState() != 1)
        {
            ledOn = true;
        }
        else if (sParam.prm.status.isFault())
        {
            ledOn = true;
        }
        else if (sParam.prm.status.isWarning())
        {
            ledOn = true;
        }
        else if (sParam.prm.isIndCom())
        {
            ledOn = true;
        }
    }

    if (sParam.prd.status.isEnable())
    {
        if (sParam.prd.status.getState() != 1)
        {
            ledOn = true;
        }
        else if (sParam.prd.status.isFault())
        {
            ledOn = true;
        }
        else if (sParam.prd.status.isWarning())
        {
            ledOn = true;
        }
        else if (sParam.prd.isIndCom())
        {
            ledOn = true;
        }
    }

    if (sParam.glb.isLedOn())
    {
        ledOn = true;
        sParam.glb.setLedOn(false);
    }

    return ledOn;
}

// �������� �������� �������� ���� � ������������� �����.
bool clMenu::isRzskM() const
{
    bool check = false;

    if (sParam.typeDevice == AVANT_RZSK)
    {
        if (sParam.glb.getCompRZSK() == GB_COMP_RZSK_M)
        {
            check = true;
        }
    }

    return check;
}

// ���������� �������������� ������� ������ ����������.
TControl::ctrl_t clMenu::onFnButton(eKEY& key)
{
    eKEY tkey             = key;
    key                   = KEY_NO;
    TControl::ctrl_t ctrl = TControl::CTRL_NO;

    switch (tkey)
    {
    case KEY_CALL: ctrl = TControl::CTRL_Call; break;

    case KEY_PUSK_UD:
        if (sParam.def.status.isEnable())
        {
            if (sParam.glb.getNumDevices() == GB_NUM_DEVICES_3)
            {
                ctrl = TControl::CTRL_RemotePuskAll;
            }
            else
            {
                ctrl = TControl::CTRL_RemotePusk;
            }
        }
        break;

    case KEY_AC_PUSK_UD:
        if (sParam.def.status.isEnable())
        {
            eGB_COMP_R400M comp = sParam.glb.getCompR400m();
            if (comp != GB_COMP_R400M_R400)
            {
                ctrl = TControl::CTRL_RemoteAcPusk;
            }
        }
        break;

    case KEY_PUSK_NALAD:
        if (sParam.def.status.isEnable())
        {
            if (sParam.def.status.getState() == 7)
            {
                ctrl = TControl::CTRL_PuskAdjOff;
            }
            else
            {
                ctrl = TControl::CTRL_PuskAdjOn;
            }
        }
        break;

    case KEY_AC_RESET:
        if (sParam.def.status.isEnable())
        {
            ctrl = TControl::CTRL_AcReset;
        }
        break;

    case KEY_AC_PUSK:
        if (sParam.def.status.isEnable())
        {
            if (sParam.typeDevice == AVANT_R400M)
            {
                eGB_COMP_R400M comp = sParam.glb.getCompR400m();
                if ((comp != GB_COMP_R400M_LINER) && (comp != GB_COMP_R400M_R400))
                {
                    if ((comp == GB_COMP_R400M_AVZK80) || (comp == GB_COMP_R400M_PVZ90))
                    {
                        ctrl = TControl::CTRL_AcPusk;
                    }
                    else
                    {
                        ctrl = TControl::CTRL_AcPuskSelf;
                    }
                }
            }
        }
        break;

    case KEY_AC_REGIME:
        if (sParam.def.status.isEnable())
        {
            ctrl = TControl::CTRL_AcRegime;
        }
        break;

    case KEY_RESET_IND:
        if (sParam.prd.status.isEnable() || sParam.prm.status.isEnable())
        {
            ctrl = TControl::CTRL_IndReset;
        }
        break;

    case KEY_PUSK:
        if (sParam.prm.status.isEnable())
        {
            ctrl = TControl::CTRL_PuskPrm;
        }
        break;

    case KEY_RESET: ctrl = TControl::CTRL_Reset; break;

    default: key = tkey;
    }

    return ctrl;
}

//
eGB_TYPE_DEVICE clMenu::getKeyboardLayout()
{
    eGB_TYPE_DEVICE layout = AVANT_K400;

    switch (sParam.typeDevice)
    {
    case AVANT_K400: layout = AVANT_K400; break;

    case AVANT_R400:
    case AVANT_R400M: layout = AVANT_R400M; break;

    case AVANT_RZSK:
        {
            eGB_COMP_RZSK comp = sParam.glb.getCompRZSK();

            layout = (comp == GB_COMP_RZSK_3E8) ? AVANT_K400 : AVANT_RZSK;
            break;
        }

    case AVANT_OPTO:
        {
            if (sParam.def.status.isEnable())
            {
                bool hasprm = sParam.prm.status.isEnable();
                bool hasprd = sParam.prd.status.isEnable();

                layout = hasprd || hasprm ? AVANT_RZSK : AVANT_R400M;
            }
            else
            {
                layout = AVANT_K400;
            }
            break;
        }


    case AVANT_NO: break;
    case AVANT_MAX: break;
    }

    return layout;
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ������� ��� �������� ������� ����������.
 * @param[in] ctrl ������ ����������.
 *
 * *****************************************************************************
 */
void clMenu::AddControlToSend(TControl::ctrl_t ctrl)
{
    eGB_COM com     = GB_COM_NO;
    bool    hasbyte = false;
    uint8_t byte    = 0;

    if (mControl.getData(ctrl, com, hasbyte, byte))
    {
        SET_DEBUG_BYTE(1, com);
        SET_DEBUG_BYTE(2, byte);

        if (com != GB_COM_NO)
        {
            if (hasbyte)
            {
                sParam.txComBuf.setInt8(byte);
            }
            sParam.txComBuf.addFastCom(com);
        }
    }
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ������ ���������� � ������ ������� ����.
 * @param[in] ctrl ������ ����������.
 *
 * *****************************************************************************
 */
void clMenu::AddControlToPunkts(TControl::ctrl_t ctrl)
{
    Q_ASSERT(ctrl < TControl::CTRL_MAX);

    if (ctrl < TControl::CTRL_MAX)
    {
        Punkts_.Add(mControl.getText(ctrl), static_cast<uint8_t>(ctrl));
    }
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ������ �������� ���������� �� ������ "����������".
 * @param[in] device ����������.
 * @return True ���� ������ ��������, ����� False.
 *
 * *****************************************************************************
 */
bool clMenu::fillLvlControl(eGB_TYPE_DEVICE device)
{
    bool is_fill = false;

    Q_ASSERT(device > AVANT_NO);
    Q_ASSERT(device < AVANT_MAX);

    Punkts_.Clear();

    switch (device)
    {
    case AVANT_R400M:
        {
            is_fill = fillLvlControlR400m(sParam.glb.getCompR400m(), sParam.def.getNumDevices());
            break;
        }

    case AVANT_RZSK:
        {
            is_fill = fillLvlControlRzsk(sParam.def.getNumDevices(),
                                         sParam.def.status.isEnable(),
                                         isRzskM());
            break;
        }

    case AVANT_K400:
        {
            is_fill = fillLvlControlK400();
            break;
        }
    case AVANT_OPTO:
        {
            is_fill = fillLvlControlOpto(sParam.glb.getTypeOpto(),
                                         sParam.def.status.isEnable(),
                                         sParam.prd.status.isEnable(),
                                         sParam.prm.status.isEnable());
            break;
        }


    case AVANT_NO: [[gnu::fallthrough]];
    case AVANT_R400: [[gnu::fallthrough]];
    case AVANT_MAX: break;
    }

    return is_fill;
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ������ �������� ���������� �� ������ "����������" ��� �400�.
 *
 * @param[in] comp �������������.
 * @param[in] num_devices ���������� ��������� � �����.
 * @return True ���� ������ ��������, ����� False.
 *
 * *****************************************************************************
 */
bool clMenu::fillLvlControlR400m(eGB_COMP_R400M comp, eGB_NUM_DEVICES num_devices)
{
    Q_ASSERT(sParam.glb.getTypeDevice() == AVANT_R400M);

    if (comp != GB_COMP_R400M_PVZK)
    {
        // ������������� ���� �� �����������, ���� �� �����
        AddControlToPunkts(TControl::CTRL_PuskAdjOff);
    }
    AddControlToPunkts(TControl::CTRL_Reset);

    switch (comp)
    {
    case GB_COMP_R400M_AVANT:
        {
            if (num_devices == GB_NUM_DEVICES_3)
            {
                AddControlToPunkts(TControl::CTRL_RemoteResetAll);
                AddControlToPunkts(TControl::CTRL_RemotePusk1);
                AddControlToPunkts(TControl::CTRL_RemotePusk2);
                AddControlToPunkts(TControl::CTRL_RemotePuskAll);
            }
            else
            {
                AddControlToPunkts(TControl::CTRL_RemoteReset);
                AddControlToPunkts(TControl::CTRL_RemotePusk);
            }
            AddControlToPunkts(TControl::CTRL_AcReset);
            AddControlToPunkts(TControl::CTRL_AcPuskSelf);
            AddControlToPunkts(TControl::CTRL_RemoteAcPusk);
            AddControlToPunkts(TControl::CTRL_AcNormal);
            AddControlToPunkts(TControl::CTRL_AcAccelerated);
            AddControlToPunkts(TControl::CTRL_AcOff);
            AddControlToPunkts(TControl::CTRL_Call);
            break;
        }

    case GB_COMP_R400M_PVZ90:
        {
            AddControlToPunkts(TControl::CTRL_RemoteReset);
            AddControlToPunkts(TControl::CTRL_AcNormal);
            AddControlToPunkts(TControl::CTRL_AcAccelerated);
            AddControlToPunkts(TControl::CTRL_AcOff);
            AddControlToPunkts(TControl::CTRL_AcTest);
            AddControlToPunkts(TControl::CTRL_AcPusk);
            break;
        }

    case GB_COMP_R400M_PVZUE:
        {
            if (num_devices == GB_NUM_DEVICES_3)
            {
                AddControlToPunkts(TControl::CTRL_RemotePusk1);
                AddControlToPunkts(TControl::CTRL_RemotePusk2);
                AddControlToPunkts(TControl::CTRL_RemoteMan1);
                AddControlToPunkts(TControl::CTRL_RemoteMan2);
            }
            else
            {
                AddControlToPunkts(TControl::CTRL_RemotePusk);
                AddControlToPunkts(TControl::CTRL_RemoteMan);
            }
            AddControlToPunkts(TControl::CTRL_RemoteManAll);
            AddControlToPunkts(TControl::CTRL_AcNormal);
            AddControlToPunkts(TControl::CTRL_AcAccelerated);
            AddControlToPunkts(TControl::CTRL_AcQuick);
            AddControlToPunkts(TControl::CTRL_AcCtrlCheck);
            AddControlToPunkts(TControl::CTRL_AcOff);
            AddControlToPunkts(TControl::CTRL_Call);
            break;
        }

    case GB_COMP_R400M_AVZK80:
        {
            AddControlToPunkts(TControl::CTRL_AcNormal);
            AddControlToPunkts(TControl::CTRL_AcAccelerated);
            AddControlToPunkts(TControl::CTRL_AcOff);
            AddControlToPunkts(TControl::CTRL_AcTest);
            AddControlToPunkts(TControl::CTRL_AcPusk);
            break;
        }

    case GB_COMP_R400M_PVZL:
        {
            AddControlToPunkts(TControl::CTRL_AcReset);
            AddControlToPunkts(TControl::CTRL_AcPuskSelf);
            AddControlToPunkts(TControl::CTRL_RemoteAcPusk);
            AddControlToPunkts(TControl::CTRL_PuskPrd);
            AddControlToPunkts(TControl::CTRL_AcOn);
            AddControlToPunkts(TControl::CTRL_AcOff);
            AddControlToPunkts(TControl::CTRL_Call);
            break;
        }

    case GB_COMP_R400M_LINER:
        {
            if (num_devices == GB_NUM_DEVICES_3)
            {
                AddControlToPunkts(TControl::CTRL_RemoteReset1);
                AddControlToPunkts(TControl::CTRL_RemoteReset2);
                AddControlToPunkts(TControl::CTRL_RemotePusk1);
                AddControlToPunkts(TControl::CTRL_RemotePusk2);
                AddControlToPunkts(TControl::CTRL_RemotePuskAll);
            }
            else
            {
                AddControlToPunkts(TControl::CTRL_RemoteReset);
                AddControlToPunkts(TControl::CTRL_RemotePusk);
            }
            AddControlToPunkts(TControl::CTRL_AcAuto);
            AddControlToPunkts(TControl::CTRL_AcAccelerated);
            AddControlToPunkts(TControl::CTRL_AcOff);
            AddControlToPunkts(TControl::CTRL_Call);
            break;
        }

    case GB_COMP_R400M_PVZK:
        {
            // TODO �������� ������� ���������� ��� ������������� ����.
            break;
        }

    case GB_COMP_R400M_PVZU:
        {
            AddControlToPunkts(TControl::CTRL_AcNormal);
            AddControlToPunkts(TControl::CTRL_AcAccelerated);
            AddControlToPunkts(TControl::CTRL_AcQuick);
            AddControlToPunkts(TControl::CTRL_AcOff);
            AddControlToPunkts(TControl::CTRL_AcCtrlCheck);
            AddControlToPunkts(TControl::CTRL_Call);
            break;
        }

    case GB_COMP_R400M_PVZ:
        {
            AddControlToPunkts(TControl::CTRL_AcNormal);
            AddControlToPunkts(TControl::CTRL_AcAccelerated);
            AddControlToPunkts(TControl::CTRL_AcOff);
            AddControlToPunkts(TControl::CTRL_AcRequest);
            AddControlToPunkts(TControl::CTRL_AcReset);
            break;
        }

    case GB_COMP_R400M_R400:
        {
            if (num_devices == GB_NUM_DEVICES_3)
            {
                AddControlToPunkts(TControl::CTRL_RemoteReset1);
                AddControlToPunkts(TControl::CTRL_RemoteReset2);
                AddControlToPunkts(TControl::CTRL_RemotePusk1);
                AddControlToPunkts(TControl::CTRL_RemotePusk2);
                AddControlToPunkts(TControl::CTRL_RemotePuskAll);
            }
            else
            {
                AddControlToPunkts(TControl::CTRL_RemoteReset);
                AddControlToPunkts(TControl::CTRL_RemotePusk);
            }
            AddControlToPunkts(TControl::CTRL_AcAuto);
            AddControlToPunkts(TControl::CTRL_AcAccelerated);
            AddControlToPunkts(TControl::CTRL_AcOff);
            AddControlToPunkts(TControl::CTRL_Call);
            break;
        }

    case GB_COMP_R400M_MAX: break;
    }

    return true;
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ������ �������� ���������� �� ������ "����������" ��� ����.
 * @param[in] num_devices ���������� ��������� � �����.
 * @param[in] def ������� ������.
 * @param[in] comp_rzskm True ���� ������������� �����.
 * @return True ���� ������ ��������, ����� False.
 *
 * *****************************************************************************
 */
bool clMenu::fillLvlControlRzsk(eGB_NUM_DEVICES num_devices, bool def, bool comp_rzskm)
{
    Q_ASSERT(sParam.glb.getTypeDevice() == AVANT_RZSK);

    if (def)
    {
        AddControlToPunkts(TControl::CTRL_PuskAdjOff);
    }

    AddControlToPunkts(TControl::CTRL_Reset);

    if (num_devices == GB_NUM_DEVICES_3)
    {
        AddControlToPunkts(TControl::CTRL_RemoteResetAll);

        if (def)
        {
            AddControlToPunkts(TControl::CTRL_RemotePusk1);
            AddControlToPunkts(TControl::CTRL_RemotePusk2);
            AddControlToPunkts(TControl::CTRL_RemotePuskAll);
        }
    }
    else
    {
        AddControlToPunkts(TControl::CTRL_RemoteReset);

        if (def)
        {
            AddControlToPunkts(TControl::CTRL_RemotePusk);
        }
    }

    AddControlToPunkts(TControl::CTRL_IndReset);

    if (def)
    {
        AddControlToPunkts(TControl::CTRL_Call);

        if (comp_rzskm)
        {
            AddControlToPunkts(TControl::CTRL_SingleOn);
        }
    }

    return true;
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ������ �������� ���������� �� ������ "����������" ��� �400.
 * @return True ���� ������ ��������, ����� False.
 *
 * *****************************************************************************
 */
bool clMenu::fillLvlControlK400()
{
    Q_ASSERT(sParam.glb.getTypeDevice() == AVANT_K400);

    AddControlToPunkts(TControl::CTRL_Reset);
    AddControlToPunkts(TControl::CTRL_IndReset);

    return true;
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ������ �������� ���������� �� ������ "����������" ��� ������.
 * @param[in] type ��� ������������ ������.
 * @param[in] def ������� ������.
 * @param[in] prd ������� �����������.
 * @param[in] prm ������� ���������.
 * @return True ���� ������ ��������, ����� False.
 *
 * *****************************************************************************
 */
bool clMenu::fillLvlControlOpto(eGB_TYPE_OPTO type, bool def, bool prd, bool prm)
{
    Q_ASSERT(sParam.glb.getTypeDevice() == AVANT_OPTO);
    Q_ASSERT(sParam.glb.getTypeLine() == GB_TYPE_LINE_OPTO);

    if (def)
    {
        AddControlToPunkts(TControl::CTRL_PuskAdjOff);
    }

    AddControlToPunkts(TControl::CTRL_Reset);

    if (type == TYPE_OPTO_STANDART)
    {
        AddControlToPunkts(TControl::CTRL_RemoteReset);
    }
    else
    {
        AddControlToPunkts(TControl::CTRL_ResetAll);
    }

    if (prd || prm)
    {
        AddControlToPunkts(TControl::CTRL_IndReset);
    }

    if (def)
    {
        AddControlToPunkts(TControl::CTRL_RemotePusk);

        if (!prd && !prm)
        {
            AddControlToPunkts(TControl::CTRL_AcReset);
        }
    }

    return true;
}


/**
 * *****************************************************************************
 *
 * @brief �������� ������ ���������� ��� ���������� ������ ����.
 * @param[in] index ������ ������ ����.
 * @param[in] ctrl ������ ����������.
 *
 * *****************************************************************************
 */
void clMenu::ChangeCotnrolPunkt(uint8_t index, TControl::ctrl_t ctrl)
{
    Q_ASSERT(ctrl < TControl::CTRL_MAX);

    if (ctrl < TControl::CTRL_MAX)
    {
        Punkts_.Change(index, mControl.getText(ctrl), static_cast<uint8_t>(ctrl));
    }
}


/**
 * *****************************************************************************
 *
 * @brief �������� ������ ���� � ����������� �� �������� ��������� ��������
 *
 * ������ ������������ ������ ��� ������. �������� ����� ������ � ����-��������
 * ��������.
 *
 * @fixme ��������� ���������� �������� ��� ������ �������� ��������� �� ������ ��������.
 *
 * *****************************************************************************
 */
void clMenu::ChangeControlPunkts()
{
    if (!sParam.def.status.isEnable())
        return;

    uint8_t dnumber = sParam.glb.getDeviceNum();
    for (uint8_t i = 0; i < Punkts_.GetLen(); i++)
    {
        TControl::ctrl_t ctrl = static_cast<TControl::ctrl_t>(Punkts_.GetData(i));

        if ((ctrl == TControl::CTRL_PuskAdjOn) || (ctrl == TControl::CTRL_PuskAdjOff))
        {
            ctrl = (sParam.def.status.getState() == 7) ? (TControl::CTRL_PuskAdjOff)
                                                       : (TControl::CTRL_PuskAdjOn);
            ChangeCotnrolPunkt(i, ctrl);
        }
        else if ((ctrl == TControl::CTRL_RemoteMan1) || (ctrl == TControl::CTRL_RemoteMan2))
        {

            ctrl = (dnumber == 1) ? (TControl::CTRL_RemoteMan2) : (TControl::CTRL_RemoteMan1);
            ChangeCotnrolPunkt(i, ctrl);
            ctrl = (dnumber > 2) ? (TControl::CTRL_RemoteMan2) : (TControl::CTRL_RemoteMan3);
            ChangeCotnrolPunkt(++i, ctrl);
        }
        else if ((ctrl == TControl::CTRL_RemotePusk1) || (ctrl == TControl::CTRL_RemotePusk2))
        {
            ctrl = (dnumber == 1) ? (TControl::CTRL_RemotePusk2) : (TControl::CTRL_RemotePusk1);
            ChangeCotnrolPunkt(i, ctrl);
            ctrl = (dnumber > 2) ? (TControl::CTRL_RemotePusk2) : (TControl::CTRL_RemotePusk3);
            ChangeCotnrolPunkt(++i, ctrl);
        }
        else if ((ctrl == TControl::CTRL_RemoteReset1) || (ctrl == TControl::CTRL_RemoteReset2))
        {

            ctrl = (dnumber == 1) ? (TControl::CTRL_RemoteReset2) : (TControl::CTRL_RemoteReset1);
            ChangeCotnrolPunkt(i, ctrl);
            ctrl = (dnumber > 2) ? (TControl::CTRL_RemoteReset2) : (TControl::CTRL_RemoteReset3);
            ChangeCotnrolPunkt(++i, ctrl);
        }
        else if ((ctrl == TControl::CTRL_SingleOff) || (ctrl == TControl::CTRL_SingleOn))
        {
            ctrl = (sParam.local.getValue() != 1) ? (TControl::CTRL_SingleOn)
                                                  : (TControl::CTRL_SingleOff);
            ChangeCotnrolPunkt(i, ctrl);
        }
    }
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ������ ���������� �� ������ "���������\������".
 * @param[in] device ����������.
 * @return True ���� ������ ��������, ����� False.
 *
 * *****************************************************************************
 */
bool clMenu::fillLvlSetupParamDef(eGB_TYPE_DEVICE device)
{
    bool is_fill = false;

    Q_ASSERT(device > AVANT_NO);
    Q_ASSERT(device < AVANT_MAX);

    sParam.local.clearParams();

    switch (device)
    {
    case AVANT_R400M:
        {
            is_fill = fillLvlSetupParamDefR400m(sParam.glb.getCompR400m());
            break;
        }

    case AVANT_RZSK:
        {
            is_fill = fillLvlSetupParamDefRzsk();
            break;
        }

    case AVANT_OPTO:
        {
            is_fill = fillLvlSetupParamDefOpto();
            break;
        }

    case AVANT_K400: [[gnu::fallthrough]];
    case AVANT_NO: [[gnu::fallthrough]];
    case AVANT_R400: [[gnu::fallthrough]];
    case AVANT_MAX: break;
    }

    return is_fill;
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ������ ���������� �� ������ "���������\������" ��� �400�.
 * @param[in] comp �������������.
 * @return True ���� ������ ��������, ����� False.
 *
 * *****************************************************************************
 */
bool clMenu::fillLvlSetupParamDefR400m(eGB_COMP_R400M comp)
{
    sParam.local.addParam(GB_PARAM_NUM_OF_DEVICES);
    sParam.local.addParam(GB_PARAM_DEF_TYPE);
    sParam.local.addParam(GB_PARAM_TIME_NO_MAN);
    sParam.local.addParam(GB_PARAM_SHIFT_FRONT);
    sParam.local.addParam(GB_PARAM_SHIFT_BACK);
    sParam.local.addParam(GB_PARAM_SHIFT_PRM);
    sParam.local.addParam(GB_PARAM_SHIFT_PRD);
    sParam.local.addParam(GB_PARAM_SENS_DEC);
    if ((comp == GB_COMP_R400M_AVANT) || (comp == GB_COMP_R400M_R400))
    {
        sParam.local.addParam(GB_PARAM_AC_IN_DEC);
    }
    sParam.local.addParam(GB_PARAM_FREQ_PRD);
    sParam.local.addParam(GB_PARAM_FREQ_PRM);

    return true;
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ������ ���������� �� ������ "���������\������" ��� ����.
 * @return True ���� ������ ��������, ����� False.
 *
 * *****************************************************************************
 */
bool clMenu::fillLvlSetupParamDefRzsk()
{
    sParam.local.addParam(GB_PARAM_DEF_TYPE);
    sParam.local.addParam(GB_PARAM_TIME_NO_MAN);
    sParam.local.addParam(GB_PARAM_OVERLAP);
    sParam.local.addParam(GB_PARAM_DELAY);
    sParam.local.addParam(GB_PARAM_WARN_THD_RZ);
    sParam.local.addParam(GB_PARAM_SENS_DEC_RZ);
    sParam.local.addParam(GB_PARAM_PRM_TYPE);

    return true;
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ������ ���������� �� ������ "���������\������" ��� ������.
 * @return True ���� ������ ��������, ����� False.
 *
 * *****************************************************************************
 */
bool clMenu::fillLvlSetupParamDefOpto()
{
    sParam.local.addParam(GB_PARAM_NUM_OF_DEVICES);
    sParam.local.addParam(GB_PARAM_DEF_TYPE);
    sParam.local.addParam(GB_PARAM_TIME_NO_MAN);
    sParam.local.addParam(GB_PARAM_OVERLAP_OPTO);
    sParam.local.addParam(GB_PARAM_DELAY_OPTO);

    return true;
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ������ ���������� �� ������ "���������\�����".
 * @param[in] device ����������.
 * @return True ���� ������ ��������, ����� False.
 *
 * *****************************************************************************
 */
bool clMenu::fillLvlSetupParamGlb(eGB_TYPE_DEVICE device)
{
    bool is_fill = false;

    Q_ASSERT(device > AVANT_NO);
    Q_ASSERT(device < AVANT_MAX);

    sParam.local.clearParams();

    switch (device)
    {
    case AVANT_R400M:
        {
            is_fill = fillLvlSetupParamGlbR400m(sParam.glb.getCompR400m());
            break;
        }

    case AVANT_RZSK:
        {
            is_fill = fillLvlSetupParamGlbRzsk(sParam.glb.getCompRZSK());
            break;
        }

    case AVANT_K400:
        {
            is_fill = fillLvlSetupParamGlbK400(sParam.glb.getCompK400(),
                                               sParam.prd.status.isEnable(),
                                               sParam.prm.status.isEnable());
            break;
        }
    case AVANT_OPTO:
        {
            is_fill = fillLvlSetupParamGlbOpto(sParam.glb.getTypeOpto(),
                                               sParam.prd.status.isEnable(),
                                               sParam.prm.status.isEnable());
            break;
        }


    case AVANT_NO: [[gnu::fallthrough]];
    case AVANT_R400: [[gnu::fallthrough]];
    case AVANT_MAX: break;
    }

    return is_fill;
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ������ ���������� �� ������ "���������\�����" ��� �400�.
 * @param[in] comp �������������.
 * @return True ���� ������ ��������, ����� False.
 *
 * *****************************************************************************
 */
bool clMenu::fillLvlSetupParamGlbR400m(eGB_COMP_R400M comp)
{
    Q_ASSERT(sParam.glb.getTypeDevice() == AVANT_R400M);
    Q_ASSERT(sParam.glb.getTypeLine() == GB_TYPE_LINE_UM);

    sParam.local.addParam(GB_PARAM_COMP_P400);

    if ((comp == GB_COMP_R400M_AVANT) || (comp == GB_COMP_R400M_R400))
    {
        sParam.local.addParam(GB_PARAM_TIME_SYNCH_SRC);
    }

    sParam.local.addParam(GB_PARAM_NUM_OF_DEVICE);
    sParam.local.addParam(GB_PARAM_OUT_CHECK);
    sParam.local.addParam(GB_PARAM_WARN_THD);
    sParam.local.addParam(GB_PARAM_U_OUT_NOM);
    sParam.local.addParam(GB_PARAM_FREQ);

    if (comp == GB_COMP_R400M_PVZL)
    {
        sParam.local.addParam(GB_PARAM_IN_DEC_AC_ANSWER);
    }
    else if (comp == GB_COMP_R400M_PVZUE)
    {
        sParam.local.addParam(GB_PARAM_PVZUE_PROTOCOL);
        sParam.local.addParam(GB_PARAM_PVZUE_PARITY);
        sParam.local.addParam(GB_PARAM_PVZUE_FAIL);
        sParam.local.addParam(GB_PARAM_PVZUE_NOISE_THD);
        sParam.local.addParam(GB_PARAM_PVZUE_NOISE_LVL);
        sParam.local.addParam(GB_PARAM_PVZUE_AC_TYPE);
        sParam.local.addParam(GB_PARAM_PVZUE_AC_PERIOD);
        sParam.local.addParam(GB_PARAM_PVZUE_AC_PER_RE);
    }
    else if (comp == GB_COMP_R400M_PVZU)
    {
        sParam.local.addParam(GB_PARAM_PVZUE_FAIL);
        sParam.local.addParam(GB_PARAM_PVZU_AC_CORRECT);
    }
    else if (comp == GB_COMP_R400M_R400)
    {
        sParam.local.addParam(GB_PARAM_ALARM_CF);
    }

    sParam.local.addParam(GB_PARAM_COR_U);
    sParam.local.addParam(GB_PARAM_COR_I);

    return true;
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ������ ���������� �� ������ "���������\�����" ��� ����.
 * @param[in] comp_rzskm True ���� ������������� �����.
 * @return True ���� ������ ��������, ����� False.
 *
 * *****************************************************************************
 */
bool clMenu::fillLvlSetupParamGlbRzsk(eGB_COMP_RZSK comp)
{
    Q_ASSERT(sParam.glb.getTypeDevice() == AVANT_RZSK);
    Q_ASSERT(sParam.glb.getTypeLine() == GB_TYPE_LINE_UM);

    sParam.local.addParam(GB_PARAM_COMP_RZSK);
    sParam.local.addParam(GB_PARAM_NUM_OF_DEVICES, comp == GB_COMP_RZSK_3E8);
    sParam.local.addParam(GB_PARAM_TIME_SYNCH);
    sParam.local.addParam(GB_PARAM_NUM_OF_DEVICE);
    sParam.local.addParam(GB_PARAM_OUT_CHECK);
    sParam.local.addParam(GB_PARAM_WARN_THD_CF);
    sParam.local.addParam(GB_PARAM_TIME_RERUN);
    sParam.local.addParam(GB_PARAM_COM_PRD_KEEP);
    sParam.local.addParam(GB_PARAM_COM_PRM_KEEP);
    sParam.local.addParam(GB_PARAM_IN_DEC);
    sParam.local.addParam(GB_PARAM_FREQ);
    sParam.local.addParam(GB_PARAM_DETECTOR);
    sParam.local.addParam(GB_PARAM_COR_U);
    sParam.local.addParam(GB_PARAM_COR_I);

    return true;
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ������ ���������� �� ������ "���������\�����" ��� �400.
 * @param[in] comp �������������.
 * @param[in] prd ������� �����������.
 * @param[in] prm ������� ���������.
 * @return True ���� ������ ��������, ����� False.
 *
 * *****************************************************************************
 */
bool clMenu::fillLvlSetupParamGlbK400(eGB_COMP_K400 comp, bool prd, bool prm)
{
    Q_ASSERT(sParam.glb.getTypeDevice() == AVANT_K400);
    Q_ASSERT(sParam.glb.getTypeLine() == GB_TYPE_LINE_UM);

    sParam.local.addParam(GB_PARAM_COMP_K400);
    sParam.local.addParam(GB_PARAM_TIME_SYNCH);
    sParam.local.addParam(GB_PARAM_NUM_OF_DEVICE);

    if (prd)
    {
        sParam.local.addParam(GB_PARAM_OUT_CHECK);
    }

    if (prm)
    {
        sParam.local.addParam(GB_PARAM_WARN_THD);
        sParam.local.addParam(GB_PARAM_TIME_RERUN);
    }

    if (prd)
    {
        sParam.local.addParam(GB_PARAM_COM_PRD_KEEP);
    }

    if (prm)
    {
        sParam.local.addParam(GB_PARAM_COM_PRM_KEEP);
        sParam.local.addParam(GB_PARAM_IN_DEC);
    }

    sParam.local.addParam(GB_PARAM_FREQ);

    if (prd)
    {
        sParam.local.addParam(GB_PARAM_COR_U);
        sParam.local.addParam(GB_PARAM_COR_I);
    }

    sParam.local.addParam(GB_PARAM_NUM_OF_DEVICES);
    sParam.local.addParam(GB_PARAM_TM_K400);

    if (prm)
    {
        sParam.local.addParam(GB_PARAM_WARN_D);
        sParam.local.addParam(GB_PARAM_ALARM_D);
    }

    sParam.local.addParam(GB_PARAM_TEMP_MONITOR);
    sParam.local.addParam(GB_PARAM_TEMP_THR_HI);
    sParam.local.addParam(GB_PARAM_TEMP_THR_LOW);

    if ((comp == GB_COMP_K400_UPKC_PRD) || (comp == GB_COMP_K400_UPKC_PRM))
    {
        sParam.local.addParam(GB_PARAM_TM_SPEED);
    }

    return true;
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ������ ���������� �� ������ "���������\�����" ��� ������.
 * @param[in] type ��� ������������ ������.
 * @param[in] prd ������� �����������.
 * @param[in] prm ������� ���������.
 * @return True ���� ������ ��������, ����� False.
 *
 * *****************************************************************************
 */
bool clMenu::fillLvlSetupParamGlbOpto(eGB_TYPE_OPTO type, bool prd, bool prm)
{
    Q_ASSERT(sParam.glb.getTypeDevice() == AVANT_OPTO);
    Q_ASSERT(sParam.glb.getTypeLine() == GB_TYPE_LINE_OPTO);

    sParam.local.addParam(GB_PARAM_TIME_SYNCH);

    if (type == TYPE_OPTO_STANDART)
    {
        sParam.local.addParam(GB_PARAM_NUM_OF_DEVICE);
    }
    else
    {
        sParam.local.addParam(GB_PARAM_NUM_OF_DEVICE_RING);
    }

    if (prm)
    {
        sParam.local.addParam(GB_PARAM_TIME_RERUN);
    }

    if (prd)
    {
        sParam.local.addParam(GB_PARAM_COM_PRD_KEEP);
    }

    if (prm)
    {
        sParam.local.addParam(GB_PARAM_COM_PRM_KEEP);
    }

    if (type == TYPE_OPTO_STANDART)
    {
        sParam.local.addParam(GB_PARAM_BACKUP);
    }

    return true;
}

/**
 * *****************************************************************************
 *
 * @brief ������� � ����� ������� ������� �400�
 * @param[in] pos ��������� ������� � ������ ������.
 * @param[in] event �������.
 * @param[in] remotes ������ ��������� ���������.
 * @param[in] comp �������������.
 *
 * *****************************************************************************
 */
void clMenu::printEventTextR400m(uint8_t pos, uint8_t event, uint8_t remotes, eGB_COMP_R400M comp)
{
    const uint8_t size = DISPLAY_ROW_SIZE + 1;

    PGM_P text = getEventTextR400m(event, comp);

    if (text == nullptr)
    {
        snprintf_P(&vLCDbuf[pos], size, PSTR("������� - %d"), event);
    }
    else
    {
        uint8_t remotes_ram[4] = "?";
        PGM_P   remotes_rom    = getEventRemotesR400m(sParam.jrnEntry.getRemoteNumbers(), comp);

        if (remotes_rom != nullptr)
        {
            memcpy_P(remotes_ram, remotes_rom, SIZE_OF(remotes_ram) - 1);
            // ���������� ������� ��������� ������, �� ������ ������
            remotes_ram[SIZE_OF(remotes_ram) - 1] = '\0';
        }

        snprintf_P(&vLCDbuf[pos], size, text, remotes_ram);
    }
}


/**
 * *****************************************************************************
 *
 * @brief ���������� ����� ������� ������� �400� � ����������� �� �������������.
 * @param[in] event ��� �������.
 * @param[in] comp �������������.
 * @return ����� �������.
 * @retval nullptr � ������ ���������� ������ �������.
 *
 * *****************************************************************************
 */
PGM_P clMenu::getEventTextR400m(uint8_t event, eGB_COMP_R400M comp) const
{
    PGM_P text = nullptr;

    if (event < SIZE_OF(fcJrnEventR400_MSK))
    {
        text = fcJrnEventR400_MSK[event];

        if (comp == GB_COMP_R400M_PVZ)
        {
            if (event == 4)
            {
                text = PSTR("����");
            }
            else if (event == 26)
            {
                text = PSTR("������");
            }
            else if (event == 28)
            {
                text = PSTR("�������");
            }
        }
        else if (comp == GB_COMP_R400M_PVZU || comp == GB_COMP_R400M_PVZUE)
        {
            if (event == 21)
            {
                text = PSTR("�� - ����.������ %s");
            }
            else if (event == 22)
            {
                text = PSTR("�� - ��� ������ %s");
            }
            else if (event == 28)
            {
                text = PSTR("��%s: �� - ��� ���");
            }
            else if (event == 29)
            {
                text = PSTR("��%s: ������ � ���");
            }
            else if (event == 30)
            {
                text = PSTR("��%s: ������. ���");
            }
            else if (event == 31)
            {
                text = PSTR("��%s: ������. �.���");
            }
        }
    }

    return text;
}


/**
 * *****************************************************************************
 *
 * @brief ���������� ������ � �������� ��������� ���������.
 *
 * @param[in] numbers ������ ��������� (�� �����).
 * @param[in] comp �������������.
 * @return ������.
 *
 * *****************************************************************************
 */
PGM_P clMenu::getEventRemotesR400m(uint8_t numbers, eGB_COMP_R400M comp) const
{
    PGM_P line = nullptr;

    if (comp == GB_COMP_R400M_PVZU || comp == GB_COMP_R400M_PVZUE)
    {
        if (numbers < SIZE_OF(fcEventRemote))
        {
            line = fcEventRemote[numbers];
        }
    }
    else
    {
        line = PSTR("");
    }

    return line;
}

/**
 * *****************************************************************************
 *
 * @brief ���������� ����� �� ���������� ������������.
 * @note ������ ��� �400�.
 * @param[in] comp �������������.
 * @return ����� �� �� � ��������.
 *
 * *****************************************************************************
 */
uint16_t clMenu::GetTimeAc(eGB_COMP_R400M comp)
{
    uint16_t time;

    if (comp == GB_COMP_R400M_R400)
    {
        time = GetTimeAcR400(sParam.def.getTypeAC(),
                             sParam.glb.getNumDevices(),
                             sParam.glb.getDeviceNum(),
                             sParam.DateTime.getMinute(),
                             sParam.DateTime.getSecond());
    }
    else if (comp < GB_COMP_R400M_MAX)
    {
        time = static_cast<uint16_t>(sParam.def.getTimeToAC());
    }
    else
    {
        time = 0;
    }

    return time;
}


/**
 * *****************************************************************************
 *
 * @brief ���������� ����� �� ���������� ������������.
 * @note ������ ��� �400�.
 * @param[in] ac ����� ������������.
 * @param[in] num_devices ���������� ��������� � �����.
 * @param[in] num_device ����� ����������.
 * @param[in] min ������.
 * @param[in] sec �������.
 * @return ����� �� �� � ��������.
 *
 * *****************************************************************************
 */
uint16_t clMenu::GetTimeAcR400(
    eGB_TYPE_AC ac, eGB_NUM_DEVICES num_devices, uint8_t num_device, uint8_t min, uint8_t sec)
{
    uint16_t time;
    uint8_t  sec_offset;

    if (num_devices == GB_NUM_DEVICES_3)
    {
        sec_offset = (num_device == 3) ? (40) : (num_device == 2) ? (20) : 0;
    }
    else if (num_devices == GB_NUM_DEVICES_2)
    {
        sec_offset = (num_device == 2) ? (30) : 0;
    }
    else
    {
        sec_offset = 0;
    }

    if (sec > sec_offset)
    {
        min += 1;
        sec = 60 + sec_offset - sec;
    }
    else
    {
        sec = sec_offset - sec;
    }

    if (min >= 60)
    {
        min = 60;
    }

    if (ac == GB_TYPE_AC_AUTO_NORM || ac == GB_TYPE_AC_ACCEL)
    {
        time = sec;
    }
    else if (ac == GB_TYPE_AC_AUTO_REPEAT)
    {
        min  = (min == 0) ? 0 : 60 - min;
        time = min * 60 + sec;
    }
    else
    {
        time = 0;
    }

    return time;
}
