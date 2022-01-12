#include "gtest/gtest.h"
#include <cstdio>
#include <iostream>

using namespace std;

#include "PIg/src/menu/control.h"
#include "PIg/src/paramDef.h"

// размер массива
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class TControl_Test : public ::testing::Test
{
public:
    const uint8_t kSizeBufComFast = 3;
    const uint8_t kSizeBufCom1    = 10;
    const uint8_t kSizeBufCom2    = 4;
    const uint8_t kSizeDataBuf    = 16;


    TControl *     mObj = nullptr;
    const uint8_t *mBuf = nullptr;

    TControl_Test() { }
    virtual ~TControl_Test() override = default;

protected:
    virtual void SetUp() override { mObj = new TControl; }

    virtual void TearDown() override
    {
        delete mObj;
        mObj = nullptr;
    }
};

//
TEST_F(TControl_Test, common)
{
    struct testData_t
    {
        string           testName;  ///< Название теста.
        TControl::ctrl_t ctrl;      ///< Сигнал управления.
        string           rtext;     ///< Название синала.
        eGB_COM          rcom;      ///< Команда протокола АВАНТ.
        bool             rhasbyte;  ///< Наличие байта данных в команде.
        uint8_t          rbyte;     ///< Байт данных.
    };

    ASSERT_EQ(0x72, GB_COM_SET_CONTROL);
    ASSERT_EQ(0x8A, GB_COM_DEF_SET_TYPE_AC);

    vector<testData_t> testData = {
        { "data00", TControl::CTRL_Call, "Вызов", GB_COM_SET_CONTROL, true, 7 },
        { "data01", TControl::CTRL_AcPusk, "Пуск АК", GB_COM_DEF_SET_TYPE_AC, true, 7 },
        { "data02", TControl::CTRL_AcPuskSelf, "Пуск АК свой", GB_COM_DEF_SET_TYPE_AC, true, 6 },
        { "data03", TControl::CTRL_AcRegime, "Режим АК", GB_COM_SET_CONTROL, true, 14 },
        { "data04", TControl::CTRL_AcReset, "Сброс АК", GB_COM_SET_CONTROL, true, 3 },
        { "data05", TControl::CTRL_IndReset, "Сброс индикации", GB_COM_PRM_RES_IND, false, 0 },
        { "data06", TControl::CTRL_PuskAdjOn, "Пуск своего вкл.", GB_COM_SET_CONTROL, true, 8 },
        { "data07", TControl::CTRL_PuskAdjOff, "Пуск своего выкл.", GB_COM_SET_CONTROL, true, 9 },
        { "data08", TControl::CTRL_PuskPrm, "Пуск приемника", GB_COM_PRM_ENTER, false, 0 },
        { "data09",
          TControl::CTRL_RemoteAcPusk,
          "Пуск АК удаленного",
          GB_COM_SET_CONTROL,
          true,
          10 },
        { "data10", TControl::CTRL_RemotePusk1, "Пуск удаленного 1", GB_COM_SET_CONTROL, true, 4 },
        { "data11", TControl::CTRL_RemotePuskAll, "Пуск удаленных", GB_COM_SET_CONTROL, true, 6 },
        { "data12", TControl::CTRL_Reset, "Сброс", GB_COM_SET_CONTROL, true, 1 }
    };

    for (size_t i = 0; i < testData.size(); i++)
    {
        testData_t &testdata = testData.at(i);
        SCOPED_TRACE(testdata.testName);

        eGB_COM com     = GB_COM_NO;
        bool    hasbyte = false;
        uint8_t byte    = 0xFF;

        ASSERT_TRUE(mObj->getData(testdata.ctrl, com, hasbyte, byte));
        ASSERT_EQ(testdata.rcom, com);
        ASSERT_EQ(testdata.rhasbyte, hasbyte);
        ASSERT_EQ(testdata.rbyte, byte);

        ASSERT_STREQ(testdata.rtext.c_str(), mObj->getText(testdata.ctrl));
    }

    // Ошибочные значения

    testData = {
        { "data00", TControl::CTRL_NO, "", GB_COM_JRN_CLR, false, 0xFF },
        { "data01", TControl::CTRL_MAX, "", GB_COM_JRN_CLR, false, 0xFF },
        { "data02", TControl::ctrl_t(TControl::CTRL_MAX + 1), "", GB_COM_JRN_CLR, false, 0xFF }
    };

    for (size_t i = 0; i < testData.size(); i++)
    {
        testData_t &testdata = testData.at(i);
        SCOPED_TRACE(testdata.testName);

        eGB_COM com     = GB_COM_JRN_CLR;
        bool    hasbyte = false;
        uint8_t byte    = 0xFF;

        ASSERT_FALSE(mObj->getData(TControl::CTRL_NO, com, hasbyte, byte));
        ASSERT_EQ(testdata.rcom, com);
        ASSERT_EQ(testdata.rhasbyte, hasbyte);
        ASSERT_EQ(testdata.rbyte, byte);

        ASSERT_STREQ(testdata.rtext.c_str(), mObj->getText(testdata.ctrl));
    }
}
