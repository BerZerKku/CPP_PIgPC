/*
 * paramGlb.cpp
 *
 *  Created on: 02.10.2020
 *      Author: Shcheblykin
 */

#include "paramGlb.h"


//
uint8_t TDeviceGlb::setTypeOpto(eGB_TYPE_OPTO val) {
	uint8_t act = GB_ACT_NO;

	if (val > TYPE_OPTO_MAX) {
		act = GB_ACT_ERROR;
	} else {
		if (typeLine_ == GB_TYPE_LINE_OPTO) {
			if (typeOpto_ == val) {
				act = GB_ACT_OLD;
			} else {
				typeOpto_ = val;
				act = GB_ACT_NEW;
			}
		}
	}

	return act;
}
