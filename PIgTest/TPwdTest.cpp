#include "gtest/gtest.h"
#include "PIg/src/security/pwd.h"
#include <cstdio>
#include <iostream>
using namespace std;

// размер массива
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

	// состояние после сброса
	ASSERT_EQ(NULL, pwd->getPwd(USER_operator));
    ASSERT_EQ(int{0}, memcmp(pwdReset, pwd->getPwd(USER_engineer), PWD_LEN));
    ASSERT_EQ(int{0}, memcmp(pwdReset, pwd->getPwd(USER_admin), PWD_LEN));
	ASSERT_EQ(NULL, pwd->getPwd(USER_MAX));
	ASSERT_EQ(NULL, pwd->getPwd(USER_factory));

	// установка пароля
	ASSERT_FALSE(pwd->setPwd(USER_operator, pwdEng));
	ASSERT_TRUE(pwd->setPwd(USER_engineer, pwdEng));
	ASSERT_TRUE(pwd->setPwd(USER_admin, pwdAdm));
	ASSERT_FALSE(pwd->setPwd(USER_MAX, pwdAdm));
	ASSERT_FALSE(pwd->setPwd(USER_factory, pwdEng));

	// проверка пароля
	ASSERT_EQ(NULL, pwd->getPwd(USER_operator));
    ASSERT_EQ(int{0}, memcmp(pwdEng, pwd->getPwd(USER_engineer), PWD_LEN));
    ASSERT_EQ(int{0}, memcmp(pwdAdm, pwd->getPwd(USER_admin), PWD_LEN));
	ASSERT_EQ(NULL, pwd->getPwd(USER_MAX));
	ASSERT_EQ(NULL, pwd->getPwd(USER_factory));

	// установка пароля содержащего недопустимый символ
	ASSERT_FALSE(pwd->setPwd(USER_engineer, pwdError1));
    ASSERT_EQ(int{0}, memcmp(pwdEng, pwd->getPwd(USER_engineer), PWD_LEN));
	ASSERT_FALSE(pwd->setPwd(USER_admin, pwdError2));
    ASSERT_EQ(int{0}, memcmp(pwdAdm, pwd->getPwd(USER_admin), PWD_LEN));
}

TEST_F(TPwd_Test, getSetCounter) {
    uint8_t counter = 0;
    // состояние при включении
    ASSERT_EQ(0, pwd->getCounter(USER_operator));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_engineer));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_admin));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_MAX));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_factory));

    // установка счетчика для пользователя
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

    // проверка повторной установки счетчика
    pwd->setCounter(USER_engineer, 0);
    ASSERT_EQ(counter, pwd->getCounter(USER_engineer));

    // проверка сброса счетчиков
    pwd->reset();
    ASSERT_EQ(int{0}, pwd->getCounter(USER_operator));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_engineer));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_admin));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_MAX));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_factory));

    // установка минимального значения
    pwd->reset();
    counter = 0;
    pwd->setCounter(USER_engineer, counter);
    ASSERT_EQ(counter, pwd->getCounter(USER_engineer));

    // установка максимального значения
    pwd->reset();
    counter = 2*PWD_CNT_BLOCK;
    pwd->setCounter(USER_engineer, counter);
    ASSERT_EQ(counter, pwd->getCounter(USER_engineer));

    // установка значения превышающего максимальное
    pwd->reset();
    counter = 2*PWD_CNT_BLOCK;
    pwd->setCounter(USER_engineer, counter + 1);
    ASSERT_EQ(counter, pwd->getCounter(USER_engineer));
}

TEST_F(TPwd_Test, isLocked) {
    uint8_t counter = 0;

    // состояние при включении
    ASSERT_FALSE(pwd->isLocked(USER_operator));
    ASSERT_TRUE(pwd->isLocked(USER_engineer));
    ASSERT_TRUE(pwd->isLocked(USER_admin));
    ASSERT_TRUE(pwd->isLocked(USER_MAX));
    ASSERT_FALSE(pwd->isLocked(USER_factory));

    // проверка для минимального значения счетчика
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

    // проверка после сброса
    pwd->reset();
    ASSERT_FALSE(pwd->isLocked(USER_operator));
    ASSERT_TRUE(pwd->isLocked(USER_engineer));
    ASSERT_TRUE(pwd->isLocked(USER_admin));
    ASSERT_TRUE(pwd->isLocked(USER_MAX));
    ASSERT_FALSE(pwd->isLocked(USER_factory));

    // проверка счетчика на 1 меньше блокировки
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

    // проверка для счетчика равного блокировке
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

    // проверка для максимального значения счетчика
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

	// при включении не пароль по умолчанию
    ASSERT_FALSE(pwd->checkPassword(USER_engineer, (uint8_t *) PWD_DEFAULT));
    ASSERT_FALSE(pwd->checkPassword(USER_admin, (uint8_t *) PWD_DEFAULT));

	// у оператора всегда true
	ASSERT_TRUE(pwd->checkPassword(USER_operator, NULL));
	ASSERT_TRUE(pwd->checkPassword(USER_operator, pwdEng));
	ASSERT_TRUE(pwd->checkPassword(USER_operator, pwdError1));

    // проверка пароля для USER_MAX
    ASSERT_FALSE(pwd->checkPassword(USER_MAX, NULL));
    ASSERT_FALSE(pwd->checkPassword(USER_MAX, (uint8_t *) PWD_DEFAULT));
    ASSERT_FALSE(pwd->checkPassword(USER_MAX, (uint8_t *) PWD_RESET));

    // проверка пароля для сброса на заводские настройки
    ASSERT_FALSE(pwd->checkPassword(USER_factory, NULL));
    ASSERT_FALSE(pwd->checkPassword(USER_factory, (uint8_t *) PWD_DEFAULT));
    ASSERT_TRUE(pwd->checkPassword(USER_factory, (uint8_t *) PWD_RESET));

    // Установка пароля для администратора
    pwd->setCounter(USER_admin, 0);
    ASSERT_FALSE(pwd->isLocked(USER_admin));
    ASSERT_TRUE(pwd->setPwd(USER_admin, pwdAdm));

    ASSERT_TRUE(pwd->setPwd(USER_engineer, pwdEng));

    // Проверка пароля без установки счетчика ошибок
    ASSERT_TRUE(pwd->isLocked(USER_engineer));
    ASSERT_FALSE(pwd->checkPassword(USER_engineer, pwdEng));

    // Проверка пароля для установленного счетчика ошибок
    pwd->setCounter(USER_engineer, counter);
    ASSERT_FALSE(pwd->isLocked(USER_engineer));
    ASSERT_TRUE(pwd->checkPassword(USER_engineer, pwdEng));

    // Проверка отсутсвия увеличения счетчика ошибок для NULL пароля
    ASSERT_FALSE(pwd->checkPassword(USER_engineer, NULL));
    ASSERT_EQ(counter, pwd->getCounter(USER_engineer));

    // Проверка увеличения счетчика ошибок для неверного пароля
    ASSERT_FALSE(pwd->checkPassword(USER_engineer, pwdReset));
    ASSERT_EQ(++counter, pwd->getCounter(USER_engineer));
    ASSERT_FALSE(pwd->checkPassword(USER_engineer, (uint8_t *) PWD_DEFAULT));
    ASSERT_EQ(++counter, pwd->getCounter(USER_engineer));

    // Проверка сброса счетчика ошибок при вводе верного пароля
    counter = 0;
    ASSERT_TRUE(pwd->checkPassword(USER_engineer, pwdEng));
    ASSERT_EQ(counter, pwd->getCounter(USER_engineer));

    // Проверка счетчика после блокировки
    ASSERT_FALSE(pwd->checkPassword(USER_engineer, pwdAdm));
    ASSERT_EQ(++counter, pwd->getCounter(USER_engineer));
    ASSERT_FALSE(pwd->checkPassword(USER_engineer, pwdAdm));
    ASSERT_EQ(++counter, pwd->getCounter(USER_engineer));
    ASSERT_FALSE(pwd->checkPassword(USER_engineer, pwdAdm));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_engineer));

    // Отсутствие сброса счетчика ошибок при блокировке
    ASSERT_FALSE(pwd->checkPassword(USER_engineer, pwdEng));
    ASSERT_EQ(2*PWD_CNT_BLOCK, pwd->getCounter(USER_engineer));

    // Проверка не заблокированного администратора
    ASSERT_FALSE(pwd->isLocked(USER_admin));
    ASSERT_TRUE(pwd->checkPassword(USER_admin, pwdAdm));
}
