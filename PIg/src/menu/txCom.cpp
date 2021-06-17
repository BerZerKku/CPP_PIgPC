/*
 * TTxCom1.cpp
 *
 *  Created on: 11 июн. 2021 г.
 *      Author: Shcheblykin
 */

#include "txCom.h"

//
TTxCom::TTxCom()
{
    clear();
    clearFastCom();
}

// очистка буфера
void TTxCom::clear()
{
    numCom1_ = numCom2_ = 0;
    cnt1_ = cnt2_ = 0;
    com1_[0] = com2_[0] = GB_COM_NO;

    for(uint_fast8_t i = 0; i < kSizeDataBuf; i++) {
        data_[i] = 0;
    }
}

//
void TTxCom::clearFastCom()
{
    cntComFast_ = 0;

    for(uint_fast8_t i = 0; i < SIZE_OF(fastCom_); i++) {
        fastCom_[i].sendType = GB_SEND_NO;
    }
}

//
bool TTxCom::addCom1(eGB_COM com, uint8_t num)
{
    bool stat = false;

    if (num > 0) {
        if (numCom1_ == 0)
            numCom1_ = 1;
        com1_[numCom1_ - 1] = com;
        stat = true;
    } else {
        if (numCom1_ < SIZE_OF(com1_)) {
            com1_[numCom1_++] = com;
            stat = true;
        }
    }

    return stat;
}

//
eGB_COM TTxCom::getCom1()
{
    eGB_COM com = GB_COM_NO;

    if (numCom1_ > 0) {
        if (cnt1_ >= numCom1_) {
            cnt1_ = 0;
        }
        com = com1_[cnt1_++];
    }

    return com;
}

//
bool TTxCom::addCom2(eGB_COM com)
{
    bool stat = false;

    if (numCom2_ < SIZE_OF(com2_)) {
        com2_[numCom2_++] = com;
        stat = true;
    }

    return stat;
}

//
eGB_COM TTxCom::getCom2()
{
    eGB_COM com = GB_COM_NO;

    if (numCom2_ > 0) {
        if (cnt2_ >= numCom2_) {
            cnt2_ = 0;
        } else {
            com = com2_[cnt2_++];
        }
    }

    return com;
}

//
void TTxCom::addFastCom(eGB_COM com)
{
    if (cntComFast_ < SIZE_OF(fastCom_)) {
        fastCom_[cntComFast_].command = com;

        // сохранение данных для быстрой команды
        for(uint_fast8_t i = 0; i < kSizeDataBuf; i++) {
            fastCom_[cntComFast_].data[i] = data_[i];
            data_[i] = 0x00;
        }

        cntComFast_++;
    }
}

//
eGB_COM TTxCom::getFastCom()
{
    eGB_COM com = GB_COM_NO;

    if (cntComFast_ > 0) {
        cntComFast_--;

        com = fastCom_[cntComFast_].command;

        // извлечение данных для быстрой команды
        for(uint_fast8_t i = 0; i < kSizeDataBuf; i++) {
            data_[i] = fastCom_[cntComFast_].data[i];
        }
    }

    return com;
}

//
eGB_SEND_TYPE TTxCom::getSendType() const
{
    eGB_SEND_TYPE sendtype = GB_SEND_NO;

    if (cntComFast_ < SIZE_OF(fastCom_)) {
        sendtype = fastCom_[cntComFast_].sendType;
    }

    return sendtype;
}

//
void TTxCom::setSendType(eGB_SEND_TYPE sendType)
{
    if ((cntComFast_ > 0) && (cntComFast_ <= SIZE_OF(fastCom_))) {
        fastCom_[cntComFast_ - 1].sendType = sendType;
    }
}
