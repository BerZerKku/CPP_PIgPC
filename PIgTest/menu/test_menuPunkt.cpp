#include "gtest/gtest.h"
#include <cstdio>
#include <iostream>

using namespace std;

#include "PIg/src/menu/menuPunkt.h"

// размер массива
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class TMenuPunkt_Test : public ::testing::Test
{
public:
    TMenuPunkt*      mObj = nullptr;
    static const int mSize;

    TMenuPunkt_Test() { }
    virtual ~TMenuPunkt_Test() override = default;

protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() { }

    virtual void SetUp() override { mObj = new TMenuPunkt; }

    virtual void TearDown() override
    {
        delete mObj;
        mObj = nullptr;
    }

    // Per-test-suite tear-down.
    // Called after the last test in this test suite.
    // Can be omitted if not needed.
    static void TearDownTestSuite() { }
};

const int TMenuPunkt_Test::mSize = 20;

//
TEST_F(TMenuPunkt_Test, GetSize)
{
    ASSERT_EQ(mSize, mObj->GetSize());
}


//
TEST_F(TMenuPunkt_Test, Add_Get)
{
    uint8_t              counter;
    array<string, mSize> list;

    // начальная проверка

    ASSERT_EQ(uint8_t(0), mObj->GetLen());

    // заполнение списка

    for (counter = 0; counter < mSize / 2; counter++)
    {
        list.at(counter) = to_string(counter + 1);

        ASSERT_TRUE(mObj->Add(list.at(counter).c_str(), counter * 2));
        ASSERT_EQ(counter + 1, mObj->GetLen());
    }

    for (; counter < mSize; counter++)
    {
        ASSERT_TRUE(mObj->Add(nullptr, counter));
        ASSERT_EQ(counter + 1, mObj->GetLen());
    }

    ASSERT_EQ(mSize, mObj->GetLen());

    // проверка переполнения

    ASSERT_FALSE(mObj->Add(to_string(33).c_str()));
    ASSERT_EQ(mSize, mObj->GetLen());
    ASSERT_FALSE(mObj->Add(nullptr, 33));
    ASSERT_EQ(mSize, mObj->GetLen());

    // проверка списка

    for (counter = 0; counter < mSize / 2; counter++)
    {
        SCOPED_TRACE("counter = " + to_string(counter + 1));
        SCOPED_TRACE(mObj->GetName(counter));

        ASSERT_EQ(counter * 2, mObj->GetData(counter));
        ASSERT_STREQ(list.at(counter).c_str(), mObj->GetName(counter));
    }

    for (; counter < mSize; counter++)
    {
        ASSERT_EQ(counter, mObj->GetData(counter));
    }

    // проверка значений выходящих за диапазон

    ASSERT_EQ(uint8_t(0), mObj->GetData(mSize));
    ASSERT_STREQ(nullptr, mObj->GetName(mSize));

    // проверка значений выходящих за текущее количество

    mObj->Clear();

    ASSERT_TRUE(mObj->Add(list.at(0).c_str()));
    ASSERT_TRUE(mObj->Add(nullptr, counter));

    ASSERT_EQ(uint8_t(0), mObj->GetData(2));
    ASSERT_STREQ(nullptr, mObj->GetName(2));
}


//
TEST_F(TMenuPunkt_Test, Clear)
{
    uint8_t counter;

    // начальное состояние

    ASSERT_EQ(uint8_t(0), mObj->GetLen());
    ASSERT_EQ(uint8_t(0), mObj->GetData(0));
    ASSERT_STREQ(nullptr, mObj->GetName(0));

    // заполнение списка

    for (counter = 0; counter < mSize / 2; counter++)
    {
        ASSERT_TRUE(mObj->Add(to_string(counter + 1).c_str()));
    }

    for (; counter < mSize; counter++)
    {
        ASSERT_TRUE(mObj->Add(nullptr, counter));
    }

    ASSERT_EQ(mSize, mObj->GetLen());

    // проверка очистки

    mObj->Clear();

    ASSERT_EQ(uint8_t(0), mObj->GetLen());
    ASSERT_EQ(uint8_t(0), mObj->GetData(0));
    ASSERT_STREQ(nullptr, mObj->GetName(0));
}


//
TEST_F(TMenuPunkt_Test, Change)
{
    uint8_t counter;

    // начальное состояние

    ASSERT_EQ(uint8_t(0), mObj->GetLen());
    ASSERT_EQ(uint8_t(0), mObj->GetData(0));
    ASSERT_STREQ(nullptr, mObj->GetName(0));

    // заполнение списка

    for (counter = 0; counter < mSize / 2; counter++)
    {
        ASSERT_TRUE(mObj->Add(to_string(counter + 1).c_str()));
    }

    ASSERT_EQ(counter, mObj->GetLen());

    // изменение произвольного пунтка, в данном случае 1

    mObj->Change(1, "Hello", 58);
    ASSERT_EQ(58, mObj->GetData(1));
    ASSERT_STREQ("Hello", mObj->GetName(1));
    ASSERT_EQ(counter, mObj->GetLen());
}
