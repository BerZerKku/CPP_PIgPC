#include <algorithm>
#include <cstdio>
#include <iostream>

#include "gtest/gtest.h"

using namespace std;

#include "menu/test.h"
#include "flash.h"
#include "glbDefine.h"

// ������ �������
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

    // ���������� ����������, �� ������� �����
    static void SetUpTestCase() { }

    // ���������� ����������, ����� ���������� �����
    static void TearDownTestCase() { }
};

//
TEST_F(TTest_Test, signal)
{
    ASSERT_EQ(1, mObj->getNumSignals());
    ASSERT_EQ(GB_SIGNAL_OFF, mObj->getCurrentSignal());
    ASSERT_EQ(GB_SIGNAL_OFF, mObj->getCurrentSignal2());

    ASSERT_EQ(100, MAX_NUM_TEST_SIGNAL);

    // �������� ���������� ��������

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

    // �������� ������� ������ ��������

    mObj->clear();

    for(uint8_t i = 0; i < MAX_NUM_TEST_SIGNAL; i++) {
        ASSERT_EQ(GB_SIGNAL_OFF, mObj->signalList[i]);
    }

    ASSERT_EQ(1, mObj->getNumSignals());

    // �������� ���������� ������������� ���������� ��������

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

    // ��1 - ��� ����������
    // ��2 - ���� ����������
    // ��������� ������:
    // �����: [b1] ... [b6]
    // ���:        4      3      2      1
    // b1/b4 :  [��2]  [��1]  [��2]  [��1]
    // b2/b5 : [���4] [���3] [���2] [���1]

    // �������� ������� �������

    struct testData_t {
        string testName;
        uint8_t data[6];
        eGB_TEST_SIGNAL resultSignal;
        const char *resultText;
    };

    std::vector<testData_t> testdata = {
        {"data00", {0x00, 0x00, 0xFF}, GB_SIGNAL_OFF,         "����."},
        {"data01", {0x05, 0x00, 0xFF}, GB_SIGNAL_CF_NO_RZ,    "��1"},
        {"data02", {0x09, 0x00, 0xFF}, GB_SIGNAL_CF_RZ,       "��1 � ����"},
        {"data03", {0x06, 0x00, 0xFF}, GB_SIGNAL_CF2_NO_RZ,   "��2"},
        {"data04", {0x0A, 0x00, 0xFF}, GB_SIGNAL_CF2_RZ,      "��2 � ����"},
        {"data05", {0x04, 0x01, 0xFF}, GB_SIGNAL_COM1_NO_RZ,  "�������1"},
        {"data06", {0x04, 0x02, 0xFF}, GB_SIGNAL_COM2_NO_RZ,  "�������2"},
        {"data07", {0x04, 0x04, 0xFF}, GB_SIGNAL_COM3_NO_RZ,  "�������3"},
        {"data08", {0x04, 0x08, 0xFF}, GB_SIGNAL_COM4_NO_RZ,  "�������4"},
        {"data09", {0x04, 0x10, 0xFF}, GB_SIGNAL_COM5_NO_RZ,  "�������5"},
        {"data10", {0x04, 0x20, 0xFF}, GB_SIGNAL_COM6_NO_RZ,  "�������6"},
        {"data11", {0x04, 0x40, 0xFF}, GB_SIGNAL_COM7_NO_RZ,  "�������7"},
        {"data12", {0x04, 0x80, 0xFF}, GB_SIGNAL_COM8_NO_RZ,  "�������8"},
        {"data13", {0x08, 0x01, 0xFF}, GB_SIGNAL_COM1_RZ,     "���1+����"},
        {"data14", {0x08, 0x02, 0xFF}, GB_SIGNAL_COM2_RZ,     "���2+����"},
        {"data15", {0x08, 0x04, 0xFF}, GB_SIGNAL_COM3_RZ,     "���3+����"},
        {"data16", {0x08, 0x08, 0xFF}, GB_SIGNAL_COM4_RZ,     "���4+����"},
        {"data17", {0x08, 0x10, 0xFF}, GB_SIGNAL_COM5_RZ,     "���5+����"},
        {"data18", {0x08, 0x20, 0xFF}, GB_SIGNAL_COM6_RZ,     "���6+����"},
        {"data19", {0x08, 0x40, 0xFF}, GB_SIGNAL_COM7_RZ,     "���7+����"},
        {"data20", {0x08, 0x80, 0xFF}, GB_SIGNAL_COM8_RZ,     "���8+����"},
        // ����� ���������� � ������ �������, ��������� ������ ���������
        {"data21", {0x04, 0x0F, 0xFF}, GB_SIGNAL_COM1_NO_RZ,  "�������1"},
        {"data22", {0x08, 0x0F, 0xFF}, GB_SIGNAL_COM1_RZ,     "���1+����"},
        {"data23", {0x0F, 0x0F, 0xFF}, GB_SIGNAL_COM1_NO_RZ,  "�������1"},
        // ����� ���������� � �������� ���������� � ��1 (�� ����)
        {"data24", {0x0C, 0x0E, 0xFF}, GB_SIGNAL_COM2_NO_RZ,  "�������2"},
        // ��� ������� ������������: ��1 + ��2, ��2 + ��2, ��1 + ��1, ��1 + ��1
        {"data25", {0xFF, 0x00, 0xFF}, GB_SIGNAL_CF2_NO_RZ,   "��2"},
        {"data26", {0xFB, 0x00, 0xFF}, GB_SIGNAL_CF2_RZ,      "��2 � ����"},
        {"data27", {0xFD, 0x00, 0xFF}, GB_SIGNAL_CF_NO_RZ,    "��1"},
        {"data28", {0xF0, 0x00, 0xFF}, GB_SIGNAL_OFF,         "����."}
    };

    for(auto &item : testdata) {
        SCOPED_TRACE(item.testName);

        mObj->setCurrentSignal(item.data, device, typeopto);
        eGB_TEST_SIGNAL signal = mObj->getCurrentSignal();

        ASSERT_EQ(item.resultSignal, signal);
        ASSERT_STREQ(item.resultText, fcTest1K400[signal]);
    }

    // �������� ��� ������� �������

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
         GB_SIGNAL_OFF,         "����.",    GB_SIGNAL_CF2_NO_RZ, "��2"},
        {"data01", {0x04, 0x01, 0x04, 0x08, 0x01, 0xFF},
         GB_SIGNAL_COM1_NO_RZ,  "�������1", GB_SIGNAL_COM1_RZ,   "���1+����"},
        {"data02", {0x04, 0x0F, 0x04, 0xFD, 0x00, 0xFF},
         GB_SIGNAL_COM1_NO_RZ,  "�������1", GB_SIGNAL_CF_NO_RZ,  "��1"}
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

    // ��1 - ��� ����������
    // ��2 - ���� ����������
    // ��������� ������:
    // �����: [b1] ... [b6]
    // ���:    5      4      3      2      1
    // b1:  [��2]  [��4]  [��3]  [��2]  [��1]

    struct testData_t {
        string testName;
        uint8_t data[6];
        eGB_TEST_SIGNAL resultSignal;
        const char *resultText;
    };

    std::vector<testData_t> testdata = {
        {"data00", {0x00, 0xFF, 0xFF}, GB_SIGNAL_OFF,          "����."},
        {"data01", {0x01, 0xFF, 0xFF}, GB_SIGNAL_CF1,          "��1"},
        {"data02", {0x02, 0xFF, 0xFF}, GB_SIGNAL_CF2,          "��2"},
        {"data03", {0x04, 0xFF, 0xFF}, GB_SIGNAL_CF3,          "��3"},
        {"data04", {0x08, 0xFF, 0xFF}, GB_SIGNAL_CF4,          "��4"},
        {"data05", {0x10, 0xFF, 0xFF}, GB_SIGNAL_RZ,           "��"},
        {"data06", {0x11, 0xFF, 0xFF}, GB_SIGNAL_CF1_RZ_R400M, "��1 � ��"},
        {"data07", {0x12, 0xFF, 0xFF}, GB_SIGNAL_CF2_RZ_R400M, "��2 � ��"},
        {"data08", {0x14, 0xFF, 0xFF}, GB_SIGNAL_CF3_RZ_R400M, "��3 � ��"},
        {"data09", {0x18, 0xFF, 0xFF}, GB_SIGNAL_CF4_RZ_R400M, "��4 � ��"},
        // ����� �� ���������� � 1
        {"data10", {0x0F, 0xFF, 0xFF}, GB_SIGNAL_CF1,          "��1"},
        {"data11", {0xFF, 0xFF, 0xFF}, GB_SIGNAL_CF1_RZ_R400M, "��1 � ��"}
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

    // ��1 - ��� ����������
    // ��2 - ���� ����������
    // ��������� ������:
    // �����: [b1] ... [b10]
    // ���:         8       7       6       5       4       3       2       1
    // b1/b6:      [x]     [x]     [x]     [x]     [x]     [x]   [��2]   [��1]
    // b2/b7:   [���8]  [���7]  [���6]  [���5]  [���4]  [���3]  [���2]  [���1]
    // b3/b8:  [���16] [���15] [���14] [���13] [���12] [���11] [���10]  [���9]
    // b4/b9:  [���24] [���23] [���22] [���21] [���20] [���19] [���18] [���17]
    // b5/b10: [���32] [���31] [���30] [���29] [���28] [���27] [���26] [���25]

    // �������� ������� �������

    struct testData_t {
        string testName;
        uint8_t data[5];
        eGB_TEST_SIGNAL resultSignal;
        const char *resultText;
    };

    std::vector<testData_t> testdata = {
        {"data00", {0x00, 0x00, 0x00, 0x00, 0x00}, GB_SIGNAL_OFF,   "����."},
        {"data01", {0x01, 0x00, 0x00, 0x00, 0x00}, GB_SIGNAL_CF1,   "��1"},
        {"data02", {0x02, 0x00, 0x00, 0x00, 0x00}, GB_SIGNAL_CF2,   "��2"},
        {"data03", {0x00, 0x01, 0x00, 0x00, 0x00}, GB_SIGNAL_COM1,  "�������1"},
        {"data04", {0x00, 0x02, 0x00, 0x00, 0x00}, GB_SIGNAL_COM2,  "�������2"},
        {"data05", {0x00, 0x04, 0x00, 0x00, 0x00}, GB_SIGNAL_COM3,  "�������3"},
        {"data06", {0x00, 0x08, 0x00, 0x00, 0x00}, GB_SIGNAL_COM4,  "�������4"},
        {"data07", {0x00, 0x10, 0x00, 0x00, 0x00}, GB_SIGNAL_COM5,  "�������5"},
        {"data08", {0x00, 0x20, 0x00, 0x00, 0x00}, GB_SIGNAL_COM6,  "�������6"},
        {"data09", {0x00, 0x40, 0x00, 0x00, 0x00}, GB_SIGNAL_COM7,  "�������7"},
        {"data10", {0x00, 0x80, 0x00, 0x00, 0x00}, GB_SIGNAL_COM8,  "�������8"},
        {"data11", {0x00, 0x00, 0x01, 0x00, 0x00}, GB_SIGNAL_COM9,  "�������9"},
        {"data12", {0x00, 0x00, 0x02, 0x00, 0x00}, GB_SIGNAL_COM10, "�������10"},
        {"data13", {0x00, 0x00, 0x04, 0x00, 0x00}, GB_SIGNAL_COM11, "�������11"},
        {"data14", {0x00, 0x00, 0x08, 0x00, 0x00}, GB_SIGNAL_COM12, "�������12"},
        {"data15", {0x00, 0x00, 0x10, 0x00, 0x00}, GB_SIGNAL_COM13, "�������13"},
        {"data16", {0x00, 0x00, 0x20, 0x00, 0x00}, GB_SIGNAL_COM14, "�������14"},
        {"data17", {0x00, 0x00, 0x40, 0x00, 0x00}, GB_SIGNAL_COM15, "�������15"},
        {"data18", {0x00, 0x00, 0x80, 0x00, 0x00}, GB_SIGNAL_COM16, "�������16"},
        {"data19", {0x00, 0x00, 0x00, 0x01, 0x00}, GB_SIGNAL_COM17, "�������17"},
        {"data20", {0x00, 0x00, 0x00, 0x02, 0x00}, GB_SIGNAL_COM18, "�������18"},
        {"data21", {0x00, 0x00, 0x00, 0x04, 0x00}, GB_SIGNAL_COM19, "�������19"},
        {"data22", {0x00, 0x00, 0x00, 0x08, 0x00}, GB_SIGNAL_COM20, "�������20"},
        {"data23", {0x00, 0x00, 0x00, 0x10, 0x00}, GB_SIGNAL_COM21, "�������21"},
        {"data24", {0x00, 0x00, 0x00, 0x20, 0x00}, GB_SIGNAL_COM22, "�������22"},
        {"data25", {0x00, 0x00, 0x00, 0x40, 0x00}, GB_SIGNAL_COM23, "�������23"},
        {"data26", {0x00, 0x00, 0x00, 0x80, 0x00}, GB_SIGNAL_COM24, "�������24"},
        {"data27", {0x00, 0x00, 0x00, 0x00, 0x01}, GB_SIGNAL_COM25, "�������25"},
        {"data28", {0x00, 0x00, 0x00, 0x00, 0x02}, GB_SIGNAL_COM26, "�������26"},
        {"data29", {0x00, 0x00, 0x00, 0x00, 0x04}, GB_SIGNAL_COM27, "�������27"},
        {"data30", {0x00, 0x00, 0x00, 0x00, 0x08}, GB_SIGNAL_COM28, "�������28"},
        {"data31", {0x00, 0x00, 0x00, 0x00, 0x10}, GB_SIGNAL_COM29, "�������29"},
        {"data32", {0x00, 0x00, 0x00, 0x00, 0x20}, GB_SIGNAL_COM30, "�������30"},
        {"data33", {0x00, 0x00, 0x00, 0x00, 0x40}, GB_SIGNAL_COM31, "�������31"},
        {"data34", {0x00, 0x00, 0x00, 0x00, 0x80}, GB_SIGNAL_COM32, "�������32"},
        // ����� ������� � �������: ��1->��2->���1->...->���32
        {"data35", {0xFC, 0x00, 0x00, 0x00, 0x00}, GB_SIGNAL_OFF,   "����."},
        {"data36", {0x03, 0xFF, 0xFF, 0xFF, 0xFF}, GB_SIGNAL_CF1,   "��1"},
        {"data37", {0x02, 0xFF, 0xFF, 0xFF, 0xFF}, GB_SIGNAL_CF2,   "��2"},
        {"data38", {0x00, 0xFF, 0xFF, 0xFF, 0xFF}, GB_SIGNAL_COM1,  "�������1"},
        {"data39", {0x00, 0x00, 0x00, 0xC0, 0x0F}, GB_SIGNAL_COM23, "�������23"},
        {"data40", {0x00, 0x00, 0x00, 0x00, 0x70}, GB_SIGNAL_COM29, "�������29"}
    };

    for(auto &item : testdata) {
        SCOPED_TRACE(item.testName);

        mObj->setCurrentSignal(item.data, device, typeopto);
        eGB_TEST_SIGNAL signal = mObj->getCurrentSignal();

        ASSERT_EQ(item.resultSignal, signal);
        ASSERT_STREQ(item.resultText, fcTest1K400[signal]);
    }

    // �������� ��� ������� �������

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
         GB_SIGNAL_CF2,   "��2",       GB_SIGNAL_OFF,   "����."},
        {"data01", {0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00},
         GB_SIGNAL_COM1,  "�������1",  GB_SIGNAL_CF1,   "��1"},
        {"data02", {0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0xC0},
         GB_SIGNAL_COM16, "�������16", GB_SIGNAL_COM1,  "�������1"},
        {"data03", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0},
         GB_SIGNAL_OFF,   "����.",     GB_SIGNAL_COM31, "�������31"}
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

    // ��1 - ��� ����������
    // ��2 - ���� ����������
    // ��������� ������:
    // �����: [b1] ... [b10]
    // ���:      8       7       6       5       4       3       2       1
    // b1:      [x]     [x]     [x]    [��]     [x]     [x]    [x]     [��]
    // b2:   [���8]  [���7]  [���6]  [���5]  [���4]  [���3]  [���2]  [���1]
    // b3:  [���16] [���15] [���14] [���13] [���12] [���11] [���10]  [���9]
    // b4:  [���24] [���23] [���22] [���21] [���20] [���19] [���18] [���17]
    // b5:  [���32] [���31] [���30] [���29] [���28] [���27] [���26] [���25]

    struct testData_t {
        string testName;
        uint8_t data[10];
        eGB_TEST_SIGNAL resultSignal;
        const char *resultText;
    };

    std::vector<testData_t> testdata = {
        {"data00", {0x00, 0x00, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_OFF,   "����."},
        {"data01", {0x01, 0x00, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_CS,    "��"},
        {"data02", {0x10, 0x00, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_RZ,    "��"},
        {"data03", {0x00, 0x01, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM1,  "�������1"},
        {"data04", {0x00, 0x02, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM2,  "�������2"},
        {"data05", {0x00, 0x04, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM3,  "�������3"},
        {"data06", {0x00, 0x08, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM4,  "�������4"},
        {"data07", {0x00, 0x10, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM5,  "�������5"},
        {"data08", {0x00, 0x20, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM6,  "�������6"},
        {"data09", {0x00, 0x40, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM7,  "�������7"},
        {"data10", {0x00, 0x80, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM8,  "�������8"},
        {"data11", {0x00, 0x00, 0x01, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM9,  "�������9"},
        {"data12", {0x00, 0x00, 0x02, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM10, "�������10"},
        {"data13", {0x00, 0x00, 0x04, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM11, "�������11"},
        {"data14", {0x00, 0x00, 0x08, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM12, "�������12"},
        {"data15", {0x00, 0x00, 0x10, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM13, "�������13"},
        {"data16", {0x00, 0x00, 0x20, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM14, "�������14"},
        {"data17", {0x00, 0x00, 0x40, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM15, "�������15"},
        {"data18", {0x00, 0x00, 0x80, 0x00, 0x00, 0xFF}, GB_SIGNAL_COM16, "�������16"},
        {"data19", {0x00, 0x00, 0x00, 0x01, 0x00, 0xFF}, GB_SIGNAL_COM17, "�������17"},
        {"data20", {0x00, 0x00, 0x00, 0x02, 0x00, 0xFF}, GB_SIGNAL_COM18, "�������18"},
        {"data21", {0x00, 0x00, 0x00, 0x04, 0x00, 0xFF}, GB_SIGNAL_COM19, "�������19"},
        {"data22", {0x00, 0x00, 0x00, 0x08, 0x00, 0xFF}, GB_SIGNAL_COM20, "�������20"},
        {"data23", {0x00, 0x00, 0x00, 0x10, 0x00, 0xFF}, GB_SIGNAL_COM21, "�������21"},
        {"data24", {0x00, 0x00, 0x00, 0x20, 0x00, 0xFF}, GB_SIGNAL_COM22, "�������22"},
        {"data25", {0x00, 0x00, 0x00, 0x40, 0x00, 0xFF}, GB_SIGNAL_COM23, "�������23"},
        {"data26", {0x00, 0x00, 0x00, 0x80, 0x00, 0xFF}, GB_SIGNAL_COM24, "�������24"},
        {"data27", {0x00, 0x00, 0x00, 0x00, 0x01, 0xFF}, GB_SIGNAL_COM25, "�������25"},
        {"data28", {0x00, 0x00, 0x00, 0x00, 0x02, 0xFF}, GB_SIGNAL_COM26, "�������26"},
        {"data29", {0x00, 0x00, 0x00, 0x00, 0x04, 0xFF}, GB_SIGNAL_COM27, "�������27"},
        {"data30", {0x00, 0x00, 0x00, 0x00, 0x08, 0xFF}, GB_SIGNAL_COM28, "�������28"},
        {"data31", {0x00, 0x00, 0x00, 0x00, 0x10, 0xFF}, GB_SIGNAL_COM29, "�������29"},
        {"data32", {0x00, 0x00, 0x00, 0x00, 0x20, 0xFF}, GB_SIGNAL_COM30, "�������30"},
        {"data33", {0x00, 0x00, 0x00, 0x00, 0x40, 0xFF}, GB_SIGNAL_COM31, "�������31"},
        {"data34", {0x00, 0x00, 0x00, 0x00, 0x80, 0xFF}, GB_SIGNAL_COM32, "�������32"},
        // ����� ������� � �������: ��->��->���1->...->���32
        {"data35", {0xEE, 0x00, 0x00, 0x00, 0x00, 0xFF}, GB_SIGNAL_OFF,   "����."},
        {"data36", {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, GB_SIGNAL_RZ,    "��"},
        {"data37", {0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, GB_SIGNAL_CS,    "��"},
        {"data38", {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, GB_SIGNAL_COM1,  "�������1"},
        {"data39", {0x00, 0x00, 0x00, 0xC0, 0x0F, 0xFF}, GB_SIGNAL_COM23, "�������23"},
        {"data40", {0x00, 0x00, 0x00, 0x00, 0x70, 0xFF}, GB_SIGNAL_COM29, "�������29"}
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

    // ��1 - ��� ����������
    // ��2 - ���� ����������
    // ��������� ������:
    // �����: [b1] .. [b13]
    // ���:       8        7        6        5        4        3        2        1
    // b1:       [x]      [x]      [x]     [��]      [x]      [x]      [x]     [��]
    // b2:   [���8A]  [���7A]  [���6A]  [���5A]  [���4A]  [���3A]  [���2A]  [���1A]
    // b3:  [���16A] [���15A] [���14A] [���13A] [���12A] [���11A] [���10A]  [���9A]
    // b4:  [���24A] [���23A] [���22A] [���21A] [���20A] [���19A] [���18A] [���17A]
    // b5:  [���32A] [���31A] [���30A] [���29A] [���28A] [���27A] [���26A] [���25A]
    // b6:   [���8B]  [���7B]  [���6B]  [���5B]  [���4B]  [���3B]  [���2B]  [���1B]
    // b7:  [���16B] [���15B] [���14B] [���13B] [���12B] [���11B] [���10B]  [���9B]
    // b8:  [���24B] [���23B] [���22B] [���21B] [���20B] [���19B] [���18B] [���17B]
    // b9:  [���32B] [���31B] [���30B] [���29B] [���28B] [���27B] [���26B] [���25B]
    // b10:  [���8C]  [���7C]  [���6C]  [���5C]  [���4C]  [���3C]  [���2C]  [���1C]
    // b11: [���16C] [���15C] [���14C] [���13C] [���12C] [���11C] [���10C]  [���9C]
    // b12: [���24C] [���23C] [���22C] [���21C] [���20C] [���19C] [���18C] [���17C]
    // b13: [���32C] [���31C] [���30C] [���29C] [���28C] [���27C] [���26C] [���25C]

    struct testData_t {
        string testName;
        uint8_t data[14];
        eGB_TEST_SIGNAL resultSignal;
        const char *resultText;
    };

    std::vector<testData_t> testdata = {
        {"data00", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_OFF,   "����."},
        {"data01", {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_CS,    "��"},
        {"data02", {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_RZ,    "��"},
        {"data03", {0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM1A,   "�������1A"},
        {"data04", {0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM2A,   "�������2A"},
        {"data05", {0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM21A,  "�������21A"},
        {"data06", {0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM32A,  "�������32A"},
        {"data07", {0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM1B,   "�������1B"},
        {"data08", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM10B,  "�������10B"},
        {"data09", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM19B,  "�������19B"},
        {"data10", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM32B,  "�������32B"},
        {"data11", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM1C,   "�������1C"},
        {"data12", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0xFF},
         GB_SIGNAL_COM11C,  "�������11C"},
        {"data13", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0xFF},
         GB_SIGNAL_COM24C,  "�������24C"},
        {"data14", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF},
         GB_SIGNAL_COM32C,  "�������32C"},
        // ����� ������� � �������: ��->��->���1A->...->���32C
        {"data15", {0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
         GB_SIGNAL_OFF,     "����."},
        {"data16", {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
         GB_SIGNAL_RZ,    "��"},
        {"data17", {0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
         GB_SIGNAL_CS,    "��"},
        {"data18", {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
         GB_SIGNAL_COM1A,  "�������1A"},
        {"data19", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
         GB_SIGNAL_COM14B, "�������14B"},
        {"data20", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xFF},
         GB_SIGNAL_COM26C, "�������26C"}
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

    // ��1 - ��� ����������
    // ��2 - ���� ����������
    // b1 : cf, 0 - ���, 1 - ��1, 2 - ��2, 3 - ���1, 4 - ���2, ... 6 - ���4
    // b2 : rz, 0 - ���, 1 - ��1, 2 - ��2
    testData_t testRzskHf[] {
        // ���� ��
        // b1 : cf, 0 - ���, 1 - ��1, 2 - ��2, 3 - ���1, 4 - ���2, ... 10 - ���8
        // b2 : rz, 0 - ���, 1 - ��1, 2 - ��2
        {"Rzsk 00", {0x00, 0x00}, GB_SIGNAL_OFF,         "����."},
        {"Rzsk 01", {0x01, 0x01}, GB_SIGNAL_CF_NO_RZ,    "��1"},
        {"Rzsk 02", {0x01, 0x02}, GB_SIGNAL_CF_RZ,       "��1 � ����"},
        {"Rzsk 03", {0x02, 0x01}, GB_SIGNAL_CF2_NO_RZ,   "��2"},
        {"Rzsk 04", {0x02, 0x02}, GB_SIGNAL_CF2_RZ,      "��2 � ����"},
        {"Rzsk 05", {0x03, 0x01}, GB_SIGNAL_COM1_NO_RZ,  "�������1"},
        {"Rzsk 06", {0x04, 0x01}, GB_SIGNAL_COM2_NO_RZ,  "�������2"},
        {"Rzsk 07", {0x05, 0x01}, GB_SIGNAL_COM3_NO_RZ,  "�������3"},
        {"Rzsk 08", {0x06, 0x01}, GB_SIGNAL_COM4_NO_RZ,  "�������4"},
        {"Rzsk 09", {0x07, 0x01}, GB_SIGNAL_COM5_NO_RZ,  "�������5"},
        {"Rzsk 10", {0x08, 0x01}, GB_SIGNAL_COM6_NO_RZ,  "�������6"},
        {"Rzsk 11", {0x09, 0x01}, GB_SIGNAL_COM7_NO_RZ,  "�������7"},
        {"Rzsk 12", {0x0A, 0x01}, GB_SIGNAL_COM8_NO_RZ,  "�������8"},
        {"Rzsk 13", {0x03, 0x02}, GB_SIGNAL_COM1_RZ,     "���1+����"},
        {"Rzsk 14", {0x04, 0x02}, GB_SIGNAL_COM2_RZ,     "���2+����"},
        {"Rzsk 15", {0x05, 0x02}, GB_SIGNAL_COM3_RZ,     "���3+����"},
        {"Rzsk 16", {0x06, 0x02}, GB_SIGNAL_COM4_RZ,     "���4+����"},
        {"Rzsk 17", {0x07, 0x02}, GB_SIGNAL_COM5_RZ,     "���5+����"},
        {"Rzsk 18", {0x08, 0x02}, GB_SIGNAL_COM6_RZ,     "���6+����"},
        {"Rzsk 19", {0x09, 0x02}, GB_SIGNAL_COM7_RZ,     "���7+����"},
        {"Rzsk 20", {0x0A, 0x02}, GB_SIGNAL_COM8_RZ,     "���8+����"},
        // �400�
        {"R400 00", {0x00, 0x00}, GB_SIGNAL_OFF,         "����."},
        {"R400 01", {0x00, 0x01}, GB_SIGNAL_RZ,          "��"},
        {"R400 02", {0x01, 0x00}, GB_SIGNAL_CF,          "��"},
        {"R400 03", {0x01, 0x00}, GB_SIGNAL_CF1,         "��1"},
        {"R400 04", {0x02, 0x00}, GB_SIGNAL_CF2,         "��2"},
        {"R400 05", {0x03, 0x00}, GB_SIGNAL_CF3,         "��3"},
        {"R400 06", {0x04, 0x00}, GB_SIGNAL_CF4,         "��4"},
        // �400
        {"K400 00", {0x00, 0x00}, GB_SIGNAL_OFF,         "����."},
        {"K400 01", {0x01, 0x00}, GB_SIGNAL_CF1,         "��1"},
        {"K400 02", {0x02, 0x00}, GB_SIGNAL_CF2,         "��2"},
        {"K400 03", {0x03, 0x00}, GB_SIGNAL_COM1,        "�������1"},
        {"K400 04", {0x04, 0x00}, GB_SIGNAL_COM2,        "�������2"},
        {"K400 05", {0x1B, 0x00}, GB_SIGNAL_COM25,       "�������25"},
        {"K400 06", {0x22, 0x00}, GB_SIGNAL_COM32,       "�������32"},
        // ����
        {"OPTO 00", {0x00, 0x00}, GB_SIGNAL_OFF,         "����."},
        {"OPTO 01", {0x01, 0x00}, GB_SIGNAL_CS,          "��"},
        {"OPTO 02", {0x00, 0x01}, GB_SIGNAL_RZ,          "��"},
        {"OPTO 03", {0x03, 0x00}, GB_SIGNAL_COM1,        "�������1"},
        {"OPTO 04", {0x04, 0x00}, GB_SIGNAL_COM2,        "�������2"},
        {"OPTO 05", {0x1B, 0x00}, GB_SIGNAL_COM25,       "�������25"},
        {"OPTO 06", {0x22, 0x00}, GB_SIGNAL_COM32,       "�������32"},
        // ���� ������
        {"RING 00", {0x00, 0x00}, GB_SIGNAL_OFF,         "����."},
        {"RING 01", {0x01, 0x00}, GB_SIGNAL_CS,          "��"},
        {"RING 02", {0x00, 0x01}, GB_SIGNAL_RZ,          "��"},
        {"RING 03", {0x03, 0x00}, GB_SIGNAL_COM1A,       "�������1A"},
        {"RING 04", {0x04, 0x00}, GB_SIGNAL_COM2A,       "�������2A"},
        {"RING 05", {0x3B, 0x00}, GB_SIGNAL_COM25B,      "�������25B"},
        {"RING 06", {0x62, 0x00}, GB_SIGNAL_COM32C,      "�������32C"},
        // ������� ��������� �� ������, �� �� ��� �� ��������
        {"NULL 00", {0x00, 0x00}, GB_SIGNAL_CF_RZ_R400M,  "�� � ��"},
        {"NULL 01", {0x00, 0x00}, GB_SIGNAL_CF1_RZ_R400M, "��1 � ��"},
        {"NULL 02", {0x00, 0x00}, GB_SIGNAL_CF2_RZ_R400M, "��2 � ��"},
        {"NULL 03", {0x00, 0x00}, GB_SIGNAL_CF3_RZ_R400M, "��3 � ��"},
        {"NULL 04", {0x00, 0x00}, GB_SIGNAL_CF4_RZ_R400M, "��4 � ��"}
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

    // �������� ������ 1 - 32

    for(int i = 0; i < 32; i++) {
        SCOPED_TRACE("command " + std::to_string(i));

        eGB_TEST_SIGNAL signal;
        uint8_t cf = 0;
        uint8_t rz = 0;

        signal =  static_cast<eGB_TEST_SIGNAL> (GB_SIGNAL_COM1 + i);
        mObj->getBytes(cf, rz, signal);

        ASSERT_EQ(3 + i, cf);
        ASSERT_EQ(0U, rz);

        std::string text = "�������" + std::to_string(i + 1);
        ASSERT_STREQ(text.c_str(), fcTest1K400[signal]);

        signalTested.at(signal) = true;
    }

    // �������� ������ 1A - 32C

    for(int i = 0; i < 96; i++) {
        SCOPED_TRACE("command " + std::to_string(i));

        eGB_TEST_SIGNAL signal;
        uint8_t cf = 0;
        uint8_t rz = 0;

        signal =  static_cast<eGB_TEST_SIGNAL> (GB_SIGNAL_COM1A + i);
        mObj->getBytes(cf, rz, signal);

        ASSERT_EQ(3 + i, cf);
        ASSERT_EQ(0U, rz);

        std::string text = "�������" + std::to_string(i%32 + 1);
        text += i < 32 ? "A" : i < 64 ? "B" : "C";
        ASSERT_STREQ(text.c_str(), fcTest1K400[signal]);

        signalTested.at(signal) = true;
    }

    // �������� �� �� ��� ��� ������� � ������ ���� ��������������

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
