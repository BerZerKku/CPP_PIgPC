/*
 * bspParam.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMBSP_H_
#define PARAMBSP_H_

#include "glbDefine.h"
#include "paramPrm.h"
#include "paramPrd.h"
#include "paramDef.h"
#include "paramGlb.h"
#include "paramUart.h"
#include "parameter/LocalParams.h"
#include "menu/measure.h"
#include "menu/password.h"
#include "menu/test.h"
#include "menu/txCom.h"


/// ��������� ���������� ���
struct stGBparam {
	// false - �������� ��� ���� ��������� ���� ��� ������� ��� ��������
	bool device;

	bool connectionBsp;
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
	TMeasure measParam;

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
