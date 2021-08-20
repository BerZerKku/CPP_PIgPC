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

// \todo Перенести тесты локальных параметров в свой файл!!!!

//
TEST_F(LocalParams_Test, setNumDevices)
{
    eGB_PARAM pn = GB_PARAM_IN_DEC;
    mObj->clearParams();
    mObj->addParam(pn);

    ASSERT_EQ(Param::DEPEND_SAME_ON_NUM_DEVS, getDependSame(pn));
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
TEST_F(LocalParams_Test, getNumOfSameParams)
{
    mObj->clearParams();
    mObj->addParam(GB_PARAM_PRD_COM_LONG);
    ASSERT_EQ(int(0), mObj->getNumOfSameParams());
    mObj->setNumComPrd(32);
    ASSERT_EQ(32, mObj->getNumOfSameParams());

    mObj->clearParams();
    mObj->addParam(GB_PARAM_PRM_COM_BLOCK);
    ASSERT_EQ(int(0), mObj->getNumOfSameParams());
    mObj->setNumComPrm(31);
    ASSERT_EQ(31, mObj->getNumOfSameParams());

    mObj->clearParams();
    mObj->addParam(GB_PARAM_IN_DEC);
    ASSERT_EQ(1, mObj->getNumOfSameParams());
    mObj->setNumDevices(3);
    ASSERT_EQ(2, mObj->getNumOfSameParams());
}
