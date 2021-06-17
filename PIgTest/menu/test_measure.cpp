#include "gtest/gtest.h"
#include <cstdio>
#include <iostream>

using namespace std;

#include "menu/measure.h"

// размер массива
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class TMeasure_Test: public ::testing::Test
{
public:
    TMeasure *mObj;

    TMeasure_Test() {}
    virtual ~TMeasure_Test() override = default;

protected:
    virtual void SetUp() override { mObj = new TMeasure; }

    virtual void TearDown() override { delete mObj; }
};

//
TEST_F(TMeasure_Test, common)
{
    // Проверка на то что установка одного параметра не меняет другой

    ASSERT_TRUE(mObj->setVoltageDef(1));
    ASSERT_TRUE(mObj->setVoltageDef2(2));
    ASSERT_TRUE(mObj->setVoltageCf(3));
    ASSERT_TRUE(mObj->setVoltageCf2(4));
    ASSERT_TRUE(mObj->setVoltageNoise(5));
    ASSERT_TRUE(mObj->setVoltageNoise2(6));
    ASSERT_TRUE(mObj->setVoltageOut(7, 8));
    ASSERT_TRUE(mObj->setCurrentOut(9));
    ASSERT_TRUE(mObj->setResistOut(10));
    ASSERT_TRUE(mObj->setPulseWidth(11));
    ASSERT_TRUE(mObj->setD(12));
    ASSERT_TRUE(mObj->setTemperature(13));
    ASSERT_TRUE(mObj->setFreqDev(14));

    ASSERT_EQ(1,  mObj->getVoltageDef());
    ASSERT_EQ(2,  mObj->getVoltageDef2());
    ASSERT_EQ(3,  mObj->getVoltageCf());
    ASSERT_EQ(4,  mObj->getVoltageCf2());
    ASSERT_EQ(5,  mObj->getVoltageNoise());
    ASSERT_EQ(6,  mObj->getVoltageNoise2());
    ASSERT_EQ(78, mObj->getVoltageOut());
    ASSERT_EQ(9,  mObj->getCurrentOut());
    ASSERT_EQ(10, mObj->getResistOut());
    ASSERT_EQ(11, mObj->getPulseWidth());
    ASSERT_EQ(12, mObj->getD());
    ASSERT_EQ(13, mObj->getTemperature());
    ASSERT_EQ(14, mObj->getFreqDev());
}

//
TEST_F(TMeasure_Test, voltageDef)
{
    const int8_t min = -99;
    const int8_t max = 99;

    struct result_t {
        bool set;
        int8_t value;
    };

    struct testData_t {
        string testName;
        int8_t value;
        result_t result;
    };

    std::vector<testData_t> testdata = {
        {"data00", min - 1, {false, 0}},
        {"data01", min,     {true,  min}},
        {"data02", -1,      {true,  -1}},
        {"data03", 0,       {true,  0}},
        {"data04", 1,       {true,  1}},
        {"data05", max,     {true,  max}},
        {"data06", 15,      {true,  15}},
        {"data07", max + 1, {false, 15}}
    };

    ASSERT_EQ(0U, mObj->getVoltageDef());

    for(testData_t &item : testdata) {
        ASSERT_EQ(item.result.set, mObj->setVoltageDef(item.value));
        ASSERT_EQ(item.result.value, mObj->getVoltageDef());
    }

    ASSERT_EQ(0U, mObj->getVoltageDef2());

    for(testData_t &item : testdata) {
        ASSERT_EQ(item.result.set, mObj->setVoltageDef2(item.value));
        ASSERT_EQ(item.result.value, mObj->getVoltageDef2());
    }
}

//
TEST_F(TMeasure_Test, voltageCf)
{
    const int8_t min = -99;
    const int8_t max = 99;

    struct result_t {
        bool set;
        int8_t value;
    };

    struct testData_t {
        string testName;
        int8_t value;
        result_t result;
    };

    std::vector<testData_t> testdata = {
        {"data00", min - 1, {false, 0}},
        {"data01", min,     {true,  min}},
        {"data02", -1,      {true,  -1}},
        {"data03", 0,       {true,  0}},
        {"data04", 1,       {true,  1}},
        {"data05", max,     {true,  max}},
        {"data06", 15,      {true,  15}},
        {"data07", max + 1, {false, 15}}
    };

    ASSERT_EQ(0U, mObj->getVoltageCf());

    for(testData_t &item : testdata) {
        SCOPED_TRACE(item.testName);

        ASSERT_EQ(item.result.set, mObj->setVoltageCf(item.value));
        ASSERT_EQ(item.result.value, mObj->getVoltageCf());
    }

    ASSERT_EQ(0U, mObj->getVoltageCf2());

    for(testData_t &item : testdata) {
        SCOPED_TRACE(item.testName);

        ASSERT_EQ(item.result.set, mObj->setVoltageCf2(item.value));
        ASSERT_EQ(item.result.value, mObj->getVoltageCf2());
    }
}

//
TEST_F(TMeasure_Test, voltageOut)
{
    const uint8_t maxInt = 99;
    const uint8_t maxFrt = 9;

    struct result_t {
        bool set;
        uint16_t value;
    };

    struct testData_t {
        string testName;
        uint8_t valueInt;
        uint8_t valueFrt;
        result_t result;
    };

    std::vector<testData_t> testdata = {
        {"data00", 1, 2,            {true,  12}},
        {"data01", 0, 0,            {true,  0}},
        {"data02", 0, 9,            {true,  9}},
        {"data03", 9, 0,            {true,  90}},
        {"data04", maxInt, maxFrt,  {true,  999}},
        {"data05", 65, 8,           {true,  658}},
        {"data06", maxInt+1, 0,     {false, 658}},
        {"data07", 0, maxFrt+1,     {false, 658}}
    };

    ASSERT_EQ(0U, mObj->getVoltageOut());
    ASSERT_EQ(0U, mObj->getVoltageOutInt());
    ASSERT_EQ(0U, mObj->getVoltageOutFract());

    for(testData_t &item : testdata) {
        SCOPED_TRACE(item.testName);

        ASSERT_EQ(item.result.set, mObj->setVoltageOut(item.valueInt, item.valueFrt));
        ASSERT_EQ(item.result.value, mObj->getVoltageOut());
        ASSERT_EQ(item.result.value/10, mObj->getVoltageOutInt());
        ASSERT_EQ(item.result.value%10, mObj->getVoltageOutFract());
    }
}

//
TEST_F(TMeasure_Test, currentOut)
{
    const uint16_t max = 999;

    struct result_t {
        bool set;
        uint16_t value;
    };

    struct testData_t {
        string testName;
        uint16_t value;
        result_t result;
    };

    std::vector<testData_t> testdata = {
        {"data00", 1,     {true,  1}},
        {"data01", 0,     {true,  0}},
        {"data02", 613,   {true,  613}},
        {"data03", max,   {true,  max}},
        {"data04", 37,    {true,  37}},
        {"data05", max+1, {false, 37}}
    };

    ASSERT_EQ(0U, mObj->getCurrentOut());

    for(testData_t &item : testdata) {
        SCOPED_TRACE(item.testName);

        ASSERT_EQ(item.result.set, mObj->setCurrentOut(item.value));
        ASSERT_EQ(item.result.value, mObj->getCurrentOut());
    }
}

//
TEST_F(TMeasure_Test, resistOut)
{
    const uint16_t max = 999;

    struct result_t {
        bool set;
        uint16_t value;
    };

    struct testData_t {
        string testName;
        uint16_t value;
        result_t result;
    };

    std::vector<testData_t> testdata = {
        {"data00", 1,     {true,  1}},
        {"data01", 0,     {true,  0}},
        {"data02", 613,   {true,  613}},
        {"data03", max,   {true,  max}},
        {"data04", 37,    {true,  37}},
        {"data05", max+1, {false, 37}}
    };

    ASSERT_EQ(0U, mObj->getResistOut());

    for(testData_t &item : testdata) {
        SCOPED_TRACE(item.testName);

        ASSERT_EQ(item.result.set, mObj->setResistOut(item.value));
        ASSERT_EQ(item.result.value, mObj->getResistOut());
    }
}

//
TEST_F(TMeasure_Test, voltageNoise)
{
    const int8_t min = -99;
    const int8_t max = 99;

    struct result_t {
        bool set;
        int8_t value;
    };

    struct testData_t {
        string testName;
        int8_t value;
        result_t result;
    };

    std::vector<testData_t> testdata = {
        {"data00", min - 1, {false, 0}},
        {"data01", min,     {true,  min}},
        {"data02", -1,      {true,  -1}},
        {"data03", 0,       {true,  0}},
        {"data04", 1,       {true,  1}},
        {"data05", max,     {true,  max}},
        {"data06", 15,      {true,  15}},
        {"data07", max + 1, {false, 15}}
    };

    ASSERT_EQ(0U, mObj->getVoltageNoise());

    for(testData_t &item : testdata) {
        SCOPED_TRACE(item.testName);

        ASSERT_EQ(item.result.set, mObj->setVoltageNoise(item.value));
        ASSERT_EQ(item.result.value, mObj->getVoltageNoise());
    }

    ASSERT_EQ(0U, mObj->getVoltageNoise2());

    for(testData_t &item : testdata) {
        SCOPED_TRACE(item.testName);

        ASSERT_EQ(item.result.set, mObj->setVoltageNoise2(item.value));
        ASSERT_EQ(item.result.value, mObj->getVoltageNoise2());
    }
}

//
TEST_F(TMeasure_Test, pulseWidth)
{
    const uint16_t max = 360;

    struct result_t {
        bool set;
        uint16_t value;
    };

    struct testData_t {
        string testName;
        uint16_t value;
        result_t result;
    };

    std::vector<testData_t> testdata = {
        {"data00", 1,     {true,  1}},
        {"data01", 0,     {true,  0}},
        {"data02", 271,   {true,  271}},
        {"data03", max,   {true,  max}},
        {"data04", 37,    {true,  37}},
        {"data05", max+1, {false, 37}}
    };

    ASSERT_EQ(0U, mObj->getPulseWidth());

    for(testData_t &item : testdata) {
        SCOPED_TRACE(item.testName);

        ASSERT_EQ(item.result.set, mObj->setPulseWidth(item.value));
        ASSERT_EQ(item.result.value, mObj->getPulseWidth());
    }
}

//
TEST_F(TMeasure_Test, D)
{
    const int8_t min = -64;
    const int8_t max = 64;

    struct result_t {
        bool set;
        int8_t value;
    };

    struct testData_t {
        string testName;
        int8_t value;
        result_t result;
    };

    std::vector<testData_t> testdata = {
        {"data00", min - 1, {false, 0}},
        {"data01", min,     {true,  min}},
        {"data02", -1,      {true,  -1}},
        {"data03", 0,       {true,  0}},
        {"data04", 1,       {true,  1}},
        {"data05", max,     {true,  max}},
        {"data06", 15,      {true,  15}},
        {"data07", max + 1, {false, 15}}
    };

    ASSERT_EQ(0U, mObj->getD());

    for(testData_t &item : testdata) {
        SCOPED_TRACE(item.testName);

        ASSERT_EQ(item.result.set, mObj->setD(item.value));
        ASSERT_EQ(item.result.value, mObj->getD());
    }
}

//
TEST_F(TMeasure_Test, Temperature)
{
    const int8_t min = -100;
    const int8_t max = 125;

    struct result_t {
        bool set;
        int8_t value;
    };

    struct testData_t {
        string testName;
        int8_t value;
        result_t result;
    };

    std::vector<testData_t> testdata = {
        {"data00", min - 1, {false, -100}},
        {"data01", min,     {true,  min}},
        {"data02", -1,      {true,  -1}},
        {"data03", 0,       {true,  0}},
        {"data04", 1,       {true,  1}},
        {"data05", max,     {true,  max}},
        {"data06", 15,      {true,  15}},
        {"data07", max + 1, {false, 15}}
    };

    ASSERT_EQ(min, mObj->getTemperature());

    for(testData_t &item : testdata) {
        SCOPED_TRACE(item.testName);

        ASSERT_EQ(item.result.set, mObj->setTemperature(item.value));
        ASSERT_EQ(item.result.value, mObj->getTemperature());
    }
}

//
TEST_F(TMeasure_Test, freqDiv)
{
    const uint8_t max = 100;

    struct result_t {
        bool set;
        uint8_t value;
    };

    struct testData_t {
        string testName;
        uint8_t value;
        result_t result;
    };

    std::vector<testData_t> testdata = {
        {"data00", 1,     {true,  1}},
        {"data01", 0,     {true,  0}},
        {"data02", 56,    {true,  56}},
        {"data03", max,   {true,  max}},
        {"data04", 37,    {true,  37}},
        {"data05", max+1, {false, 37}}
    };

    ASSERT_EQ(0U, mObj->getFreqDev());

    for(testData_t &item : testdata) {
        SCOPED_TRACE(item.testName);

        ASSERT_EQ(item.result.set, mObj->setFreqDev(item.value));
        ASSERT_EQ(item.result.value, mObj->getFreqDev());
    }
}
