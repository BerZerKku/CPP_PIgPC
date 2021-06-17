#include "gtest/gtest.h"
#include <cstdio>
#include <iostream>

using namespace std;

#include "PIg/src/flash.h"
#include "PIg/src/flashParams.h"
#include "PIg/src/parameter/param.h"

// ������ �������
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class TFlash_Test: public ::testing::Test {
   public:

    TFlash_Test() {}
    virtual ~TFlash_Test() override = default;

    bool checkParam(eGB_PARAM param, Param &prop) {
        bool check = true;

        EXPECT_STREQ(prop.name, getNameOfParam(param));
        EXPECT_EQ(prop.com, getCom(param));
        EXPECT_EQ(prop.param, getParamType(param));
        EXPECT_EQ(prop.range, getRangeType(param));
        EXPECT_EQ(prop.dim, getDim(param));
        EXPECT_STREQ(prop.listValues, getListOfValues(param));
        EXPECT_EQ(prop.num, getAbsMaxNumOfSameParams(param));
        EXPECT_EQ(prop.min, getAbsMin(param));
        EXPECT_EQ(prop.max, getAbsMax(param));
        EXPECT_EQ(prop.disc, getDisc(param));
        EXPECT_EQ(prop.fract, getFract(param));
        EXPECT_EQ(prop.send, getSendType(param));
        EXPECT_EQ(prop.sendDop, getSendDop(param));
        EXPECT_EQ(prop.dependMax, getDependMax(param));
        EXPECT_EQ(prop.dependSame, getDependSame(param));
        EXPECT_EQ(prop.changeReg, getChangeReg(param));

        cntTestedParam++;

        return check;
    }

   protected:
    static int cntTestedParam;
};

int TFlash_Test::cntTestedParam = 0;

TEST_F(TFlash_Test, gbParam_NullParam) {
    Param prop = {
        "",                     // �������� ���������
        GB_COM_NO,              // ������� ������������ ���������
        Param::PARAM_NO,        // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        1,                      // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_NO,             // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_NULL_PARAM, prop));
}

//
TEST_F(TFlash_Test, gbParam_TimeSynch) {
    Param prop = {
        "������������� �����",  // �������� ���������
        GB_COM_GET_TIME_SINCHR, // ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_TIME_SYNCH, prop));
}

//
TEST_F(TFlash_Test, gbParam_TimeSynchSrc) {
    Param prop = {
        "������������� �����",  // �������� ���������
        GB_COM_GET_TIME_SINCHR, // ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcTymeSynchSrc[0],         // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcTymeSynchSrc),   // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
};

    ASSERT_TRUE(checkParam(GB_PARAM_TIME_SYNCH_SRC, prop));
}

//
TEST_F(TFlash_Test, gbParam_NumOfDevice) {
    Param prop = {
        "����� ��������",       // �������� ���������
        GB_COM_GET_DEVICE_NUM,  // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_NO,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        1,                      // ����������� ��������
        3,                      // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_ON_NUM_DEVS,  // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_NUM_OF_DEVICE, prop));
}

//
TEST_F(TFlash_Test, gbParam_NumOfDeviceRing) {
    Param prop = {
        "����� ��������",       // �������� ���������
        GB_COM_GET_DEVICE_NUM,  // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_NO,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        1,                      // ����������� ��������
        255,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_NUM_OF_DEVICE_RING, prop));
}

//
TEST_F(TFlash_Test, gbParam_OutCheck) {
    Param prop = {
        "�������� ���.�������", // �������� ���������
        GB_COM_GET_OUT_CHECK,   // ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_OUT_CHECK, prop));
}

//
TEST_F(TFlash_Test, gbParam_WarnThd) {
    Param prop = {
        "����� ��������������", // �������� ���������
        GB_COM_GET_CF_THRESHOLD,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DB,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        22,                     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_WARN_THD, prop));
}

//
TEST_F(TFlash_Test, gbParam_WarnThdCf) {
    Param prop = {
        "����� �������. �� �C", // �������� ���������
        GB_COM_GET_CF_THRESHOLD,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DB,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        22,                     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_WARN_THD_CF, prop));
}

//
TEST_F(TFlash_Test, gbParam_TimeRerun) {
    Param prop = {
        "����� �����������",    // �������� ���������
        GB_COM_GET_TIME_RERUN,  // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_SEC,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        5,                      // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_TIME_RERUN, prop));
}

//
TEST_F(TFlash_Test, gbParam_ComPrdKeep) {
    Param prop = {
        "�����. ���� ���. ���", // �������� ���������
        GB_COM_GET_COM_PRD_KEEP,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        POS_COM_PRD_KEEP_prdKeep,   // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_COM_PRD_KEEP, prop));
}

//
TEST_F(TFlash_Test, gbParam_ComPrmKeep) {
    Param prop = {
        "�����. ���� ���. ���", // �������� ���������
        GB_COM_GET_COM_PRM_KEEP,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_COM_PRM_KEEP, prop));
}

//
TEST_F(TFlash_Test, gbParam_InDec) {
    Param prop = {
        "�������. ������. ���", // �������� ���������
        GB_COM_GET_CF_THRESHOLD,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DB,          // �����������
        fcNullBuf,              // ������ ��������
        2,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        22,                     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        2,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_ON_NUM_DEVS, // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_IN_DEC, prop));
}

//
TEST_F(TFlash_Test, gbParam_NetAddress) {
    Param prop = {
        "������� �����",        // �������� ���������
        GB_COM_GET_NET_ADR,     // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_NO,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        1,                      // ����������� ��������
        247,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_NO    // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_NET_ADDRESS, prop));
}

//
TEST_F(TFlash_Test, gbParam_UOutNom) {
    Param prop = {
        "U��� �����������",     // �������� ���������
        GB_COM_GET_COM_PRM_KEEP,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_V,           // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        18,                     // ����������� ��������
        50,                     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_U_OUT_NOM, prop));
}

//
TEST_F(TFlash_Test, gbParam_Freq) {
    Param prop = {
        "�������",              // �������� ���������
        GB_COM_GET_FREQ,        // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT_NO_DIM,// �������� ��������
        Param::DIM_KHZ,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        1000,                   // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT16_BE,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_FREQ, prop));
}

//
TEST_F(TFlash_Test, gbParam_CompP400) {
    Param prop = {
        "�������������",        // �������� ���������
        GB_COM_GET_COM_PRD_KEEP,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcCompatibility[0],     // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcCompatibility),// ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_COMP_P400, prop));
}

//
TEST_F(TFlash_Test, gbParam_InDecAcAnswe) {
    Param prop = {
        "�������� ������ ��",   // �������� ���������
        GB_COM_GET_TIME_RERUN,  // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DB,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        20,                     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_IN_DEC_AC_ANSWER, prop));
}

//
TEST_F(TFlash_Test, gbParam_InDetector) {
    Param prop = {
        "��� ���������",        // �������� ���������
        GB_COM_GET_TIME_SINCHR, // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_NO,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        1,                      // ����������� ��������
        3,                      // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        2,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_DETECTOR, prop));
}

//
TEST_F(TFlash_Test, gbParam_CorU) {
    Param prop = {
        "��������� ����������", // �������� ���������
        GB_COM_GET_COR_U_I,     // ������� ������������ ���������
        Param::PARAM_U_COR,     // ��� ���������
        Param::RANGE_U_COR,     // �������� ��������
        Param::DIM_V,           // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        -600,                   // ����������� ��������
        600,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_COR_U,          // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_NO    // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_COR_U, prop));
}

//
TEST_F(TFlash_Test, gbParam_CorI) {
    Param prop = {
        "��������� ����",       // �������� ���������
        GB_COM_GET_COR_U_I,     // ������� ������������ ���������
        Param::PARAM_I_COR,     // ��� ���������
        Param::RANGE_INT_PM,    // �������� ��������
        Param::DIM_MA,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        -999,                   // ����������� ��������
        999,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_COR_I,          // ��� ��������� ��� ���������� ����� ��������
        2,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_NO    // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_COR_I, prop));
}

//
TEST_F(TFlash_Test, gbParam_PvzueProtocol) {
    Param prop = {
        "�������� ������",      // �������� ���������
        GB_COM_GET_TIME_RERUN,  // ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcPvzueProtocol[0],     // ������ ��������
        1,                      // ���-�� ���������� ���������
        1,                      // ����������� ��������
        SIZE_OF(fcPvzueProtocol),// ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PVZUE_PROTOCOL, prop));
}

//
TEST_F(TFlash_Test, gbParam_PvzueParity) {
    Param prop = {
        "������� ��������",     // �������� ���������
        GB_COM_GET_TIME_RERUN,  // ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcPvzueParity[0],       // ������ ��������
        1,                      // ���-�� ���������� ���������
        1,                      // ����������� ��������
        SIZE_OF(fcPvzueParity), // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        2,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PVZUE_PARITY, prop));
}

//
TEST_F(TFlash_Test, gbParam_PvzueFail) {
    Param prop = {
        "���������� �������",   // �������� ���������
        GB_COM_GET_TIME_RERUN,  // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DEG,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        80,                     // ������������ ��������
        2,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        3,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PVZUE_FAIL, prop));
}

//
TEST_F(TFlash_Test, gbParam_PvzueNoiseThd) {
    Param prop = {
        "����� �� ������",      // �������� ���������
        GB_COM_GET_TIME_RERUN,  // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_SEC,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        255,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        4,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PVZUE_NOISE_THD, prop));
}

//
TEST_F(TFlash_Test, gbParam_PvzueNoiseLvl) {
    Param prop = {
        "���������� ������",    // �������� ���������
        GB_COM_GET_TIME_RERUN,  // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DEG,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        20,                     // ����������� ��������
        80,                     // ������������ ��������
        20,                     // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        5,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PVZUE_NOISE_LVL, prop));
}

//
TEST_F(TFlash_Test, gbParam_PvzueAcType) {
    Param prop = {
        "��� ������������",     // �������� ���������
        GB_COM_GET_TIME_RERUN,  // ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcPvzueTypeAC[0],       // ������ ��������
        1,                      // ���-�� ���������� ���������
        1,                      // ����������� ��������
        SIZE_OF(fcPvzueTypeAC), // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        6,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PVZUE_AC_TYPE, prop));
}

//
TEST_F(TFlash_Test, gbParam_PvzueAcPeriod) {
    Param prop = {
        "������ ������� ��",    // �������� ���������
        GB_COM_GET_TIME_RERUN,  // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_SEC,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        1,                      // ����������� ��������
        255,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        7,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PVZUE_AC_PERIOD, prop));
}

//
TEST_F(TFlash_Test, gbParam_PvzueAcPerRe) {
    Param prop = {
        "������ ����.����. ��", // �������� ���������
        GB_COM_GET_TIME_RERUN,  // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_SEC,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        1,                      // ����������� ��������
        255,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        8,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PVZUE_AC_PER_RE, prop));
}

//
TEST_F(TFlash_Test, gbParam_Backup) {
    Param prop = {
        "��������������",       // �������� ���������
        GB_COM_GET_COR_U_I,     // ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_BACKUP, prop));
}

//
TEST_F(TFlash_Test, gbParam_CompK400) {
    Param prop = {
        "�������������",        // �������� ���������
        GB_COM_GET_COM_PRD_KEEP,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcCompK400[0],          // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcCompK400),    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        POS_COM_PRD_KEEP_compK400,  // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_COMP_K400, prop));
}

//
TEST_F(TFlash_Test, gbParam_NumOfDevices) {
    Param prop = {
        "��� �����",            // �������� ���������
        GB_COM_DEF_GET_LINE_TYPE,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcNumDevices[0],        // ������ ��������
        1,                      // ���-�� ���������� ���������
        1,                      // ����������� ��������
        SIZE_OF(fcNumDevices),  // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_NUM_OF_DEVICES, prop));
}

//
TEST_F(TFlash_Test, gbParam_TmK400) {
    Param prop = {
        "������������",         // �������� ���������
        GB_COM_GET_COM_PRD_KEEP,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        POS_COM_PRD_KEEP_tmK400,// ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_TM_K400, prop));
}

//
TEST_F(TFlash_Test, gbParam_WarnD) {
    Param prop = {
        "����� �������. �� D",  // �������� ���������
        GB_COM_GET_COM_PRD_KEEP,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DB,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        -64,                    // ����������� ��������
        64,                     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        POS_COM_PRD_KEEP_warnD, // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_WARN_D, prop));
}

//
TEST_F(TFlash_Test, gbParam_AlarmD) {
    Param prop = {
        "����� ������ �� D",    // �������� ���������
        GB_COM_GET_COM_PRD_KEEP,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DB,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        -64,                    // ����������� ��������
        64,                     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        POS_COM_PRD_KEEP_alarmD,// ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_ALARM_D, prop));
}

//
TEST_F(TFlash_Test, gbParam_TempMonitor) {
    Param prop = {
        "�������� �����������", // �������� ���������
        GB_COM_GET_COM_PRD_KEEP,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        POS_COM_PRD_KEEP_tempMonitor,   // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_TEMP_MONITOR, prop));
}

//
TEST_F(TFlash_Test, gbParam_TempThrHi) {
    Param prop = {
        "����.�������� ������", // �������� ���������
        GB_COM_GET_COM_PRD_KEEP,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DEG,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        100,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        POS_COM_PRD_KEEP_tempThrHi, // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_TEMP_THR_HI, prop));
}

//
TEST_F(TFlash_Test, gbParam_TempThrLow) {
    Param prop = {
        "����.�������� ������", // �������� ���������
        GB_COM_GET_COM_PRD_KEEP,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DEG,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        100,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        POS_COM_PRD_KEEP_tempThrLow,    // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_TEMP_THR_LOW, prop));
}

//
TEST_F(TFlash_Test, gbParam_TmSpeed) {
    Param prop = {
        "�������� ��",          // �������� ���������
        GB_COM_GET_COM_PRD_KEEP,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcTMSpeed[0],           // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        POS_COM_PRD_KEEP_tmSpeed,   // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_TM_SPEED, prop));
}

//
TEST_F(TFlash_Test, gbParam_CompRZSK) {
    Param prop = {
        "�������������",        // �������� ���������
        GB_COM_GET_COM_PRD_KEEP,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcCompRZSK[0],          // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcCompRZSK),    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        2,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_COMP_RZSK, prop));
}

//
TEST_F(TFlash_Test, gbParam_DefType) {
    Param prop = {
        "��� ������",           // �������� ���������
        GB_COM_DEF_GET_DEF_TYPE,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcDefType[0],           // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcDefType),     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_DEF_TYPE, prop));
}

//
TEST_F(TFlash_Test, gbParam_TimeNoMan) {
    Param prop = {
        "���. ����� ��� ���.",  // �������� ���������
        GB_COM_DEF_GET_T_NO_MAN,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_HOUR,        // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        99,                     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_TIME_NO_MAN, prop));
}

//
TEST_F(TFlash_Test, gbParam_Overlap) {
    Param prop = {
        "���������� ���������", // �������� ���������
        GB_COM_DEF_GET_OVERLAP, // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DEG,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        54,                     // ������������ ��������
        2,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_OVERLAP, prop));
}

//
TEST_F(TFlash_Test, gbParam_OverlapP400) {
    Param prop = {
        "���������� ���������", // �������� ���������
        GB_COM_DEF_GET_OVERLAP, // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DEG,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        18,                     // ����������� ��������
        54,                     // ������������ ��������
        2,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_OVERLAP_P400, prop));
}

//
TEST_F(TFlash_Test, gbParam_OverlapOpto) {
    Param prop = {
        "���������� ���������", // �������� ���������
        GB_COM_DEF_GET_OVERLAP, // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DEG,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        54,                     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_OVERLAP_OPTO, prop));
}

//
TEST_F(TFlash_Test, gbParam_Delay) {
    Param prop = {
        "����������� ��������", // �������� ���������
        GB_COM_DEF_GET_DELAY,   // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DEG,         // �����������
        fcNullBuf,              // ������ ��������
        2,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        18,                     // ������������ ��������
        2,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_ON_NUM_DEVS, // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_DELAY, prop));
}

//
TEST_F(TFlash_Test, gbParam_DelayOpto) {
    Param prop = {
        "����������� ��������", // �������� ���������
        GB_COM_DEF_GET_DELAY,   // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DEG,         // �����������
        fcNullBuf,              // ������ ��������
        2,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        18,                     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_ON_NUM_DEVS, // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_DELAY_OPTO, prop));
}

//
TEST_F(TFlash_Test, gbParam_WarnThdRz) {
    Param prop = {
        "����� �������. �� ��", // �������� ���������
        GB_COM_DEF_GET_RZ_THRESH,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DB,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        16,                     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_WARN_THD_RZ, prop));
}

//
TEST_F(TFlash_Test, gbParam_SensDec) {
    Param prop = {
        "����������� ��������", // �������� ���������
        GB_COM_DEF_GET_RZ_DEC,  // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DB,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        32,                     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_SENS_DEC, prop));
}

//
TEST_F(TFlash_Test, gbParam_SensDecRz) {
    Param prop = {
        "�������. ������. ��",  // �������� ���������
        GB_COM_DEF_GET_RZ_DEC,  // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DB,          // �����������
        fcNullBuf,              // ������ ��������
        2,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        32,                     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_ON_NUM_DEVS, // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ����������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_SENS_DEC_RZ, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmType) {
    Param prop = {
        "��� ���������",        // �������� ���������
        GB_COM_DEF_GET_PRM_TYPE,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcPrmType[0],           // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcPrmType),     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_TYPE, prop));
}

//
TEST_F(TFlash_Test, gbParam_AcInDec) {
    Param prop = {
        "�������� ������ ��",   // �������� ���������
        GB_COM_DEF_GET_PRM_TYPE,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_AC_IN_DEC, prop));
}

//
TEST_F(TFlash_Test, gbParam_FreqPrd) {
    Param prop = {
        "������� ���",          // �������� ���������
        GB_COM_DEF_GET_FREQ_PRD,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcPvzlFreq[0],          // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcPvzlFreq),    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_FREQ_PRD, prop));
}

//
TEST_F(TFlash_Test, gbParam_FreqPrm) {
    Param prop = {
        "������� ���",          // �������� ���������
        GB_COM_DEF_GET_RZ_THRESH,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcPvzlFreq[0],          // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcPvzlFreq),    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_FREQ_PRM, prop));
}

//
TEST_F(TFlash_Test, gbParam_ShiftFront) {
    Param prop = {
        "����� ���.������ ���", // �������� ���������
        GB_COM_DEF_GET_OVERLAP, // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DEG,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        255,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_SHIFT_FRONT, prop));
}

//
TEST_F(TFlash_Test, gbParam_ShiftBack) {
    Param prop = {
        "����� ���.������ ���", // �������� ���������
        GB_COM_DEF_GET_OVERLAP, // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DEG,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        255,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        2,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_SHIFT_BACK, prop));
}

//
TEST_F(TFlash_Test, gbParam_ShiftPrm) {
    Param prop = {
        "����� ���",            // �������� ���������
        GB_COM_DEF_GET_OVERLAP, // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DEG,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        255,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        3,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_SHIFT_PRM, prop));
}

//
TEST_F(TFlash_Test, gbParam_ShiftPrd) {
    Param prop = {
        "����� �� ��� �� ����", // �������� ���������
        GB_COM_DEF_GET_OVERLAP, // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DEG,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        255,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        4,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_SHIFT_PRD, prop));
}

//
TEST_F(TFlash_Test, gbParam_LimitPrd) {
    Param prop = {
        "�������. ������ ���",  // �������� ���������
        GB_COM_DEF_GET_LIMIT_PRD,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_LIMIT_PRD, prop));
}

//
TEST_F(TFlash_Test, gbParam_DelayOffPrm) {
    Param prop = {
        "�������� ����. ���",   // �������� ���������
        GB_COM_DEF_GET_LIMIT_PRD,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DEG,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        255,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        2,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_DELAY_OFF_PRM, prop));
}

//
TEST_F(TFlash_Test, gbParam_DelayOnPrm) {
    Param prop = {
        "�������� ���. ���",    // �������� ���������
        GB_COM_DEF_GET_LIMIT_PRD,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DEG,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        255,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        3,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_DELAY_ON_PRM, prop));
}

//
TEST_F(TFlash_Test, gbParam_DelayOnPrd) {
    Param prop = {
        "�������� ���. ���",    // �������� ���������
        GB_COM_DEF_GET_LIMIT_PRD,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DEG,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        255,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        4,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_DELAY_ON_PRD, prop));
}

//
TEST_F(TFlash_Test, gbParam_MinTimePrd) {
    Param prop = {
        "���. ����. ���",       // �������� ���������
        GB_COM_DEF_GET_LIMIT_PRD,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DEG,         // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        255,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        5,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_MIN_TIME_PRD, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdInDelay) {
    Param prop = {
        "�������� ������. ���", // �������� ���������
        GB_COM_PRD_GET_TIME_ON, // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_MSEC,        // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        20,                     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_IN_DELAY, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdDurationL) {
    Param prop = {
        "������������ �������", // �������� ���������
        GB_COM_PRD_GET_DURATION,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_MSEC,        // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        30,                     // ����������� ��������
        100,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������                      // ���� �������������� ���������� ��� ����������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������                  // ���� �������������� ���������� ��� ����������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_DURATION_L, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdDurationO) {
    Param prop = {
        "������������ �������", // �������� ���������
        GB_COM_PRD_GET_DURATION,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_MSEC,        // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        30,                     // ����������� ��������
        500,                    // ������������ ��������
        10,                     // ������������
        10,                     // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_DURATION_O, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdTestCom) {
    Param prop = {
        "�������� �������",     // �������� ���������
        GB_COM_PRD_GET_TEST_COM,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_TEST_COM, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdComLong) {
    Param prop = {
        "�������� �������",     // �������� ���������
        GB_COM_PRD_GET_LONG_COM,// ������� ������������ ���������
        Param::PARAM_BITES,     // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        32,                     // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_BITES,      // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_ON_COM_PRD,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_COM_LONG, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdComBlock) {
    Param prop = {
        "��������. �������",    // �������� ���������
        GB_COM_PRD_GET_BLOCK_COM,// ������� ������������ ���������
        Param::PARAM_BITES,     // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        32,                     // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_BITES,      // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_ON_COM_PRD,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_COM_BLOCK, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdDrEnable) {
    Param prop = {
        "���������� ��",        // �������� ���������
        GB_COM_PRD_GET_DR_STATE,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_DR_ENABLE, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdDrComBlock) {
    Param prop = {
        "��������. ������� ��", // �������� ���������
        GB_COM_PRD_GET_DR_BLOCK,// ������� ������������ ���������
        Param::PARAM_BITES,     // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        32,                     // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_BITES,      // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_ON_COM_PRD,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_DR_COM_BLOCK, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdComNums) {
    Param prop = {
        "���������� ������",    // �������� ���������
        GB_COM_PRD_GET_COM,     // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_NO,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        4,                      // ����������� ��������
        32,                     // ������������ ��������
        4,                      // ������������
        4,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_COM_NUMS, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdComNumsA) {
    Param prop = {
        "���������� ������ �",  // �������� ���������
        GB_COM_PRD_GET_COM_A,   // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_NO,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        32,                     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_ON_COM_PRD,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_COM_NUMS_A, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdFreqCorr) {
    Param prop = {
        "��������� �������",    // �������� ���������
        GB_COM_PRD_GET_FREQ_CORR,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT_PM,    // �������� ��������
        Param::DIM_HZ,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        -100,                   // ����������� ��������
        100,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_FREQ_CORR, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdDecCf) {
    Param prop = {
        "�������� ������ ��",   // �������� ���������
        GB_COM_PRD_GET_CF_TM,   // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DB,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        20,                     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_DEC_CF, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdDecTm) {
    Param prop = {
        "�������� ������ ��",   // �������� ���������
        GB_COM_PRD_GET_CF_TM,   // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_DB,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        20,                     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        2,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ����������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_DEC_TM, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdDefaultCf) {
    Param prop = {
        "�� �� ���������",      // �������� ���������
        GB_COM_PRD_GET_CF_TM,   // ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcCF[0],                // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcCF),          // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        3,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ����������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_DEFAULT_CF, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdComSignal) {
    Param prop = {
        "������������ ������",  // �������� ���������
        GB_COM_PRD_GET_COM_SIGN,// ������� ������������ ���������
        Param::PARAM_BITES,     // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        32,                     // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_BITES,      // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_ON_COM_PRD,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_COM_SIGNAL, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmTimeOn) {
    Param prop = {
        "�������� �� ����.���", // �������� ���������
        GB_COM_PRM_GET_TIME_ON, // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_MSEC,        // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        10,                      // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ����������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_TIME_ON, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmComBlock) {
    Param prop = {
        "��������. �������",    // �������� ���������
        GB_COM_PRM_GET_BLOCK_COM,// ������� ������������ ���������
        Param::PARAM_BITES,     // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        32,                     // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_BITES,      // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ����������� ���������
        Param::DEPEND_SAME_ON_COM_PRM,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_COM_BLOCK, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmTimeOff) {
    Param prop = {
        "�������� �� ����.���", // �������� ���������
        GB_COM_PRM_GET_TIME_OFF,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_MSEC,        // �����������
        fcNullBuf,              // ������ ��������
        32,                     // ���-�� ���������� ���������
        0,                      // ����������� ��������
        1000,                   // ������������ ��������
        50,                     // ������������
        10,                     // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_ON_COM_PRM,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_TIME_OFF, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmDrEnable) {
    Param prop = {
        "���������� ��",        // �������� ���������
        GB_COM_PRM_GET_DR_STATE,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_DR_ENABLE, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmDrComBlock) {
    Param prop = {
        "��������. ������� ��", // �������� ���������
        GB_COM_PRM_GET_DR_BLOCK,// ������� ������������ ���������
        Param::PARAM_BITES,     // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        32,                     // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_BITES,      // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_ON_COM_PRM,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_DR_COM_BLOCK, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmDrComToHf) {
    Param prop = {
        "������� ��� � ��",     // �������� ���������
        GB_COM_PRM_GET_DR_COM,  // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_NO,          // �����������
        fcNullBuf,              // ������ ��������
        32,                     // ���-�� ���������� ���������
        1,                      // ����������� ��������
        32,                     // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_ON_COM_PRM,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_DR_COM_TO_HF, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmComNums) {
    Param prop = {
        "���������� ������",    // �������� ���������
        GB_COM_PRM_GET_COM,     // ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_NO,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        4,                      // ����������� ��������
        32,                     // ������������ ��������
        4,                      // ������������
        4,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_COM_NUMS, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmTestCom) {
    Param prop = {
        "����� �������� ���.",  // �������� ���������
        GB_COM_PRM_GET_TEST_COM,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_TEST_COM, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmFreqCorr) {
    Param prop = {
        "��������� �������",    // �������� ���������
        GB_COM_PRM_GET_FREQ_CORR,// ������� ������������ ���������
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT_PM,    // �������� ��������
        Param::DIM_HZ,          // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        -100,                   // ����������� ��������
        100,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_FREQ_CORR, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmComSignal) {
    Param prop = {
        "������������ ������",  // �������� ���������
        GB_COM_PRM_GET_COM_SIGN,// ������� ������������ ���������
        Param::PARAM_BITES,     // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        32,                     // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_BITES,      // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,             // ����������� ���������
        Param::DEPEND_SAME_ON_COM_PRM,  // ����������� ����������
        Param::CHANGE_REG_DISABLE       // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_COM_SIGNAL, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmIncSafety) {
    Param prop = {
        "��������� �������-��", // �������� ���������
        GB_COM_PRM_GET_INC_SAFETY,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,  // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8,           // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ����������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_INC_SAFETY, prop));
}

//
TEST_F(TFlash_Test, gbParam_IntfProtocol) {
    Param prop = {
        "��������",             // �������� ���������
        GB_COM_NO,              // ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcProtocol[0],          // ������ ��������
        1,                      // ���-�� ���������� ���������
        TProtocol::MIN,         // ����������� ��������
        SIZE_OF(fcProtocol),    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_NO,             // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_NO    // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_INTF_PROTOCOL, prop));
}

//
TEST_F(TFlash_Test, gbParam_IntfBaudrate) {
    Param prop = {
        "�������� ��������",    // �������� ���������
        GB_COM_NO,              // ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcBaudRate[0],          // ������ ��������
        1,                      // ���-�� ���������� ���������
        TBaudRate::MIN,         // ����������� ��������
        SIZE_OF(fcBaudRate),    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_NO,             // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_NO    // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_INTF_BAUDRATE, prop));
}

//
TEST_F(TFlash_Test, gbParam_IntfDataBits) {
    Param prop = {
        "���� ������",          // �������� ���������
        GB_COM_NO,              // ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcDataBits[0],          // ������ ��������
        1,                      // ���-�� ���������� ���������
        TDataBits::MIN,         // ����������� ��������
        SIZE_OF(fcDataBits),    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_NO,             // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_NO    // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_INTF_DATA_BITS, prop));
}

//
TEST_F(TFlash_Test, gbParam_IntfParity) {
    Param prop = {
        "��������",             // �������� ���������
        GB_COM_NO,              // ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcParity[0],            // ������ ��������
        1,                      // ���-�� ���������� ���������
        TParity::MIN,           // ����������� ��������
        SIZE_OF(fcParity),      // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_NO,             // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_NO    // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_INTF_PARITY, prop));
}

//
TEST_F(TFlash_Test, gbParam_IntfStopBits) {
    Param prop = {
        "�������� ����",        // �������� ���������
        GB_COM_NO,              // ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcStopBits[0],          // ������ ��������
        1,                      // ���-�� ���������� ���������
        TStopBits::MIN,         // ����������� ��������
        SIZE_OF(fcStopBits),    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_NO,             // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_NO    // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_INTF_STOP_BITS, prop));
}

//
TEST_F(TFlash_Test, gbParam_IntfInterface) {
    Param prop = {
        "��������� �����",      // �������� ���������
        GB_COM_NO,              // ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcInterface[0],         // ������ ��������
        1,                      // ���-�� ���������� ���������
        TInterface::MIN,        // ����������� ��������
        SIZE_OF(fcInterface),   // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_NO,             // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_NO    // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_INTF_INTERFACE, prop));
}

//
TEST_F(TFlash_Test, gbParam_RingTimeWait) {
    Param prop = {
        "����� �������� ���.",  // �������� ���������
        GB_COM_GET_COM_PRD_KEEP,// ������� ������������ ��������� TODO
        Param::PARAM_INT,       // ��� ���������
        Param::RANGE_INT,       // �������� ��������
        Param::DIM_MSEC,        // �����������
        fcNullBuf,              // ������ ��������
        1,                      // ���-�� ���������� ���������
        0,                      // ����������� ��������
        255,                    // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_INT8_DOP,       // ��� ��������� ��� ���������� ����� ��������
        POS_COM_PRD_KEEP_ringTimeWait,  // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_RING_TIME_WAIT, prop));
}

//
TEST_F(TFlash_Test, gbParam_RingComTransit) {
    Param prop = {
        "���������� �������",   // �������� ���������
        GB_COM_GET_COM_PRD_KEEP,// ������� ������������ ��������� TODO
        Param::PARAM_BITES,     // ��� ���������
        Param::RANGE_ON_OFF,    // �������� ��������
        Param::DIM_NO,          // �����������
        fcOnOff[0],             // ������ ��������
        MAX_NUM_COM_RING,       // ���-�� ���������� ���������
        0,                      // ����������� ��������
        SIZE_OF(fcOnOff),       // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_BITES_DOP,      // ��� ��������� ��� ���������� ����� ��������
        POS_COM_PRD_KEEP_ringComTransit,    // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_NO,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_RING_COM_TRANSIT, prop));
    ASSERT_EQ(96, MAX_NUM_COM_RING);
}

//
TEST_F(TFlash_Test, gbParam_RingComRec) {
    Param prop = {
        "�������������� ���",   // �������� ���������
        GB_COM_PRM_GET_RING_COM_REC,// ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcRingRenumber[0],      // ������ ��������
        MAX_NUM_COM_PRM,        // ���-�� ���������� ���������
        0,                      // ����������� ��������
        MAX_NUM_COM_RING + 1,   // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_ON_COM_PRM,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_RING_COM_REC, prop));
    ASSERT_EQ(96, MAX_NUM_COM_RING);
}

//
TEST_F(TFlash_Test, gbParam_RingComTr) {
    Param prop = {
        "�������������� ���",   // �������� ���������
        GB_COM_PRD_GET_RING_COM_TR, // ������� ������������ ���������
        Param::PARAM_LIST,      // ��� ���������
        Param::RANGE_LIST,      // �������� ��������
        Param::DIM_NO,          // �����������
        fcRingRenumber[0],      // ������ ��������
        MAX_NUM_COM_PRD,        // ���-�� ���������� ���������
        0,                      // ����������� ��������
        MAX_NUM_COM_RING + 1,   // ������������ ��������
        1,                      // ������������
        1,                      // ��������� ��� ������������ ���������
        GB_SEND_DOP_INT8,       // ��� ��������� ��� ���������� ����� ��������
        1,                      // ���� �������������� ���������� ��� ����������
        Param::DEPEND_MAX_NO,   // ���������� ���������
        Param::DEPEND_SAME_ON_COM_PRM,  // ����������� ����������
        Param::CHANGE_REG_DISABLE   // ������� ��� ��������� ���������
    };

    ASSERT_TRUE(checkParam(GB_PARAM_RING_COM_TR, prop));
}

//
TEST_F(TFlash_Test, gbParam_common) {
    ASSERT_EQ(GB_PARAM_MAX, getSizeOfParam());
    ASSERT_EQ(GB_PARAM_MAX, cntTestedParam);

    ASSERT_EQ(32, MAX_NUM_COM_PRM);
    ASSERT_EQ(32, MAX_NUM_COM_PRD);
    ASSERT_EQ(96, MAX_NUM_COM_RING);

}
