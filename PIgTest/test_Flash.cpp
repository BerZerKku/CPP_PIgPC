#include "gtest/gtest.h"
#include <cstdio>
#include <iostream>

using namespace std;

#include "PIg/src/flash.h"
#include "PIg/src/flashParams.h"
#include "PIg/src/parameter/param.h"

// размер массива
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
        "",                     // название параметра
        GB_COM_NO,              // команда стандартного протокола
        Param::PARAM_NO,        // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        1,                      // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_NO,             // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_NULL_PARAM, prop));
}

//
TEST_F(TFlash_Test, gbParam_TimeSynch) {
    Param prop = {
        "Синхронизация часов",  // название параметра
        GB_COM_GET_TIME_SINCHR, // команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_TIME_SYNCH, prop));
}

//
TEST_F(TFlash_Test, gbParam_TimeSynchSrc) {
    Param prop = {
        "Синхронизация часов",  // название параметра
        GB_COM_GET_TIME_SINCHR, // команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcTymeSynchSrc[0],         // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcTymeSynchSrc),   // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
};

    ASSERT_TRUE(checkParam(GB_PARAM_TIME_SYNCH_SRC, prop));
}

//
TEST_F(TFlash_Test, gbParam_NumOfDevice) {
    Param prop = {
        "Номер аппарата",       // название параметра
        GB_COM_GET_DEVICE_NUM,  // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_NO,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        1,                      // минимальное значение
        3,                      // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_ON_NUM_DEVS,  // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_NUM_OF_DEVICE, prop));
}

//
TEST_F(TFlash_Test, gbParam_NumOfDeviceRing) {
    Param prop = {
        "Номер аппарата",       // название параметра
        GB_COM_GET_DEVICE_NUM,  // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_NO,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        1,                      // минимальное значение
        255,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_NUM_OF_DEVICE_RING, prop));
}

//
TEST_F(TFlash_Test, gbParam_OutCheck) {
    Param prop = {
        "Контроль вых.сигнала", // название параметра
        GB_COM_GET_OUT_CHECK,   // команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_OUT_CHECK, prop));
}

//
TEST_F(TFlash_Test, gbParam_WarnThd) {
    Param prop = {
        "Порог предупреждения", // название параметра
        GB_COM_GET_CF_THRESHOLD,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DB,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        22,                     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_WARN_THD, prop));
}

//
TEST_F(TFlash_Test, gbParam_WarnThdCf) {
    Param prop = {
        "Порог предупр. по КC", // название параметра
        GB_COM_GET_CF_THRESHOLD,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DB,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        22,                     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_WARN_THD_CF, prop));
}

//
TEST_F(TFlash_Test, gbParam_TimeRerun) {
    Param prop = {
        "Время перезапуска",    // название параметра
        GB_COM_GET_TIME_RERUN,  // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_SEC,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        5,                      // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_TIME_RERUN, prop));
}

//
TEST_F(TFlash_Test, gbParam_ComPrdKeep) {
    Param prop = {
        "Удерж. реле ком. ПРД", // название параметра
        GB_COM_GET_COM_PRD_KEEP,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        POS_COM_PRD_KEEP_prdKeep,   // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_COM_PRD_KEEP, prop));
}

//
TEST_F(TFlash_Test, gbParam_ComPrmKeep) {
    Param prop = {
        "Удерж. реле ком. ПРМ", // название параметра
        GB_COM_GET_COM_PRM_KEEP,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_COM_PRM_KEEP, prop));
}

//
TEST_F(TFlash_Test, gbParam_InDec) {
    Param prop = {
        "Загрубл. чувств. ПРМ", // название параметра
        GB_COM_GET_CF_THRESHOLD,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DB,          // размерность
        fcNullBuf,              // массив значений
        2,                      // кол-во повторений параметра
        0,                      // минимальное значение
        22,                     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        2,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_ON_NUM_DEVS, // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_IN_DEC, prop));
}

//
TEST_F(TFlash_Test, gbParam_NetAddress) {
    Param prop = {
        "Сетевой адрес",        // название параметра
        GB_COM_GET_NET_ADR,     // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_NO,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        1,                      // минимальное значение
        247,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_NO    // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_NET_ADDRESS, prop));
}

//
TEST_F(TFlash_Test, gbParam_UOutNom) {
    Param prop = {
        "Uвых номинальное",     // название параметра
        GB_COM_GET_COM_PRM_KEEP,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_V,           // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        18,                     // минимальное значение
        50,                     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_U_OUT_NOM, prop));
}

//
TEST_F(TFlash_Test, gbParam_Freq) {
    Param prop = {
        "Частота",              // название параметра
        GB_COM_GET_FREQ,        // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT_NO_DIM,// диапазон измнения
        Param::DIM_KHZ,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        1000,                   // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT16_BE,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_FREQ, prop));
}

//
TEST_F(TFlash_Test, gbParam_CompP400) {
    Param prop = {
        "Совместимость",        // название параметра
        GB_COM_GET_COM_PRD_KEEP,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcCompatibility[0],     // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcCompatibility),// максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_COMP_P400, prop));
}

//
TEST_F(TFlash_Test, gbParam_InDecAcAnswe) {
    Param prop = {
        "Снижение ответа АК",   // название параметра
        GB_COM_GET_TIME_RERUN,  // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DB,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        20,                     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_IN_DEC_AC_ANSWER, prop));
}

//
TEST_F(TFlash_Test, gbParam_InDetector) {
    Param prop = {
        "Тип детектора",        // название параметра
        GB_COM_GET_TIME_SINCHR, // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_NO,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        1,                      // минимальное значение
        3,                      // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        2,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_DETECTOR, prop));
}

//
TEST_F(TFlash_Test, gbParam_CorU) {
    Param prop = {
        "Коррекция напряжения", // название параметра
        GB_COM_GET_COR_U_I,     // команда стандартного протокола
        Param::PARAM_U_COR,     // тип параметра
        Param::RANGE_U_COR,     // диапазон измнения
        Param::DIM_V,           // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        -600,                   // минимальное значение
        600,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_COR_U,          // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_NO    // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_COR_U, prop));
}

//
TEST_F(TFlash_Test, gbParam_CorI) {
    Param prop = {
        "Коррекция тока",       // название параметра
        GB_COM_GET_COR_U_I,     // команда стандартного протокола
        Param::PARAM_I_COR,     // тип параметра
        Param::RANGE_INT_PM,    // диапазон измнения
        Param::DIM_MA,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        -999,                   // минимальное значение
        999,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_COR_I,          // тип параметра для сохранения новго значения
        2,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_NO    // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_COR_I, prop));
}

//
TEST_F(TFlash_Test, gbParam_PvzueProtocol) {
    Param prop = {
        "Протокол обмена",      // название параметра
        GB_COM_GET_TIME_RERUN,  // команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcPvzueProtocol[0],     // массив значений
        1,                      // кол-во повторений параметра
        1,                      // минимальное значение
        SIZE_OF(fcPvzueProtocol),// максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PVZUE_PROTOCOL, prop));
}

//
TEST_F(TFlash_Test, gbParam_PvzueParity) {
    Param prop = {
        "Признак четности",     // название параметра
        GB_COM_GET_TIME_RERUN,  // команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcPvzueParity[0],       // массив значений
        1,                      // кол-во повторений параметра
        1,                      // минимальное значение
        SIZE_OF(fcPvzueParity), // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        2,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PVZUE_PARITY, prop));
}

//
TEST_F(TFlash_Test, gbParam_PvzueFail) {
    Param prop = {
        "Допустимые провалы",   // название параметра
        GB_COM_GET_TIME_RERUN,  // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DEG,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        80,                     // максимальное значение
        2,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        3,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PVZUE_FAIL, prop));
}

//
TEST_F(TFlash_Test, gbParam_PvzueNoiseThd) {
    Param prop = {
        "Порог по помехе",      // название параметра
        GB_COM_GET_TIME_RERUN,  // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_SEC,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        255,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        4,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PVZUE_NOISE_THD, prop));
}

//
TEST_F(TFlash_Test, gbParam_PvzueNoiseLvl) {
    Param prop = {
        "Допустимая помеха",    // название параметра
        GB_COM_GET_TIME_RERUN,  // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DEG,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        20,                     // минимальное значение
        80,                     // максимальное значение
        20,                     // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        5,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PVZUE_NOISE_LVL, prop));
}

//
TEST_F(TFlash_Test, gbParam_PvzueAcType) {
    Param prop = {
        "Тип автоконтроля",     // название параметра
        GB_COM_GET_TIME_RERUN,  // команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcPvzueTypeAC[0],       // массив значений
        1,                      // кол-во повторений параметра
        1,                      // минимальное значение
        SIZE_OF(fcPvzueTypeAC), // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        6,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PVZUE_AC_TYPE, prop));
}

//
TEST_F(TFlash_Test, gbParam_PvzueAcPeriod) {
    Param prop = {
        "Период беглого АК",    // название параметра
        GB_COM_GET_TIME_RERUN,  // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_SEC,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        1,                      // минимальное значение
        255,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        7,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PVZUE_AC_PERIOD, prop));
}

//
TEST_F(TFlash_Test, gbParam_PvzueAcPerRe) {
    Param prop = {
        "Период повт.бегл. АК", // название параметра
        GB_COM_GET_TIME_RERUN,  // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_SEC,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        1,                      // минимальное значение
        255,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        8,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PVZUE_AC_PER_RE, prop));
}

//
TEST_F(TFlash_Test, gbParam_Backup) {
    Param prop = {
        "Резервирование",       // название параметра
        GB_COM_GET_COR_U_I,     // команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_BACKUP, prop));
}

//
TEST_F(TFlash_Test, gbParam_CompK400) {
    Param prop = {
        "Совместимость",        // название параметра
        GB_COM_GET_COM_PRD_KEEP,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcCompK400[0],          // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcCompK400),    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        POS_COM_PRD_KEEP_compK400,  // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_COMP_K400, prop));
}

//
TEST_F(TFlash_Test, gbParam_NumOfDevices) {
    Param prop = {
        "Тип линии",            // название параметра
        GB_COM_DEF_GET_LINE_TYPE,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcNumDevices[0],        // массив значений
        1,                      // кол-во повторений параметра
        1,                      // минимальное значение
        SIZE_OF(fcNumDevices),  // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_NUM_OF_DEVICES, prop));
}

//
TEST_F(TFlash_Test, gbParam_TmK400) {
    Param prop = {
        "Телемеханика",         // название параметра
        GB_COM_GET_COM_PRD_KEEP,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        POS_COM_PRD_KEEP_tmK400,// байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_TM_K400, prop));
}

//
TEST_F(TFlash_Test, gbParam_WarnD) {
    Param prop = {
        "Порог предупр. по D",  // название параметра
        GB_COM_GET_COM_PRD_KEEP,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DB,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        -64,                    // минимальное значение
        64,                     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        POS_COM_PRD_KEEP_warnD, // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_WARN_D, prop));
}

//
TEST_F(TFlash_Test, gbParam_AlarmD) {
    Param prop = {
        "Порог аварии по D",    // название параметра
        GB_COM_GET_COM_PRD_KEEP,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DB,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        -64,                    // минимальное значение
        64,                     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        POS_COM_PRD_KEEP_alarmD,// байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_ALARM_D, prop));
}

//
TEST_F(TFlash_Test, gbParam_TempMonitor) {
    Param prop = {
        "Контроль температуры", // название параметра
        GB_COM_GET_COM_PRD_KEEP,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        POS_COM_PRD_KEEP_tempMonitor,   // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_TEMP_MONITOR, prop));
}

//
TEST_F(TFlash_Test, gbParam_TempThrHi) {
    Param prop = {
        "Верх.значение темпер", // название параметра
        GB_COM_GET_COM_PRD_KEEP,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DEG,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        100,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        POS_COM_PRD_KEEP_tempThrHi, // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_TEMP_THR_HI, prop));
}

//
TEST_F(TFlash_Test, gbParam_TempThrLow) {
    Param prop = {
        "Нижн.значение темпер", // название параметра
        GB_COM_GET_COM_PRD_KEEP,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DEG,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        100,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        POS_COM_PRD_KEEP_tempThrLow,    // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_TEMP_THR_LOW, prop));
}

//
TEST_F(TFlash_Test, gbParam_TmSpeed) {
    Param prop = {
        "Скорость ТМ",          // название параметра
        GB_COM_GET_COM_PRD_KEEP,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcTMSpeed[0],           // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        POS_COM_PRD_KEEP_tmSpeed,   // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_TM_SPEED, prop));
}

//
TEST_F(TFlash_Test, gbParam_CompRZSK) {
    Param prop = {
        "Совместимость",        // название параметра
        GB_COM_GET_COM_PRD_KEEP,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcCompRZSK[0],          // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcCompRZSK),    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        2,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_COMP_RZSK, prop));
}

//
TEST_F(TFlash_Test, gbParam_DefType) {
    Param prop = {
        "Тип защиты",           // название параметра
        GB_COM_DEF_GET_DEF_TYPE,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcDefType[0],           // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcDefType),     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_DEF_TYPE, prop));
}

//
TEST_F(TFlash_Test, gbParam_TimeNoMan) {
    Param prop = {
        "Доп. время без ман.",  // название параметра
        GB_COM_DEF_GET_T_NO_MAN,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_HOUR,        // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        99,                     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_TIME_NO_MAN, prop));
}

//
TEST_F(TFlash_Test, gbParam_Overlap) {
    Param prop = {
        "Перекрытие импульсов", // название параметра
        GB_COM_DEF_GET_OVERLAP, // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DEG,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        54,                     // максимальное значение
        2,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_OVERLAP, prop));
}

//
TEST_F(TFlash_Test, gbParam_OverlapP400) {
    Param prop = {
        "Перекрытие импульсов", // название параметра
        GB_COM_DEF_GET_OVERLAP, // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DEG,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        18,                     // минимальное значение
        54,                     // максимальное значение
        2,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_OVERLAP_P400, prop));
}

//
TEST_F(TFlash_Test, gbParam_OverlapOpto) {
    Param prop = {
        "Перекрытие импульсов", // название параметра
        GB_COM_DEF_GET_OVERLAP, // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DEG,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        54,                     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_OVERLAP_OPTO, prop));
}

//
TEST_F(TFlash_Test, gbParam_Delay) {
    Param prop = {
        "Компенсация задержки", // название параметра
        GB_COM_DEF_GET_DELAY,   // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DEG,         // размерность
        fcNullBuf,              // массив значений
        2,                      // кол-во повторений параметра
        0,                      // минимальное значение
        18,                     // максимальное значение
        2,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_ON_NUM_DEVS, // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_DELAY, prop));
}

//
TEST_F(TFlash_Test, gbParam_DelayOpto) {
    Param prop = {
        "Компенсация задержки", // название параметра
        GB_COM_DEF_GET_DELAY,   // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DEG,         // размерность
        fcNullBuf,              // массив значений
        2,                      // кол-во повторений параметра
        0,                      // минимальное значение
        18,                     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_ON_NUM_DEVS, // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_DELAY_OPTO, prop));
}

//
TEST_F(TFlash_Test, gbParam_WarnThdRz) {
    Param prop = {
        "Порог предупр. по РЗ", // название параметра
        GB_COM_DEF_GET_RZ_THRESH,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DB,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        16,                     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_WARN_THD_RZ, prop));
}

//
TEST_F(TFlash_Test, gbParam_SensDec) {
    Param prop = {
        "Загрубление чувствит", // название параметра
        GB_COM_DEF_GET_RZ_DEC,  // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DB,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        32,                     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_SENS_DEC, prop));
}

//
TEST_F(TFlash_Test, gbParam_SensDecRz) {
    Param prop = {
        "Загрубл. чувств. РЗ",  // название параметра
        GB_COM_DEF_GET_RZ_DEC,  // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DB,          // размерность
        fcNullBuf,              // массив значений
        2,                      // кол-во повторений параметра
        0,                      // минимальное значение
        32,                     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_ON_NUM_DEVS, // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметрай
    };

    ASSERT_TRUE(checkParam(GB_PARAM_SENS_DEC_RZ, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmType) {
    Param prop = {
        "Тип приемника",        // название параметра
        GB_COM_DEF_GET_PRM_TYPE,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcPrmType[0],           // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcPrmType),     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_TYPE, prop));
}

//
TEST_F(TFlash_Test, gbParam_AcInDec) {
    Param prop = {
        "Снижение уровня АК",   // название параметра
        GB_COM_DEF_GET_PRM_TYPE,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_AC_IN_DEC, prop));
}

//
TEST_F(TFlash_Test, gbParam_FreqPrd) {
    Param prop = {
        "Частота ПРД",          // название параметра
        GB_COM_DEF_GET_FREQ_PRD,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcPvzlFreq[0],          // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcPvzlFreq),    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_FREQ_PRD, prop));
}

//
TEST_F(TFlash_Test, gbParam_FreqPrm) {
    Param prop = {
        "Частота ПРМ",          // название параметра
        GB_COM_DEF_GET_RZ_THRESH,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcPvzlFreq[0],          // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcPvzlFreq),    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_FREQ_PRM, prop));
}

//
TEST_F(TFlash_Test, gbParam_ShiftFront) {
    Param prop = {
        "Сдвиг пер.фронта ПРД", // название параметра
        GB_COM_DEF_GET_OVERLAP, // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DEG,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        255,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_SHIFT_FRONT, prop));
}

//
TEST_F(TFlash_Test, gbParam_ShiftBack) {
    Param prop = {
        "Сдвиг зад.фронта ПРД", // название параметра
        GB_COM_DEF_GET_OVERLAP, // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DEG,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        255,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        2,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_SHIFT_BACK, prop));
}

//
TEST_F(TFlash_Test, gbParam_ShiftPrm) {
    Param prop = {
        "Сдвиг ПРМ",            // название параметра
        GB_COM_DEF_GET_OVERLAP, // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DEG,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        255,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        3,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_SHIFT_PRM, prop));
}

//
TEST_F(TFlash_Test, gbParam_ShiftPrd) {
    Param prop = {
        "Сдвиг ВЧ ПРД от ПУСК", // название параметра
        GB_COM_DEF_GET_OVERLAP, // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DEG,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        255,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        4,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_SHIFT_PRD, prop));
}

//
TEST_F(TFlash_Test, gbParam_LimitPrd) {
    Param prop = {
        "Огранич. полосы ПРД",  // название параметра
        GB_COM_DEF_GET_LIMIT_PRD,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_LIMIT_PRD, prop));
}

//
TEST_F(TFlash_Test, gbParam_DelayOffPrm) {
    Param prop = {
        "Задержка выкл. ПРМ",   // название параметра
        GB_COM_DEF_GET_LIMIT_PRD,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DEG,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        255,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        2,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_DELAY_OFF_PRM, prop));
}

//
TEST_F(TFlash_Test, gbParam_DelayOnPrm) {
    Param prop = {
        "Задержка вкл. ПРМ",    // название параметра
        GB_COM_DEF_GET_LIMIT_PRD,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DEG,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        255,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        3,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_DELAY_ON_PRM, prop));
}

//
TEST_F(TFlash_Test, gbParam_DelayOnPrd) {
    Param prop = {
        "Задержка вкл. ПРД",    // название параметра
        GB_COM_DEF_GET_LIMIT_PRD,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DEG,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        255,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        4,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_DELAY_ON_PRD, prop));
}

//
TEST_F(TFlash_Test, gbParam_MinTimePrd) {
    Param prop = {
        "Мин. длит. ПРД",       // название параметра
        GB_COM_DEF_GET_LIMIT_PRD,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DEG,         // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        255,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        5,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_MIN_TIME_PRD, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdInDelay) {
    Param prop = {
        "Задержка срабат. ПРД", // название параметра
        GB_COM_PRD_GET_TIME_ON, // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_MSEC,        // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        20,                     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_IN_DELAY, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdDurationL) {
    Param prop = {
        "Длительность команды", // название параметра
        GB_COM_PRD_GET_DURATION,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_MSEC,        // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        30,                     // минимальное значение
        100,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения                      // байт дополнительной информации для сохранения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра                  // байт дополнительной информации для сохранения
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_DURATION_L, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdDurationO) {
    Param prop = {
        "Длительность команды", // название параметра
        GB_COM_PRD_GET_DURATION,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_MSEC,        // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        30,                     // минимальное значение
        500,                    // максимальное значение
        10,                     // дискретность
        10,                     // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_DURATION_O, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdTestCom) {
    Param prop = {
        "Тестовая команда",     // название параметра
        GB_COM_PRD_GET_TEST_COM,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_TEST_COM, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdComLong) {
    Param prop = {
        "Следящие команды",     // название параметра
        GB_COM_PRD_GET_LONG_COM,// команда стандартного протокола
        Param::PARAM_BITES,     // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        32,                     // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_BITES,      // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_ON_COM_PRD,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_COM_LONG, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdComBlock) {
    Param prop = {
        "Блокиров. команды",    // название параметра
        GB_COM_PRD_GET_BLOCK_COM,// команда стандартного протокола
        Param::PARAM_BITES,     // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        32,                     // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_BITES,      // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_ON_COM_PRD,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_COM_BLOCK, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdDrEnable) {
    Param prop = {
        "Трансляция ЦП",        // название параметра
        GB_COM_PRD_GET_DR_STATE,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_DR_ENABLE, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdDrComBlock) {
    Param prop = {
        "Блокиров. команды ЦП", // название параметра
        GB_COM_PRD_GET_DR_BLOCK,// команда стандартного протокола
        Param::PARAM_BITES,     // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        32,                     // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_BITES,      // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_ON_COM_PRD,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_DR_COM_BLOCK, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdComNums) {
    Param prop = {
        "Количество команд",    // название параметра
        GB_COM_PRD_GET_COM,     // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_NO,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        4,                      // минимальное значение
        32,                     // максимальное значение
        4,                      // дискретность
        4,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_COM_NUMS, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdComNumsA) {
    Param prop = {
        "Количество команд А",  // название параметра
        GB_COM_PRD_GET_COM_A,   // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_NO,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        32,                     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_ON_COM_PRD,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_COM_NUMS_A, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdFreqCorr) {
    Param prop = {
        "Коррекция частоты",    // название параметра
        GB_COM_PRD_GET_FREQ_CORR,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT_PM,    // диапазон измнения
        Param::DIM_HZ,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        -100,                   // минимальное значение
        100,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_FREQ_CORR, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdDecCf) {
    Param prop = {
        "Снижение уровня КС",   // название параметра
        GB_COM_PRD_GET_CF_TM,   // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DB,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        20,                     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_DEC_CF, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdDecTm) {
    Param prop = {
        "Снижение уровня ТМ",   // название параметра
        GB_COM_PRD_GET_CF_TM,   // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_DB,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        20,                     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        2,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // зависимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_DEC_TM, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdDefaultCf) {
    Param prop = {
        "КС по умолчанию",      // название параметра
        GB_COM_PRD_GET_CF_TM,   // команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcCF[0],                // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcCF),          // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        3,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // зависимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_DEFAULT_CF, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrdComSignal) {
    Param prop = {
        "Сигнализация команд",  // название параметра
        GB_COM_PRD_GET_COM_SIGN,// команда стандартного протокола
        Param::PARAM_BITES,     // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        32,                     // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_BITES,      // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_ON_COM_PRD,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRD_COM_SIGNAL, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmTimeOn) {
    Param prop = {
        "Задержка на фикс.ком", // название параметра
        GB_COM_PRM_GET_TIME_ON, // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_MSEC,        // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        10,                      // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // зависимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_TIME_ON, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmComBlock) {
    Param prop = {
        "Блокиров. команды",    // название параметра
        GB_COM_PRM_GET_BLOCK_COM,// команда стандартного протокола
        Param::PARAM_BITES,     // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        32,                     // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_BITES,      // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // зависимость максимума
        Param::DEPEND_SAME_ON_COM_PRM,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_COM_BLOCK, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmTimeOff) {
    Param prop = {
        "Задержка на выкл.ком", // название параметра
        GB_COM_PRM_GET_TIME_OFF,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_MSEC,        // размерность
        fcNullBuf,              // массив значений
        32,                     // кол-во повторений параметра
        0,                      // минимальное значение
        1000,                   // максимальное значение
        50,                     // дискретность
        10,                     // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_ON_COM_PRM,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_TIME_OFF, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmDrEnable) {
    Param prop = {
        "Трансляция ЦП",        // название параметра
        GB_COM_PRM_GET_DR_STATE,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_DR_ENABLE, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmDrComBlock) {
    Param prop = {
        "Блокиров. команды ЦП", // название параметра
        GB_COM_PRM_GET_DR_BLOCK,// команда стандартного протокола
        Param::PARAM_BITES,     // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        32,                     // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_BITES,      // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_ON_COM_PRM,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_DR_COM_BLOCK, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmDrComToHf) {
    Param prop = {
        "Команда ПРМ в ЦП",     // название параметра
        GB_COM_PRM_GET_DR_COM,  // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_NO,          // размерность
        fcNullBuf,              // массив значений
        32,                     // кол-во повторений параметра
        1,                      // минимальное значение
        32,                     // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_ON_COM_PRM,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_DR_COM_TO_HF, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmComNums) {
    Param prop = {
        "Количество команд",    // название параметра
        GB_COM_PRM_GET_COM,     // команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_NO,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        4,                      // минимальное значение
        32,                     // максимальное значение
        4,                      // дискретность
        4,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_COM_NUMS, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmTestCom) {
    Param prop = {
        "Прием тестовой ком.",  // название параметра
        GB_COM_PRM_GET_TEST_COM,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_TEST_COM, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmFreqCorr) {
    Param prop = {
        "Коррекция частоты",    // название параметра
        GB_COM_PRM_GET_FREQ_CORR,// команда стандартного протокола
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT_PM,    // диапазон измнения
        Param::DIM_HZ,          // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        -100,                   // минимальное значение
        100,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_FREQ_CORR, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmComSignal) {
    Param prop = {
        "Сигнализация команд",  // название параметра
        GB_COM_PRM_GET_COM_SIGN,// команда стандартного протокола
        Param::PARAM_BITES,     // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        32,                     // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_BITES,      // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,             // зависимость максимума
        Param::DEPEND_SAME_ON_COM_PRM,  // зависимость повторений
        Param::CHANGE_REG_DISABLE       // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_COM_SIGNAL, prop));
}

//
TEST_F(TFlash_Test, gbParam_PrmIncSafety) {
    Param prop = {
        "Повышение безопас-ти", // название параметра
        GB_COM_PRM_GET_INC_SAFETY,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,  // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8,           // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // зависимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_PRM_INC_SAFETY, prop));
}

//
TEST_F(TFlash_Test, gbParam_IntfProtocol) {
    Param prop = {
        "Протокол",             // название параметра
        GB_COM_NO,              // команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcProtocol[0],          // массив значений
        1,                      // кол-во повторений параметра
        TProtocol::MIN,         // минимальное значение
        SIZE_OF(fcProtocol),    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_NO,             // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_NO    // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_INTF_PROTOCOL, prop));
}

//
TEST_F(TFlash_Test, gbParam_IntfBaudrate) {
    Param prop = {
        "Скорость передачи",    // название параметра
        GB_COM_NO,              // команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcBaudRate[0],          // массив значений
        1,                      // кол-во повторений параметра
        TBaudRate::MIN,         // минимальное значение
        SIZE_OF(fcBaudRate),    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_NO,             // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_NO    // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_INTF_BAUDRATE, prop));
}

//
TEST_F(TFlash_Test, gbParam_IntfDataBits) {
    Param prop = {
        "Биты данных",          // название параметра
        GB_COM_NO,              // команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcDataBits[0],          // массив значений
        1,                      // кол-во повторений параметра
        TDataBits::MIN,         // минимальное значение
        SIZE_OF(fcDataBits),    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_NO,             // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_NO    // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_INTF_DATA_BITS, prop));
}

//
TEST_F(TFlash_Test, gbParam_IntfParity) {
    Param prop = {
        "Четность",             // название параметра
        GB_COM_NO,              // команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcParity[0],            // массив значений
        1,                      // кол-во повторений параметра
        TParity::MIN,           // минимальное значение
        SIZE_OF(fcParity),      // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_NO,             // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_NO    // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_INTF_PARITY, prop));
}

//
TEST_F(TFlash_Test, gbParam_IntfStopBits) {
    Param prop = {
        "Стоповые биты",        // название параметра
        GB_COM_NO,              // команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcStopBits[0],          // массив значений
        1,                      // кол-во повторений параметра
        TStopBits::MIN,         // минимальное значение
        SIZE_OF(fcStopBits),    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_NO,             // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_NO    // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_INTF_STOP_BITS, prop));
}

//
TEST_F(TFlash_Test, gbParam_IntfInterface) {
    Param prop = {
        "Интерфейс связи",      // название параметра
        GB_COM_NO,              // команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcInterface[0],         // массив значений
        1,                      // кол-во повторений параметра
        TInterface::MIN,        // минимальное значение
        SIZE_OF(fcInterface),   // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_NO,             // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_NO    // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_INTF_INTERFACE, prop));
}

//
TEST_F(TFlash_Test, gbParam_RingTimeWait) {
    Param prop = {
        "Время ожидания ком.",  // название параметра
        GB_COM_GET_COM_PRD_KEEP,// команда стандартного протокола TODO
        Param::PARAM_INT,       // тип параметра
        Param::RANGE_INT,       // диапазон измнения
        Param::DIM_MSEC,        // размерность
        fcNullBuf,              // массив значений
        1,                      // кол-во повторений параметра
        0,                      // минимальное значение
        255,                    // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_INT8_DOP,       // тип параметра для сохранения новго значения
        POS_COM_PRD_KEEP_ringTimeWait,  // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_RING_TIME_WAIT, prop));
}

//
TEST_F(TFlash_Test, gbParam_RingComTransit) {
    Param prop = {
        "Транзитные команды",   // название параметра
        GB_COM_GET_COM_PRD_KEEP,// команда стандартного протокола TODO
        Param::PARAM_BITES,     // тип параметра
        Param::RANGE_ON_OFF,    // диапазон измнения
        Param::DIM_NO,          // размерность
        fcOnOff[0],             // массив значений
        MAX_NUM_COM_RING,       // кол-во повторений параметра
        0,                      // минимальное значение
        SIZE_OF(fcOnOff),       // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_BITES_DOP,      // тип параметра для сохранения новго значения
        POS_COM_PRD_KEEP_ringComTransit,    // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_NO,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_RING_COM_TRANSIT, prop));
    ASSERT_EQ(96, MAX_NUM_COM_RING);
}

//
TEST_F(TFlash_Test, gbParam_RingComRec) {
    Param prop = {
        "Переназначение ПРМ",   // название параметра
        GB_COM_PRM_GET_RING_COM_REC,// команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcRingRenumber[0],      // массив значений
        MAX_NUM_COM_PRM,        // кол-во повторений параметра
        0,                      // минимальное значение
        MAX_NUM_COM_RING + 1,   // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_ON_COM_PRM,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
    };

    ASSERT_TRUE(checkParam(GB_PARAM_RING_COM_REC, prop));
    ASSERT_EQ(96, MAX_NUM_COM_RING);
}

//
TEST_F(TFlash_Test, gbParam_RingComTr) {
    Param prop = {
        "Переназначение ПРД",   // название параметра
        GB_COM_PRD_GET_RING_COM_TR, // команда стандартного протокола
        Param::PARAM_LIST,      // тип параметра
        Param::RANGE_LIST,      // диапазон измнения
        Param::DIM_NO,          // размерность
        fcRingRenumber[0],      // массив значений
        MAX_NUM_COM_PRD,        // кол-во повторений параметра
        0,                      // минимальное значение
        MAX_NUM_COM_RING + 1,   // максимальное значение
        1,                      // дискретность
        1,                      // множитель для стандартного протокола
        GB_SEND_DOP_INT8,       // тип параметра для сохранения новго значения
        1,                      // байт дополнительной информации для сохранения
        Param::DEPEND_MAX_NO,   // заивимость максимума
        Param::DEPEND_SAME_ON_COM_PRM,  // зависимость повторений
        Param::CHANGE_REG_DISABLE   // условие для изменения параметра
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
