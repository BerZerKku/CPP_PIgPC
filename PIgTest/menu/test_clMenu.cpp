#include "gtest/gtest.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <functional>
#include <iostream>

using namespace std;

#define TEST_FRIENDS friend class clMenu_Test;

#include "menu/menu.h"

// размер массива
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class clMenu_Test : public ::testing::Test
{
public:
    clMenu *mObj = nullptr;

    clMenu_Test() { }
    virtual ~clMenu_Test() override = default;

protected:
    virtual void SetUp() override { mObj = new clMenu; }

    virtual void TearDown() override { delete mObj; }

    // вызывается однократно, до первого теста
    static void SetUpTestCase() { }

    // вызывается однократно, после последнего теста
    static void TearDownTestCase() { }
};
