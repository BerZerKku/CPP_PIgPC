#include "gtest/gtest.h"
#include "PIg/src/glbDefine.h"
#include <cstdio>
#include <iostream>

#define TEST_FRIENDS \
    FRIEND_TEST(clProtocolBspS_Test, getComNetAdr);

#include "PIg/src/protocols/standart/protocolBspS.h"

using namespace std;

// размер массива
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class clProtocolBspS_Test: public ::testing::Test {
public:
    struct uartCfg_t {
        TProtocol::PROTOCOL protocol = TProtocol::STANDART;
        TBaudRate::BAUD_RATE baudrate = TBaudRate::_9600;
        TDataBits::DATA_BITS databits = TDataBits::_8;
        TParity::PARITY parity = TParity::EVEN;
        TStopBits::STOP_BITS stopbits = TStopBits::ONE;
        uint16_t password = 10000;
        uint8_t netaddress = 1;

    };

    const uint8_t kBufSize = 255;
    uint8_t *mBuf;
    stGBparam *mParam;
    clProtocolBspS *mProtocol;

    clProtocolBspS_Test() {}
    virtual ~clProtocolBspS_Test() override {}

    /// Проверка параметров Uart
    bool checkUartCfg(uartCfg_t &cfg) {
        bool check = true;
        if (cfg.protocol != mParam->Uart.Protocol.get()) {
            check = false;
            EXPECT_EQ(cfg.protocol, mParam->Uart.Protocol.get());
        }
        if (cfg.baudrate != mParam->Uart.BaudRate.get()) {
            check = false;
            EXPECT_EQ(cfg.baudrate, mParam->Uart.BaudRate.get());
        }
        if (cfg.databits != mParam->Uart.DataBits.get()) {
            check = false;
            EXPECT_EQ(cfg.databits, mParam->Uart.DataBits.get());
        }
        if (cfg.parity != mParam->Uart.Parity.get()) {
            check = false;
            EXPECT_EQ(cfg.parity, mParam->Uart.Parity.get());
        }
        if (cfg.stopbits != mParam->Uart.StopBits.get()) {
            check = false;
            EXPECT_EQ(cfg.stopbits, mParam->Uart.StopBits.get());
        }
        if (cfg.netaddress != mParam->Uart.NetAddress.get()) {
            EXPECT_EQ(cfg.netaddress, mParam->Uart.NetAddress.get());
        }
        return check;
    }

private:

    virtual void SetUp() override {
        mParam = new stGBparam();
        mBuf = new uint8_t[kBufSize];
        mProtocol = new clProtocolBspS(mBuf, kBufSize, mParam);

        for(uint16_t i = 0; i < kBufSize; i++) {
            mBuf[i] = 0xFF;
        }
    }
    virtual void TearDown() override {
        delete mProtocol;
        delete[] mBuf;
        delete mParam;
    }
};

//
TEST_F(clProtocolBspS_Test, getComNetAdr) {
    uartCfg_t cfg;

    ASSERT_EQ(1, POS_COM_NET_ADR_netAdr);
    ASSERT_EQ(2, POS_COM_NET_ADR_protocol);
    ASSERT_EQ(3, POS_COM_NET_ADR_baudrate);
    ASSERT_EQ(4, POS_COM_NET_ADR_dataBits);
    ASSERT_EQ(5, POS_COM_NET_ADR_parity);
    ASSERT_EQ(6, POS_COM_NET_ADR_stopBits);
    ASSERT_EQ(7, POS_COM_NET_ADR_password);
    ASSERT_EQ(9, POS_COM_NET_ADR_vpSac2);

    ASSERT_TRUE(checkUartCfg(cfg));

    mBuf[0] = cfg.netaddress = 135;
    ASSERT_EQ(1, mProtocol->getComNetAdr(POS_COM_NET_ADR_netAdr, mBuf, 1));

    ASSERT_TRUE(checkUartCfg(cfg));

    mBuf[0] = cfg.protocol = TProtocol::MODBUS;
    ASSERT_EQ(1, mProtocol->getComNetAdr(POS_COM_NET_ADR_protocol, mBuf, 1));

    ASSERT_TRUE(checkUartCfg(cfg));

    mBuf[1] = cfg.baudrate = TBaudRate::_4800;
    ASSERT_EQ(1, mProtocol->getComNetAdr(POS_COM_NET_ADR_baudrate, mBuf+1, 1));

    ASSERT_TRUE(checkUartCfg(cfg));

    mBuf[0] = cfg.databits = TDataBits::_8;
    ASSERT_EQ(1, mProtocol->getComNetAdr(POS_COM_NET_ADR_dataBits, mBuf, 1));

    ASSERT_TRUE(checkUartCfg(cfg));

    mBuf[0] = cfg.parity = TParity::ODD;
    ASSERT_EQ(1, mProtocol->getComNetAdr(POS_COM_NET_ADR_parity, mBuf, 1));

    ASSERT_TRUE(checkUartCfg(cfg));

    mBuf[2] = cfg.stopbits = TStopBits::TWO;
    ASSERT_EQ(1, mProtocol->getComNetAdr(POS_COM_NET_ADR_stopBits, mBuf+2, 1));

    ASSERT_TRUE(checkUartCfg(cfg));

    *((uint16_t*) &mBuf[0]) = cfg.password = 3456;
    ASSERT_EQ(1, mProtocol->getComNetAdr(POS_COM_NET_ADR_password, mBuf, 1));

    ASSERT_TRUE(checkUartCfg(cfg));

    ASSERT_EQ(1, mProtocol->getComNetAdr(POS_COM_NET_ADR_vpSac2, mBuf, 1));

    ASSERT_TRUE(checkUartCfg(cfg));
}
