/*
 * bspParam.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMBSP_H_
#define PARAMBSP_H_

#include "src/glbDefine.h"
#include "src/paramDef.h"
#include "src/paramGlb.h"
#include "src/paramPrm.h"
#include "src/paramPrd.h"
#include "src/paramUart.h"
#include "src/menu/dateTime.hpp"
#include "src/menu/journalEntry.hpp"
#include "src/menu/jrnScada.hpp"
#include "src/menu/txCom.hpp"
#include "src/parameter/LocalParams.h"

/// ��������� ���������� ���
struct stGBparam {
	// false - �������� ��� ���� ��������� ���� ��� ������� ��� ��������
	bool device;

    // ���� ������� ����� � ���.
    bool connectionBsp;
    // ���� ������� ����� � ��.
    bool connectionPc;

	// ��� ��������
	eGB_TYPE_DEVICE typeDevice;

	// ��� ����������� ��������
	eGB_TYPE_OPTO typeOpto;

	// ������
	TPassword password;

	// ����/�����
    TDateTime DateTime;

	// ����/����� ��� � ������ ��������� ����� �������
    TDateTime DateTimeReq;

	// ����/�����

	// ���������� ���������
	TMeasuredParameters measParam;

	// ������� ��������� ���������
	TDeviceDef def;
	TDevicePrm prm;
	TDevicePrd prd;
	TDeviceGlb glb;

	// ��������� ���������
	LocalParams local;

	// ��������� ��� ������ � ���������������� ������
	TUartData Uart;

	// ����� ������
	TTxCom txComBuf;

	// ������ � �������
	TJournalEntry jrnEntry;

	// ������ ��� SCADA
	TJrnSCADA jrnScada;

	// �����
	TTest test;
};

#endif /* PARAMBSP_H_ */
