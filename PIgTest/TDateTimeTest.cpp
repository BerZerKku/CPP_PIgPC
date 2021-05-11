#include "gtest/gtest.h"
#include <cstdio>
#include <iostream>

using namespace std;

#include "PIg/src/menu/dateTime.hpp"

// размер массива
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class TDateTime_Test: public ::testing::Test {
   public:
    TDateTime *time;

    TDateTime_Test() {}
    virtual ~TDateTime_Test() override = default;

   protected:
    virtual void SetUp() override {
        time = new TDateTime;
    }

    virtual void TearDown() override {
        delete time;
    }
};

TEST_F(TDateTime_Test, binToBcd) {
    ASSERT_EQ(int{0x00}, time->binToBcd(0));
    ASSERT_EQ(0x01, time->binToBcd(1));
    ASSERT_EQ(0x10, time->binToBcd(10));
    ASSERT_EQ(0x51, time->binToBcd(51));
    ASSERT_EQ(0x99, time->binToBcd(99));

    // FIXME Нужно доделать binToBcd для проверки входного диапазона значений
    ASSERT_EQ(0xA0, time->binToBcd(100));
    ASSERT_EQ(0xA9, time->binToBcd(109));
    ASSERT_EQ(0xB0, time->binToBcd(110));
}

//
TEST_F(TDateTime_Test, year_bin)
{
    const uint8_t errorvalue = 0;
    uint8_t year = 0;

    ASSERT_EQ(int{0}, time->getYear());

    year = 20;
    ASSERT_TRUE(time->setYear(year, false));
    ASSERT_EQ(year, time->getYear());

    year = 51;
    ASSERT_TRUE(time->setYear(year, false));
    ASSERT_EQ(year, time->getYear());

    year = 99;
    ASSERT_TRUE(time->setYear(year, false));
    ASSERT_EQ(year, time->getYear());

    //

    year = 19;
    ASSERT_FALSE(time->setYear(year, false));
    ASSERT_EQ(errorvalue, time->getYear());

    year = 100;
    ASSERT_FALSE(time->setYear(year, false));
    ASSERT_EQ(errorvalue, time->getYear());

    //

    year = 20;
    ASSERT_TRUE(time->setYear(year, false));
    ASSERT_EQ(year, time->getYear());
}

//
TEST_F(TDateTime_Test, year_bcd) {
    const uint8_t errorvalue = 0;
    uint8_t year = 0x00;

    ASSERT_EQ(int{0}, time->getYear());

    year =0x20;
    ASSERT_TRUE(time->setYear(year, true));
    ASSERT_EQ(20, time->getYear());

    year = 0x51;
    ASSERT_TRUE(time->setYear(year, true));
    ASSERT_EQ(51, time->getYear());

    year = 0x99;
    ASSERT_TRUE(time->setYear(year, true));
    ASSERT_EQ(99, time->getYear());

    //

    year = 0x00;
    ASSERT_FALSE(time->setYear(year, true));
    ASSERT_EQ(errorvalue, time->getYear());

    year = 0x19;
    ASSERT_FALSE(time->setYear(year, true));
    ASSERT_EQ(errorvalue, time->getYear());

    year = 0x0A;
    ASSERT_FALSE(time->setYear(year, true));
    ASSERT_EQ(errorvalue, time->getYear());

    year = 0xA0;
    ASSERT_FALSE(time->setYear(year, true));
    ASSERT_EQ(errorvalue, time->getYear());

    //

    year = 0x20;
    ASSERT_TRUE(time->setYear(year, true));
    ASSERT_EQ(20, time->getYear());
}

//
TEST_F(TDateTime_Test, month_bin)
{
    const uint8_t errorvalue = 13;
    uint8_t month = 0;

    ASSERT_EQ(1, time->getMonth());

    month = 1;
    ASSERT_TRUE(time->setMonth(month, false));
    ASSERT_EQ(month, time->getMonth());

    month = 7;
    ASSERT_TRUE(time->setMonth(month, false));
    ASSERT_EQ(month, time->getMonth());

    month = 12;
    ASSERT_TRUE(time->setMonth(month, false));
    ASSERT_EQ(month, time->getMonth());

    //

    month = 0;
    ASSERT_FALSE(time->setMonth(month, false));
    ASSERT_EQ(errorvalue, time->getMonth());

    month = 13;
    ASSERT_FALSE(time->setMonth(month, false));
    ASSERT_EQ(errorvalue, time->getMonth());

    month = 255;
    ASSERT_FALSE(time->setMonth(month, false));
    ASSERT_EQ(errorvalue, time->getMonth());

    //

    month = 1;
    ASSERT_TRUE(time->setMonth(month, false));
    ASSERT_EQ(month, time->getMonth());
}

//
TEST_F(TDateTime_Test, month_bcd) {
    const uint8_t errorvalue = 13;
    uint8_t month = 0x00;

    ASSERT_EQ(1, time->getMonth());

    month = 0x01;
    ASSERT_TRUE(time->setMonth(month, true));
    ASSERT_EQ(1, time->getMonth());

    month = 0x07;
    ASSERT_TRUE(time->setMonth(month, true));
    ASSERT_EQ(7, time->getMonth());

    month = 0x12;
    ASSERT_TRUE(time->setMonth(month, true));
    ASSERT_EQ(12, time->getMonth());

    //

    month = 0x00;
    ASSERT_FALSE(time->setMonth(month, true));
    ASSERT_EQ(errorvalue, time->getMonth());

    month = 0x13;
    ASSERT_FALSE(time->setMonth(month, true));
    ASSERT_EQ(errorvalue, time->getMonth());

    month = 0x0A;
    ASSERT_FALSE(time->setMonth(month, true));
    ASSERT_EQ(errorvalue, time->getMonth());

    //

    month = 0x01;
    ASSERT_TRUE(time->setMonth(month, true));
    ASSERT_EQ(1, time->getMonth());
}

//
TEST_F(TDateTime_Test, dayWeek)
{
    const uint8_t errorvalue = 8;
    uint8_t dayweek = 0;

    ASSERT_EQ(1, time->getDayOfWeek());

    dayweek = 1;
    ASSERT_TRUE(time->setDayOfWeek(dayweek));
    ASSERT_EQ(dayweek, time->getDayOfWeek());

    dayweek = 3;
    ASSERT_TRUE(time->setDayOfWeek(dayweek));
    ASSERT_EQ(dayweek, time->getDayOfWeek());

    dayweek = 7;
    ASSERT_TRUE(time->setDayOfWeek(dayweek));
    ASSERT_EQ(dayweek, time->getDayOfWeek());

    //

    dayweek = 0;
    ASSERT_FALSE(time->setDayOfWeek(dayweek));
    ASSERT_EQ(errorvalue, time->getDayOfWeek());

    dayweek = 8;
    ASSERT_FALSE(time->setDayOfWeek(dayweek));
    ASSERT_EQ(errorvalue, time->getDayOfWeek());

    dayweek = 255;
    ASSERT_FALSE(time->setDayOfWeek(dayweek));
    ASSERT_EQ(errorvalue, time->getDayOfWeek());

    //

    dayweek = 1;
    ASSERT_TRUE(time->setDayOfWeek(dayweek));
    ASSERT_EQ(dayweek, time->getDayOfWeek());
}

//
TEST_F(TDateTime_Test, getNumDaysInMonth) {
    const uint8_t defvalue = 31;

    ASSERT_EQ(31, time->getNumDaysInMonth(1, 21));
    ASSERT_EQ(28, time->getNumDaysInMonth(2, 21));
    ASSERT_EQ(31, time->getNumDaysInMonth(3, 21));
    ASSERT_EQ(30, time->getNumDaysInMonth(4, 21));
    ASSERT_EQ(31, time->getNumDaysInMonth(5, 21));
    ASSERT_EQ(30, time->getNumDaysInMonth(6, 21));
    ASSERT_EQ(31, time->getNumDaysInMonth(7, 21));
    ASSERT_EQ(31, time->getNumDaysInMonth(8, 21));
    ASSERT_EQ(30, time->getNumDaysInMonth(9, 21));
    ASSERT_EQ(31, time->getNumDaysInMonth(10, 21));
    ASSERT_EQ(30, time->getNumDaysInMonth(11, 21));
    ASSERT_EQ(31, time->getNumDaysInMonth(12, 21));

    // февраль
    ASSERT_EQ(29, time->getNumDaysInMonth(2, 0));
    ASSERT_EQ(28, time->getNumDaysInMonth(2, 1));
    ASSERT_EQ(28, time->getNumDaysInMonth(2, 2));
    ASSERT_EQ(28, time->getNumDaysInMonth(2, 3));
    ASSERT_EQ(29, time->getNumDaysInMonth(2, 4));
    ASSERT_EQ(28, time->getNumDaysInMonth(2, 5));
    ASSERT_EQ(28, time->getNumDaysInMonth(2, 100));
    ASSERT_EQ(28, time->getNumDaysInMonth(2, 200));
    ASSERT_EQ(29, time->getNumDaysInMonth(2, 252));

    // ошибочный номер месяца
    ASSERT_EQ(defvalue, time->getNumDaysInMonth(0, 21));
    ASSERT_EQ(defvalue, time->getNumDaysInMonth(13, 21));
    ASSERT_EQ(defvalue, time->getNumDaysInMonth(0x11, 21));
}

// Значения месяца и года не задаются, а берутся текущие
TEST_F(TDateTime_Test, getNumDaysInMonth_preset) {
    const uint8_t defvalue = 31;

    time->setYear(21, false);

    time->setMonth(1, false);
    ASSERT_EQ(31, time->getNumDaysInMonth());

    time->setMonth(2, false);
    ASSERT_EQ(28, time->getNumDaysInMonth());

    time->setMonth(3, false);
    ASSERT_EQ(31, time->getNumDaysInMonth());

    time->setMonth(4, false);
    ASSERT_EQ(30, time->getNumDaysInMonth(4, 21));

    time->setMonth(5, false);
    ASSERT_EQ(31, time->getNumDaysInMonth());

    time->setMonth(6, false);
    ASSERT_EQ(30, time->getNumDaysInMonth());

    time->setMonth(7, false);
    ASSERT_EQ(31, time->getNumDaysInMonth());

    time->setMonth(8, false);
    ASSERT_EQ(31, time->getNumDaysInMonth());

    time->setMonth(9, false);
    ASSERT_EQ(30, time->getNumDaysInMonth());

    time->setMonth(10, false);
    ASSERT_EQ(31, time->getNumDaysInMonth());

    time->setMonth(11, false);
    ASSERT_EQ(30, time->getNumDaysInMonth());

    time->setMonth(12, false);
    ASSERT_EQ(31, time->getNumDaysInMonth());

    // февраль
    time->setMonth(2, false);

    time->setYear(0, false);
    ASSERT_EQ(29, time->getNumDaysInMonth());

    time->setYear(20, false);
    ASSERT_EQ(29, time->getNumDaysInMonth());

    time->setYear(21, false);
    ASSERT_EQ(28, time->getNumDaysInMonth());

    time->setYear(96, false);
    ASSERT_EQ(29, time->getNumDaysInMonth());

    time->setYear(99, false);
    ASSERT_EQ(28, time->getNumDaysInMonth());

    // ошибочные значения
    time->setYear(1, false);
    ASSERT_EQ(29, time->getNumDaysInMonth());
    time->setYear(21, false);
    ASSERT_EQ(28, time->getNumDaysInMonth());
    time->setMonth(13, false);
    ASSERT_EQ(defvalue, time->getNumDaysInMonth());
}

//
TEST_F(TDateTime_Test, day_bin)
{
    const uint8_t errorvalue = 13;
    uint8_t month = 0;

    ASSERT_EQ(1, time->getMonth());

    month = 1;
    ASSERT_TRUE(time->setMonth(month, false));
    ASSERT_EQ(month, time->getMonth());

    month = 7;
    ASSERT_TRUE(time->setMonth(month, false));
    ASSERT_EQ(month, time->getMonth());

    month = 12;
    ASSERT_TRUE(time->setMonth(month, false));
    ASSERT_EQ(month, time->getMonth());

    //

    month = 0;
    ASSERT_FALSE(time->setMonth(month, false));
    ASSERT_EQ(errorvalue, time->getMonth());

    month = 13;
    ASSERT_FALSE(time->setMonth(month, false));
    ASSERT_EQ(errorvalue, time->getMonth());

    month = 255;
    ASSERT_FALSE(time->setMonth(month, false));
    ASSERT_EQ(errorvalue, time->getMonth());

    //

    month = 1;
    ASSERT_TRUE(time->setMonth(month, false));
    ASSERT_EQ(month, time->getMonth());
}

//
TEST_F(TDateTime_Test, day_bcd) {
    const uint8_t errorvalue = 13;
    uint8_t month = 0x00;

    ASSERT_EQ(1, time->getMonth());

    month = 0x01;
    ASSERT_TRUE(time->setMonth(month, true));
    ASSERT_EQ(1, time->getMonth());

    month = 0x07;
    ASSERT_TRUE(time->setMonth(month, true));
    ASSERT_EQ(7, time->getMonth());

    month = 0x12;
    ASSERT_TRUE(time->setMonth(month, true));
    ASSERT_EQ(12, time->getMonth());

    //

    month = 0x00;
    ASSERT_FALSE(time->setMonth(month, true));
    ASSERT_EQ(errorvalue, time->getMonth());

    month = 0x13;
    ASSERT_FALSE(time->setMonth(month, true));
    ASSERT_EQ(errorvalue, time->getMonth());

    month = 0x0A;
    ASSERT_FALSE(time->setMonth(month, true));
    ASSERT_EQ(errorvalue, time->getMonth());

    //

    month = 0x01;
    ASSERT_TRUE(time->setMonth(month, true));
    ASSERT_EQ(1, time->getMonth());
}

//
TEST_F(TDateTime_Test, hour_bin)
{
    const uint8_t errorvalue = 24;
    uint8_t hour = 0;

    ASSERT_EQ(hour, time->getHour());

    hour = 1;
    ASSERT_TRUE(time->setHour(hour, false));
    ASSERT_EQ(hour, time->getHour());

    hour = 11;
    ASSERT_TRUE(time->setHour(hour, false));
    ASSERT_EQ(hour, time->getHour());

    hour = 23;
    ASSERT_TRUE(time->setHour(hour, false));
    ASSERT_EQ(hour, time->getHour());

    hour = 0;
    ASSERT_TRUE(time->setHour(hour, false));
    ASSERT_EQ(hour, time->getHour());

    //

    hour = 24;
    ASSERT_FALSE(time->setHour(hour, false));
    ASSERT_EQ(errorvalue, time->getHour());

    hour = 255;
    ASSERT_FALSE(time->setHour(hour, false));
    ASSERT_EQ(errorvalue, time->getHour());

    //

    hour = 0;
    ASSERT_TRUE(time->setHour(hour, false));
    ASSERT_EQ(hour, time->getHour());
}

//
TEST_F(TDateTime_Test, hour_bcd) {
    const uint8_t errorvalue = 24;
    uint8_t hour = 0x00;

    ASSERT_EQ(hour, time->getHour());

    hour =0x01;
    ASSERT_TRUE(time->setHour(hour, true));
    ASSERT_EQ(1, time->getHour());

    hour = 0x12;
    ASSERT_TRUE(time->setHour(hour, true));
    ASSERT_EQ(12, time->getHour());

    hour = 0x23;
    ASSERT_TRUE(time->setHour(hour, true));
    ASSERT_EQ(23, time->getHour());

    hour = 0x00;
    ASSERT_TRUE(time->setHour(hour, true));
    ASSERT_EQ(int{0}, time->getHour());

    //

    hour = 0x24;
    ASSERT_FALSE(time->setHour(hour, true));
    ASSERT_EQ(errorvalue, time->getHour());

    hour = 0x0A;
    ASSERT_FALSE(time->setHour(hour, true));
    ASSERT_EQ(errorvalue, time->getHour());

    //

    hour = 0x00;
    ASSERT_TRUE(time->setHour(hour, true));
    ASSERT_EQ(int{0}, time->getHour());
}

//
TEST_F(TDateTime_Test, minute_bin)
{
    const uint8_t errorvalue = 60;
    uint8_t minute = 0;

    ASSERT_EQ(minute, time->getMinute());

    minute = 1;
    ASSERT_TRUE(time->setMinute(minute, false));
    ASSERT_EQ(minute, time->getMinute());

    minute = 31;
    ASSERT_TRUE(time->setMinute(minute, false));
    ASSERT_EQ(minute, time->getMinute());

    minute = 59;
    ASSERT_TRUE(time->setMinute(minute, false));
    ASSERT_EQ(minute, time->getMinute());

    minute = 0;
    ASSERT_TRUE(time->setMinute(minute, false));
    ASSERT_EQ(minute, time->getMinute());

    //

    minute = 60;
    ASSERT_FALSE(time->setMinute(minute, false));
    ASSERT_EQ(errorvalue, time->getMinute());

    minute = 255;
    ASSERT_FALSE(time->setMinute(minute, false));
    ASSERT_EQ(errorvalue, time->getMinute());

    //

    minute = 0;
    ASSERT_TRUE(time->setMinute(minute, false));
    ASSERT_EQ(minute, time->getMinute());
}

//
TEST_F(TDateTime_Test, minute_bcd)
{
    const uint8_t errorvalue = 60;
    uint8_t minute = 0x00;

    ASSERT_EQ(minute, time->getMinute());

    minute =0x01;
    ASSERT_TRUE(time->setMinute(minute, true));
    ASSERT_EQ(1, time->getMinute());

    minute = 0x31;
    ASSERT_TRUE(time->setMinute(minute, true));
    ASSERT_EQ(31, time->getMinute());

    minute = 0x59;
    ASSERT_TRUE(time->setMinute(minute, true));
    ASSERT_EQ(59, time->getMinute());

    minute = 0x00;
    ASSERT_TRUE(time->setMinute(minute, true));
    ASSERT_EQ(int{0}, time->getMinute());

    //

    minute = 0x60;
    ASSERT_FALSE(time->setMinute(minute, true));
    ASSERT_EQ(errorvalue, time->getMinute());

    minute = 0x0A;
    ASSERT_FALSE(time->setMinute(minute, true));
    ASSERT_EQ(errorvalue, time->getMinute());

    //

    minute = 0x00;
    ASSERT_TRUE(time->setMinute(minute, true));
    ASSERT_EQ(int{0}, time->getMinute());
}

//
TEST_F(TDateTime_Test, second_bin)
{
    const uint8_t errorvalue = 60;
    uint8_t second = 0;

    ASSERT_EQ(second, time->getSecond());

    second = 1;
    ASSERT_TRUE(time->setSecond(second, false));
    ASSERT_EQ(second, time->getSecond());

    second = 31;
    ASSERT_TRUE(time->setSecond(second, false));
    ASSERT_EQ(second, time->getSecond());

    second = 59;
    ASSERT_TRUE(time->setSecond(second, false));
    ASSERT_EQ(second, time->getSecond());

    second = 0;
    ASSERT_TRUE(time->setSecond(second, false));
    ASSERT_EQ(second, time->getSecond());

    //

    second = 60;
    ASSERT_FALSE(time->setSecond(second, false));
    ASSERT_EQ(errorvalue, time->getSecond());

    second = 255;
    ASSERT_FALSE(time->setSecond(second, false));
    ASSERT_EQ(errorvalue, time->getSecond());

    //

    second = 0;
    ASSERT_TRUE(time->setSecond(second, false));
    ASSERT_EQ(second, time->getSecond());
}

//
TEST_F(TDateTime_Test, second_bcd)
{
    const uint8_t errorvalue = 60;
    uint8_t second = 0x00;

    ASSERT_EQ(second, time->getSecond());

    second =0x01;
    ASSERT_TRUE(time->setSecond(second, true));
    ASSERT_EQ(1, time->getSecond());

    second = 0x31;
    ASSERT_TRUE(time->setSecond(second, true));
    ASSERT_EQ(31, time->getSecond());

    second = 0x59;
    ASSERT_TRUE(time->setSecond(second, true));
    ASSERT_EQ(59, time->getSecond());

    second = 0x00;
    ASSERT_TRUE(time->setSecond(second, true));
    ASSERT_EQ(int{0}, time->getSecond());

    //

    second = 0x60;
    ASSERT_FALSE(time->setSecond(second, true));
    ASSERT_EQ(errorvalue, time->getSecond());

    second = 0x0A;
    ASSERT_FALSE(time->setSecond(second, true));
    ASSERT_EQ(errorvalue, time->getSecond());

    //

    second = 0x00;
    ASSERT_TRUE(time->setSecond(second, true));
    ASSERT_EQ(second, time->getSecond());
}

//
TEST_F(TDateTime_Test, msSecond) {
    const uint16_t errorvalue = 1000;
    uint16_t ms = 0x00;

    ASSERT_EQ(int{0}, time->getMsSecond());

    ms = 1;
    ASSERT_TRUE(time->setMsSecond(ms));
    ASSERT_EQ(ms, time->getMsSecond());

    ms = 14;
    ASSERT_TRUE(time->setMsSecond(ms));
    ASSERT_EQ(ms, time->getMsSecond());

    ms = 476;
    ASSERT_TRUE(time->setMsSecond(ms));
    ASSERT_EQ(ms, time->getMsSecond());

    ms = 999;
    ASSERT_TRUE(time->setMsSecond(ms));
    ASSERT_EQ(ms, time->getMsSecond());

    ms = 0x00;
    ASSERT_TRUE(time->setMsSecond(ms));
    ASSERT_EQ(int{0}, time->getMsSecond());

    //

    ms = 1000;
    ASSERT_FALSE(time->setMsSecond(ms));
    ASSERT_EQ(errorvalue, time->getMsSecond());

    ms = 1001;
    ASSERT_FALSE(time->setMsSecond(ms));
    ASSERT_EQ(errorvalue, time->getMsSecond());

    ms = 38756;
    ASSERT_FALSE(time->setMsSecond(ms));
    ASSERT_EQ(errorvalue, time->getMsSecond());

    //

    ms = 0;
    ASSERT_TRUE(time->setMsSecond(ms));
    ASSERT_EQ(ms, time->getMsSecond());
}

