#include "dateTime.hpp"

//
uint8_t TDateTime::getSecond() const
{
    return second_;
}

//
bool TDateTime::setSecond(uint8_t val, bool bcd)
{
    const uint8_t max = 59;
    bool error = false;

    if (bcd) {
        error = !bcdToBin(val);
    }

    if (val > max) {
        error = true;
    }

    if (error) {
        val = max + 1;
    }

    second_ = val;

    return !error;
}

//
uint8_t TDateTime::getMinute() const
{
    return minute_;
}

//
bool TDateTime::setMinute(uint8_t val, bool bcd)
{
    const uint8_t max = 59;
    bool error = false;

    if (bcd) {
        error = !bcdToBin(val);
    }

    if (val > max) {
        error = true;
    }

    if (error) {
        val = max + 1;
    }

    minute_ = val;

    return !error;
}

//
uint8_t TDateTime::getHour() const
{
    return hour_;
}

//
bool TDateTime::setHour(uint8_t val, bool bcd)
{
    const uint8_t max = 23;
    bool error = false;

    if (bcd) {
        error = !bcdToBin(val);
    }

    if (val > max) {
        error = true;
    }

    if (error) {
        val = max + 1;
    }

    hour_ = val;

    return !error;
}

//
uint8_t TDateTime::getDay() const
{
    return day_;
}

//
bool TDateTime::setDay(uint8_t val, bool bcd)
{
    const uint8_t max = 31;
    bool error = false;

    if (bcd) {
        error = !bcdToBin(val);
    }

    if ((val == 0) || (val > getNumDaysInMonth())) {
        error = true;
    }

    if (error) {
        val = max + 1;
    }

    day_ = val;

    return !error;
}

//
uint8_t TDateTime::getNumDaysInMonth(uint8_t month, uint8_t year) const
{
    uint8_t num = 31;

	if (month == 0) {
		month = month_;
	}

	if (year == 0) {
		year = year_;
	}

	if ((month == 4) || (month == 6) || (month == 9) || (month == 11)) {
		num = 30;
	} else if (month == 2) {
        num = 28;
        // “.к. значени€ года от 0 до 255, то проверка упрощаетс€.
        if (year == 0) {
            num += 1;
        } else if ((year % 4) == 0) {
            if ((year != 100) && (year != 200)) {
                num += 1;
            }
        }
    }

	return num;
}

//
uint8_t TDateTime::getMonth() const
{
    return month_;
}

//
bool TDateTime::setMonth(uint8_t val, bool bcd)
{
    const uint8_t max = 12;
    bool error = false;

    if (bcd) {
        error = !bcdToBin(val);
    }

    if ((val == 0) || (val > max)) {
        error = true;
    }

    if (error) {
        val = max + 1;
    }

    month_ = val;

    return !error;
}

//
uint8_t TDateTime::getYear() const
{
    return year_;
}

//
bool TDateTime::setYear(uint8_t val, bool bcd)
{
    const uint8_t min = 20;
    const uint8_t max = 99;
    bool error = false;

    if (bcd) {
        error = !bcdToBin(val);
    }

    if ((val < min) || (val > max)) {
        error = true;
    }

    if (error) {
        val = 0;
    }

    year_ = val;

    return !error;
}

//
uint16_t TDateTime::getMsSecond() const
{
    return msSecond_;
}

//
bool TDateTime::setMsSecond(uint16_t val)
{
    const uint16_t max = 999;
    bool error = false;


    if (val > max) {
        error = true;
    }

    if (error) {
        val = max + 1;
    }

    msSecond_ = val;

    return !error;
}

//
uint8_t TDateTime::getDayOfWeek() const
{
    return dayWeek_;
}

//
bool TDateTime::setDayOfWeek(uint8_t val)
{
    const uint16_t max = 7;
    bool error = false;


    if ((val == 0) || (val > max)) {
        error = true;
    }

    if (error) {
        val = max + 1;
    }

    dayWeek_ = val;

    return !error;
}

//
uint8_t TDateTime::binToBcd(uint8_t val) const
{
    return static_cast<uint8_t> (((val / 10) << 4) + (val % 10));
}

bool TDateTime::bcdToBin(uint8_t &val) const
{
    uint8_t digit1 = (val >> 4) & 0x0F;
    uint8_t digit2 = val & 0x0F;

    if ((digit1 > 9) || (digit2 > 9)) {
        return false;
    }

    val = digit1*10 + digit2;
    return true;
}
