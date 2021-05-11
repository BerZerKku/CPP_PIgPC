#include "base.hpp"
#include "src/avr.h"
#include "src/drivers/ks0108.h"
#include "src/protocols/standart/protocolBspS.h"
#include "src/protocols/standart/protocolPcS.h"
#include "src/protocols/modbus/protocolPcM.h"
#include "src/protocols/iec101/protocolPcI.h"

/// ����� ������ ������ ����� (������� �� ��������� ��������), ��
#define TIME_CYLCE 100
/// ������������ ���-�� ������������ ��������� �� ��� ��� ������ �����
#define MAX_LOST_COM_FROM_BSP 10
/// ������������ ����� ��������� ��������� ��� ����������� ������ ����� � ��
#define TIME_LOST_PC_CONNECTION 3
/// ���������� ��������� ��������� ������ ��� ����������� ������ ����� � ��
#define MAX_LOST_COM_FROM_PC (TIME_LOST_PC_CONNECTION * (1000 / TIME_CYLCE))
/// ����� ������� �����������
#define TEMP_IC_ADR 0x48

/// ��������� ������� ���������� � ��
static uint8_t lastPcCom = 0;
/// ����� ��� ����� � �� �� ����������������� �����
uint8_t uBufUartPc[BUFF_SIZE_PC];
/// ����� ��� ����� � ��� �� ����������������� �����
uint8_t uBufUartBsp[BUFF_SIZE_BSP];

/// ����� ����
clMenu menu;
/// ����� ������������ ��������� ����������� � ��
static clProtocolPcS protPCs(uBufUartPc, BUFF_SIZE_PC, &menu.sParam);
/// ����� ������ ��������� MODBUS
static TProtocolPcM protPCm(&menu.sParam, uBufUartPc, BUFF_SIZE_PC);
/// ����� ��������� ��� 101 ����������� � ��
static TProtocolPcI protPCi(&menu.sParam, uBufUartPc, BUFF_SIZE_PC);
/// ����� ������������ ��������� ����������� � ���
static clProtocolBspS protBSPs(uBufUartBsp, BUFF_SIZE_BSP, &menu.sParam);

static void checkInterface();
static void checkNetAddress();
static void setProtocol(TProtocol::PROTOCOL protocol, uint16_t baud);

void bspRead() {
    // ���-�� ������������ ��������� � ���
    static uint8_t cntLostCom = MAX_LOST_COM_FROM_BSP;

    if (cntLostCom < MAX_LOST_COM_FROM_BSP) {
        cntLostCom++;
    }

    // ����� ������� �������� ������ �� ���������
    protBSPs.checkStat();
    // �������� ������� ��������� � ��� � �� ���������
    if (protBSPs.getCurrentStatus() == PRTS_STATUS_READ_OK) {
        // �������� ����������� ����� ����������� ��������� �
        // ��������� ������ ���� ��� ������������� ����������
        if (protBSPs.checkReadData()) {
            // ��������� ��������� ���������
            protBSPs.getData(lastPcCom == protBSPs.getCurrentCom());

            // �������� ������������ ������� ����������� � �� � �������
            // ���������� �� ��� � ���� ��������� ��������� ��������� �� ��
            // ��� ������� GB_COM_GET_VERS ���������� ���������� ������ ���-��
            if (lastPcCom == protBSPs.getCurrentCom()) {
                if (protPCs.isEnable()) {
                    if (protPCs.copyCommandFrom(protBSPs.buf))
                        protPCs.modifyCommand();
                }
            }
        }
        lastPcCom = 0;
        cntLostCom = 0;
    }

    menu.sParam.connectionBsp = (cntLostCom < MAX_LOST_COM_FROM_BSP);
}

void pcRead() {
    // ���-�� ������������ ��������� � ���
    static uint8_t cntLostCom = MAX_LOST_COM_FROM_PC;

    if (cntLostCom < MAX_LOST_COM_FROM_PC) {
        cntLostCom++;
    }

    if (protPCs.isEnable()) {
        // ����� ������� �������� ������ �� ���������
        protPCs.checkStat();
        // �������� ������� ������� � �� � �� ���������
        if (protPCs.getCurrentStatus() == PRTS_STATUS_READ_OK) {
            // �������� ����������� ����� ����������� ��������� �
            // ��������� ������ ���� ��� ������������� ����������
            if (protPCs.checkReadData()) {
                // ��������� ��������� ���������
                // ���� ��������� ������ ����������, �������� ��� � ���
                // (�.�. ���� ��� �� ������/��������� ������)
                if (!protPCs.getData()) {
                    // ���������� ������������� �� �������
                    lastPcCom = protPCs.getCurrentCom();

                    // ��������� ��������� � ���
                    if (protBSPs.getCurrentStatus() == PRTS_STATUS_NO) {
                        protBSPs.copyCommandFrom(protPCs.buf);
                        protPCs.setCurrentStatus(PRTS_STATUS_WAIT_ANSWER);
                    }
                }
            }
            cntLostCom = 0;
        }
    } else if (protPCm.isEnable()) {
        if (protPCm.isReadData()) {
            protPCm.readData();
        }
    } else if (protPCi.isEnable()) {
        if (protPCi.isReadData()) {
            protPCi.readData();
        }
    }

    menu.sParam.connectionPc = (cntLostCom < MAX_LOST_COM_FROM_PC);
}

uint8_t pcWrite() {
    uint8_t num = 0;
    if (protPCs.isEnable()) {
        // ����� ��������� �������� ������ ��������� �� ���������.
        protPCs.checkStat();
        // �������� ������������� �������� ������� �� �� � �� ��������
        ePRTS_STATUS stat = protPCs.getCurrentStatus();
        if (stat == PRTS_STATUS_WRITE_PC) {
            // ��������� ������ ���
            num = protPCs.trCom();
        } else if (stat == PRTS_STATUS_WRITE) {
            // �������� ������ ��
            num = protPCs.trCom();
        }
    } else if (protPCm.isEnable()) {
        num = protPCm.sendData();
    } else if (protPCi.isEnable()) {
        num =protPCi.sendData();
    }

    return num;
}

uint8_t bspWrite()
{
    uint8_t num = 0;

    // ����� ��������� �������� ������ ��������� �� ���������.
    protBSPs.checkStat();
    // �������� ��� �� ������������� �������� ������� � ��
    // ���� ���, �� ������� ������� � ����
    ePRTS_STATUS stat = protBSPs.getCurrentStatus();
    if (stat == PRTS_STATUS_WRITE_PC) {
        // ��������� ������� ��
        num = protBSPs.trCom();
    } else if (stat == PRTS_STATUS_NO) {
        // �������� ������� ���
        eGB_COM com = menu.getTxCommand();
        // ���� ���� �������, ���������� �� � ���
        if (com != GB_COM_NO) {
            num = protBSPs.sendData(com);
        }
    }

    return num;
}

void checkInterface() {
    bool changed = false;
    TInterface::INTERFACE intf = menu.sParam.Uart.Interface.get();

    if (menu.sParam.Uart.Interface.isChanged()) {
        changed = true;
    }

//    if (intf != TInterface::USB) {
//        changed |= menu.sParam.Uart.Protocol.isChanged();
//        changed |= menu.sParam.Uart.BaudRate.isChanged();
//        changed |= menu.sParam.Uart.DataBits.isChanged();
//        changed |= menu.sParam.Uart.Parity.isChanged();
//        changed |= menu.sParam.Uart.StopBits.isChanged();
//    }

    if (changed) {
//        if (intf == TInterface::USB) {
            setupUart(intf, 19200, TDataBits::_8, TParity::NONE, TStopBits::TWO);
            setProtocol(TProtocol::STANDART, 19200);
//        } else {
//            TProtocol::PROTOCOL protocol = menu.sParam.Uart.Protocol.get();
//            uint16_t baudrate = menu.sParam.Uart.BaudRate.getValue();
//            TDataBits::DATA_BITS dbits = menu.sParam.Uart.DataBits.get();
//            TParity::PARITY parity = menu.sParam.Uart.Parity.get();
//            TStopBits::STOP_BITS sbits = menu.sParam.Uart.StopBits.get();

//            setupUart(intf, baudrate, dbits, parity, sbits);
//            setProtocol(protocol, baudrate);
//        }
    }
}

void checkNetAddress() {
    if (menu.sParam.Uart.NetAddress.isChanged()) {
        uint8_t nadr = menu.sParam.Uart.NetAddress.get();
        if (protPCm.getAddressLan() != nadr) {
            protPCm.setAddressLan(nadr);
        }
        if (protPCi.getAddressLan() != nadr) {
            protPCi.setAddressLan(nadr);
        }
    }
}

void setProtocol(TProtocol::PROTOCOL protocol, uint16_t baud) {
    protPCs.setDisable();
    protPCi.setDisable();
    protPCm.setDisable();

    switch(protocol) {
        case TProtocol::STANDART:
            protPCs.setEnable(PRTS_STATUS_NO);
            menu.sParam.jrnScada.setIdle();
            break;
        case TProtocol::MODBUS:
            protPCm.setTick(baud, 50);
            protPCm.setEnable();
            menu.sParam.jrnScada.setIdle();
            break;
        case TProtocol::IEC_101:
            protPCi.setTick(baud, 50);
            protPCi.setEnable();
            menu.sParam.jrnScada.setReadyToRead();
            break;
        case TProtocol::MAX: break;
    }
}

void mainInit() {
    protBSPs.setEnable(PRTS_STATUS_NO);

//    setupUart(TInterface::BVP, 19200, TDataBits::_8, TParity::NONE, TStopBits::TWO);
//    setProtocol(TProtocol::STANDART, 19200);

    vLCDinit();
    vLCDclear();
}

void mainCycle(void) {
    static uint8_t cnt_lcd = 0;
    static uint8_t cnt_1s = 0;

    checkInterface();
//    checkNetAddress();

    if (++cnt_1s >= 10) {
        cnt_1s = 0;
    }

    if (++cnt_lcd >= (MENU_TIME_CYLCE / TIME_CYLCE)) {
        cnt_lcd = 0;
        menu.proc();
    }
}

/// ���������� �� ��������� �������� ������ UART1
void pcTxEnd() {
    if (protPCs.isEnable()) {
        protPCs.setCurrentStatus(PRTS_STATUS_NO);
    } else if (protPCm.isEnable()) {
        protPCm.setReadState();
    } else if (protPCi.isEnable()) {
        protPCi.setReadState();
    }
}


void pcPushByteFrom(uint8_t byte, bool error) {
    if (error) {
        protPCs.setCurrentStatus(PRTS_STATUS_NO);
    } else {
        if (protPCs.isEnable()) {
            protPCs.checkByte(byte);
        } else if (protPCm.isEnable()) {
            // �������� MODBUS
            protPCm.push(byte);
        } else if (protPCi.isEnable()) {
            protPCi.push(byte);
        }
    }
}

void bspTxEnd() {
    protBSPs.setCurrentStatus(PRTS_STATUS_NO);
}

void bspPushByteFrom(uint8_t byte, bool error) {
    if (error) {
        protBSPs.setCurrentStatus(PRTS_STATUS_NO);
    } else {
        protBSPs.checkByte(byte);
    }
}

void timer50us() {
    if (protPCm.isEnable()) {
        protPCm.tick();
    } else if (protPCi.isEnable()) {
        protPCi.tick();
    }
}


