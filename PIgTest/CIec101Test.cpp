#include "gtest/gtest.h"
#include <stdint.h>
#include <cstdio>
#include <iostream>

using namespace std;

#define TEST_FRIENDS \
		friend class TIec101_Full_readData_Fix_check_Test; \
		friend class TIec101_Full_readData_Var_check_Test; \
		friend class TIec101_Full_readData_Other_Test; \
		friend class TIec101_Full_readData_reset_Test; \
		friend class TIec101_Full_readData_TimeSynch_Test; \
		friend class TIec101_Full_readData_Iterrog_Test; \
		friend class TIec101_Full_readData_Events_Class2_Test; \
		friend class TIec101_Full_readData_Events_Class1_Test

#include "protocols/iec101/CIec101.h"

// ������ ��������
static const uint8_t SIZE = 64;

// ����� ���������
static char msg[1000];

// ������� ������ � ������ ���������
static uint16_t posMsg = 0;

// ������� ����� � ������������� ������
static const uint8_t FRAME_START_CHARACTER_FIX = 0x10;

// ������� ����� � ���������� ������
static const uint8_t FRAME_START_CHARACTER_VAR = 0x68;

// ������� ��������� �����
static const uint8_t FRAME_STOP_CHARCTER = 0x16;

/**	����� ������� � ������ ������ ��������� � ����������� �������� �������.
 *
 * 	@param[in] name ������.
 */
static void printTestName(const char name[]) {
	posMsg = 0;
	posMsg += sprintf(&msg[posMsg], name);
}

/**	����� ������� � ����� ���������.
 *
 *	� �������� ������ ������� ����������� ����� ��������� \a msg � ����������
 *	������� ������ � ���.
 *
 *	@param[in] name ��� �������.
 *	@param[in] buf ������.
 *	@param[in] size ������ �������.
 */
static void printMassive(const char name[], const uint8_t buf[], uint8_t size) {
	posMsg += sprintf(&msg[posMsg], " %s[] = ", name);
	for(uint8_t i = 0; i < size; i++) {
		posMsg += sprintf(&msg[posMsg], "0x%02X ", buf[i]);
	}
	posMsg += sprintf(&msg[posMsg], "\n");
}

/**	��������� ���� ��������.
 *
 * @param[in] mas ���������� ������.
 * @param[in] masSize ������ ����������� �������.
 * @param[in] exp ������ ���������.
 * @param[in] expSize ������ ���������� �������.
 *
 * @retval True ���� ���������� ������ ������ � ���������.
 * @retval False ���� ���������� ������ �� ������ � ���������.
 */
bool checkMass(const uint8_t mas[], uint8_t masSize, const uint8_t exp[], uint8_t expSize) {
	bool state = true;

	if (masSize != expSize) {
		posMsg += sprintf(&msg[posMsg], " ������ ����������� ������� %d, ������ %d",
				masSize, expSize);
		state = false;
	} else {
		for(uint8_t i = 0; i < masSize; i++) {
			if (mas[i] != exp[i]) {
				posMsg += sprintf(&msg[posMsg], " ������ ����� mas[%d] = %02X, ������ %02X",
						i, mas[i], exp[i]);
				state = false;
				break;
			}
		}
	}

	if (!state) {
		printMassive("mas", mas, masSize);
	}

	return state;
}

/**	���������� ��������� � ����� ���������.
 *
 *	� ����� ��������� ���������� �������� ���-�� ������ \a size �� ������� \a mas,
 *	� ����� ����������� ���� ������� ��������� ���������.
 *
 *	@param[in] iec101 - �������� 101.
 *	@param[in] mas - ������ ������ ��� ��������� � �������� 101.
 *	@param[in] size - ���-�� ������ ��� ��������� � �������� 101.
 */
void putData(CIec101 *iec101, uint8_t *mas, uint8_t size) {
	iec101->setReadState();
	for(uint8_t i = 0; i < size; i++) {
		iec101->push(mas[i]);
	}
	iec101->setState(CIec101::STATE_READ_OK);
}

class TIec101_Full: public ::testing::Test {

#define CONTROL(code, fcb) (uint8_t) (0x40 + code + getFcb(fcb))

#define FIX_FRAME_IN(code, fcb) \
		{ 								\
	FRAME_START_CHARACTER_FIX, 	\
	CONTROL(code, fcb),			\
	0x01, 						\
	(uint8_t) (CONTROL(code, false) + 0x01),	\
	FRAME_STOP_CHARCTER 		\
		};


public:
	uint8_t buf[SIZE];	// ����� ������
	CIec101 *iec101;
	uint8_t fcb; // ��� ����� ������


	/**	����� ����������.
	 *
	 * 	��������� ���� ������ ������ � fcb = 0.
	 */
	void resetRemoteLink() {
		{
			printTestName(" ������ � ��c������ ������. \n");
			uint8_t mas[] = FIX_FRAME_IN(9, false); // REQUEST_STATUS_OF_LINK
			uint8_t resp[] = {FRAME_START_CHARACTER_FIX, 11, 0x01, 0x0C, FRAME_STOP_CHARCTER}; // RESPOND_STATUS_OF_LINK
			putData(iec101, mas, sizeof(mas));
			iec101->readData();
			ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
			ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
			ASSERT_TRUE(!iec101->isReset());
		}

		{
			printTestName(" ����� ���������� ������. \n");
			uint8_t mas[] = FIX_FRAME_IN(0, false); // RESET_REMOTE_LINK
			uint8_t resp[] = {FRAME_START_CHARACTER_FIX, 0x00, 0x01, 0x01, FRAME_STOP_CHARCTER}; // CONFIRM_ACK
			putData(iec101, mas, sizeof(mas));
			iec101->readData();
			ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
			ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
			ASSERT_TRUE(iec101->isReset()) << msg;
		}

		{
			printTestName(" ������ ������ ������ 1 � �������� ����� ����� ������. \n");
			uint8_t mas[] = FIX_FRAME_IN(10, false); // REQUEST_USER_DATA_CLASS_1
			uint8_t resp[] = {FRAME_START_CHARACTER_FIX, 0x09, 0x01, 0x0A, FRAME_STOP_CHARCTER}; // RESPOND_NACK
			putData(iec101, mas, sizeof(mas));
			iec101->readData();
			ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
			ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
			ASSERT_TRUE(iec101->isReset()) << msg;
		}

		{
			printTestName(" ������ ������ ������ 1 � ������ ����� ����� ������. \n");
			uint8_t mas[] = FIX_FRAME_IN(10, true); // REQUEST_USER_DATA_CLASS_1, fcb = 1
			printMassive("MAS", mas, sizeof(mas));
			uint8_t resp[] =  {
					FRAME_START_CHARACTER_VAR, 0x09, 0x09, FRAME_START_CHARACTER_VAR,
					0x08, 		// control = RESPOND_USER_DATA, prm = 0, acd = 0
					0x01,		// link address
					0x46,		// TYPE_ID_M_EI_NA_1
					0x01,		// variableStructureQualifier
					0x04,		// COT = COT_INIT
					0x01,		// common address
					0x00, 0x00,	// information object address
					0x01,		// COI = COI_LOCAL_MANUAL_RESET
					0x56, 		// crc
					FRAME_STOP_CHARCTER};
			putData(iec101, mas, sizeof(mas));
			iec101->readData();
			ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
			ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
			ASSERT_TRUE(iec101->isReset()) << msg;
		}
	};

	/**	���������� ����� �������� ���� ����� ������.
	 *
	 *	���������� ����� ���������� 1 (��� ������).
	 *
	 * 	@param v ���� True - ���������� ����� ��������, False - ����������.
	 * 	@retval 0x20
	 * 	@retval 0x00
	 */
	uint8_t getFcb(bool v) {
		if (v) {
			fcb = (fcb == 0x30) ? 0x10 : 0x30;
		}
		return fcb;
	}

	/**	������� ����������� ����� ����� ���������� �����.
	 *
	 * 	@param[in] mas ������ ������
	 * 	@return ����������� �����.
	 */
	uint8_t getCrcVarLenght(uint8_t mas[]) {
		uint8_t crc = 0;
		uint8_t len = mas[1];

		for(uint8_t i = 0; i < len; i++) {
			crc += mas[4 + i];
		}
		return crc;
	}

private:
	virtual void SetUp() {
		fcb = 0;
		posMsg = 0;
		msg[0] = 0x00;
		iec101 = new CIec101(buf, SIZE);

	};

	virtual void TearDown() {
		delete iec101;
	};
};

TEST_F(TIec101_Full, constants) {
	ASSERT_EQ(0, 	iec101->getAddressError());
	ASSERT_EQ(1, 	iec101->getAddressMin());
	ASSERT_EQ(247, 	iec101->getAddressMax());
	ASSERT_EQ(255, 	iec101->getAddressGlb());
}

TEST_F(TIec101_Full, adressLan) {
	uint8_t adrError = iec101->getAddressError();
	uint8_t adrMin = iec101->getAddressMin();
	uint8_t adrMax = iec101->getAddressMax();

	// 1. �������� ������ ��-���������.
	if (iec101->getAddressLan() != 0) {
		posMsg += sprintf(&msg[posMsg],
				"  >>> ����� �� ��������� �� ��������� � ADDRESS_ERR.");
		ASSERT_TRUE(false) << msg;
	}

	// 2. �������� ���������/���������� ������.
	for (uint16_t i = 0; i <= 255; i++) {
		bool t = iec101->setAddressLan(i);

		// �������� ������������� �������� �������� serAddress())
		if (((i < adrMin) || (i > adrMax)) == t) {
			posMsg += sprintf(&msg[posMsg],
					"  >>> setAddress() ���������� ��������� ��������� �� ���� %d", i);
			ASSERT_TRUE(false) << msg;
		}

		// �������� �������������� ��������
		if (i < adrMin) {
			if (iec101->getAddressLan() != adrError) {
				posMsg += sprintf(&msg[posMsg],
						"  >>> �������� ������ ADDRESS_MIN �� ����� � ADDRESS_ERR �� ���� %d", i);
				ASSERT_TRUE(false) << msg;
			}
		} else if (i > adrMax) {
			if (iec101->getAddressLan() != adrMax) {
				posMsg += sprintf(&msg[posMsg],
						"  >>> �������� ������ ADDRESS_MAX �� ����� � ADDRESS_MAX �� ���� %d", i);
				ASSERT_TRUE(false) << msg;
			}
		} else if (i != iec101->getAddressLan()) {
			posMsg += sprintf(&msg[posMsg],
					"  >>> ������������� ����� �� ��������� �� ��������� �� ���� %d", i);
			ASSERT_TRUE(false) << msg;
		}
	}
}

TEST_F(TIec101_Full, setState_and_checkState) {
	// ������������ ������� ��������� � �������� �������� ���������.

	uint8_t min = CIec101::STATE_OFF;
	uint8_t max = CIec101::STATE_ERROR;

	for(uint8_t i = min; i < max; i++) {
		CIec101::EState t = static_cast<CIec101::EState> (i);
		iec101->setState(t);
		if (!iec101->checkState(t)) {
			posMsg += sprintf(&msg[posMsg],"  >>> ������ �� ���� %d", t);
			ASSERT_TRUE(false) << msg;
		}
	}
}

TEST_F(TIec101_Full, getState) {
	uint8_t min = CIec101::STATE_OFF;
	uint8_t max = CIec101::STATE_ERROR;

	for(uint8_t i = min; i < max; i++) {
		CIec101::EState t = static_cast<CIec101::EState> (i);
		iec101->setState(t);
		if (t != iec101->getState()) {
			posMsg += sprintf(&msg[posMsg], "  >>> ������ �� ���� %d", i);
			ASSERT_TRUE(false) << msg;
		}
	}
}

TEST_F(TIec101_Full, setEnable) {
	iec101->setEnable();
	ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ));
}

TEST_F(TIec101_Full, setDisable) {
	iec101->setEnable();
	iec101->setDisable();
	ASSERT_TRUE(iec101->checkState(CIec101::STATE_OFF));
	ASSERT_TRUE(!iec101->isReset());
}

TEST_F(TIec101_Full, isEnable) {
	iec101->setEnable();
	ASSERT_TRUE(iec101->isEnable());
	iec101->setDisable();
	ASSERT_TRUE(!iec101->isEnable());
}

TEST_F(TIec101_Full, setReadState) {
	iec101->setReadState();
	ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ));
}

TEST_F(TIec101_Full, isReadData) {
	uint8_t min = CIec101::STATE_OFF;
	uint8_t max = CIec101::STATE_ERROR;

	for(uint8_t i = min; i < max; i++) {
		CIec101::EState t = static_cast<CIec101::EState> (i);
		iec101->setState(t);
		if ((t == CIec101::STATE_READ_OK) != iec101->isReadData()) {
			posMsg += sprintf(&msg[posMsg], "  >>> ������ �� ���� %d", t);
			ASSERT_TRUE(false) << msg;
		}
	}

	iec101->setDisable();
	ASSERT_TRUE(!iec101->isReadData());
	iec101->setReadState();
	ASSERT_TRUE(!iec101->isReadData());
}

TEST_F(TIec101_Full, setTick) {
	ASSERT_EQ(17, iec101->setTick(19200, 50));
	ASSERT_EQ(8, iec101->setTick(19200, 100));
	ASSERT_EQ(1100, iec101->setTick(300, 50));
	ASSERT_EQ(550, iec101->setTick(300, 100));
}

TEST_F(TIec101_Full, getDelay) {
	iec101->setReadState();
	iec101->setTick(19200, 50);
	ASSERT_EQ(46, iec101->getDelay());	// 12 �� ��, 2 ������ ��, 32 �� ���
	for(uint8_t i = 0; i < 20; i++)
		iec101->tick();
	ASSERT_EQ(47, iec101->getDelay());	// 12 �� ��, 2+1 ������ ��, 32 �� ���

	iec101->setReadState();
	iec101->setTick(19200, 100);
	ASSERT_EQ(46, iec101->getDelay());	// 12 �� ��, 2 ������ ��, 32 �� ���
	for(uint8_t i = 0; i < 80; i++)
		iec101->tick();
	ASSERT_EQ(54, iec101->getDelay());	// 12 �� ��, 2+8 ������ ��, 32 �� ���

	iec101->setReadState();
	iec101->setTick(300, 50);
	ASSERT_EQ(804, iec101->getDelay());	// 770 �� ��, 2 ������ ��, 32 �� ���
	for(uint8_t i = 0; i < 80; i++)
		iec101->tick();
	ASSERT_EQ(808, iec101->getDelay());	// 770 �� ��, 2+4 ������ ��, 32 �� ���

}

TEST_F(TIec101_Full, push) {

	iec101->setState(CIec101::STATE_READ);

	// 1. �������� ������ ����������� ���������� ���-�� ���� ������
	// ��� �������� ������������ tick
	for (uint16_t i = 1; i <= 2 * sizeof(buf); i++) {
		uint16_t t = iec101->push(i);
		if (i <= sizeof(buf)) {
			if (t != i) {
				posMsg += sprintf(&msg[posMsg],
						"  <<< �������� ���������� �������� ���� �� ���� %d", i);
				ASSERT_TRUE(false) << msg;
			}
		} else {
			if (t != sizeof (buf)) {
				posMsg += sprintf(&msg[posMsg],
						"  <<< ������������ ������ �� ���� %d", i);
				ASSERT_TRUE(false) << msg;
			}
		}
	}

	// 2. �������� ������ ������� ��� ����� 1.5 ���������.
	struct sData {
		CIec101::EState stateStart;	// ��������� ��������� � ������
		uint8_t numOfPush;			// ���-�� �������� ������
		uint8_t stepToReset;		// ���, ����� �������� ������� ����������� ����� ������
		uint8_t numOfByte;			// ���-�� �������� ���� �� ���������
	};

	sData data[] = {
			{CIec101::STATE_OFF, 		10, 11,	0},
			{CIec101::STATE_OFF, 		10, 5, 	0},
			{CIec101::STATE_READ, 		10, 10,	10},
			{CIec101::STATE_READ, 		10, 18,	10},
			{CIec101::STATE_READ, 		10, 2,	2},
			{CIec101::STATE_READ_ERROR,	10, 18,	0},
			{CIec101::STATE_READ_OK, 	10, 18,	0},
			{CIec101::STATE_WRITE_WAIT,	10, 18,	0},
			{CIec101::STATE_WRITE, 		10, 21,	0}
	};

	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {
		iec101->setTick(57600, 100);
		iec101->setState(CIec101::STATE_READ);	// ��� ������ ��������
		iec101->setState(data[i].stateStart);	// ������ ��� ����� ���������

		uint8_t num = 0;
		for (uint8_t j = 0; j < data[i].numOfPush; j++) {
			// ������������ ����� ������
			if (j == (data[i].stepToReset)) {
				iec101->setState(CIec101::STATE_READ_OK);
			}
			num = iec101->push(0x00);
		}

		if (num != data[i].numOfByte) {
			posMsg += sprintf(&msg[posMsg],
					"  >>> ��������� ���-�� �������� ���� �� ���� %d", i);
			posMsg += sprintf(&msg[posMsg],
					"\n num = %d, need = %d", num, data[i].numOfByte);
			ASSERT_TRUE(false) << msg;
		}
	}
}

TEST_F(TIec101_Full, tick) {

	struct sData {
		CIec101::EState startState;	// ��������� ��������� ���������
		CIec101::EState stopState;	// �������� ��������� ���������
		uint8_t	 numBytes;			// ���-�� ���� ������ � ������
		uint16_t baudrate;			// �������� ������ �����
		uint8_t  period; 			// ������ ������ ������� tick()
		uint16_t numTicks; 			// ���-�� �����

	};

	uint8_t mas[] = FIX_FRAME_IN(3, false);	// USER_DATA_CONFIRM

	sData data[] = {
			// �������� �� ���-�� �������� ������, ������ N ��������� �� ��������
			{CIec101::STATE_READ, 		CIec101::STATE_READ, 		2, 57600, 50, 35},
			{CIec101::STATE_READ, 		CIec101::STATE_READ,	 	3, 57600, 50, 35},
			{CIec101::STATE_READ, 		CIec101::STATE_READ,	 	4, 57600, 50, 35},
			// �������� �� ����������� ��������� ���������� ���������
			{CIec101::STATE_READ, 		CIec101::STATE_READ_OK, 	5, 19200, 50, 40},
			{CIec101::STATE_READ, 		CIec101::STATE_READ, 		5, 19200, 50, 16},
			{CIec101::STATE_READ, 		CIec101::STATE_READ_OK,		5, 19200, 50, 17},
			{CIec101::STATE_READ, 		CIec101::STATE_READ_OK, 	5, 19200, 50, 41},
			// �������� ������ � ��������� ����������� ������
			{CIec101::STATE_READ_ERROR, CIec101::STATE_READ, 		5, 19200, 50, 34},
			{CIec101::STATE_READ_ERROR, CIec101::STATE_READ_ERROR,	5, 19200, 50, 33},
			// �������� �� ������ � ���������� �� STATE_READ
			{CIec101::STATE_OFF, 		CIec101::STATE_OFF, 		5, 19200, 50, 50},
			{CIec101::STATE_READ_OK, 	CIec101::STATE_READ_OK, 	5, 19200, 50, 50},
			{CIec101::STATE_WRITE_WAIT, CIec101::STATE_WRITE_WAIT, 	5, 19200, 50, 50},
			{CIec101::STATE_WRITE, 		CIec101::STATE_WRITE, 		5, 19200, 50, 50},
	};

	iec101->setAddressLan(1);
	for (uint16_t i = 0; i < (sizeof (data) / sizeof (data[0])); i++) {

		uint16_t step = iec101->setTick(data[i].baudrate, data[i].period);
		iec101->setState(CIec101::STATE_READ);	// ��� ������ ��������
		iec101->setState(data[i].startState);	// ������ ��� ����� ���������

		// ������� � ����� �������� ���-�� ���� ������
		for(uint8_t j = 0; j < data[i].numBytes; j++) {
			iec101->push(mas[j]);
		}

		for (uint8_t j = 0; j < data[i].numTicks; j++) {
			iec101->tick();
		}

		CIec101::EState state = iec101->getState();
		if (state != data[i].stopState) {
			posMsg += sprintf(&msg[posMsg], "  >>> ������ �� ���� %d", i);
			posMsg += sprintf(&msg[posMsg], "\n start = %d,  finish = %d, need = %d",
					data[i].startState, state, data[i].stopState);
			posMsg += sprintf(&msg[posMsg], "\n step tick = %d", step);
			ASSERT_TRUE(false) << msg;
		}
	}
}

TEST_F(TIec101_Full, getNumOfBytes) {
	// �������� �� ���������
	ASSERT_EQ(0, iec101->getNumOfBytes());

	iec101->setState(CIec101::STATE_READ);	// ��� ������ ��������
	// �������� ��� ���-�� ���� ������ �� ����� ������� ������
	for(uint8_t i = 0; i < SIZE; i++) {
		iec101->push(0x00);
		if ((i + 1) != iec101->getNumOfBytes()) {
			posMsg += sprintf(&msg[posMsg], "  >>> ������ �� ���� %d", i);
			posMsg += sprintf(&msg[posMsg], "\n num = %d, need = %d",
					iec101->getNumOfBytes(), i + 1);
			ASSERT_TRUE(false) << msg;
		}
	}

	// �������� ��� ������ �� ������� ������ ������ (�� ��� ��������)
	for(uint8_t i = 0; i < SIZE; i++) {
		iec101->push(0x00);
		if (SIZE != iec101->getNumOfBytes()) {
			posMsg += sprintf(&msg[posMsg], "  >>> ������ �� ���� %d", i);
			posMsg += sprintf(&msg[posMsg], "\n num = %d, need = %d",
					iec101->getNumOfBytes(), SIZE);
			ASSERT_TRUE(false) << msg;
		}
	}
}

TEST_F(TIec101_Full, readData_Fix_check) {
	// �������� ����� � ������������� ������.

	{
		printTestName(" ��������� ���� ����������. \n");
		uint8_t mas[] = {FRAME_START_CHARACTER_FIX, 0x80, 0x01, 0x81, FRAME_STOP_CHARCTER};
		putData(iec101, mas, sizeof(mas));
		iec101->setAddressLan(1);
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_PRM_BITE) << msg;
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ_ERROR)) << msg;
	}

	{
		printTestName(" ��������� �����. \n");
		uint8_t mas[] = {FRAME_START_CHARACTER_FIX, 0x40, 0x00, 0x40, FRAME_STOP_CHARCTER};
		putData(iec101, mas, sizeof(mas));
		iec101->setAddressLan(1);
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_LINK_ADDRESS) << msg;
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ_ERROR)) << msg;
	}

	{
		printTestName(" ��������� �����. \n");
		uint8_t mas[] = {FRAME_START_CHARACTER_FIX, 0x40, 0xF8, 0x38, FRAME_STOP_CHARCTER};
		putData(iec101, mas, sizeof(mas));
		iec101->setAddressLan(1);
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_LINK_ADDRESS) << msg;
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ_ERROR)) << msg;
	}

	{
		printTestName(" ��������� ��. \n");
		uint8_t mas[] = {FRAME_START_CHARACTER_FIX, 0x40, 0x01, 0x42, FRAME_STOP_CHARCTER};
		putData(iec101, mas, sizeof(mas));
		iec101->setAddressLan(1);
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_CRC) << msg;
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ_ERROR)) << msg;
	}

	{
		printTestName(" ��������� ����� ��������� . \n");
		uint8_t mas[] = {FRAME_START_CHARACTER_FIX, 0x40, 0x01, 0x41, FRAME_STOP_CHARCTER - 1};
		putData(iec101, mas, sizeof(mas));
		iec101->setAddressLan(1);
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_STOP_CHAR) << msg;
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ_ERROR)) << msg;
	}

	{
		// ��� ����� ���� ��������� ������ !!!
		printTestName(" ����� ����� ������ ���������� �� 1. \n");
		uint8_t mas[] = {FRAME_START_CHARACTER_FIX, 0x40, 0x01,  FRAME_STOP_CHARCTER};
		putData(iec101, mas, sizeof(mas));
		iec101->setAddressLan(1);
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_COMMON) << msg;
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ_ERROR)) << msg;
	}

	{
		printTestName(" ����� ����� ������ ���������� �� 1. \n");
		uint8_t mas[] = {FRAME_START_CHARACTER_FIX, 0x40, 0x01, 0x41, FRAME_STOP_CHARCTER, 0x00};
		putData(iec101, mas, sizeof(mas));
		iec101->setAddressLan(1);
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_SIZE_FRAME) << msg;
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ_ERROR)) << msg;
	}
}

TEST_F(TIec101_Full, readData_Var_check) {
	// �������� ����� � ���������� ������.
	{
		printTestName(" ��������� ���� ����������. \n");
		uint8_t mas[] = {
				FRAME_START_CHARACTER_VAR, 0x09, 0x09, FRAME_START_CHARACTER_VAR,
				0x33, 0x01, // control + link address
				0x64, 0x01, // address (2 bytes) + siq
				0x06, 0x01, 0x00, 0x00, 0x14,
				0xB4, // crc
				FRAME_STOP_CHARCTER
		};
		putData(iec101, mas, sizeof(mas));
		iec101->setAddressLan(1);
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_PRM_BITE) << msg;
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ_ERROR)) << msg;
	}

	{
		printTestName(" ��������� �����. \n");
		uint8_t mas[] = {
				FRAME_START_CHARACTER_VAR, 0x09, 0x09, FRAME_START_CHARACTER_VAR,
				0x73, 0x02, // control + link address
				0x64, 0x01, // address (2 bytes) + siq
				0x06, 0x01, 0x00, 0x00, 0x14,
				0xF5, // crc
				FRAME_STOP_CHARCTER
		};
		putData(iec101, mas, sizeof(mas));
		iec101->setAddressLan(1);
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_LINK_ADDRESS) << msg;
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ_ERROR)) << msg;
	}

	{
		printTestName(" ��������� ��. \n");
		uint8_t mas[] = {
				FRAME_START_CHARACTER_VAR, 0x09, 0x09, FRAME_START_CHARACTER_VAR,
				0x73, 0x01, // control + link address
				0x64, 0x01, // address (2 bytes) + siq
				0x06, 0x01, 0x00, 0x00, 0x14,
				0xF4 + 1, // crc
				FRAME_STOP_CHARCTER
		};
		putData(iec101, mas, sizeof(mas));
		iec101->setAddressLan(1);
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_CRC) << msg;
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ_ERROR)) << msg;
	}

	{
		printTestName(" ��������� ����� ��������� . \n");
		uint8_t mas[] = {
				FRAME_START_CHARACTER_VAR, 0x09, 0x09, FRAME_START_CHARACTER_VAR,
				0x73, 0x01, // control + link address
				0x64, 0x01, // address (2 bytes) + siq
				0x06, 0x01, 0x00, 0x00, 0x14,
				0xF4, // crc
				FRAME_STOP_CHARCTER + 1
		};
		putData(iec101, mas, sizeof(mas));
		iec101->setAddressLan(1);
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_STOP_CHAR) << msg;
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ_ERROR)) << msg;
	}

	{
		printTestName(" ��������� �������� ��������� �����. \n");
		uint8_t mas[] = {
				FRAME_START_CHARACTER_VAR, 0x09, 0x09, FRAME_START_CHARACTER_VAR + 1,
				0x73, 0x01, // control + link address
				0x64, 0x01, // address (2 bytes) + siq
				0x06, 0x01, 0x00, 0x00, 0x14,
				0xF4, // crc
				FRAME_STOP_CHARCTER
		};
		putData(iec101, mas, sizeof(mas));
		iec101->setAddressLan(1);
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_START2_CHAR) << msg;
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ_ERROR)) << msg;
	}

	{
		printTestName(" ��������� ������ ���� ����� ���������������� ������. \n");
		uint8_t mas[] = {
				FRAME_START_CHARACTER_VAR, 0x08, 0x09, FRAME_START_CHARACTER_VAR,
				0x73, 0x01, // control + link address
				0x64, 0x01, // address (2 bytes) + siq
				0x06, 0x01, 0x00, 0x00, 0x14,
				0xF4, // crc
				FRAME_STOP_CHARCTER
		};
		putData(iec101, mas, sizeof(mas));
		iec101->setAddressLan(1);
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_LENGHT2) << msg;
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ_ERROR)) << msg;
	}

	{
		printTestName(" ��������� ������ ���� ����� ���������������� ������. \n");
		uint8_t mas[] = {
				FRAME_START_CHARACTER_VAR, 0x09, 0x08, FRAME_START_CHARACTER_VAR,
				0x73, 0x01, // control + link address
				0x64, 0x01, // address (2 bytes) + siq
				0x06, 0x01, 0x00, 0x00, 0x14,
				0xF4, // crc
				FRAME_STOP_CHARCTER
		};
		putData(iec101, mas, sizeof(mas));
		iec101->setAddressLan(1);
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_LENGHT2) << msg;
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ_ERROR)) << msg;
	}

	{
		printTestName(" ��������� ���� ����� ���������������� ������, �� ����������. \n");
		uint8_t mas[] = {
				FRAME_START_CHARACTER_VAR, 0x08, 0x08, FRAME_START_CHARACTER_VAR,
				0x73, 0x01, // control + link address
				0x64, 0x01, // address (2 bytes) + siq
				0x06, 0x01, 0x00, 0x00, 0x14,
				0xF4, // crc
				FRAME_STOP_CHARCTER
		};
		putData(iec101, mas, sizeof(mas));
		iec101->setAddressLan(1);
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_LENGHT1) << msg;
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ_ERROR)) << msg;
	}

	{
		printTestName(" ����� ����� ������ ���������� ����������. \n");
		uint8_t mas[] = {
				FRAME_START_CHARACTER_VAR, 0x08, 0x08, FRAME_START_CHARACTER_VAR,
				0x73, 0x01, // control + link address
				0x64, 0x01, // address (2 bytes) + siq
				0x06, 0x01, 0x00,0x14,
				0xF4, // crc
				FRAME_STOP_CHARCTER
		};
		putData(iec101, mas, sizeof(mas));
		iec101->setAddressLan(1);
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_SIZE_FRAME) << msg;
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ_ERROR)) << msg;
	}
}

TEST_F(TIec101_Full, readData_Other) {
	// �������� ������������ ����� (����������� ������ ������ ���� ������).
	// �� ������� ���� ����������� ����������� ������� ?!
	for(uint8_t i = 0; i < 255; i++) {
		iec101->setReadState();
		iec101->push(i);
		iec101->setState(CIec101::STATE_READ_OK);
		if (iec101->checkFrame() == CIec101::ERROR_NO) {
			posMsg += sprintf(&msg[posMsg], "  >>> ������ �� ���� %d", i);
			ASSERT_TRUE(false) << msg;
		}
	}
}

TEST_F(TIec101_Full, readData_reset) {
	// �������� �� ������������� ������ ��� ������ ������ ���������

	iec101->setEnable();

	ASSERT_TRUE(!iec101->isReset()) << " �������� ������ ������� ������, ����� ��������� "<< endl;
	iec101->setAddressLan(1);

	// �������� ������ ������������� ������, ��� ������ �� ������ ���������.
	// TODO ��� ��������� ��������� ��� ���. ����������� ?! ���� �������� ���� ������
	// �� ���������� ������ �� ��������, �� ���� ���� � ����� �� ��������� ������.
	{
		printTestName(" ���������������� ������ � �������. \n");
		uint8_t mas[] = FIX_FRAME_IN(3, false);	// USER_DATA_CONFIRM
		putData(iec101, mas, sizeof(mas));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg;
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ)) << msg << " " << iec101->getState();
		ASSERT_TRUE(!iec101->isReset()) << msg;
	}

	{
		printTestName(" ���������������� ������ ��� ������. \n");
		uint8_t mas[] = FIX_FRAME_IN(4, false);	// USER_DATA_NO_REPLY
		putData(iec101, mas, sizeof(mas));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg;
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ)) << msg << " " << iec101->getState();
		ASSERT_TRUE(!iec101->isReset()) << msg;
	}

	{
		printTestName(" ������ ������ ������ 1. \n");
		uint8_t mas[] = FIX_FRAME_IN(10, false);	// REQUEST_USER_DATA_CLASS_1
		putData(iec101, mas, sizeof(mas));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg;
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ)) << msg << " " << iec101->getState();
		ASSERT_TRUE(!iec101->isReset()) << msg;
	}

	{
		printTestName(" ������ ������ ������ 2. \n");
		uint8_t mas[] = FIX_FRAME_IN(10, false);	// REQUEST_USER_DATA_CLASS_2
		putData(iec101, mas, sizeof(mas));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg;
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ)) << msg << " " << iec101->getState();
		ASSERT_TRUE(!iec101->isReset()) << msg;
	}

	// �������� ������ ���������� �����. �� ������������� �� ������ ��������!
	// �������� ������ ����������� �� ���������� ��������!
	{
		// TODO ���������� ������
		printTestName(" ����� ���������� ������. \n");
		uint8_t mas[] = {
				FRAME_START_CHARACTER_VAR, 0x09, 0x09, FRAME_START_CHARACTER_VAR,
				0x70 + 0, 0x01, // control + link address RESET_REMOTE_LINK
				0x00, 0x01, 0x06, // address (2 bytes) + siq
				0x01, 0x00, 0x00, 0x14,
				0x8D + 0, // crc
				FRAME_STOP_CHARCTER
		};
		putData(iec101, mas, sizeof(mas));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg;
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ)) << msg << " " << iec101->getState();
		ASSERT_TRUE(!iec101->isReset()) << msg;
	}

	{
		// TODO ���������� ������
		printTestName(" ���������������� ������ � �������. \n");
		uint8_t mas[] = {
				FRAME_START_CHARACTER_VAR, 0x09, 0x09, FRAME_START_CHARACTER_VAR,
				0x70 + 3, 0x01, // control + link address USER_DATA_CONFIRM
				0x00, 0x01, 0x06, // address (2 bytes) + siq
				0x01, 0x00, 0x00, 0x14,
				0x8D + 3, // crc
				FRAME_STOP_CHARCTER
		};
		putData(iec101, mas, sizeof(mas));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg;
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ)) << msg << " " << iec101->getState();
	}

	{
		// TODO ���������� ������
		printTestName(" ���������������� ������ ��� ������. \n");
		uint8_t mas[] = {
				FRAME_START_CHARACTER_VAR, 0x09, 0x09, FRAME_START_CHARACTER_VAR,
				0x70 + 4, 0x01, // control + link address USER_DATA_NO_REPLY
				0x00, 0x01, 0x06, // address (2 bytes) + siq
				0x01, 0x00, 0x00, 0x14,
				0x8D + 4, // crc
				FRAME_STOP_CHARCTER
		};
		putData(iec101, mas, sizeof(mas));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg;
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ)) << msg << " " << iec101->getState();
	}

	{
		// TODO ���������� ������
		printTestName(" ������ � ��c������ ������. \n");
		uint8_t mas[] = {
				FRAME_START_CHARACTER_VAR, 0x09, 0x09, FRAME_START_CHARACTER_VAR,
				0x70 + 9, 0x01, // control + link address REQUEST_STATUS_OF_LINK
				0x00, 0x01, 0x06, // address (2 bytes) + siq
				0x01, 0x00, 0x00, 0x14,
				0x8D + 9, // crc
				FRAME_STOP_CHARCTER
		};
		putData(iec101, mas, sizeof(mas));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg;
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ)) << msg << " " << iec101->getState();
		ASSERT_TRUE(!iec101->isReset()) << msg;
	}

	{
		// TODO ���������� ������
		printTestName(" ������ ������ ������ 1. \n");
		uint8_t mas[] = {
				FRAME_START_CHARACTER_VAR, 0x09, 0x09, FRAME_START_CHARACTER_VAR,
				0x70 + 10, 0x01, // control + link address REQUEST_USER_DATA_CLASS_1
				0x00, 0x01, 0x06, // address (2 bytes) + siq
				0x01, 0x00, 0x00, 0x14,
				0x8D + 10, // crc
				FRAME_STOP_CHARCTER
		};
		putData(iec101, mas, sizeof(mas));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg;
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ)) << msg << " " << iec101->getState();
		ASSERT_TRUE(!iec101->isReset()) << msg;
	}

	{
		// TODO ���������� ������
		printTestName(" ������ ������ ������ 2. \n");
		uint8_t mas[] = {
				FRAME_START_CHARACTER_VAR, 0x09, 0x09, FRAME_START_CHARACTER_VAR,
				0x70 + 11, 0x01, // control + link address REQUEST_USER_DATA_CLASS_2
				0x00, 0x01, 0x06, // address (2 bytes) + siq
				0x01, 0x00, 0x00, 0x14,
				0x8D + 11, // crc
				FRAME_STOP_CHARCTER
		};
		putData(iec101, mas, sizeof(mas));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg;
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_READ)) << msg << " " << iec101->getState();
		ASSERT_TRUE(!iec101->isReset()) << msg;
	}

	// �������� ������ �������������
	resetRemoteLink();

	// �������� ������ ����� ��������� ��� ���������� ���������
	iec101->setDisable();
	ASSERT_TRUE(!iec101->isReset()) << "�� ������������ ���� ������ ��� ���������� ���������";
}

TEST_F(TIec101_Full, readData_TimeSynch) {
	// ������������� �����

	iec101->setEnable();
	iec101->setAddressLan(1);
	resetRemoteLink();
	ASSERT_TRUE(iec101->isReset()) << " �������� �� �������. ";

	{
		printTestName(" �������� ������� ���������� �������. \n");
		uint8_t req[] =  {
				FRAME_START_CHARACTER_VAR, 0x0F, 0x0F, FRAME_START_CHARACTER_VAR,
				(uint8_t) (0x43 + getFcb(false)), 	// control = USER_DATA_CONFIRM, fcb = 1, fcv = 1, prm = 1
				0x01,		// link address
				0x67,		// TYPE_ID_C_CS_NA_1
				0x01,		// variableStructureQualifier
				0x06,		// COT = COT_ACT
				0x01,		// common address
				0x00, 0x00,	// information object address
				0x39, 0x30,	// ms
				0x06,		// min
				0x07,		// hours
				0x08,		// days
				0x09,		// months
				0x0A,		// years
				(uint8_t) (0x44 + + getFcb(false)), 		// crc
				FRAME_STOP_CHARCTER};
		uint8_t resp[] =  {
				FRAME_START_CHARACTER_VAR, 0x09, 0x09, FRAME_START_CHARACTER_VAR,
				0x08, 		// control = RESPOND_USER_DATA, prm = 0, acd = 0
				0x01,		// link address
				0x46,		// TYPE_ID_M_EI_NA_1
				0x01,		// variableStructureQualifier
				0x04,		// COT = COT_INIT
				0x01,		// common address
				0x00, 0x00,	// information object address
				0x01,		// COI = COI_LOCAL_MANUAL_RESET
				0x56, 		// crc
				FRAME_STOP_CHARCTER
		};
		putData(iec101, req, sizeof(req));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
		ASSERT_TRUE(iec101->isReset()) << msg;
		ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
	}

	{
		printTestName(" ������� ������������� �����. \n");
		uint8_t req[] =  {
				FRAME_START_CHARACTER_VAR, 0x0F, 0x0F, FRAME_START_CHARACTER_VAR,
				(uint8_t) (0x43 + getFcb(true)), // control = USER_DATA_CONFIRM, prm = 1
				0x01,		// link address
				0x67,		// TYPE_ID_C_CS_NA_1
				0x01,		// variableStructureQualifier
				0x06,		// COT = COT_ACT
				0x01,		// common address
				0x00, 0x00,	// information object address
				0x39, 0x30,	// ms
				0x06,		// min
				0x07,		// hours
				0x08,		// days
				0x09,		// months
				0x0A,		// years
				(uint8_t) (0x44 + getFcb(false)),	// crc
				FRAME_STOP_CHARCTER
		};
		uint8_t resp[] = {FRAME_START_CHARACTER_FIX, 0x00, 0x01, 0x01, FRAME_STOP_CHARCTER}; // CONFIRM_ACK
		putData(iec101, req, sizeof(req));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
		ASSERT_TRUE(iec101->isReset()) << msg;
		ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;

		// �������� �� ��, ��� � ����� ��������� ���������� �����
		uint8_t time_exp[7] = {0x39, 0x30, 0x06, 0x07,0x08, 0x09, 0x0A};
		ASSERT_TRUE(checkMass((uint8_t *) &iec101->stTime, sizeof(iec101->stTime), time_exp , sizeof(time_exp))) << msg;
	}

	//	{
	//		printTestName(" �������� ������� ��������� ��������� �������. \n");
	//		uint8_t time_now[9] = {0x00, 0x00, 0x01, 0x02, 0x03, 0x00, 0x04, 0x05, 0x06};
	//		uint8_t time_exp[6] = {0x0A, 0x09, 0x08, 0x07, 0x06, 0x0C};
	//		uint8_t tmp_exp[6] = {0};
	//		ASSERT_TRUE(iec101->getTime(tmp_exp, time_now)) << msg;
	//		ASSERT_TRUE(checkMass(tmp_exp, sizeof(tmp_exp), time_exp , sizeof(time_exp))) << msg;
	//	}

	{
		printTestName(" ������� ��������� ������������� �����. \n");

		uint8_t req[] = FIX_FRAME_IN(10, true);	// REQUEST_USER_DATA_CLASS_1 + fcb(1) + fcv(1)
		uint8_t resp[] = {
				FRAME_START_CHARACTER_VAR, 0x0F, 0x0F, FRAME_START_CHARACTER_VAR,
				0x08, 		// control = RESPOND_USER_DATA, prm = 0, acd = 0, dfc = 0
				0x01,		// link address
				0x67,		// TYPE_ID_C_CS_NA_1
				0x01,		// variableStructureQualifier
				0x07,		// COT = COT_ACTCON
				0x01,		// common address
				0x00, 0x00,	// information object address
				0xE8, 0x03,	// ms
				0x02,		// min
				0x03,		// hours
				0x04,		// days
				0x05,		// months
				0x06,		// years
				0x78, 		// crc
				FRAME_STOP_CHARCTER
		};
		putData(iec101, req, sizeof(req));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
		ASSERT_TRUE(iec101->isReset()) << msg;
		ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
	}
}

TEST_F(TIec101_Full, readData_Iterrog) {

	iec101->setEnable();
	iec101->setAddressLan(1);
	resetRemoteLink();
	ASSERT_TRUE(iec101->isReset()) << " �������� �� �������. ";

	{
		printTestName(" ���� ������. \n");
		uint8_t req[] =  {
				FRAME_START_CHARACTER_VAR, 0x09, 0x09, FRAME_START_CHARACTER_VAR,
				(uint8_t) (0x43 + getFcb(true)), // control = USER_DATA_CONFIRM, fcb = 0, fcv = 1, prm = 1
				0x01,		// link address
				0x64,		// TYPE_ID_C_IC_NA_1
				0x01,		// variableStructureQualifier
				0x06,		// COT = COT_ACT
				0x01,		// common address
				0x00, 0x00,	// information object address
				0x14,		// qoi
				(uint8_t) (0xC4 + getFcb(false)), 	// crc
				FRAME_STOP_CHARCTER
		};
		uint8_t resp[] = {FRAME_START_CHARACTER_FIX, 0x00, 0x01, 0x01, FRAME_STOP_CHARCTER}; // CONFIRM_ACK
		putData(iec101, req, sizeof(req));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
		ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
	}

	{
		printTestName(" ���� � ������� ������������� ������. \n");
		uint8_t req[] = FIX_FRAME_IN(11, true);
		uint8_t resp[] = {
				FRAME_START_CHARACTER_VAR, 0x09, 0x09, FRAME_START_CHARACTER_VAR,
				0x28,		// RESPOND_USER_DATA + acd = 1
				0x01,		// link address
				0x64,		// TYPE_ID_C_IC_NA_1
				0x01,		// variableStructureQualifier
				0x07,		// COT = COT_ACTCON
				0x01,		// common address
				0x00, 0x00,	// information object address
				0x00,		// qoi
				0x96, 		// crc
				FRAME_STOP_CHARCTER
		};
		resp[sizeof(resp) - 2] = getCrcVarLenght(resp);	// ������� CRC
		putData(iec101, req, sizeof(req));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
		ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
	}

	{
		//		�����.
		//		- ������ 201, 202 = false;
		//		- ������ 203, 204 = true;
		//		- ������ 301- 316 = false;
		//		- ������ 317- 332 = true;

		printTestName(" ������ ������. \n");
		uint8_t resp[] = {
				FRAME_START_CHARACTER_VAR, 0x09, 0x09, FRAME_START_CHARACTER_VAR,
				0x28,		// RESPOND_USER_DATA + acd = 1
				0x01,		// link address
				0x01,		// TYPE_ID_M_SP_NA_1
				0x01,		// variableStructureQualifie
				0x14,		// COT = COT_INROGEN
				0x01,		// common address
				0xC9, 0x00,	// information object address
				0x00,		// val = false
				0x00,		// crc
				FRAME_STOP_CHARCTER
		};

		uint16_t i = 200;
		while(i < 332) {
			i++;
			printTestName(" ������ ������. \n" );
			posMsg += sprintf(&msg[posMsg], " ����� %d. \n", i);
			if (i <= 204) {
				uint8_t req[] = FIX_FRAME_IN(11, true);
				resp[10] = (uint8_t) i;
				resp[11] = (uint8_t) (i >> 8);
				if (i <= 202) {
					resp[12] = 0x00;
				} else if (i <= 204) {
					resp[12] = 0x01;
				} else {
					break;
				}
				resp[sizeof(resp) - 2] = getCrcVarLenght(resp);	// ������� CRC
				putData(iec101, req, sizeof(req));
				ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
				iec101->readData();
				ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
				ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
			} else {
				break;
			}
		}
	}

	{
		printTestName(" ���� ���������� ������. \n");
		uint8_t req[] = FIX_FRAME_IN(11, true);
		uint8_t resp[] = {
				FRAME_START_CHARACTER_VAR, 0x09, 0x09, FRAME_START_CHARACTER_VAR,
				0x08,		// RESPOND_USER_DATA + acd = 0
				0x01,		// link address
				0x64,		// TYPE_ID_C_IC_NA_1
				0x01,		// variableStructureQualifier
				0x0A,		// COT = COT_ACTTERM
				0x01,		// common address
				0x00, 0x00,	// information object address
				0x00,		// qoi
				0x96, 		// crc
				FRAME_STOP_CHARCTER
		};
		resp[sizeof(resp) - 2] = getCrcVarLenght(resp);	// ������� CRC
		putData(iec101, req, sizeof(req));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
		ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
	}
}

TEST_F(TIec101_Full, readData_Events_Class2) {
	iec101->setEnable();
	iec101->setAddressLan(1);
	resetRemoteLink();
	ASSERT_TRUE(iec101->isReset()) << " �������� �� �������. ";

	{
		printTestName(" ���������� ������ ������ 2 �� ��������. \n");
		uint8_t req[] = FIX_FRAME_IN(11, true);
		uint8_t resp[] = {FRAME_START_CHARACTER_FIX, 0x09, 0x01, 0x0A, FRAME_STOP_CHARCTER};

		putData(iec101, req, sizeof(req));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
		ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
	}



	{
		// ������������� ���� ������� �������
		iec101->sendClass2();

		uint16_t adr = 254;
		uint16_t ms = 98;
		uint16_t sec = 16;
		uint8_t year = 12;
		uint8_t month = 5;
		uint8_t day = 14;
		uint8_t hour = 8;
		uint8_t min = 23;
		for(uint8_t i = 0; i < 4; i++) {
			printTestName(" �������� ������������ ������ ������ 2. \n");
			posMsg += sprintf(&msg[posMsg], " ��� %d, ����� %d. \n", i, adr);
			uint8_t req[] = FIX_FRAME_IN(11, true);
			uint8_t resp[] = {
					FRAME_START_CHARACTER_VAR, 0x10, 0x10, FRAME_START_CHARACTER_VAR,
					0x28,		// RESPOND_USER_DATA + acd = 1
					0x01,		// link address
					0x1E,		// TYPE_ID_M_SP_TB_1
					0x01,		// variableStructureQualifier
					0x03,		// COT = COT_SPONT
					0x01,		// common address
					0x00, 0x00,	// information object address
					0x00,		// siq
					0x39, 0x30,	// ms
					0x06,		// min
					0x07,		// hours
					0x08,		// days
					0x09,		// months
					0x0A,		// years
					0x00, 		// crc
					FRAME_STOP_CHARCTER
			};
			resp[4] = 0x08; //(i == 3) ? 0x08 : 0x28;
			resp[12] = (adr == 254) ? 0x00 : 0x01;
			*((uint16_t *) &resp[10]) = adr++;
			*((uint16_t *) &resp[13]) = (sec++ * 1000) + ms++;
			resp[15] = min++;
			resp[16] = hour++;
			resp[17] = day++;
			resp[18] = month++;
			resp[19] = year++;
			resp[sizeof(resp) - 2] = getCrcVarLenght(resp);	// ������� CRC
			putData(iec101, req, sizeof(req));
			ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
			iec101->readData();
			ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
			ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
		}
	}

	{
		printTestName(" ��������� ������ ������ 2 �� ��������. \n");
		uint8_t req[] = FIX_FRAME_IN(11, true);
		uint8_t resp[] = {FRAME_START_CHARACTER_FIX, 0x09, 0x01, 0x0A, FRAME_STOP_CHARCTER};

		putData(iec101, req, sizeof(req));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
		ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
	}


	{
		// ������������� ���� ������� �������
		iec101->sendClass2();
		{
			uint16_t adr = 254;
			uint16_t ms = 98;
			uint16_t sec = 16;
			uint8_t year = 12;
			uint8_t month = 5;
			uint8_t day = 14;
			uint8_t hour = 8;
			uint8_t min = 23;
			for(uint8_t j = 0; j < 8; j++) {
				printTestName(" �������� ������� �������� ������ ������ 2. \n");
				posMsg += sprintf(&msg[posMsg], " ��� %d, ����� %d. \n", j, adr);
				uint8_t req[] = FIX_FRAME_IN(11, ((j % 2) == 0));
				uint8_t resp[] = {
						FRAME_START_CHARACTER_VAR, 0x10, 0x10, FRAME_START_CHARACTER_VAR,
						0x28,		// RESPOND_USER_DATA + acd = 1
						0x01,		// link address
						0x1E,		// TYPE_ID_M_SP_TB_1
						0x01,		// variableStructureQualifier
						0x03,		// COT = COT_SPONT
						0x01,		// common address
						0x00, 0x00,	// information object address
						0x00,		// siq
						0x39, 0x30,	// ms
						0x06,		// min
						0x07,		// hours
						0x08,		// days
						0x09,		// months
						0x0A,		// years
						0x00, 		// crc
						FRAME_STOP_CHARCTER
				};
				resp[4] = 0x08; //((j / 2) == 3) ? 0x08 : 0x28;
				resp[12] = (adr == 254) ? 0x00 : 0x01;
				*((uint16_t *) &resp[10]) = adr;
				*((uint16_t *) &resp[13]) = (sec * 1000) + ms;
				resp[15] = min;
				resp[16] = hour;
				resp[17] = day;
				resp[18] = month;
				resp[19] = year;
				if ((j % 2) == 1) {
					adr++;
					ms++;
					sec++;
					min++;
					hour++;
					day++;
					month++;
					year++;
				}
				resp[sizeof(resp) - 2] = getCrcVarLenght(resp);	// ������� CRC
				putData(iec101, req, sizeof(req));
				ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
				iec101->readData();
				ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
				ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
			}
		}
	}
}

TEST_F(TIec101_Full, readData_Events_Class1) {
	iec101->setEnable();
	iec101->setAddressLan(1);
	resetRemoteLink();
	ASSERT_TRUE(iec101->isReset()) << " �������� �� �������. ";

	{
		printTestName(" ��������� ������ ������ 1 �� �������� (RESPOND_NACK + ack=0 � ������� ������ 2). \n");
		uint8_t req[] = FIX_FRAME_IN(10, true);
		uint8_t resp[] = {FRAME_START_CHARACTER_FIX, 0x09, 0x01, 0x0A, FRAME_STOP_CHARCTER};

		putData(iec101, req, sizeof(req));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
		ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
	}

	{
		printTestName(" ��������� ������ ������ 1 �� �������� (RESPOND_NACK + ack=0 � ������� ������ 1). \n");
		uint8_t req[] = FIX_FRAME_IN(11, true);
		uint8_t resp[] = {FRAME_START_CHARACTER_FIX, 0x09, 0x01, 0x0A, FRAME_STOP_CHARCTER};

		putData(iec101, req, sizeof(req));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
		ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
	}

	{
		printTestName(" ������� ������ ������ 1 �� ��������. \n");

		iec101->sendClass1();

		uint8_t req[] = FIX_FRAME_IN(11, true);
		uint8_t resp[] = {FRAME_START_CHARACTER_FIX, 0x29, 0x01, 0x2A, FRAME_STOP_CHARCTER};

		putData(iec101, req, sizeof(req));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
		ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
	}

	{
		uint16_t adr = 250;
		uint16_t ms = 98;
		uint16_t sec = 16;
		uint8_t year = 12;
		uint8_t month = 7;
		uint8_t day = 10;
		uint8_t hour = 8;
		uint8_t min = 23;
		for(uint8_t j = 0; j < 4; j++) {
			printTestName(" �������� ������� �������� ������ ������ 1. \n");
			posMsg += sprintf(&msg[posMsg], " ��� %d, ����� %d. \n", j, adr);
			uint8_t req[] = FIX_FRAME_IN(10, ((j % 2) == 0));
			uint8_t resp[] = {
					FRAME_START_CHARACTER_VAR, 0x10, 0x10, FRAME_START_CHARACTER_VAR,
					0x28,		// RESPOND_USER_DATA + acd = 1
					0x01,		// link address
					0x1E,		// TYPE_ID_M_SP_TB_1
					0x01,		// variableStructureQualifier
					0x03,		// COT = COT_SPONT
					0x01,		// common address
					0x00, 0x00,	// information object address
					0x00,		// siq
					0x39, 0x30,	// ms
					0x06,		// min
					0x07,		// hours
					0x08,		// days
					0x09,		// months
					0x0A,		// years
					0x00, 		// crc
					FRAME_STOP_CHARCTER
			};
			resp[4] =  (j < 2) ? 0x28 : 0x08;
			resp[12] = (adr == 250) ? 0x01 : 0x00;
			*((uint16_t *) &resp[10]) = adr;
			*((uint16_t *) &resp[13]) = (sec * 1000) + ms;
			resp[15] = min;
			resp[16] = hour;
			resp[17] = day;
			resp[18] = month;
			resp[19] = year;
			if ((j % 2) == 1) {
				adr++;
				ms++;
				sec++;
				min++;
				hour++;
				day++;
				month++;
				year++;
			}
			resp[sizeof(resp) - 2] = getCrcVarLenght(resp);	// ������� CRC
			putData(iec101, req, sizeof(req));
			ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
			iec101->readData();
			ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
			ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
		}
	}

	{
		printTestName(" ��������� ������ ������ 1 �� ��������. \n");
		uint8_t req[] = FIX_FRAME_IN(10, true);
		uint8_t resp[] = {FRAME_START_CHARACTER_FIX, 0x09, 0x01, 0x0A, FRAME_STOP_CHARCTER};

		putData(iec101, req, sizeof(req));
		ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
		iec101->readData();
		ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
		ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
	}

	{
		printTestName(" �������� �������� ������ ������ 1 ��� ������� ������ ������ 2. \n");

		iec101->sendClass2();

		uint16_t adr2 = 254;
		uint16_t ms2 = 98;
		uint16_t sec2 = 16;
		uint8_t year2 = 12;
		uint8_t month2 = 5;
		uint8_t day2 = 14;
		uint8_t hour2 = 8;
		uint8_t min2 = 23;


		{
			// ������ ������� ������ 2

			for(uint8_t j = 0; j < 2; j++) {
				posMsg += sprintf(&msg[posMsg], " ��� %d, ����� %d.\n", 0, adr2);
				uint8_t req[] = FIX_FRAME_IN(11, true);
				uint8_t resp[] = {
						FRAME_START_CHARACTER_VAR, 0x10, 0x10, FRAME_START_CHARACTER_VAR,
						0x08,		// RESPOND_USER_DATA + acd = 0
						0x01,		// link address
						0x1E,		// TYPE_ID_M_SP_TB_1
						0x01,		// variableStructureQualifier
						0x03,		// COT = COT_SPONT
						0x01,		// common address
						0x00, 0x00,	// information object address
						0x00,		// siq
						0x39, 0x30,	// ms
						0x06,		// min
						0x07,		// hours
						0x08,		// days
						0x09,		// months
						0x0A,		// years
						0x00, 		// crc
						FRAME_STOP_CHARCTER
				};
				resp[4] =  (j == 0) ? 0x08 : 0x28;
				resp[12] = (adr2 == 254) ? 0x00 : 0x01;
				*((uint16_t *) &resp[10]) = adr2;
				*((uint16_t *) &resp[13]) = (sec2 * 1000) + ms2;
				resp[15] = min2;
				resp[16] = hour2;
				resp[17] = day2;
				resp[18] = month2;
				resp[19] = year2;

				adr2++;
				ms2++;
				sec2++;
				min2++;
				hour2++;
				day2++;
				month2++;
				year2++;

				resp[sizeof(resp) - 2] = getCrcVarLenght(resp);	// ������� CRC
				putData(iec101, req, sizeof(req));
				ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
				iec101->readData();
				ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
				ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;

				if (j == 0) {
					iec101->sendClass1();
				}
			}
		}


		{
			// ��������� ������ ������ 1

			posMsg += sprintf(&msg[posMsg], " ��� %d, ����� %d. \n", 1, adr2);

			uint8_t req[] = FIX_FRAME_IN(11, true);
			uint8_t resp[] = {FRAME_START_CHARACTER_FIX, 0x29, 0x01, 0x2A, FRAME_STOP_CHARCTER};

			putData(iec101, req, sizeof(req));
			ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
			iec101->readData();
			ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
			ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
		}

		{
			// ���������� ������ ������ 1

			uint16_t adr = 250;
			uint16_t ms = 98;
			uint16_t sec = 16;
			uint8_t year = 12;
			uint8_t month = 7;
			uint8_t day = 10;
			uint8_t hour = 8;
			uint8_t min = 23;
			for(uint8_t j = 0; j < 2; j++) {
				posMsg += sprintf(&msg[posMsg], " ��� %d, ����� %d. \n", 2, adr);

				uint8_t req[] = FIX_FRAME_IN(10, true);
				uint8_t resp[] = {
						FRAME_START_CHARACTER_VAR, 0x10, 0x10, FRAME_START_CHARACTER_VAR,
						0x28,		// RESPOND_USER_DATA + acd = 1
						0x01,		// link address
						0x1E,		// TYPE_ID_M_SP_TB_1
						0x01,		// variableStructureQualifier
						0x03,		// COT = COT_SPONT
						0x01,		// common address
						0x00, 0x00,	// information object address
						0x00,		// siq
						0x39, 0x30,	// ms
						0x06,		// min
						0x07,		// hours
						0x08,		// days
						0x09,		// months
						0x0A,		// years
						0x00, 		// crc
						FRAME_STOP_CHARCTER
				};
				resp[4] =  (j == 0) ? 0x28 : 0x08;
				resp[12] = (adr == 250) ? 0x01 : 0x00;
				*((uint16_t *) &resp[10]) = adr;
				*((uint16_t *) &resp[13]) = (sec * 1000) + ms;
				resp[15] = min;
				resp[16] = hour;
				resp[17] = day;
				resp[18] = month;
				resp[19] = year;

				adr++;
				ms++;
				sec++;
				min++;
				hour++;
				day++;
				month++;
				year++;

				resp[sizeof(resp) - 2] = getCrcVarLenght(resp);	// ������� CRC
				putData(iec101, req, sizeof(req));
				ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
				iec101->readData();
				ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
				ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
			}
		}

		{
			// ����������� ���������� ������ ������ 2
			for(uint8_t j = 0; j < 2; j++) {
				posMsg += sprintf(&msg[posMsg], " ��� %d, ����� %d.\n", 3, adr2);

				uint8_t req[] = FIX_FRAME_IN(11, true);
				uint8_t resp[] = {
						FRAME_START_CHARACTER_VAR, 0x10, 0x10, FRAME_START_CHARACTER_VAR,
						0x08,		// RESPOND_USER_DATA + acd = 0
						0x01,		// link address
						0x1E,		// TYPE_ID_M_SP_TB_1
						0x01,		// variableStructureQualifier
						0x03,		// COT = COT_SPONT
						0x01,		// common address
						0x00, 0x00,	// information object address
						0x00,		// siq
						0x39, 0x30,	// ms
						0x06,		// min
						0x07,		// hours
						0x08,		// days
						0x09,		// months
						0x0A,		// years
						0x00, 		// crc
						FRAME_STOP_CHARCTER
				};
				resp[4] =  0x08;
				resp[12] = (adr2 == 254) ? 0x00 : 0x01;
				*((uint16_t *) &resp[10]) = adr2;
				*((uint16_t *) &resp[13]) = (sec2 * 1000) + ms2;
				resp[15] = min2;
				resp[16] = hour2;
				resp[17] = day2;
				resp[18] = month2;
				resp[19] = year2;

				adr2++;
				ms2++;
				sec2++;
				min2++;
				hour2++;
				day2++;
				month2++;
				year2++;

				resp[sizeof(resp) - 2] = getCrcVarLenght(resp);	// ������� CRC
				putData(iec101, req, sizeof(req));
				ASSERT_EQ(iec101->checkFrame(), CIec101::ERROR_NO) << msg << " ���-�� ����: " << (uint16_t) iec101->getNumOfBytes();
				iec101->readData();
				ASSERT_TRUE(iec101->checkState(CIec101::STATE_WRITE_READY)) << msg << " " << iec101->getState();
				ASSERT_TRUE(checkMass(buf, iec101->getNumOfBytes(), resp, sizeof(resp))) << msg;
			}
		}
	}
}
