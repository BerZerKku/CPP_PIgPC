#include "gtest/gtest.h"
#include <cstdio>
#include <iostream>

using namespace std;

#include "menu/dateTime.h"

// размер массива
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class TDateTime_Test: public ::testing::Test
{
public:

    // Структура теста
    struct test_t {
        uint16_t value;     // Значение
        bool check;         // Результат проверки
        uint16_t result;    // Результат
        bool bcd;           // Флаг bcd
    };

    TDateTime *time;

    TDateTime_Test() {}
    virtual ~TDateTime_Test() override = default;

protected:
    virtual void SetUp() override { time = new TDateTime; }

    virtual void TearDown() override { delete time; }
};

TEST_F(TDateTime_Test, binToBcd)
{

    // Флаг bcd в этом тесте не участвует
    std::vector<test_t> tests = {
        {0,   true,  0x00, false},
        {1,   true,  0x01, false},
        {10,  true,  0x10, false},
        {51,  true,  0x51, false},
        {99,  true,  0x99, false},
        {100, false, 100,  false},
        {200, false, 200,  false},
        {UINT8_MAX, false, UINT8_MAX, false}
    };

    for(size_t i = 0; i < tests.size(); i++) {
        SCOPED_TRACE("test " + std::to_string(i));

        test_t *item = &tests.at(i);
        uint8_t value = item->value;

        ASSERT_EQ(item->check, time->binToBcd(value));
        ASSERT_EQ(item->result, value);
    }
}

TEST_F(TDateTime_Test, bcdToBin)
{

    // Флаг bcd в этом тесте не участвует
    std::vector<test_t> tests = {
        {0x00, true,  0,    false},
        {0x01, true,  1,    false},
        {0x10, true,  10,   false},
        {0x51, true,  51,   false},
        {0x99, true,  99,   false},
        {0x9A, false, 0x9A, false},
        {0x0A, false, 0x0A, false},
        {0xF0, false, 0xF0, false},
        {0xFF, false, 0xFF, false}
    };

    for(size_t i = 0; i < tests.size(); i++) {
        SCOPED_TRACE("test " + std::to_string(i));

        test_t *item = &tests.at(i);
        uint8_t value = item->value;

        ASSERT_EQ(item->check, time->bcdToBin(value));
        ASSERT_EQ(item->result, value);
    }
}

//
TEST_F(TDateTime_Test, year)
{
    std::vector<test_t> tests = {
        {0,    true,   0,  false},
        {5,    true,   5,  false},
        {20,   true,   20, false},
        {51,   true,   51, false},
        {99,   true,   99, false},
        {100,  false,  0,  false},
        {200,  false,  0,  false},
        {0x00, true,   0,  true},
        {0x05, true,   5,  true},
        {0x20, true,   20, true},
        {0x51, true,   51, true},
        {0x99, true,   99, true},
        {0x1A, false,  0,  true},
        {0x0A, false,  0,  true},
        {0xF0, false,  0,  true},
        {0xFF, false,  0,  true}
    };

    ASSERT_EQ(0U, time->getYear());

    for(size_t i = 0; i < tests.size(); i++) {
        SCOPED_TRACE("test " + std::to_string(i));

        test_t *item = &tests.at(i);

        ASSERT_EQ(item->check, time->setYear(item->value, item->bcd));
        ASSERT_EQ(item->result, time->getYear());
    }
}

//
TEST_F(TDateTime_Test, month)
{
    std::vector<test_t> tests = {
        {0,    false,  13, false},
        {1,    true,   1,  false},
        {5,    true,   5,  false},
        {12,   true,   12, false},
        {13,   false,  13, false},
        {100,  false,  13, false},
        {0x00, false,  13, true},
        {0x01, true,   1,  true},
        {0x05, true,   5,  true},
        {0x12, true,   12, true},
        {0x13, false,  13, true},
        {0x0A, false,  13, true},
        {0xF0, false,  13, true},
        {0xFF, false,  13, true}
    };

    ASSERT_EQ(1, time->getMonth());

    for(size_t i = 0; i < tests.size(); i++) {
        SCOPED_TRACE("test " + std::to_string(i));

        test_t *item = &tests.at(i);

        ASSERT_EQ(item->check, time->setMonth(item->value, item->bcd));
        ASSERT_EQ(item->result, time->getMonth());
    }
}

//
TEST_F(TDateTime_Test, numDaysInMonth)
{
    // Проверка дней в феврале

    std::vector<test_t> tests = {
        {20, true,  29, false},
        {21, true,  28, false},
        {95, true,  28, false},
        {96, true,  29, false}
    };

    for(size_t i = 0; i < tests.size(); i++) {
        SCOPED_TRACE("test " + std::to_string(i));

        test_t *item = &tests.at(i);

        ASSERT_TRUE(time->setYear(item->value, item->bcd));
        ASSERT_EQ(item->result, time->getNumDaysInMonth(2));
    }

    // Проверка дней в невисокосном году

    tests = {
        {31, true,  31, false},
        {28, true,  28, false},
        {31, true,  31, false},
        {30, true,  30, false},
        {31, true,  31, false},
        {30, true,  30, false},
        {31, true,  31, false},
        {31, true,  31, false},
        {30, true,  30, false},
        {31, true,  31, false},
        {30, true,  30, false},
        {31, true,  31, false}
    };

    ASSERT_EQ(12, tests.size());

    time->setYear(21, false);
    for(size_t i = 0; i < tests.size(); i++) {
        SCOPED_TRACE("test " + std::to_string(i));

        test_t *item = &tests.at(i);

        ASSERT_EQ(item->result, time->getNumDaysInMonth(i + 1));
    }

    // Проверка дней в високосном году

    tests = {
        {31, true,  31, false},
        {29, true,  29, false},
        {31, true,  31, false},
        {30, true,  30, false},
        {31, true,  31, false},
        {30, true,  30, false},
        {31, true,  31, false},
        {31, true,  31, false},
        {30, true,  30, false},
        {31, true,  31, false},
        {30, true,  30, false},
        {31, true,  31, false}
    };

    ASSERT_EQ(12, tests.size());

    time->setYear(24, false);
    for(size_t i = 0; i < tests.size(); i++) {
        SCOPED_TRACE("test " + std::to_string(i));

        test_t *item = &tests.at(i);

        ASSERT_EQ(item->result, time->getNumDaysInMonth(i + 1));
    }
}

//
TEST_F(TDateTime_Test, day)
{
    // проверка на основе января
    std::vector<test_t> tests = {
        {0,    false,  32, false},
        {1,    true,   1,  false},
        {5,    true,   5,  false},
        {30,   true,   30, false},
        {30,   true,   30, false},
        {31,   true,   31, false},
        {32,   false,  32, false},
        {100,  false,  32, false},
        {0x00, false,  32, true},
        {0x01, true,   1,  true},
        {0x05, true,   5,  true},
        {0x30, true,   30, true},
        {0x31, true,   31, true},
        {0x9A, false,  32, true},
        {0xF0, false,  32, true},
        {0xFF, false,  32, true}
    };

    ASSERT_EQ(1, time->getDay());

    time->setMonth(1, false);
    for(size_t i = 0; i < tests.size(); i++) {
        SCOPED_TRACE("test " + std::to_string(i));

        test_t *item = &tests.at(i);

        ASSERT_EQ(item->check,  time->setDay(item->value, item->bcd));
        ASSERT_EQ(item->result, time->getDay());
    }

    // Проверка дней в феврале

    tests = {
        {20, true,  29, false},
        {21, true,  28, false},
        {95, true,  28, false},
        {96, true,  29, false}
    };

    time->setMonth(2, false);
    for(size_t i = 0; i < tests.size(); i++) {
        SCOPED_TRACE("test " + std::to_string(i));

        test_t *item = &tests.at(i);

        ASSERT_TRUE(time->setYear(item->value, item->bcd));
        uint8_t value = time->getNumDaysInMonth(2);
        ASSERT_TRUE(time->setDay(value, false));
        ASSERT_EQ(item->result, time->getDay());
        ASSERT_FALSE(time->setDay(value + 1, false));
        ASSERT_EQ(32, time->getDay());
    }

    // Проверка дней в невисокосном году

    tests = {
        {31, true,  31, false},
        {28, true,  28, false},
        {31, true,  31, false},
        {30, true,  30, false},
        {31, true,  31, false},
        {30, true,  30, false},
        {31, true,  31, false},
        {31, true,  31, false},
        {30, true,  30, false},
        {31, true,  31, false},
        {30, true,  30, false},
        {31, true,  31, false}
    };

    ASSERT_EQ(12, tests.size());

    time->setYear(21, false);
    for(size_t i = 0; i < tests.size(); i++) {
        SCOPED_TRACE("test " + std::to_string(i));

        ASSERT_TRUE(time->setMonth(i + 1, false));

        test_t *item = &tests.at(i);

        // день устанавливается корректно
        ASSERT_EQ(item->check,  time->setDay(item->value, item->bcd));
        ASSERT_EQ(item->result, time->getDay());

        // (день + 1) уже с ошибкой
        ASSERT_FALSE(time->setDay(item->value + 1, item->bcd));
        ASSERT_EQ(32, time->getDay());
    }

    // Проверка дней в високосном году

    tests = {
        {31, true,  31, false},
        {28, true,  28, false},
        {31, true,  31, false},
        {30, true,  30, false},
        {31, true,  31, false},
        {30, true,  30, false},
        {31, true,  31, false},
        {31, true,  31, false},
        {30, true,  30, false},
        {31, true,  31, false},
        {30, true,  30, false},
        {31, true,  31, false}
    };

    ASSERT_EQ(12, tests.size());

    time->setYear(21, false);
    for(size_t i = 0; i < tests.size(); i++) {
        SCOPED_TRACE("test " + std::to_string(i));

        ASSERT_TRUE(time->setMonth(i + 1, false));

        test_t *item = &tests.at(i);

        // день устанавливается корректно
        ASSERT_EQ(item->check,  time->setDay(item->value, item->bcd));
        ASSERT_EQ(item->result, time->getDay());

        // (день + 1) уже с ошибкой
        ASSERT_FALSE(time->setDay(item->value + 1, item->bcd));
        ASSERT_EQ(32, time->getDay());
    }
}

//
TEST_F(TDateTime_Test, dayOfWeek)
{
    std::vector<test_t> tests = {
        {0,    false,  8,  false},
        {1,    true,   1,  false},
        {7,    true,   7,  false},
        {8,    false,  8,  false},
        {100,  false,  8,  false}
    };

    ASSERT_EQ(0, time->getDayOfWeek());

    for(size_t i = 0; i < tests.size(); i++) {
        SCOPED_TRACE("test " + std::to_string(i));

        test_t *item = &tests.at(i);

        ASSERT_EQ(item->check, time->setDayOfWeek(item->value));
        ASSERT_EQ(item->result, time->getDayOfWeek());
    }
}


//
TEST_F(TDateTime_Test, hour)
{
    std::vector<test_t> tests = {
        {0,    true,   0,  false},
        {1,    true,   1,  false},
        {5,    true,   5,  false},
        {23,   true,   23, false},
        {24,   false,  24, false},
        {100,  false,  24, false},
        {0x00, true,   0,  true},
        {0x01, true,   1,  true},
        {0x05, true,   5,  true},
        {0x23, true,   23, true},
        {0x24, false,  24, true},
        {0x0A, false,  24, true},
        {0xF0, false,  24, true},
        {0xFF, false,  24, true}
    };

    ASSERT_EQ(0U, time->getHour());

    for(size_t i = 0; i < tests.size(); i++) {
        SCOPED_TRACE("test " + std::to_string(i));

        test_t *item = &tests.at(i);

        ASSERT_EQ(item->check, time->setHour(item->value, item->bcd));
        ASSERT_EQ(item->result, time->getHour());
    }
}

//
TEST_F(TDateTime_Test, minute)
{
    std::vector<test_t> tests = {
        {0,    true,   0,  false},
        {1,    true,   1,  false},
        {5,    true,   5,  false},
        {59,   true,   59, false},
        {60,   false,  60, false},
        {100,  false,  60, false},
        {0x00, true,   0,  true},
        {0x01, true,   1,  true},
        {0x05, true,   5,  true},
        {0x59, true,   59, true},
        {0x60, false,  60, true},
        {0x0A, false,  60, true},
        {0xF0, false,  60, true},
        {0xFF, false,  60, true}
    };

    ASSERT_EQ(0U, time->getMinute());

    for(size_t i = 0; i < tests.size(); i++) {
        SCOPED_TRACE("test " + std::to_string(i));

        test_t *item = &tests.at(i);

        ASSERT_EQ(item->check, time->setMinute(item->value, item->bcd));
        ASSERT_EQ(item->result, time->getMinute());
    }
}

//
TEST_F(TDateTime_Test, second)
{
    std::vector<test_t> tests = {
        {0,    true,   0,  false},
        {1,    true,   1,  false},
        {5,    true,   5,  false},
        {59,   true,   59, false},
        {60,   false,  60, false},
        {100,  false,  60, false},
        {0x00, true,   0,  true},
        {0x01, true,   1,  true},
        {0x05, true,   5,  true},
        {0x59, true,   59, true},
        {0x60, false,  60, true},
        {0x0A, false,  60, true},
        {0xF0, false,  60, true},
        {0xFF, false,  60, true}
    };

    ASSERT_EQ(0U, time->getSecond());

    for(size_t i = 0; i < tests.size(); i++) {
        SCOPED_TRACE("test " + std::to_string(i));

        test_t *item = &tests.at(i);

        ASSERT_EQ(item->check, time->setSecond(item->value, item->bcd));
        ASSERT_EQ(item->result, time->getSecond());
    }
}

//
TEST_F(TDateTime_Test, millisecond)
{
    std::vector<test_t> tests = {
        {0,    true,   0,    false},
        {1,    true,   1,    false},
        {105,  true,   105,  false},
        {699,  true,   699,  false},
        {999,  true,   999,  false},
        {1000, false,  1000, false},
        {9999, false,  1000, false}
    };

    ASSERT_EQ(0U, time->getMsSecond());

    for(size_t i = 0; i < tests.size(); i++) {
        SCOPED_TRACE("test " + std::to_string(i));

        test_t *item = &tests.at(i);

        ASSERT_EQ(item->check, time->setMsSecond(item->value));
        ASSERT_EQ(item->result, time->getMsSecond());
    }
}
