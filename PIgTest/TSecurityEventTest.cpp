#include "gtest/gtest.h"
#include "security/securityevent.h"
#include <cstdio>
#include <iostream>
#include <cstring>
using namespace std;

// ������ �������
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class TSecurityTest_Test: public ::testing::Test {

protected:
	TSecurityEvent *event;

	virtual void SetUp() {
		event = new TSecurityEvent();

	};

	virtual void TearDown() {
		delete event;
	};
};

TEST_F(TSecurityTest_Test, constant) {
	ASSERT_EQ(17, TSecurityEvent::EVENT_MAX);
	ASSERT_EQ(10, IS_MSG_FIFO_SIZE);
}

TEST_F(TSecurityTest_Test, eventString) {
	TSecurityEvent::event_t ev;

	ev = TSecurityEvent::EVENT_autoEnd;
	ASSERT_EQ(0, strcmp("����. ������. ������", event->getEventString(ev)));

	ev = TSecurityEvent::EVENT_chgSettings;
	ASSERT_EQ(0, strcmp("��������� ��������", event->getEventString(ev)));

	ev = TSecurityEvent::EVENT_chgTime;
	ASSERT_EQ(0, strcmp("��������� ����/����", event->getEventString(ev)));

	ev = TSecurityEvent::EVENT_setOperator;
	ASSERT_EQ(0, strcmp("�������. �� ��������", event->getEventString(ev)));

	ev = TSecurityEvent::EVENT_setEngineer;
	ASSERT_EQ(0, strcmp("�������. �� �������", event->getEventString(ev)));

	ev = TSecurityEvent::EVENT_setAdmin;
	ASSERT_EQ(0, strcmp("�������. �� �����.", event->getEventString(ev)));

	ev = TSecurityEvent::EVENT_blkEngineer;
	ASSERT_EQ(0, strcmp("���������� ��������", event->getEventString(ev)));

	ev = TSecurityEvent::EVENT_blkAdmin;
	ASSERT_EQ(0, strcmp("���������� �����.", event->getEventString(ev)));

	ev = TSecurityEvent::EVENT_chgPwdEng;
	ASSERT_EQ(0, strcmp("���. ������ ��������", event->getEventString(ev)));

	ev = TSecurityEvent::EVENT_chgPwdAdm;
	ASSERT_EQ(0, strcmp("���. ������ �����.", event->getEventString(ev)));

	ev = TSecurityEvent::EVENT_wrgPwdEng;
	ASSERT_EQ(0, strcmp("��. ������ ��������", event->getEventString(ev)));

	ev = TSecurityEvent::EVENT_wrgPwdAdm;
	ASSERT_EQ(0, strcmp("��. ������ �����.", event->getEventString(ev)));

	ev = TSecurityEvent::EVENT_resetPwd;
	ASSERT_EQ(0, strcmp("����� �������", event->getEventString(ev)));

	ev = TSecurityEvent::EVENT_regEnable;
	ASSERT_EQ(0, strcmp("����� ������", event->getEventString(ev)));

	ev = TSecurityEvent::EVENT_regDisable;
	ASSERT_EQ(0, strcmp("����� �������", event->getEventString(ev)));

	ev = TSecurityEvent::EVENT_regTestPrd;
	ASSERT_EQ(0, strcmp("����� ���� ���", event->getEventString(ev)));

	ev = TSecurityEvent::EVENT_regTestPrm;
	ASSERT_EQ(0, strcmp("����� ���� ���", event->getEventString(ev)));

	ev = TSecurityEvent::EVENT_MAX;
	ASSERT_EQ(0, strcmp("������� - %d", event->getEventString(ev)));

	ev = static_cast<TSecurityEvent::event_t> (TSecurityEvent::EVENT_MAX + 1);
	ASSERT_EQ(0, strcmp("������� - %d", event->getEventString(ev)));
}

TEST_F(TSecurityTest_Test, getUserSourceString) {
	userSrc_t src;
	ASSERT_EQ(2, USER_SOURCE_MAX);

	src = USER_SOURCE_pi;
	ASSERT_EQ(0, strcmp("��", event->getUserSourceString(src)));

	src = USER_SOURCE_pc;
	ASSERT_EQ(0, strcmp("��", event->getUserSourceString(src)));

	src = USER_SOURCE_MAX;
	ASSERT_EQ(0, strcmp("��", event->getUserSourceString(src)));

	src = static_cast<userSrc_t> (USER_SOURCE_MAX + 1);
	ASSERT_EQ(0, strcmp("��", event->getUserSourceString(src)));
}

TEST_F(TSecurityTest_Test, pushPwdBlocked) {
	user_t user;
	userSrc_t source;
	TSecurityEvent::event_t ev;

	ASSERT_TRUE(event->isEmpty());

	// ������ 1
	ASSERT_TRUE(event->pushPwdBlocked(USER_operator, USER_SOURCE_pi, USER_admin));
	ASSERT_FALSE(event->isEmpty());

	// ������ 2
	ASSERT_TRUE(event->pushPwdBlocked(USER_engineer, USER_SOURCE_pc, USER_engineer));
	ASSERT_FALSE(event->isEmpty());

	// ������ 3
	ASSERT_TRUE(event->pushPwdBlocked(USER_engineer, USER_SOURCE_MAX, USER_engineer));
	ASSERT_FALSE(event->isEmpty());

	// ������ 4
	ASSERT_TRUE(event->pushPwdBlocked(USER_admin, USER_SOURCE_pc, USER_admin));
	ASSERT_FALSE(event->isEmpty());

	// ������ 5
	ASSERT_TRUE(event->pushPwdBlocked(USER_MAX, USER_SOURCE_pi, USER_engineer));
	ASSERT_FALSE(event->isEmpty());

	// ������: ������ ������� ��� ��������� ��� ������������� ������������
	ASSERT_FALSE(event->pushPwdBlocked(USER_admin, USER_SOURCE_pi, USER_operator));
	ASSERT_FALSE(event->isEmpty());
	ASSERT_FALSE(event->pushPwdBlocked(USER_engineer, USER_SOURCE_pc, USER_factory));
	ASSERT_FALSE(event->isEmpty());
	ASSERT_FALSE(event->pushPwdBlocked(USER_engineer, USER_SOURCE_pc, USER_MAX));
	ASSERT_FALSE(event->isEmpty());

	// ������: ������ ������� ����������� ������� �������������
	user = static_cast<user_t> (USER_MAX + 1);
	ASSERT_FALSE(event->pushPwdBlocked(user, USER_SOURCE_pc, USER_operator));
	ASSERT_FALSE(event->isEmpty());

	// ������: ������ ������� � ������������ ��������� �������
	source = static_cast<userSrc_t> (USER_SOURCE_MAX + 1);
	ASSERT_FALSE(event->pushPwdBlocked(USER_engineer, source, USER_operator));
	ASSERT_FALSE(event->isEmpty());

	// ** �������� ����������� ������ **

	// ������ 1
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_operator, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_blkAdmin, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 2
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_blkEngineer, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 3
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_MAX, source);
	ASSERT_EQ(TSecurityEvent::EVENT_blkEngineer, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 4
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_admin, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_blkAdmin, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 5
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_MAX, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_blkEngineer, ev);
	ASSERT_TRUE(event->isEmpty());

	// ���������� �� ������������ ������
	ASSERT_FALSE(event->pop(user, source, ev));
	ASSERT_EQ(USER_MAX, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_blkEngineer, ev);
	ASSERT_TRUE(event->isEmpty());
}

TEST_F(TSecurityTest_Test, pushPwdChanged) {
	user_t user;
	userSrc_t source;
	TSecurityEvent::event_t ev;

	ASSERT_TRUE(event->isEmpty());

	// ������ 1
	ASSERT_TRUE(event->pushPwdChanged(USER_operator, USER_SOURCE_pi, USER_admin));
	ASSERT_FALSE(event->isEmpty());

	// ������ 2
	ASSERT_TRUE(event->pushPwdChanged(USER_engineer, USER_SOURCE_pc, USER_engineer));
	ASSERT_FALSE(event->isEmpty());

	// ������ 3
	ASSERT_TRUE(event->pushPwdChanged(USER_engineer, USER_SOURCE_MAX, USER_engineer));
	ASSERT_FALSE(event->isEmpty());

	// ������ 4
	ASSERT_TRUE(event->pushPwdChanged(USER_admin, USER_SOURCE_pc, USER_admin));
	ASSERT_FALSE(event->isEmpty());

	// ������ 5
	ASSERT_TRUE(event->pushPwdChanged(USER_MAX, USER_SOURCE_pi, USER_engineer));
	ASSERT_FALSE(event->isEmpty());

	// ������: ������ ������� ��� ��������� ��� ������������� ������������
	ASSERT_FALSE(event->pushPwdChanged(USER_admin, USER_SOURCE_pi, USER_operator));
	ASSERT_FALSE(event->isEmpty());
	ASSERT_FALSE(event->pushPwdChanged(USER_engineer, USER_SOURCE_pc, USER_factory));
	ASSERT_FALSE(event->isEmpty());
	ASSERT_FALSE(event->pushPwdChanged(USER_engineer, USER_SOURCE_pi, USER_MAX));
	ASSERT_FALSE(event->isEmpty());

	// ������: ����������� ������� ������������
	user = static_cast<user_t> (USER_MAX + 1);
	ASSERT_FALSE(event->pushPwdChanged(user, USER_SOURCE_pc, USER_operator));
	ASSERT_FALSE(event->isEmpty());

	// ������: ����������� �������� �������
	source = static_cast<userSrc_t> (USER_SOURCE_MAX + 1);
	ASSERT_FALSE(event->pushPwdChanged(USER_engineer, source, USER_operator));
	ASSERT_FALSE(event->isEmpty());

	// ** �������� ����������� ������ **

	// ������ 1
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_operator, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_chgPwdAdm, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 2
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_chgPwdEng, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 3
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_MAX, source);
	ASSERT_EQ(TSecurityEvent::EVENT_chgPwdEng, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 4
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_admin, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_chgPwdAdm, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 5
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_MAX, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_chgPwdEng, ev);
	ASSERT_TRUE(event->isEmpty());

	// ���������� �� ������������ ������
	ASSERT_FALSE(event->pop(user, source, ev));
	ASSERT_EQ(USER_MAX, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_chgPwdEng, ev);
	ASSERT_TRUE(event->isEmpty());
}

TEST_F(TSecurityTest_Test, pushPwdReset) {
	user_t user;
	userSrc_t source;
	TSecurityEvent::event_t ev;

	ASSERT_TRUE(event->isEmpty());

	// ������ 1
	ASSERT_TRUE(event->pushPwdReset(USER_operator, USER_SOURCE_pi));
	ASSERT_FALSE(event->isEmpty());

	// ������ 2
	ASSERT_TRUE(event->pushPwdReset(USER_engineer, USER_SOURCE_pc));
	ASSERT_FALSE(event->isEmpty());

	// ������ 3
	ASSERT_TRUE(event->pushPwdReset(USER_engineer, USER_SOURCE_MAX));
	ASSERT_FALSE(event->isEmpty());

	// ������ 4
	ASSERT_TRUE(event->pushPwdReset(USER_admin, USER_SOURCE_pc));
	ASSERT_FALSE(event->isEmpty());

	// ������ 5
	ASSERT_TRUE(event->pushPwdReset(USER_MAX, USER_SOURCE_pi));
	ASSERT_FALSE(event->isEmpty());

	// ������: ����������� ������� ������������
	user = static_cast<user_t> (USER_MAX + 1);
	ASSERT_FALSE(event->pushPwdReset(user, USER_SOURCE_pc));
	ASSERT_FALSE(event->isEmpty());

	// ������: ����������� �������� �������
	source = static_cast<userSrc_t> (USER_SOURCE_MAX + 1);
	ASSERT_FALSE(event->pushPwdReset(USER_engineer, source));
	ASSERT_FALSE(event->isEmpty());

	// ** �������� ����������� ������ **

	// ������ 1
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_operator, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_resetPwd, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 2
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_resetPwd, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 3
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_MAX, source);
	ASSERT_EQ(TSecurityEvent::EVENT_resetPwd, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 4
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_admin, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_resetPwd, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 5
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_MAX, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_resetPwd, ev);
	ASSERT_TRUE(event->isEmpty());

	// ���������� �� ������������ ������
	ASSERT_FALSE(event->pop(user, source, ev));
	ASSERT_EQ(USER_MAX, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_resetPwd, ev);
	ASSERT_TRUE(event->isEmpty());
}

TEST_F(TSecurityTest_Test, pushPwdWrong) {
	user_t user;
	userSrc_t source;
	TSecurityEvent::event_t ev;

	ASSERT_TRUE(event->isEmpty());

	// ������ 1
	ASSERT_TRUE(event->pushPwdWrong(USER_operator, USER_SOURCE_pi, USER_admin));
	ASSERT_FALSE(event->isEmpty());

	// ������ 2
	ASSERT_TRUE(event->pushPwdWrong(USER_engineer, USER_SOURCE_pc, USER_engineer));
	ASSERT_FALSE(event->isEmpty());

	// ������ 3
	ASSERT_TRUE(event->pushPwdWrong(USER_engineer, USER_SOURCE_MAX, USER_engineer));
	ASSERT_FALSE(event->isEmpty());

	// ������ 4
	ASSERT_TRUE(event->pushPwdWrong(USER_admin, USER_SOURCE_pc, USER_admin));
	ASSERT_FALSE(event->isEmpty());

	// ������ 5
	ASSERT_TRUE(event->pushPwdWrong(USER_MAX, USER_SOURCE_pi, USER_engineer));
	ASSERT_FALSE(event->isEmpty());

	// ������: ������ ������� ��� ��������� ��� ������������� ������������
	ASSERT_FALSE(event->pushPwdWrong(USER_admin, USER_SOURCE_pi, USER_operator));
	ASSERT_FALSE(event->isEmpty());
	ASSERT_FALSE(event->pushPwdWrong(USER_engineer, USER_SOURCE_pc, USER_factory));
	ASSERT_FALSE(event->isEmpty());
	ASSERT_FALSE(event->pushPwdWrong(USER_engineer, USER_SOURCE_pi, USER_MAX));
	ASSERT_FALSE(event->isEmpty());

	// ������: ����������� ������� ������������
	user = static_cast<user_t> (USER_MAX + 1);
	ASSERT_FALSE(event->pushPwdWrong(user, USER_SOURCE_pc, USER_operator));
	ASSERT_FALSE(event->isEmpty());

	// ������: ����������� �������� �������
	source = static_cast<userSrc_t> (USER_SOURCE_MAX + 1);
	ASSERT_FALSE(event->pushPwdWrong(USER_engineer, source, USER_operator));
	ASSERT_FALSE(event->isEmpty());

	// ** �������� ����������� ������ **

	// ������ 1
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_operator, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_wrgPwdAdm, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 2
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_wrgPwdEng, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 3
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_MAX, source);
	ASSERT_EQ(TSecurityEvent::EVENT_wrgPwdEng, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 4
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_admin, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_wrgPwdAdm, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 5
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_MAX, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_wrgPwdEng, ev);
	ASSERT_TRUE(event->isEmpty());

	// ���������� �� ������������ ������
	ASSERT_FALSE(event->pop(user, source, ev));
	ASSERT_EQ(USER_MAX, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_wrgPwdEng, ev);
	ASSERT_TRUE(event->isEmpty());
}

TEST_F(TSecurityTest_Test, pushUserChanged) {
	user_t user;
	userSrc_t source;
	TSecurityEvent::event_t ev;

	ASSERT_TRUE(event->isEmpty());

	// ������ 1
	ASSERT_TRUE(event->pushUserChanged(USER_operator, USER_SOURCE_pi, USER_admin));
	ASSERT_FALSE(event->isEmpty());

	// ������ 2
	ASSERT_TRUE(event->pushUserChanged(USER_engineer, USER_SOURCE_pc, USER_engineer));
	ASSERT_FALSE(event->isEmpty());

	// ������ 3
	ASSERT_TRUE(event->pushUserChanged(USER_engineer, USER_SOURCE_MAX, USER_engineer));
	ASSERT_FALSE(event->isEmpty());

	// ������ 4
	ASSERT_TRUE(event->pushUserChanged(USER_admin, USER_SOURCE_pc, USER_admin));
	ASSERT_FALSE(event->isEmpty());

	// ������ 5
	ASSERT_TRUE(event->pushUserChanged(USER_MAX, USER_SOURCE_pi, USER_engineer));
	ASSERT_FALSE(event->isEmpty());

	// ������ 6
	ASSERT_TRUE(event->pushUserChanged(USER_admin, USER_SOURCE_pi, USER_operator));
	ASSERT_FALSE(event->isEmpty());

	// ������: ������ ������� ��� ������������� ������������
	ASSERT_FALSE(event->pushUserChanged(USER_engineer, USER_SOURCE_pc, USER_factory));
	ASSERT_FALSE(event->isEmpty());
	ASSERT_FALSE(event->pushUserChanged(USER_engineer, USER_SOURCE_pi, USER_MAX));
	ASSERT_FALSE(event->isEmpty());

	// ������: ����������� ������� ������������
	user = static_cast<user_t> (USER_MAX + 1);
	ASSERT_FALSE(event->pushUserChanged(user, USER_SOURCE_pc, USER_operator));
	ASSERT_FALSE(event->isEmpty());

	// ������: ����������� �������� �������
	source = static_cast<userSrc_t> (USER_SOURCE_MAX + 1);
	ASSERT_FALSE(event->pushUserChanged(USER_engineer, source, USER_operator));
	ASSERT_FALSE(event->isEmpty());

	// ** �������� ����������� ������ **

	// ������ 1
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_operator, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_setAdmin, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 2
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_setEngineer, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 3
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_MAX, source);
	ASSERT_EQ(TSecurityEvent::EVENT_setEngineer, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 4
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_admin, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_setAdmin, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 5
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_MAX, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_setEngineer, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 6
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_admin, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_setOperator, ev);
	ASSERT_TRUE(event->isEmpty());

	// ���������� �� ������������ ������
	ASSERT_FALSE(event->pop(user, source, ev));
	ASSERT_EQ(USER_admin, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_setOperator, ev);
	ASSERT_TRUE(event->isEmpty());
}

TEST_F(TSecurityTest_Test, pushUserChangeAuto) {
	user_t user;
	userSrc_t source;
	TSecurityEvent::event_t ev;

	ASSERT_TRUE(event->isEmpty());

	// ������ 1
	ASSERT_TRUE(event->pushUserChangeAuto(USER_operator, USER_SOURCE_pi));
	ASSERT_FALSE(event->isEmpty());

	// ������ 2
	ASSERT_TRUE(event->pushUserChangeAuto(USER_engineer, USER_SOURCE_pc));
	ASSERT_FALSE(event->isEmpty());

	// ������ 3
	ASSERT_TRUE(event->pushUserChangeAuto(USER_engineer, USER_SOURCE_MAX));
	ASSERT_FALSE(event->isEmpty());

	// ������ 4
	ASSERT_TRUE(event->pushUserChangeAuto(USER_admin, USER_SOURCE_pc));
	ASSERT_FALSE(event->isEmpty());

	// ������ 5
	ASSERT_TRUE(event->pushUserChangeAuto(USER_MAX, USER_SOURCE_pi));
	ASSERT_FALSE(event->isEmpty());

	// ������: ����������� ������� ������������
	user = static_cast<user_t> (USER_MAX + 1);
	ASSERT_FALSE(event->pushUserChangeAuto(user, USER_SOURCE_pc));
	ASSERT_FALSE(event->isEmpty());

	// ������: ����������� �������� �������
	source = static_cast<userSrc_t> (USER_SOURCE_MAX + 1);
	ASSERT_FALSE(event->pushUserChangeAuto(USER_engineer, source));
	ASSERT_FALSE(event->isEmpty());

	// ** �������� ����������� ������ **

	// ������ 1
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_operator, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_autoEnd, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 2
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_autoEnd, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 3
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_MAX, source);
	ASSERT_EQ(TSecurityEvent::EVENT_autoEnd, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 4
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_admin, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_autoEnd, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 5
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_MAX, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_autoEnd, ev);
	ASSERT_TRUE(event->isEmpty());

	// ���������� �� ������������ ������
	ASSERT_FALSE(event->pop(user, source, ev));
	ASSERT_EQ(USER_MAX, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_autoEnd, ev);
	ASSERT_TRUE(event->isEmpty());
}

TEST_F(TSecurityTest_Test, overflow) {
	user_t user;
	userSrc_t source;
	TSecurityEvent::event_t ev;

	ASSERT_TRUE(event->isEmpty());

	// ������ 1
	ASSERT_TRUE(event->pushPwdBlocked(USER_operator, USER_SOURCE_pi, USER_admin));
	ASSERT_FALSE(event->isEmpty());

	// ������ 2
	ASSERT_TRUE(event->pushPwdBlocked(USER_engineer, USER_SOURCE_pc, USER_engineer));
	ASSERT_FALSE(event->isEmpty());

	// ������ 3
	ASSERT_TRUE(event->pushPwdChanged(USER_operator, USER_SOURCE_pi, USER_admin));
	ASSERT_FALSE(event->isEmpty());

	// ������ 4
	ASSERT_TRUE(event->pushPwdChanged(USER_engineer, USER_SOURCE_pc, USER_engineer));
	ASSERT_FALSE(event->isEmpty());

	// ������ 5
	ASSERT_TRUE(event->pushPwdReset(USER_operator, USER_SOURCE_pi));
	ASSERT_FALSE(event->isEmpty());

	// ������ 6
	ASSERT_TRUE(event->pushPwdReset(USER_engineer, USER_SOURCE_pc));
	ASSERT_FALSE(event->isEmpty());

	// ������ 7
	ASSERT_TRUE(event->pushPwdWrong(USER_operator, USER_SOURCE_pi, USER_admin));
	ASSERT_FALSE(event->isEmpty());

	// ������ 8
	ASSERT_TRUE(event->pushPwdWrong(USER_engineer, USER_SOURCE_pc, USER_engineer));
	ASSERT_FALSE(event->isEmpty());

	// ������ 9
	ASSERT_TRUE(event->pushUserChanged(USER_operator, USER_SOURCE_pi, USER_admin));
	ASSERT_FALSE(event->isEmpty());

	// ������ 10
	ASSERT_TRUE(event->pushUserChanged(USER_engineer, USER_SOURCE_pc, USER_engineer));
	ASSERT_FALSE(event->isEmpty());

	// ������ 11
	ASSERT_FALSE(event->pushUserChangeAuto(USER_operator, USER_SOURCE_pi));
	ASSERT_FALSE(event->isEmpty());

	// ������ 12
	ASSERT_FALSE(event->pushUserChangeAuto(USER_engineer, USER_SOURCE_pc));
	ASSERT_FALSE(event->isEmpty());

	// ** �������� ����������� ������ **

	// ������ 1
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_operator, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_blkAdmin, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 2
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_blkEngineer, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 3
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_operator, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_chgPwdAdm, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 4
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_chgPwdEng, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 5
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_operator, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_resetPwd, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 6
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_resetPwd, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 7
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_operator, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_wrgPwdAdm, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 8
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_wrgPwdEng, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 9
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_operator, user);
	ASSERT_EQ(USER_SOURCE_pi, source);
	ASSERT_EQ(TSecurityEvent::EVENT_setAdmin, ev);
	ASSERT_FALSE(event->isEmpty());

	// ������ 10
	ASSERT_TRUE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_setEngineer, ev);
	ASSERT_TRUE(event->isEmpty());

	// ������ 11 - �� ���
	ASSERT_FALSE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_setEngineer, ev);
	ASSERT_TRUE(event->isEmpty());

	// ������ 12 - �� ���
	ASSERT_FALSE(event->pop(user, source, ev));
	ASSERT_EQ(USER_engineer, user);
	ASSERT_EQ(USER_SOURCE_pc, source);
	ASSERT_EQ(TSecurityEvent::EVENT_setEngineer, ev);
	ASSERT_TRUE(event->isEmpty());
}
