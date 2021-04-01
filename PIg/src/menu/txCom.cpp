/*
 * txCom.cpp
 *
 *  Created on: 2 окт. 2020 г.
 *      Author: Shcheblykin
 */

#include "txCom.hpp"

//
void TTxCom::clear() {
    numCom1_ = numCom2_ = 0;
    cnt1_ = cnt2_ = 0;
    cntComFast = 0;
    for(uint_fast8_t j = 0; j < MAX_NUM_FAST_COM; j++) {
        for(uint_fast8_t i = 0; i < BUFFER_SIZE; i++) {
            buf_[j] [i] = 0;
        }
    }
    com1_[0] = com2_[0] = GB_COM_NO;
    sendType = GB_SEND_NO;
}

//
bool TTxCom::addCom1(eGB_COM com, uint8_t num) {
    bool stat = false;

    if (num > 0) {
        if (numCom1_ == 0)
            numCom1_ = 1;
        com1_[numCom1_ - 1] = com;
        stat = true;
    } else {
        if (numCom1_ < MAX_NUM_COM_BUF1) {
            com1_[numCom1_++] = com;
            stat = true;
        }
    }
    return stat;
}

//
eGB_COM TTxCom::getCom1() {
    eGB_COM com= GB_COM_NO;

    if (cnt1_ < numCom1_) {
        com = com1_[cnt1_++];
    } else {
        cnt1_ = 0;
    }

    return com;
}

//
bool TTxCom::addCom2(eGB_COM com) {
    bool stat = false;
    if (numCom2_ < MAX_NUM_COM_BUF2) {
        com2_[numCom2_++] = com;
        stat = true;
    }
    return stat;
}

//
eGB_COM TTxCom::getCom2() {
    eGB_COM com= GB_COM_NO;

    if (cnt2_ < numCom2_) {
        com = com2_[cnt2_++];
    } else {
        cnt2_ = 0;
    }

    return com;
}

//
void TTxCom::addFastCom(eGB_COM com) {
    if (cntComFast < MAX_NUM_FAST_COM) {
        comFast_[cntComFast] = com;
        cntComFast++;

        // сохранение данных для быстрой команды
        for(uint_fast8_t i = 0; i < BUFFER_SIZE; i++) {
            buf_[cntComFast] [i] = buf_[0] [i];
        }
    }
}

//
eGB_COM TTxCom::getFastCom() {
    eGB_COM com = GB_COM_NO;

    if (cntComFast > 0) {
        com = comFast_[cntComFast - 1];

        // извлечение данныз для быстрой команды
        for(uint_fast8_t i = 0; i < BUFFER_SIZE; i++) {
            buf_[0] [i] = buf_[cntComFast] [i];
        }

        cntComFast--;
    }
    return com;
}

//
void TTxCom::setInt8(uint8_t byte, uint8_t num) {
    if (num < BUFFER_SIZE)
        buf_[0][num] = byte;
}

//
uint8_t TTxCom::getInt8(uint8_t num) const {
    uint8_t byte = 0;
    if (num < BUFFER_SIZE)
        byte = buf_[0][num];
    return byte;
}

//
void TTxCom::setInt16(uint16_t val) {
    *((uint16_t *) (buf_[0] + 1)) = val;
}

//
uint16_t TTxCom::getInt16() const {
    return *((uint16_t *) (buf_[0] + 1));
}

//
uint8_t* TTxCom::getBuferAddress() {
    return &buf_[0] [0];
}

//
eGB_SEND_TYPE TTxCom::getSendType() const {
    return sendType;
}

//
void TTxCom::setSendType(eGB_SEND_TYPE sendType) {
    this->sendType = sendType;
}
