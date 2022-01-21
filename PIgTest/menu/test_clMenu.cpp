#include "gtest/gtest.h"
#include <algorithm>
#include <cstdio>
#include <functional>
#include <iostream>

using namespace std;

#define TEST_FRIENDS                                \
    friend class clMenu_Test;                       \
    FRIEND_TEST(clMenu_Test, checkLedOn);           \
    FRIEND_TEST(clMenu_Test, onFnButton);           \
    FRIEND_TEST(clMenu_Test, AddControlToSend);     \
    FRIEND_TEST(clMenu_Test, AddControlToPunkts);   \
    FRIEND_TEST(clMenu_Test, ChangeCotnrolPunkt);   \
    FRIEND_TEST(clMenu_Test, ChangeControlPunkts);  \
    FRIEND_TEST(clMenu_Test, isRzskM);              \
    FRIEND_TEST(clMenu_Test, getKeyboardLayout);    \
    FRIEND_TEST(clMenu_Test, fillLvlControl_K400);  \
    FRIEND_TEST(clMenu_Test, fillLvlControl_Opto);  \
    FRIEND_TEST(clMenu_Test, fillLvlControl_Rzsk);  \
    FRIEND_TEST(clMenu_Test, fillLvlControl_R400m); \
    FRIEND_TEST(clMenu_Test, fillLvlSetupGlb_K400); \
    FRIEND_TEST(clMenu_Test, fillLvlSetupGlb_Opto); \
    FRIEND_TEST(clMenu_Test, fillLvlSetupGlb_Rzsk); \
    FRIEND_TEST(clMenu_Test, fillLvlSetupGlb_R400m);

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
    bool checkLocalParam(eGB_PARAM param, bool read_only = false);

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
bool clMenu_Test::checkLocalParam(eGB_PARAM param, bool read_only)
{
    bool check = true;

    if (param != mObj->sParam.local.getParam())
    {
        check = false;
        EXPECT_EQ(param, mObj->sParam.local.getParam());
    }

    if (read_only != mObj->sParam.local.isReadOnly())
    {
        check = false;
        EXPECT_EQ(read_only, mObj->sParam.local.isReadOnly());
    }

    mObj->sParam.local.nextParam();

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

    ASSERT_TRUE(testFnButton(key, TControl::CTRL_Call));
}

//
TEST_F(clMenu_Test, onFnButton_puskUd)
{
    // ***
    eKEY key = KEY_PUSK_UD;

    mObj->sParam.def.status.setEnable(true);

    mObj->sParam.glb.setNumDevices(GB_NUM_DEVICES_2);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_RemotePusk));
    mObj->sParam.glb.setNumDevices(GB_NUM_DEVICES_3);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_RemotePuskAll));

    mObj->sParam.def.status.setEnable(false);

    ASSERT_TRUE(testFnButton(key, TControl::CTRL_NO));
}

//
TEST_F(clMenu_Test, onFnButton_acPuskUd)
{
    eKEY key = KEY_AC_PUSK_UD;

    mObj->sParam.typeDevice = AVANT_R400M;

    mObj->sParam.def.status.setEnable(true);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_RemoteAcPusk));
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_R400);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_NO));

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
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_PVZ);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_AcPuskSelf));
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_PVZU);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_AcPuskSelf));
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_R400);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_NO));

    mObj->sParam.typeDevice = AVANT_RZSK;
    mObj->sParam.glb.setCompatibility(GB_COMP_RZSK);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_NO));
    mObj->sParam.glb.setCompatibility(GB_COMP_RZSK_M);
    ASSERT_TRUE(testFnButton(key, TControl::CTRL_NO));
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
    ASSERT_EQ(AVANT_RZSK, mObj->getKeyboardLayout());
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
TEST_F(clMenu_Test, AddControlToSend)
{
    mObj->AddControlToSend(TControl::CTRL_Call);
    ASSERT_EQ(GB_COM_SET_CONTROL, mObj->sParam.txComBuf.getFastCom());
    ASSERT_EQ(7, mObj->sParam.txComBuf.getInt8());

    mObj->AddControlToSend(TControl::CTRL_PuskPrm);
    ASSERT_EQ(GB_COM_PRM_ENTER, mObj->sParam.txComBuf.getFastCom());

    mObj->AddControlToSend(TControl::CTRL_NO);
    ASSERT_EQ(GB_COM_NO, mObj->sParam.txComBuf.getFastCom());

    mObj->AddControlToSend(TControl::CTRL_MAX);
    ASSERT_EQ(GB_COM_NO, mObj->sParam.txComBuf.getFastCom());
}

//
TEST_F(clMenu_Test, AddControlToPunkts)
{
    // Заполнение пунктов меню. Количество пунктов превышает допустимый размер.

    for (uint8_t i = 0; i < mObj->Punkts_.GetSize() + 2; i++)
    {
        TControl::ctrl_t ctrl = static_cast<TControl::ctrl_t>(i);
        mObj->AddControlToPunkts(ctrl);
    }

    // Проверка пунктов.

    ASSERT_EQ(mObj->Punkts_.GetSize(), mObj->Punkts_.GetLen());

    for (uint8_t i = 0; i < mObj->Punkts_.GetLen(); i++)
    {
        TControl::ctrl_t ctrl = static_cast<TControl::ctrl_t>(i);
        ASSERT_EQ(ctrl, mObj->Punkts_.GetData(i));
        ASSERT_STREQ(mObj->mControl.getText(ctrl), mObj->Punkts_.GetName(i));
    }
}


//
TEST_F(clMenu_Test, ChangeCotnrolPunkt)
{
    TControl::ctrl_t ctrl;

    // Заполнение пунктов меню.

    for (uint8_t i = 0; i < mObj->Punkts_.GetSize(); i++)
    {
        ctrl = static_cast<TControl::ctrl_t>(i);
        mObj->AddControlToPunkts(ctrl);
    }

    // Замена

    ctrl = TControl::CTRL_Reset;
    ASSERT_NE(ctrl, mObj->Punkts_.GetData(3));
    mObj->ChangeCotnrolPunkt(3, ctrl);

    // Проверка

    ASSERT_EQ(ctrl, mObj->Punkts_.GetData(3));
    ASSERT_STREQ(mObj->mControl.getText(ctrl), mObj->Punkts_.GetName(3));
}


//
TEST_F(clMenu_Test, ChangeControlPunkts)
{
    TControl::ctrl_t ctrl;

    // Список команд управления зависящих от состояния аппарата
    QVector<TControl::ctrl_t> ctrl_list = { TControl::CTRL_PuskAdjOn,     //
                                            TControl::CTRL_RemoteMan2,    //
                                            TControl::CTRL_RemoteMan3,    //
                                            TControl::CTRL_RemotePusk2,   //
                                            TControl::CTRL_RemotePusk3,   //
                                            TControl::CTRL_RemoteReset2,  //
                                            TControl::CTRL_RemoteReset3,  //
                                            TControl::CTRL_SingleOn };

    // Заполнение списка
    mObj->Punkts_.Clear();
    for (int i = 0; i < ctrl_list.size(); i++)
    {
        mObj->AddControlToPunkts(ctrl_list.at(i));
    }

    // 3-х концевая линия, для установки номера аппарата 3
    mObj->sParam.glb.setNumDevices(GB_NUM_DEVICES_3);

    // защита есть, состояние не "Нал.пуск", номер аппарата 1, одност. режим 0
    mObj->sParam.typeDevice = AVANT_R400;
    mObj->sParam.def.status.setEnable(true);
    mObj->sParam.def.status.setState(0);
    mObj->sParam.glb.setDeviceNum(1);
    mObj->sParam.local.setValue(0);

    mObj->ChangeControlPunkts();

    for (uint8_t i = 0; i < static_cast<uint8_t>(ctrl_list.size()); i++)
    {
        ctrl = ctrl_list.at(i);
        SCOPED_TRACE("ctrl[" + to_string(i) + "] = " + to_string(ctrl));

        ASSERT_EQ(ctrl_list.at(i), mObj->Punkts_.GetData(i));
        ASSERT_STREQ(mObj->mControl.getText(ctrl_list.at(i)), mObj->Punkts_.GetName(i));
    }

    // защита есть, состояние не "Нал.пуск", номер аппарата 3, одност. режим 0
    // в командах управления содержащих номер аппарата "2" меняется на "1"
    // в командах управления содержащих номер аппарата "3" меняется на "2"
    mObj->sParam.glb.setDeviceNum(3);
    ctrl_list.replace(ctrl_list.indexOf(TControl::CTRL_RemoteMan2), TControl::CTRL_RemoteMan1);
    ctrl_list.replace(ctrl_list.indexOf(TControl::CTRL_RemoteMan3), TControl::CTRL_RemoteMan2);
    ctrl_list.replace(ctrl_list.indexOf(TControl::CTRL_RemotePusk2), TControl::CTRL_RemotePusk1);
    ctrl_list.replace(ctrl_list.indexOf(TControl::CTRL_RemotePusk3), TControl::CTRL_RemotePusk2);
    ctrl_list.replace(ctrl_list.indexOf(TControl::CTRL_RemoteReset2), TControl::CTRL_RemoteReset1);
    ctrl_list.replace(ctrl_list.indexOf(TControl::CTRL_RemoteReset3), TControl::CTRL_RemoteReset2);

    mObj->ChangeControlPunkts();

    for (uint8_t i = 0; i < static_cast<uint8_t>(ctrl_list.size()); i++)
    {
        ctrl = ctrl_list.at(i);
        SCOPED_TRACE("ctrl[" + to_string(i) + "] = " + to_string(ctrl));

        ASSERT_EQ(ctrl_list.at(i), mObj->Punkts_.GetData(i));
        ASSERT_STREQ(mObj->mControl.getText(ctrl_list.at(i)), mObj->Punkts_.GetName(i));
    }

    // защита есть, состояние не "Нал.пуск", номер аппарата 2, одност. режим 0
    // в командах управления содержащих номер аппарата "2" меняется на "3"
    mObj->sParam.glb.setDeviceNum(2);
    ctrl_list.replace(ctrl_list.indexOf(TControl::CTRL_RemoteMan2), TControl::CTRL_RemoteMan3);
    ctrl_list.replace(ctrl_list.indexOf(TControl::CTRL_RemotePusk2), TControl::CTRL_RemotePusk3);
    ctrl_list.replace(ctrl_list.indexOf(TControl::CTRL_RemoteReset2), TControl::CTRL_RemoteReset3);

    mObj->ChangeControlPunkts();

    for (uint8_t i = 0; i < static_cast<uint8_t>(ctrl_list.size()); i++)
    {
        ctrl = ctrl_list.at(i);
        SCOPED_TRACE("ctrl[" + to_string(i) + "] = " + to_string(ctrl));

        ASSERT_EQ(ctrl_list.at(i), mObj->Punkts_.GetData(i));
        ASSERT_STREQ(mObj->mControl.getText(ctrl_list.at(i)), mObj->Punkts_.GetName(i));
    }

    // защита есть, состояние "Нал.пуск", номер аппарата 2, одност. режим 0
    // наладочный пуск меняется с "вкл." на "выкл."
    mObj->sParam.def.status.setState(7);
    ctrl_list.replace(ctrl_list.indexOf(TControl::CTRL_PuskAdjOn), TControl::CTRL_PuskAdjOff);

    mObj->ChangeControlPunkts();

    for (uint8_t i = 0; i < static_cast<uint8_t>(ctrl_list.size()); i++)
    {
        ctrl = ctrl_list.at(i);
        SCOPED_TRACE("ctrl[" + to_string(i) + "] = " + to_string(ctrl));

        ASSERT_EQ(ctrl_list.at(i), mObj->Punkts_.GetData(i));
        ASSERT_STREQ(mObj->mControl.getText(ctrl_list.at(i)), mObj->Punkts_.GetName(i));
    }

    // защита есть, состояние "Нал.пуск", номер аппарата 2, одност. режим 0
    // наладочный пуск меняется с "выкл." на "вкл."
    mObj->sParam.def.status.setState(6);
    ctrl_list.replace(ctrl_list.indexOf(TControl::CTRL_PuskAdjOff), TControl::CTRL_PuskAdjOn);

    mObj->ChangeControlPunkts();

    for (uint8_t i = 0; i < static_cast<uint8_t>(ctrl_list.size()); i++)
    {
        ctrl = ctrl_list.at(i);
        SCOPED_TRACE("ctrl[" + to_string(i) + "] = " + to_string(ctrl));

        ASSERT_EQ(ctrl_list.at(i), mObj->Punkts_.GetData(i));
        ASSERT_STREQ(mObj->mControl.getText(ctrl_list.at(i)), mObj->Punkts_.GetName(i));
    }

    // защита есть, состояние "Нал.пуск", номер аппарата 2, одност. режим 1
    // односторонний режим меняестся с "вкл." на "выкл."
    mObj->sParam.local.setValue(1);
    ctrl_list.replace(ctrl_list.indexOf(TControl::CTRL_SingleOn), TControl::CTRL_SingleOff);

    mObj->ChangeControlPunkts();

    for (uint8_t i = 0; i < static_cast<uint8_t>(ctrl_list.size()); i++)
    {
        ctrl = ctrl_list.at(i);
        SCOPED_TRACE("ctrl[" + to_string(i) + "] = " + to_string(ctrl));

        ASSERT_EQ(ctrl_list.at(i), mObj->Punkts_.GetData(i));
        ASSERT_STREQ(mObj->mControl.getText(ctrl_list.at(i)), mObj->Punkts_.GetName(i));
    }

    // защита есть, состояние "Нал.пуск", номер аппарата 2, одност. режим 0
    // односторонний режим меняестся с "выкл." на "вкл."
    mObj->sParam.local.setValue(0);
    ctrl_list.replace(ctrl_list.indexOf(TControl::CTRL_SingleOff), TControl::CTRL_SingleOn);

    mObj->ChangeControlPunkts();

    for (uint8_t i = 0; i < static_cast<uint8_t>(ctrl_list.size()); i++)
    {
        ctrl = ctrl_list.at(i);
        SCOPED_TRACE("ctrl[" + to_string(i) + "] = " + to_string(ctrl));

        ASSERT_EQ(ctrl_list.at(i), mObj->Punkts_.GetData(i));
        ASSERT_STREQ(mObj->mControl.getText(ctrl_list.at(i)), mObj->Punkts_.GetName(i));
    }

    // защиты нет, состояние не "Нал.пуск", номер аппарата 1, одност. режим 1
    // т.к. защиты нет то проверка на замену не производится
    mObj->sParam.def.status.setEnable(false);
    mObj->sParam.glb.setDeviceNum(1);
    mObj->sParam.def.status.setState(0);
    mObj->sParam.local.setValue(1);

    mObj->ChangeControlPunkts();

    for (uint8_t i = 0; i < static_cast<uint8_t>(ctrl_list.size()); i++)
    {
        ctrl = ctrl_list.at(i);
        SCOPED_TRACE("ctrl[" + to_string(i) + "] = " + to_string(ctrl));

        ASSERT_EQ(ctrl_list.at(i), mObj->Punkts_.GetData(i));
        ASSERT_STREQ(mObj->mControl.getText(ctrl_list.at(i)), mObj->Punkts_.GetName(i));
    }
}

//
TEST_F(clMenu_Test, fillLvlControl_K400)
{
    uint8_t counter;

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_K400));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_IndReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());
}

//
TEST_F(clMenu_Test, fillLvlControl_Opto)
{
    uint8_t counter;

    mObj->sParam.glb.setTypeLine(GB_TYPE_LINE_OPTO);  // требуется для установки типа оптики

    // защита + приемник + передатчик, стандартная оптика
    mObj->sParam.def.status.setEnable(true);
    mObj->sParam.prm.status.setEnable(true);
    mObj->sParam.prd.status.setEnable(true);
    mObj->sParam.glb.setTypeOpto(TYPE_OPTO_STANDART);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_OPTO));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_PuskAdjOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_IndReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // приемник, стандратная оптика
    mObj->sParam.def.status.setEnable(false);
    mObj->sParam.prd.status.setEnable(false);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_OPTO));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_IndReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // передатчик, кольцо
    mObj->sParam.prm.status.setEnable(false);
    mObj->sParam.prd.status.setEnable(true);
    mObj->sParam.glb.setTypeOpto(TYPE_OPTO_RING_BI);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_OPTO));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_ResetAll, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_IndReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // защита, стандратная оптика
    mObj->sParam.prd.status.setEnable(false);
    mObj->sParam.def.status.setEnable(true);
    mObj->sParam.glb.setTypeOpto(TYPE_OPTO_STANDART);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_OPTO));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_PuskAdjOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());
}

//
TEST_F(clMenu_Test, fillLvlControl_Rzsk)
{
    uint8_t counter;
    mObj->sParam.typeDevice = AVANT_RZSK;  // для совместимости РЗСКм

    // защита, 2-концевая линия, совместимость РЗСК
    mObj->sParam.def.status.setEnable(true);
    mObj->sParam.def.setNumDevices(GB_NUM_DEVICES_2);
    mObj->sParam.glb.setCompatibility(GB_COMP_RZSK);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_RZSK));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_PuskAdjOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_IndReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Call, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // защита, 3-концевая линия, совместимость РЗСК
    mObj->sParam.def.setNumDevices(GB_NUM_DEVICES_3);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_RZSK));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_PuskAdjOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteResetAll, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk1, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk2, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePuskAll, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_IndReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Call, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // защита, 3-концевая линия, совместимость РЗСКм
    mObj->sParam.glb.setCompatibility(GB_COMP_RZSK_M);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_RZSK));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_PuskAdjOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteResetAll, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk1, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk2, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePuskAll, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_IndReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Call, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_SingleOn, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // без защиты, 3-концевая линия
    mObj->sParam.def.status.setEnable(false);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_RZSK));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteResetAll, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_IndReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // без защиты, 2-концевая линия
    mObj->sParam.def.setNumDevices(GB_NUM_DEVICES_2);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_RZSK));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_IndReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());
}

//
TEST_F(clMenu_Test, fillLvlControl_R400m)
{
    uint8_t counter;

    // 2-концевая линия, совместимость АВАНТ
    mObj->sParam.def.setNumDevices(GB_NUM_DEVICES_2);
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_AVANT);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_R400M));

    // Выборочная проверка заполнения названия
    ASSERT_STREQ(mObj->Punkts_.GetName(1), mObj->mControl.getText(TControl::CTRL_Reset));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_PuskAdjOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcPuskSelf, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteAcPusk, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcNormal, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcAccelerated, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Call, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // 3-концевая линия, совместимость АВАНТ
    mObj->sParam.def.setNumDevices(GB_NUM_DEVICES_3);
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_AVANT);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_R400M));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_PuskAdjOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteResetAll, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk1, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk2, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePuskAll, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcPuskSelf, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteAcPusk, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcNormal, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcAccelerated, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Call, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // 2-концевая линия, совместимость ПВЗ90
    mObj->sParam.def.setNumDevices(GB_NUM_DEVICES_2);
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_PVZ90);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_R400M));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_PuskAdjOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcNormal, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcAccelerated, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcTest, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcPusk, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // 2-концевая линия, совместимость ПВЗУ-Е
    mObj->sParam.def.setNumDevices(GB_NUM_DEVICES_2);
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_PVZUE);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_R400M));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_PuskAdjOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteMan, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteManAll, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcNormal, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcAccelerated, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcQuick, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcCtrlCheck, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Call, mObj->Punkts_.GetData(counter++));

    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // 3-концевая линия, совместимость ПВЗУ-Е
    mObj->sParam.def.setNumDevices(GB_NUM_DEVICES_3);
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_PVZUE);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_R400M));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_PuskAdjOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk1, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk2, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteMan1, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteMan2, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteManAll, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcNormal, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcAccelerated, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcQuick, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcCtrlCheck, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Call, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // 2-концевая линия, совместимость АВЗК-80
    mObj->sParam.def.setNumDevices(GB_NUM_DEVICES_2);
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_AVZK80);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_R400M));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_PuskAdjOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcNormal, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcAccelerated, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcTest, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcPusk, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // 2-концевая линия, совместимость ПВЗЛ
    mObj->sParam.def.setNumDevices(GB_NUM_DEVICES_2);
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_PVZL);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_R400M));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_PuskAdjOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcPuskSelf, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteAcPusk, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_PuskPrd, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcOn, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Call, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // 2-концевая линия, совместимость ЛИНИЯ-Р
    mObj->sParam.def.setNumDevices(GB_NUM_DEVICES_2);
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_LINER);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_R400M));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_PuskAdjOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcAuto, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcAccelerated, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Call, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // 3-концевая линия, совместимость ЛИНИЯ-Р
    mObj->sParam.def.setNumDevices(GB_NUM_DEVICES_3);
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_LINER);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_R400M));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_PuskAdjOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteReset1, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteReset2, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk1, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk2, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePuskAll, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcAuto, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcAccelerated, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Call, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // 2-концевая линия, совместимость ПВЗУ
    mObj->sParam.def.setNumDevices(GB_NUM_DEVICES_2);
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_PVZU);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_R400M));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_PuskAdjOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcNormal, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcAccelerated, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcQuick, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcCtrlCheck, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Call, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // 2-концевая линия, совместимость ПВЗ
    mObj->sParam.def.setNumDevices(GB_NUM_DEVICES_2);
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_PVZ);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_R400M));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_PuskAdjOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcNormal, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcAccelerated, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcRequest, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // 2-концевая линия, совместимость р400
    mObj->sParam.def.setNumDevices(GB_NUM_DEVICES_2);
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_R400);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_R400M));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_PuskAdjOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteReset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcAuto, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcAccelerated, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Call, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());

    // 3-концевая линия, совместимость р400
    mObj->sParam.def.setNumDevices(GB_NUM_DEVICES_3);
    mObj->sParam.glb.setCompatibility(GB_COMP_R400M_R400);

    ASSERT_TRUE(mObj->fillLvlControl(AVANT_R400M));

    counter = 0;
    ASSERT_EQ(TControl::CTRL_PuskAdjOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Reset, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteReset1, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemoteReset2, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk1, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePusk2, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_RemotePuskAll, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcAuto, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcAccelerated, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_AcOff, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(TControl::CTRL_Call, mObj->Punkts_.GetData(counter++));
    ASSERT_EQ(counter, mObj->Punkts_.GetLen());
}

//
TEST_F(clMenu_Test, fillLvlSetupGlb_K400)
{
    mObj->sParam.typeDevice = AVANT_K400;
    mObj->sParam.glb.setTypeLine(GB_TYPE_LINE_UM);  // требуется для установки совместимости

    struct test_t
    {
        eGB_PARAM param;
        uint      comp_mask;  // бит = 1 - значит есть в этой совместимости (UINT_MAX - есть всегда)
        bool      prd;        // true - есть только в передатчике
        bool      prm;        // true - есть только в приемнике
    };

    QVector<test_t> test_data {
        { GB_PARAM_COMP_K400, UINT_MAX, false, false },           //
        { GB_PARAM_TIME_SYNCH, UINT_MAX, false, false },          //
        { GB_PARAM_NUM_OF_DEVICE, UINT_MAX, false, false },       //
        { GB_PARAM_OUT_CHECK, UINT_MAX, false, false },           //
        { GB_PARAM_WARN_THD, UINT_MAX, false, true },             //
        { GB_PARAM_TIME_RERUN, UINT_MAX, false, true },           //
        { GB_PARAM_COM_PRD_KEEP, UINT_MAX, true, false },         //
        { GB_PARAM_COM_PRM_KEEP, UINT_MAX, false, true },         //
        { GB_PARAM_IN_DEC, UINT_MAX, false, true },               //
        { GB_PARAM_FREQ, UINT_MAX, false, false },                //
        { GB_PARAM_COR_U, UINT_MAX, true, false },                //
        { GB_PARAM_COR_I, UINT_MAX, true, false },                //
        { GB_PARAM_NUM_OF_DEVICES, UINT_MAX, false, false },      //
        { GB_PARAM_TM_K400, UINT_MAX, false, false },             //
        { GB_PARAM_WARN_D, UINT_MAX, false, false },              //
        { GB_PARAM_ALARM_D, UINT_MAX, false, false },             //
        { GB_PARAM_TEMP_MONITOR, UINT_MAX, false, false },        //
        { GB_PARAM_TEMP_THR_HI, UINT_MAX, false, false },         //
        { GB_PARAM_TEMP_THR_LOW, UINT_MAX, false, false },        //
        { GB_PARAM_TM_SPEED, (1 << 7) | (1 << 8), false, false }  //
    };

    for (int comp = GB_COMP_K400_MIN; comp < GB_COMP_K400_MAX; comp++)
    {
        for (int prd = 0; prd < 2; prd++)
        {
            for (int prm = 0; prm < 2; prm++)
            {
                SCOPED_TRACE("Compatibility " + to_string(comp) + ", prd " + to_string(prd)
                             + ", prm" + to_string(prm));

                mObj->sParam.glb.setCompatibility(static_cast<eGB_COMP_K400>(comp));
                mObj->sParam.prd.status.setEnable(prd != 0);
                mObj->sParam.prm.status.setEnable(prm != 1);

                ASSERT_TRUE(mObj->fillLvlSetupParamGlb(mObj->sParam.typeDevice));

                int counter = 0;
                for (int i = 0; i < test_data.size(); i++)
                {
                    eGB_PARAM param = static_cast<eGB_PARAM>(test_data.at(i).param);

                    SCOPED_TRACE("param[" + to_string(i) + "] = " + to_string(param));

                    // пропустить параметр который есть только в передатчике
                    if (test_data.at(i).prd && !mObj->sParam.prd.status.isEnable())
                    {
                        continue;
                    }

                    // пропустить параметр который есть только в приемнике
                    if (test_data.at(i).prm && !mObj->sParam.prm.status.isEnable())
                    {
                        continue;
                    }

                    // пропустить параметр которого нет в данной совместимости
                    if ((test_data.at(i).comp_mask & (1 << comp)) == 0)
                    {
                        continue;
                    }

                    counter++;
                    ASSERT_TRUE(checkLocalParam(param));
                }

                // Проверка количества параметров в списке
                ASSERT_EQ(counter, mObj->sParam.local.getNumOfParams());
            }
        }
    }
}

//
TEST_F(clMenu_Test, fillLvlSetupGlb_Opto)
{
    mObj->sParam.typeDevice = AVANT_OPTO;
    mObj->sParam.glb.setTypeLine(GB_TYPE_LINE_OPTO);  // требуется для установки типа оптики

    struct test_t
    {
        eGB_PARAM param;
        uint8_t   type_mask;  // бит = 1 - значит есть в этом типе линии (0xFF - есть всегда)
        bool      prd;  // true - есть только в передатчике
        bool      prm;  // true - есть только в приемнике
    };

    QVector<test_t> test_data {
        { GB_PARAM_TIME_SYNCH, 0xFF, false, false },          //
        { GB_PARAM_NUM_OF_DEVICE, 0x01, false, false },       //
        { GB_PARAM_NUM_OF_DEVICE_RING, 0xFE, false, false },  //
        { GB_PARAM_TIME_RERUN, 0XFF, false, true },           //
        { GB_PARAM_COM_PRD_KEEP, 0XFF, true, false },         //
        { GB_PARAM_COM_PRM_KEEP, 0XFF, false, true },         //
        { GB_PARAM_BACKUP, 0x01, false, false }               //
    };

    for (int type = TYPE_OPTO_STANDART; type < TYPE_OPTO_MAX; type++)
    {
        for (int prd = 0; prd < 2; prd++)
        {
            for (int prm = 0; prm < 2; prm++)
            {
                SCOPED_TRACE("Type opto " + to_string(type) + ", prd " + to_string(prd) + ", prm"
                             + to_string(prm));

                mObj->sParam.glb.setTypeOpto(static_cast<eGB_TYPE_OPTO>(type));
                mObj->sParam.prd.status.setEnable(prd != 0);
                mObj->sParam.prm.status.setEnable(prm != 1);

                ASSERT_TRUE(mObj->fillLvlSetupParamGlb(mObj->sParam.typeDevice));

                int counter = 0;
                for (int i = 0; i < test_data.size(); i++)
                {
                    eGB_PARAM param = static_cast<eGB_PARAM>(test_data.at(i).param);

                    SCOPED_TRACE("param[" + to_string(i) + "] = " + to_string(param));

                    // пропустить параметр который есть только в передатчике
                    if (test_data.at(i).prd && !mObj->sParam.prd.status.isEnable())
                    {
                        continue;
                    }

                    // пропустить параметр который есть только в приемнике
                    if (test_data.at(i).prm && !mObj->sParam.prm.status.isEnable())
                    {
                        continue;
                    }

                    // пропустить параметр которого нет в данной совместимости
                    if ((test_data.at(i).type_mask & (1 << type)) == 0)
                    {
                        continue;
                    }

                    counter++;
                    ASSERT_TRUE(checkLocalParam(param));
                }

                // Проверка количества параметров в списке
                ASSERT_EQ(counter, mObj->sParam.local.getNumOfParams());
            }
        }
    }
}


//
TEST_F(clMenu_Test, fillLvlSetupGlb_Rzsk)
{
    mObj->sParam.typeDevice = AVANT_RZSK;
    mObj->sParam.glb.setTypeLine(GB_TYPE_LINE_UM);  // требуется для установки совместимости

    struct test_t
    {
        eGB_PARAM param;
        uint      comp_readonly_mask;  // бит = 1 - только чтение в этой совместимости
    };

    QVector<test_t> test_data {
        { GB_PARAM_COMP_RZSK, 0 },                             //
        { GB_PARAM_NUM_OF_DEVICES, (1 << GB_COMP_RZSK_3E8) },  //
        { GB_PARAM_TIME_SYNCH_SRC, 0 },                        //
        { GB_PARAM_NUM_OF_DEVICE, 0 },                         //
        { GB_PARAM_OUT_CHECK, 0 },                             //
        { GB_PARAM_WARN_THD_CF, 0 },                           //
        { GB_PARAM_TIME_RERUN, 0 },                            //
        { GB_PARAM_COM_PRD_KEEP, 0 },                          //
        { GB_PARAM_COM_PRM_KEEP, 0 },                          //
        { GB_PARAM_IN_DEC, 0 },                                //
        { GB_PARAM_FREQ, 0 },                                  //
        { GB_PARAM_DETECTOR, 0 },                              //
        { GB_PARAM_COR_U, 0 },                                 //
        { GB_PARAM_COR_I, 0 }                                  //
    };

    for (int comp = GB_COMP_RZSK_MIN; comp < GB_COMP_RZSK_MAX; comp++)
    {
        for (int prd = 0; prd < 2; prd++)
        {
            for (int prm = 0; prm < 2; prm++)
            {
                SCOPED_TRACE("Compatibility " + to_string(comp) + ", prd " + to_string(prd)
                             + ", prm" + to_string(prm));

                mObj->sParam.glb.setCompatibility(static_cast<eGB_COMP_RZSK>(comp));
                mObj->sParam.prd.status.setEnable(prd != 0);
                mObj->sParam.prm.status.setEnable(prm != 1);

                ASSERT_TRUE(mObj->fillLvlSetupParamGlb(mObj->sParam.typeDevice));

                int counter = 0;
                for (int i = 0; i < test_data.size(); i++)
                {
                    eGB_PARAM param     = static_cast<eGB_PARAM>(test_data.at(i).param);
                    bool      read_only = test_data.at(i).comp_readonly_mask & (1 << comp);

                    SCOPED_TRACE("param[" + to_string(i) + "] = " + to_string(param));

                    counter++;
                    ASSERT_TRUE(checkLocalParam(param, read_only));
                }

                // Проверка количества параметров в списке
                ASSERT_EQ(counter, mObj->sParam.local.getNumOfParams());
            }
        }
    }
}

//
TEST_F(clMenu_Test, fillLvlSetupGlb_R400m)
{
    mObj->sParam.typeDevice = AVANT_R400M;
    mObj->sParam.glb.setTypeLine(GB_TYPE_LINE_UM);  // требуется для установки совместимости

    struct test_t
    {
        eGB_PARAM param;
        uint comp_mask;  // бит = 1 - только чтение в этой совместимости
    };

    QVector<test_t> test_data {
        //
        { GB_PARAM_COMP_P400, 0xFFFF },                                                       //
        { GB_PARAM_TIME_SYNCH_SRC, (1 << GB_COMP_R400M_AVANT) | (1 << GB_COMP_R400M_R400) },  //
        { GB_PARAM_NUM_OF_DEVICE, 0xFFFF },                                                   //
        { GB_PARAM_OUT_CHECK, 0xFFFF },                                                       //
        { GB_PARAM_WARN_THD, 0xFFFF },                                                        //
        { GB_PARAM_U_OUT_NOM, 0xFFFF },                                                       //
        { GB_PARAM_FREQ, 0xFFFF },                                                            //
        { GB_PARAM_IN_DEC_AC_ANSWER, (1 << GB_COMP_R400M_PVZL) },                             //
        { GB_PARAM_PVZUE_PROTOCOL, (1 << GB_COMP_R400M_PVZUE) },                              //
        { GB_PARAM_PVZUE_PARITY, (1 << GB_COMP_R400M_PVZUE) },                                //
        { GB_PARAM_PVZUE_FAIL, (1 << GB_COMP_R400M_PVZUE) | (1 << GB_COMP_R400M_PVZU) },      //
        { GB_PARAM_PVZUE_NOISE_THD, (1 << GB_COMP_R400M_PVZUE) },                             //
        { GB_PARAM_PVZUE_NOISE_LVL, (1 << GB_COMP_R400M_PVZUE) },                             //
        { GB_PARAM_PVZUE_AC_TYPE, (1 << GB_COMP_R400M_PVZUE) },                               //
        { GB_PARAM_PVZUE_AC_PERIOD, (1 << GB_COMP_R400M_PVZUE) },                             //
        { GB_PARAM_PVZUE_AC_PER_RE, (1 << GB_COMP_R400M_PVZUE) },                             //
        { GB_PARAM_PVZU_AC_CORRECT, (1 << GB_COMP_R400M_PVZU) },
        { GB_PARAM_ALARM_CF, (1 << GB_COMP_R400M_R400) },  //
        { GB_PARAM_COR_U, 0xFFFF },                        //
        { GB_PARAM_COR_I, 0xFFFF }
    };

    for (int comp = GB_COMP_R400M_MIN; comp < GB_COMP_R400M_MAX; comp++)
    {

        SCOPED_TRACE("Compatibility " + to_string(comp));

        mObj->sParam.glb.setCompatibility(static_cast<eGB_COMP_RZSK>(comp));

        ASSERT_TRUE(mObj->fillLvlSetupParamGlb(mObj->sParam.typeDevice));

        int counter = 0;
        for (int i = 0; i < test_data.size(); i++)
        {
            eGB_PARAM param = static_cast<eGB_PARAM>(test_data.at(i).param);

            SCOPED_TRACE("param[" + to_string(i) + "] = " + to_string(param));

            if ((test_data.at(i).comp_mask & (1 << comp)) == 0)
            {
                continue;
            }

            counter++;
            ASSERT_TRUE(checkLocalParam(param));
        }

        // Проверка количества параметров в списке
        ASSERT_EQ(counter, mObj->sParam.local.getNumOfParams());
    }
}
