/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include "protocolPcS.h"

clProtocolPcS::clProtocolPcS(uint8_t *buf, uint8_t size, stGBparam *sParam) :
clProtocolS(buf, size, sParam) {
	lastCom = GB_COM_NO;
}


bool clProtocolPcS::getData() {

	bool stat = false;
    lastCom = static_cast<eGB_COM> (buf[2]);

    switch(getCurrentCom()) {
        case GB_COM_GET_PASSWORD: {
            uint16_t password = sParam_->password.get();
            uint8_t bytehi = static_cast<uint8_t> (password >> 8);
            uint8_t bytelo = static_cast<uint8_t> (password);
            stat = (addCom(GB_COM_GET_PASSWORD, bytehi, bytelo) > 0);
        } break;

        case GB_COM_SET_PASSWORD: {
            if (buf[NUM] == 2) {
                // ≈сли в команде 2 байта, то
                // сформируем команду установки нового значени€ парол€.
                // »наче завернем команду обратно.
                uint8_t bytehi = buf[B1];
                uint8_t bytelo = buf[B2];

                buf[COM] = GB_COM_SET_NET_ADR;
                buf[NUM] = 3;
                buf[B1] = POS_COM_NET_ADR_password;
                buf[B2] = bytelo;
                buf[B3] = bytehi;
                buf[B4] = getCRC();
            } else {
                stat = (addCom() > 0);
            }
        } break;

        case GB_COM_GET_NET_ADR: {
            if (buf[NUM] >= 2) {
                uint16_t value = *((uint16_t *) &buf[B1]);
                sParam_->glb.setVersProgIC16(value, GB_IC_BVP_STM32);
            }
            if (buf[NUM] >= 4) {
                uint16_t value = *((uint16_t *) &buf[B3]);
                sParam_->glb.setVersProgIC16(value, GB_IC_VP);
            }
            // убрать из команды все данные
            addCom(GB_COM_GET_NET_ADR);
        } break;

        default: {

        }
    }

	return stat;
}

bool clProtocolPcS::modifyCommand() {
	bool state = false;

    switch(getCurrentCom()) {
        case GB_COM_GET_VERS: {
            uint8_t crc = buf[maxLen_ - 1];
            uint8_t len = buf[3];
            if ( len < 19) {
                for(uint8_t i = len + 4; len < 19; i++, len++) {
                    buf[i] = 0x00;
                }
                crc += 19 - buf[3];
                buf[3] = len;
                maxLen_ = len + 5;
            } else {
                crc -= buf[B18];
                crc -= buf[B19];
            }
            uint16_t vers = sParam_->glb.getVersProgIC(GB_IC_PI_MCU);
            crc += (buf[B18] = (vers >> 8));
            crc += (buf[B19] = (vers & 0xFF));
            buf[maxLen_ - 1] = crc;
        } break;

        case GB_COM_SET_NET_ADR: {
            if ((lastCom == GB_COM_SET_PASSWORD) &&
                (buf[B1] == POS_COM_NET_ADR_password) &&
                (buf[NUM] == 3)) {
                uint8_t bytehi = buf[B3];
                uint8_t bytelo = buf[B2];
                state = addCom(GB_COM_SET_PASSWORD, bytehi, bytelo) > 0;
            }
        } break;
    }

    lastCom = GB_COM_NO;

	return state;
}
