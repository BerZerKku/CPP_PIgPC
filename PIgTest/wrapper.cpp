#include "wrapper.h"

#include "PIg/src/drivers/keyboard.h"
#include "PIg/src/drivers/ks0108.h"
#include "PIg/src/menu/base.h"
#include "PIg/src/paramUart.h"

#include <QTextCodec>

void vKEYmain(void)
{
}

uint8_t eKEYget(void)
{
    return 0;
}

uint8_t timePressKey()
{
    return 0;
}

void vLCDclear(void)
{
}

void vLCDinit(void)
{
}

void vLCDmain(void)
{
}

void vLCDrefresh(void)
{
}

bool vLCDdrawBoard(uint8_t num)
{
    Q_UNUSED(num);
    return true;
}

bool vLCDputchar(const char* buf, uint8_t num)
{
    Q_UNUSED(buf);
    Q_UNUSED(num);
    return true;
}

void vLCDsetLed(eLCD_LED val)
{
    Q_UNUSED(val);
}

void vLCDled()
{
}

void setupUart(TInterface::INTERFACE intf,
               uint16_t              baudrate,
               TDataBits::DATA_BITS  dbits,
               TParity::PARITY       parity,
               TStopBits::STOP_BITS  sbits)
{
    qDebug() << "interface = " << intf << ", baudrate = " << baudrate << ", databits = " << dbits
             << ", parity = " << parity << ", stopbits = " << sbits;
}

void saveEeprom(eGB_PARAM param, uint16_t value)
{
    Q_UNUSED(param)
    Q_UNUSED(value)
}

uint16_t loadEeprom(eGB_PARAM param)
{
    Q_UNUSED(param)

    return 0;
}
