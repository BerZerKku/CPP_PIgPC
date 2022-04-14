#ifndef BASE_HPP
#define BASE_HPP

#include "menu/menu.h"
#include <stdint.h>

/// ������ ������ ��� ������� � ���
#define BUFF_SIZE_BSP 144

extern clMenu menu;

extern void mainInit();
extern void mainCycle();

extern void    bspRead();
extern uint8_t bspWrite();
extern void    bspPushByteFrom(uint8_t byte, bool error);
extern void    bspTxEnd();

/// ����� ��� ����� � ��� �� ����������������� �����
extern uint8_t uBufUartBsp[BUFF_SIZE_BSP];

#endif  // BASE_HPP
