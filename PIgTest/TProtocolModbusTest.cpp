#include "gtest/gtest.h"
#include "PIg/src/protocols/modbus/protocolModbus.h"
#include <cstdio>
#include <iostream>
using namespace std;

// ������ �������
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

using TPM = TProtocolModbus;

static char msg[1000];		// ����� ���������
static uint16_t cnt_msg = 0; 	// ���-�� ������ � ������

class TProtocolModbus_Test: public ::testing::Test {
public:
    static const uint16_t SIZE_BUF = 255; 	// ������ ������ ������
    uint8_t buf[SIZE_BUF];					// ����� ������
    TPM *mb;								// ����������� �����

    // �����������
    TProtocolModbus_Test() {
        mb = new TPM(buf, SIZE_BUF);
        mb->setTick(57600, 100);
    }

    // ����������
    virtual ~TProtocolModbus_Test() {
        delete mb;
    }

    // ��������� ������� ��������� �������
    void readCom(uint8_t *buf, uint16_t size) {

        mb->setReadState();

        // ��������� ������ � �����
        for (uint8_t i = 0; i < size; i++) {
            mb->push(*buf++);
        }

        // ������������ ������� ��������� �������
        for(uint8_t k = 0; k < 50; k++) {
            mb->tick();
        }
    }

    // ��������� ��������, true - ���������
    bool checkArray(uint8_t *buf1, uint16_t size1, uint8_t *buf2, uint16_t size2) {
        bool state = true;

        if (size1 != size2) {
            cnt_msg += sprintf(&msg[cnt_msg], "(sizeof(expected) = %d) != ", size1);
            cnt_msg += sprintf(&msg[cnt_msg], "(sizeof(response) = %d) \n", size2);
            state = false;
        }


        for (uint16_t i = 0; i < size1; i++) {
            if (buf1[i] != buf2[i]) {
                cnt_msg += sprintf(&msg[cnt_msg], "(buf1[%d] = 0x%02X) != ", i, buf1[i]);
                cnt_msg += sprintf(&msg[cnt_msg], "(buf2[%d] = 0x%02X) \n", i, buf2[i]);
                state = false;
                break;
            }
        }

        if (!state) {
            for(uint16_t i = 0; i < size1; i++) {
                cnt_msg += sprintf(&msg[cnt_msg], "0x%02X ", buf1[i]);
            }

            cnt_msg +=  sprintf(&msg[cnt_msg], "\n");

            for(uint16_t i = 0; i < size2; i++) {
                cnt_msg += sprintf(&msg[cnt_msg], "0x%02X ", buf2[i]);
            }
        }

        return state;
    }

    // ������������ ������
    bool test_rx(uint8_t *request, uint16_t size1, uint8_t *response,
                 uint16_t size2) {
        cnt_msg = 0;
        readCom(request, size1);
        mb->setAddressLan(request[0]);
        mb->readData();
        return checkArray(response, size2, buf, mb->getNumOfBytes());
    }

    // ������������ ��������
    bool test_tx(uint8_t *request, uint16_t size1, uint8_t *response, uint16_t size2) {
        cnt_msg = 0;
        readCom(request, size1);
        mb->setAddressLan(request[0]);
        mb->readData();
        mb->sendData();
        return checkArray(response, size2, buf, mb->getNumOfBytes());
    }


    char* printArray(uint8_t *arr, uint16_t size) {
        uint16_t cnt = 0;
        for(uint8_t i = 0; i < size; i++) {
            cnt += sprintf(&msg[cnt], "0x%02X ", arr[i]);
        }
        return reinterpret_cast<char *> (arr);
    }

private:
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(TProtocolModbus_Test, getMaxNumRegisters) {
	TPM mb(buf, SIZE_BUF);

	ASSERT_EQ(32, mb.getMaxNumRegisters());
    ASSERT_EQ(256, mb.getMaxNumCoils());
    ASSERT_EQ(255, mb.getAddressError());
    ASSERT_EQ(1, mb.getAddressMin());
    ASSERT_EQ(247, mb.getAddressMax());
}

TEST_F(TProtocolModbus_Test, setState_and_checkState) {
	TPM mb(buf, SIZE_BUF);

    ASSERT_TRUE(mb.checkState(TPM::STATE_OFF));

	// ������������ ������� ��������� � �������� �������� ���������.
	uint8_t min = TPM::STATE_OFF;
	uint8_t max = TPM::STATE_ERROR;
	for(uint8_t i = min; i < max; i++) {
		TPM::STATE t = static_cast<TPM::STATE> (i);
		mb.setState(t);
		if (!mb.checkState(t)) {
			sprintf(msg, "  >>> ������ �� ���� %d", t);
			ASSERT_TRUE(false) << msg;
		}
	}
}

TEST_F(TProtocolModbus_Test, getState) {
	TPM mb(buf, SIZE_BUF);

	uint8_t min = TPM::STATE_OFF;
	uint8_t max = TPM::STATE_ERROR;
	for(uint8_t i = min; i < max; i++) {
		TPM::STATE t = static_cast<TPM::STATE> (i);
		mb.setState(t);
		if (t != mb.getState()) {
			sprintf(msg, "  >>> ������ �� ���� %d", i);
			ASSERT_TRUE(false) << msg;
		}
	}
}

TEST_F(TProtocolModbus_Test, setEnable) {
	TPM mb(buf, SIZE_BUF);

	mb.setEnable();
	ASSERT_TRUE(mb.checkState(TPM::STATE_READ));
}

TEST_F(TProtocolModbus_Test, setDisable) {
	TPM mb(buf, SIZE_BUF);

	mb.setEnable();
	mb.setDisable();
	ASSERT_TRUE(mb.checkState(TPM::STATE_OFF));
}

TEST_F(TProtocolModbus_Test, isEnable) {
	TPM mb(buf, SIZE_BUF);

	mb.setEnable();
	ASSERT_TRUE(mb.isEnable());
	mb.setDisable();
	ASSERT_TRUE(!mb.isEnable());
}

TEST_F(TProtocolModbus_Test, setReadState) {
	TPM mb(buf, SIZE_BUF);

	mb.setReadState();
	ASSERT_TRUE(mb.checkState(TPM::STATE_READ));
}

TEST_F(TProtocolModbus_Test, isReadData) {
	TPM mb(buf, SIZE_BUF);

	uint8_t min = TPM::STATE_OFF;
	uint8_t max = TPM::STATE_ERROR;
	for(uint8_t i = min; i < max; i++) {
		TPM::STATE t = static_cast<TPM::STATE> (i);
		mb.setState(t);
		if ((t == TPM::STATE_READ_OK) != mb.isReadData()) {
			sprintf(msg, "  >>> ������ �� ���� %d", t);
			ASSERT_TRUE(false) << msg;
		}
	}

	mb.setDisable();
	ASSERT_TRUE(!mb.isReadData());
	mb.setReadState();
	ASSERT_TRUE(!mb.isReadData());
}

TEST_F(TProtocolModbus_Test, setTick) {
	TPM mb(buf, SIZE_BUF);

	ASSERT_EQ(1000, mb.setTick(57600, 50));
	ASSERT_EQ(2000, mb.setTick(57600, 100));
	ASSERT_EQ(872, mb.setTick(19200, 50));
	ASSERT_EQ(1745, mb.setTick(19200, 100));
	ASSERT_EQ(13, mb.setTick(300, 50));
	ASSERT_EQ(27, mb.setTick(300, 100));
}

TEST_F(TProtocolModbus_Test, tick) {
	TPM mb(buf, SIZE_BUF);

	// ������ ���� ���� ������� �������� ���� �� �����
	// ������� ��� ����������� � ����

	struct sData {
		TPM::STATE startState;	// ��������� ��������� ���������
		TPM::STATE stopState;	// �������� ��������� ���������
		uint8_t	 numBytes;		// ���-�� ���� ������ � ������
		uint16_t baudrate;		// �������� ������ �����
		uint8_t  period; 		// ������ ������ ������� tick()
		uint16_t numTicks; 		// ���-�� �����

	};

	sData data[] = {
			{TPM::STATE_OFF, 		TPM::STATE_OFF, 		8, 19200, 50, 50},
			{TPM::STATE_READ, 		TPM::STATE_READ, 		3, 57600, 50, 35},
			{TPM::STATE_READ, 		TPM::STATE_READ_OK, 	4, 57600, 50, 35},
			{TPM::STATE_READ, 		TPM::STATE_READ_OK, 	5, 57600, 50, 35},
			{TPM::STATE_READ, 		TPM::STATE_READ_OK, 	6, 19200, 50, 41},
			{TPM::STATE_READ, 		TPM::STATE_READ, 		8, 19200, 50, 40},
			{TPM::STATE_READ, 		TPM::STATE_READ_OK, 	8, 19200, 50, 41},
			{TPM::STATE_READ_ERROR, TPM::STATE_READ, 		8, 19200, 50, 41},
			{TPM::STATE_READ_OK, 	TPM::STATE_READ_OK, 	8, 19200, 50, 50},
			{TPM::STATE_WRITE_WAIT, TPM::STATE_WRITE_WAIT, 	8, 19200, 50, 50},
			{TPM::STATE_WRITE, 		TPM::STATE_WRITE, 		8, 19200, 50, 50}
	};

	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {

		uint16_t step = mb.setTick(data[i].baudrate, data[i].period);
		mb.setState(TPM::STATE_READ);	// ��� ������ ��������
		mb.setState(data[i].startState);	// ������ ��� ����� ���������

		// ������� � ����� �������� ���-�� ���� ������
		for(uint8_t j = 0; j < data[i].numBytes; j++) {
			mb.push(0x00);
		}

		for (uint8_t j = 0; j < data[i].numTicks; j++) {
			mb.tick();
		}

		TPM::STATE state = mb.getState();
		if (state != data[i].stopState) {
            int cnt = sprintf(msg, "  >>> ������ �� ���� %d", i);
			cnt += sprintf(&msg[cnt], "\n start = %d,  finish = %d, need = %d",
					data[i].startState, state, data[i].stopState);
			cnt += sprintf(&msg[cnt], "\n step tick = %d", step);
			ASSERT_TRUE(false) << msg;
		}
	}
}

TEST_F(TProtocolModbus_Test, push) {
	TPM mb(buf, SIZE_BUF);

	mb.setState(TPM::STATE_READ);

	// 1. �������� ������ ����������� ���������� ���-�� ���� ������
	// ��� �������� ������������ tick
	for (uint16_t i = 1; i <= 2 * sizeof (buf); i++) {
        uint16_t t = mb.push(static_cast<uint8_t> (i));
		if (i <= sizeof (buf)) {
			if (t != i) {
				sprintf(msg, "  <<< �������� ���������� �������� ���� �� ���� %d", i);
				ASSERT_TRUE(false) << msg;
			}
		} else {
			if (t != sizeof (buf)) {
				sprintf(msg, "  <<< ������������ ������ �� ���� %d", i);
				ASSERT_TRUE(false) << msg;
			}
		}
	}

	// 2. �������� ������ ������� ��� ����� 1.5 ���������.
	struct sData {
		TPM::STATE stateStart;	// ��������� ��������� � ������
		TPM::STATE stateStop;	// ��������� ��������� � �����
		uint8_t numOfPush;	// ���-�� �������� ������
		uint8_t stepToReset;	// ���, �� ������� ����������� ����� ������
		uint8_t numOfByte;	// ���-�� �������� ���� �� ���������
	};

	sData data[] = {
			{TPM::STATE_OFF, 		TPM::STATE_OFF, 		20, 21,	0},
			{TPM::STATE_OFF, 		TPM::STATE_OFF, 		20, 5, 	0},
			{TPM::STATE_READ, 		TPM::STATE_READ, 		20, 20,	20},
			{TPM::STATE_READ, 		TPM::STATE_READ_ERROR, 	20, 18,	18},
			{TPM::STATE_READ, 		TPM::STATE_READ_ERROR, 	20, 2,	2},
			{TPM::STATE_READ_ERROR,	TPM::STATE_READ_ERROR, 	20, 18,	0},
			{TPM::STATE_READ_OK, 	TPM::STATE_READ_OK, 	20, 18,	0},
			{TPM::STATE_WRITE_WAIT,	TPM::STATE_WRITE_WAIT, 	20, 18,	0},
			{TPM::STATE_WRITE, 		TPM::STATE_WRITE, 		20, 21,	0}
	};

	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {
		mb.setTick(57600, 100);
		mb.setState(TPM::STATE_READ);	// ��� ������ ��������
		mb.setState(data[i].stateStart);	// ������ ��� ����� ���������

		uint8_t num = 0;
		for (uint8_t j = 0; j < data[i].numOfPush; j++) {
			// ������������ ����� ������
			if (j == data[i].stepToReset) {
				for(uint8_t k = 0; k < 15; k++) {
					mb.tick();
				}
			}
			num = mb.push(0x00);
		}

		TPM::STATE state = mb.getState();
		if (state != data[i].stateStop) {
            int cnt = sprintf(msg, "  >>> ��������� ��������� �� ���� %d", i);
			cnt += sprintf(&msg[cnt], "\n start = %d,  finish = %d, need = %d", data[i].stateStart, state, data[i].stateStop);
			cnt += sprintf(&msg[cnt], "\n num = %d", num);
			ASSERT_TRUE(false) << msg;
		}

		if (num != data[i].numOfByte) {
            int cnt = sprintf(msg, "  >>> ��������� ���-�� �������� ���� �� ���� %d", i);
			cnt += sprintf(&msg[cnt], "\n start = %d,  finish = %d", data[i].stateStart, data[i].stateStop);
			cnt += sprintf(&msg[cnt], "\n num = %d, need = %d", num, data[i].numOfByte);
			ASSERT_TRUE(false) << msg;
		}
	}
}

TEST_F(TProtocolModbus_Test, get_and_set) {
	TPM mb(buf, sizeof (buf));

	uint8_t adrError = mb.getAddressError();
	uint8_t adrMin = mb.getAddressMin();
	uint8_t adrMax = mb.getAddressMax();

	// 1. �������� ������ ��-���������.
	if (mb.getAddressLan() != adrError) {
		sprintf(msg, "  >>> ����� �� ��������� �� ��������� � ADDRESS_ERR.");
		ASSERT_TRUE(false) << msg;
	}

	// 2. �������� ���������/���������� ������.
	for (uint16_t i = 0; i <= 255; i++) {
        bool t = mb.setAddressLan(static_cast<uint8_t> (i));

		// �������� ������������� �������� �������� serAddress())
		if (((i < adrMin) || (i > adrMax)) == t) {
			sprintf(msg, "  >>> setAddress() ���������� ��������� ��������� �� ���� %d", i);
			ASSERT_TRUE(false) << msg;
		}

		// �������� �������������� ��������
		if (i < adrMin) {
			if (mb.getAddressLan() != adrError) {
				sprintf(msg, "  >>> �������� ������ ADDRESS_MIN �� ����� � ADDRESS_ERR �� ���� %d", i);
				ASSERT_TRUE(false) << msg;
			}
		} else if (i > adrMax) {
			if (mb.getAddressLan() != adrMax) {
				sprintf(msg, "  >>> �������� ������ ADDRESS_MAX �� ����� � ADDRESS_MAX �� ���� %d", i);
				ASSERT_TRUE(false) << msg;
			}
		} else if (i != mb.getAddressLan()) {
			sprintf(msg, "  >>> ������������� ����� �� ��������� �� ��������� �� ���� %d", i);
			ASSERT_TRUE(false) << msg;
		}
	}
}

TEST_F(TProtocolModbus_Test, getNumOfBytes) {
	// ����������� ������ �������� �� ���������
	// �������������� �������� � readData
	ASSERT_EQ(0, mb->getNumOfBytes());
}

// �������� ������ � �������� ����������� ������
TEST_F(TProtocolModbus_Test, com_crc_error) {

	uint8_t req1[] = {0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0xFD, 0x1A};
	uint8_t res1[] = {};
    ASSERT_TRUE(test_rx(req1, SIZE_ARRAY(req1), res1, SIZE_ARRAY(res1))) << msg;

	uint8_t req2[] = {0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0xED, 0xCA};
	uint8_t res2[] = {};
    ASSERT_TRUE(test_rx(req2, SIZE_ARRAY(req2), res2, SIZE_ARRAY(res2))) << msg;
}

// �������� ������� ������ ������
TEST_F(TProtocolModbus_Test, com_0x01_read_coil) {

	// ���������� ���������� ������
	uint8_t req1[] = {0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0xFD, 0xCA};
	uint8_t res1[] = {0x01, 0x01, 0x01, 0x01};
    ASSERT_TRUE(test_rx(req1, SIZE_ARRAY(req1), res1, SIZE_ARRAY(res1))) << msg;

	// ���������� �������������� ������
	uint8_t req2[] = {0x01, 0x01, 0x00, 0x65, 0x00, 0x01, 0xED, 0xD5};
	uint8_t res2[] = {0x01, 0x01, 0x01, 0x00};
    ASSERT_TRUE(test_rx(req2, SIZE_ARRAY(req2), res2, SIZE_ARRAY(res2))) << msg;

	// ���������� ���������� ������
	uint8_t req3[] = {0x01, 0x01, 0x01, 0x2B, 0x00, 0x01, 0x8C, 0x3E};
	uint8_t res3[] = {0x01, 0x01, 0x01, 0x01};
    ASSERT_TRUE(test_rx(req3, SIZE_ARRAY(req3), res3, SIZE_ARRAY(res3))) << msg;

	// ���������� ������ �������
	uint8_t req4[] = {0x11, 0x01, 0x00, 0x50, 0x00, 0x29, 0xFF, 0x55};
	uint8_t res4[] = {0x11, 0x01, 0x06, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00};
    ASSERT_TRUE(test_rx(req4, SIZE_ARRAY(req4), res4, SIZE_ARRAY(res4))) << msg;

	// c��������� ����������� ���������� ���-�� �������
	uint8_t req5[] = {0x11, 0x01, 0x00, 0x00, 0x01, 0x00, 0x3F, 0x0A};
	uint8_t res5[] = {0x11, 0x01, 0x20,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	};
    ASSERT_TRUE(test_rx(req5, SIZE_ARRAY(req5), res5, SIZE_ARRAY(res5))) << msg;

	// ������: ���������� ������� 0
	// ���������� EXCEPTION_03H_ILLEGAL_DATA_VAL
	uint8_t req6[] = {0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x0A};
	uint8_t res6[] = {0x01, 0x81, 0x03};
    ASSERT_TRUE(test_rx(req6, SIZE_ARRAY(req6), res6, SIZE_ARRAY(res6))) << msg;

	// ������: ���������� ���-�� ������������� ������� 247
	// ���������� EXCEPTION_03H_ILLEGAL_DATA_VAL
	uint8_t req7[] = {0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0xFC, 0x5A};
	uint8_t res7[] = {0x01, 0x81, 0x03};
    ASSERT_TRUE(test_rx(req7, SIZE_ARRAY(req7), res7, SIZE_ARRAY(res7))) << msg;

	// ������: ����� �� �������� ��������� �������, ��� ���������� ���������
	// ���������� EXCEPTION_02H_ILLEGAL_DATA_ADR
	uint8_t req8[] = {0x01, 0x01, 0x01, 0x21, 0x00, 0x0C, 0x6D, 0xF9};
	uint8_t res8[] = {0x01, 0x81, 0x02};
    ASSERT_TRUE(test_rx(req8, SIZE_ARRAY(req8), res8, SIZE_ARRAY(res8))) << msg;

	// ������: ����� �� �������� ��������� �������, ��� ������ ������
	// ���������� EXCEPTION_02H_ILLEGAL_DATA_ADR
	uint8_t req9[] = {0x01, 0x01, 0x01, 0x2C, 0x00, 0x01, 0x3D, 0xFF};
	uint8_t res9[] = {0x01, 0x81, 0x02};
    ASSERT_TRUE(test_rx(req9, SIZE_ARRAY(req9), res9, SIZE_ARRAY(res9))) << msg;
}

// �������� ������� ������ ���������� ������
TEST_F(TProtocolModbus_Test, com_0x02_discrete_inputs) {

	// ���������� ���������� ������
	uint8_t req1[] = {0x01, 0x02, 0x00, 0x00, 0x00, 0x01, 0xB9, 0xCA};
	uint8_t res1[] = {0x01, 0x02, 0x01, 0x01};
    ASSERT_TRUE(test_rx(req1, SIZE_ARRAY(req1), res1, SIZE_ARRAY(res1))) << msg;

	// ���������� �������������� ������
	uint8_t req2[] = {0x01, 0x02, 0x00, 0x65, 0x00, 0x01, 0xA9, 0xD5};
	uint8_t res2[] = {0x01, 0x02, 0x01, 0x00};
    ASSERT_TRUE(test_rx(req2, SIZE_ARRAY(req2), res2, SIZE_ARRAY(res2))) << msg;

	// ���������� ���������� ������
	uint8_t req3[] = {0x01, 0x02, 0x01, 0x2B, 0x00, 0x01, 0xC8, 0x3E};
	uint8_t res3[] = {0x01, 0x02, 0x01, 0x01};
    ASSERT_TRUE(test_rx(req3, SIZE_ARRAY(req3), res3, SIZE_ARRAY(res3))) << msg;

	// ���������� ������ �������
	uint8_t req4[] = {0x11, 0x02, 0x00, 0x50, 0x00, 0x29, 0xBB, 0x55};
	uint8_t res4[] = {0x11, 0x02, 0x06, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00};
    ASSERT_TRUE(test_rx(req4, SIZE_ARRAY(req4), res4, SIZE_ARRAY(res4))) << msg;

	// c��������� ����������� ���������� ���-�� �������
	uint8_t req5[] = {0x11, 0x02, 0x00, 0x00, 0x01, 0x00, 0x7B, 0x0A};
	uint8_t res5[] = {0x11, 0x02, 0x20,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	};
    ASSERT_TRUE(test_rx(req5, SIZE_ARRAY(req5), res5, SIZE_ARRAY(res5))) << msg;

	// ������: ���������� ������� 0
	// ���������� EXCEPTION_03H_ILLEGAL_DATA_VAL
	uint8_t req6[] = {0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0A};
	uint8_t res6[] = {0x01, 0x82, 0x03};
    ASSERT_TRUE(test_rx(req6, SIZE_ARRAY(req6), res6, SIZE_ARRAY(res6))) << msg;

	// ������: ���������� ���-�� ������������� ������� 247
	// ���������� EXCEPTION_03H_ILLEGAL_DATA_VAL
	uint8_t req7[] = {0x01, 0x02, 0x00, 0x00, 0x01, 0x01, 0xB8, 0x5A};
	uint8_t res7[] = {0x01, 0x82, 0x03};
    ASSERT_TRUE(test_rx(req7, SIZE_ARRAY(req7), res7, SIZE_ARRAY(res7))) << msg;

	// ������: ����� �� �������� ��������� �������, ��� ���������� ���������
	// ���������� EXCEPTION_02H_ILLEGAL_DATA_ADR
	uint8_t req8[] = {0x01, 0x02, 0x01, 0x21, 0x00, 0x0C, 0x29, 0xF9};
	uint8_t res8[] = {0x01, 0x82, 0x02};
    ASSERT_TRUE(test_rx(req8, SIZE_ARRAY(req8), res8, SIZE_ARRAY(res8))) << msg;

	// ������: ����� �� �������� ��������� �������, ��� ������ ������
	// ���������� EXCEPTION_02H_ILLEGAL_DATA_ADR
	uint8_t req9[] = {0x01, 0x02, 0x01, 0x2C, 0x00, 0x01, 0x79, 0xFF};
	uint8_t res9[] = {0x01, 0x82, 0x02};
    ASSERT_TRUE(test_rx(req9, SIZE_ARRAY(req9), res9, SIZE_ARRAY(res9))) << msg;
}

// �������� ������� ������ ��������� ���������
TEST_F(TProtocolModbus_Test, com_0x03_read_holding_register) {

	// ���������� �������
	uint8_t req1[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
	uint8_t res1[] = {0x01, 0x03, 0x02, 0x00, 0x01};
    ASSERT_TRUE(test_rx(req1, SIZE_ARRAY(req1), res1, SIZE_ARRAY(res1))) << msg;

	// ���������� �������������� ������
	uint8_t req2[] = {0x01, 0x03, 0x00, 0x63, 0x00, 0x01, 0x74, 0x14};
	uint8_t res2[] = {0x01, 0x03, 0x02, 0x00, 0x64};
    ASSERT_TRUE(test_rx(req2, SIZE_ARRAY(req2), res2, SIZE_ARRAY(res2))) << msg;

	// ���������� ���������� ������
	uint8_t req3[] = {0x01, 0x03, 0x01, 0x2B, 0x00, 0x01, 0xF5, 0xFE};
	uint8_t res3[] = {0x01, 0x03, 0x02, 0x01, 0x2C};
    ASSERT_TRUE(test_rx(req3, SIZE_ARRAY(req3), res3, SIZE_ARRAY(res3))) << msg;

	// ���������� ������ �������
	uint8_t req4[] = {0x01, 0x03, 0x00, 0x60, 0x00, 0x05, 0x85, 0xD7};
	uint8_t res4[] = {0x01, 0x03, 0x0A, 0x00, 0x61, 0x00, 0x62, 0x00, 0x63,
			0x00, 0x64, 0xFF, 0xFF};
    ASSERT_TRUE(test_rx(req4, SIZE_ARRAY(req4), res4, SIZE_ARRAY(res4))) << msg;

	// ���������� ������������� ���������� �������
	uint8_t req5[] = {0x01, 0x03, 0x00, 0x50, 0x00, 0x20, 0x44, 0x03};
	uint8_t res5[] = {0x01, 0x03, 0x40,
			0x00, 0x51, 0x00, 0x52, 0x00, 0x53, 0x00, 0x54,
			0x00, 0x55, 0x00, 0x56, 0x00, 0x57, 0x00, 0x58,
			0x00, 0x59, 0x00, 0x5A, 0x00, 0x5B, 0x00, 0x5C,
			0x00, 0x5D, 0x00, 0x5E, 0x00, 0x5F, 0x00, 0x60,
			0x00, 0x61, 0x00, 0x62,	0x00, 0x63, 0x00, 0x64,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	};
    ASSERT_TRUE(test_rx(req5, SIZE_ARRAY(req5), res5, SIZE_ARRAY(res5))) << msg;

	// ������: ���������� ������� 0
	// ���������� EXCEPTION_03H_ILLEGAL_DATA_VAL
	uint8_t req6[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x45, 0xCA};
	uint8_t res6[] = {0x01, 0x83, 0x03};
    ASSERT_TRUE(test_rx(req6, SIZE_ARRAY(req6), res6, SIZE_ARRAY(res6))) << msg;

	// ������: ���������� ���-�� ������������� ������� 33
	// ���������� EXCEPTION_03H_ILLEGAL_DATA_VAL
	uint8_t req7[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x21, 0x85, 0xD2};
	uint8_t res7[] = {0x01, 0x83, 0x03};
    ASSERT_TRUE(test_rx(req7, SIZE_ARRAY(req7), res7, SIZE_ARRAY(res7))) << msg;

	// ������: ����� �� �������� ��������� �������, ��� ���������� ���������
	// ���������� EXCEPTION_02H_ILLEGAL_DATA_ADR
	uint8_t req8[] = {0x01, 0x03, 0x01, 0x2A, 0x00, 0x03, 0x25, 0xFF};
	uint8_t res8[] = {0x01, 0x83, 0x02};
    ASSERT_TRUE(test_rx(req8, SIZE_ARRAY(req8), res8, SIZE_ARRAY(res8))) << msg;

	// ������: ����� �� �������� ��������� �������, ��� ������ ������
	// ���������� EXCEPTION_02H_ILLEGAL_DATA_ADR
	uint8_t req9[] = {0x01, 0x03, 0x01, 0x2C, 0x00, 0x01, 0x44, 0x3F};
	uint8_t res9[] = {0x01, 0x83, 0x02};
    ASSERT_TRUE(test_rx(req9, SIZE_ARRAY(req9), res9, SIZE_ARRAY(res9))) << msg;
}

// �������� ������� ������ ������� ���������
TEST_F(TProtocolModbus_Test, com_0x04_read_input_register) {

	// ���������� �������
	uint8_t req1[] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x01, 0x31, 0xCA};
	uint8_t res1[] = {0x01, 0x04, 0x02, 0x00, 0x01};
    ASSERT_TRUE(test_rx(req1, SIZE_ARRAY(req1), res1, SIZE_ARRAY(res1))) << msg;

	// ���������� �������������� ������
	uint8_t req2[] = {0x01, 0x04, 0x00, 0x63, 0x00, 0x01, 0xC1, 0xD4};
	uint8_t res2[] = {0x01, 0x04, 0x02, 0x00, 0x64};
    ASSERT_TRUE(test_rx(req2, SIZE_ARRAY(req2), res2, SIZE_ARRAY(res2))) << msg;

	// ���������� ���������� ������
	uint8_t req3[] = {0x01, 0x04, 0x01, 0x2B, 0x00, 0x01, 0x40, 0x3E};
	uint8_t res3[] = {0x01, 0x04, 0x02, 0x01, 0x2C};
    ASSERT_TRUE(test_rx(req3, SIZE_ARRAY(req3), res3, SIZE_ARRAY(res3))) << msg;

	// ���������� ������ �������
	uint8_t req4[] = {0x01, 0x04, 0x00, 0x60, 0x00, 0x05, 0x30, 0x17};
	uint8_t res4[] = {0x01, 0x04, 0x0A, 0x00, 0x61, 0x00, 0x62, 0x00, 0x63,
			0x00, 0x64, 0xFF, 0xFF};
    ASSERT_TRUE(test_rx(req4, SIZE_ARRAY(req4), res4, SIZE_ARRAY(res4))) << msg;

	// ���������� ������������� ���������� �������
	uint8_t req5[] = {0x01, 0x04, 0x00, 0x50, 0x00, 0x20, 0xF1, 0xC3};
	uint8_t res5[] = {0x01, 0x04, 0x40,
			0x00, 0x51, 0x00, 0x52, 0x00, 0x53, 0x00, 0x54,
			0x00, 0x55, 0x00, 0x56, 0x00, 0x57, 0x00, 0x58,
			0x00, 0x59, 0x00, 0x5A, 0x00, 0x5B, 0x00, 0x5C,
			0x00, 0x5D, 0x00, 0x5E, 0x00, 0x5F, 0x00, 0x60,
			0x00, 0x61, 0x00, 0x62,	0x00, 0x63, 0x00, 0x64,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	};
    ASSERT_TRUE(test_rx(req5, SIZE_ARRAY(req5), res5, SIZE_ARRAY(res5))) << msg;

	// ������: ���������� ������� 0
	// ���������� EXCEPTION_03H_ILLEGAL_DATA_VAL
	uint8_t req6[] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x0A};
	uint8_t res6[] = {0x01, 0x84, 0x03};
    ASSERT_TRUE(test_rx(req6, SIZE_ARRAY(req6), res6, SIZE_ARRAY(res6))) << msg;

	// ������: ���������� ���-�� ������������� ������� 33
	// ���������� EXCEPTION_03H_ILLEGAL_DATA_VAL
	uint8_t req7[] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x21, 0x30, 0x12};
	uint8_t res7[] = {0x01, 0x84, 0x03};
    ASSERT_TRUE(test_rx(req7, SIZE_ARRAY(req7), res7, SIZE_ARRAY(res7))) << msg;

	// ������: ����� �� �������� ��������� �������, ��� ���������� ���������
	// ���������� EXCEPTION_02H_ILLEGAL_DATA_ADR
	uint8_t req8[] = {0x01, 0x04, 0x01, 0x2A, 0x00, 0x03, 0x90, 0x3F};
	uint8_t res8[] = {0x01, 0x84, 0x02};
    ASSERT_TRUE(test_rx(req8, SIZE_ARRAY(req8), res8, SIZE_ARRAY(res8))) << msg;

	// ������: ����� �� �������� ��������� �������, ��� ������ ������
	// ���������� EXCEPTION_02H_ILLEGAL_DATA_ADR
	uint8_t req9[] = {0x01, 0x04, 0x01, 0x2C, 0x00, 0x01, 0xF1, 0xFF};
	uint8_t res9[] = {0x01, 0x84, 0x02};
    ASSERT_TRUE(test_rx(req9, SIZE_ARRAY(req9), res9, SIZE_ARRAY(res9))) << msg;
}

// �������� ������� ������ ������ �����
TEST_F(TProtocolModbus_Test, com_0x05_write_single_coil) {

	// �������� ������ � ������ �����
	uint8_t req1[] = {0x01, 0x05, 0x00, 0x00, 0xFF, 0x00, 0x8C, 0x3A};
	uint8_t res1[] = {0x01, 0x05, 0x00, 0x00, 0xFF, 0x00};
    ASSERT_TRUE(test_rx(req1, SIZE_ARRAY(req1), res1, SIZE_ARRAY(res1))) << msg;

	// �������� ������ � ��������� �����
	uint8_t req2[] = {0x01, 0x05, 0x01, 0x2B, 0x00, 0x00, 0xBC, 0x3E};
	uint8_t res2[] = {0x01, 0x05, 0x01, 0x2B, 0x00, 0x00};
    ASSERT_TRUE(test_rx(req2, SIZE_ARRAY(req2), res2, SIZE_ARRAY(res2))) << msg;

	// �������� ������ � ������������� ����� �������� true
	uint8_t req3[] = {0x01, 0x05, 0x00, 0x98, 0xFF, 0x00, 0x0D, 0xD5};
	uint8_t res3[] = {0x01, 0x05, 0x00, 0x98, 0xFF, 0x00};
    ASSERT_TRUE(test_rx(req3, SIZE_ARRAY(req3), res3, SIZE_ARRAY(res3))) << msg;

	// �������� ������ � ������������� ����� �������� false
	uint8_t req4[] = {0x01, 0x05, 0x00, 0xEE, 0x00, 0x00, 0xAD, 0xFF};
	uint8_t res4[] = {0x01, 0x05, 0x00, 0xEE, 0x00, 0x00};
    ASSERT_TRUE(test_rx(req4, SIZE_ARRAY(req4), res4, SIZE_ARRAY(res4))) << msg;

	// ������: ������ � �������������� �����
	// ���������� EXCEPTION_02H_ILLEGAL_DATA_ADR
	uint8_t req5[] = {0x01, 0x05, 0x01, 0x2C, 0x00, 0x00, 0x0D, 0xFF};
	uint8_t res5[] = {0x01, 0x85, 0x02};
    ASSERT_TRUE(test_rx(req5, SIZE_ARRAY(req5), res5, SIZE_ARRAY(res5))) << msg;

	// ������: ������������ �������� ��� ������ true
	//  ���������� EXCEPTION_04H_DEVICE_FAILURE
	uint8_t req6[] = {0x01, 0x05, 0x00, 0xC8, 0xFF, 0x00, 0x0D, 0xC4};
	uint8_t res6[] = {0x01, 0x85, 0x04};
    ASSERT_TRUE(test_rx(req6, SIZE_ARRAY(req6), res6, SIZE_ARRAY(res6))) << msg;

	// ������: ������������ �������� ��� ������ false
	//  ���������� EXCEPTION_04H_DEVICE_FAILURE
	uint8_t req7[] = {0x01, 0x05, 0x00, 0x64, 0x00, 0x00, 0x8C, 0x15};
	uint8_t res7[] = {0x01, 0x85, 0x04};
    ASSERT_TRUE(test_rx(req7, SIZE_ARRAY(req7), res7, SIZE_ARRAY(res7))) << msg;

	// ������: �������� �������� �����
	// ���������� EXCEPTION_03H_ILLEGAL_DATA_VAL
	uint8_t req8[] = {0x88, 0x05, 0x01, 0x60, 0x00, 0xFF, 0x93, 0x31};
	uint8_t res8[] = {0x88, 0x85, 0x03};
    ASSERT_TRUE(test_rx(req8, SIZE_ARRAY(req8), res8, SIZE_ARRAY(res8))) << msg;
}

// �������� ������� ������ ������ ������
TEST_F(TProtocolModbus_Test, com_0x0F_write_multiplie_coils) {

	// �������� ������ ������ �����
	uint8_t req1[] = {0x01, 0x0F, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0xEF, 0x57};
	uint8_t res1[] = {0x01, 0x0F, 0x00, 0x00, 0x00, 0x01};
    ASSERT_TRUE(test_rx(req1, SIZE_ARRAY(req1), res1, SIZE_ARRAY(res1))) << msg;

	// �������� ������ ������ ������, ������ 8
	uint8_t req2[] = {0x01, 0x0F, 0x00, 0x00, 0x00, 0x07, 0x01, 0x15, 0x0F, 0x59};
	uint8_t res2[] = {0x01, 0x0F, 0x00, 0x00, 0x00, 0x07};
    ASSERT_TRUE(test_rx(req2, SIZE_ARRAY(req2), res2, SIZE_ARRAY(res2))) << msg;

	// �������� ������ ������ ������, ������ 8
	uint8_t req3[] = {0x01, 0x0F, 0x00, 0x00, 0x00, 0x0B, 0x02, 0x15, 0x01, 0x2B, 0x94};
	uint8_t res3[] = {0x01, 0x0F, 0x00, 0x00, 0x00, 0x0B};
    ASSERT_TRUE(test_rx(req3, SIZE_ARRAY(req3), res3, SIZE_ARRAY(res3))) << msg;

	// �������� ������ ������ ������, ������ 8
	uint8_t req4[] = {0x01, 0x0F, 0x00, 0x00, 0x00, 0x18, 0x03, 0x15, 0x11, 0x28, 0x5D, 0xFE};
	uint8_t res4[] = {0x01, 0x0F, 0x00, 0x00, 0x00, 0x18};
    ASSERT_TRUE(test_rx(req4, SIZE_ARRAY(req4), res4, SIZE_ARRAY(res4))) << msg;

	// �������� ������ ������������� ���-�� ������, 256
	uint8_t req5[] = {0x01, 0x0F, 0x00, 0x00, 0x01, 0x00, 0x20,
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
			0x09, 0x0A, 0x0B, 0x0C, 0xF0, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x8B, 0x8A };
	uint8_t res5[] = {0x01, 0x0F, 0x00, 0x00, 0x01, 0x00};
    ASSERT_TRUE(test_rx(req5, SIZE_ARRAY(req5), res5, SIZE_ARRAY(res5))) << msg;

	// ������: ��������� ����� ��������� ������������ �����
	// ���������� EXCEPTION_02H_ILLEGAL_DATA_ADR
	uint8_t req6[] = {0x01, 0x0F, 0x01, 0x2C, 0x00, 0x02, 0x01, 0x03, 0x0E, 0x81};
	uint8_t res6[] = {0x01, 0x8F, 0x02};
    ASSERT_TRUE(test_rx(req6, SIZE_ARRAY(req6), res6, SIZE_ARRAY(res6))) << msg;

	// ������: ��������� ����� + ���������� ������ ��������� ������������ �����
	//  ���������� EXCEPTION_02H_ILLEGAL_DATA_ADR
	uint8_t req7[] = {0x01, 0x0F, 0x01, 0x22, 0x00, 0x10, 0x02, 0x00, 0x00, 0x15, 0xA2, 0x48};
	uint8_t res7[] = {0x01, 0x8F, 0x02};
    ASSERT_TRUE(test_rx(req7, SIZE_ARRAY(req7), res7, SIZE_ARRAY(res7))) << msg;

	// ������: ������� ���������� ���� ��������������� ������ ��� ������
	// ���������� EXCEPTION_04H_DEVICE_FAILURE
	uint8_t req8[] = {0x88, 0x0F, 0x00, 0xFC, 0x00, 0x02, 0x01, 0x02, 0xC7, 0x48};
	uint8_t res8[] = {0x88, 0x8F, 0x04};
    ASSERT_TRUE(test_rx(req8, SIZE_ARRAY(req8), res8, SIZE_ARRAY(res8))) << msg;

	// ������: ������� �������� ���� ��������������� ������ ��� ���������
	// ���������� EXCEPTION_04H_DEVICE_FAILURE
	uint8_t req9[] = {0x88, 0x0F, 0x00, 0x71, 0x00, 0x02, 0x01, 0x00, 0x6A, 0x96};
	uint8_t res9[] = {0x88, 0x8F, 0x04};
    ASSERT_TRUE(test_rx(req9, SIZE_ARRAY(req9), res9, SIZE_ARRAY(res9))) << msg;

	// �������� ������ �������� ���-�� ������
	// ���������� EXCEPTION_03H_ILLEGAL_DATA_VAL
	uint8_t req10[] = {0x11, 0x0F, 0x00, 0x13, 0x00, 0x00, 0x00, 0x1E, 0x7A};
	uint8_t res10[] = {0x11, 0x8F, 0x03};
    ASSERT_TRUE(test_rx(req10, SIZE_ARRAY(req10), res10, SIZE_ARRAY(res10))) << msg;

	// �������� ������ ���-�� ������� ������������ ��������, 257
	// ���������� EXCEPTION_03H_ILLEGAL_DATA_VAL
	uint8_t req11[] = {0x11, 0x0F, 0x00, 0x13, 0x01, 0x00, 0x21,
			0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
			0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
			0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
			0x05,
			0xA7, 0x9E};
	uint8_t res11[] = {0x11, 0x8F, 0x03};
    ASSERT_TRUE(test_rx(req11, SIZE_ARRAY(req11), res11, SIZE_ARRAY(res11))) << msg;
}

// �������� ������ ������ ��������
TEST_F(TProtocolModbus_Test, com_0x06_write_single_register) {

	// �������� ������ � ������ �����
	uint8_t req1[] = {0x01, 0x06, 0x00, 0x00, 0xFF, 0x00, 0xC8, 0x3A};
	uint8_t res1[] = {0x01, 0x06, 0x00, 0x00, 0xFF, 0x00};
    ASSERT_TRUE(test_rx(req1, SIZE_ARRAY(req1), res1, SIZE_ARRAY(res1))) << msg;

	// �������� ������ � ��������� �����
	uint8_t req2[] = {0x01, 0x06, 0x01, 0x2B, 0x11, 0x17, 0xB4, 0x60};
	uint8_t res2[] = {0x01, 0x06, 0x01, 0x2B, 0x11, 0x17};
    ASSERT_TRUE(test_rx(req2, SIZE_ARRAY(req2), res2, SIZE_ARRAY(res2))) << msg;

	// �������� ������ � ������������� ����� 0x0000
	uint8_t req3[] = {0x01, 0x06, 0x00, 0x66, 0x00, 0x00, 0x69, 0xD5};
	uint8_t res3[] = {0x01, 0x06, 0x00, 0x66, 0x00, 0x00};
    ASSERT_TRUE(test_rx(req3, SIZE_ARRAY(req3), res3, SIZE_ARRAY(res3))) << msg;

	// �������� ������ � ������������� ����� 0xFFFF
	uint8_t req4[] = {0x01, 0x06, 0x00, 0xEE, 0xFF, 0xFF, 0xE8, 0x4F};
	uint8_t res4[] = {0x01, 0x06, 0x00, 0xEE, 0xFF, 0xFF};
    ASSERT_TRUE(test_rx(req4, SIZE_ARRAY(req4), res4, SIZE_ARRAY(res4))) << msg;

	// ������: ������ � �������������� �����
	// ���������� EXCEPTION_02H_ILLEGAL_DATA_ADR
	uint8_t req5[] = {0x01, 0x06, 0x01, 0x2C, 0x00, 0x00, 0x49, 0xFF};
	uint8_t res5[] = {0x01, 0x86, 0x02};
    ASSERT_TRUE(test_rx(req5, SIZE_ARRAY(req5), res5, SIZE_ARRAY(res5))) << msg;

	// ������: ������������ �������� ��� ������ � �������� "�� ������ 100"
	//  ���������� EXCEPTION_04H_DEVICE_FAILURE
	uint8_t req6[] = {0x01, 0x06, 0x00, 0xC5, 0x80, 0x65, 0x38, 0x1C};
	uint8_t res6[] = {0x01, 0x86, 0x04};
    ASSERT_TRUE(test_rx(req6, SIZE_ARRAY(req6), res6, SIZE_ARRAY(res6))) << msg;

	// ������: ������������ �������� ��� ������ � �������� "�� ������ 1000"
	//  ���������� EXCEPTION_04H_DEVICE_FAILURE
	uint8_t req7[] = {0x01, 0x06, 0x01, 0x21, 0x00, 0x01, 0x19, 0xFC};
	uint8_t res7[] = {0x01, 0x86, 0x04};
    ASSERT_TRUE(test_rx(req7, SIZE_ARRAY(req7), res7, SIZE_ARRAY(res7))) << msg;
}

// �������� ������� ������ ������ ���������
TEST_F(TProtocolModbus_Test, com_0x10_write_multiplie_registers) {

	// �������� ������ ������ ��������
	uint8_t req1[] = {0x01, 0x10, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0xA6, 0x50};
	uint8_t res1[] = {0x01, 0x10, 0x00, 0x00, 0x00, 0x01};
    ASSERT_TRUE(test_rx(req1, SIZE_ARRAY(req1), res1, SIZE_ARRAY(res1))) << msg;

	// �������� ������ ������ ���������, ������ 8
	uint8_t req2[] = {0x01, 0x10, 0x00, 0x10, 0x00, 0x07, 0x0E,
			0x15, 0x0F, 0x14, 0x88, 0x91, 0x94, 0x71, 0x98,
			0x12, 0xFF, 0xE1, 0x12, 0x17, 0x71,
			0xEA, 0x36};
	uint8_t res2[] = {0x01, 0x10, 0x00, 0x10, 0x00, 0x07};
    ASSERT_TRUE(test_rx(req2, SIZE_ARRAY(req2), res2, SIZE_ARRAY(res2))) << msg;

	// �������� ������ ������ ���������, ������ 8
	uint8_t req3[] = {0x01, 0x10, 0x00, 0x25, 0x00, 0x09, 0x12,
			0x15, 0x0F, 0x14, 0x88, 0x91, 0x94, 0x71, 0x98,
			0x12, 0xFF, 0xE1, 0x12, 0x17, 0x71, 0x77, 0x99,
			0x13, 0x14,
			0x0A, 0x27};
	uint8_t res3[] = {0x01, 0x10, 0x00, 0x25, 0x00, 0x09};
    ASSERT_TRUE(test_rx(req3, SIZE_ARRAY(req3), res3, SIZE_ARRAY(res3))) << msg;

	// �������� ������ ������ ���������, ������ 8
	uint8_t req4[] = {0x01, 0x10, 0x00, 0xC6, 0x00, 0x08, 0x10,
			0x00, 0x11, 0x00, 0x62, 0x91, 0x94, 0x71, 0x98,
			0x12, 0xFF, 0xE1, 0x12, 0x17, 0x71, 0x77, 0x99,
			0x0B, 0x51};
	uint8_t res4[] = {0x01, 0x10, 0x00, 0xC6, 0x00, 0x08};
    ASSERT_TRUE(test_rx(req4, SIZE_ARRAY(req4), res4, SIZE_ARRAY(res4))) << msg;

	// �������� ������ ������������� ���-�� ���������, 32
	uint8_t req5[] = {0x11, 0x10, 0x00, 0x01, 0x00, 0x20, 0x40,
			 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
			 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
			 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
			 0xED, 0xDE};
	uint8_t res5[] = {0x11, 0x10, 0x00, 0x01, 0x00, 0x20};
    ASSERT_TRUE(test_rx(req5, SIZE_ARRAY(req5), res5, SIZE_ARRAY(res5))) << msg;

	// ������: ��������� ����� ��������� ������������ �����
	// ���������� EXCEPTION_02H_ILLEGAL_DATA_ADR
	uint8_t req6[] = {0x01, 0x10, 0x01, 0x2C, 0x00, 0x02, 0x04,
			0xFF, 0xFF, 0xAA, 0xBB,
			0xC2, 0x85};
	uint8_t res6[] = {0x01, 0x90, 0x02};
    ASSERT_TRUE(test_rx(req6, SIZE_ARRAY(req6), res6, SIZE_ARRAY(res6))) << msg;

	// ������: ��������� ����� + ���������� ��������� ��������� ������������ �����
	//  ���������� EXCEPTION_02H_ILLEGAL_DATA_ADR
	uint8_t req7[] = {0x01, 0x10, 0x01, 0x2A, 0x00, 0x04, 0x08,
			0xFF, 0xFF, 0x10, 0x00, 0x51, 0x98, 0x78, 0xFF,
			0x9C, 0x94};
	uint8_t res7[] = {0x01, 0x90, 0x02};
    ASSERT_TRUE(test_rx(req7, SIZE_ARRAY(req7), res7, SIZE_ARRAY(res7))) << msg;

	// ������: ������� ������ � ������� "�� ������ 100" ��������� ��������
	// ���������� EXCEPTION_04H_DEVICE_FAILURE
	uint8_t req8[] = {0x88, 0x10, 0x00, 0x7D, 0x00, 0x02, 0x04,
			0x00, 0x01, 0x00, 0x65, 0x23, 0xA7};
	uint8_t res8[] = {0x88, 0x90, 0x04};
    ASSERT_TRUE(test_rx(req8, SIZE_ARRAY(req8), res8, SIZE_ARRAY(res8))) << msg;

	//  ������: ������� ������ � ������� "�� ������ 1000" ��������� ��������
	// ���������� EXCEPTION_04H_DEVICE_FAILURE
	uint8_t req9[] = {0x88, 0x10, 0x00, 0xC8, 0x00, 0x02, 0x04,
			0x03, 0xEE, 0x03, 0xE7, 0x59, 0xCC};
	uint8_t res9[] = {0x88, 0x90, 0x04};
    ASSERT_TRUE(test_rx(req9, SIZE_ARRAY(req9), res9, SIZE_ARRAY(res9))) << msg;

	// �������� ������ �������� ���-�� ������
	// ���������� EXCEPTION_03H_ILLEGAL_DATA_VAL
	uint8_t req10[] = {0x11, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x19, 0x6D};
	uint8_t res10[] = {0x11, 0x90, 0x03};
    ASSERT_TRUE(test_rx(req10, SIZE_ARRAY(req10), res10, SIZE_ARRAY(res10))) << msg;

	// �������� ������ ���-�� ������� ������������ ��������, 33
	// ���������� EXCEPTION_03H_ILLEGAL_DATA_VAL
	uint8_t req11[] = {0x11, 0x10, 0x00, 0x01, 0x00, 0x21, 0x42,
			 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
			 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
			 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
			 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
			 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
			 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
			 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
			 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
			 0x09, 0x09, 0x19, 0xEC};
	uint8_t res11[] = {0x11, 0x90, 0x03};
    ASSERT_TRUE(test_rx(req11, SIZE_ARRAY(req11), res11, SIZE_ARRAY(res11))) << msg;
}

// �������� ������� ������ ���������� �� ����������
TEST_F(TProtocolModbus_Test, com_0x11_slave_ID) {

	uint8_t req1[] = {0x03, 0x11, 0xC1, 0x4C};
	uint8_t res1[] = {0x03, 0x11, 0x09, 'V','i','r','t','u','a','l', '\0', 0x00};

    ASSERT_TRUE(test_rx(req1, SIZE_ARRAY(req1), res1, SIZE_ARRAY(res1))) << msg;
}

TEST_F(TProtocolModbus_Test, trResponse) {

	// ��������� ������������ ������ � ��������� STATE_WRITE_READY
	uint8_t req1[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
	uint8_t min = TPM::STATE_OFF;
	uint8_t max = TPM::STATE_ERROR;
	for(uint8_t i = min; i < max; i++) {
		readCom(req1, SIZE_ARRAY(req1));
		mb->setAddressLan(req1[0]);
		mb->readData();

		TPM::STATE t = static_cast<TPM::STATE> (i);
		mb->setState(t);

		if ((t == TPM::STATE_WRITE_READY) != (mb->sendData() != 0)) {
			sprintf(msg, "  >>> ������ �� ���� %d", t);
			ASSERT_TRUE(false) << msg;
		}
	}

	// �������� �������� ����������� ����� � ������ ������� 0x01
	uint8_t req2[] = {0x11, 0x01, 0x00, 0x50, 0x00, 0x29, 0xFF, 0x55};
	uint8_t res2[] = {0x11, 0x01, 0x06, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x6E, 0x63};
    ASSERT_TRUE(test_tx(req2, SIZE_ARRAY(req2), res2, SIZE_ARRAY(res2))) << msg;

	// �������� �������� ����������� ����� � ������ ������� 0x03
	uint8_t req3[] = {0x01, 0x03, 0x00, 0x63, 0x00, 0x01, 0x74, 0x14};
	uint8_t res3[] = {0x01, 0x03, 0x02, 0x00, 0x64, 0xB9, 0xAF};
    ASSERT_TRUE(test_tx(req3, SIZE_ARRAY(req3), res3, SIZE_ARRAY(res3))) << msg;

	// �������� �������� ����������� ����� � ������ ������� 0x05
	uint8_t req4[] = {0x01, 0x05, 0x00, 0xEE, 0x00, 0x00, 0xAD, 0xFF};
	uint8_t res4[] = {0x01, 0x05, 0x00, 0xEE, 0x00, 0x00, 0xAD, 0xFF};
    ASSERT_TRUE(test_tx(req4, SIZE_ARRAY(req4), res4, SIZE_ARRAY(res4))) << msg;

	// �������� �������� ����������� ����� � ������ ������� 0x06
	uint8_t req5[] = {0x01, 0x06, 0x00, 0xEE, 0xFF, 0xFF, 0xE8, 0x4F};
	uint8_t res5[] = {0x01, 0x06, 0x00, 0xEE, 0xFF, 0xFF, 0xE8, 0x4F};
    ASSERT_TRUE(test_tx(req5, SIZE_ARRAY(req5), res5, SIZE_ARRAY(res5))) << msg;

	// �������� �������� ����������� ����� � ������ ������� 0x0F
	uint8_t req6[] = {0x01, 0x0F, 0x00, 0x00, 0x01, 0x00, 0x20,
			0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
			0x09, 0x0A, 0x0B, 0x0C, 0xF0, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x8B, 0x8A };
	uint8_t res6[] = {0x01, 0x0F, 0x00, 0x00, 0x01, 0x00, 0x54, 0x5B};
    ASSERT_TRUE(test_tx(req6, SIZE_ARRAY(req6), res6, SIZE_ARRAY(res6))) << msg;

	// �������� �������� ����������� ����� � ������ ������� 0x10
	uint8_t req7[] = {0x01, 0x10, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0xA6, 0x50};
	uint8_t res7[] = {0x01, 0x10, 0x00, 0x00, 0x00, 0x01, 0x01, 0xC9};
    ASSERT_TRUE(test_tx(req7, SIZE_ARRAY(req7), res7, SIZE_ARRAY(res7))) << msg;

	// �������� �������� ����������� ����� � ������ ������� 0x11
	uint8_t req8[] = {0x03, 0x11, 0xC1, 0x4C};
	uint8_t res8[] = {0x03, 0x11, 0x09, 'V','i','r','t','u','a','l', '\0', 0x00, 0xFB, 0x6D};
    ASSERT_TRUE(test_tx(req8, SIZE_ARRAY(req8), res8, SIZE_ARRAY(res8))) << msg;

	// �������� �������� ����������� ����� � ����������
	uint8_t req9[] = {0x11, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x19, 0x6D};
	uint8_t res9[] = {0x11, 0x90, 0x03, 0x0D, 0xC4};
    ASSERT_TRUE(test_tx(req9, SIZE_ARRAY(req9), res9, SIZE_ARRAY(res9))) << msg;

	// �������� �������� ����������� ����� � ������ ������� 0x02
	uint8_t req10[] = {0x01, 0x02, 0x01, 0x2B, 0x00, 0x01, 0xC8, 0x3E};
	uint8_t res10[] = {0x01, 0x02, 0x01, 0x01, 0x60, 0x48};
    ASSERT_TRUE(test_tx(req10, SIZE_ARRAY(req10), res10, SIZE_ARRAY(res10))) << msg;

	// �������� �������� ����������� ����� � ������ ������� 0x04
	uint8_t req11[] = {0x01, 0x04, 0x00, 0x60, 0x00, 0x05, 0x30, 0x17};
	uint8_t res11[] = {0x01, 0x04, 0x0A, 0x00, 0x61, 0x00, 0x62, 0x00, 0x63,
			0x00, 0x64, 0xFF, 0xFF, 0xB3, 0x4E};
    ASSERT_TRUE(test_tx(req11, SIZE_ARRAY(req11), res11, SIZE_ARRAY(res11))) << msg;
}
