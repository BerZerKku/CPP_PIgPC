#include "gtest/gtest.h"
#include <cstdio>
#include <iostream>

using namespace std;

#include "menu/password.h"

// ������ �������
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class TPassword_Test : public ::testing::Test
{
public:
    TPassword *mObj = nullptr;

    TPassword_Test() { }
    virtual ~TPassword_Test() override = default;

protected:
    virtual void SetUp() override { mObj = new TPassword; }

    virtual void TearDown() override { delete mObj; }

    // ���������� ����������, �� ������� �����
    static void SetUpTestCase() { }

    // ���������� ����������, ����� ���������� �����
    static void TearDownTestCase() { }
};

TEST_F(TPassword_Test, common)
{
    // ��������� ������
    ASSERT_EQ(10000, mObj->get());

    // �������� ������������� ������

    mObj->init(10000);
    ASSERT_EQ(0, mObj->get());

    mObj->init(9999);
    ASSERT_EQ(9999, mObj->get());

    mObj->init(0);
    ASSERT_EQ(0, mObj->get());
    ASSERT_TRUE(mObj->check(0));

    // �������� ��������� ������

    ASSERT_TRUE(mObj->set(1));
    ASSERT_EQ(1, mObj->get());
    ASSERT_TRUE(mObj->check(1));

    ASSERT_TRUE(mObj->set(0));
    ASSERT_EQ(0U, mObj->get());

    ASSERT_TRUE(mObj->set(9999));
    ASSERT_EQ(9999, mObj->get());
    ASSERT_TRUE(mObj->check(9999));

    ASSERT_FALSE(mObj->set(10000));
    ASSERT_EQ(9999, mObj->get());

    // �������� ������� ������

    ASSERT_FALSE(mObj->check(9998));
    ASSERT_TRUE(mObj->check(6352));
}
