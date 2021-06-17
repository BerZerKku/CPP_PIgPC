#include <algorithm>
#include <cstdio>
#include <iostream>

#include "gtest/gtest.h"

using namespace std;

#include "src/menu/test.h"
#include "src/flash.h"
#include "src/glbDefine.h"

// размер массива
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class TTest_Test: public ::testing::Test
{
public:
    TTest *mObj;

    TTest_Test()
    {

    }

    virtual ~TTest_Test() override = default;

protected:
    virtual void SetUp() override {
        mObj = new TTest;
    }

    virtual void TearDown() override {
        delete mObj;
    }

    // вызывается однократно, до первого теста
    static void SetUpTestCase() { }

    // вызывается однократно, после последнего теста
    static void TearDownTestCase() { }
};

//
TEST_F(TTest_Test, signal)
{
    ASSERT_EQ(1, mObj->getNumSignals());
    ASSERT_EQ(GB_SIGNAL_OFF, mObj->getCurrentSignal());
    ASSERT_EQ(GB_SIGNAL_OFF, mObj->getCurrentSignal2());

    ASSERT_EQ(100, MAX_NUM_TEST_SIGNAL);

    // проверка добавления сигналов

    ASSERT_TRUE(mObj->addSignalToList(GB_SIGNAL_OFF));
    ASSERT_TRUE(mObj->addSignalToList(GB_SIGNAL_COM32));
    ASSERT_TRUE(mObj->addSignalToList(eGB_TEST_SIGNAL(GB_SIGNAL_MAX - 1)));
    ASSERT_FALSE(mObj->addSignalToList(eGB_TEST_SIGNAL(GB_SIGNAL_MAX)));

    ASSERT_EQ(4, mObj->getNumSignals());
    ASSERT_EQ(GB_SIGNAL_OFF, mObj->getCurrentSignal());
    ASSERT_EQ(GB_SIGNAL_OFF, mObj->getCurrentSignal2());

    ASSERT_EQ(GB_SIGNAL_OFF, mObj->signalList[0]);
    ASSERT_EQ(GB_SIGNAL_OFF, mObj->signalList[1]);
    ASSERT_EQ(GB_SIGNAL_COM32, mObj->signalList[2]);
    ASSERT_EQ(GB_SIGNAL_MAX - 1, mObj->signalList[3]);
    ASSERT_EQ(GB_SIGNAL_OFF, mObj->signalList[4]);

    // проверка очистки списка сигналов

    mObj->clear();

    for(uint8_t i = 0; i < MAX_NUM_TEST_SIGNAL; i++) {
        ASSERT_EQ(GB_SIGNAL_OFF, mObj->signalList[i]);
    }

    ASSERT_EQ(1, mObj->getNumSignals());

    // проверка добавления максимального количества сигналов

    for(uint8_t i = 0; i < MAX_NUM_TEST_SIGNAL - 1; i++) {
        eGB_TEST_SIGNAL signal = static_cast<eGB_TEST_SIGNAL> (i % GB_SIGNAL_MAX);
        ASSERT_TRUE(mObj->addSignalToList(signal));
    }

    ASSERT_FALSE(mObj->addSignalToList(GB_SIGNAL_COM32));

    ASSERT_EQ(GB_SIGNAL_OFF, mObj->signalList[0]);
    for(uint8_t i = 0; i < MAX_NUM_TEST_SIGNAL - 1; i++) {
        eGB_TEST_SIGNAL signal = static_cast<eGB_TEST_SIGNAL> (i % GB_SIGNAL_MAX);
        ASSERT_EQ(signal, mObj->signalList[i + 1]);
    }
}

//
TEST_F(TTest_Test, setCurrentSignal_RzskHf)
{
    eGB_TYPE_DEVICE device = AVANT_RZSK;
    eGB_TYPE_OPTO typeopto = TYPE_OPTO_MAX;

    // РЗ1 - нет блокировки
    // РЗ2 - есть блокировка
    // Структура данных:
    // Байты: [b1] ... [b6]
    // бит:        4      3      2      1
    // b1/b4 :  [рз2]  [рз1]  [кч2]  [кч1]
    // b2/b5 : [ком4] [ком3] [ком2] [ком1]

    // проверка первого сигнала

    struct testData_t {
        string testName;
        uint8_t data[6];
        eGB_TEST_SIGNAL resultSignal;
        const char *resultText;
    };

    std::vector<testData_t> testdata = {
        {"data00", {0x00, 0x00, 0xFF}, GB_SIGNAL_OFF,         "выкл."},
        {"data01", {0x05, 0x00, 0xFF}, GB_SIGNAL_CF_NO_RZ,    "КЧ1"},
        {"data02", {0x09, 0x00, 0xFF}, GB_SIGNAL_CF_RZ,       "КЧ1 с блок"},
        {"data03", {0x06, 0x00, 0xFF}, GB_SIGNAL_CF2_NO_RZ,   "КЧ2"},
        {"data04", {0x0A, 0x00, 0xFF}, GB_SIGNAL_CF2_RZ,      "КЧ2 с блок"},
        {"data05", {0x04, 0x01, 0xFF}, GB_SIGNAL_COM1_NO_RZ,  "Команда1"},
        {"data06", {0x04, 0x02, 0xFF}, GB_SIGNAL_COM2_NO_RZ,  "Команда2"},
        {"data07", {0x04, 0x04, 0xFF}, GB_SIGNAL_COM3_NO_RZ,  "Команда3"},
        {"data08", {0x04, 0x08, 0xFF}, GB_SIGNAL_COM4_NO_RZ,  "Команда4"},
        {"data09", {0x04, 0x10, 0xFF}, GB_SIGNAL_COM5_NO_RZ,  "Команда5"},
        {"data10", {0x04, 0x20, 0xFF}, GB_SIGNAL_COM6_NO_RZ,  "Команда6"},
        {"data11", {0x04, 0x40, 0xFF}, GB_SIGNAL_COM7_NO_RZ,  "Команда7"},
        {"data12", {0x04, 0x80, 0xFF}, GB_SIGNAL_COM8_NO_RZ,  "Команда8"},
        {"data13", {0x08, 0x01, 0xFF}, GB_SIGNAL_COM1_RZ,     "Ком1+блок"},
        {"data14", {0x08, 0x02, 0xFF}, GB_SIGNAL_COM2_RZ,     "Ком2+блок"},
        {"data15", {0x08, 0x04, 0xFF}, GB_SIGNAL_COM3_RZ,     "Ком3+блок"},
        {"data16", {0x08, 0x08, 0xFF}, GB_SIGNAL_COM4_RZ,     "Ком4+блок"},
        {"data17", {0x08, 0x10, 0xFF}, GB_SIGNAL_COM5_RZ,     "Ком5+блок"},
        {"data18", {0x08, 0x20, 0xFF}, GB_SIGNAL_COM6_RZ,     "Ком6+блок"},
        {"data19", {0x08, 0x40, 0xFF}, GB_SIGNAL_COM7_RZ,     "Ком7+блок"},
        {"data20", {0x08, 0x80, 0xFF}, GB_SIGNAL_COM8_RZ,     "Ком8+блок"},
        // поиск начинается с первой команды, выводится первая найденная
        {"data21", {0x04, 0x0F, 0xFF}, GB_SIGNAL_COM1_NO_RZ,  "Команда1"},
        {"data22", {0x08, 0x0F, 0xFF}, GB_SIGNAL_COM1_RZ,     "Ком1+блок"},
        {"data23", {0x0F, 0x0F, 0xFF}, GB_SIGNAL_COM1_NO_RZ,  "Команда1"},
        // поиск блокировки с командой начинается с РЗ1 (не блок)
        {"data24", {0x0C, 0x0E, 0xFF}, GB_SIGNAL_COM2_NO_RZ,  "Команда2"},
        // без команды перебирается: РЗ1 + КЧ2, РЗ2 + КЧ2, РЗ1 + КЧ1, РЗ1 + КЧ1
        {"data25", {0xFF, 0x00, 0xFF}, GB_SIGNAL_CF2_NO_RZ,   "КЧ2"},
        {"data26", {0xFB, 0x00, 0xFF}, GB_SIGNAL_CF2_RZ,      "КЧ2 с блок"},
        {"data27", {0xFD, 0x00, 0xFF}, GB_SIGNAL_CF_NO_RZ,    "КЧ1"},
        {"data28", {0xF0, 0x00, 0xFF}, GB_SIGNAL_OFF,         "выкл."}
    };

    for(auto &item : testdata) {
        SCOPED_TRACE(item.testName);

        mObj->setCurrentSignal(item.data, device, typeopto);
        eGB_TEST_SIGNAL signal = mObj->getCurrentSignal();

        ASSERT_EQ(item.resultSignal, signal);
        ASSERT_STREQ(item.resultText, fcTest1K400[signal]);
    }

    // Проверка для второго сигнала

    struct testData2_t {
        string testName;
        uint8_t data[6];
        eGB_TEST_SIGNAL resultSignal1;
        const char *resultText1;
        eGB_TEST_SIGNAL resultSignal2;
        const char *resultText2;
    };

    std::vector<testData2_t> testdata2 = {
        {"data00", {0x00, 0x00, 0x01, 0x06, 0x00, 0x02},
         GB_SIGNAL_OFF,         "выкл.",    GB_SIGNAL_CF2_NO_RZ, "КЧ2"},
        {"data01", {0x04, 0x01, 0x04, 0x08, 0x01, 0xFF},
         GB_SIGNAL_COM1_NO_RZ,  "Команда1", GB_SIGNAL_COM1_RZ,   "Ком1+блок"},
        {"data02", {0x04, 0x0F, 0x04, 0xFD, 0x00, 0xFF},
         GB_SIGNAL_COM1_NO_RZ,  "Команда1", GB_SIGNAL_CF_NO_RZ,  "КЧ1"}
    };

    for(auto &item : testdata2) {
        SCOPED_TRACE(item.testName);

        mObj->setCurrentSignal(item.data, device, typeopto);
        eGB_TEST_SIGNAL signal1 = mObj->getCurrentSignal();
        eGB_TEST_SIGNAL signal2 = mObj->getCurrentSignal2();

        ASSERT_EQ(item.resultSignal1, signal1);
        ASSERT_STREQ(item.resultText1, fcTest1K400[signal1]);

        ASSERT_EQ(item.resultSignal2, signal2);
        ASSERT_STREQ(item.resultText2, fcTest1K400[signal2]);
    }
}

//
TEST_F(TTest_Test, setCurrentSignal_R400mHf)
{
    eGB_TYPE_DEVICE device = AVANT_R400M;
    eGB_TYPE_OPTO typeopto = TYPE_OPTO_MAX;

    // РЗ1 - нет блокировки
    // РЗ2 - есть блокировка
    // Структура данных:
    // Байты: [b1] ... [b6]
    // бит:    5      4      3      2      1
    // b1:  [рз2]  [кч4]  [кч3]  [кч2]  [кч1]

    struct testData_t {
        string testName;
        uint8_t data[6];
        eGB_TEST_SIGNAL resultSignal;
        const char *resultText;
    };

    std::vector<testData_t> testdata = {
        {"data00", {0x00, 0xFF, 0xFF}, GB_SIGNAL_OFF,          "выкл."},
        {"data01", {0x01, 0xFF, 0xFF}, GB_SIGNAL_CF1,          "КЧ1"},
        {"data02", {0x02, 0xFF, 0xFF}, GB_SIGNAL_CF2,          "КЧ2"},
        {"data03", {0x04, 0xFF, 0xFF}, GB_SIGNAL_CF3,          "КЧ3"},
        {"data04", {0x08, 0xFF, 0xFF}, GB_SIGNAL_CF4,          "КЧ4"},
        {"data05", {0x10, 0xFF, 0xFF}, GB_SIGNAL_RZ,           "РЗ"},
        {"data06", {0x11, 0xFF, 0xFF}, GB_SIGNAL_CF1_RZ_R400M, "КЧ1 и РЗ"},
        {"data07", {0x12, 0xFF, 0xFF}, GB_SIGNAL_CF2_RZ_R400M, "КЧ2 и РЗ"},
        {"data08", {0x14, 0xFF, 0xFF}, GB_SIGNAL_CF3_RZ_R400M, "КЧ3 и РЗ"},
        {"data09", {0x18, 0xFF, 0xFF}, GB_SIGNAL_CF4_RZ_R400M, "КЧ4 и РЗ"},
        // поиск КЧ начинается с 1
        {"data10", {0x0F, 0xFF, 0xFF}, GB_SIGNAL_CF1,          "КЧ1"},
        {"data11", {0xFF, 0xFF, 0xFF}, GB_SIGNAL_CF1_RZ_R400M, "КЧ1 и РЗ"}
    };

    for(auto &item : testdata) {
        SCOPED_TRACE(item.testName);

        mObj->setCurrentSignal(item.data, device, typeopto);
        eGB_TEST_SIGNAL signal = mObj->getCurrentSignal();

        ASSERT_EQ(item.resultSignal, signal);
        ASSERT_STREQ(item.resultText, fcTest1K400[signal]);

        ASSERT_EQ(GB_SIGNAL_MAX, mObj->getCurrentSignal2());
    }
}

//
TEST_F(TTest_Test, setCurrentSignal_K400Hf)
{
    eGB_TYPE_DEVICE device = AVANT_K400;
    eGB_TYPE_OPTO typeopto = TYPE_OPTO_MAX;

    // РЗ1 - нет блокировки
    // РЗ2 - есть блокировка
    // Структура данных:
    // Байты: [b1] ... [b10]
    // бит:         8       7       6       5       4       3       2       1
    // b1/b6:      [x]     [x]     [x]     [x]     [x]     [x]   [кч2]   [кч1]
    // b2/b7:   [ком8]  [ком7]  [ком6]  [ком5]  [ком4]  [ком3]  [ком2]  [ком1]
    // b3/b8:  [ком16] [ком15] [ком14] [ком13] [ком12] [ком11] [ком10]  [ком9]
    // b4/b9:  [ком24] [ком23] [ком22] [ком21] [ком20] [ком19] [ком18] [ком17]
    // b5/b10: [ком32] [ком31] [ком30] [ком29] [ком28] [ком27] [ком26] [ком25]

    // проверка первого сигнала

    struct testData_t {
        string testName;
        uint8_t data[5];
        eGB_TEST_SIGNAL resultSignal;
        const char *resultText;
    };

    std::vector<testData_t> testdata = {
        {"data00", {0x00, 0x00, 0x00, 0x00, 0x00}, GB_SIGNAL_OFF,   "выкл."},
        {"data01", {0x01, 0x00, 0x00, 0x00, 0x00}, GB_SIGNAL_CF1,   "КЧ1"},
        {"data02", {0x02, 0x00, 0x00, 0x00, 0x00}, GB_SIGNAL_CF2,   "КЧ2"},
        {"data03", {0x00, 0x01, 0x00, 0x00, 0x00}, GB_SIGNAL_COM1,  "Команда1"},
        {"data04", {0x00, 0x02, 0x00, 0x00, 0x00}, GB_SIGNAL_COM2,  "Команда2"},
        {"data05", {0x00, 0x04, 0x00, 0x00, 0x00}, GB_SIGNAL_COM3,  "Команда3"},
        {"data06", {0x00, 0x08, 0x00, 0x00, 0x00}, GB_SIGNAL_COM4,  "Команда4"},
        {"data07", {0x00, 0x10, 0x00, 0x00, 0x00}, GB_SIGNAL_COM5,  "Команда5"},
        {"data08", {0x00, 0x20, 0x00, 0x00, 0x00}, GB_SIGNAL_COM6,  "Команда6"},
        {"data09", {0x00, 0x40, 0x00, 0x00, 0x00}, GB_SIGNAL_COM7,  "Команда7"},
        {"data10", {0x00, 0x80, 0x00, 0x00, 0x00}, GB_SIGNAL_COM8,  "Команда8"},
        {"data11", {0x00, 0x00, 0x01, 0x00, 0x00}, GB_SIGNAL_COM9,  "Команда9"},
        {"data12", {0x00, 0x00, 0x02, 0x00, 0x00}, GB_SIGNAL_COM10, "Команда10"},
        {"data13", {0x00, 0x00, 0x04, 0x00, 0x00}, GB_SIGNAL_COM11, "Команда11"},
        {"data14", {0x00, 0x00, 0x08, 0x00, 0x00}, GB_SIGNAL_COM12, "Команда12"},
        {"data15", {0x00, 0x00, 0x10, 0x00, 0x00}, GB_SIGNAL_COM13, "Команда13"},
        {"data16", {0x00, 0x00, 0x20, 0x00, 0x00}, GB_SIGNAL_COM14, "Команда14"},
        {"data17", {0x00, 0x00, 0x40, 0x00, 0x00}, GB_SIGNAL_COM15, "Команда15"},
        {"data18", {0x00, 0x00, 0x80, 0x00, 0x00}, GB_SIGNAL_COM16, "Команда16"},
        {"data19", {0x00, 0x00, 0x00, 0x01, 0x00}, GB_SIGNAL_COM17, "Команда17"},
        {"data20", {0x00, 0x00, 0x00, 0x02, 0x00}, GB_SIGNAL_COM18, "Команда18"},
        {"data21", {0x00, 0x00, 0x00, 0x04, 0x00}, GB_SIGNAL_COM19, "Команда19"},
        {"data22", {0x00, 0x00, 0x00, 0x08, 0x00}, GB_SIGNAL_COM20, "Команда20"},
        {"data23", {0x00, 0x00, 0x00, 0x10, 0x00}, GB_SIGNAL_COM21, "Команда21"},
        {"data24", {0x00, 0x00, 0x00, 0x20, 0x00}, GB_SIGNAL_COM22, "Команда22"},
        {"data25", {0x00, 0x00, 0x00, 0x40, 0x00}, GB_SIGNAL_COM23, "Команда23"},
        {"data26", {0x00, 0x00, 0x00, 0x80, 0x00}, GB_SIGNAL_COM24, "Команда24"},
        {"data27", {0x00, 0x00, 0x00, 0x00, 0x01}, GB_SIGNAL_COM25, "Команда25"},
        {"data28", {0x00, 0x00, 0x00, 0x00, 0x02}, GB_SIGNAL_COM26, "Команда26"},
        {"data29", {0x00, 0x00, 0x00, 0x00, 0x04}, GB_SIGNAL_COM27, "Команда27"},
        {"data30", {0x00, 0x00, 0x00, 0x00, 0x08}, GB_SIGNAL_COM28, "Команда28"},
        {"data31", {0x00, 0x00, 0x00, 0x00, 0x10}, GB_SIGNAL_COM29, "Команда29"},
        {"data32", {0x00, 0x00, 0x00, 0x00, 0x20}, GB_SIGNAL_COM30, "Команда30"},
        {"data33", {0x00, 0x00, 0x00, 0x00, 0x40}, GB_SIGNAL_COM31, "Команда31"},
        {"data34", {0x00, 0x00, 0x00, 0x00, 0x80}, GB_SIGNAL_COM32, "Команда32"},
        // Поиск ведется в порядке: кч1->кч2->ком1->...->ком32
        {"data35", {0xFC, 0x00, 0x00, 0x00, 0x00}, GB_SIGNAL_OFF,   "выкл."},
        {"data36", {0x03, 0xFF, 0xFF, 0xFF, 0xFF}, GB_SIGNAL_CF1,   "КЧ1"},
        {"data37", {0x02, 0xFF, 0xFF, 0xFF, 0xFF}, GB_SIGNAL_CF2,   "КЧ2"},
        {"data38", {0x00, 0xFF, 0xFF, 0xFF, 0xFF}, GB_SIGNAL_COM1,  "Команда1"},
        {"data39", {0x00, 0x00, 0x00, 0xC0, 0x0F}, GB_SIGNAL_COM23, "Команда23"},
        {"data40", {0x00, 0x00, 0x00, 0x00, 0x70}, GB_SIGNAL_COM29, "Команда29"}
    };

    for(auto &item : testdata) {
        SCOPED_TRACE(item.testName);

        mObj->setCurrentSignal(item.data, device, typeopto);
        eGB_TEST_SIGNAL signal = mObj->getCurrentSignal();

        ASSERT_EQ(item.resultSignal, signal);
        ASSERT_STREQ(item.resultText, fcTest1K400[signal]);
    }

    // Проверка для второго сигнала

    struct testData2_t {
        string testName;
        uint8_t data[10];
        eGB_TEST_SIGNAL resultSignal1;
        const char *resultText1;
        eGB_TEST_SIGNAL resultSignal2;
        const char *resultText2;
    };

    std::vector<testData2_t> testdata2 = {
        {"data00", {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
         GB_SIGNAL_CF2,   "КЧ2",       GB_SIGNAL_OFF,   "выкл."},
        {"data01", {0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00},
         GB_SIGNAL_COM1,  "Команда1",  GB_SIGNAL_CF1,   "КЧ1"},
        {"data02", {0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0xC0},
         GB_SIGNAL_COM16, "Команда16", GB_SIGNAL_COM1,  "Команда1"},
        {"data03", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0},
         GB_SIGNAL_OFF,   "выкл.",     GB_SIGNAL_COM31, "Команда31"}
    };

    for(auto &item : testdata2) {
        SCOPED_TRACE(item.testName);

        mObj->setCurrentSignal(item.data, device, typeopto);
        eGB_TEST_SIGNAL signal1 = mObj->getCurrentSignal();
        eGB_TEST_SIGNAL signal2 = mObj->getCurrentSignal2();

        ASSERT_EQ(item.resultSignal1, signal1);
        ASSERT_STREQ(item.resultText1, fcTest1K400[signal1]);

        ASSERT_EQ(item.resultSignal2, signal2);
        ASSERT_STREQ(item.resultText2, fcTest1K400[signal2]);
    }
}

//
TEST_F(TTest_Test, setCurrentSignal_Opto)
{
    eGB_TYPE_DEVICE device = AVANT_OPTO;
    eGB_TYPE_OPTO typeopto = TYPE_OPTO_STANDART;

    // РЗ1 - нет блокировки
    // РЗ2 - есть блокировка
    // Структура данных:
    // Байты: [b1] ... [b10]
    // бит:      8       7       6       5       4       3       2       1
    // b1:      [x]     [x]     [x]    [рз]     [x]     [x]    [x]     [кч]
    // b2:   [ком8]  [ком7]  [ком6]  [ком5]  [ком4]  [ком3]  [ком2]  [ком1]
    // b3:  [ком16] [ком15] [ком14] [ком13] [ком12] [ком11] [ком10]  [ком9]
    // b4:  [ком24] [ком23] [ком22] [ком21] [ком20] [ком19] [ком18] [ком17]
    // b5:  [ком32] [ком31] [ком30] [ком29] [ком28] [ком27] [ком26] [ком25]

    struct testData_t {
        string testName;
        uint8_t data[10];
        eGB_TEST_SIGNAL resultSignal;
        const char *resultText;
    };

    std::vector<testData_t> testdata = {
        {"data00", {0x00, 0x00, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_OFF,   "выкл."},
        {"data01", {0x01, 0x00, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_CS,    "КС"},
        {"data02", {0x10, 0x00, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_RZ,    "РЗ"},
        {"data03", {0x00, 0x01, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM1,  "Команда1"},
        {"data04", {0x00, 0x02, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM2,  "Команда2"},
        {"data05", {0x00, 0x04, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM3,  "Команда3"},
        {"data06", {0x00, 0x08, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM4,  "Команда4"},
        {"data07", {0x00, 0x10, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM5,  "Команда5"},
        {"data08", {0x00, 0x20, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM6,  "Команда6"},
        {"data09", {0x00, 0x40, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM7,  "Команда7"},
        {"data10", {0x00, 0x80, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM8,  "Команда8"},
        {"data11", {0x00, 0x00, 0x01, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM9,  "Команда9"},
        {"data12", {0x00, 0x00, 0x02, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM10, "Команда10"},
        {"data13", {0x00, 0x00, 0x04, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM11, "Команда11"},
        {"data14", {0x00, 0x00, 0x08, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM12, "Команда12"},
        {"data15", {0x00, 0x00, 0x10, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM13, "Команда13"},
        {"data16", {0x00, 0x00, 0x20, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM14, "Команда14"},
        {"data17", {0x00, 0x00, 0x40, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM15, "Команда15"},
        {"data18", {0x00, 0x00, 0x80, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM16, "Команда16"},
        {"data19", {0x00, 0x00, 0x00, 0x01, 0x00, 0xFF}, GB_SIGNAL_COM17, "Команда17"},
        {"data20", {0x00, 0x00, 0x00, 0x02, 0x00, 0xFF}, GB_SIGNAL_COM18, "Команда18"},
        {"data21", {0x00, 0x00, 0x00, 0x04, 0x00, 0xFF}, GB_SIGNAL_COM19, "Команда19"},
        {"data22", {0x00, 0x00, 0x00, 0x08, 0x00, 0xFF}, GB_SIGNAL_COM20, "Команда20"},
        {"data23", {0x00, 0x00, 0x00, 0x10, 0x00, 0xFF}, GB_SIGNAL_COM21, "Команда21"},
        {"data24", {0x00, 0x00, 0x00, 0x20, 0x00, 0xFF}, GB_SIGNAL_COM22, "Команда22"},
        {"data25", {0x00, 0x00, 0x00, 0x40, 0x00, 0xFF}, GB_SIGNAL_COM23, "Команда23"},
        {"data26", {0x00, 0x00, 0x00, 0x80, 0x00, 0xFF}, GB_SIGNAL_COM24, "Команда24"},
        {"data27", {0x00, 0x00, 0x00, 0x00, 0x01, 0xFF}, GB_SIGNAL_COM25, "Команда25"},
        {"data28", {0x00, 0x00, 0x00, 0x00, 0x02, 0xFF}, GB_SIGNAL_COM26, "Команда26"},
        {"data29", {0x00, 0x00, 0x00, 0x00, 0x04, 0xFF}, GB_SIGNAL_COM27, "Команда27"},
        {"data30", {0x00, 0x00, 0x00, 0x00, 0x08, 0xFF}, GB_SIGNAL_COM28, "Команда28"},
        {"data31", {0x00, 0x00, 0x00, 0x00, 0x10, 0xFF}, GB_SIGNAL_COM29, "Команда29"},
        {"data32", {0x00, 0x00, 0x00, 0x00, 0x20, 0xFF}, GB_SIGNAL_COM30, "Команда30"},
        {"data33", {0x00, 0x00, 0x00, 0x00, 0x40, 0xFF}, GB_SIGNAL_COM31, "Команда31"},
        {"data34", {0x00, 0x00, 0x00, 0x00, 0x80, 0xFF}, GB_SIGNAL_COM32, "Команда32"},
        // Поиск ведется в порядке: рз->кс->ком1->...->ком32
        {"data35", {0xEE, 0x00, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_OFF,   "выкл."},
        {"data36", {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, GB_SIGNAL_RZ,    "РЗ"},
        {"data37", {0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, GB_SIGNAL_CS,    "КС"},
        {"data38", {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, GB_SIGNAL_COM1,  "Команда1"},
        {"data39", {0x00, 0x00, 0x00, 0xC0, 0x0F, 0xFF}, GB_SIGNAL_COM23, "Команда23"},
        {"data40", {0x00, 0x00, 0x00, 0x00, 0x70, 0xFF}, GB_SIGNAL_COM29, "Команда29"}
    };

    for(auto &item : testdata) {
        SCOPED_TRACE(item.testName);

        mObj->setCurrentSignal(item.data, device, typeopto);
        eGB_TEST_SIGNAL signal = mObj->getCurrentSignal();

        ASSERT_EQ(item.resultSignal, signal);
        ASSERT_STREQ(item.resultText, fcTest1K400[signal]);

        ASSERT_EQ(GB_SIGNAL_MAX, mObj->getCurrentSignal2());
    }
}

//
TEST_F(TTest_Test, setCurrentSignal_OptoRingUni)
{
    eGB_TYPE_DEVICE device = AVANT_OPTO;
    eGB_TYPE_OPTO typeopto = TYPE_OPTO_RING_UNI;

    // РЗ1 - нет блокировки
    // РЗ2 - есть блокировка
    // Структура данных:
    // Байты: [b1] .. [b13]
    // бит:       8        7        6        5        4        3        2        1
    // b1:       [x]      [x]      [x]     [рз]      [x]      [x]      [x]     [кч]
    // b2:   [ком8A]  [ком7A]  [ком6A]  [ком5A]  [ком4A]  [ком3A]  [ком2A]  [ком1A]
    // b3:  [ком16A] [ком15A] [ком14A] [ком13A] [ком12A] [ком11A] [ком10A]  [ком9A]
    // b4:  [ком24A] [ком23A] [ком22A] [ком21A] [ком20A] [ком19A] [ком18A] [ком17A]
    // b5:  [ком32A] [ком31A] [ком30A] [ком29A] [ком28A] [ком27A] [ком26A] [ком25A]
    // b6:   [ком8B]  [ком7B]  [ком6B]  [ком5B]  [ком4B]  [ком3B]  [ком2B]  [ком1B]
    // b7:  [ком16B] [ком15B] [ком14B] [ком13B] [ком12B] [ком11B] [ком10B]  [ком9B]
    // b8:  [ком24B] [ком23B] [ком22B] [ком21B] [ком20B] [ком19B] [ком18B] [ком17B]
    // b9:  [ком32B] [ком31B] [ком30B] [ком29B] [ком28B] [ком27B] [ком26B] [ком25B]
    // b10:  [ком8C]  [ком7C]  [ком6C]  [ком5C]  [ком4C]  [ком3C]  [ком2C]  [ком1C]
    // b11: [ком16C] [ком15C] [ком14C] [ком13C] [ком12C] [ком11C] [ком10C]  [ком9C]
    // b12: [ком24C] [ком23C] [ком22C] [ком21C] [ком20C] [ком19C] [ком18C] [ком17C]
    // b13: [ком32C] [ком31C] [ком30C] [ком29C] [ком28C] [ком27C] [ком26C] [ком25C]

    struct testData_t {
        string testName;
        uint8_t data[14];
        eGB_TEST_SIGNAL resultSignal;
        const char *resultText;
    };

    std::vector<testData_t> testdata = {
        {"data00", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_OFF,   "выкл."},
        {"data01", {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_CS,    "КС"},
        {"data02", {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_RZ,    "РЗ"},
        {"data03", {0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM1A,   "Команда1A"},
        {"data04", {0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM2A,   "Команда2A"},
        {"data05", {0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM21A,  "Команда21A"},
        {"data06", {0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM32A,  "Команда32A"},
        {"data07", {0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM1B,   "Команда1B"},
        {"data08", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM10B,  "Команда10B"},
        {"data09", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM19B,  "Команда19B"},
        {"data10", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM32B,  "Команда32B"},
        {"data11", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM1C,   "Команда1C"},
        {"data12", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM11C,  "Команда11C"},
        {"data13", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0xFF},
         GB_SIGNAL_COM24C,  "Команда24C"},
        {"data14", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF},
         GB_SIGNAL_COM32C,  "Команда32C"},
        // Поиск ведется в порядке: рз->кс->ком1A->...->ком32C
        {"data15", {0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_OFF,     "выкл."},
        {"data16", {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
         GB_SIGNAL_RZ,    "РЗ"},
        {"data17", {0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
         GB_SIGNAL_CS,    "КС"},
        {"data18", {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
         GB_SIGNAL_COM1A,  "Команда1A"},
        {"data19", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
         GB_SIGNAL_COM14B, "Команда14B"},
        {"data20", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xFF},
         GB_SIGNAL_COM26C, "Команда26C"}
    };

    for(auto &item : testdata) {
        SCOPED_TRACE(item.testName);

        mObj->setCurrentSignal(item.data, device, typeopto);
        eGB_TEST_SIGNAL signal = mObj->getCurrentSignal();

        ASSERT_EQ(item.resultSignal, signal);
        ASSERT_STREQ(item.resultText, fcTest1K400[signal]);

        ASSERT_EQ(GB_SIGNAL_MAX, mObj->getCurrentSignal2());
    }
}

//
TEST_F(TTest_Test, getBytes)
{
    std::array<bool, GB_SIGNAL_MAX> signalTested;
    signalTested.fill({});

    struct result_t {
        uint8_t cf;
        uint8_t rz;
    };

    struct testData_t {
        string testName;
        result_t result;
        eGB_TEST_SIGNAL signal;
        const char *signalText;

    };

    // РЗ1 - нет блокировки
    // РЗ2 - есть блокировка
    // b1 : cf, 0 - нет, 1 - кч1, 2 - кч2, 3 - ком1, 4 - ком2, ... 6 - ком4
    // b2 : rz, 0 - нет, 1 - рз1, 2 - рз2
    testData_t testRzskHf[] {
        // РЗСК ВЧ
        // b1 : cf, 0 - нет, 1 - кч1, 2 - кч2, 3 - ком1, 4 - ком2, ... 10 - ком8
        // b2 : rz, 0 - нет, 1 - рз1, 2 - рз2
        {"Rzsk 00", {0x00, 0x00}, GB_SIGNAL_OFF,         "выкл."},
        {"Rzsk 01", {0x01, 0x01}, GB_SIGNAL_CF_NO_RZ,    "КЧ1"},
        {"Rzsk 02", {0x01, 0x02}, GB_SIGNAL_CF_RZ,       "КЧ1 с блок"},
        {"Rzsk 03", {0x02, 0x01}, GB_SIGNAL_CF2_NO_RZ,   "КЧ2"},
        {"Rzsk 04", {0x02, 0x02}, GB_SIGNAL_CF2_RZ,      "КЧ2 с блок"},
        {"Rzsk 05", {0x03, 0x01}, GB_SIGNAL_COM1_NO_RZ,  "Команда1"},
        {"Rzsk 06", {0x04, 0x01}, GB_SIGNAL_COM2_NO_RZ,  "Команда2"},
        {"Rzsk 07", {0x05, 0x01}, GB_SIGNAL_COM3_NO_RZ,  "Команда3"},
        {"Rzsk 08", {0x06, 0x01}, GB_SIGNAL_COM4_NO_RZ,  "Команда4"},
        {"Rzsk 09", {0x07, 0x01}, GB_SIGNAL_COM5_NO_RZ,  "Команда5"},
        {"Rzsk 10", {0x08, 0x01}, GB_SIGNAL_COM6_NO_RZ,  "Команда6"},
        {"Rzsk 11", {0x09, 0x01}, GB_SIGNAL_COM7_NO_RZ,  "Команда7"},
        {"Rzsk 12", {0x0A, 0x01}, GB_SIGNAL_COM8_NO_RZ,  "Команда8"},
        {"Rzsk 13", {0x03, 0x02}, GB_SIGNAL_COM1_RZ,     "Ком1+блок"},
        {"Rzsk 14", {0x04, 0x02}, GB_SIGNAL_COM2_RZ,     "Ком2+блок"},
        {"Rzsk 15", {0x05, 0x02}, GB_SIGNAL_COM3_RZ,     "Ком3+блок"},
        {"Rzsk 16", {0x06, 0x02}, GB_SIGNAL_COM4_RZ,     "Ком4+блок"},
        {"Rzsk 17", {0x07, 0x02}, GB_SIGNAL_COM5_RZ,     "Ком5+блок"},
        {"Rzsk 18", {0x08, 0x02}, GB_SIGNAL_COM6_RZ,     "Ком6+блок"},
        {"Rzsk 19", {0x09, 0x02}, GB_SIGNAL_COM7_RZ,     "Ком7+блок"},
        {"Rzsk 20", {0x0A, 0x02}, GB_SIGNAL_COM8_RZ,     "Ком8+блок"},
        // Р400м
        {"R400 00", {0x00, 0x00}, GB_SIGNAL_OFF,         "выкл."},
        {"R400 01", {0x00, 0x01}, GB_SIGNAL_RZ,          "РЗ"},
        {"R400 02", {0x01, 0x00}, GB_SIGNAL_CF,          "КЧ"},
        {"R400 03", {0x01, 0x00}, GB_SIGNAL_CF1,         "КЧ1"},
        {"R400 04", {0x02, 0x00}, GB_SIGNAL_CF2,         "КЧ2"},
        {"R400 05", {0x03, 0x00}, GB_SIGNAL_CF3,         "КЧ3"},
        {"R400 06", {0x04, 0x00}, GB_SIGNAL_CF4,         "КЧ4"},
        // К400
        {"K400 00", {0x00, 0x00}, GB_SIGNAL_OFF,         "выкл."},
        {"K400 01", {0x01, 0x00}, GB_SIGNAL_CF1,         "КЧ1"},
        {"K400 02", {0x02, 0x00}, GB_SIGNAL_CF2,         "КЧ2"},
        {"K400 03", {0x03, 0x00}, GB_SIGNAL_COM1,        "Команда1"},
        {"K400 04", {0x04, 0x00}, GB_SIGNAL_COM2,        "Команда2"},
        {"K400 05", {0x1B, 0x00}, GB_SIGNAL_COM25,       "Команда25"},
        {"K400 06", {0x22, 0x00}, GB_SIGNAL_COM32,       "Команда32"},
        // ВОЛС
        {"OPTO 00", {0x00, 0x00}, GB_SIGNAL_OFF,         "выкл."},
        {"OPTO 01", {0x01, 0x00}, GB_SIGNAL_CS,          "КС"},
        {"OPTO 02", {0x00, 0x01}, GB_SIGNAL_RZ,          "РЗ"},
        {"OPTO 03", {0x03, 0x00}, GB_SIGNAL_COM1,        "Команда1"},
        {"OPTO 04", {0x04, 0x00}, GB_SIGNAL_COM2,        "Команда2"},
        {"OPTO 05", {0x1B, 0x00}, GB_SIGNAL_COM25,       "Команда25"},
        {"OPTO 06", {0x22, 0x00}, GB_SIGNAL_COM32,       "Команда32"},
        // ВОЛС кольцо
        {"RING 00", {0x00, 0x00}, GB_SIGNAL_OFF,         "выкл."},
        {"RING 01", {0x01, 0x00}, GB_SIGNAL_CS,          "КС"},
        {"RING 02", {0x00, 0x01}, GB_SIGNAL_RZ,          "РЗ"},
        {"RING 03", {0x03, 0x00}, GB_SIGNAL_COM1A,       "Команда1A"},
        {"RING 04", {0x04, 0x00}, GB_SIGNAL_COM2A,       "Команда2A"},
        {"RING 05", {0x3B, 0x00}, GB_SIGNAL_COM25B,      "Команда25B"},
        {"RING 06", {0x62, 0x00}, GB_SIGNAL_COM32C,      "Команда32C"},
        // Команды имеющиеся на приеме, но их нет на передаче
        {"NULL 00", {0x00, 0x00}, GB_SIGNAL_CF_RZ_R400M,  "КЧ и РЗ"},
        {"NULL 01", {0x00, 0x00}, GB_SIGNAL_CF1_RZ_R400M, "КЧ1 и РЗ"},
        {"NULL 02", {0x00, 0x00}, GB_SIGNAL_CF2_RZ_R400M, "КЧ2 и РЗ"},
        {"NULL 03", {0x00, 0x00}, GB_SIGNAL_CF3_RZ_R400M, "КЧ3 и РЗ"},
        {"NULL 04", {0x00, 0x00}, GB_SIGNAL_CF4_RZ_R400M, "КЧ4 и РЗ"}
    };

    for(auto &item : testRzskHf) {
        SCOPED_TRACE(item.testName);

        uint8_t cf;
        uint8_t rz;
        mObj->getBytes(cf, rz, item.signal);

        ASSERT_EQ(item.result.cf, cf);
        ASSERT_EQ(item.result.rz, rz);
        ASSERT_STREQ(item.signalText, fcTest1K400[item.signal]);

        signalTested.at(item.signal) = true;
    }

    // проверка команд 1 - 32

    for(int i = 0; i < 32; i++) {
        SCOPED_TRACE("command " + std::to_string(i));

        eGB_TEST_SIGNAL signal;
        uint8_t cf = 0;
        uint8_t rz = 0;

        signal =  static_cast<eGB_TEST_SIGNAL> (GB_SIGNAL_COM1 + i);
        mObj->getBytes(cf, rz, signal);

        ASSERT_EQ(3 + i, cf);
        ASSERT_EQ(0U, rz);

        std::string text = "Команда" + std::to_string(i + 1);
        ASSERT_STREQ(text.c_str(), fcTest1K400[signal]);

        signalTested.at(signal) = true;
    }

    // проверка команд 1A - 32C

    for(int i = 0; i < 96; i++) {
        SCOPED_TRACE("command " + std::to_string(i));

        eGB_TEST_SIGNAL signal;
        uint8_t cf = 0;
        uint8_t rz = 0;

        signal =  static_cast<eGB_TEST_SIGNAL> (GB_SIGNAL_COM1A + i);
        mObj->getBytes(cf, rz, signal);

        ASSERT_EQ(3 + i, cf);
        ASSERT_EQ(0U, rz);

        std::string text = "Команда" + std::to_string(i%32 + 1);
        text += i < 32 ? "A" : i < 64 ? "B" : "C";
        ASSERT_STREQ(text.c_str(), fcTest1K400[signal]);

        signalTested.at(signal) = true;
    }

    // проверка на то что все сигналы в списке были протестированы

    int count = 0;
    for(size_t i = 0; i < signalTested.size(); i++) {
        SCOPED_TRACE("item " + std::to_string(i));

        EXPECT_TRUE(signalTested.at(i));

        if (signalTested.at(i)) {
            count++;
        }
    }

    ASSERT_EQ(GB_SIGNAL_MAX, count);
}

//
