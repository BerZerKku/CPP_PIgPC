/*
 * glb_def.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

/**
 * 	������������ �������� ��� ��������� ���������� �� 1 ������ ����������,
 * 	��� �������� � ������� ���������� �������� � �������� �����������
 * 	��������������.
 * 	��� ��� ��� ����� ���������� �������� �������� ������ ���� ������ �� 1 !!
 */

#ifndef GLBDEF_H_
#define GLBDEF_H_

#include <stdint.h>
#include "avr.h"
#include "src/debug/debug.hpp"

/// ������ �������
#define SIZE_OF(mas) (sizeof(mas) / sizeof(mas[0]))

/// ������ ��������������
#define PASSWORD_ADMIN 6352

/// ������ ������������ �� ���������
#define PASSWORD_USER 0

/// ������ ������� ��������
#define VERS 0x6401

/// ����������� ���-�� ������ �� ����� (������ ���� ������ 8)
#define MAX_NUM_COM_PRM 32

/// ������������ ���-�� ������ �� �������� (������ ���� ������ 8)
#define MAX_NUM_COM_PRD 32

/// ������������ ���-�� ���������� ������ (+1 - ��� 0 �������)
#define MAX_NUM_COM_RING 96

/// �������������� ���� CHAR � INT
#define TO_INT16(high, low) (((uint16_t) (high) << 8) + (low))

/// ����������� ��������� ���-�� ��������� ���������
#define MAX_NUM_DEVICE_STATE 14

/// ������������ ���-�� �������������� ��� ������ ����������
#define MAX_NUM_FAULTS 16

/// ������������ ���-�� �������������� ��� ������ ����������
#define MAX_NUM_WARNINGS 16

/// ����������� ���-�� ������ ������������ �� ���� �������
#define MIN_NUM_COM_SEND_IN_1_SEK 8

///	������������ ���-�� ������� �������
// �� ������ ������ ���� ������ ��� ������ ����, �.�. ��� ���������� 2 �������
// � ��� �� �������� ��������� ������� ������� ��� ������ � ��� ��
#define MAX_NUM_FAST_COM 3

/// ������������ ���-�� ������ � ������ ������
#define MAX_NUM_COM_BUF1 10

/// ������������ ���-�� ������ �� ������ ������
#define MAX_NUM_COM_BUF2 4

// �� ��������� ������� �������, �.�. ��� ��������� ���������� �����
#if (MIN_NUM_COM_SEND_IN_1_SEK - MAX_NUM_COM_BUF2 < 4)
#error ������� ���� �������������� ������ ������������ � ��� �� ���� ������!!!
#endif

/// ������������ ���-�� �������� � ������
#define MAX_NUM_TEST_SIGNAL 100

// ����� �������� ������ (+1 - ������ ����� ������)
#define STRING_LENGHT (11 + 1)

// ����� ������ (+1 - ������ ����� ������)
#define NAME_PARAM_LENGHT (20 + 1)

/// ������������ � ����������� ��� ���� ������� � ������� �������
#define MIN_JRN_EVENT_VALUE 1
#define MAX_JRN_EVENT_VALUE 38

#define GLB_JRN_EVENT_K400_MAX 	512		/// ���-�� ������� � ������� ������� �400
#define GLB_JRN_EVENT_R400M_MAX 512		/// ���-�� ������� � ������� ������� �400�
#define GLB_JRN_EVENT_RZSK_MAX 	256		/// ���-�� ������� � ������� ������� ����
#define GLB_JRN_EVENT_OPTO_MAX 	256		/// ���-�� ������� � ������� ������� ������

#define GLB_JRN_DEF_K400_MAX 	0		///< ���-�� ������� � ������� ������ �400
#define GLB_JRN_DEF_R400M_MAX 	1024	///< ���-�� ������� � ������� ������ �400�
#define GLB_JRN_DEF_RZSK_MAX 	256		///< ���-�� ������� � ������� ������ ����
#define GLB_JRN_DEF_OPTO_MAX 	2048	///< ���-�� ������� � ������� ������ ������

#define GLB_JRN_PRM_K400_MAX 	512		///< ���-�� ������� � ������� ��������� �400
#define GLB_JRN_PRM_RZSK_MAX 	256		///< ���-�� ������� � ������� ��������� ����
#define GLB_JRN_PRM_OPTO_MAX	256		///< ���-�� ������� � ������� ��������� ������

#define GLB_JRN_PRD_K400_MAX 	512		/// ���-�� ������� � ������� ����������� �400
#define GLB_JRN_PRD_RZSK_MAX 	256		/// ���-�� ������� � ������� ����������� ����
#define GLB_JRN_PRD_OPTO_MAX	256		/// ���-�� ������� � ������� ����������� ������

/// ����������� ��������� ���-�� ������� � ����� ������
#define GLB_JRN_MAX 2048

/// ������������ ��������� ���.����� � ������ ������� �400/����
#define GLB_JRN_DOP_MAX 6

/// ����� ���� ����� ������ � ����, ��
#define MENU_TIME_CYLCE 200

/// ����� ������ ���.��������� �� �����, ��
#define TIME_MESSAGE (3000 / MENU_TIME_CYLCE)

/// ����� ������ ������� ����� ����������
enum eMENU_ENTER_PARAM {
	MENU_ENTER_PARAM_NO,		///< ������ ��������� ���������
	MENU_ENTER_PARAM_INT,		///< ��������� ���������, ����� ��������
	MENU_ENTER_PARAM_LIST,		///< ��������� ���������, ����� �� ������
	MENU_ENTER_PARAM_LIST_2,	///< ��������� ���������, ����� �� ������ ��������
	MENU_ENTER_PARAM_U_COR,		///< ��������� ���������, ��������� ����������
	MENU_ENTER_PASSWORD,		///< ���� ������
	MENU_ENTER_PASSWORD_NEW,	///< ���� ������ ������
	MENU_ENTER_PASSWORD_READY,	///< ������ ������ ������
    MENU_ENTER_PASSWORD_N_READY,///< ����� ������ ������ ���������
	MENU_ENTER_PARAM_READY,		///< ���������� �������� ��������
	MENU_ENTER_PARAM_MESSAGE,	///< ����� ��������� �� ����� ��� ����� ���������
};

/// ��� ��������
enum eGB_TYPE_DEVICE {
	AVANT_NO = 0,	// ��������� ��������
	AVANT_R400,		//
	AVANT_RZSK,		//
	AVANT_K400,		//
	AVANT_R400M,	//
	AVANT_OPTO,		//
	AVANT_MAX
};

/// ��� ����������� ��������
enum eGB_TYPE_OPTO {
	TYPE_OPTO_STANDART = 0,	// ����������� ������
	TYPE_OPTO_RING_UNI,		// ���������� ���������������� ������
	TYPE_OPTO_RING_BI,		// ��������� ��������������� ������
	TYPE_OPTO_MAX
};

/// ����������
enum eGB_DEVICE {
	GB_DEVICE_MIN = 0,
	GB_DEVICE_DEF = 0,
	GB_DEVICE_PRM,
	GB_DEVICE_PRD,
	GB_DEVICE_GLB,
	GB_DEVICE_MAX
};

/// ���������� � ������� ������� �400
enum eGB_DEVICE_K400 {
	GB_DEVICE_K400_MIN = 0,
	GB_DEVICE_K400_DEF = 0,
	GB_DEVICE_K400_PRM1,
	GB_DEVICE_K400_PRM2,
	GB_DEVICE_K400_PRD,
	GB_DEVICE_K400_GLB,
	GB_DEVICE_K400_PRM1_PRM2,
	GB_DEVICE_K400_MAX
};

/// ��� ������ �����
enum eGB_TYPE_LINE {
	GB_TYPE_LINE_MIN = 1,
	GB_TYPE_LINE_UM = 1,
	GB_TYPE_LINE_OPTO = 2,
	GB_TYPE_LINE_E1 = 3,
	GB_TYPE_LINE_MAX
};

/// ��� ���������, � ����� ���������� ������ ��������.
enum eGB_SEND_TYPE {
    GB_SEND_NO,             ///< ������� ������������ ��������� ���
    GB_SEND_INT8,           ///< ���������� ���� ���� ������.
    GB_SEND_INT8_DOP,       ///< ���������� ��� ����� ������ (��������, ���.����).
    GB_SEND_DOP_INT8,       ///< ���������� ��� ����� ������ (���.����, ��������).
    GB_SEND_DOP_INT16_LE,   ///< ���������� ��� ����� ������ (���.����, int16&0xFF, in16>>8).
    GB_SEND_INT16_BE,       ///< ���������� ��� ����� ������ (in16>>8, int16&0xFF).
    GB_SEND_DOP_BITES,      ///< ���������� ������� ���������� (���.����, ��������).
    GB_SEND_BITES_DOP,      ///< ���������� ������� ���������� (��������, ���.����).
    GB_SEND_COR_U,          ///< ���������� ��������� ����������.
    GB_SEND_COR_I           ///< ���������� ��������� ����.
};

/// ���-�� ��������� � �����
// 	������ ��� 1 � 2 ������������ ��� ���������� � 3-� ��������. �� ������ !!!
enum eGB_NUM_DEVICES {
	GB_NUM_DEVICES_MIN = 1,
	GB_NUM_DEVICES_2 = 1,
	GB_NUM_DEVICES_3 = 2,
	GB_NUM_DEVICES_MAX
};

/// ������ ������
enum eGB_REGIME {
	GB_REGIME_MIN = 0, 				//
	GB_REGIME_DISABLED = 0,			// �������
	GB_REGIME_READY,				// �����
	GB_REGIME_ENABLED,				// ������
	GB_REGIME_TALK,					// ����
	GB_REGIME_TEST_1,				// ���� ���
	GB_REGIME_TEST_2,				// ���� ���
	GB_REGIME_MAX,
};

// ������ ������ ��� ����� � ����������
// ������� �� ��������� � eGB_REGIME
enum eGB_REGIME_ENTER {
	GB_REGIME_ENTER_MIN = 0,
	GB_REGIME_ENTER_DISABLED = 0,
	GB_REGIME_ENTER_ENABLED = 1,
	GB_REGIME_ENTER_TEST_1,
	GB_REGIME_ENTER_TEST_2,
	GB_REGIME_ENTER_MAX
};

/// �������
enum eGB_COM {
    // + �������� ��� ������� �����������������
	GB_COM_NO = 0,
	GB_COM_DEF_GET_DEF_TYPE 	= 0x01,	// +
	GB_COM_DEF_GET_LINE_TYPE 	= 0x02,	// +
	GB_COM_DEF_GET_T_NO_MAN 	= 0x03,	// +
	GB_COM_DEF_GET_DELAY 		= 0x04,	// +
	GB_COM_DEF_GET_OVERLAP 		= 0x05,	// +
	GB_COM_DEF_GET_RZ_DEC 		= 0x06,	// +
	GB_COM_DEF_GET_PRM_TYPE 	= 0x07,	// + ! � �400� ��� �������� ������ ��
	GB_COM_DEF_GET_FREQ_PRD 	= 0x08,	// +
	GB_COM_DEF_GET_RZ_THRESH 	= 0x09,	// + ! � �400� ��� ������� ���
	GB_COM_DEF_GET_TYPE_AC 		= 0x0A,	// +
	GB_COM_DEF_GET_LIMIT_PRD	= 0x0B,	// +
	GB_COM_PRM_GET_TIME_ON 		= 0x11,	// +
	GB_COM_PRM_GET_TEST_COM		= 0x12,	// +
	GB_COM_PRM_GET_TIME_OFF 	= 0x13,	// +
	GB_COM_PRM_GET_BLOCK_COM 	= 0x14,	// +
	GB_COM_PRM_GET_FREQ_CORR	= 0x15,	// +
	GB_COM_PRM_GET_INC_SAFETY	= 0x16,	// +
	GB_COM_PRM_GET_DR_STATE 	= 0x17, // +
	GB_COM_PRM_GET_DR_BLOCK		= 0x18,	// +
	GB_COM_PRM_GET_DR_COM		= 0x19,	// +
	GB_COM_PRM_GET_RING_COM_REC	= 0x1B,	// +
	GB_COM_PRM_GET_COM			= 0x1C, // +
	GB_COM_PRM_GET_COM_SIGN		= 0x1D,	// +
    GB_COM_PRM_GET_BLOCK_ALL    = 0x1E, //
	GB_COM_PRD_GET_TIME_ON 		= 0x21,	// +
	GB_COM_PRD_GET_DURATION 	= 0x22,	// +
	GB_COM_PRD_GET_FREQ_CORR	= 0x23,	// +
	GB_COM_PRD_GET_BLOCK_COM 	= 0x24,	// +
	GB_COM_PRD_GET_LONG_COM 	= 0x25,	// +
	GB_COM_PRD_GET_TEST_COM 	= 0x26,	// +
	GB_COM_PRD_GET_DR_STATE 	= 0x27, // +
	GB_COM_PRD_GET_DR_BLOCK		= 0x28,	// +
	GB_COM_PRD_GET_COM_A		= 0x29,	// +
	GB_COM_PRD_GET_RING_COM_TR	= 0x2B,	// +
	GB_COM_PRD_GET_COM			= 0x2C,	// +
	GB_COM_PRD_GET_CF_TM		= 0x2D,	// +
	GB_COM_PRD_GET_COM_SIGN		= 0x2E, // +
	GB_COM_GET_SOST 			= 0x30,	// +
	GB_COM_GET_FAULT 			= 0x31,	// +
	GB_COM_GET_TIME 			= 0x32,	// +
	GB_COM_GET_COR_U_I 			= 0x33,	// + ! � ������ ��� ��������������
	GB_COM_GET_MEAS 			= 0x34,	// +
	GB_COM_GET_TIME_SINCHR 		= 0x35,	// +
	GB_COM_GET_COM_PRM_KEEP 	= 0x36, // + ! � �400� ��� U��� �����������
	GB_COM_GET_COM_PRD_KEEP 	= 0x37,	// + ! ������������� ��� ���������� ��������, ������������, �������������� � ������ �� D � �400 � �.�.
	GB_COM_GET_NET_ADR 			= 0x38,	// +
	GB_COM_GET_TIME_RERUN 		= 0x39,	// + ! � �400� ��� ��������� ��� ��������������
	GB_COM_GET_FREQ 			= 0x3A,	// +
	GB_COM_GET_DEVICE_NUM 		= 0x3B,	// +
	GB_COM_GET_CF_THRESHOLD 	= 0x3C,	// + ! ����� �������������� � �����������
	GB_COM_GET_OUT_CHECK 		= 0x3D,	// +
	GB_COM_GET_TEST 			= 0x3E,	// +
	GB_COM_GET_VERS 			= 0x3F,	// +
	GB_COM_PRM_ENTER 			= 0x51,	// +
	GB_COM_SET_REG_DISABLED 	= 0x70,	// +
	GB_COM_SET_REG_ENABLED 		= 0x71,	// +
	GB_COM_SET_CONTROL 			= 0x72,	// +
	GB_COM_SET_PASSWORD 		= 0x73,	// + ! ������ � ��
	GB_COM_GET_PASSWORD 		= 0x74,	// + ! ������ � ��
	GB_COM_SET_REG_TEST_2 		= 0x7D,	// +
	GB_COM_SET_REG_TEST_1 		= 0x7E,	// +
	GB_COM_DEF_SET_DEF_TYPE 	= 0x81,	// +
	GB_COM_DEF_SET_LINE_TYPE 	= 0x82,	// +
	GB_COM_DEF_SET_T_NO_MAN 	= 0x83,	// +
	GB_COM_DEF_SET_DELAY 		= 0x84,	// +
	GB_COM_DEF_SET_OVERLAP 		= 0x85,	// +
	GB_COM_DEF_SET_RZ_DEC 		= 0x86,	// +
	GB_COM_DEF_SET_PRM_TYPE 	= 0x87,	// + ! � �400� ��� �������� ������ ��
	GB_COM_DEF_SET_FREQ_PRD 	= 0x88,	// +
	GB_COM_DEF_SET_RZ_THRESH 	= 0x89,	// + ! � �400� ��� ������� ���
	GB_COM_DEF_SET_TYPE_AC 		= 0x8A,	// +
	GB_COM_DEF_SET_LIMIT_PRD	= 0x8B,	//
	GB_COM_PRM_SET_TIME_ON 		= 0x91,	// +
	GB_COM_PRM_SET_TEST_COM		= 0x92,	// +
	GB_COM_PRM_SET_TIME_OFF 	= 0x93,	// +
	GB_COM_PRM_SET_BLOCK_COM 	= 0x94,	// +
	GB_COM_PRM_SET_FREQ_CORR	= 0x95,	// +
	GB_COM_PRM_SET_INC_SAFETY	= 0x16,	// +
	GB_COM_PRM_SET_DR_STATE 	= 0x97, // +
	GB_COM_PRM_SET_DR_BLOCK		= 0x98,	// +
	GB_COM_PRM_SET_DR_COM		= 0x99,	// +
	GB_COM_PRM_RES_IND			= 0x9A,	// +
	GB_COM_PRM_SET_RING_COM_REC	= 0x9B,	// +
	GB_COM_PRM_SET_COM			= 0x9C,	// +
	GB_COM_PRM_SET_COM_SIGN		= 0x9D,	// +
    GB_COM_PRM_SET_BLOCK_ALL    = 0x9E, //
	GB_COM_PRD_SET_TIME_ON 		= 0xA1,	// +
	GB_COM_PRD_SET_DURATION 	= 0xA2,	// +
	GB_COM_PRD_SET_FREQ_CORR	= 0xA3,	// +
	GB_COM_PRD_SET_BLOCK_COM 	= 0xA4,	// +
	GB_COM_PRD_SET_LONG_COM 	= 0xA5,	// +
	GB_COM_PRD_SET_TEST_COM 	= 0xA6,	// +
	GB_COM_PRD_SET_DR_STATE 	= 0xA7, // +
	GB_COM_PRD_SET_DR_BLOCK		= 0xA8,	// +
	GB_COM_PRD_SET_COM_A		= 0xA9,	// +
	GB_COM_PRD_RES_IND 			= 0xAA,	// +
	GB_COM_PRD_SET_RING_COM_TR	= 0xAB,	// +
	GB_COM_PRD_SET_COM			= 0xAC,	// +
	GB_COM_PRD_SET_CF_TM		= 0xAD,	// +
	GB_COM_PRD_SET_COM_SIGN		= 0xAE, // +
	GB_COM_SET_TIME 			= 0xB2,	// +
	GB_COM_SET_COR_U_I 			= 0xB3,	// +
	GB_COM_SET_TIME_SINCHR 		= 0xB5,	// +
	GB_COM_SET_COM_PRM_KEEP 	= 0xB6, // + ! � �400� ��� U��� �����������
	GB_COM_SET_COM_PRD_KEEP 	= 0xB7, // + ! ������������� ��� ���������� ��������, ������������
	GB_COM_SET_NET_ADR 			= 0xB8,	// +
	GB_COM_SET_TIME_RERUN 		= 0xB9,	// +
	GB_COM_SET_FREQ 			= 0xBA,	// +
	GB_COM_SET_DEVICE_NUM 		= 0xBB,	// +
	GB_COM_SET_CF_THRESHOLD 	= 0xBC,	// +
	GB_COM_SET_OUT_CHECK 		= 0xBD,	// +
	GB_COM_DEF_GET_JRN_CNT 		= 0xC1,	// +
	GB_COM_DEF_GET_JRN_ENTRY 	= 0xC2,	// +
	GB_COM_DEF_JRN_CLR 			= 0xCA,	// ! �������� ������� ���, ������ � ��
	GB_COM_PRM_GET_JRN_CNT 		= 0xD1,	// +
	GB_COM_PRM_GET_JRN_ENTRY 	= 0xD2,	// +
	GB_COM_PRM_JRN_CLR 			= 0xDA,	// ! �������� ������� ���, ������ � ��
	GB_COM_PRD_GET_JRN_CNT 		= 0xE1,	// +
	GB_COM_PRD_GET_JRN_ENTRY 	= 0xE2,	// +
	GB_COM_PRD_JRN_CLR 			= 0xEA,	// ! �������� ������� ���, ������ � ��
	GB_COM_GET_JRN_CNT 			= 0xF1,	// +
	GB_COM_GET_JRN_ENTRY 		= 0xF2,	// +
	GB_COM_JRN_CLR 				= 0xFA	// ! �������� ������� �������, ������ � ��
};

/// ����� ������
enum eGB_COM_MASK {
	// ��� ����������
	GB_COM_MASK_DEVICE = 0x30,
	GB_COM_MASK_DEVICE_DEF = 0x00,
	GB_COM_MASK_DEVICE_PRM = 0x10,
	GB_COM_MASK_DEVICE_PRD = 0x20,
	GB_COM_MASK_DEVICE_GLB = 0x30,

	// ������ �������
	GB_COM_MASK_GROUP = 0xC0,
	GB_COM_MASK_GROUP_READ_PARAM = 0x00,
	GB_COM_MASK_GROUP_WRITE_REGIME = 0x40,
	GB_COM_MASK_GROUP_WRITE_PARAM = 0x80,
	GB_COM_MASK_GROUP_READ_JOURNAL = 0xC0
};

/// ������������������ ���������� � ������� ������� ����� (0�38)
enum posComNetAdr_t {
    POS_COM_NET_ADR_netAdr = 1,
    POS_COM_NET_ADR_protocol,
    POS_COM_NET_ADR_baudrate,
    POS_COM_NET_ADR_dataBits,
    POS_COM_NET_ADR_parity,
    POS_COM_NET_ADR_stopBits,
    POS_COM_NET_ADR_password,
    POS_COM_NET_ADR_vpSac2 = POS_COM_NET_ADR_password + 2,
    POS_COM_NET_ADR_vpSam
};

enum posComPrdKeep_t {
    POS_COM_PRD_KEEP_prdKeep = 1,
    POS_COM_PRD_KEEP_compK400,
    POS_COM_PRD_KEEP_tmK400,
    POS_COM_PRD_KEEP_warnD,
    POS_COM_PRD_KEEP_alarmD,
    POS_COM_PRD_KEEP_tempMonitor,
    POS_COM_PRD_KEEP_tempThrHi,
    POS_COM_PRD_KEEP_tempThrLow,
    POS_COM_PRD_KEEP_tmSpeed,
    POS_COM_PRD_KEEP_ringTimeWait,
    POS_COM_PRD_KEEP_ringComTransit,
    POS_COM_PRD_KEEP_alarmResetMode = POS_COM_PRD_KEEP_ringComTransit + (MAX_NUM_COM_RING / 8)
};

enum posComPrmBlockAll_t {
    POS_COM_PRM_BLOCK_ALL_vpSac1 = 1
};

/// ��������� (������� � fParams)
typedef enum {
	GB_PARAM_NULL_PARAM = 0,	///< �������� ��������
	// ����� ���������
	GB_PARAM_TIME_SYNCH,		///< ������������� �����
	GB_PARAM_NUM_OF_DEVICE,		///< ����� ��������
	GB_PARAM_NUM_OF_DEVICE_RING,///< ����� �������� � ���������� ������
	GB_PARAM_OUT_CHECK,			///< �������� ��������� �������
	GB_PARAM_WARN_THD, 			///< ����� ��������������
	GB_PARAM_WARN_THD_CF,		///< ����� �������������� �� �� (��� ����)
	GB_PARAM_TIME_RERUN,		///< ����� �����������
	GB_PARAM_COM_PRD_KEEP,		///< ��������� ���� ������ ���
	GB_PARAM_COM_PRM_KEEP,		///< ��������� ���� ������ ���
	GB_PARAM_IN_DEC,			///< ����������� ���������������� ���
	GB_PARAM_NET_ADDRESS,		///< ����� � ��������� ����
	GB_PARAM_U_OUT_NOM,			///< ������������ �������� ����������
	GB_PARAM_FREQ,				///< �������
	GB_PARAM_COMP_P400,			///< ������������� (�400, �400�)
	GB_PARAM_IN_DEC_AC_ANSWER,	///< �������� ������ �� (����)
	GB_PARAM_DETECTOR,			///< ��� ���������
	GB_PARAM_COR_U,				///< ��������� ����������
	GB_PARAM_COR_I,				///< ��������� ����
	GB_PARAM_PVZUE_PROTOCOL,	///< �������� ������ (����-�)
	GB_PARAM_PVZUE_PARITY,		///< ������� �������� (����-�)
	GB_PARAM_PVZUE_FAIL,		///< ���������� ������� (����-�)
	GB_PARAM_PVZUE_NOISE_THD,	///< ����� �� ������ (����-�)
	GB_PARAM_PVZUE_NOISE_LVL,	///< ���������� ������ (����-�)
	GB_PARAM_PVZUE_AC_TYPE,		///< ��� ������������ (����-�)
	GB_PARAM_PVZUE_AC_PERIOD,	///< ������ ������� ������ �� (����-�)
	GB_PARAM_PVZUE_AC_PER_RE,	///< ������ ������� ������� ������ �� (����-�)
	GB_PARAM_BACKUP,			///< ��������������
	GB_PARAM_COMP_K400,			///< ������������� �400
	GB_PARAM_NUM_OF_DEVICES,	///< ��� ����� (���-�� ��������� � �����)
	GB_PARAM_TM_K400,			///< ������������
	GB_PARAM_WARN_D,			///< ������� ������������ ����������������� ������������ �� D
	GB_PARAM_ALARM_D,			///< ������� ������������ ��������� ������������ �� D
	GB_PARAM_TEMP_MONITOR,		///< �������� �����������
	GB_PARAM_TEMP_THR_HI,		///< ������� �������� �����������
	GB_PARAM_TEMP_THR_LOW,		///< ������ �������� �����������
	GB_PARAM_TM_SPEED,			///< �������� ��
    GB_PARAM_ALARM_RESET_MODE,  ///< ����� ������ ������������
	// ��������� ������
	GB_PARAM_DEF_TYPE,			///< ��� ������
	GB_PARAM_TIME_NO_MAN,		///< �������������� ����� ��� �����������
	GB_PARAM_OVERLAP,			///< ���������� ��������� (����� �400/�400�)
	GB_PARAM_OVERLAP_P400,		///< ���������� ��������� (�400/�400�)
	GB_PARAM_OVERLAP_OPTO,		///< ���������� ��������� (�400 ������)
	GB_PARAM_DELAY,				///< ����������� ��������
	GB_PARAM_DELAY_OPTO,		///< ����������� ��������
	GB_PARAM_WARN_THD_RZ,		///< ����� �������������� �� �� (��� ����)
	GB_PARAM_SENS_DEC,			///< ����������� ����������������
	GB_PARAM_SENS_DEC_RZ,		///< ����������� ���������������� �� ��
	GB_PARAM_PRM_TYPE,			///< ��� ���������
	GB_PARAM_AC_IN_DEC,			///< �������� ������ ��
	GB_PARAM_FREQ_PRD,			///< ������� ���
	GB_PARAM_FREQ_PRM,			///< ������� ���
	GB_PARAM_SHIFT_FRONT,		///< ����� ��������� ������ ���
	GB_PARAM_SHIFT_BACK,		///< ����� ������� ������ ���
	GB_PARAM_SHIFT_PRM,			///< ����� ���
	GB_PARAM_SHIFT_PRD,			///< ����� �� ��� �� ����
	GB_PARAM_LIMIT_PRD,			///< ����������� ������ �����������
	GB_PARAM_DELAY_OFF_PRM,		///< �������� ���������� ���
	GB_PARAM_DELAY_ON_PRM,		///< �������� ��������� ���
	GB_PARAM_DELAY_ON_PRD,		///< �������� ��������� ���
	GB_PARAM_MIN_TIME_PRD,		///< ����������� ������������ ���
	// ��������� �����������
	GB_PARAM_PRD_IN_DELAY,		///< ����� ��������� (�������� ������������ ����������� �����)
	GB_PARAM_PRD_DURATION_L,	///< ������������ ������� ��
	GB_PARAM_PRD_DURATION_O,	///< ������������ ������� ������
	GB_PARAM_PRD_TEST_COM,		///< �������� �������
	GB_PARAM_PRD_COM_LONG,		///< �������� �������
	GB_PARAM_PRD_COM_BLOCK,		///< ������������� �������
	GB_PARAM_PRD_DR_ENABLE,		///< ���������� ��
	GB_PARAM_PRD_DR_COM_BLOCK,	///< ������������� ������� ��
	GB_PARAM_PRD_COM_NUMS,		///< ���������� ������
	GB_PARAM_PRD_COM_NUMS_A,	///< ���������� ������ ������ �
	GB_PARAM_PRD_FREQ_CORR,		///< ��������� ������� ���
	GB_PARAM_PRD_DEC_CF,		///< �������� ������ ��
	GB_PARAM_PRD_DEC_TM,		///< �������� ������ ��
	GB_PARAM_PRD_DEFAULT_CF,	///< �� �� ���������
	GB_PARAM_PRD_COM_SIGNAL,	///< ������������ ������ ���
	// ��������� ���������
	GB_PARAM_PRM_TIME_ON,		///< �������� �� �������� ������� (����� ���������)
	GB_PARAM_PRM_TIME_ON_K400,	///< �������� �� �������� ������� (����� ���������) � �400
    GB_PARAM_PRM_COM_BLOCK,		///< ������������� �������
	GB_PARAM_PRM_TIME_OFF,		///< �������� �� ����������
	GB_PARAM_PRM_DR_ENABLE,		///< ���������� ��
	GB_PARAM_PRM_DR_COM_BLOCK,	///< ������������� ������� ��
	GB_PARAM_PRM_DR_COM_TO_HF,	///< ������� �� � ��
	GB_PARAM_PRM_COM_NUMS,		///< ���������� ������
	GB_PARAM_PRM_TEST_COM,		///< ����� �������� �������
	GB_PARAM_PRM_FREQ_CORR,		///< ��������� ������� ���
	GB_PARAM_PRM_COM_SIGNAL,	///< ������������ ������ ���
	GB_PARAM_PRM_INC_SAFETY,	///< ��������� ������������
	// ��������� ����������
	GB_PARAM_INTF_PROTOCOL,		///< ��������
	GB_PARAM_INTF_BAUDRATE,		///< �������� ��������
	GB_PARAM_INTF_DATA_BITS,	///< ���� ������
	GB_PARAM_INTF_PARITY,		///< ��������
	GB_PARAM_INTF_STOP_BITS,	///< �������� ����
    GB_PARAM_INTF_INTERFACE,	///< ���������
	// ��������� ������
	GB_PARAM_RING_TIME_WAIT,	///< ����� �������� ������
	GB_PARAM_RING_COM_TRANSIT,	///< ���������� �������
	GB_PARAM_RING_COM_REC,		///< �������������� ������ ���������
    GB_PARAM_RING_COM_TR,		///< �������������� ������ ���������� ������
    // ����������� ������
    GB_PARAM_VP_SAC1,           ///< ����� ��� (SAC1)
    GB_PARAM_VP_SAC2,           ///< ���������� (SAC2)
    GB_PARAM_VP_SAm,            ///< ���������� ������������ ������ ������ (SAnn.x / SA.m)
    // ������
    GB_PARAM_USER_PASSWORD,     ///< ������ ������������.
    //
    GB_PARAM_MAX
} eGB_PARAM;

/// �������� ������ ����������
enum eGB_CONTROL {
	GB_CONTROL_RESET_SELF 	= 1,	// ����� ������
	GB_CONTROL_RESET_UD 	= 2,	// ����� ���������� (-��)
	GB_CONTROL_RESET_AC		= 3,	// ����� ��
	GB_CONTROL_PUSK_UD_1 	= 4,	// ���� ���������� (1)
	GB_CONTROL_PUSK_UD_2 	= 5,	// ���� ���������� 2
	GB_CONTROL_PUSK_UD_ALL 	= 6,	// ���� ���������
	GB_CONTROL_CALL 		= 7,	// �����
	GB_CONTROL_PUSK_ON 		= 8,	// ���� ���������� ���.
	GB_CONTROL_PUSK_OFF 	= 9,	// ���� ���������� ����.
	GB_CONTROL_PUSK_AC_UD 	= 10,	// ���� �� ���������
	GB_CONTROL_RESET_UD_1	= 11,	// ����� ���������� 1
	GB_CONTROL_RESET_UD_2	= 12,	// ����� ���������� 2
	GB_CONTROL_PUSK_UD_3 	= 13,	// ���� ���������� 3
	GB_CONTROL_REG_AC 		= 14,	// ����� ��
	GB_CONTROL_RESET_UD_3	= 15,	// ����� ���������� 3
	GB_CONTROL_MAN_1 		= 16,	// ���� ��� ���������� (1)
	GB_CONTROL_MAN_2 		= 17,	// ���� ��� ���������� 2
	GB_CONTROL_MAN_3 		= 18,	// ���� ��� ���������� 3
	GB_CONTROL_MAN_ALL 		= 19,	// ���� ��� ���������
	GB_CONTROL_MAX					//
};

/// ������� ������� �����������/��������� - ����� � ������ �������
enum eGB_STATE_COM {
	GB_STATE_COM_MIN 	= 0,		///< ����������� ��������
	GB_STATE_COM_END 	= 0,		///< ��������� �������
	GB_STATE_COM_START,				///< ������ �������
	GB_STATE_COM_MAX				///< ������������ ��������
};

/// �������� ������������ �������
enum eGB_SOURCE_COM {
	GB_SOURCE_COM_DI 	= 0, 		///< ������� ��������� � ����������� �����
	GB_SOURCE_COM_DR	= 1,		///< ������� ��������� � ��������� �����
	GB_SOURCE_COM_MAX				///< ��������� ��������
};

/// ������� � ����1 � ����2
enum eGB_TEST_SIGNAL {
	GB_SIGNAL_OFF = 0, 				//
	GB_SIGNAL_CF,					// ���� / �400�
	GB_SIGNAL_CF1,
	GB_SIGNAL_CF2,
	GB_SIGNAL_CF3,
	GB_SIGNAL_CF4,
	GB_SIGNAL_CF_NO_RZ,				// ����
	GB_SIGNAL_CF_RZ,				// ����
	GB_SIGNAL_CF2_NO_RZ,			// ����
	GB_SIGNAL_CF2_RZ,				// ����
	GB_SIGNAL_RZ,
	GB_SIGNAL_COM1_NO_RZ,			// ����
	GB_SIGNAL_COM2_NO_RZ,			// ����
	GB_SIGNAL_COM3_NO_RZ,			// ����
	GB_SIGNAL_COM4_NO_RZ,			// ����
	GB_SIGNAL_COM1_RZ,				// ����
	GB_SIGNAL_COM2_RZ,				// ����
	GB_SIGNAL_COM3_RZ,				// ����
	GB_SIGNAL_COM4_RZ,				// ����
	GB_SIGNAL_COM1,					// vvvvvvvvvvvvvvvvvvvvvvvvvv
	GB_SIGNAL_COM2,					// ������� ������ ������ ����
	GB_SIGNAL_COM3,					// ������ ��� ���������� �400
	GB_SIGNAL_COM4,
	GB_SIGNAL_COM5,
	GB_SIGNAL_COM6,
	GB_SIGNAL_COM7,
	GB_SIGNAL_COM8,
	GB_SIGNAL_COM9,
	GB_SIGNAL_COM10,
	GB_SIGNAL_COM11,
	GB_SIGNAL_COM12,
	GB_SIGNAL_COM13,
	GB_SIGNAL_COM14,
	GB_SIGNAL_COM15,
	GB_SIGNAL_COM16,
	GB_SIGNAL_COM17,
	GB_SIGNAL_COM18,
	GB_SIGNAL_COM19,
	GB_SIGNAL_COM20,
	GB_SIGNAL_COM21,
	GB_SIGNAL_COM22,
	GB_SIGNAL_COM23,
	GB_SIGNAL_COM24,
	GB_SIGNAL_COM25,
	GB_SIGNAL_COM26,
	GB_SIGNAL_COM27,
	GB_SIGNAL_COM28,
	GB_SIGNAL_COM29,
	GB_SIGNAL_COM30,				// ������� ������ ������ ����
	GB_SIGNAL_COM31,				// ������ ��� ���������� �400
	GB_SIGNAL_COM32,				// ^^^^^^^^^^^^^^^^^^^^^^^^^^
	GB_SIGNAL_CF_RZ_R400M,
	GB_SIGNAL_CF1_RZ_R400M,
	GB_SIGNAL_CF2_RZ_R400M,
	GB_SIGNAL_CF3_RZ_R400M,
	GB_SIGNAL_CF4_RZ_R400M,
	GB_SIGNAL_CS,					// � ������ ������ ��
	GB_SIGNAL_COM1A,				// ������ ���������������� vvv
	GB_SIGNAL_COM2A,
	GB_SIGNAL_COM3A,
	GB_SIGNAL_COM4A,
	GB_SIGNAL_COM5A,
	GB_SIGNAL_COM6A,
	GB_SIGNAL_COM7A,
	GB_SIGNAL_COM8A,
	GB_SIGNAL_COM9A,
	GB_SIGNAL_COM10A,
	GB_SIGNAL_COM11A,
	GB_SIGNAL_COM12A,
	GB_SIGNAL_COM13A,
	GB_SIGNAL_COM14A,
	GB_SIGNAL_COM15A,
	GB_SIGNAL_COM16A,
	GB_SIGNAL_COM17A,
	GB_SIGNAL_COM18A,
	GB_SIGNAL_COM19A,
	GB_SIGNAL_COM20A,
	GB_SIGNAL_COM21A,
	GB_SIGNAL_COM22A,
	GB_SIGNAL_COM23A,
	GB_SIGNAL_COM24A,
	GB_SIGNAL_COM25A,
	GB_SIGNAL_COM26A,
	GB_SIGNAL_COM27A,
	GB_SIGNAL_COM28A,
	GB_SIGNAL_COM29A,
	GB_SIGNAL_COM30A,
	GB_SIGNAL_COM31A,
	GB_SIGNAL_COM32A,
	GB_SIGNAL_COM1B,
	GB_SIGNAL_COM2B,
	GB_SIGNAL_COM3B,
	GB_SIGNAL_COM4B,
	GB_SIGNAL_COM5B,
	GB_SIGNAL_COM6B,
	GB_SIGNAL_COM7B,
	GB_SIGNAL_COM8B,
	GB_SIGNAL_COM9B,
	GB_SIGNAL_COM10B,
	GB_SIGNAL_COM11B,
	GB_SIGNAL_COM12B,
	GB_SIGNAL_COM13B,
	GB_SIGNAL_COM14B,
	GB_SIGNAL_COM15B,
	GB_SIGNAL_COM16B,
	GB_SIGNAL_COM17B,
	GB_SIGNAL_COM18B,
	GB_SIGNAL_COM19B,
	GB_SIGNAL_COM20B,
	GB_SIGNAL_COM21B,
	GB_SIGNAL_COM22B,
	GB_SIGNAL_COM23B,
	GB_SIGNAL_COM24B,
	GB_SIGNAL_COM25B,
	GB_SIGNAL_COM26B,
	GB_SIGNAL_COM27B,
	GB_SIGNAL_COM28B,
	GB_SIGNAL_COM29B,
	GB_SIGNAL_COM30B,
	GB_SIGNAL_COM31B,
	GB_SIGNAL_COM32B,
	GB_SIGNAL_COM1C,
	GB_SIGNAL_COM2C,
	GB_SIGNAL_COM3C,
	GB_SIGNAL_COM4C,
	GB_SIGNAL_COM5C,
	GB_SIGNAL_COM6C,
	GB_SIGNAL_COM7C,
	GB_SIGNAL_COM8C,
	GB_SIGNAL_COM9C,
	GB_SIGNAL_COM10C,
	GB_SIGNAL_COM11C,
	GB_SIGNAL_COM12C,
	GB_SIGNAL_COM13C,
	GB_SIGNAL_COM14C,
	GB_SIGNAL_COM15C,
	GB_SIGNAL_COM16C,
	GB_SIGNAL_COM17C,
	GB_SIGNAL_COM18C,
	GB_SIGNAL_COM19C,
	GB_SIGNAL_COM20C,
	GB_SIGNAL_COM21C,
	GB_SIGNAL_COM22C,
	GB_SIGNAL_COM23C,
	GB_SIGNAL_COM24C,
	GB_SIGNAL_COM25C,
	GB_SIGNAL_COM26C,
	GB_SIGNAL_COM27C,
	GB_SIGNAL_COM28C,
	GB_SIGNAL_COM29C,
	GB_SIGNAL_COM30C,
	GB_SIGNAL_COM31C,
	GB_SIGNAL_COM32C,	// ������ ���������������� ^^^
	GB_SIGNAL_MAX
};



/// ��������� ��������� ��������� �������� ���������.
enum eGB_ACT {
	GB_ACT_NO 		= 0,		// �������� �� ���������
	GB_ACT_OLD		= 0x01,		// ����� �������� ��������� � ����������
	GB_ACT_NEW		= 0x02,		// ����������� ����� ��������
	GB_ACT_ERROR	= 0x04		// ��������� ��������
};


/// ���������� ������� � �400
enum eGB_K400_NUM_COM {
	GB_K400_NUM_COM_MIN = 0,	///< ����������� ��������
	GB_K400_NUM_COM_0	= 0,	///<
	GB_K400_NUM_COM_4	= 1,	///<
	GB_K400_NUM_COM_8	= 2,	///<
	GB_K400_NUM_COM_12	= 3,	///<
	GB_K400_NUM_COM_16	= 4,	///<
	GB_K400_NUM_COM_24	= 5,	///<
	GB_K400_NUM_COM_32	= 6,	///<
	GB_K400_NUM_COM_MAX			///< ���-�� ������� � ������
};

/// ����� ��� ������
class TPassword {

public:
	TPassword() {
        password_ = 10000;
		admin_ = PASSWORD_ADMIN;
	}

	// ���������� ������� ������ ������������
	uint16_t get() const {
		return password_;
	}

	// ������������� ������ ������������
	bool set(uint16_t pas) {
		bool stat = false;
        if (pas < 10000) {
			password_ = pas;
			stat = true;
		}
		return stat;
	}

	// ��������� ������������� ������ ������������
	// ���� ������� �������� ��������, ������ ����� PASSWORD_USER
	void init(uint16_t pas) {
		if (!set(pas))
			set(PASSWORD_USER);
	}

	// �������� �������� �� ���������� � ������� ������������ � ��������������
	bool check(uint16_t pas) const {
		return ((pas == password_) || (pas == admin_));
	}

private:
	// ������ ������������
	uint16_t password_;

	// ������ ��������������
	uint16_t admin_;
};

/// ����� ��� ���������� ����������
class TMeasuredParameters {
public:
	TMeasuredParameters() {
		voltDef_ = 0;
		voltDef2_ = 0;
		voltCf_ = 0;
		voltCf2_ = 0;
		voltNoise_ = 0;
		voltNoise2_ = 0;
		voltOut_ = 0;
		curOut_ = 0;
		resistOut_ = 0;
		pulseWidth_ = 0;
		d_ = 0;
		temperature_ = -100;
		freqDev_ = 0;
	}

	// ����� �� ������
	int8_t getVoltageDef() const {
		return voltDef_;
	}
	bool setVoltageDef(int8_t voltDef) {
		bool stat = false;
		if ((voltDef > -100) && (voltDef < 100)) {
			voltDef_ = voltDef;
			stat = true;
		}
		return stat;
	}

	// ����� �� ������ ������
	int8_t getVoltageDef2() const {
		return voltDef2_;
	}
	bool setVoltageDef2(int8_t voltDef) {
		bool stat = false;
		if ((voltDef > -100) && (voltDef < 100)) {
			voltDef2_ = voltDef;
			stat = true;
		}
		return stat;
	}

	// ����� �� ��
	int8_t getVoltageCf() const {
		return voltCf_;
	}
	bool setVoltageCf(int8_t voltCf) {
		bool stat = false;
		if ((voltCf > -100) && (voltCf < 100)) {
			voltCf_ = voltCf;
			stat = true;
		}
		return stat;
	}

	// ����� �� �� ������
	int8_t getVoltageCf2() const {
		return voltCf2_;
	}
	bool setVoltageCf2(int8_t voltCf) {
		bool stat = false;
		if ((voltCf > -100) && (voltCf < 100)) {
			voltCf2_ = voltCf;
			stat = true;
		}
		return stat;
	}

	// ���������� ������
	uint8_t getVoltageOutInt() const {
		return (voltOut_ / 10);
	}
	uint8_t getVoltageOutFract() const {
		return (voltOut_ % 10);
	}
	uint16_t getVoltageOut() const {
		return voltOut_;
	}
	bool setVoltageOut(uint8_t voltOutInt, uint8_t voltOutFract) {
		bool stat = false;
		if ((voltOutInt < 100) && (voltOutFract < 10)) {
			voltOut_ = (((uint16_t) voltOutInt) * 10) + voltOutFract;
			stat = true;
		}
		return stat;
	}

	// ��� ������
	uint16_t getCurrentOut() const {
		return curOut_;
	}
	bool setCurrentOut(uint16_t curOut) {
		bool stat = false;
		if (curOut <= 999) {
			curOut_ = curOut;
			stat = true;
		}
		return stat;
	}

	// ������������� ������
	uint16_t getResistOut() const {
		return resistOut_;
	}
	bool setResistOut(uint16_t resistOut) {
		bool stat = false;
		if (resistOut <= 999) {
			resistOut_ = resistOut;
			stat = true;
		}
		return stat;
	}

	// ������� ����
	int8_t getVoltageNoise() const {
		return voltNoise_;
	}
	bool setVoltageNoise(int8_t voltNoise) {
		bool stat = false;
		if ((voltNoise > -100) && (voltNoise < 100)) {
			voltNoise_ = voltNoise;
			stat = true;
		}
		return stat;
	}

	// ������� ���� 2
	int8_t getVoltageNoise2() const {
		return voltNoise2_;
	}
	bool setVoltageNoise2(int8_t val) {
		bool stat = false;
		if ((val > -100) && (val < 100)) {
			voltNoise2_ = val;
			stat = true;
		}
		return stat;
	}

	// ������������ ��������� �� ���������� �� ������ ���������
	uint16_t getPulseWidth() const {
		return pulseWidth_;
	}
	bool setPulseWidth(uint16_t val) {
		bool stat = false;
		if (val <= 360) {
			pulseWidth_ = val;
			stat = true;
		}
		return stat;
	}

	// ����� �� ����.������� (��������) ��� ��������� ������/������ (��������)
	int8_t getD() const {
		return d_;
	}
	bool setD(int8_t val) {
		bool stat = false;
		if ((val >= -64) && (val <= 64)) {
			d_ = val;
			stat = true;
		}
		return stat;
	}


	int8_t getTemperature() const {
		return temperature_;
	}
	bool setTemperature(int8_t val){
		bool stat = false;
		if ((val >= -100) && (val <= 125)) {
			temperature_ = val;
			stat = true;
		}
		return stat;
	}

	// ���������� ������� �� �� ���
	int8_t getFreqDev() const {
		return freqDev_;
	}
	bool setFreqDev(int8_t val) {
		bool stat = false;
		if ((val >= -100) && (val <= 100)) {
			freqDev_ = val;
			stat = true;
		}
		return stat;
	}


private:
	// ����� �� ������ (-99 .. 99)��
	int8_t voltDef_;
	// ����� �� ������ ������� ������ (-99..99)��
	int8_t voltDef2_;
	// ����� �� �� (-99 .. 99)��
	int8_t voltCf_;
	// ����� �� �� ������� ������ (-99..99)��
	int8_t voltCf2_;
	// ������� ����� (-99 .. 99)��
	int8_t voltNoise_;
	// ������� ����� 2 (-99 .. 99)��
	int8_t voltNoise2_;
	// �������� ���������� ���������� �� 10 (0 .. 999)�
	uint16_t voltOut_;
	// �������� ��� (0 .. 999)��
	uint16_t curOut_;
	// �������� ������������� (0 .. 999)��
	uint16_t resistOut_;
	// ������������ ��������� �� ���������� �� ������ ��������� (0..360)�
	uint16_t pulseWidth_;
	// ����� �� ����.������� (��������) ��� ��������� ������/������ (��������) [-64..64]��
	int8_t d_;
	// ����������� (-100..125), -100 - ������
	int8_t temperature_;
	// ���������� ������� �� �� ��� (-100..100)��
	int8_t freqDev_;
};

//
class TTest {
public:
	TTest() {
		clear();
	}
	// ������� ������ ��������
	void clear() {
		for(uint_fast8_t i = 0; i < MAX_NUM_TEST_SIGNAL; i++)
			signalList[i] = GB_SIGNAL_OFF;
		num_ = 1;
		currentSignal_ = GB_SIGNAL_OFF;
		currentSignal2_ = GB_SIGNAL_OFF;
	}

	bool addSignalToList(eGB_TEST_SIGNAL signal) {
		bool stat = false;
		if ((num_ < MAX_NUM_TEST_SIGNAL) && (signal < GB_SIGNAL_MAX)) {
			signalList[num_++] = signal;
			stat = true;
		}
		return stat;
	}

	/**	�������������� ��������� ������� � �������� ��� �������� � ���.
	 * 	@param [out] cf	��� ������� ��/�������.
	 * 	@param [out] rz	��� ������� ��.
	 *	@param sig �������� ������.
	 *
	 */
	void getBytes(uint8_t &cf, uint8_t &rz, eGB_TEST_SIGNAL sig) {
		if ((sig >= GB_SIGNAL_COM1) && (sig <= GB_SIGNAL_COM32)) {
			rz = 0;
			cf = 3 + sig - GB_SIGNAL_COM1; // 3 - ���-�� �� ?!
		} else if ((sig >= GB_SIGNAL_CF1) && (sig <= GB_SIGNAL_CF4)) {
			rz = 0;
			cf = 1 + sig - GB_SIGNAL_CF1;
		} else if (sig == GB_SIGNAL_CF_NO_RZ) {
			rz = 1;
			cf = 1;
		} else if (sig == GB_SIGNAL_CF_RZ) {
			rz = 2;
			cf = 1;
		} else if (sig == GB_SIGNAL_CF2_NO_RZ) {
			rz = 1;
			cf = 2;
		} else if (sig == GB_SIGNAL_CF2_RZ) {
			rz = 2;
			cf = 2;
		} else if (sig == GB_SIGNAL_RZ) {
			rz = 1;
			cf = 0;
		} else if ((sig >= GB_SIGNAL_COM1_RZ) && (sig <= GB_SIGNAL_COM4_RZ)) {
			rz = 2;
			cf = 3 + sig - GB_SIGNAL_COM1_RZ; // 3 - ���-�� �� ?!
		} else if ((sig>=GB_SIGNAL_COM1_NO_RZ)&&(sig<=GB_SIGNAL_COM4_NO_RZ)) {
			rz = 1;
			cf = 3 + sig - GB_SIGNAL_COM1_NO_RZ; // 3 - ���-�� �� ?!
		} else if ((sig == GB_SIGNAL_CF) || (sig == GB_SIGNAL_CS)) {
			rz = 0;
			cf = 1;
		} else if ((sig >= GB_SIGNAL_COM1A) && (sig <= GB_SIGNAL_COM32C)) {
			rz = 0;
			cf = 3 + sig - GB_SIGNAL_COM1A;
		} else {
			rz = 0;
			cf = 0;
		}
	}

	/** ���������� ������������ ���-�� �������� � �����.
	 *	@return ������������ ���-�� �������� � �����.
	 */
	uint8_t getNumSignals() const {
		return num_;
	}

	/** ����������� �������� ������� ������������� / ������������� � ������.
	 *
	 * 	� ����� ��������� ������ ����������� 6 ���� ������.
	 * 	� ����� �����������: � ����/�400� 3 �����, � �400/������ 5 ����.
	 *
	 * 	� 3-� �������� ��������, ���-�� ���� ������ �����������.
	 *
	 * 	@param s ��������� �� ������ ������� ������.
	 * 	@param type ��� ��������.
	 * 	@param numBytes ������������ ���������� ������.
	 */
	void setCurrentSignal(uint8_t *s, eGB_TYPE_DEVICE type, eGB_TYPE_OPTO opto) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_MAX;
		eGB_TEST_SIGNAL signal2 = GB_SIGNAL_MAX;

		if (type == AVANT_K400) {
			signal = getCurrentSignalK400(s);
			signal2 = getCurrentSignalK400((s + 5));
		} else if (type == AVANT_RZSK) {
			signal = getCurrentSignalRZSK(s);
			signal2 = getCurrentSignalRZSK((s + 3));
		} else if (type == AVANT_R400M) {
			signal = getCurrentSignalR400M(s);
		} else if (type == AVANT_OPTO) {
			if (opto == TYPE_OPTO_RING_UNI) {
				signal = getCurrentSignalOptoRingUni(s);
			} else {
				signal = getCurrentSignalOpto(s);
			}
		}
		currentSignal_ = signal;
		currentSignal2_ = signal2;
	}

	/** ���������� ������� ������ � ����� ��� ������� ���������.
	 * 	@return ������� ������ � �����.
	 */
	eGB_TEST_SIGNAL getCurrentSignal() const {
		return currentSignal_;
	}

	/** ���������� ������� ������ � ����� ��� ������� ���������.
	 * 	@return ������� ������ � �����.
	 */
	eGB_TEST_SIGNAL getCurrentSignal2() const {
		return currentSignal2_;
	}

	// ������ ��������
	uint8_t signalList[MAX_NUM_TEST_SIGNAL];

private:
	// ���-�� ��������� �������� � �����
	uint8_t num_;

	// ������� ������
	eGB_TEST_SIGNAL currentSignal_;

	// ������� ������ ������� ���������
	eGB_TEST_SIGNAL currentSignal2_;

	// ���������� ����� ������� �������������� ���� 1..8, ���� 0
	// �������� ���������� � 0-��� ����
	uint8_t getSetBit(uint8_t byte) {
		uint8_t bit = 0;

		if (byte) {
			uint8_t b = 0;
			for (uint_fast8_t i = 1; i > 0; i <<= 1) {
				b++;
				if (byte & i) {
					bit = b;
					break;
				}
			}
		}
		return bit;
	}

	/** ���������� ������� � ������ ��� �400.
	 * 	���: 7		6		5		4		3		2		1		0		;
	 * 	b1 : x 		x 		x 		x 		x		x 		[��2]	[��1]	;
	 * 	b2 : [���8] [���7]	[���6] 	[���5] 	[���4] 	[���3] 	[���2] 	[���1]	;
	 * 	b3 : [���16][���15] [���14] [���13] [���12] [���11] [���10] [���9]	;
	 * 	b4 : [���24][���23] [���22] [���21] [���20] [���19] [���18] [���17]	;
	 * 	b5 : [���32][���31] [���30] [���29] [���28] [���27] [���26] [���25]	;
	 * 	������������� ��� �������� ������� ������� ������� �� ��������.
	 * 	����� ������� �� ������� �������������� ����.
	 * 	@param *s ��������� �� ������ ������.
	 * 	@return ������� �������� ������.
	 */
	eGB_TEST_SIGNAL getCurrentSignalK400(uint8_t *s) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

		uint8_t t = *s;
		// ������� ����������� ������� ��1-��2
		if (t & 0x01)
			signal = GB_SIGNAL_CF1;
		else if (t & 0x02)
			signal = GB_SIGNAL_CF2;
		else {
			// ����������� ������� ������ � 1 �� 8
			t = getSetBit(*(++s));
			if (t) {
				t = GB_SIGNAL_COM1 + t - 1;
				signal = (eGB_TEST_SIGNAL) t;
			} else {
				// ����������� ������� ������ � 9 �� 16
				t = getSetBit(*(++s));
				if (t) {
					t = GB_SIGNAL_COM9 + t - 1;
					signal = (eGB_TEST_SIGNAL) t;
				} else {
					// ����������� ������� ������ � 17 �� 24
					t = getSetBit(*(++s));
					if (t) {
						t = GB_SIGNAL_COM17 + t - 1;
						signal = (eGB_TEST_SIGNAL) t;
					} else {
						// ����������� ������� ������ � 25 �� 32
						t = getSetBit(*(++s));
						if (t) {
							t = GB_SIGNAL_COM25 + t - 1;
							signal = (eGB_TEST_SIGNAL) t;
						}
					}
				}
			}
		}
		return signal;
	}

	/** ���������� ������� � ������ ��� ����.
	 * 	���: 7	6	5	4	3		2		1		0		;
	 * 	b1 : 0 	0 	0	0	[��2] 	[��1] 	[��2] 	[��1]	;
	 * 	b2 : 0	0	0 	0 	[���4] 	[���3] 	[���2] 	[���1]	;
	 * 	������������� ��� �������� ������� ������� ������� �� ��������.
	 * 	���� ���� ������ ��, �� ��� �� ������ �� ��. ������� ��� ��1 ����!
	 * 	��1 + ���N = ������� ��� ����;
	 * 	��2 + ���N = ������� � ����;
	 * 	��1 + ��2  = ��2 ��� ����;
	 * 	��2 + ��2  = ��2 � ����;
	 * 	��1 + ��1  = �� ��� ����;
	 * 	��2 + ��1  = �� � ����.
	 * 	@param *s ��������� �� ������ ������.
	 * 	@return ������� �������� ������.
	 */
	eGB_TEST_SIGNAL getCurrentSignalRZSK(uint8_t *s) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

		uint8_t b1 = (*s) & 0x0F;
		uint8_t b2 = (*(++s)) & 0x0F;

		if (b2) {
			b2 = getSetBit(b2) - 1;
			if (b1 & 0x04) {
				signal = (eGB_TEST_SIGNAL) (b2 + GB_SIGNAL_COM1_NO_RZ);
			} else if (b1 & 0x08) {
				signal = (eGB_TEST_SIGNAL) (b2 + GB_SIGNAL_COM1_RZ);
			}
		} else if (b1) {
			// ���� ���� ��, �� ���� � ��
			if ((b1 & 0x06) == 0x06) {
				signal = GB_SIGNAL_CF2_NO_RZ;
			} else if ((b1 & 0x0A) == 0x0A) {
				signal = GB_SIGNAL_CF2_RZ;
			} else if ((b1 & 0x05) == 0x05) {
				signal = GB_SIGNAL_CF_NO_RZ;
			} else if ((b1 & 0x09) == 0x09)  {
				signal = GB_SIGNAL_CF_RZ;
			}
		}

		return signal;
	}

	/** ���������� ������� � ������ ��� �400�.
	 * 	���: 7		6		5		4		3		2		1		0		;
	 * 	b1 : x 		x 		x 		[��]	[��4]	[��3]	[��2]	[��1]	;
	 * 	������������� ��� �������� ������� ������� ������� �� ��������.
	 * 	������������ ����� �������������� ��� �������.
	 * 	@param *s ��������� �� ������ ������.
	 * 	@return ������� �������� ������.
	 */
	eGB_TEST_SIGNAL getCurrentSignalR400M(uint8_t *s) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

		// TODO �400� 3-� �������� ����� �������� �����
		// �������� ��� �������� ��� ������� �� + ��1, ��2 � �.�.

		uint8_t t = *s;
		if (t & 0x10) {
			signal = GB_SIGNAL_RZ;

			// ��������� "�� � ��" ��� �� ������������ �������
			if (t & 0x01) {
				signal = GB_SIGNAL_CF1_RZ_R400M;
			} else if (t & 0x02) {
				signal = GB_SIGNAL_CF2_RZ_R400M;
			} else if (t & 0x04) {
				signal = GB_SIGNAL_CF3_RZ_R400M;
			} else if (t & 0x08) {
				signal = GB_SIGNAL_CF4_RZ_R400M;
			}
		} else {
			if (t & 0x01) {
				signal = GB_SIGNAL_CF1;
			} else if (t & 0x02) {
				signal = GB_SIGNAL_CF2;
			} else if (t & 0x04) {
				signal = GB_SIGNAL_CF3;
			} else if (t & 0x08) {
				signal = GB_SIGNAL_CF4;
			}
		}

		return signal;
	}

	/** ���������� ������� � ������ ��� ������.
	 * 	���: 7		6		5		4		3		2		1		0		;
	 * 	b1 : x 		x 		x 		[��]	x		x 		x 		[��]	;
	 * 	b2 : [���8] [���7]	[���6] 	[���5] 	[���4] 	[���3] 	[���2] 	[���1]	;
	 * 	b3 : [���16][���15] [���14] [���13] [���12] [���11] [���10] [���9]	;
	 * 	b4 : [���24][���23] [���22] [���21] [���20] [���19] [���18] [���17]	;
	 * 	b5 : [���32][���31] [���30] [���29] [���28] [���27] [���26] [���25]	;
	 * 	������������� ��� �������� ������� ������� ������� �� ��������.
	 *	����� ������� �� ������� �������������� ����.
	 * 	@param *s ��������� �� ������ ������.
	 * 	@return ������� �������� ������.
	 */
	eGB_TEST_SIGNAL getCurrentSignalOpto(uint8_t *s) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

		uint8_t t = *s;
		if (t & 0x10) {
			signal = GB_SIGNAL_RZ;
		} else if (t & 0x01) {
			signal = GB_SIGNAL_CS;
		} else {
			t = getSetBit(*(++s));
			if (t != 0) {
				t = (t - 1) + GB_SIGNAL_COM1;
				signal = (eGB_TEST_SIGNAL) t;
			} else {
				// ����������� ������� ������ � 9 �� 16
				t = getSetBit(*(++s));
				if (t) {
					t = GB_SIGNAL_COM9 + t - 1;
					signal = (eGB_TEST_SIGNAL) t;
				} else {
					// ����������� ������� ������ � 17 �� 24
					t = getSetBit(*(++s));
					if (t) {
						t = GB_SIGNAL_COM17 + t - 1;
						signal = (eGB_TEST_SIGNAL) t;
					} else {
						// ����������� ������� ������ � 25 �� 32
						t = getSetBit(*(++s));
						if (t) {
							t = GB_SIGNAL_COM25 + t - 1;
							signal = (eGB_TEST_SIGNAL) t;
						}
					}
				}
			}
		}
		return signal;
	}

	/** ���������� ������� � ������ ��� ������.
		 * 	���: 7		6		5		4		3		2		1		0		;
		 * 	b1 : x 		x 		x 		[��]	x		x 		x 		[��]	;
		 * 	b2 : [�8A]	[�7A]	[�6A] 	[�5A] 	[�4A] 	[�3A] 	[�2A] 	[�1A]	;
		 * 	b3 : [�16A]	[�15A] 	[�14A] 	[�13A] 	[�12A] 	[�11A] 	[�10A] 	[�9A]	;
		 * 	b4 : [�24A]	[�23A] 	[�22A] 	[�21A] 	[�20A] 	[�19A] 	[�18A] 	[�17A]	;
		 * 	b5 : [�32A]	[�31A]	[�30A] 	[�29A] 	[�28A] 	[�27A] 	[�26A] 	[�25A]	;
		 * 	b6 : [�8B]	[�7B]	[�6B] 	[�5B] 	[�4B] 	[�3B] 	[�2B] 	[�1B]	;
		 * 	b7 : [�16B]	[�15B] 	[�14B] 	[�13B] 	[�12B] 	[�11B] 	[�10B] 	[�9B]	;
		 * 	b8 : [�24B]	[�23B] 	[�22B] 	[�21B] 	[�20B] 	[�19B] 	[�18B] 	[�17B]	;
		 * 	b9 : [�32B]	[�31B]	[�30B] 	[�29B] 	[�28B] 	[�27B] 	[�26B] 	[�25B]	;
		 * 	b10: [�8C]	[�7C]	[�6C] 	[�5C] 	[�4C] 	[�3C] 	[�2C] 	[�1C]	;
		 * 	b11: [�16C]	[�15C] 	[�14C] 	[�13C] 	[�12C] 	[�11C] 	[�10C] 	[�9C]	;
		 * 	b12: [�24C]	[�23C] 	[�22C] 	[�21C] 	[�20C] 	[�19C] 	[�18C] 	[�17C]	;
		 * 	b13: [�32C]	[�31C]	[�30C] 	[�29C] 	[�28C] 	[�27C] 	[�26C] 	[�25C]	;
		 * 	������������� ��� �������� ������� ������� ������� �� ��������.
		 *	����� ������� �� ������� �������������� ����.
		 * 	@param *s ��������� �� ������ ������.
		 * 	@return ������� �������� ������.
		 */
	eGB_TEST_SIGNAL getCurrentSignalOptoRingUni(uint8_t *s) {
		eGB_TEST_SIGNAL signal = GB_SIGNAL_OFF;

		uint8_t t = *s;
		if (t & 0x10) {
			signal = GB_SIGNAL_RZ;
		} else if (t & 0x01) {
			signal = GB_SIGNAL_CS;
		} else {
			for(uint8_t i = 0; i < (MAX_NUM_COM_RING / 8); i++) {
				t = getSetBit(*(++s));
				if (t != 0) {
					t = (t - 1) + GB_SIGNAL_COM1A + i*8;
					signal = (eGB_TEST_SIGNAL) t;
				}
			}
		}
		return signal;
	}
};

#endif /* GLBDEF_H_ */
