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

    ASSERT_EQ(uint8_t(0), m_obj->getMaxNumPunkts());

    // заполнение списка

    for (counter = 0; counter < k_max_num_punkts / 2; counter++)
    {
        list.at(counter) = to_string(counter + 1);

        ASSERT_TRUE(m_obj->addName(list.at(counter).c_str()));
        ASSERT_EQ(counter + 1, m_obj->getMaxNumPunkts());
    }

    for (; counter < k_max_num_punkts; counter++)
    {
        ASSERT_TRUE(m_obj->addNumber(counter));
        ASSERT_EQ(counter + 1, m_obj->getMaxNumPunkts());
    }

    ASSERT_EQ(k_max_num_punkts, m_obj->getMaxNumPunkts());

    // проверка переполнения

    ASSERT_FALSE(m_obj->addName(to_string(33).c_str()));
    ASSERT_EQ(k_max_num_punkts, m_obj->getMaxNumPunkts());
    ASSERT_FALSE(m_obj->addNumber(33));
    ASSERT_EQ(k_max_num_punkts, m_obj->getMaxNumPunkts());

    // проверка списка

    for (counter = 0; counter < k_max_num_punkts / 2; counter++)
    {
        SCOPED_TRACE("counter = " + to_string(counter + 1));
        SCOPED_TRACE(m_obj->getName(counter));

        ASSERT_STREQ(list.at(counter).c_str(), m_obj->getName(counter));
    }

    for (; counter < k_max_num_punkts; counter++)
    {
        ASSERT_EQ(counter, m_obj->getNumber(counter));
    }

    // проверка значений выходящих за диапазон

    ASSERT_EQ(0, m_obj->getNumber(k_max_num_punkts));
    ASSERT_STREQ("?", m_obj->getName(k_max_num_punkts));

    // проверка значений выходящих за текущее количество

    m_obj->clear();

    ASSERT_TRUE(m_obj->addName(list.at(0).c_str()));
    ASSERT_TRUE(m_obj->addNumber(counter));

    ASSERT_EQ(0, m_obj->getNumber(2));
    ASSERT_STREQ("?", m_obj->getName(2));
}


//
TEST_F(TMenuPunkt_Test, clear)
{
    uint8_t counter;

    // начальное состояние

    ASSERT_EQ(uint8_t(0), m_obj->getMaxNumPunkts());
    ASSERT_EQ(0, m_obj->getNumber(0));
    ASSERT_STREQ("?", m_obj->getName(0));

    // заполнение списка

    for (counter = 0; counter < k_max_num_punkts / 2; counter++)
    {
        ASSERT_TRUE(m_obj->addName(to_string(counter + 1).c_str()));
    }

    for (; counter < k_max_num_punkts; counter++)
    {
        ASSERT_TRUE(m_obj->addNumber(counter));
    }

    ASSERT_EQ(k_max_num_punkts, m_obj->getMaxNumPunkts());

    // проверка очистки

    m_obj->clear();

    ASSERT_EQ(uint8_t(0), m_obj->getMaxNumPunkts());
    ASSERT_EQ(0, m_obj->getNumber(0));
    ASSERT_STREQ("?", m_obj->getName(0));
}


//
TEST_F(TMenuPunkt_Test, choose)
{
    array<string, k_max_num_punkts> list;

    // заполнение списка

    for (uint8_t counter = 0; counter < k_max_num_punkts; counter++)
    {
        list.at(counter) = to_string(counter + 1);

        ASSERT_TRUE(m_obj->addName(list.at(counter).c_str()));
        ASSERT_EQ(counter + 1, m_obj->getMaxNumPunkts());
    }

    // проверка замены (меняется первое встреченное из пары значение)

    ASSERT_EQ(int(0), m_obj->choose(list.at(0).c_str(), list.at(2).c_str(), false));
    ASSERT_STREQ(list.at(2).c_str(), m_obj->getName(0));
    ASSERT_STREQ(list.at(2).c_str(), m_obj->getName(2));

    ASSERT_EQ(int(0), m_obj->choose(list.at(0).c_str(), list.at(2).c_str(), true));
    ASSERT_STREQ(list.at(0).c_str(), m_obj->getName(0));
    ASSERT_STREQ(list.at(2).c_str(), m_obj->getName(2));

    ASSERT_EQ(8, m_obj->choose(list.at(8).c_str(), list.at(13).c_str(), false));
    ASSERT_STREQ(list.at(13).c_str(), m_obj->getName(8));
    ASSERT_STREQ(list.at(13).c_str(), m_obj->getName(13));

    ASSERT_EQ(8, m_obj->choose(list.at(8).c_str(), list.at(13).c_str(), true));
    ASSERT_STREQ(list.at(8).c_str(), m_obj->getName(8));
    ASSERT_STREQ(list.at(13).c_str(), m_obj->getName(13));

    ASSERT_EQ(17, m_obj->choose(list.at(k_max_num_punkts - 1).c_str(), list.at(17).c_str(), false));
    ASSERT_STREQ(list.at(17).c_str(), m_obj->getName(17));
    ASSERT_STREQ(list.at(k_max_num_punkts - 1).c_str(), m_obj->getName(k_max_num_punkts - 1));

    ASSERT_EQ(17, m_obj->choose(list.at(k_max_num_punkts - 1).c_str(), list.at(17).c_str(), true));
    ASSERT_STREQ(list.at(k_max_num_punkts - 1).c_str(), m_obj->getName(17));
    ASSERT_STREQ(list.at(k_max_num_punkts - 1).c_str(), m_obj->getName(k_max_num_punkts - 1));

    // проверка замены при разных значениях начального номера

    ASSERT_EQ(2, m_obj->choose(list.at(0).c_str(), list.at(2).c_str(), true, 1));
    ASSERT_STREQ(list.at(0).c_str(), m_obj->getName(0));
    ASSERT_STREQ(list.at(0).c_str(), m_obj->getName(2));

    ASSERT_EQ(2, m_obj->choose(list.at(0).c_str(), list.at(2).c_str(), false, 2));
    ASSERT_STREQ(list.at(0).c_str(), m_obj->getName(0));
    ASSERT_STREQ(list.at(2).c_str(), m_obj->getName(2));

    ASSERT_EQ(17,
              m_obj->choose(list.at(k_max_num_punkts - 1).c_str(), list.at(17).c_str(), true, 17));
    ASSERT_STREQ(list.at(k_max_num_punkts - 1).c_str(), m_obj->getName(17));
    ASSERT_STREQ(list.at(k_max_num_punkts - 1).c_str(), m_obj->getName(k_max_num_punkts - 1));

    ASSERT_EQ(k_max_num_punkts - 1,
              m_obj->choose(list.at(k_max_num_punkts - 1).c_str(), list.at(17).c_str(), false, 18));
    ASSERT_STREQ(list.at(k_max_num_punkts - 1).c_str(), m_obj->getName(17));
    ASSERT_STREQ(list.at(17).c_str(), m_obj->getName(k_max_num_punkts - 1));


    // обновление списка пунктов

    m_obj->clear();
    for (uint8_t counter = 0; counter < k_max_num_punkts; counter++)
    {
        ASSERT_TRUE(m_obj->addName(list.at(5).c_str()));
    }

    // проверка попытки замены отсутствующих имен

    ASSERT_EQ(int(0), m_obj->choose(list.at(6).c_str(), list.at(0).c_str(), true));
    ASSERT_EQ(int(0), m_obj->choose(list.at(14).c_str(), list.at(1).c_str(), false));
    ASSERT_EQ(int(0), m_obj->choose(list.at(7).c_str(), list.at(8).c_str(), true));
    ASSERT_EQ(int(0),
              m_obj->choose(list.at(k_max_num_punkts - 1).c_str(), list.at(2).c_str(), false));
    ASSERT_EQ(int(0), m_obj->choose(list.at(0).c_str(), list.at(2).c_str(), true, 3));
    ASSERT_EQ(int(0),
              m_obj->choose(list.at(0).c_str(), list.at(2).c_str(), true, k_max_num_punkts));

    for (uint8_t counter = 0; counter < k_max_num_punkts; counter++)
    {
        ASSERT_STREQ(list.at(5).c_str(), m_obj->getName(counter));
    }
}
