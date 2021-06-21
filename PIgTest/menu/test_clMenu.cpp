#include "gtest/gtest.h"
#include <algorithm>
#include <cstdio>
#include <iostream>

using namespace std;

#define TEST_FRIENDS \
    friend class clMenu_Test; \
    FRIEND_TEST(clMenu_Test, checkLedOn); \
    FRIEND_TEST(clMenu_Test, onFnButton); \
    FRIEND_TEST(clMenu_Test, isRzskM);

#include "menu/menu.h"

// размер массива
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class clMenu_Test: public ::testing::Test
{
public:
    clMenu *mObj = nullptr;

    clMenu_Test() {}
    virtual ~clMenu_Test() override = default;

    bool testFnButton(eKEY key, eGB_COM rcom, uint8_t rbyte) const;
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
bool clMenu_Test::testFnButton(eKEY key, eGB_COM rcom, uint8_t rbyte) const
{
    bool check = true;

    EXPECT_EQ(KEY_NO, mObj->onFnButton(key));

    eGB_COM com = mObj->sParam.txComBuf.getFastCom();
    if (rcom != com) {
        EXPECT_EQ(rcom, com);
        check = false;
    }

    if (rcom != GB_COM_NO) {
        uint8_t byte = mObj->sParam.txComBuf.getInt8();
        if (byte != rbyte) {
            EXPECT_EQ(rbyte, byte);
            check = false;
        }
    }

    return check;
}

//
bool clMenu_Test::testCheckLedOn(TDeviceStatus &status) const
{
    bool check = true;

    if (mObj->checkLedOn()) {check = false; EXPECT_FALSE(mObj->checkLedOn());}

    status.setEnable(true);
    status.setWarning(0);
    status.setFault(0);
    status.setState(1);

    if (mObj->checkLedOn()) {check = false; EXPECT_FALSE(mObj->checkLedOn());}

    status.setState(0);
    if (!mObj->checkLedOn()) {check = false; EXPECT_TRUE(mObj->checkLedOn());}
    status.setState(2);
    if (!mObj->checkLedOn()) {check = false; EXPECT_TRUE(mObj->checkLedOn());}
    status.setState(1);
    if (mObj->checkLedOn()) {check = false; EXPECT_FALSE(mObj->checkLedOn());}

    status.setWarning(1);
    if (!mObj->checkLedOn()) {check = false; EXPECT_TRUE(mObj->checkLedOn());}
    status.setWarning(0);
    if (mObj->checkLedOn()) {check = false; EXPECT_FALSE(mObj->checkLedOn());}

    status.setFault(1);
    if (!mObj->checkLedOn()) {check = false; EXPECT_TRUE(mObj->checkLedOn());}
    status.setFault(0);
    if (mObj->checkLedOn()) {check = false; EXPECT_FALSE(mObj->checkLedOn());}

    if (&status == &mObj->sParam.prd.status) {
        mObj->sParam.prd.setIndCom8(0, 1);
        if (!mObj->checkLedOn()) {check = false; EXPECT_TRUE(mObj->checkLedOn());}
        mObj->sParam.prd.setIndCom8(0, 0);
        if (mObj->checkLedOn()) {check = false; EXPECT_FALSE(mObj->checkLedOn());}
    }

    if (&status == &mObj->sParam.prm.status) {
        mObj->sParam.prm.setIndCom8(0, 1);
        if (!mObj->checkLedOn()) {check = false; EXPECT_TRUE(mObj->checkLedOn());}
        mObj->sParam.prm.setIndCom8(0, 0);
        if (mObj->checkLedOn()) {check = false; EXPECT_FALSE(mObj->checkLedOn());}
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
    std::vector<eKEY> fnkeys = {
        KEY_CALL,
        KEY_PUSK_UD,
        KEY_AC_PUSK_UD,
        KEY_PUSK_NALAD,
        KEY_AC_RESET,
        KEY_AC_PUSK,
        KEY_AC_REGIME,
        KEY_RESET_IND,
        KEY_PUSK,
        KEY_RESET
    };

    ASSERT_EQ(0, KEY_NO);
    for(int i = KEY_NO; i <= KEY_MAX; i++) {
        eKEY key = static_cast<eKEY> (i);
        bool isfn = std::find(fnkeys.begin(), fnkeys.end(), key) != fnkeys.end();
        eKEY result = isfn ? KEY_NO : key;

        SCOPED_TRACE("key " + to_string(i));

        ASSERT_EQ(result, mObj->onFnButton(key));
    }
}

//
TEST_F(clMenu_Test, onFnButton_call)
{
    eKEY key = KEY_CALL;

    ASSERT_TRUE(testFnButton(KEY_CALL, GB_COM_SET_CONTROL, GB_CONTROL_CALL));
}

//
TEST_F(clMenu_Test, onFnButton_puskUd)
{
    // ***
    eKEY key = KEY_PUSK_UD;

    mObj->sParam.def.status.setEnable(true);

    mObj->sParam.glb.setNumDevices(GB_NUM_DEVICES_2);
    ASSERT_TRUE(testFnButton(key, GB_COM_SET_CONTROL, GB_CONTROL_PUSK_UD_1));
    mObj->sParam.glb.setNumDevices(GB_NUM_DEVICES_3);
    ASSERT_TRUE(testFnButton(key, GB_COM_SET_CONTROL, GB_CONTROL_PUSK_UD_ALL));

    mObj->sParam.def.status.setEnable(false);

    ASSERT_TRUE(testFnButton(key, GB_COM_NO, 0));
}

//
TEST_F(clMenu_Test, onFnButton_acPuskUd)
{
    eKEY key = KEY_AC_PUSK_UD;

    mObj->sParam.def.status.setEnable(true);
    ASSERT_TRUE(testFnButton(key, GB_COM_SET_CONTROL, GB_CONTROL_PUSK_AC_UD));

    mObj->sParam.def.status.setEnable(false);
    ASSERT_TRUE(testFnButton(key, GB_COM_NO, 0));
}

//
TEST_F(clMenu_Test, onFnButton_puskNalad)
{
    eKEY key = KEY_PUSK_NALAD;

    mObj->sParam.def.status.setEnable(true);

    mObj->sParam.def.status.setState(7);
    ASSERT_TRUE(testFnButton(key, GB_COM_SET_CONTROL, GB_CONTROL_PUSK_OFF));
    mObj->sParam.def.status.setState(6);
    ASSERT_TRUE(testFnButton(key, GB_COM_SET_CONTROL, GB_CONTROL_PUSK_ON));

    mObj->sParam.def.status.setEnable(false);

    ASSERT_TRUE(testFnButton(key, GB_COM_NO, 0));
}

//
TEST_F(clMenu_Test, onFnButton_acReset)
{
    eKEY key = KEY_AC_RESET;

    mObj->sParam.def.status.setEnable(true);
    ASSERT_TRUE(testFnButton(key, GB_COM_SET_CONTROL, GB_CONTROL_RESET_AC));

    mObj->sParam.def.status.setEnable(false);
    ASSERT_TRUE(testFnButton(key, GB_COM_NO, 0));
}

TEST_F(clMenu_Test, onFnButton_acPusk)
{
    eKEY key = KEY_AC_PUSK;

    mObj->sParam.def.status.setEnable(true);

    mObj->sParam.typeDevice = AVANT_R400M;
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_LINER);
    ASSERT_TRUE(testFnButton(key, GB_COM_NO, 0));
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_AVZK80);
    ASSERT_TRUE(testFnButton(key, GB_COM_DEF_SET_TYPE_AC, GB_TYPE_AC_PUSK));
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_PVZ90);
    ASSERT_TRUE(testFnButton(key, GB_COM_DEF_SET_TYPE_AC, GB_TYPE_AC_PUSK));
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_AVANT);
    ASSERT_TRUE(testFnButton(key, GB_COM_DEF_SET_TYPE_AC, GB_TYPE_AC_PUSK_SELF));
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_PVZUE);
    ASSERT_TRUE(testFnButton(key, GB_COM_DEF_SET_TYPE_AC, GB_TYPE_AC_PUSK_SELF));
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_PVZL);
    ASSERT_TRUE(testFnButton(key, GB_COM_DEF_SET_TYPE_AC, GB_TYPE_AC_PUSK_SELF));
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_PVZK);
    ASSERT_TRUE(testFnButton(key, GB_COM_DEF_SET_TYPE_AC, GB_TYPE_AC_PUSK_SELF));

    mObj->sParam.typeDevice = AVANT_RZSK;
    mObj->sParam.glb.setCompatibility(GB_COMP_RZSK);
    ASSERT_TRUE(testFnButton(key, GB_COM_NO, 0));
    mObj->sParam.glb.setCompatibility(GB_COMP_RZSK_M);
    ASSERT_TRUE(testFnButton(key, GB_COM_DEF_SET_TYPE_AC, GB_TYPE_AC_PUSK_SELF));
    mObj->sParam.glb.setCompatibility(GB_COMP_RZSK_3E8);
    ASSERT_TRUE(testFnButton(key, GB_COM_NO, 0));

    mObj->sParam.def.status.setEnable(false);

    ASSERT_TRUE(testFnButton(key, GB_COM_NO, 0));
}

//
TEST_F(clMenu_Test, onFnButton_acRegime)
{
    eKEY key = KEY_AC_REGIME;

    mObj->sParam.def.status.setEnable(true);
    ASSERT_TRUE(testFnButton(key, GB_COM_SET_CONTROL, GB_CONTROL_REG_AC));

    mObj->sParam.def.status.setEnable(false);
    ASSERT_TRUE(testFnButton(key, GB_COM_NO, 0));
}

//
TEST_F(clMenu_Test, onFnButton_resetInd)
{
    eKEY key = KEY_RESET_IND;

    mObj->sParam.def.status.setEnable(true);
    mObj->sParam.prd.status.setEnable(false);
    mObj->sParam.prm.status.setEnable(false);
    ASSERT_TRUE(testFnButton(key, GB_COM_NO, 0));

    mObj->sParam.def.status.setEnable(false);
    mObj->sParam.prd.status.setEnable(true);
    mObj->sParam.prm.status.setEnable(false);
    ASSERT_TRUE(testFnButton(key, GB_COM_PRM_RES_IND, 0));

    mObj->sParam.def.status.setEnable(false);
    mObj->sParam.prd.status.setEnable(false);
    mObj->sParam.prm.status.setEnable(true);
    ASSERT_TRUE(testFnButton(key, GB_COM_PRM_RES_IND, 0));

    mObj->sParam.def.status.setEnable(true);
    mObj->sParam.prd.status.setEnable(true);
    mObj->sParam.prm.status.setEnable(true);
    ASSERT_TRUE(testFnButton(key, GB_COM_PRM_RES_IND, 0));
}

//
TEST_F(clMenu_Test, onFnButton_pusk)
{
    eKEY key = KEY_PUSK;

    mObj->sParam.prm.status.setEnable(true);
    ASSERT_TRUE(testFnButton(key, GB_COM_PRM_ENTER, 0));

    mObj->sParam.prm.status.setEnable(false);
    ASSERT_TRUE(testFnButton(key, GB_COM_NO, 0));
}

//
TEST_F(clMenu_Test, onFnButton_reset)
{
    eKEY key = KEY_RESET;

    ASSERT_TRUE(testFnButton(key, GB_COM_SET_CONTROL, GB_CONTROL_RESET_SELF));
}

//
TEST_F(clMenu_Test, isRzskM)
{
    for(int device = 0; device <= AVANT_MAX; device++) {
        for(int comp = GB_COMP_RZSK_MIN; comp <= GB_COMP_RZSK_MAX; comp++) {
            SCOPED_TRACE("typeDevice = " + to_string(device) +
                         ", compatibility = " + to_string(comp));
            bool isRzsk = (device == AVANT_RZSK) && (comp == GB_COMP_RZSK_M);

            mObj->sParam.typeDevice = static_cast<eGB_TYPE_DEVICE> (device);
            mObj->sParam.glb.setCompatibility(comp);

            ASSERT_EQ(isRzsk, mObj->isRzskM());
        }
    }
}
