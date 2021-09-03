#ifndef BASE_HPP
#define BASE_HPP

#include "menu/menu.h"
#include "paramUart.h"
#include <stdint.h>

/// ������ ������ ��� ������� � ���
#define BUFF_SIZE_BSP 128

/// ������ ������ ��� ������� � ��
#define BUFF_SIZE_PC 128

extern clMenu menu;

extern void mainInit();
extern void mainCycle();

extern void    pcRead();
extern uint8_t pcWrite();
extern void    pcPushByteFrom(uint8_t byte, bool error);
extern void    pcTxEnd();

extern void    bspRead();
extern uint8_t bspWrite();
extern void    bspPushByteFrom(uint8_t byte, bool error);
extern void    bspTxEnd();

extern void timer50us();
extern void setupUart(TInterface::INTERFACE intf,
                      uint16_t              baudrate,
                      TDataBits::DATA_BITS  dbits,
                      TParity::PARITY       parity,
                      TStopBits::STOP_BITS  sbits);

/// ����� ��� ����� � �� �� ����������������� �����
extern uint8_t uBufUartPc[BUFF_SIZE_PC];
/// ����� ��� ����� � ��� �� ����������������� �����
extern uint8_t uBufUartBsp[BUFF_SIZE_BSP];

#endif  // BASE_HPP
