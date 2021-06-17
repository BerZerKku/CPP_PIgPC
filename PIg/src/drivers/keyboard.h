/*
 * keyboard.h
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "glbDefine.h"

/// ����� ������������ * 10��
#define TIME_DELAY 10
/// ����� �� ���������� �����������
#define TIME_DELAY_REPEAT 100

/// ������� ������
enum eKEY {
	KEY_NO = 0x00,	// ��� ������� ������
	KEY_EMPTY,		// ���� ������ ������, �� ��� �����-���� �������
	KEY_FUNC,		// �������
	KEY_ENTER,		// ����
	KEY_CANCEL,		// ������
	KEY_DOWN,		// ����
	KEY_LEFT,		// �����
	KEY_RIGHT,		// ������
	KEY_UP,			// �����

	KEY_MENU,		// ���� � ����
	KEY_RESET,		// �����
	KEY_PUSK,		// ����	(���� ��������� � ������)
	KEY_PUSK_UD,	// ����. ���� ���
	KEY_PUSK_NALAD,	// �.���� / �.���� ��� (���������� ����)
	KEY_RESET_IND,	// ����� ���.
	KEY_CALL,		// �����

	KEY_AC_PUSK,	// ���� ��
	KEY_AC_PUSK_UD,	// ����. ���� ��
	KEY_AC_RESET,	// ����� ��
	KEY_AC_REGIME	// ����� ��
};

void vKEYmain	(void);
void vKEYset	(eGB_TYPE_DEVICE type);
eKEY eKEYget	(void);
uint8_t timePressKey();

#endif /* KEYBOARD_H_ */
