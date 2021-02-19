#include "gtest/gtest.h"
#include "PIg/src/security/user.h"
#include <cstdio>
#include <iostream>
using namespace std;

// ������ �������
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class TUser_Test: public ::testing::Test {

protected:
	TUser *user;

	virtual void SetUp() {
		user = new TUser();
    };

	virtual void TearDown() {
		delete user;
    };
};

TEST_F(TUser_Test, constant) {
	ASSERT_EQ(2, USER_SOURCE_MAX);
	ASSERT_EQ(3, USER_MAX);
	ASSERT_EQ(900, kTimeToReset);
}

TEST_F(TUser_Test, getSet) {
	user_t userpi = USER_operator;
	user_t userpc = USER_operator;

	ASSERT_EQ(userpi, user->get(USER_SOURCE_pi));
	ASSERT_EQ(userpc, user->get(USER_SOURCE_pc));
	ASSERT_EQ(USER_MAX, user->get(USER_SOURCE_MAX));

	// ��������� ������������ ��� ��
	userpi = USER_engineer;
	ASSERT_TRUE(user->set(USER_SOURCE_pi, userpi));
	ASSERT_EQ(userpi, user->get(USER_SOURCE_pi));
	ASSERT_EQ(userpc, user->get(USER_SOURCE_pc));
	ASSERT_EQ(USER_MAX, user->get(USER_SOURCE_MAX));

	// ��������� ������������ ��� ��
	userpc = USER_admin;
	ASSERT_TRUE(user->set(USER_SOURCE_pc, userpc));
	ASSERT_EQ(userpi, user->get(USER_SOURCE_pi));
	ASSERT_EQ(userpc, user->get(USER_SOURCE_pc));
	ASSERT_EQ(USER_MAX, user->get(USER_SOURCE_MAX));

	// �������� ��������� ��� ���������� ��������� �������
	ASSERT_FALSE(user->set(USER_SOURCE_MAX, userpc));
	ASSERT_EQ(userpi, user->get(USER_SOURCE_pi));
	ASSERT_EQ(userpc, user->get(USER_SOURCE_pc));
	ASSERT_EQ(USER_MAX, user->get(USER_SOURCE_MAX));

	// �������� ��������� ���������� ������������
	ASSERT_FALSE(user->set(USER_SOURCE_pc, USER_MAX));
	ASSERT_EQ(userpi, user->get(USER_SOURCE_pi));
	ASSERT_EQ(userpc, user->get(USER_SOURCE_pc));
	ASSERT_EQ(USER_MAX, user->get(USER_SOURCE_MAX));

	// �������� ��������� ���������
	userpi = USER_operator;
	ASSERT_TRUE(user->set(USER_SOURCE_pi, userpi));
	ASSERT_EQ(userpi, user->get(USER_SOURCE_pi));
	ASSERT_EQ(userpc, user->get(USER_SOURCE_pc));
	ASSERT_EQ(USER_MAX, user->get(USER_SOURCE_MAX));
}

TEST_F(TUser_Test, reset) {
	user_t userpi = USER_engineer;
	user_t userpc = USER_admin;

	user->set(USER_SOURCE_pi, userpi);
	user->set(USER_SOURCE_pc, userpc);
	ASSERT_EQ(userpi, user->get(USER_SOURCE_pi));
	ASSERT_EQ(userpc, user->get(USER_SOURCE_pc));
	ASSERT_EQ(USER_MAX, user->get(USER_SOURCE_MAX));

	user->reset(USER_SOURCE_pi);
	ASSERT_EQ(USER_operator, user->get(USER_SOURCE_pi));
	ASSERT_EQ(userpc, user->get(USER_SOURCE_pc));
	ASSERT_EQ(USER_MAX, user->get(USER_SOURCE_MAX));

	user->set(USER_SOURCE_pi, userpi);
	user->reset(USER_SOURCE_pc);
	ASSERT_EQ(userpi, user->get(USER_SOURCE_pi));
	ASSERT_EQ(USER_operator, user->get(USER_SOURCE_pc));
	ASSERT_EQ(USER_MAX, user->get(USER_SOURCE_MAX));

	user->set(USER_SOURCE_pi, userpi);
	user->set(USER_SOURCE_pc, userpc);
	user->reset(USER_SOURCE_MAX);
	ASSERT_EQ(userpi, user->get(USER_SOURCE_pi));
	ASSERT_EQ(userpc, user->get(USER_SOURCE_pc));
	ASSERT_EQ(USER_MAX, user->get(USER_SOURCE_MAX));

	user->reset();
	ASSERT_EQ(USER_operator, user->get(USER_SOURCE_pi));
	ASSERT_EQ(USER_operator, user->get(USER_SOURCE_pc));
	ASSERT_EQ(USER_MAX, user->get(USER_SOURCE_MAX));
}

TEST_F(TUser_Test, tick) {
	ASSERT_EQ(USER_operator, user->get(USER_SOURCE_pi));
	ASSERT_EQ(USER_operator, user->get(USER_SOURCE_pc));
	ASSERT_EQ(900, user->getTimer(USER_SOURCE_pi));
	ASSERT_EQ(900, user->getTimer(USER_SOURCE_pc));
	ASSERT_EQ(0, user->getTimer(USER_SOURCE_MAX));

	// �������� ������ ����� � ������ ���������
	user->tick();
	user->tick();
	user->tick();
	ASSERT_EQ(900, user->getTimer(USER_SOURCE_pi));
	ASSERT_EQ(900, user->getTimer(USER_SOURCE_pc));
	ASSERT_EQ(0, user->getTimer(USER_SOURCE_MAX));

	// �������� ������ ����� ��� ������ ������������
	user->set(USER_SOURCE_pi, USER_engineer);
	user->tick();
	user->tick();
	ASSERT_EQ(898, user->getTimer(USER_SOURCE_pi));
	ASSERT_EQ(900, user->getTimer(USER_SOURCE_pc));
	ASSERT_EQ(0, user->getTimer(USER_SOURCE_MAX));

	// �������� ������ ����� ��� ����� ���� �� ��������
	user->set(USER_SOURCE_pi, USER_operator);
	ASSERT_EQ(900, user->getTimer(USER_SOURCE_pi));

	// �������� ������ ����� ��� ������� ������������
	user->set(USER_SOURCE_pc, USER_engineer);
	user->tick();
	ASSERT_EQ(900, user->getTimer(USER_SOURCE_pi));
	ASSERT_EQ(899, user->getTimer(USER_SOURCE_pc));
	ASSERT_EQ(0, user->getTimer(USER_SOURCE_MAX));

	// �������� ������ ����� ��� ����� ���� �� �� ���������
	user->set(USER_SOURCE_pi, USER_engineer);
	user->tick();
	user->set(USER_SOURCE_pc, USER_admin);
	ASSERT_EQ(899, user->getTimer(USER_SOURCE_pi));
	ASSERT_EQ(900, user->getTimer(USER_SOURCE_pc));

	// �������� ������ �����
	user->tick();
	user->resetTimer(USER_SOURCE_pc);
	ASSERT_EQ(900, user->getTimer(USER_SOURCE_pc));

	// �������� ������ ����� ��� ������ ����
	user->set(USER_SOURCE_pi, USER_engineer);
	user->set(USER_SOURCE_pc, USER_engineer);
	user->tick();
	ASSERT_EQ(897, user->getTimer(USER_SOURCE_pi));
	ASSERT_EQ(899, user->getTimer(USER_SOURCE_pc));
	user->reset(USER_SOURCE_pc);
	ASSERT_EQ(897, user->getTimer(USER_SOURCE_pi));
	ASSERT_EQ(900, user->getTimer(USER_SOURCE_pc));

	// �������� ������ ����� ��� ������� ���� �����
	user->set(USER_SOURCE_pi, USER_engineer);
	user->set(USER_SOURCE_pc, USER_engineer);
	user->tick();
	ASSERT_EQ(896, user->getTimer(USER_SOURCE_pi));
	ASSERT_EQ(899, user->getTimer(USER_SOURCE_pc));
	user->reset();
	ASSERT_EQ(900, user->getTimer(USER_SOURCE_pi));
	ASSERT_EQ(900, user->getTimer(USER_SOURCE_pc));
}

TEST_F(TUser_Test, autoreset) {
	user_t userpi = USER_engineer;
	user_t userpc = USER_admin;
	user_t userpiold = userpi;
	user_t userpcold = userpc;
	user_t usermaxold = USER_operator;

	// ������� � 1 ���, ����� ������������ ������������ �� ������������
	user->set(USER_SOURCE_pi, userpi);
	user->tick();
	user->set(USER_SOURCE_pc, userpc);

	// �������� ������� �� ������� ������
	for(uint16_t i = kTimeToReset; i > 2; i--) {
		user->tick();

		// ������������ �� ��������
		ASSERT_EQ(userpi, user->get(USER_SOURCE_pi));
		ASSERT_EQ(userpc, user->get(USER_SOURCE_pc));
		ASSERT_EQ(USER_MAX, user->get(USER_SOURCE_MAX));

		// �������� ��������� �������
		ASSERT_EQ(i - 2, user->getTimer(USER_SOURCE_pi));
		ASSERT_EQ(i - 1, user->getTimer(USER_SOURCE_pc));
		ASSERT_EQ(0, user->getTimer(USER_SOURCE_MAX));

		// ������ ������������ �� ���
		userpiold = USER_MAX;
		ASSERT_FALSE(user->isAutoReset(USER_SOURCE_pi, userpiold));
		ASSERT_EQ(userpi, userpiold);

		// ������ ������������ �� ���
		userpcold = USER_operator;
		ASSERT_FALSE(user->isAutoReset(USER_SOURCE_pc, userpcold));
		ASSERT_EQ(userpc, userpcold);

		// ������ ������ ������������� ���
		usermaxold = USER_engineer;
		ASSERT_FALSE(user->isAutoReset(USER_SOURCE_MAX, usermaxold));
		ASSERT_EQ(USER_engineer, usermaxold);
	}

	// ����� ������������ ��
	userpiold = USER_operator;
	user->tick();
	ASSERT_EQ(USER_operator, user->get(USER_SOURCE_pi));
	ASSERT_TRUE(user->isAutoReset(USER_SOURCE_pi, userpiold));
	ASSERT_EQ(userpi, userpiold);
	ASSERT_FALSE(user->isAutoReset(USER_SOURCE_pi, userpiold));
	ASSERT_EQ(USER_operator, userpiold);
	ASSERT_EQ(900, user->getTimer(USER_SOURCE_pi));
	ASSERT_EQ(userpc, user->get(USER_SOURCE_pc));
	ASSERT_EQ(1, user->getTimer(USER_SOURCE_pc));

	// ����� ������������ ��
	userpcold = USER_operator;
	user->tick();
	ASSERT_EQ(USER_operator, user->get(USER_SOURCE_pi));
	ASSERT_FALSE(user->isAutoReset(USER_SOURCE_pi, userpiold));
	ASSERT_EQ(USER_operator, userpiold);
	ASSERT_EQ(900, user->getTimer(USER_SOURCE_pc));
	ASSERT_EQ(USER_operator, user->get(USER_SOURCE_pc));
	ASSERT_TRUE(user->isAutoReset(USER_SOURCE_pc, userpcold));
	ASSERT_EQ(userpc, userpcold);
	ASSERT_FALSE(user->isAutoReset(USER_SOURCE_pc, userpcold));
	ASSERT_EQ(USER_operator, userpcold);
	ASSERT_EQ(900, user->getTimer(USER_SOURCE_pc));
}
