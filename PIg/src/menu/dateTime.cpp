/*
 * dateTime.cpp
 *
 *  Created on: 15 июн. 2021 г.
 *      Author: Shcheblykin
 */

#include "dateTime.h"

//
TDateTime::TDateTime()
{
    msSecond_ = 0;
    second_ = 0;
    minute_ = 0;
    hour_ = 0;
    day_ = 1;
    month_ = 1;
    year_ = 0;
    dayWeek_ = 0;

    setTimeBsp_ = false;
}

//
bool TDateTime::setSecond(uint8_t val, bool bcd)
{
    const uint8_t max = 59;
    bool check = true;

    if (bcd) {
        check &= bcdToBin(val);
    }

    check &= val <= max;
    second_ = check ? val : max + 1;

    return check;
}

//
bool TDateTime::setMinute(uint8_t val, bool bcd)
{
    const uint8_t max = 59;
    bool check = true;

    if (bcd) {
        check &= bcdToBin(val);
    }

    check &= val <= max;
    minute_  = check ? val : max + 1;

    return check;
}

//
bool TDateTime::setHour(uint8_t val, bool bcd)
{
    const uint8_t max = 23;
    bool check = true;

    if (bcd) {
        check &= bcdToBin(val);
    }

    check &= val <= max;
    hour_ = check ? val : max + 1;

    return check;
}

//
bool TDateTime::setDay(uint8_t val, bool bcd)
{
    const uint8_t min = 1;
    const uint8_t max = 31;
    bool check = true;

    if (bcd) {
        check &= bcdToBin(val);
    }

    check &= (val >= min) && (val <= getNumDaysInMonth());
    day_ = check ? val : max + 1;

    return check;
}

//
bool TDateTime::setMonth(uint8_t val, bool bcd)
{
    const uint8_t min = 1;
    const uint8_t max = 12;
    bool check = true;

    if (bcd) {
        check &= bcdToBin(val);
    }

    check &= (val >= min) && (val <= max);
    month_= check ? val : max + 1;

    return check;
}

//
bool TDateTime::setYear(uint8_t val, bool bcd)
{
    const uint8_t max = 99;
    bool check = true;

    if (bcd) {
        check &= bcdToBin(val);
    }

    check &= (val <= max);
    year_= check ? val : 0;

    return check;
}

//
bool TDateTime::setMsSecond(uint16_t val)
{
    const uint16_t max = 999;
    bool check = (val <= max);

    msSecond_ = check ? val : max + 1;
    return check;
}

//
bool TDateTime::setDayOfWeek(uint8_t val)
{
    bool check = (val >= 1) && (val <= 7);

    dayWeek_ = check ? val : 8;
    return check;
}

//
uint8_t TDateTime::getNumDaysInMonth(uint8_t month, uint8_t year) const
{
    uint8_t num = 0;

    if (month == 0) {
        month = month_;
    }

    if (year == 0) {
        year = year_;
    }

    if ((month > 0) && (month < 13)) {
        if ((month == 4) || (month == 6) || (month == 9) || (month == 11)) {
            num = 30;
        } else if (month == 2) {
            // ”прощенна€ проверка, т.к. диапазон от 20[01] до 20[99].
            num = ((year % 4) == 0) ? 29 : 28;
        } else  {
            num = 31;
        }
    }

    return num;
}

//
bool TDateTime::binToBcd(uint8_t &val) const
{
    bool check = val < 100;

    if (check) {
        val = static_cast<uint8_t> (((val / 10) << 4) + (val % 10));
    }

    return check;
}

//
bool TDateTime::bcdToBin(uint8_t &val) const
{
    uint8_t digit1 = (val >> 4) & 0x0F;
    uint8_t digit2 = val & 0x0F;

    bool check = (digit1 <= 9) && (digit2 <= 9);
    if (check) {
        val = digit1*10 + digit2;
    }

    return check;
}
