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
    static const uint8_t k_max_num_punkts;

    TMenuPunkt* m_obj = nullptr;

    TMenuPunkt_Test() { }
    virtual ~TMenuPunkt_Test() override = default;

protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() { }

    virtual void SetUp() override { m_obj = new TMenuPunkt; }

    virtual void TearDown() override
    {
        delete m_obj;
        m_obj = nullptr;
    }

    // Per-test-suite tear-down.
    // Called after the last test in this test suite.
    // Can be omitted if not needed.
    static void TearDownTestSuite() { }
};

const uint8_t TMenuPunkt_Test::k_max_num_punkts = 20;

//
TEST_F(TMenuPunkt_Test, add_get)
{
    uint8_t                         counter;
    array<string, k_max_num_punkts> list;

    // начальная проверка

    ASSERT_EQ(uint8_t(0), m_obj->GetLen());

    // заполнение списка

    for (counter = 0; counter < k_max_num_punkts / 2; counter++)
    {
        list.at(counter) = to_string(counter + 1);

        ASSERT_TRUE(m_obj->Add(list.at(counter).c_str(), counter * 2));
        ASSERT_EQ(counter + 1, m_obj->GetLen());
    }

    for (; counter < k_max_num_punkts; counter++)
    {
        ASSERT_TRUE(m_obj->Add(nullptr, counter));
        ASSERT_EQ(counter + 1, m_obj->GetLen());
    }

    ASSERT_EQ(k_max_num_punkts, m_obj->GetLen());

    // проверка переполнения

    ASSERT_FALSE(m_obj->Add(to_string(33).c_str()));
    ASSERT_EQ(k_max_num_punkts, m_obj->GetLen());
    ASSERT_FALSE(m_obj->Add(nullptr, 33));
    ASSERT_EQ(k_max_num_punkts, m_obj->GetLen());

    // проверка списка

    for (counter = 0; counter < k_max_num_punkts / 2; counter++)
    {
        SCOPED_TRACE("counter = " + to_string(counter + 1));
        SCOPED_TRACE(m_obj->GetName(counter));

        ASSERT_EQ(counter * 2, m_obj->GetData(counter));
        ASSERT_STREQ(list.at(counter).c_str(), m_obj->GetName(counter));
    }

    for (; counter < k_max_num_punkts; counter++)
    {
        ASSERT_EQ(counter, m_obj->GetData(counter));
    }

    // проверка значений выходящих за диапазон

    ASSERT_EQ(uint8_t(0), m_obj->GetData(k_max_num_punkts));
    ASSERT_STREQ(nullptr, m_obj->GetName(k_max_num_punkts));

    // проверка значений выходящих за текущее количество

    m_obj->Clear();

    ASSERT_TRUE(m_obj->Add(list.at(0).c_str()));
    ASSERT_TRUE(m_obj->Add(nullptr, counter));

    ASSERT_EQ(uint8_t(0), m_obj->GetData(2));
    ASSERT_STREQ(nullptr, m_obj->GetName(2));
}


//
TEST_F(TMenuPunkt_Test, clear)
{
    uint8_t counter;

    // начальное состояние

    ASSERT_EQ(uint8_t(0), m_obj->GetLen());
    ASSERT_EQ(uint8_t(0), m_obj->GetData(0));
    ASSERT_STREQ(nullptr, m_obj->GetName(0));

    // заполнение списка

    for (counter = 0; counter < k_max_num_punkts / 2; counter++)
    {
        ASSERT_TRUE(m_obj->Add(to_string(counter + 1).c_str()));
    }

    for (; counter < k_max_num_punkts; counter++)
    {
        ASSERT_TRUE(m_obj->Add(nullptr, counter));
    }

    ASSERT_EQ(k_max_num_punkts, m_obj->GetLen());

    // проверка очистки

    m_obj->Clear();

    ASSERT_EQ(uint8_t(0), m_obj->GetLen());
    ASSERT_EQ(uint8_t(0), m_obj->GetData(0));
    ASSERT_STREQ(nullptr, m_obj->GetName(0));
}
