#include <algorithm>
#include <cstdio>
#include <iostream>

#include "gtest/gtest.h"

using namespace std;

#include "parameter/LocalParams.h"

// размер массива
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class LocalParams_Test : public ::testing::Test
{
public:
    LocalParams *mObj;

    LocalParams_Test()                   = default;
    virtual ~LocalParams_Test() override = default;

protected:
    virtual void SetUp() override { mObj = new LocalParams; }

    virtual void TearDown() override { delete mObj; }

    // вызывается однократно, до первого теста
    static void SetUpTestCase() { }

    // вызывается однократно, после последнего теста
    static void TearDownTestCase() { }
};

//
TEST_F(LocalParams_Test, addParam_getParam_clearParams_nextParam_prevParam)
{
    const int size = 25;

    ASSERT_EQ(int(0), mObj->getNumOfParams());
    ASSERT_EQ(GB_PARAM_NULL_PARAM, mObj->getParam());

    // заполнение максималным количеством параметров

    for (int i = 1; i <= size; i++)
    {
        SCOPED_TRACE("Counter = " + std::to_string(i));

        ASSERT_TRUE(mObj->addParam(static_cast<eGB_PARAM>(i)));
        ASSERT_EQ(i, mObj->getNumOfParams());
        ASSERT_EQ(1, mObj->getParam());
    }

    // проверка на переполнение

    ASSERT_FALSE(mObj->addParam(static_cast<eGB_PARAM>(size + 1)));

    // проверка перехода на следующий параметр

    for (int i = 1; i <= size; i++)
    {
        SCOPED_TRACE("Counter = " + std::to_string(i));

        ASSERT_EQ(i, mObj->getParam());
        mObj->nextParam();
    }

    ASSERT_EQ(1, mObj->getParam());

    // проверка перехода на предыдущий параметр

    for (int i = size; i >= 1; i--)
    {
        SCOPED_TRACE("Counter = " + std::to_string(i));

        mObj->prevParam();
        ASSERT_EQ(i, static_cast<eGB_PARAM>(i));
    }

    mObj->prevParam();
    ASSERT_EQ(size, mObj->getParam());

    // проверка очистки параметров

    mObj->clearParams();
    ASSERT_EQ(int(0), mObj->getNumOfParams());
    ASSERT_EQ(GB_PARAM_NULL_PARAM, mObj->getParam());

    // проверка работы с не полностью заполненным списком

    ASSERT_TRUE(mObj->addParam(static_cast<eGB_PARAM>(1)));
    ASSERT_TRUE(mObj->addParam(static_cast<eGB_PARAM>(2)));
    ASSERT_EQ(1, mObj->getParam());

    mObj->nextParam();
    ASSERT_EQ(2, mObj->getParam());

    mObj->nextParam();
    ASSERT_EQ(1, mObj->getParam());

    mObj->prevParam();
    ASSERT_EQ(2, mObj->getParam());

    mObj->prevParam();
    ASSERT_EQ(1, mObj->getParam());

    mObj->clearParams();
    ASSERT_EQ(int(0), mObj->getNumOfParams());
    ASSERT_EQ(GB_PARAM_NULL_PARAM, mObj->getParam());
}

//
TEST_F(LocalParams_Test, setNumDevices)
{
    eGB_PARAM pn = GB_PARAM_IN_DEC;
    mObj->clearParams();
    mObj->addParam(pn);

    ASSERT_TRUE(Param::DEPEND_SAME_ON_NUM_DEVS == getDependSame(pn));
    ASSERT_EQ(1, mObj->getNumOfSameParams());

    mObj->setNumDevices(3);
    ASSERT_EQ(2, mObj->getNumOfSameParams());

    mObj->setNumDevices(2);
    ASSERT_EQ(1, mObj->getNumOfSameParams());

    mObj->setNumDevices(3);
    mObj->setNumDevices(0);
    ASSERT_EQ(1, mObj->getNumOfSameParams());

    mObj->setNumDevices(3);
    mObj->setNumDevices(4);
    ASSERT_EQ(1, mObj->getNumOfSameParams());
}

//
TEST_F(LocalParams_Test, setNumComPrm)
{
    eGB_PARAM pn = GB_PARAM_PRM_TIME_OFF;
    mObj->clearParams();
    mObj->addParam(pn);

    ASSERT_TRUE(Param::DEPEND_SAME_ON_COM_PRM == getDependSame(pn));
    ASSERT_EQ(int(0), mObj->getNumOfSameParams());

    mObj->setNumComPrm(1);
    ASSERT_EQ(1, mObj->getNumOfSameParams());

    mObj->setNumComPrm(10);
    ASSERT_EQ(10, mObj->getNumOfSameParams());

    mObj->setNumComPrm(255);
    ASSERT_EQ(255, mObj->getNumOfSameParams());

    mObj->setNumComPrm(0);
    ASSERT_EQ(int(0), mObj->getNumOfSameParams());
}

//
TEST_F(LocalParams_Test, setNumComPrd)
{
    eGB_PARAM pn = GB_PARAM_PRD_COM_LONG;
    mObj->clearParams();
    mObj->addParam(pn);

    ASSERT_TRUE(Param::DEPEND_SAME_ON_COM_PRD == getDependSame(pn));
    ASSERT_EQ(int(0), mObj->getNumOfSameParams());

    mObj->setNumComPrd(1);
    ASSERT_EQ(1, mObj->getNumOfSameParams());

    mObj->setNumComPrd(10);
    ASSERT_EQ(10, mObj->getNumOfSameParams());

    mObj->setNumComPrd(255);
    ASSERT_EQ(255, mObj->getNumOfSameParams());

    mObj->setNumComPrd(0);
    ASSERT_EQ(int(0), mObj->getNumOfSameParams());
}

//
TEST_F(LocalParams_Test, getNumOfSameParams)
{
    eGB_PARAM pn;

    // Для изменения списка зависимостей необходимо доработать тесты
    ASSERT_EQ(4, Param::DEPEND_SAME_MAX);

    // зависимость от количества команд передатчика

    pn = GB_PARAM_PRD_COM_LONG;
    mObj->clearParams();
    mObj->addParam(pn);

    ASSERT_TRUE(Param::DEPEND_SAME_ON_COM_PRD == getDependSame(pn));
    ASSERT_EQ(int(0), mObj->getNumOfSameParams());
    mObj->setNumComPrd(32);
    ASSERT_EQ(32, mObj->getNumOfSameParams());

    // зависимость от количества команд приемника

    pn = GB_PARAM_PRM_COM_BLOCK;
    mObj->clearParams();
    mObj->addParam(pn);

    ASSERT_TRUE(Param::DEPEND_SAME_ON_COM_PRM == getDependSame(pn));
    ASSERT_EQ(int(0), mObj->getNumOfSameParams());
    mObj->setNumComPrm(31);
    ASSERT_EQ(31, mObj->getNumOfSameParams());

    // зависимость от количества аппаратов в линии

    pn = GB_PARAM_IN_DEC;
    mObj->clearParams();
    mObj->addParam(pn);

    ASSERT_TRUE(Param::DEPEND_SAME_ON_NUM_DEVS == getDependSame(pn));
    ASSERT_EQ(1, mObj->getNumOfSameParams());
    mObj->setNumDevices(3);
    ASSERT_EQ(2, mObj->getNumOfSameParams());

    // нет зависимостей

    pn = GB_PARAM_TIME_SYNCH;
    mObj->clearParams();
    mObj->addParam(pn);
    ASSERT_EQ(1, mObj->getNumOfSameParams());
}

//
TEST_F(LocalParams_Test, getNumOfCurrSameParam_nextSameParam_prevSameParam)
{
    eGB_PARAM pn;

    // Для изменения списка зависимостей необходимо доработать тесты
    ASSERT_EQ(4, Param::DEPEND_SAME_MAX);

    // зависимость от количества команд передатчика
    // проверка смены текущего номера параметра с переходом через мин/макс

    pn = GB_PARAM_PRD_COM_BLOCK;
    mObj->clearParams();
    mObj->addParam(pn);
    mObj->setNumComPrd(4);

    ASSERT_TRUE(Param::DEPEND_SAME_ON_COM_PRD == getDependSame(pn));
    ASSERT_TRUE(Param::PARAM_BITES == getParamType(pn));

    ASSERT_EQ(1, mObj->getNumOfCurrSameParam());
    mObj->nextSameParam();
    ASSERT_EQ(2, mObj->getNumOfCurrSameParam());
    mObj->nextSameParam();
    ASSERT_EQ(3, mObj->getNumOfCurrSameParam());
    mObj->nextSameParam();
    ASSERT_EQ(4, mObj->getNumOfCurrSameParam());
    mObj->nextSameParam();
    ASSERT_EQ(1, mObj->getNumOfCurrSameParam());
    mObj->prevSameParam();
    ASSERT_EQ(4, mObj->getNumOfCurrSameParam());
    mObj->prevSameParam();
    ASSERT_EQ(3, mObj->getNumOfCurrSameParam());

    // зависимость от количества команд приемника

    pn = GB_PARAM_PRM_TIME_OFF;
    mObj->clearParams();
    mObj->addParam(pn);
    mObj->setNumComPrm(3);

    ASSERT_TRUE(Param::DEPEND_SAME_ON_COM_PRM == getDependSame(pn));
    ASSERT_FALSE(Param::PARAM_BITES == getParamType(pn));

    ASSERT_EQ(1, mObj->getNumOfCurrSameParam());
    mObj->nextSameParam();
    ASSERT_EQ(2, mObj->getNumOfCurrSameParam());
    mObj->prevSameParam();
    ASSERT_EQ(1, mObj->getNumOfCurrSameParam());

    // зависимость от количества аппаратов в линии

    pn = GB_PARAM_IN_DEC;
    mObj->clearParams();
    mObj->addParam(pn);
    mObj->setNumDevices(3);

    ASSERT_TRUE(Param::DEPEND_SAME_ON_NUM_DEVS == getDependSame(pn));
    ASSERT_FALSE(Param::PARAM_BITES == getParamType(pn));

    ASSERT_EQ(1, mObj->getNumOfCurrSameParam());
    mObj->nextSameParam();
    ASSERT_EQ(2, mObj->getNumOfCurrSameParam());
    mObj->prevSameParam();
    ASSERT_EQ(1, mObj->getNumOfCurrSameParam());

    // битовые параметры

    pn = GB_PARAM_RING_COM_TRANSIT;
    mObj->clearParams();
    mObj->addParam(pn);

    ASSERT_TRUE(Param::DEPEND_SAME_NO == getDependSame(pn));
    ASSERT_TRUE(Param::PARAM_BITES == getParamType(pn));

    ASSERT_EQ(1, mObj->getNumOfCurrSameParam());
    mObj->prevSameParam();
    ASSERT_EQ(getAbsMaxNumOfSameParams(pn), mObj->getNumOfCurrSameParam());
    mObj->nextSameParam();
    ASSERT_EQ(1, mObj->getNumOfCurrSameParam());
    mObj->nextSameParam();
    ASSERT_EQ(2, mObj->getNumOfCurrSameParam());

    // параметры без повторений

    pn = GB_PARAM_TIME_SYNCH;
    mObj->clearParams();
    mObj->addParam(pn);

    ASSERT_TRUE(Param::DEPEND_SAME_NO == getDependSame(pn));
    ASSERT_FALSE(Param::PARAM_BITES == getParamType(pn));

    ASSERT_EQ(1, mObj->getNumOfCurrSameParam());
    mObj->prevSameParam();
    ASSERT_EQ(1, mObj->getNumOfCurrSameParam());
    mObj->nextSameParam();
    ASSERT_EQ(1, mObj->getNumOfCurrSameParam());
}
