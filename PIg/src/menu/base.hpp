#ifndef BASE_HPP
#define BASE_HPP

#include <stdint.h>
#include "src/menu/menu.h"
#include "src/paramUart.h"

/// Размер буфера для общения с БСП
#define BUFF_SIZE_BSP 128

/// Размер буфера для общения с ПК
#define BUFF_SIZE_PC 128

extern clMenu menu;

extern void mainInit();
extern void mainCycle();

extern void pcRead();
extern uint8_t pcWrite();
extern void pcPushByteFrom(uint8_t byte, bool error);
extern void pcTxEnd();

extern void bspRead();
extern uint8_t bspWrite();
extern void bspPushByteFrom(uint8_t byte, bool error);
extern void bspTxEnd();

extern void timer50us();
extern void setupUart(TInterface::INTERFACE intf, uint16_t baudrate,
                      TDataBits::DATA_BITS dbits, TParity::PARITY parity,
                      TStopBits::STOP_BITS sbits);

/// Буфер для связи с ПК по последовательному порту
extern uint8_t uBufUartPc[BUFF_SIZE_PC];
/// Буфер для свящи с БСП по последовательному порту
extern uint8_t uBufUartBsp[BUFF_SIZE_BSP];

#endif // BASE_HPP
