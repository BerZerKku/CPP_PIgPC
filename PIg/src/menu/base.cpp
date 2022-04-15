#include "base.h"
#include "avr.h"
#include "drivers/ks0108.h"
#include "protocols/standart/protocolS.h"


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
            s_protocol.getData(reinterpret_cast<uint8_t *>(menu.vLCDbuf), SIZE_BUF_STRING);
        }
        cntLostCom = 0;
    }

    menu.SetConnection(cntLostCom < MAX_LOST_COM_FROM_BSP);
}


uint8_t bspWrite()
{
    uint8_t num = 0;

    // ����� ��������� �������� ������ ��������� �� ���������.
    s_protocol.checkStat();

    if (s_protocol.getCurrentStatus() == PRTS_STATUS_WRITE_READY)
    {
        uint16_t keys = menu.GetKeys();

        num = s_protocol.sendData(0x01, reinterpret_cast<uint8_t *>(&keys), sizeof(keys));
    }

    return num;
}


void mainInit()
{
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
