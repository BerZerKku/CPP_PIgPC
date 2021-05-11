/*
 * LocalParams.cpp
 *
 *  Created on: 25.03.2015
 *      Author: Shcheblykin
 */

#include "LocalParams.h"

LocalParams::LocalParams() {
	clearParams();
}

//	ѕереход к следующему параметру.
void LocalParams::nextParam() {
	uint8_t t = currParam;

	t = (t < (numOfParams - 1)) ? (t + 1) : 0;

	if (t != currParam) {
		currParam = t;
		currSameParam = 0;
		refreshParam();
	}
}

// ѕереход к предыдущему параметру.
void LocalParams::prevParam() {
	uint8_t t = currParam;

	t = (t > 0) ? (t - 1) : (numOfParams - 1);

	if (t != currParam) {
		currParam = t;
		currSameParam = 0;
		refreshParam();
	}
}

// ѕереход к следующему однотипному параметру.
void LocalParams::nextSameParam() {
	uint8_t t = currSameParam;

	t = (t < (getNumOfSameParams() - 1)) ? (t + 1) : 0;

	if (t != currSameParam) {
		currSameParam = t;
		refreshParam();
	}
}

// ѕереход к предыдущему однотипному параметру.
void LocalParams::prevSameParam() {
	uint8_t t = currSameParam;

	t = (t > 0) ? (t - 1) : (getNumOfSameParams() - 1);

	if (t != currSameParam) {
		currSameParam = t;
		refreshParam();
	}
}

// ƒобавление в список нового параметра.
bool LocalParams::addParam(eGB_PARAM newParam) {
	if (numOfParams >= (MAX_NUM_OF_PARAMS - 1))
		return false;

	param[numOfParams++] = newParam;
	return true;
}

// ”становка нового значени€ параметра и его проверка на корректность.
void LocalParams::setValue(int16_t val) {
    const eGB_PARAM pn = getParam();
    uint8_t fract = getFract(pn);
    uint8_t disc = getDisc(pn);

	val = val * fract;
	val = (val / disc) * disc;
	this->val = val;

	checkValue();
}

// ¬озвращает текущее значение параметра.
int16_t LocalParams::getValue() const {
    const eGB_PARAM pn = getParam();
    int16_t v = val;

    if (getParamType(pn) == Param::PARAM_BITES) {
        uint8_t cur = getNumOfCurrSameParam() - 1;
        uint8_t bite = cur % 8;

        v = ((val & (1 << bite)) != 0) ? 1 : 0;
    }

	return v;
}

// ¬озвращает текущий байт дл€ битовых параметров.
uint8_t LocalParams::getValueB() const {
	return val;
}

// ќчистка списка параметров.
void LocalParams::clearParams() {
	param[0] = GB_PARAM_NULL_PARAM;
	currParam = 0;
	val = 0;
	numOfParams = 0;
	currSameParam = 0;
	state = STATE_READ_PARAM;
}

int16_t LocalParams::getMin() const {
    return getAbsMin(getParam());
}

//	¬озвращает максимальное значение параметра.
int16_t LocalParams::getMax() const {
    uint16_t max = 0;
    const eGB_PARAM pn = getParam();

    switch(getDependMax(pn)) {
		case Param::DEPEND_MAX_NO:
            max = getAbsMax(pn);
			break;
		case Param::DEPEND_MAX_ON_NUM_DEVS:
			max = numDevices;
			break;
		case Param::DEPEND_MAX_ON_COM_PRD:
			max = numComPrd;
	}

	// дл€ строковых параметров  в максимуме записано макс.количество элементов
	// списка, поэтому скорректируем максимальное значение
    Param::PARAM_TYPE type = getParamType(pn);
	if ((type == Param::PARAM_LIST) || (type == Param::PARAM_BITES)) {
		max = getMin() + max - 1;
	}

	return max;
}

//	¬озвращает текущий номер однотипного параметра.
uint8_t LocalParams::getNumOfCurrSameParam() const {
	uint8_t num = currSameParam + 1;
	uint8_t max = getNumOfSameParams();

	num = (num > max) ? max : num;
	return num;
}

//	¬озвращает количество однотипных парметров.
uint8_t LocalParams::getNumOfSameParams() const {
    uint8_t num = 1;
    const eGB_PARAM pn = getParam();

    switch(getDependSame(pn)) {
        case Param::DEPEND_SAME_NO:
            num = getAbsMaxNumOfSameParams(pn);
            break;
        case Param::DEPEND_SAME_ON_NUM_DEVS:
            num = numDevices - 1;
            break;
        case Param::DEPEND_SAME_ON_COM_PRD:
            num = numComPrd;
            break;
        case Param::DEPEND_SAME_ON_COM_PRM:
            num = numComPrm;
            break;
    }

    return num;
}

//	ѕроверка установленного значени€ параметра на корректность.
void LocalParams::checkValue() {
	// ƒл€ совместимости с битовыми переменными \a Param::PARAM_BITES
	// провер€ем значение возвращаемое getValue(), а не работаем со значением
	// переменной this->val на пр€мую.
	int16_t val = getValue();
	if ((val >= getMin()) && (val <= getMax())) {
		state = STATE_NO_ERROR;
	} else {
		state = STATE_ERROR;
	}
}

//	ќбновление параметра.
void LocalParams::refreshParam() {
	val = 0;
	state = STATE_READ_PARAM;
}
