/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include "protocolPcS.h"

clProtocolPcS::clProtocolPcS(uint8_t *buf, uint8_t size, stGBparam *sParam) :
clProtocolS(buf, size, sParam) {

}


bool clProtocolPcS::getData() {

	bool stat = false;
	eGB_COM com = (eGB_COM) buf[2];

	if (com == GB_COM_GET_PASSWORD)	{
		uint16_t tmp = sParam_->password.get();
		buf[3] = 2;
		buf[4] = tmp >> 8;
		buf[5] = tmp;
		addCom();
		stat = true;
	} else if (com == GB_COM_SET_PASSWORD) {
		if (buf[3] == 2) {
			uint16_t tmp = ((uint16_t) buf[4] << 8) + buf[5];
			sParam_->password.set(tmp);
		}
		addCom(); // эхо
		stat = true;
	}

	return stat;
}

//
bool clProtocolPcS::update() {
	bool update = false;
	eGB_COM com = static_cast<eGB_COM> (getCurrentCom());

	if (com == GB_COM_GET_VERS) {
		update = addVersion();
	} else if (com == GB_COM_GET_MEAS) {
		update = addTemperature();
	}

	return update;
}

// Добавляет температуру.
bool clProtocolPcS::addTemperature()
{
	const uint8_t size = 16; // мин. количество байт данных в посылке с температурой

	bool update = false;
	uint8_t crc = buf[maxLen_ - 1];
	uint8_t len = buf[3];

	if (len < size) {
		for(uint8_t i = len + B1; len < size; i++, len++ ) {
			buf[i] = 0x00;
		}

		buf[B16] = sParam_->measParam.getTemperature();
		crc += buf[B16];

		crc -= buf[NUM];
		buf[NUM] = size;
		crc += buf[NUM];

		maxLen_ = buf[NUM] + 5;
		buf[maxLen_ - 1] = crc;

		update = true;
	}

	return update;
}

// Добавляет версию МК ПИ.
bool clProtocolPcS::addVersion()
{
	const uint8_t size = 19; // мин. количество байт данных в посылке с версией ПИ

	uint8_t crc = buf[maxLen_ - 1];
	uint8_t len = buf[NUM];
	if ( len < size) {
		for(uint8_t i = len + B1; len < 19; i++, len++) {
			buf[i] = 0x00;
		}
		crc += size - buf[3];
		buf[NUM] = size;
		maxLen_ = buf[NUM] + 5;
	} else {
		crc -= buf[B18];
		crc -= buf[B19];
	}
	uint16_t vers = sParam_->glb.getVersProgIC(GB_IC_PI_MCU);
	crc += (buf[B18] = (vers >> 8));
	crc += (buf[B19] = (vers & 0xFF));
	buf[maxLen_ - 1] = crc;

	return true;
}


