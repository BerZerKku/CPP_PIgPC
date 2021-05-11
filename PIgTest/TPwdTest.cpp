#include "gtest/gtest.h"
#include "PIg/src/security/pwd.h"
#include <cstdio>
#include <iostream>
using namespace std;

// ������ �������
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class TPwd_Test: public ::testing::Test {

protected:
	TPwd *pwd;

	virtual void SetUp() {
		pwd = new TPwd();

    }

	virtual void TearDown() {
		delete pwd;
    }

	const uint8_t pwdReset[PWD_LEN] =  {0};
};

TEST_F(TPwd_Test, constant) {
	ASSERT_EQ(600, kTickToDecCounter);
	ASSERT_EQ(1, kTickWait);
	ASSERT_EQ(5, kTickToRestPwd);
	ASSERT_EQ(8, PWD_LEN);
	ASSERT_EQ(0, memcmp(PWD_DEFAULT, "00000000", PWD_LEN));
	ASSERT_EQ(0, memcmp(PWD_RESET, "00000001", PWD_LEN));
}

TEST_F(TPwd_Test, getSet) {
	uint8_t pwdEng[] = "12345678";
	uint8_t pwdAdm[] = "55432198";
	uint8_t pwdError1[] = "a5432198";
	uint8_t pwdError2[] = "1543219A";

	// ��������� ����� ������
	ASSERT_EQ(NULL, pwd->getPwd(USER_operator));
    ASSERT_EQ(int{0}, memcmp(pwdReset, pwd->getPwd(USER_engineer), PWD_LEN));
    ASSERT_EQ(int{0}, memcmp(pwdReset, pwd->getPwd(USER_admin), PWD_LEN));
	ASSERT_EQ(NULL, pwd->getPwd(USER_MAX));
	ASSERT_EQ(NULL, pwd->getPwd(USER_factory));

	// ��������� ������
	ASSERT_FALSE(pwd->setPwd(USER_operator, pwdEng));
	ASSERT_TRUE(pwd->setPwd(USER_engineer, pwdEng));
	ASSERT_TRUE(pwd->setPwd(USER_admin, pwdAdm));
	ASSERT_FALSE(pwd->setPwd(USER_MAX, pwdAdm));
	ASSERT_FALSE(pwd->setPwd(USER_factory, pwdEng));

	// �������� ������
	ASSERT_EQ(NULL, pwd->getPwd(USER_operator));
    ASSERT_EQ(int{0}, memcmp(pwdEng, pwd->getPwd(USER_engineer), PWD_LEN));
    ASSERT_EQ(int{0}, memcmp(pwdAdm, pwd->getPwd(USER_admin), PWD_LEN));
	ASSERT_EQ(NULL, pwd->getPwd(USER_MAX));
	ASSERT_EQ(NULL, pwd->getPwd(USER_factory));

	// ��������� ������ ����������� ������������ ������
	ASSERT_FALSE(pwd->setPwd(USER_engineer, pwdError1));
    ASSERT_EQ(int{0}, memcmp(pwdEng, pwd->getPwd(USER_engineer), PWD_LEN));
	ASSERT_FALSE(pwd->setPwd(USER_admin, pwdError2));
    ASSERT_EQ(int{0}, memcmp(pwdAdm, pwd->getPwd(USER_admin), PWD_LEN));
}

TEST_F(TPwd_Test, getSetCounter) {
    uint8_t counter = 0;
    // ��������� ��� ���������
    ASSERT_EQ(0, pwd->getCounter(USER_operator));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_engineer));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_admin));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_MAX));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_factory));

    // ��������� �������� ��� ������������
    counter = 1;
    pwd->setCounter(USER_operator, counter);
    ASSERT_EQ(int{0}, pwd->getCounter(USER_operator));
    pwd->setCounter(USER_engineer, counter);
    ASSERT_EQ(counter, pwd->getCounter(USER_engineer));
    pwd->setCounter(USER_admin, counter);
    ASSERT_EQ(counter, pwd->getCounter(USER_admin));
    pwd->setCounter(USER_MAX, counter);
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_MAX));
    pwd->setCounter(USER_factory, counter);
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_factory));

    // �������� ��������� ��������� ��������
    pwd->setCounter(USER_engineer, 0);
    ASSERT_EQ(counter, pwd->getCounter(USER_engineer));

    // �������� ������ ���������
    pwd->reset();
    ASSERT_EQ(int{0}, pwd->getCounter(USER_operator));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_engineer));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_admin));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_MAX));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_factory));

    // ��������� ������������ ��������
    pwd->reset();
    counter = 0;
    pwd->setCounter(USER_engineer, counter);
    ASSERT_EQ(counter, pwd->getCounter(USER_engineer));

    // ��������� ������������� ��������
    pwd->reset();
    counter = 2*PWD_CNT_BLOCK;
    pwd->setCounter(USER_engineer, counter);
    ASSERT_EQ(counter, pwd->getCounter(USER_engineer));

    // ��������� �������� ������������ ������������
    pwd->reset();
    counter = 2*PWD_CNT_BLOCK;
    pwd->setCounter(USER_engineer, counter + 1);
    ASSERT_EQ(counter, pwd->getCounter(USER_engineer));
}

TEST_F(TPwd_Test, isLocked) {
    uint8_t counter = 0;

    // ��������� ��� ���������
    ASSERT_FALSE(pwd->isLocked(USER_operator));
    ASSERT_TRUE(pwd->isLocked(USER_engineer));
    ASSERT_TRUE(pwd->isLocked(USER_admin));
    ASSERT_TRUE(pwd->isLocked(USER_MAX));
    ASSERT_FALSE(pwd->isLocked(USER_factory));

    // �������� ��� ������������ �������� ��������
    counter = 0;
    pwd->setCounter(USER_operator, counter);
    ASSERT_FALSE(pwd->isLocked(USER_operator));
    pwd->setCounter(USER_engineer, counter);
    ASSERT_FALSE(pwd->isLocked(USER_engineer));
    pwd->setCounter(USER_admin, counter);
    ASSERT_FALSE(pwd->isLocked(USER_admin));
    pwd->setCounter(USER_MAX, counter);
    ASSERT_TRUE(pwd->isLocked(USER_MAX));
    pwd->setCounter(USER_factory, counter);
    ASSERT_FALSE(pwd->isLocked(USER_factory));

    // �������� ����� ������
    pwd->reset();
    ASSERT_FALSE(pwd->isLocked(USER_operator));
    ASSERT_TRUE(pwd->isLocked(USER_engineer));
    ASSERT_TRUE(pwd->isLocked(USER_admin));
    ASSERT_TRUE(pwd->isLocked(USER_MAX));
    ASSERT_FALSE(pwd->isLocked(USER_factory));

    // �������� �������� �� 1 ������ ����������
    pwd->reset();
    counter = PWD_CNT_BLOCK - 1;
    pwd->setCounter(USER_operator, counter);
    ASSERT_FALSE(pwd->isLocked(USER_operator));
    pwd->setCounter(USER_engineer, counter);
    ASSERT_FALSE(pwd->isLocked(USER_engineer));
    pwd->setCounter(USER_admin, counter);
    ASSERT_FALSE(pwd->isLocked(USER_admin));
    pwd->setCounter(USER_MAX, counter);
    ASSERT_TRUE(pwd->isLocked(USER_MAX));
    pwd->setCounter(USER_factory, counter);
    ASSERT_FALSE(pwd->isLocked(USER_factory));

    // �������� ��� �������� ������� ����������
    pwd->reset();
    counter = PWD_CNT_BLOCK;
    pwd->setCounter(USER_operator, counter);
    ASSERT_FALSE(pwd->isLocked(USER_operator));
    pwd->setCounter(USER_engineer, counter);
    ASSERT_TRUE(pwd->isLocked(USER_engineer));
    pwd->setCounter(USER_admin, counter);
    ASSERT_TRUE(pwd->isLocked(USER_admin));
    pwd->setCounter(USER_MAX, counter);
    ASSERT_TRUE(pwd->isLocked(USER_MAX));
    pwd->setCounter(USER_factory, counter);
    ASSERT_FALSE(pwd->isLocked(USER_factory));

    // �������� ��� ������������� �������� ��������
    pwd->reset();
    counter = 2*PWD_CNT_BLOCK;
    pwd->setCounter(USER_operator, counter);
    ASSERT_FALSE(pwd->isLocked(USER_operator));
    pwd->setCounter(USER_engineer, counter);
    ASSERT_TRUE(pwd->isLocked(USER_engineer));
    pwd->setCounter(USER_admin, counter);
    ASSERT_TRUE(pwd->isLocked(USER_admin));
    pwd->setCounter(USER_MAX, counter);
    ASSERT_TRUE(pwd->isLocked(USER_MAX));
    pwd->setCounter(USER_factory, counter);
    ASSERT_FALSE(pwd->isLocked(USER_factory));

}

TEST_F(TPwd_Test, checkPassword) {
    uint8_t counter = 0;
	uint8_t pwdEng[] = "12345678";
	uint8_t pwdAdm[] = "55432198";
	uint8_t pwdError1[] = "a5432198";

	// ��� ��������� �� ������ �� ���������
    ASSERT_FALSE(pwd->checkPassword(USER_engineer, (uint8_t *) PWD_DEFAULT));
    ASSERT_FALSE(pwd->checkPassword(USER_admin, (uint8_t *) PWD_DEFAULT));

	// � ��������� ������ true
	ASSERT_TRUE(pwd->checkPassword(USER_operator, NULL));
	ASSERT_TRUE(pwd->checkPassword(USER_operator, pwdEng));
	ASSERT_TRUE(pwd->checkPassword(USER_operator, pwdError1));

    // �������� ������ ��� USER_MAX
    ASSERT_FALSE(pwd->checkPassword(USER_MAX, NULL));
    ASSERT_FALSE(pwd->checkPassword(USER_MAX, (uint8_t *) PWD_DEFAULT));
    ASSERT_FALSE(pwd->checkPassword(USER_MAX, (uint8_t *) PWD_RESET));

    // �������� ������ ��� ������ �� ��������� ���������
    ASSERT_FALSE(pwd->checkPassword(USER_factory, NULL));
    ASSERT_FALSE(pwd->checkPassword(USER_factory, (uint8_t *) PWD_DEFAULT));
    ASSERT_TRUE(pwd->checkPassword(USER_factory, (uint8_t *) PWD_RESET));

    // ��������� ������ ��� ��������������
    pwd->setCounter(USER_admin, 0);
    ASSERT_FALSE(pwd->isLocked(USER_admin));
    ASSERT_TRUE(pwd->setPwd(USER_admin, pwdAdm));

    ASSERT_TRUE(pwd->setPwd(USER_engineer, pwdEng));

    // �������� ������ ��� ��������� �������� ������
    ASSERT_TRUE(pwd->isLocked(USER_engineer));
    ASSERT_FALSE(pwd->checkPassword(USER_engineer, pwdEng));

    // �������� ������ ��� �������������� �������� ������
    pwd->setCounter(USER_engineer, counter);
    ASSERT_FALSE(pwd->isLocked(USER_engineer));
    ASSERT_TRUE(pwd->checkPassword(USER_engineer, pwdEng));

    // �������� ��������� ���������� �������� ������ ��� NULL ������
    ASSERT_FALSE(pwd->checkPassword(USER_engineer, NULL));
    ASSERT_EQ(counter, pwd->getCounter(USER_engineer));

    // �������� ���������� �������� ������ ��� ��������� ������
    ASSERT_FALSE(pwd->checkPassword(USER_engineer, pwdReset));
    ASSERT_EQ(++counter, pwd->getCounter(USER_engineer));
    ASSERT_FALSE(pwd->checkPassword(USER_engineer, (uint8_t *) PWD_DEFAULT));
    ASSERT_EQ(++counter, pwd->getCounter(USER_engineer));

    // �������� ������ �������� ������ ��� ����� ������� ������
    counter = 0;
    ASSERT_TRUE(pwd->checkPassword(USER_engineer, pwdEng));
    ASSERT_EQ(counter, pwd->getCounter(USER_engineer));

    // �������� �������� ����� ����������
    ASSERT_FALSE(pwd->checkPassword(USER_engineer, pwdAdm));
    ASSERT_EQ(++counter, pwd->getCounter(USER_engineer));
    ASSERT_FALSE(pwd->checkPassword(USER_engineer, pwdAdm));
    ASSERT_EQ(++counter, pwd->getCounter(USER_engineer));
    ASSERT_FALSE(pwd->checkPassword(USER_engineer, pwdAdm));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_engineer));

    // ���������� ������ �������� ������ ��� ����������
    ASSERT_FALSE(pwd->checkPassword(USER_engineer, pwdEng));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_engineer));

    // �������� �� ���������������� ��������������
    ASSERT_FALSE(pwd->isLocked(USER_admin));
    ASSERT_TRUE(pwd->checkPassword(USER_admin, pwdAdm));
}
