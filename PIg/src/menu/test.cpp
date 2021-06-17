/*
 *  test.cpp
 *
 *  Created on: 15 июн. 2021 г.
 *      Author: Shcheblykin
 */

#include <limits.h>
#include "test.h"

//
void TTest::clear()
{
    for(uint_fast8_t i = 0; i < MAX_NUM_TEST_SIGNAL; i++) {
        signalList[i] = GB_SIGNAL_OFF;
    }
    num_ = 1;
    currentSignal_ = GB_SIGNAL_OFF;
    currentSignal2_ = GB_SIGNAL_OFF;
}

//
bool TTest::addSignalToList(eGB_TEST_SIGNAL signal)
{
    bool stat = false;

    if ((num_ < MAX_NUM_TEST_SIGNAL) && (signal < GB_SIGNAL_MAX)) {
        signalList[num_++] = signal;
        stat = true;
    }

    return stat;
}

//
void TTest::getBytes(uint8_t &cf, uint8_t &rz, eGB_TEST_SIGNAL sig)
{
    if ((sig >= GB_SIGNAL_COM1) && (sig <= GB_SIGNAL_COM32)) {
        rz = 0;
        cf = 3 + sig - GB_SIGNAL_COM1; // 3 - кол-во кч ?!
    } else if ((sig >= GB_SIGNAL_CF1) && (sig <= GB_SIGNAL_CF4)) {
        rz = 0;
        cf = 1 + sig - GB_SIGNAL_CF1;
    } else if (sig == GB_SIGNAL_CF_NO_RZ) {
        rz = 1;
        cf = 1;
    } else if (sig == GB_SIGNAL_CF_RZ) {
        rz = 2;
        cf = 1;
    } else if (sig == GB_SIGNAL_CF2_NO_RZ) {
        rz = 1;
        cf = 2;
    } else if (sig == GB_SIGNAL_CF2_RZ) {
        rz = 2;
        cf = 2;
    } else if (sig == GB_SIGNAL_RZ) {
        rz = 1;
        cf = 0;
    } else if ((sig >= GB_SIGNAL_COM1_RZ) && (sig <= GB_SIGNAL_COM8_RZ)) {
        rz = 2;
        cf = 3 + sig - GB_SIGNAL_COM1_RZ; // 3 т.к. 1 и 2 это КЧ
    } else if ((sig>=GB_SIGNAL_COM1_NO_RZ) && (sig<=GB_SIGNAL_COM8_NO_RZ)) {
        rz = 1;
        cf = 3 + sig - GB_SIGNAL_COM1_NO_RZ; // 3 т.к. 1 и 2 это КЧ
    } else if ((sig == GB_SIGNAL_CF) || (sig == GB_SIGNAL_CS)) {
        rz = 0;
        cf = 1;
    } else if ((sig >= GB_SIGNAL_COM1A) && (sig <= GB_SIGNAL_COM32C)) {
        rz = 0;
        cf = 3 + sig - GB_SIGNAL_COM1A;
    } else {
        rz = 0;
        cf = 0;
    }
}

//
void TTest::setCurrentSignal(const uint8_t *s,
                             eGB_TYPE_DEVICE type, eGB_TYPE_OPTO opto)
{
    eGB_TEST_SIGNAL signal = GB_SIGNAL_MAX;
    eGB_TEST_SIGNAL signal2 = GB_SIGNAL_MAX;

    switch(type) {
        case AVANT_K400: {
            signal = getCurrentSignalK400(s);
            signal2 = getCurrentSignalK400((s + 5));
        } break;

        case AVANT_RZSK: {
            signal = getCurrentSignalRZSK(s);
            signal2 = getCurrentSignalRZSK((s + 3));
        } break;

        case AVANT_R400: // DOWN
        case AVANT_R400M: {
            signal = getCurrentSignalR400M(s);
        } break;

        case AVANT_OPTO: {
            if (opto == TYPE_OPTO_RING_UNI) {
                signal = getCurrentSignalOptoRingUni(s);
            } else {
                signal = getCurrentSignalOpto(s);
            }
        } break;

        case AVANT_NO:  break;
        case AVANT_MAX: break;
    }

    currentSignal_ = signal;
    currentSignal2_ = signal2;
}

//
uint8_t TTest::getSetBit(uint8_t byte)
{
    uint8_t bit = 0;

    if (byte > 0) {
        uint8_t i = 0;
        while(i < CHAR_BIT) {
            if (byte & (1 << i)) {
                bit = i + 1;
                break;
            }
            i++;
        }
    }

    return bit;
}

//
eGB_TEST_SIGNAL TTest::getCurrentSignalK400(const uint8_t *s)
{
    eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

    uint8_t t = *s;
    if (t & 0x01) {
        signal = GB_SIGNAL_CF1;
    } else if (t & 0x02){
        signal = GB_SIGNAL_CF2;
    } else {
        for(uint8_t i = 0; i < (MAX_NUM_COM_PRD / 8); i++) {
            t = getSetBit(*(++s));
            if (t != 0) {
                t = (t - 1) + GB_SIGNAL_COM1 + i*8;
                signal = static_cast<eGB_TEST_SIGNAL> (t);
                break;
            }
        }
    }

    return signal;
}

//
eGB_TEST_SIGNAL TTest::getCurrentSignalRZSK(const uint8_t *s)
{
    eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

    uint8_t b1 = (*s) & 0x0F;
    uint8_t b2 = (*(++s)) & 0xFF;

    if (b2) {
        b2 = getSetBit(b2) - 1;
        if (b1 & 0x04) {
            signal = static_cast<eGB_TEST_SIGNAL> (b2 + GB_SIGNAL_COM1_NO_RZ);
        } else if (b1 & 0x08) {
            signal = static_cast<eGB_TEST_SIGNAL> (b2 + GB_SIGNAL_COM1_RZ);
        }
    } else if (b1) {
        // если есть РЗ, то есть и КЧ
        if ((b1 & 0x06) == 0x06) {
            signal = GB_SIGNAL_CF2_NO_RZ;
        } else if ((b1 & 0x0A) == 0x0A) {
            signal = GB_SIGNAL_CF2_RZ;
        } else if ((b1 & 0x05) == 0x05) {
            signal = GB_SIGNAL_CF_NO_RZ;
        } else if ((b1 & 0x09) == 0x09)  {
            signal = GB_SIGNAL_CF_RZ;
        }
    }

    return signal;
}

//
eGB_TEST_SIGNAL TTest::getCurrentSignalR400M(const uint8_t *s)
{
    eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

    // TODO Р400М 3-х концевая вывод сигналов теста
    // подумать как выводить при наличии РЗ + КЧ1, КЧ2 и т.д.

    uint8_t t = *s;
    if (t & 0x10) {
        signal = GB_SIGNAL_RZ;

        // TODO Подумать как выводить GB_SIGNAL_CF_RZ_R400M в Р400м
        // Сейчас вместо нее получается GB_SIGNAL_CF1_RZ_R400M

        // выводится "КЧ и РЗ" при их одновременно наличии
        if (t & 0x01) {
            signal = GB_SIGNAL_CF1_RZ_R400M;
        } else if (t & 0x02) {
            signal = GB_SIGNAL_CF2_RZ_R400M;
        } else if (t & 0x04) {
            signal = GB_SIGNAL_CF3_RZ_R400M;
        } else if (t & 0x08) {
            signal = GB_SIGNAL_CF4_RZ_R400M;
        }
    } else {
        if (t & 0x01) {
            signal = GB_SIGNAL_CF1;
        } else if (t & 0x02) {
            signal = GB_SIGNAL_CF2;
        } else if (t & 0x04) {
            signal = GB_SIGNAL_CF3;
        } else if (t & 0x08) {
            signal = GB_SIGNAL_CF4;
        }
    }

    return signal;
}

//
eGB_TEST_SIGNAL TTest::getCurrentSignalOpto(const uint8_t *s)
{
    eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

    uint8_t t = *s;
    if (t & 0x10) {
        signal = GB_SIGNAL_RZ;
    } else if (t & 0x01) {
        signal = GB_SIGNAL_CS;
    } else {
        for(uint8_t i = 0; i < (MAX_NUM_COM_PRD / 8); i++) {
            t = getSetBit(*(++s));
            if (t != 0) {
                t = (t - 1) + GB_SIGNAL_COM1 + i*8;
                signal = static_cast<eGB_TEST_SIGNAL> (t);
                break;
            }
        }
    }

    return signal;
}

//
eGB_TEST_SIGNAL TTest::getCurrentSignalOptoRingUni(const uint8_t *s)
{
    eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

    uint8_t t = *s;
    if (t & 0x10) {
        signal = GB_SIGNAL_RZ;
    } else if (t & 0x01) {
        signal = GB_SIGNAL_CS;
    } else {
        for(uint8_t i = 0; i < (MAX_NUM_COM_RING / 8); i++) {
            t = getSetBit(*(++s));
            if (t != 0) {
                t = (t - 1) + GB_SIGNAL_COM1A + i*8;
                signal = (eGB_TEST_SIGNAL) t;
                break;
            }
        }
    }

    return signal;
}
