#include "gtest/gtest.h"
#include <cstdio>
#include <iostream>

using namespace std;

#define TEST_FRIENDS \
    FRIEND_TEST(clMenu_Test, strFromFlash);

#include "PIg/src/menu/menu.h"

// размер массива
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class clMenu_Test: public ::testing::Test {
   public:
    clMenu *menu;

    clMenu_Test() {}
    virtual ~clMenu_Test() {}
    virtual void Clean() {}

   protected:
    virtual void SetUp() {
        menu = new clMenu();
    }

    virtual void TearDown() {
        delete menu;
    }
};


TEST_F(clMenu_Test, strFromFlash) {
    const uint8_t size = 40;
    const uint8_t maxlen = 30;

    {
        char array[size] = {0};
        ASSERT_EQ(0, strcmp(array, menu->strFromFlash(array)));
    }

    {
        char array[size] = "1";
        ASSERT_STREQ(array, menu->strFromFlash(array));
    }

    {
        char array[size] = "123";
        ASSERT_STREQ(array, menu->strFromFlash(array));
    }

    {
        char array[size] = "12345678901234567890";
        ASSERT_STREQ(array, menu->strFromFlash(array));
    }

    {
        char array[size] = "123456789012345678901234567890";
        ASSERT_STREQ(array, menu->strFromFlash(array));
    }

    {
        char array[size] = "1234567890123456789012345678901";
        ASSERT_EQ(0, strncmp(menu->strFromFlash(array), array, maxlen));
        ASSERT_NE(0, strncmp(menu->strFromFlash(array), array, maxlen+1));
    }
}


