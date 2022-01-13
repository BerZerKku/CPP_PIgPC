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
        uint8_t          rcom;      ///< Команда протокола АВАНТ.
        bool             rhasbyte;  ///< Наличие байта данных в команде.
        uint8_t          rbyte;     ///< Байт данных.
    };

    ASSERT_EQ(0x9A, GB_COM_PRM_RES_IND);
    ASSERT_EQ(0x51, GB_COM_PRM_ENTER);
    ASSERT_EQ(0x72, GB_COM_SET_CONTROL);
    ASSERT_EQ(0x8A, GB_COM_DEF_SET_TYPE_AC);

    vector<testData_t> testData = {
        { "Call", TControl::CTRL_Call, "Вызов", 0x72, true, 7 },
        { "AcAuto", TControl::CTRL_AcAuto, "АК автоматическ.", 0x8A, true, 1 },
        { "AcAccelerated", TControl::CTRL_AcAccelerated, "АК ускоренный", 0x8A, true, 3 },
        { "AcNormal", TControl::CTRL_AcNormal, "АК нормальный", 0x8A, true, 1 },
        { "AcOff", TControl::CTRL_AcOff, "АК выключен", 0x8A, true, 4 },
        { "AcOn", TControl::CTRL_AcOn, "АК включен", 0x8A, true, 1 },
        { "AcPusk", TControl::CTRL_AcPusk, "АК пуск", 0x8A, true, 7 },
        { "AcPuskSelf", TControl::CTRL_AcPuskSelf, "Пуск АК свой", 0x8A, true, 6 },
        { "AcQuick", TControl::CTRL_AcQuick, "АК беглый", 0x8A, true, 5 },
        { "AcRegime", TControl::CTRL_AcRegime, "Режим АК", 0x72, true, 14 },
        { "AcReset", TControl::CTRL_AcReset, "Сброс АК", 0x72, true, 3 },
        { "AcTest", TControl::CTRL_AcTest, "АК испытания", 0x8A, true, 6 },
        { "CtrlCheck", TControl::CTRL_CtrlCheck, "АК контр.провер.", 0x8A, true, 6 },
        { "IndReset", TControl::CTRL_IndReset, "Сброс индикации", 0x9A, false, 0 },
        { "PuskAdjOn", TControl::CTRL_PuskAdjOn, "Пуск налад. вкл.", 0x72, true, 8 },
        { "PuskAdjOff", TControl::CTRL_PuskAdjOff, "Пуск налад. выкл", 0x72, true, 9 },
        { "PuskPrd", TControl::CTRL_PuskPrd, "Пуск ПРД", 0x72, true, 4 },
        { "PuskPrm", TControl::CTRL_PuskPrm, "Пуск приемника", 0x51, false, 0 },
        { "RemoteAcPusk", TControl::CTRL_RemoteAcPusk, "Пуск АК удаленн.", 0x72, true, 10 },
        { "RemoteMan", TControl::CTRL_RemoteMan, "Пуск удален. МАН", 0x72, true, 16 },
        { "RemoteMan1", TControl::CTRL_RemoteMan1, "Пуск удал. МАН 1", 0x72, true, 16 },
        { "RemoteMan2", TControl::CTRL_RemoteMan2, "Пуск удал. МАН 2", 0x72, true, 17 },
        { "RemoteMan3", TControl::CTRL_RemoteMan3, "Пуск удал. МАН 3", 0x72, true, 18 },
        { "RemoteManAll", TControl::CTRL_RemoteManAll, "Пуск удал-ых МАН", 0x72, true, 19 },
        { "RemotePusk", TControl::CTRL_RemotePusk, "Пуск удаленного", 0x72, true, 4 },
        { "RemotePusk1", TControl::CTRL_RemotePusk1, "Пуск удаленн. 1", 0x72, true, 4 },
        { "RemotePusk2", TControl::CTRL_RemotePusk2, "Пуск удаленн. 2", 0x72, true, 5 },
        { "RemotePusk3", TControl::CTRL_RemotePusk3, "Пуск удаленн. 3", 0x72, true, 13 },
        { "RemotePuskAll", TControl::CTRL_RemotePuskAll, "Пуск удаленных", 0x72, true, 6 },
        { "RemoteReset", TControl::CTRL_RemoteReset, "Сброс удаленного", 0x72, true, 2 },
        { "RemoteReset1", TControl::CTRL_RemoteReset1, "Сброс удален. 1", 0x72, true, 11 },
        { "RemoteReset2", TControl::CTRL_RemoteReset2, "Сброс удален. 2", 0x72, true, 12 },
        { "RemoteReset3", TControl::CTRL_RemoteReset3, "Сброс удален. 3", 0x72, true, 15 },
        { "RemoteResetAll", TControl::CTRL_RemoteResetAll, "Сброс удаленных", 0x72, true, 2 },
        { "Reset", TControl::CTRL_Reset, "Сброс своего", 0x72, true, 1 },
        { "ResetAll", TControl::CTRL_ResetAll, "Сброс всех", 0x72, true, 2 },
        { "SingleOff", TControl::CTRL_SingleOff, "Одност.реж. выкл", 0x8A, true, 0 },
        { "SingleOn", TControl::CTRL_SingleOn, "Одност.реж. вкл", 0x8A, true, 1 }
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
