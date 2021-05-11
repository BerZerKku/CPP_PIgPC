/*
 * enterParam.cpp
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#include "enterParam.h"

// ѕроверка текущего статуса работы с параметром.
bool TEnterParam::isEnable() {
	// проверка текущего статуса на достоверное значение
	if ((status_ < MENU_ENTER_PARAM_NO) || (status_ > MENU_ENTER_PARAM_MESSAGE)) {
		status_ = MENU_ENTER_PARAM_NO;
	}

	return (status_ != MENU_ENTER_PARAM_NO);
}

// —тарт ввода нового значени€ параметра.
void TEnterParam::setEnable(eMENU_ENTER_PARAM s) {

	if ((s >= MENU_ENTER_PARAM_INT) && (s <= MENU_ENTER_PASSWORD_NEW)) {
		if ((s == MENU_ENTER_PARAM_LIST) || (s == MENU_ENTER_PARAM_LIST_2)) {
			disc_ = 1;
			fract_ = 1;
		} else if ((s == MENU_ENTER_PASSWORD)
				|| (s == MENU_ENTER_PASSWORD_NEW)) {
			val_ = 0;
			min_ = 0;
			max_ = 9999;
		}
		status_ = s;
	}
}

// ќкончание ввода нового значени€ параметра.
void TEnterParam::setDisable() {
	status_ = MENU_ENTER_PARAM_NO;
	cnt_ = TIME_MESSAGE;
	com = GB_COM_NO;
	disc_ = 1;
	param_ = GB_PARAM_NULL_PARAM;
}

// ”величение текущего значени€.
uint16_t TEnterParam::incValue(uint8_t velocity) {
	eMENU_ENTER_PARAM s = status_;

	if ((s == MENU_ENTER_PARAM_INT) || (s == MENU_ENTER_PARAM_U_COR)) {
		// увеличение значени€
		//			val_ = (val_ <= (max_ - disc_)) ? val_ + disc_ : min_;
		int16_t disc = disc_;
		if (velocity >= 1) {
			if ((max_ / disc) >= 10) {
				disc *= 10;
			}
		}
		if (velocity >= 2) {
			if ((max_ / disc) >= 10) {
				disc *= 5;
			}
		}
		if (max_ != 0) {
			val_ = (val_ <= (max_ - disc)) ? val_ + disc : min_;
		}
	} else if ((s == MENU_ENTER_PARAM_LIST) ||
			(s == MENU_ENTER_PARAM_LIST_2)) {
		// в списке пор€док обратный (уменьшение индекса массива)
		val_ = (val_ > min_) ? val_ - 1 : max_;

		// FIXME ѕо приказу „иркова убрана возможность установки "лишних" совместимостей.
		if (param_ == GB_PARAM_COMP_P400) {
			if (val_ == GB_COMPATIBILITY_PVZUE) {
				val_ = (val_ > min_) ? val_ - 1 : max_;
			} else if (val_ == GB_COMPATIBILITY_LINER) {
				val_ = (val_ > min_) ? val_ - 1 : max_;
			}
		}

	} else if ((s == MENU_ENTER_PASSWORD) ||
			(s == MENU_ENTER_PASSWORD_NEW)) {
		uint16_t t = 0;

		// находитс€ разр€д заданный дискретностью
		// например дл€ числа 1234 и дискрету 100, получаетс€ 2
		t = val_ % (disc_ * 10);
		t = t / disc_;

		if (t == 9)
			val_ -= 9 * disc_;
		else
			val_ += disc_;
	}
	return val_;
}

// ”меньшение текущего значени€.
uint16_t TEnterParam::decValue(uint8_t velocity) {
	eMENU_ENTER_PARAM s = status_;

	if ((s == MENU_ENTER_PARAM_INT)
			|| (s == MENU_ENTER_PARAM_U_COR)) {
		// уменьшение значние
		int16_t disc = disc_;
		if (velocity >= 1) {
			if ((max_ / disc) >= 10) {
				disc *= 10;
			}
		}
		if (velocity >= 2) {
			if ((max_ / disc) >= 10) {
				disc *= 5;
			}
		}
		val_ = (val_ >= (min_ + disc)) ? val_ - disc : max_;
	} else if ((s == MENU_ENTER_PARAM_LIST)
			|| (s == MENU_ENTER_PARAM_LIST_2)) {
		// в списке пор€док обратный (увеличие индекса массива)
		val_ = (val_ < max_) ? val_ + 1 : min_;

		// FIXME ѕо приказу „иркова убрана возможность установки "лишних" совместимостей.
		if (param_ == GB_PARAM_COMP_P400) {
			if (val_ == GB_COMPATIBILITY_PVZUE) {
				val_ = (val_ < max_) ? val_ + 1 : min_;
			} else if (val_ == GB_COMPATIBILITY_LINER) {
				val_ = (val_ < max_) ? val_ + 1 : min_;
			}
		}
	} else if ((s == MENU_ENTER_PASSWORD)
			|| (s == MENU_ENTER_PASSWORD_NEW)) {

	}
	return val_;
}
