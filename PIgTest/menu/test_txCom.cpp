#include "gtest/gtest.h"
#include <cstdio>
#include <iostream>

using namespace std;

#include "src/menu/txCom.h"

// размер массива
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class TTxCom_Test: public ::testing::Test
{
public:
    const uint8_t kSizeBufComFast = 3;
    const uint8_t kSizeBufCom1 = 10;
    const uint8_t kSizeBufCom2 = 4;
    const uint8_t kSizeDataBuf = 16;


    TTxCom *mTxCom = nullptr;
    const uint8_t *mBuf = nullptr;

    TTxCom_Test() {}
    virtual ~TTxCom_Test() override = default;

protected:
    virtual void SetUp() override {
        mTxCom = new TTxCom;
        mBuf = mTxCom->getBuferAddress();
    }

    virtual void TearDown() override {
        delete mTxCom;

        mTxCom = nullptr;
        mBuf = nullptr;
    }
};

//
TEST_F(TTxCom_Test, int8)
{
    ASSERT_EQ(0, mTxCom->getInt8(0));
    ASSERT_EQ(0, mBuf[0]);

    ASSERT_EQ(0, mTxCom->getInt8(1));
    ASSERT_EQ(0, mBuf[1]);

    ASSERT_EQ(0, mTxCom->getInt8(kSizeDataBuf-1));
    ASSERT_EQ(0, mBuf[kSizeDataBuf-1]);

    ASSERT_EQ(0, mTxCom->getInt8(kSizeDataBuf));

    mTxCom->setInt8(1, 0);
    mTxCom->setInt8(2, 1);
    mTxCom->setInt8(16, kSizeDataBuf-1);
    mTxCom->setInt8(17, kSizeDataBuf);

    ASSERT_EQ(1, mTxCom->getInt8(0));
    ASSERT_EQ(1, mBuf[0]);

    ASSERT_EQ(2, mTxCom->getInt8(1));
    ASSERT_EQ(2, mBuf[1]);

    ASSERT_EQ(16, mTxCom->getInt8(kSizeDataBuf-1));
    ASSERT_EQ(16, mBuf[kSizeDataBuf-1]);

    ASSERT_EQ(0, mTxCom->getInt8(kSizeDataBuf));
}

TEST_F(TTxCom_Test, int16) {

    ASSERT_EQ(0, mTxCom->getInt16());
    ASSERT_EQ(0, mBuf[0]);
    ASSERT_EQ(0, mBuf[1]);
    ASSERT_EQ(0, mBuf[2]);

    mTxCom->setInt16(0xABCD);

    ASSERT_EQ(0xABCD, mTxCom->getInt16());
    ASSERT_EQ(0x00, mBuf[0]);
    ASSERT_EQ(0xCD, mBuf[1]);
    ASSERT_EQ(0xAB, mBuf[2]);
    ASSERT_EQ(0x00, mBuf[3]);

    mTxCom->setInt16(0x1234);

    ASSERT_EQ(0x1234, mTxCom->getInt16());
    ASSERT_EQ(0x00, mBuf[0]);
    ASSERT_EQ(0x34, mBuf[1]);
    ASSERT_EQ(0x12, mBuf[2]);
    ASSERT_EQ(0x00, mBuf[3]);
}

//
TEST_F(TTxCom_Test, com1)
{
    for(int i = 0; i < kSizeBufCom1 + 5; i++) {
        if (mTxCom->getCom1() != GB_COM_NO) {
            ASSERT_TRUE(false) << "number " << i;
        }
    }

    for(int i = 0; i < kSizeBufCom1; i++) {
        ASSERT_TRUE(mTxCom->addCom1(static_cast<eGB_COM> (i))) << "number " << i;
    }

    ASSERT_FALSE(mTxCom->addCom1(static_cast<eGB_COM> (kSizeBufCom1)));

    for(int i = 0; i < kSizeBufCom1; i++) {
        ASSERT_EQ(static_cast<eGB_COM> (i), mTxCom->getCom1()) << "number " << i;
    }

    ASSERT_EQ(GB_COM_NO, mTxCom->getCom1());
}

//
TEST_F(TTxCom_Test, fastCom)
{
    ASSERT_EQ(mTxCom->getFastCom(), GB_COM_NO);

    mTxCom->addFastCom(GB_COM_GET_TIME);

    ASSERT_EQ(mTxCom->getFastCom(), GB_COM_GET_TIME);
    ASSERT_EQ(mTxCom->getFastCom(), GB_COM_NO);

    mTxCom->setInt8(1, 0);
    mTxCom->setInt8(2, 1);
    mTxCom->setInt8(kSizeDataBuf, kSizeDataBuf-1);
    mTxCom->addFastCom(GB_COM_SET_TIME);
    mTxCom->setSendType(GB_SEND_INT8);

    // Проверка возможности добавления нескольких одинаковых команд с разными данными
    mTxCom->setInt16(0x1234);
    mTxCom->addFastCom(GB_COM_SET_TIME);
    mTxCom->setSendType(GB_SEND_DOP_BITES);

    mTxCom->setInt8(0xFF, 0);
    mTxCom->setInt16(0xABCD);
    mTxCom->addFastCom(GB_COM_SET_CONTROL);
    mTxCom->setSendType(GB_SEND_DOP_INT8);

    // Проверка выхода за размеры буфера быстрых команд, размер которого 3.
    mTxCom->setInt8(0x11, 0);
    mTxCom->setInt16(0x1111);
    mTxCom->addFastCom(GB_COM_GET_PASSWORD);

    ASSERT_EQ(GB_COM_SET_CONTROL, mTxCom->getFastCom());
    ASSERT_EQ(0xFF, mTxCom->getInt8(0));
    ASSERT_EQ(GB_SEND_DOP_INT8, mTxCom->getSendType());
    ASSERT_EQ(0xABCD, mTxCom->getInt16());

    ASSERT_EQ(GB_COM_SET_TIME, mTxCom->getFastCom());
    ASSERT_EQ(0x00, mTxCom->getInt8(0));
    ASSERT_EQ(0x1234, mTxCom->getInt16());
    ASSERT_EQ(GB_SEND_DOP_BITES, mTxCom->getSendType());

    ASSERT_EQ(GB_COM_SET_TIME, mTxCom->getFastCom());
    ASSERT_EQ(1, mTxCom->getInt8(0));
    ASSERT_EQ(2, mTxCom->getInt8(1));
    ASSERT_EQ(kSizeDataBuf, mTxCom->getInt8(kSizeDataBuf-1));
    ASSERT_EQ(GB_SEND_INT8, mTxCom->getSendType());


    ASSERT_EQ(GB_COM_NO, mTxCom->getFastCom());
}
