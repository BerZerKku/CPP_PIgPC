/*
 * keyboard.h
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */

#ifndef KBD_LAYOUT_H_
#define KBD_LAYOUT_H_

#include "glbDefine.h"

#define NUM_KEY_IN_LAYOUT 9

/// ������� ������
enum eKEY {
    KEY_NO = 0x00,  // ��� ������� ������
    KEY_EMPTY,      // ���� ������ ������, �� ��� �����-���� �������
    KEY_FUNC,       // �������
    KEY_ENTER,      // ����
    KEY_CANCEL,     // ������
    KEY_DOWN,       // ����
    KEY_LEFT,       // �����
    KEY_RIGHT,      // ������
    KEY_UP,         // �����

    KEY_MENU,       // ���� � ����
    KEY_RESET,      // �����
    KEY_PUSK,       // ����	(���� ��������� � ������)
    KEY_PUSK_UD,    // ����. ���� ���
    KEY_PUSK_NALAD, // �.���� / �.���� ��� (���������� ����)
    KEY_RESET_IND,  // ����� ���.
    KEY_CALL,       // �����

    KEY_AC_PUSK,    // ���� ��
    KEY_AC_PUSK_UD, // ����. ���� ��
    KEY_AC_RESET,   // ����� ��
    KEY_AC_REGIME,  // ����� ��
    //
    KEY_MAX
};

void vKEYset(eGB_TYPE_DEVICE type);
eKEY vKEYgetButton(uint8_t number);
eKEY vKEYgetButtonLayout(uint8_t number);

#endif /* KBD_LAYOUT_H_ */
