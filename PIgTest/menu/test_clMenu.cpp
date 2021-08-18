#include "gtest/gtest.h"
#include <algorithm>
#include <cstdio>
#include <functional>
#include <iostream>

using namespace std;

#define TEST_FRIENDS                            \
    friend class clMenu_Test;                   \
    FRIEND_TEST(clMenu_Test, checkLedOn);       \
    FRIEND_TEST(clMenu_Test, onFnButton);       \
    FRIEND_TEST(clMenu_Test, addControlToSend); \
    FRIEND_TEST(clMenu_Test, isRzskM);          \
    FRIEND_TEST(clMenu_Test, getKeyboardLayout);


#include "menu/menu.h"

// размер массива
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class clMenu_Test : public ::testing::Test
{
public:
    clMenu *mObj = nullptr;

    clMenu_Test() { }
    virtual ~clMenu_Test() override = default;

    bool testFnButton(eKEY key, TControl::ctrl_t rctrl) const;
    bool testCheckLedOn(TDeviceStatus &status) const;

protected:
    virtual void SetUp() override { mObj = new clMenu; }

    virtual void TearDown() override { delete mObj; }

    // вызывается однократно, до первого теста
    static void SetUpTestCase() { }

    // вызывается однократно, после последнего теста
    static void TearDownTestCase() { }
};

//
bool clMenu_Test::testFnButton(eKEY key, TControl::ctrl_t rctrl) const
{
    bool             check = true;
    TControl::ctrl_t ctrl  = mObj->onFnButton(key);

    if (rctrl != ctrl)
    {
        EXPECT_EQ(rctrl, ctrl);
        check = false;
    }

    if (key != KEY_NO)
    {
        EXPECT_EQ(KEY_NO, key);
        check = false;
    }

    return check;
}

//
bool clMenu_Test::testCheckLedOn(TDeviceStatus &status) const
{
    bool check = true;

    if (mObj->checkLedOn())
    {
        check = false;
        EXPECT_FALSE(mObj->checkLedOn());
    }

    status.setEnable(true);
    status.setWarning(0);
    status.setFault(0);
    status.setState(1);

    if (mObj->checkLedOn())
    {
        check = false;
        EXPECT_FALSE(mObj->checkLedOn());
    }

    status.setState(0);
    if (!mObj->checkLedOn())
    {
        check = false;
        EXPECT_TRUE(mObj->checkLedOn());
    }
    status.setState(2);
    if (!mObj->checkLedOn())
    {
        check = false;
        EXPECT_TRUE(mObj->checkLedOn());
    }
    status.setState(1);
    if (mObj->checkLedOn())
    {
        check = false;
        EXPECT_FALSE(mObj->checkLedOn());
    }

    status.setWarning(1);
    if (!mObj->checkLedOn())
    {
        check = false;
        EXPECT_TRUE(mObj->checkLedOn());
    }
    status.setWarning(0);
    if (mObj->checkLedOn())
    {
        check = false;
        EXPECT_FALSE(mObj->checkLedOn());
    }

    status.setFault(1);
    if (!mObj->checkLedOn())
    {
        check = false;
        EXPECT_TRUE(mObj->checkLedOn());
    }
    status.setFault(0);
    if (mObj->checkLedOn())
    {
        check = false;
        EXPECT_FALSE(mObj->checkLedOn());
    }

    if (&status == &mObj->sParam.prd.status)
    {
        mObj->sParam.prd.setIndCom8(0, 1);
        if (!mObj->checkLedOn())
        {
            check = false;
            EXPECT_TRUE(mObj->checkLedOn());
        }
        mObj->sParam.prd.setIndCom8(0, 0);
        if (mObj->checkLedOn())
        {
            check = false;
            EXPECT_FALSE(mObj->checkLedOn());
        }
    }

    if (&status == &mObj->sParam.prm.status)
    {
        mObj->sParam.prm.setIndCom8(0, 1);
        if (!mObj->checkLedOn())
        {
            check = false;
            EXPECT_TRUE(mObj->checkLedOn());
        }
        mObj->sParam.prm.setIndCom8(0, 0);
        if (mObj->checkLedOn())
        {
            check = false;
            EXPECT_FALSE(mObj->checkLedOn());
        }
    }

    status.setEnable(false);

    return check;
}

//
TEST_F(clMenu_Test, checkLedOn)
{
    ASSERT_TRUE(mObj->checkLedOn());

    // Подсветка горит если режим не "Введен"
    mObj->sParam.glb.status.setRegime(GB_REGIME_DISABLED);
    ASSERT_TRUE(mObj->checkLedOn());
    mObj->sParam.glb.status.setRegime(GB_REGIME_TEST_1);
    ASSERT_TRUE(mObj->checkLedOn());
    mObj->sParam.glb.status.setRegime(GB_REGIME_TEST_2);
    ASSERT_TRUE(mObj->checkLedOn());
    mObj->sParam.glb.status.setRegime(GB_REGIME_ENABLED);
    ASSERT_FALSE(mObj->checkLedOn());

    // Подсветка горит если есть общая неисправность или предупреждение
    mObj->sParam.glb.status.setFault(1);
    ASSERT_TRUE(mObj->checkLedOn());
    mObj->sParam.glb.status.setFault(0);
    mObj->sParam.glb.status.setWarning(1);
    ASSERT_TRUE(mObj->checkLedOn());
    mObj->sParam.glb.status.setWarning(0);

    // Подсветка горит если у устройства:
    // - состояние не "Контроль";
    // - есть неисправность;
    // - есть предупреждение;
    // - в случае команд, имееется индиакация команды на панели БСП.
    ASSERT_TRUE(testCheckLedOn(mObj->sParam.def.status));
    ASSERT_TRUE(testCheckLedOn(mObj->sParam.prm.status));
    ASSERT_TRUE(testCheckLedOn(mObj->sParam.prd.status));

    // Подсветка зажигается сигналом с МК БСП
    mObj->sParam.glb.setLedOn(true);
    ASSERT_TRUE(mObj->checkLedOn());
    ASSERT_FALSE(mObj->checkLedOn());
}

//
TEST_F(clMenu_Test, onFnButton)
{
    std::vector<eKEY> fnkeys = { KEY_CALL,     KEY_PUSK_UD, KEY_AC_PUSK_UD, KEY_PUSK_NALAD,
                                 KEY_AC_RESET, KEY_AC_PUSK, KEY_AC_REGIME,  KEY_RESET_IND,
                                 KEY_PUSK,     KEY_RESET };

    ASSERT_EQ(0, KEY_NO);
    for (int i = KEY_NO; i <= KEY_MAX; i++)
    {
        eKEY key    = static_cast<eKEY>(i);
        bool isfn   = std::find(fnkeys.begin(), fnkeys.end(), key) != fnkeys.end();
        eKEY result = isfn ? KEY_NO : key;

        SCOPED_TRACE("key " + to_string(i));

        mObj->onFnButton(key);
        ASSERT_EQ(result, key);
    }
}

//
TEST_F(clMenu_Test, onFnButton_call)
{
    eKEY key = KEY_CALL;

    ASSERT_TRUE(testFnButton(KEY_CALL, TControl::CTRL_Call));
}

//
TEST_F(clMenu_Test, onFnButton_puskUd)
{
    // ***
    eKEY key = KEY_PUSK_UD;

    mObj->sParam.def.status.setEnable(true);

    mObj->sParam.glb.setNumDevices(GB_NUM_DEVICES_2);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_RemotePusk1));
    mObj->sParam.glb.setNumDevices(GB_NUM_DEVICES_3);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_RemotePuskAll));

    mObj->sParam.def.status.setEnable(false);

    ASSERT_TRUE(testFnButton(key, TControl::CTRL_NO));
}

//
TEST_F(clMenu_Test, onFnButton_acPuskUd)
{
    eKEY key = KEY_AC_PUSK_UD;

    mObj->sParam.def.status.setEnable(true);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_RemoteAcPusk));

    mObj->sParam.def.status.setEnable(false);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_NO));
}

//
TEST_F(clMenu_Test, onFnButton_puskNalad)
{
    eKEY key = KEY_PUSK_NALAD;

    mObj->sParam.def.status.setEnable(true);

    mObj->sParam.def.status.setState(7);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_PuskAdjOff));
    mObj->sParam.def.status.setState(6);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_PuskAdjOn));

    mObj->sParam.def.status.setEnable(false);

    ASSERT_TRUE(testFnButton(key, TControl::CTRL_NO));
}

//
TEST_F(clMenu_Test, onFnButton_acReset)
{
    eKEY key = KEY_AC_RESET;

    mObj->sParam.def.status.setEnable(true);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_AcReset));

    mObj->sParam.def.status.setEnable(false);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_NO));
}

TEST_F(clMenu_Test, onFnButton_acPusk)
{
    eKEY key = KEY_AC_PUSK;

    mObj->sParam.def.status.setEnable(true);

    mObj->sParam.typeDevice = AVANT_R400M;
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_LINER);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_NO));
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_AVZK80);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_AcPusk));
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_PVZ90);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_AcPusk));
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_AVANT);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_AcPuskSelf));
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_PVZUE);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_AcPuskSelf));
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_PVZL);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_AcPuskSelf));
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_PVZK);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_AcPuskSelf));

    mObj->sParam.typeDevice = AVANT_RZSK;
    mObj->sParam.glb.setCompatibility(GB_COMP_RZSK);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_NO));
    mObj->sParam.glb.setCompatibility(GB_COMP_RZSK_M);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_AcPuskSelf));
    mObj->sParam.glb.setCompatibility(GB_COMP_RZSK_3E8);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_NO));

    mObj->sParam.def.status.setEnable(false);

    ASSERT_TRUE(testFnButton(key, TControl::CTRL_NO));
}

//
TEST_F(clMenu_Test, onFnButton_acRegime)
{
    eKEY key = KEY_AC_REGIME;

    mObj->sParam.def.status.setEnable(true);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_AcRegime));

    mObj->sParam.def.status.setEnable(false);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_NO));
}

//
TEST_F(clMenu_Test, onFnButton_resetInd)
{
    eKEY key = KEY_RESET_IND;

    mObj->sParam.def.status.setEnable(true);
    mObj->sParam.prd.status.setEnable(false);
    mObj->sParam.prm.status.setEnable(false);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_NO));

    mObj->sParam.def.status.setEnable(false);
    mObj->sParam.prd.status.setEnable(true);
    mObj->sParam.prm.status.setEnable(false);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_IndReset));

    mObj->sParam.def.status.setEnable(false);
    mObj->sParam.prd.status.setEnable(false);
    mObj->sParam.prm.status.setEnable(true);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_IndReset));

    mObj->sParam.def.status.setEnable(true);
    mObj->sParam.prd.status.setEnable(true);
    mObj->sParam.prm.status.setEnable(true);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_IndReset));
}

//
TEST_F(clMenu_Test, onFnButton_pusk)
{
    eKEY key = KEY_PUSK;

    mObj->sParam.prm.status.setEnable(true);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_PuskPrm));

    mObj->sParam.prm.status.setEnable(false);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_NO));
}

//
TEST_F(clMenu_Test, onFnButton_reset)
{
    eKEY key = KEY_RESET;

    ASSERT_TRUE(testFnButton(key, TControl::CTRL_Reset));
}

//
TEST_F(clMenu_Test, isRzskM)
{
    for (int device = 0; device <= AVANT_MAX; device++)
    {
        for (int comp = GB_COMP_RZSK_MIN; comp <= GB_COMP_RZSK_MAX; comp++)
        {
            SCOPED_TRACE("typeDevice = " + to_string(device)
                         + ", compatibility = " + to_string(comp));
            bool isRzsk = (device == AVANT_RZSK) && (comp == GB_COMP_RZSK_M);

            mObj->sParam.typeDevice = static_cast<eGB_TYPE_DEVICE>(device);
            mObj->sParam.glb.setCompatibility(comp);

            ASSERT_EQ(isRzsk, mObj->isRzskM());
        }
    }
}

//
TEST_F(clMenu_Test, getKeyboardLayout)
{
    // По умолчанию устанавливается расскладка для К400
    ASSERT_EQ(AVANT_K400, mObj->getKeyboardLayout());

    // Р400, Р400м
    mObj->sParam.typeDevice = AVANT_R400M;
    ASSERT_EQ(AVANT_R400M, mObj->getKeyboardLayout());
    mObj->sParam.typeDevice = AVANT_R400;
    ASSERT_EQ(AVANT_R400M, mObj->getKeyboardLayout());

    // К400
    mObj->sParam.typeDevice = AVANT_K400;
    ASSERT_EQ(AVANT_K400, mObj->getKeyboardLayout());

    // РЗСК
    mObj->sParam.typeDevice = AVANT_RZSK;
    mObj->sParam.glb.setCompatibility(GB_COMP_RZSK);
    ASSERT_EQ(AVANT_RZSK, mObj->getKeyboardLayout());
    mObj->sParam.glb.setCompatibility(GB_COMP_RZSK_M);
    ASSERT_EQ(AVANT_R400M, mObj->getKeyboardLayout());
    mObj->sParam.glb.setCompatibility(GB_COMP_RZSK_3E8);
    ASSERT_EQ(AVANT_K400, mObj->getKeyboardLayout());
    // Чтобы не забыть добавить тесты в случае добавления новой совместимости
    ASSERT_EQ(GB_COMP_RZSK_MAX, GB_COMP_RZSK_3E8 + 1);

    // ОПТИКА
    mObj->sParam.typeDevice = AVANT_OPTO;
    mObj->sParam.def.status.setEnable(true);
    mObj->sParam.prd.status.setEnable(false);
    mObj->sParam.prm.status.setEnable(false);
    ASSERT_EQ(AVANT_R400M, mObj->getKeyboardLayout());  // защ
    mObj->sParam.prd.status.setEnable(true);
    ASSERT_EQ(AVANT_RZSK, mObj->getKeyboardLayout());  // защ + прд
    mObj->sParam.prd.status.setEnable(false);
    mObj->sParam.prm.status.setEnable(true);
    ASSERT_EQ(AVANT_RZSK, mObj->getKeyboardLayout());  // защ + прм
    mObj->sParam.def.status.setEnable(false);
    ASSERT_EQ(AVANT_K400, mObj->getKeyboardLayout());  // прм
    mObj->sParam.prd.status.setEnable(true);
    ASSERT_EQ(AVANT_K400, mObj->getKeyboardLayout());  // прм + прд
    mObj->sParam.prm.status.setEnable(false);
    ASSERT_EQ(AVANT_K400, mObj->getKeyboardLayout());  // прд

    mObj->sParam.typeDevice = AVANT_NO;
    ASSERT_EQ(AVANT_K400, mObj->getKeyboardLayout());
    mObj->sParam.typeDevice = AVANT_MAX;
    ASSERT_EQ(AVANT_K400, mObj->getKeyboardLayout());
}

//
TEST_F(clMenu_Test, addControlToSend)
{
    mObj->addControlToSend(TControl::CTRL_Call);
    ASSERT_EQ(GB_COM_SET_CONTROL, mObj->sParam.txComBuf.getFastCom());
    ASSERT_EQ(7, mObj->sParam.txComBuf.getInt8());

    mObj->addControlToSend(TControl::CTRL_PuskPrm);
    ASSERT_EQ(GB_COM_PRM_ENTER, mObj->sParam.txComBuf.getFastCom());

    mObj->addControlToSend(TControl::CTRL_NO);
    ASSERT_EQ(GB_COM_NO, mObj->sParam.txComBuf.getFastCom());

    mObj->addControlToSend(TControl::CTRL_MAX);
    ASSERT_EQ(GB_COM_NO, mObj->sParam.txComBuf.getFastCom());
}
