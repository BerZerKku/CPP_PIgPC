#include <algorithm>
#include <cstdio>
#include <iostream>

#include "gtest/gtest.h"

using namespace std;

#define TEST_FRIENDS                  \
    friend class clProtocolBspS_Test; \
    FRIEND_TEST(clProtocolBspS_Test, getLocalParam);

#include "protocols/standart/protocolBspS.h"

// размер массива
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class clProtocolBspS_Test : public ::testing::Test
{
public:
    static const uint16_t SIZE_BUF = 255;  // размер буфера данных
    uint8_t               buf[SIZE_BUF];   // буфер данных
    stGBparam             m_params;        // параметры

    clProtocolBspS *m_obj;

    clProtocolBspS_Test()           = default;
    ~clProtocolBspS_Test() override = default;

protected:
    virtual void SetUp() override { m_obj = new clProtocolBspS(buf, SIZE_BUF, &m_params); }

    virtual void TearDown() override { delete m_obj; }

    // вызывается однократно, до первого теста
    static void SetUpTestCase() { }

    // вызывается однократно, после последнего теста
    static void TearDownTestCase() { }
};

//
TEST_F(clProtocolBspS_Test, getLocalParam)
{
    eGB_PARAM pn;

    // Загрубление чувствительности ПРМ

    pn = GB_PARAM_IN_DEC;
    m_params.local.clearParams();
    m_params.local.addParam(pn);
    m_params.local.setNumDevices(3);
    ASSERT_EQ(pn, m_params.local.getParam());

    buf[B1] = 10;
    buf[B2] = 11;
    buf[B3] = 12;
    buf[B4] = 13;

    ASSERT_TRUE(m_obj->getLocalParam(getCom(pn)));
    ASSERT_EQ(buf[B2], m_params.local.getValue());

    m_params.local.nextSameParam();

    ASSERT_TRUE(m_obj->getLocalParam(getCom(pn)));
    ASSERT_EQ(buf[B3], m_params.local.getValue());

    // Частота

    uint16_t freq;
    pn = GB_PARAM_FREQ;
    m_params.local.clearParams();
    m_params.local.addParam(pn);
    ASSERT_EQ(pn, m_params.local.getParam());

    freq    = 377;
    buf[B1] = static_cast<uint8_t>(freq >> 8);
    buf[B2] = static_cast<uint8_t>(freq);

    ASSERT_TRUE(m_obj->getLocalParam(getCom(pn)));
    ASSERT_EQ(freq, m_params.local.getValue());

    // Частота ПРД

    pn = GB_PARAM_FREQ_PRD;
    m_params.local.clearParams();
    m_params.local.addParam(pn);
    ASSERT_EQ(pn, m_params.local.getParam());

    freq    = 415;
    buf[B1] = static_cast<uint8_t>(freq >> 8);
    buf[B2] = static_cast<uint8_t>(freq);

    ASSERT_TRUE(m_obj->getLocalParam(getCom(pn)));
    ASSERT_EQ(freq, m_params.local.getValue());

    // Частота ПРМ

    pn = GB_PARAM_FREQ_PRM;
    m_params.local.clearParams();
    m_params.local.addParam(pn);
    ASSERT_EQ(pn, m_params.local.getParam());

    freq    = 917;
    buf[B3] = static_cast<uint8_t>(freq >> 8);
    buf[B4] = static_cast<uint8_t>(freq);

    ASSERT_TRUE(m_obj->getLocalParam(getCom(pn)));
    ASSERT_EQ(freq, m_params.local.getValue());

    // Коррекция напряжения

    int16_t cor_u;
    pn = GB_PARAM_COR_U;
    m_params.local.clearParams();
    m_params.local.addParam(pn);
    ASSERT_EQ(pn, m_params.local.getParam());

    cor_u   = 599;
    buf[B1] = static_cast<uint8_t>(cor_u / 10);
    buf[B2] = static_cast<uint8_t>((cor_u % 10) * 10);

    ASSERT_TRUE(m_obj->getLocalParam(getCom(pn)));
    ASSERT_EQ(cor_u, m_params.local.getValue());

    cor_u   = -599;
    buf[B1] = static_cast<uint8_t>(cor_u / 10);
    buf[B2] = static_cast<uint8_t>((cor_u % 10) * 10);

    ASSERT_TRUE(m_obj->getLocalParam(getCom(pn)));
    ASSERT_EQ(cor_u, m_params.local.getValue());

    // Коррекция тока

    int16_t cor_i;
    pn = GB_PARAM_COR_I;
    m_params.local.clearParams();
    m_params.local.addParam(pn);

    ASSERT_EQ(pn, m_params.local.getParam());

    cor_i   = 999;
    buf[B3] = static_cast<uint8_t>(cor_i >> 8);
    buf[B4] = static_cast<uint8_t>(cor_i);

    ASSERT_TRUE(m_obj->getLocalParam(getCom(pn)));
    ASSERT_EQ(cor_i, m_params.local.getValue());

    cor_i   = -999;
    buf[B3] = static_cast<uint8_t>(cor_i >> 8);
    buf[B4] = static_cast<uint8_t>(cor_i);

    ASSERT_TRUE(m_obj->getLocalParam(getCom(pn)));
    ASSERT_EQ(cor_i, m_params.local.getValue());

    // Битовые значения

    pn = GB_PARAM_PRD_COM_BLOCK;
    m_params.local.clearParams();
    m_params.local.addParam(pn);
    m_params.local.setNumComPrd(32);

    ASSERT_EQ(Param::PARAM_BITES, getParamType(pn));
    ASSERT_EQ(pn, m_params.local.getParam());

    buf[B1] = 0x05;
    buf[B2] = 0x02;
    buf[B3] = 0x04;
    buf[B4] = 0x80;

    ASSERT_EQ(1, m_params.local.getNumOfCurrSameParam());
    ASSERT_TRUE(m_obj->getLocalParam(getCom(pn)));
    ASSERT_EQ(1, m_params.local.getValue());

    m_params.local.nextSameParam();

    ASSERT_EQ(2, m_params.local.getNumOfCurrSameParam());
    ASSERT_TRUE(m_obj->getLocalParam(getCom(pn)));
    ASSERT_EQ(int(0), m_params.local.getValue());

    m_params.local.nextSameParam();

    ASSERT_EQ(3, m_params.local.getNumOfCurrSameParam());
    ASSERT_TRUE(m_obj->getLocalParam(getCom(pn)));
    ASSERT_EQ(1, m_params.local.getValue());

    for (int i = 4; i <= 18; i++)
    {
        m_params.local.nextSameParam();
    }

    ASSERT_EQ(18, m_params.local.getNumOfCurrSameParam());
    ASSERT_TRUE(m_obj->getLocalParam(getCom(pn)));
    ASSERT_EQ(int(0), m_params.local.getValue());

    m_params.local.nextSameParam();

    ASSERT_EQ(19, m_params.local.getNumOfCurrSameParam());
    ASSERT_TRUE(m_obj->getLocalParam(getCom(pn)));
    ASSERT_EQ(1, m_params.local.getValue());

    // Остальные параметры, т.е. простые однобайтные одиночные и повторяющиеся

    pn = GB_PARAM_PRM_TIME_OFF;
    m_params.local.clearParams();
    m_params.local.addParam(pn);
    m_params.local.setNumComPrm(32);

    ASSERT_FALSE(Param::PARAM_BITES == getParamType(pn));
    ASSERT_FALSE(Param::DEPEND_SAME_NO == getDependSame(pn));
    ASSERT_EQ(pn, m_params.local.getParam());

    buf[B1] = 5;
    buf[B2] = 15;
    buf[B3] = 100;

    ASSERT_EQ(1, m_params.local.getNumOfCurrSameParam());
    ASSERT_TRUE(m_obj->getLocalParam(getCom(pn)));
    ASSERT_EQ(50, m_params.local.getValue());

    m_params.local.nextSameParam();

    ASSERT_EQ(2, m_params.local.getNumOfCurrSameParam());
    ASSERT_TRUE(m_obj->getLocalParam(getCom(pn)));
    ASSERT_EQ(150, m_params.local.getValue());

    m_params.local.nextSameParam();

    ASSERT_EQ(3, m_params.local.getNumOfCurrSameParam());
    ASSERT_TRUE(m_obj->getLocalParam(getCom(pn)));
    ASSERT_EQ(1000, m_params.local.getValue());


    pn = GB_PARAM_PRD_DEC_CF;
    m_params.local.clearParams();
    m_params.local.addParam(pn);

    ASSERT_FALSE(Param::PARAM_BITES == getParamType(pn));
    ASSERT_TRUE(Param::DEPEND_SAME_NO == getDependSame(pn));
    ASSERT_EQ(pn, m_params.local.getParam());

    buf[B1] = 2;
    buf[B2] = 3;
    buf[B3] = 4;

    ASSERT_TRUE(m_obj->getLocalParam(getCom(pn)));
    ASSERT_EQ(buf[B1], m_params.local.getValue());
}
