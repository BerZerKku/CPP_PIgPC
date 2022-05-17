#include "base.h"
#include "avr.h"
#include "drivers/ks0108.h"
#include "protocols/standart/protocolS.h"
#include "version.hpp"


/// ������������ ���-�� ������������ ��������� �� ��� ��� ������ �����
#define MAX_LOST_COM_FROM_BSP 3
/// ������������ ����� ��������� ��������� ��� ����������� ������ ����� � ��
#define TIME_LOST_PC_CONNECTION 3
/// ���������� ��������� ��������� ������ ��� ����������� ������ ����� � ��
#define MAX_LOST_COM_FROM_PC (TIME_LOST_PC_CONNECTION * (1000 / TIME_CYLCE))
/// ����� ������� �����������
#define TEMP_IC_ADR 0x48

/// ����� ��� ����� � ��� �� ����������������� �����
uint8_t uBufUartBsp[BUFF_SIZE_BSP];

/// ����� ����
clMenu menu;
/// ����� ������������ ��������� ����������� � ���
static clProtocolS s_protocol(uBufUartBsp, BUFF_SIZE_BSP);

static data_tx_x11_t s_data_tx_x11;

void bspRead()
{
    static uint8_t cntLostCom = MAX_LOST_COM_FROM_BSP;

    if (cntLostCom < MAX_LOST_COM_FROM_BSP)
    {
        cntLostCom++;
    }

    s_protocol.checkStat();
    if (s_protocol.getCurrentStatus() == PRTS_STATUS_READ_OK)
    {
        if (s_protocol.checkReadData())
        {
            s_protocol.getData(s_data_tx_x11);
            cntLostCom = 0;
        }
    }

    menu.SetConnection(cntLostCom < MAX_LOST_COM_FROM_BSP);
}


uint8_t bspWrite()
{
    uint8_t num = 0;

    // ����� ��������� �������� ������ ��������� �� ���������.
    s_protocol.checkStat();

    if (s_protocol.getCurrentStatus() == PRTS_STATUS_NO)
    {
        uint16_t keys = menu.GetKeys();
        uint8_t  data[4];

        data[0] = PROJECT_VER_MAJOR;
        data[1] = PROJECT_VER_MINOR;
        data[2] = static_cast<uint8_t>(keys >> 8);
        data[3] = static_cast<uint8_t>(keys);

        num = s_protocol.sendData(0x01, data, SIZE_OF(data));
    }

    return num;
}


void mainInit()
{
    s_data_tx_x11.lcd_buf      = menu.vLCDbuf;
    s_data_tx_x11.lcd_buf_size = SIZE_OF(menu.vLCDbuf) - 1;
    s_data_tx_x11.top_lines    = &menu.m_top_lines;
    s_data_tx_x11.led_on       = &menu.m_led_on;
    s_data_tx_x11.cursor_on    = &menu.m_cursor_on;
    s_data_tx_x11.cursor_pos   = &menu.m_cursor_pos;

    s_protocol.setEnable(PRTS_STATUS_NO);

    vLCDinit();
    vLCDclear();
}


void mainCycle(void)
{
    menu.proc();
}


void bspTxEnd()
{
    s_protocol.setCurrentStatus(PRTS_STATUS_NO);
}


void bspPushByteFrom(uint8_t byte, bool error)
{
    if (error)
    {
        s_protocol.setCurrentStatus(PRTS_STATUS_NO);
    }
    else
    {
        s_protocol.checkByte(byte);
    }
}

/* ******************************** E N D *********************************** */
