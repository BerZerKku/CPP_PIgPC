/*
 * glb_def.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

/**
 *  ������������ �������� ��� ��������� ���������� �� 1 ������ ����������,
 *  ��� �������� � ������� ���������� �������� � �������� �����������
 *  ��������������.
 *  ��� ��� ��� ����� ���������� �������� �������� ������ ���� ������ �� 1 !!
 */

#ifndef GLBDEF_H_
#define GLBDEF_H_

#include <stdint.h>
#include "avr.h"
#include "menu/dateTime.h"
#include "debug/debug.hpp"

/// ������ �������
#define SIZE_OF(mas) (sizeof(mas) / sizeof(mas[0]))

/// ������ ������� ��������
#define VERS 0x0149

/// ����������� ���-�� ������ �� ����� (������ ���� ������ 8)
#define MAX_NUM_COM_PRM 32

/// ������������ ���-�� ������ �� �������� (������ ���� ������ 8)
#define MAX_NUM_COM_PRD 32

/// ������������ ���-�� ���������� ������ (+1 - ��� 0 �������)
#define MAX_NUM_COM_RING 96

/// �������������� ���� CHAR � INT
#define TO_INT16(high, low) (((uint16_t) (high) << 8) + (low))

/// �������������� �������-����������� ����� � �����
#define BCD_TO_BIN(val) ((val >> 4) * 10 + (val & 0x0F))

/// �������������� ������ ����� � �������-���������� ���
#define BIN_TO_BCD(val) (((val / 10) << 4) + (val % 10))

/// ����������� ��������� ���-�� ��������� ���������
#define MAX_NUM_DEVICE_STATE 14

/// ������������ ���-�� �������������� ��� ������ ����������
#define MAX_NUM_FAULTS 16

/// ������������ ���-�� �������������� ��� ������ ����������
#define MAX_NUM_WARNINGS 16

/// ����������� ���-�� ������ ������������ �� ���� �������
#define MIN_NUM_COM_SEND_IN_1_SEK 8

/// ������������ ���-�� �������� � ������
#define MAX_NUM_TEST_SIGNAL 100

// ����� �������� ������ (+1 - ������ ����� ������)
#define STRING_LENGHT (11 + 1)

// ����� ������ (+1 - ������ ����� ������)
#define NAME_PARAM_LENGHT (20 + 1)

/// ������������ � ����������� ��� ���� ������� � ������� �������
#define MIN_JRN_EVENT_VALUE 1
#define MAX_JRN_EVENT_VALUE 38

#define GLB_JRN_EVENT_K400_MAX  512     /// ���-�� ������� � ������� ������� �400
#define GLB_JRN_EVENT_R400M_MAX 512     /// ���-�� ������� � ������� ������� �400�
#define GLB_JRN_EVENT_RZSK_MAX  256     /// ���-�� ������� � ������� ������� ����
#define GLB_JRN_EVENT_OPTO_MAX  256     /// ���-�� ������� � ������� ������� ������

#define GLB_JRN_DEF_K400_MAX    0       ///< ���-�� ������� � ������� ������ �400
#define GLB_JRN_DEF_R400M_MAX   1024    ///< ���-�� ������� � ������� ������ �400�
#define GLB_JRN_DEF_RZSK_MAX    256     ///< ���-�� ������� � ������� ������ ����
#define GLB_JRN_DEF_OPTO_MAX    2048    ///< ���-�� ������� � ������� ������ ������

#define GLB_JRN_PRM_K400_MAX    512     ///< ���-�� ������� � ������� ��������� �400
#define GLB_JRN_PRM_RZSK_MAX    256     ///< ���-�� ������� � ������� ��������� ����
#define GLB_JRN_PRM_OPTO_MAX    256     ///< ���-�� ������� � ������� ��������� ������

#define GLB_JRN_PRD_K400_MAX    512     /// ���-�� ������� � ������� ����������� �400
#define GLB_JRN_PRD_RZSK_MAX    256     /// ���-�� ������� � ������� ����������� ����
#define GLB_JRN_PRD_OPTO_MAX    256     /// ���-�� ������� � ������� ����������� ������

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
    MENU_ENTER_PARAM_NO,        ///< ������ ��������� ���������
    MENU_ENTER_PARAM_INT,       ///< ��������� ���������, ����� ��������
    MENU_ENTER_PARAM_LIST,      ///< ��������� ���������, ����� �� ������
    MENU_ENTER_PARAM_LIST_2,    ///< ��������� ���������, ����� �� ������ ��������
    MENU_ENTER_PARAM_U_COR,     ///< ��������� ���������, ��������� ����������
    MENU_ENTER_PASSWORD,        ///< ���� ������
    MENU_ENTER_PASSWORD_NEW,    ///< ���� ������ ������
    MENU_ENTER_PASSWORD_READY,  ///< ������ ������ ������
    MENU_ENTER_PASSWORD_N_READY,///< ����� ������ ������ ���������
    MENU_ENTER_PARAM_READY,     ///< ���������� �������� ��������
    MENU_ENTER_PARAM_MESSAGE,   ///< ����� ��������� �� ����� ��� ����� ���������
};

/// ��� ��������
enum eGB_TYPE_DEVICE {
    AVANT_NO = 0,   // ��������� ��������
    AVANT_R400,     //
    AVANT_RZSK,     //
    AVANT_K400,     //
    AVANT_R400M,    //
    AVANT_OPTO,     //
    AVANT_MAX
};

/// ��� ����������� ��������
enum eGB_TYPE_OPTO {
    TYPE_OPTO_STANDART = 0, // ����������� ������
    TYPE_OPTO_RING_UNI,     // ���������� ���������������� ������
    TYPE_OPTO_RING_BI,      // ��������� ��������������� ������
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
    GB_SEND_NO,         ///< ������� ������������ ��������� ���
    GB_SEND_INT8,       ///< ���������� ���� ���� ������.
    GB_SEND_INT8_DOP,   ///< ���������� ��� ����� ������ (��������, ���.����).
    GB_SEND_DOP_INT8,   ///< ���������� ��� ����� ������ (���.����, ��������).
    GB_SEND_INT16_BE,   ///< ���������� ��� ����� ������ (in16>>8, int16&0xFF).
    GB_SEND_DOP_BITES,  ///< ���������� ������� ���������� (���.����, ��������).
    GB_SEND_BITES_DOP,  ///< ���������� ������� ���������� (��������, ���.����).
    GB_SEND_COR_U,      ///< ���������� ��������� ����������.
    GB_SEND_COR_I       ///< ���������� ��������� ����.
};

/// ���-�� ��������� � �����
//  ������ ��� 1 � 2 ������������ ��� ���������� � 3-� ��������. �� ������ !!!
enum eGB_NUM_DEVICES {
    GB_NUM_DEVICES_MIN = 1,
    GB_NUM_DEVICES_2 = 1,
    GB_NUM_DEVICES_3 = 2,
    GB_NUM_DEVICES_MAX
};

/// ������ ������
enum eGB_REGIME {
    GB_REGIME_MIN = 0,              //
    GB_REGIME_DISABLED = 0,         // �������
    GB_REGIME_READY,                // �����
    GB_REGIME_ENABLED,              // ������
    GB_REGIME_TALK,                 // ����
    GB_REGIME_TEST_1,               // ���� ���
    GB_REGIME_TEST_2,               // ���� ���
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
    GB_COM_DEF_GET_DEF_TYPE     = 0x01, // +
    GB_COM_DEF_GET_LINE_TYPE    = 0x02, // +
    GB_COM_DEF_GET_T_NO_MAN     = 0x03, // +
    GB_COM_DEF_GET_DELAY        = 0x04, // +
    GB_COM_DEF_GET_OVERLAP      = 0x05, // +
    GB_COM_DEF_GET_RZ_DEC       = 0x06, // +
    GB_COM_DEF_GET_PRM_TYPE     = 0x07, // + ! � �400� ��� �������� ������ ��
    GB_COM_DEF_GET_FREQ_PRD     = 0x08, // +
    GB_COM_DEF_GET_RZ_THRESH    = 0x09, // + ! � �400� ��� ������� ���
    GB_COM_DEF_GET_TYPE_AC      = 0x0A, // +
    GB_COM_DEF_GET_LIMIT_PRD    = 0x0B, // +
    GB_COM_PRM_GET_TIME_ON      = 0x11, // +
    GB_COM_PRM_GET_TEST_COM     = 0x12, // +
    GB_COM_PRM_GET_TIME_OFF     = 0x13, // +
    GB_COM_PRM_GET_BLOCK_COM    = 0x14, // +
    GB_COM_PRM_GET_FREQ_CORR    = 0x15, // +
    GB_COM_PRM_GET_INC_SAFETY   = 0x16, // +
    GB_COM_PRM_GET_DR_STATE     = 0x17, // +
    GB_COM_PRM_GET_DR_BLOCK     = 0x18, // +
    GB_COM_PRM_GET_DR_COM       = 0x19, // +
    GB_COM_PRM_GET_RING_COM_REC = 0x1B, // +
    GB_COM_PRM_GET_COM          = 0x1C, // +
    GB_COM_PRM_GET_COM_SIGN     = 0x1D, // +
    GB_COM_PRD_GET_TIME_ON      = 0x21, // +
    GB_COM_PRD_GET_DURATION     = 0x22, // +
    GB_COM_PRD_GET_FREQ_CORR    = 0x23, // +
    GB_COM_PRD_GET_BLOCK_COM    = 0x24, // +
    GB_COM_PRD_GET_LONG_COM     = 0x25, // +
    GB_COM_PRD_GET_TEST_COM     = 0x26, // +
    GB_COM_PRD_GET_DR_STATE     = 0x27, // +
    GB_COM_PRD_GET_DR_BLOCK     = 0x28, // +
    GB_COM_PRD_GET_COM_A        = 0x29, // +
    GB_COM_PRD_GET_RING_COM_TR  = 0x2B, // +
    GB_COM_PRD_GET_COM          = 0x2C, // +
    GB_COM_PRD_GET_CF_TM        = 0x2D, // +
    GB_COM_PRD_GET_COM_SIGN     = 0x2E, // +
    GB_COM_GET_SOST             = 0x30, // +
    GB_COM_GET_FAULT            = 0x31, // +
    GB_COM_GET_TIME             = 0x32, // +
    GB_COM_GET_COR_U_I          = 0x33, // + ! � ������ ��� ��������������
    GB_COM_GET_MEAS             = 0x34, // +
    GB_COM_GET_TIME_SINCHR      = 0x35, // +
    GB_COM_GET_COM_PRM_KEEP     = 0x36, // + ! � �400� ��� U��� �����������
    GB_COM_GET_COM_PRD_KEEP     = 0x37, // + ! ������������� ��� ���������� ��������, ������������, �������������� � ������ �� D � �400 � �.�.
    GB_COM_GET_NET_ADR          = 0x38, // +
    GB_COM_GET_TIME_RERUN       = 0x39, // + ! � �400� ��� ��������� ��� ��������������
    GB_COM_GET_FREQ             = 0x3A, // +
    GB_COM_GET_DEVICE_NUM       = 0x3B, // +
    GB_COM_GET_CF_THRESHOLD     = 0x3C, // + ! ����� �������������� � �����������
    GB_COM_GET_OUT_CHECK        = 0x3D, // +
    GB_COM_GET_TEST             = 0x3E, // +
    GB_COM_GET_VERS             = 0x3F, // +
    GB_COM_PRM_ENTER            = 0x51, // +
    GB_COM_SET_REG_DISABLED     = 0x70, // +
    GB_COM_SET_REG_ENABLED      = 0x71, // +
    GB_COM_SET_CONTROL          = 0x72, // +
    GB_COM_SET_PASSWORD         = 0x73, // + ! ������ � ��
    GB_COM_GET_PASSWORD         = 0x74, // + ! ������ � ��
    GB_COM_SET_REG_TEST_2       = 0x7D, // +
    GB_COM_SET_REG_TEST_1       = 0x7E, // +
    GB_COM_DEF_SET_DEF_TYPE     = 0x81, // +
    GB_COM_DEF_SET_LINE_TYPE    = 0x82, // +
    GB_COM_DEF_SET_T_NO_MAN     = 0x83, // +
    GB_COM_DEF_SET_DELAY        = 0x84, // +
    GB_COM_DEF_SET_OVERLAP      = 0x85, // +
    GB_COM_DEF_SET_RZ_DEC       = 0x86, // +
    GB_COM_DEF_SET_PRM_TYPE     = 0x87, // + ! � �400� ��� �������� ������ ��
    GB_COM_DEF_SET_FREQ_PRD     = 0x88, // +
    GB_COM_DEF_SET_RZ_THRESH    = 0x89, // + ! � �400� ��� ������� ���
    GB_COM_DEF_SET_TYPE_AC      = 0x8A, // +
    GB_COM_DEF_SET_LIMIT_PRD    = 0x8B, //
    GB_COM_PRM_SET_TIME_ON      = 0x91, // +
    GB_COM_PRM_SET_TEST_COM     = 0x92, // +
    GB_COM_PRM_SET_TIME_OFF     = 0x93, // +
    GB_COM_PRM_SET_BLOCK_COM    = 0x94, // +
    GB_COM_PRM_SET_FREQ_CORR    = 0x95, // +
    GB_COM_PRM_SET_INC_SAFETY   = 0x16, // +
    GB_COM_PRM_SET_DR_STATE     = 0x97, // +
    GB_COM_PRM_SET_DR_BLOCK     = 0x98, // +
    GB_COM_PRM_SET_DR_COM       = 0x99, // +
    GB_COM_PRM_RES_IND          = 0x9A, // +
    GB_COM_PRM_SET_RING_COM_REC = 0x9B, // +
    GB_COM_PRM_SET_COM          = 0x9C, // +
    GB_COM_PRM_SET_COM_SIGN     = 0x9D, // +
    GB_COM_PRD_SET_TIME_ON      = 0xA1, // +
    GB_COM_PRD_SET_DURATION     = 0xA2, // +
    GB_COM_PRD_SET_FREQ_CORR    = 0xA3, // +
    GB_COM_PRD_SET_BLOCK_COM    = 0xA4, // +
    GB_COM_PRD_SET_LONG_COM     = 0xA5, // +
    GB_COM_PRD_SET_TEST_COM     = 0xA6, // +
    GB_COM_PRD_SET_DR_STATE     = 0xA7, // +
    GB_COM_PRD_SET_DR_BLOCK     = 0xA8, // +
    GB_COM_PRD_SET_COM_A        = 0xA9, // +
    GB_COM_PRD_RES_IND          = 0xAA, // +
    GB_COM_PRD_SET_RING_COM_TR  = 0xAB, // +
    GB_COM_PRD_SET_COM          = 0xAC, // +
    GB_COM_PRD_SET_CF_TM        = 0xAD, // +
    GB_COM_PRD_SET_COM_SIGN     = 0xAE, // +
    GB_COM_SET_TIME             = 0xB2, // +
    GB_COM_SET_COR_U_I          = 0xB3, // +
    GB_COM_SET_TIME_SINCHR      = 0xB5, // +
    GB_COM_SET_COM_PRM_KEEP     = 0xB6, // + ! � �400� ��� U��� �����������
    GB_COM_SET_COM_PRD_KEEP     = 0xB7, // + ! ������������� ��� ���������� ��������, ������������
    GB_COM_SET_NET_ADR          = 0xB8, // +
    GB_COM_SET_TIME_RERUN       = 0xB9, // +
    GB_COM_SET_FREQ             = 0xBA, // +
    GB_COM_SET_DEVICE_NUM       = 0xBB, // +
    GB_COM_SET_CF_THRESHOLD     = 0xBC, // +
    GB_COM_SET_OUT_CHECK        = 0xBD, // +
    GB_COM_DEF_GET_JRN_CNT      = 0xC1, // +
    GB_COM_DEF_GET_JRN_ENTRY    = 0xC2, // +
    GB_COM_DEF_JRN_CLR          = 0xCA, // ! �������� ������� ���, ������ � ��
    GB_COM_PRM_GET_JRN_CNT      = 0xD1, // +
    GB_COM_PRM_GET_JRN_ENTRY    = 0xD2, // +
    GB_COM_PRM_JRN_CLR          = 0xDA, // ! �������� ������� ���, ������ � ��
    GB_COM_PRD_GET_JRN_CNT      = 0xE1, // +
    GB_COM_PRD_GET_JRN_ENTRY    = 0xE2, // +
    GB_COM_PRD_JRN_CLR          = 0xEA, // ! �������� ������� ���, ������ � ��
    GB_COM_GET_JRN_CNT          = 0xF1, // +
    GB_COM_GET_JRN_ENTRY        = 0xF2, // +
    GB_COM_JRN_CLR              = 0xFA  // ! �������� ������� �������, ������ � ��
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
    POS_COM_NET_ADR_netAdr = 1
};

/// ������������������ ���������� � ������� COM_PRD_KEEP (0x37)
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
    POS_COM_PRD_KEEP_ringComTransit
};

/// ��������� (������� � fParams)
typedef enum {
    GB_PARAM_NULL_PARAM = 0,    ///< �������� ��������
    // ����� ���������
    GB_PARAM_TIME_SYNCH,        ///< ������������� �����
	GB_PARAM_TIME_SYNCH_SRC,	///< ������������� ����� (����� ���������)
    GB_PARAM_NUM_OF_DEVICE,     ///< ����� ��������
    GB_PARAM_NUM_OF_DEVICE_RING,///< ����� �������� � ���������� ������
    GB_PARAM_OUT_CHECK,         ///< �������� ��������� �������
    GB_PARAM_WARN_THD,          ///< ����� ��������������
    GB_PARAM_WARN_THD_CF,       ///< ����� �������������� �� �� (��� ����)
    GB_PARAM_TIME_RERUN,        ///< ����� �����������
    GB_PARAM_COM_PRD_KEEP,      ///< ��������� ���� ������ ���
    GB_PARAM_COM_PRM_KEEP,      ///< ��������� ���� ������ ���
    GB_PARAM_IN_DEC,            ///< ����������� ���������������� ���
    GB_PARAM_NET_ADDRESS,       ///< ����� � ��������� ����
    GB_PARAM_U_OUT_NOM,         ///< ������������ �������� ����������
    GB_PARAM_FREQ,              ///< �������
    GB_PARAM_COMP_P400,         ///< ������������� (�400, �400�)
    GB_PARAM_IN_DEC_AC_ANSWER,  ///< �������� ������ �� (����)
    GB_PARAM_DETECTOR,          ///< ��� ���������
    GB_PARAM_COR_U,             ///< ��������� ����������
    GB_PARAM_COR_I,             ///< ��������� ����
    GB_PARAM_PVZUE_PROTOCOL,    ///< �������� ������ (����-�)
    GB_PARAM_PVZUE_PARITY,      ///< ������� �������� (����-�)
    GB_PARAM_PVZUE_FAIL,        ///< ���������� ������� (����-�)
    GB_PARAM_PVZUE_NOISE_THD,   ///< ����� �� ������ (����-�)
    GB_PARAM_PVZUE_NOISE_LVL,   ///< ���������� ������ (����-�)
    GB_PARAM_PVZUE_AC_TYPE,     ///< ��� ������������ (����-�)
    GB_PARAM_PVZUE_AC_PERIOD,   ///< ������ ������� ������ �� (����-�)
    GB_PARAM_PVZUE_AC_PER_RE,   ///< ������ ������� ������� ������ �� (����-�)
    GB_PARAM_BACKUP,            ///< ��������������
    GB_PARAM_COMP_K400,         ///< ������������� �400
    GB_PARAM_NUM_OF_DEVICES,    ///< ��� ����� (���-�� ��������� � �����)
    GB_PARAM_TM_K400,           ///< ������������
    GB_PARAM_WARN_D,            ///< ������� ������������ ����������������� ������������ �� D
    GB_PARAM_ALARM_D,           ///< ������� ������������ ��������� ������������ �� D
    GB_PARAM_TEMP_MONITOR,      ///< �������� �����������
    GB_PARAM_TEMP_THR_HI,       ///< ������� �������� �����������
    GB_PARAM_TEMP_THR_LOW,      ///< ������ �������� �����������
    GB_PARAM_TM_SPEED,          ///< �������� ��
    GB_PARAM_COMP_RZSK,         ///< ������������� (����)
    // ��������� ������
    GB_PARAM_DEF_TYPE,          ///< ��� ������
    GB_PARAM_TIME_NO_MAN,       ///< �������������� ����� ��� �����������
    GB_PARAM_OVERLAP,           ///< ���������� ��������� (����� �400/�400�)
    GB_PARAM_OVERLAP_P400,      ///< ���������� ��������� (�400/�400�)
    GB_PARAM_OVERLAP_OPTO,      ///< ���������� ��������� (�400 ������)
    GB_PARAM_DELAY,             ///< ����������� ��������
    GB_PARAM_DELAY_OPTO,        ///< ����������� ��������
    GB_PARAM_WARN_THD_RZ,       ///< ����� �������������� �� �� (��� ����)
    GB_PARAM_SENS_DEC,          ///< ����������� ����������������
    GB_PARAM_SENS_DEC_RZ,       ///< ����������� ���������������� �� ��
    GB_PARAM_PRM_TYPE,          ///< ��� ���������
    GB_PARAM_AC_IN_DEC,         ///< �������� ������ ��
    GB_PARAM_FREQ_PRD,          ///< ������� ���
    GB_PARAM_FREQ_PRM,          ///< ������� ���
    GB_PARAM_SHIFT_FRONT,       ///< ����� ��������� ������ ���
    GB_PARAM_SHIFT_BACK,        ///< ����� ������� ������ ���
    GB_PARAM_SHIFT_PRM,         ///< ����� ���
    GB_PARAM_SHIFT_PRD,         ///< ����� �� ��� �� ����
    GB_PARAM_LIMIT_PRD,         ///< ����������� ������ �����������
    GB_PARAM_DELAY_OFF_PRM,     ///< �������� ���������� ���
    GB_PARAM_DELAY_ON_PRM,      ///< �������� ��������� ���
    GB_PARAM_DELAY_ON_PRD,      ///< �������� ��������� ���
    GB_PARAM_MIN_TIME_PRD,      ///< ����������� ������������ ���
    // ��������� �����������
    GB_PARAM_PRD_IN_DELAY,      ///< ����� ��������� (�������� ������������ ����������� �����)
    GB_PARAM_PRD_DURATION_L,    ///< ������������ ������� ��
    GB_PARAM_PRD_DURATION_O,    ///< ������������ ������� ������
    GB_PARAM_PRD_TEST_COM,      ///< �������� �������
    GB_PARAM_PRD_COM_LONG,      ///< �������� �������
    GB_PARAM_PRD_COM_BLOCK,     ///< ������������� �������
    GB_PARAM_PRD_DR_ENABLE,     ///< ���������� ��
    GB_PARAM_PRD_DR_COM_BLOCK,  ///< ������������� ������� ��
    GB_PARAM_PRD_COM_NUMS,      ///< ���������� ������
    GB_PARAM_PRD_COM_NUMS_A,    ///< ���������� ������ ������ �
    GB_PARAM_PRD_FREQ_CORR,     ///< ��������� ������� ���
    GB_PARAM_PRD_DEC_CF,        ///< �������� ������ ��
    GB_PARAM_PRD_DEC_TM,        ///< �������� ������ ��
    GB_PARAM_PRD_DEFAULT_CF,    ///< �� �� ���������
    GB_PARAM_PRD_COM_SIGNAL,    ///< ������������ ������ ���
    // ��������� ���������
    GB_PARAM_PRM_TIME_ON,       ///< �������� �� �������� ������� (����� ���������)
    GB_PARAM_PRM_COM_BLOCK,     ///< ������������� �������
    GB_PARAM_PRM_TIME_OFF,      ///< �������� �� ����������
    GB_PARAM_PRM_DR_ENABLE,     ///< ���������� ��
    GB_PARAM_PRM_DR_COM_BLOCK,  ///< ������������� ������� ��
    GB_PARAM_PRM_DR_COM_TO_HF,  ///< ������� �� � ��
    GB_PARAM_PRM_COM_NUMS,      ///< ���������� ������
    GB_PARAM_PRM_TEST_COM,      ///< ����� �������� �������
    GB_PARAM_PRM_FREQ_CORR,     ///< ��������� ������� ���
    GB_PARAM_PRM_COM_SIGNAL,    ///< ������������ ������ ���
    GB_PARAM_PRM_INC_SAFETY,    ///< ��������� ������������
    // ��������� ����������
    GB_PARAM_INTF_INTERFACE,    ///< ��������� �����
    GB_PARAM_INTF_PROTOCOL,     ///< ��������
    GB_PARAM_INTF_BAUDRATE,     ///< �������� ��������
    GB_PARAM_INTF_DATA_BITS,    ///< ���� ������
    GB_PARAM_INTF_PARITY,       ///< ��������
    GB_PARAM_INTF_STOP_BITS,    ///< �������� ����
    // ��������� ������
    GB_PARAM_RING_TIME_WAIT,    ///< ����� �������� ������
    GB_PARAM_RING_COM_TRANSIT,  ///< ���������� �������
    GB_PARAM_RING_COM_REC,      ///< �������������� ������ ���������
    GB_PARAM_RING_COM_TR,       ///< �������������� ������ ���������� ������
    // ������
    GB_PARAM_OTHER_TEST_SIGNAL, ///< �������� ������.
    //
    GB_PARAM_MAX
} eGB_PARAM;

/// �������� ������ ����������
enum eGB_CONTROL {
    GB_CONTROL_RESET_SELF   = 1,    // ����� ������
    GB_CONTROL_RESET_UD     = 2,    // ����� ���������� (-��)
    GB_CONTROL_RESET_AC     = 3,    // ����� ��
    GB_CONTROL_PUSK_UD_1    = 4,    // ���� ���������� (1)
    GB_CONTROL_PUSK_UD_2    = 5,    // ���� ���������� 2
    GB_CONTROL_PUSK_UD_ALL  = 6,    // ���� ���������
    GB_CONTROL_CALL         = 7,    // �����
    GB_CONTROL_PUSK_ON      = 8,    // ���� ���������� ���.
    GB_CONTROL_PUSK_OFF     = 9,    // ���� ���������� ����.
    GB_CONTROL_PUSK_AC_UD   = 10,   // ���� �� ���������
    GB_CONTROL_RESET_UD_1   = 11,   // ����� ���������� 1
    GB_CONTROL_RESET_UD_2   = 12,   // ����� ���������� 2
    GB_CONTROL_PUSK_UD_3    = 13,   // ���� ���������� 3
    GB_CONTROL_REG_AC       = 14,   // ����� ��
    GB_CONTROL_RESET_UD_3   = 15,   // ����� ���������� 3
    GB_CONTROL_MAN_1        = 16,   // ���� ��� ���������� (1)
    GB_CONTROL_MAN_2        = 17,   // ���� ��� ���������� 2
    GB_CONTROL_MAN_3        = 18,   // ���� ��� ���������� 3
    GB_CONTROL_MAN_ALL      = 19,   // ���� ��� ���������
    GB_CONTROL_MAX                  //
};

/// ������� ������� �����������/��������� - ����� � ������ �������
enum eGB_STATE_COM {
    GB_STATE_COM_MIN    = 0,        ///< ����������� ��������
    GB_STATE_COM_END    = 0,        ///< ��������� �������
    GB_STATE_COM_START,             ///< ������ �������
    GB_STATE_COM_MAX                ///< ������������ ��������
};

/// �������� ������������ �������
enum eGB_SOURCE_COM {
    GB_SOURCE_COM_DI    = 0,        ///< ������� ��������� � ����������� �����
    GB_SOURCE_COM_DR    = 1,        ///< ������� ��������� � ��������� �����
    GB_SOURCE_COM_MAX               ///< ��������� ��������
};

/// ������� � ����1 � ����2
enum eGB_TEST_SIGNAL {
    GB_SIGNAL_OFF = 0,              //
    GB_SIGNAL_CF,                   // ���� / �400�
    GB_SIGNAL_CF1,
    GB_SIGNAL_CF2,
    GB_SIGNAL_CF3,
    GB_SIGNAL_CF4,
    GB_SIGNAL_CF_NO_RZ,             // ����
    GB_SIGNAL_CF_RZ,                // ����
    GB_SIGNAL_CF2_NO_RZ,            // ����
    GB_SIGNAL_CF2_RZ,               // ����
    GB_SIGNAL_RZ,
    GB_SIGNAL_COM1_NO_RZ,           // ����
    GB_SIGNAL_COM2_NO_RZ,           // ����
    GB_SIGNAL_COM3_NO_RZ,           // ����
    GB_SIGNAL_COM4_NO_RZ,           // ����
    GB_SIGNAL_COM5_NO_RZ,           // ����
    GB_SIGNAL_COM6_NO_RZ,           // ����
    GB_SIGNAL_COM7_NO_RZ,           // ����
    GB_SIGNAL_COM8_NO_RZ,           // ����
    GB_SIGNAL_COM1_RZ,              // ����
    GB_SIGNAL_COM2_RZ,              // ����
    GB_SIGNAL_COM3_RZ,              // ����
    GB_SIGNAL_COM4_RZ,              // ����
	GB_SIGNAL_COM5_RZ,              // ����
	GB_SIGNAL_COM6_RZ,              // ����
	GB_SIGNAL_COM7_RZ,              // ����
	GB_SIGNAL_COM8_RZ,              // ����
    GB_SIGNAL_COM1,                 // vvvvvvvvvvvvvvvvvvvvvvvvvv
    GB_SIGNAL_COM2,                 // ������� ������ ������ ����
    GB_SIGNAL_COM3,                 // ������ ��� ���������� �400
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
    GB_SIGNAL_COM30,                // ������� ������ ������ ����
    GB_SIGNAL_COM31,                // ������ ��� ���������� �400
    GB_SIGNAL_COM32,                // ^^^^^^^^^^^^^^^^^^^^^^^^^^
    GB_SIGNAL_CF_RZ_R400M,
    GB_SIGNAL_CF1_RZ_R400M,
    GB_SIGNAL_CF2_RZ_R400M,
    GB_SIGNAL_CF3_RZ_R400M,
    GB_SIGNAL_CF4_RZ_R400M,
    GB_SIGNAL_CS,                   // � ������ ������ ��
    GB_SIGNAL_COM1A,                // ������ ���������������� vvv
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
    GB_SIGNAL_COM32C,   // ������ ���������������� ^^^
    GB_SIGNAL_MAX
};



/// ��������� ��������� ��������� �������� ���������.
enum eGB_ACT {
    GB_ACT_NO       = 0,        // �������� �� ���������
    GB_ACT_OLD      = 0x01,     // ����� �������� ��������� � ����������
    GB_ACT_NEW      = 0x02,     // ����������� ����� ��������
    GB_ACT_ERROR    = 0x04      // ��������� ��������
};


/// ���������� ������� � �400
enum eGB_K400_NUM_COM {
    GB_K400_NUM_COM_MIN = 0,    ///< ����������� ��������
    GB_K400_NUM_COM_0   = 0,    ///<
    GB_K400_NUM_COM_4   = 1,    ///<
    GB_K400_NUM_COM_8   = 2,    ///<
    GB_K400_NUM_COM_12  = 3,    ///<
    GB_K400_NUM_COM_16  = 4,    ///<
    GB_K400_NUM_COM_24  = 5,    ///<
    GB_K400_NUM_COM_32  = 6,    ///<
    GB_K400_NUM_COM_MAX         ///< ���-�� ������� � ������
};

/// ����� ��� �������� ��������� ��������
class TDeviceStatus {
public:
    /// ������������ ���-�� ��������� ��� ������� ��������� ���������
    // �.�. ������������ 3 ����, 0 ��� - 1 �������, ... 2 ���� - 3 �������.
    // ���� ����������� ��� ��� - ������.
    static const uint8_t GB_MAX_REM_NUM = 7;

    TDeviceStatus() {
        // ������������ ������ �� ���������
        static const char nameDev[] PROGMEM = "���";
        name = nameDev;
        enable_ = false;
        fault_ = 0;
        faults_ = 0;
        numFaults_ = 0;
        warning_ = 0;
        warnings_ = 0;
        numWarnings_ = 0;
        remoteNumber_ = 0;
        regime_ = GB_REGIME_MAX;
        state_ = MAX_NUM_DEVICE_STATE;
        dopByte_ = 0;
    }

    /** ��������� ������ ���������� ��������.
     *
     *  ������������ ��� ������ ������� ��������� ���������, ��� �������
     *  �������������� ������ (����� �400(�) ������������� � ����-�).
     *
     *  @param val ����� ��������(��). ��� ������� ���� �������� �� �����������-
     *  ��� �������.
     *  @retval True - � ������ �������� ������ ���������.
     *  @retval False - ���� ���� ���������� ������ ������
     */
    bool setRemoteNumber(uint8_t val) {
        bool state = false;

        if (val > GB_MAX_REM_NUM) {
            remoteNumber_ = GB_MAX_REM_NUM;
        } else {
            remoteNumber_ = val;
            state = true;
        }

        return state;
    }

    /** ���������� ����� ���������� ��������.
     *
     *  ������������ ��� ������ ������� ��������� ���������, ��� �������
     *  �������������� ������ (����� �400(�) ������������� � ����-�).
     *
     *  @param ����� ��������(��), � ������ ������ � �������  fcRemoteNum.
     */
    uint8_t getRemoteNumber() const {
        return remoteNumber_;
    }

    // ������
    bool setFault(uint16_t faults) {
        fault_ = getFirstSetBit(faults);
        faults_ = faults;
        numFaults_ = getNumSetBits(faults);
        return true;
    }
    uint8_t getFault() const {
        return fault_;
    }
    uint16_t getFaults() const {
        return faults_;
    }
    uint8_t getNumFaults() const {
        return numFaults_;
    }

    // �������������
    bool setWarning(uint16_t warnings) {
        warning_ = getFirstSetBit(warnings);
        warnings_ = warnings;
        numWarnings_ = getNumSetBits(warnings);
        return true;
    }
    uint8_t getWarning() const {
        return warning_;
    }
    uint16_t getWarnings() const {
        return warnings_;
    }
    uint8_t getNumWarnings() const {
        return numWarnings_;
    }

    /** ���������� ������� ��������� ���������� ������ ��������������.
     *
     *  ��� �������� ��������� 0xFF ���������� true ���� ���� ���� �� ����
     *  ��������������. ��� ��������� �� 0
     *
     *  @param shift ����� ��������������.
     *  @return ��������� ��������������.
     */
    bool isWarning(uint8_t shift=0xFF) const {
        bool s = false;
        if (shift == 0xFF) {
            s = (warnings_ != 0);
        } else if (shift < 16) {
            s = ((warnings_ & (1 << shift)) != 0);
        }
        return s;
    }

    /** ���������� ������� ��������� ���������� ������ �������������.
     *
     *  ��� �������� ��������� 0xFF ���������� true ���� ���� ���� �� ����
     *  ��������������. ��� ��������� �� 0
     *
     *  @param shift ����� �������������.
     *  @return ��������� �������������.
     */
    bool isFault(uint8_t shift=0xFF) const {
        bool s = false;
        if (shift == 0xFF) {
            s = (faults_ != 0);
        } else if (shift < 16) {
            s = ((faults_ & (1 << shift)) != 0);
        }
        return s;
    }

    // ����� ������
    // ��� GLB ������������ GB_REGIME_DISABLED, � ������ ���� ��� ���������
    // ���������� ��������, ����� GB_REGIME_MAX
    bool setRegime(eGB_REGIME regime) {
        bool stat = false;
        if ((regime >= GB_REGIME_MIN) && (regime <= GB_REGIME_MAX)) {
            stat = true;
        } else
            regime = GB_REGIME_MAX;
        regime_ = regime;
        return stat;
    }
    eGB_REGIME getRegime() const {
        return regime_;
    }

    //���������
    bool setState(uint8_t state) {
        bool stat = false;
        if (state < MAX_NUM_DEVICE_STATE) {
            stat = true;
        } else {
            state = MAX_NUM_DEVICE_STATE;
        }
        state_ = state;
        return stat;
    }
    uint8_t getState() const {
        return state_;
    }

    uint8_t getDopByte() const {
        return dopByte_;
    }
    bool setDopByte(uint8_t byte) {
        dopByte_ = byte;
        return true;
    }

    // ������ � ������ ������� ����������
    // ���������� true ���� ����� �������� ���������� �� ��������
    bool setEnable(bool val) {
        uint8_t act = GB_ACT_NO;

        if (enable_ == val) {
            act |= GB_ACT_OLD;
        } else {
            act |= GB_ACT_NEW;
        }
        enable_ = val;

        return act;
    }
    bool isEnable() const {
        return enable_;
    }

    // ������� ����������� ������ � ��������������
    PGM_P faultText[MAX_NUM_FAULTS];
    PGM_P warningText[MAX_NUM_WARNINGS];
    PGM_P stateText[MAX_NUM_DEVICE_STATE + 1];
    PGM_P name;

private:
    // ������� ������������ �������������, ������������� � ���-�� ��������������
    uint8_t fault_;
    uint16_t faults_;
    uint8_t numFaults_;

    // ������� ������������ ��������������, �������������� � ���-�� ����������.
    uint8_t warning_;
    uint16_t warnings_;
    uint8_t numWarnings_;

    // ����� ���������� �������� ��� �������� ���� ������������ ������/�������.
    // ������������ � �400(�) � ������������� � ����-�
    uint8_t remoteNumber_;

    eGB_REGIME regime_;
    uint8_t state_;
    uint8_t dopByte_;

    // True - �������� ������� ������� ���������� � ������� ���������� ������
    bool enable_;


    // ���������� ���-�� ������������� � 1 ���
    uint8_t getNumSetBits(uint16_t val)
    {
        uint8_t cnt = 0;
        for(; val > 0; val >>= 1)
        {
            if (val & 0x0001)
            cnt++;
        }
        return cnt;
    }

    // ���������� ����� �������� �������������� ����
    uint8_t getFirstSetBit(uint16_t val)
    {
        uint8_t cnt = 0;
        for(; val > 0; val >>= 1)
        {
            if (val & 0x0001)
            break;
            cnt++;
        }
        return cnt;
    }
};


class TJournalEntry {
public:
    TJournalEntry() {
        clear();
    }

    void clear() {
        currentDevice_ = GB_DEVICE_MAX;
        deviceJrn_ = GB_DEVICE_K400_MAX;
        eventType_ = MAX_JRN_EVENT_VALUE - MIN_JRN_EVENT_VALUE + 1;
        regime_ = GB_REGIME_MAX;
        numCom_ = 0;
        sourceCom_ = GB_SOURCE_COM_MAX;
        srcCom_ = 0;
        signalPusk_ = false;
        signalStop_ = false;
        signalMan_ = false;
        signalPrm_ = false;
        signalPrd_ = false;
        signalOut_ = false;
        signals_ = 0;

        numJrnEntries_ = 0;
        maxNumJrnEntry_ = 0;
        overflow_ = false;
        addressFirstEntry_ = 0;

        currentEntry_ = 1;
        ready_ = false;

        val = false;
    }

    // ����� ��� ������ �������
    TDateTime dateTime;

    // ����� ��� �������� �� ����������
    TDateTime dateTimeTr;

    // �������� ��� �������� �� ���������
    bool val;

    bool setCurrentDevice(eGB_DEVICE device) {
        bool stat = false;
        if ((device >= GB_DEVICE_MIN) && (device < GB_DEVICE_MAX)) {
            currentDevice_ = device;
            stat = true;
        } else
            currentDevice_ = GB_DEVICE_MAX;
        return stat;
    }
    eGB_DEVICE getCurrentDevice() const {
        return currentDevice_;
    }

    // ������\���������� ���������� ��� �������� ������� ������
    bool setDeviceJrn(eGB_DEVICE_K400 device) {
        bool stat = false;
        if ((device >= GB_DEVICE_K400_MIN) && (device < GB_DEVICE_K400_MAX)) {
            deviceJrn_ = device;
            stat = true;
        } else
            deviceJrn_ = GB_DEVICE_K400_MAX;
        return stat;
    }
    eGB_DEVICE_K400 getDeviceJrn() const {
        return deviceJrn_;
    }

    // ��� �������
    bool setEventType(uint8_t val) {
        bool state = false;

        uint8_t min = 255;
        uint8_t max = 0;
        // ��������� ���/���� �������� �������, � ����������� ��
        // �������� �������
        if (currentDevice_ == GB_DEVICE_GLB) {
            min = MIN_JRN_EVENT_VALUE;
            max = MAX_JRN_EVENT_VALUE;
        } else if (currentDevice_ == GB_DEVICE_PRD) {
            min = GB_STATE_COM_MIN;
            max = GB_STATE_COM_MAX;
        } else if (currentDevice_ == GB_DEVICE_PRM) {
            min = GB_STATE_COM_MIN;
            max = GB_STATE_COM_MAX;
        } else if (currentDevice_ == GB_DEVICE_DEF) {
            min = 0;
            max = MAX_NUM_DEVICE_STATE;
        }

        if ((val >= min) && (val <= max)) {
            eventType_ = val;
            state = true;
        } else
            eventType_ = max;

        return state;
    }
    uint8_t getEventType() const {
        return eventType_;
    }

    // �������� �������
    bool setSrcCom(uint8_t val) {
        srcCom_ = val & 0x07;
        return true;
    }
    uint8_t getSrcCom() const {
        return srcCom_;
    }


    /** ��������� ������� ��� �������� ������.
     *  ����������� 4 �����, ������� ������.
     *  ������ ������������� ��� � ��� �������� �� ���� �������.
     *  @param buf ��������� �� ������ �� 4 ���� ������.
     *  @retval True - ������.
     */
    bool setOpticEntry(uint8_t *buf) {
        uint8_t num = 0;

        // � ������ ����� ������ ��������� ���-�� ������������� �����
        for(uint_fast8_t i = 0; i <= 3; i++) {
            uint8_t byte = *buf++;
            opticEntry_[i] = byte;
            for(uint_fast8_t j = 1; j > 0; j <<= 1) {
                if (byte & j) {
                    num++;
                }
            }
        }
        numOpticEntries_ = num;

        return true;
    }

    /** ���������� ���-�� �������� ������� � ������� ������.
     *  ��� ������� ������� ��� ���-�� �������, ��� ������ - ������.
     *  @return ���-�� �������� �������� ������� ������
     */
    uint8_t getNumOpticsEntries() const {
        return numOpticEntries_;
    }

    /** ���� � ���������� ��� �������, ������ �� ������ �������.
     *  �.�. ���� � ��� � ������ 5 �������� �������, ��� ������� �� ���
     *  ����� �������� ��� ������ (�������� ����� �������).
     *
     *  @param num ����� �������, ������� � 1.
     *  @return ��� ������� (0 - � ������ ������).
     */
    uint8_t getOpticEntry(uint8_t num) {
        uint8_t val = 0;

        // �������� �� ���������� ��������
        if ((num >= 1) && (num <= numOpticEntries_)) {
            for(uint_fast8_t i = 0; i <= 3; i++) {
                // ������� 4-� ����, ������� � ��������
                uint8_t byte = opticEntry_[3 - i];
                for(uint_fast8_t j = 0; j < 8; j++) {
                    if (byte & (1 << j)) {
                        if (--num == 0) {
                            // ����� �������������� ����, �� 1 �� 32
                            val = 1 + ((i * 8) + j);
                        }
                    }
                }
            }
        }
        return val;
    }

    /** ��������� ������� ��� �������� ������ ���.
     *
     *  ����������� 4 �����, ������� ������.
     *  ������ ������������� ��� � ��� �������� �� ���� �������.
     *
     *  @param buf ��������� �� ������ �� 4 ���� ������.
     *  @retval True - ������.
     */
    bool setOpticEntryDR(uint8_t *buf) {
        uint8_t num = 0;

        // � ������ ����� ������ ��������� ���-�� ������������� �����
        for(uint_fast8_t i = 0; i <= 3; i++) {
            uint8_t byte = *buf++;
            opticEntryDR_[i] = byte;
            for(uint_fast8_t j = 1; j > 0; j <<= 1) {
                if (byte & j) {
                    num++;
                }
            }
        }
        numOpticEntriesDR_ = num;

        return true;
    }

    /** ���������� ���� ������� ���.
     *
     *  @param com ����� ������� ��� ������� ����������� ���� 1..32.
     *  @return 1 - ���� ������� �������� �� ���, 0 - � ����������� �����.
     */
    uint8_t getOpticEntryDR(uint8_t com) {
        eGB_SOURCE_COM val = GB_SOURCE_COM_DI;

        if ((com > 0) && (com < MAX_NUM_COM_PRD)) {
            com--;

            uint8_t i = (MAX_NUM_COM_PRD - 1) / 8;
            while(com > 8) {
                com -= 8;
                i--;
            }

            if (opticEntryDR_[i] & (1 << com)) {
                val = GB_SOURCE_COM_DR;
            }
        }

        return val;
    }

    // �����
    bool setRegime(eGB_REGIME regime) {
        bool state = false;
        if ((regime >= GB_REGIME_MIN) && (regime <= GB_REGIME_MAX)) {
            regime_ = regime;
            state = true;
        } else
            regime_ = GB_REGIME_MAX;
        return state;
    }
    eGB_REGIME getRegime() const {
        return regime_;
    }

    // ������� ��� ������� ������
    bool setSignalDef(uint8_t val) {
        bool state = true;

        signalPusk_ = (val & 0x10) ? 1 : 0;
        signalStop_ = (val & 0x20) ? 1 : 0;
        signalMan_ = (val & 0x40) ? 1 : 0;
        signalPrm_ = (val & 0x01) ? 1 : 0;
        signalPrd_ = (val & 0x02) ? 1 : 0;
        signalOut_ = (val & 0x04) ? 1 : 0;
        signals_ = val;

        return state;
    }
    uint8_t getSignalPusk() const {
        return signalPusk_;
    }
    uint8_t getSignalStop() const {
        return signalStop_;
    }
    uint8_t getSignalMan() const {
        return signalMan_;
    }
    uint8_t getSignalPrm() const {
        return signalPrm_;
    }
    uint8_t getSignalPrd() const {
        return signalPrd_;
    }
    uint8_t getSignalOut() const {
        return signalOut_;
    }
    uint8_t getSignals() const {
        return signals_;
    }

    // ����� �������
    bool setNumCom(uint8_t num) {
        bool stat = false;
        if ((num > 0) && (num <= MAX_NUM_COM_PRD)) {
            numCom_ = num;
            stat = true;
        }
        return stat;
    }
    uint8_t getNumCom() const {
        return numCom_;
    }

    /** ��������� ��������� ������������ �������.
     *  � ������ �� ����������� ��������, � �������� ����� �������� ��������
     *  \a GB_SOURCE_COM_MAX.
     *
     *  @see eGB_SOURCE_COM
     *  @param source �������� ������������ �������.
     *  @return True - � ������ ����������� ��������, False - �����.
     */
    bool setSourceCom(eGB_SOURCE_COM source) {
        bool stat = false;

        switch(source) {
            case GB_SOURCE_COM_DI:
            case GB_SOURCE_COM_DR:
            case GB_SOURCE_COM_MAX: {
                stat = true;
            }
            break;
        }

        if (!stat) {
            source = GB_SOURCE_COM_MAX;
        }

        sourceCom_ = source;
        return stat;
    }

    /** ���������� ��������� ������������ �������.
     *
     *  @return �������� ������������ �������.
     */
    eGB_SOURCE_COM getSourceCom() const {
        return sourceCom_;
    }

    // ���������� ������� � �������
    bool setNumJrnEntry(uint16_t val) {
        bool stat = false;

        overflow_ = (val & 0xC000) != 0;

        val &= 0x3FFF;

        if (val <= maxNumJrnEntry_) {
            if (overflow_) {
                numJrnEntries_ = maxNumJrnEntry_;
                addressFirstEntry_ = val;
            } else {
                numJrnEntries_ = val;
                addressFirstEntry_ = 0;
            }
            stat = true;
        }
        return stat;
    }
    uint16_t getNumJrnEntries() const {
        return numJrnEntries_;
    }

    // ������������ ���-�� ������� � �������
    bool setMaxNumJrnEntries(uint16_t max) {
        bool stat = false;
        if (max <= GLB_JRN_MAX) {
            stat = true;
            maxNumJrnEntry_ = max;
        }
        return stat;
    }
    uint16_t getMaxNumJrnEntry() const {
        return maxNumJrnEntry_;
    }

    // ������������ �������
    bool isOverflow() const {
        return overflow_;
    }

    // ����� ������ ������� ������ � �������
    uint16_t getEntryAdress() const {
        uint16_t address = 0;

        if (numJrnEntries_ > 0) {
            address= (currentEntry_ + addressFirstEntry_ - 1) % numJrnEntries_;
        }

        return address;
    }

    // ������� ������
    uint16_t getCurrentEntry() const {
        return currentEntry_;
    }
    // ���������/���������� ������ ���������� true ���� ����� ��������
    // ���������� �� �����������
    bool setNextEntry() {
        bool stat = false;
        uint16_t tmp = currentEntry_;
        tmp = (tmp < numJrnEntries_) ? currentEntry_ + 1 : 1;
        if (tmp != currentEntry_) {
            currentEntry_ = tmp;
            ready_ = false;
            stat = true;
        }
        return stat;
    }
    bool setPreviousEntry() {
        bool stat = false;
        uint16_t tmp = currentEntry_;
        tmp = (tmp > 1) ? tmp - 1 : numJrnEntries_;
        if (tmp != currentEntry_) {
            currentEntry_ = tmp;
            ready_ = false;
            stat = true;
        }
        return stat;
    }

    // ��������� ������ ������
    bool setNumEntry(uint16_t num) {
        bool stat = false;
        if (num <= numJrnEntries_) {
            currentEntry_ = num;
            ready_ = false;
            stat = true;
        }
        return stat;
    }

    // ���������� � ���������� ����� ��������� ���������� � ������� ������
    bool setReady() {
        return (ready_ = true);
    }
    bool isReady() const {
        return ready_;
    }

private:
    // ������� ������
    eGB_DEVICE currentDevice_;

    // ����������
    eGB_DEVICE_K400 deviceJrn_;

    // ��� �������
    uint8_t eventType_;

    // �������� ������� (����� �������� � �������� ������ �������)
    uint8_t srcCom_;

    // �����
    eGB_REGIME regime_;

    // ������� ��� ������� ������
    bool signalPusk_;
    bool signalStop_;
    bool signalMan_;
    bool signalPrm_;
    bool signalPrd_;
    bool signalOut_;    // ����� ��������� (���2 ��� �����)
    uint8_t signals_;

    // ����� ������ ��� �������� ������
    uint8_t opticEntry_[4];

    // ����� ������ ��� �������� ������ ���
    uint8_t opticEntryDR_[4];

    // ���-�� ������� � ����� ������ ������� ������
    uint8_t numOpticEntries_;

    // ���-�� ������� � ����� ������ ������� ������ ���
    uint8_t numOpticEntriesDR_;

    // ���-�� ������� � �������
    uint16_t numJrnEntries_;

    // ������������ ���-�� ������� � �������
    uint16_t maxNumJrnEntry_;

    // ������������ �������
    bool overflow_;

    // ����� ������ ������
    uint16_t addressFirstEntry_;

    // ����� ������� ������ (������������ �� ������)
    uint16_t currentEntry_;

    // ����� ������� � ������� ������
    uint16_t numCom_;

    // �������� ������������ �������
    eGB_SOURCE_COM sourceCom_;

    // ���� ��������� ���������� � ������� ������
    bool ready_;
};


class TJrnSCADA {
public:

    /// ��������� �������.
    typedef enum __attribute__ ((__packed__)) {
        COM_OFF = 0,    ///< ��������.
        COM_ON,         ///< ��������.
        COM_MAX         ///< ������������ ��������.
    } com_t;

    /// ������� ��������� ������.
    typedef enum __attribute__ ((__packed__)) {
        STATE_IDLE = 0, ///< ��������.
        STATE_REC,      ///< ������� ����� ������ �������.
        STATE_TR_OK,    ///< ������� ������ �������� � SCADA, ������� ����� ������ �������.
        STATE_REC_OK,   ///< ������ ������� ������� � ���� �������� � SCADA.
        STATE_MAX       ///< ������������ ��������.
    } state_t;

    /// ���� �������� �������� ������.
    enum __attribute__ ((__packed__)) defSignal_t {
        DEF_SIGNAL_PUSK = 0,    ///< ����.
        DEF_SIGNAL_STOP,        ///< ����.
        DEF_SIGNAL_MAN,         ///< ���.
        DEF_SIGNAL_PRM,         ///< ���. ���.
        DEF_SIGNAL_PRD,         ///< ���. ���.
        DEF_SIGNAL_OUT,         ///< ����� ���������.
        //
        DEF_SIGNAL_MAX          ///< ���������� �������� � ������.
    };

    TJrnSCADA() {
        m_u8Com = 0;
        m_u8ComSource = 0;
        m_u8Event = 0;
        m_eJrn = GB_DEVICE_K400_MAX;

        m_eState = STATE_IDLE;
        m_step = 0;
    }

    TDateTime dtime;

    /// ��������� �������.
    void setEvent(uint8_t val) {
        m_u8Event= val;
    }

    /// ���������� �������.
    uint8_t getEvent() const {
        return m_u8Event;
    }

    /** ���������� ������� ��� ������� ������.
     *
     *  @param[out] val �������� �������.
     *  @return ������� ��� ������� ������.
     *  @retval 0 ���� ������� ������ ���.
     */
    uint8_t getDefEvent(bool &val) {
        uint8_t ev = 0;

        switch(m_step) {
            case DEF_SIGNAL_PUSK:
                ev = ++m_step;
                val = m_u8Com & 0x01 ? true : false;
                break;
            case DEF_SIGNAL_STOP:
                ev = ++m_step;
                val = m_u8Com & 0x02 ? true : false;
                break;
            case DEF_SIGNAL_MAN:
                ev = ++m_step;
                val = m_u8Com & 0x04 ? true : false;
                break;
            case DEF_SIGNAL_PRM:
                ev = ++m_step;
                val = m_u8ComSource & 0x01 ? true : false;
                break;
            case DEF_SIGNAL_PRD:
                ev = ++m_step;
                val = m_u8ComSource & 0x02 ? true : false;
                break;
            case DEF_SIGNAL_OUT:
                ev = ++m_step;
                val = m_u8ComSource & 0x04 ? true : false;
                break;
        }


        return ev;
    }

    /// ��������� ������ �������.
    void setCom(uint8_t val) {
        m_u8Com = val;
    }

    /// ���������� ����� �������.
    uint8_t getCom() const {
        return m_u8Com;
    }

    /// ��������� ��������� �������
    void setComSource(uint8_t source) {
        m_u8ComSource = source;
    }

    /** ���������� �������� �������
     *
     *  @retval 0 - ���������� ����.
     *  @retval 1 - �������� ���������.
     */
    uint8_t getComSource() const {
        return m_u8ComSource;
    }

    /// ��������� �������� �������.
    void setJrn(uint8_t val) {
        if ((val >= GB_DEVICE_K400_MIN) && (val <GB_DEVICE_K400_MAX)) {
            m_eJrn = static_cast<eGB_DEVICE_K400> (val);
        } else {
            m_eJrn = GB_DEVICE_K400_MAX;
        }
        m_step = 0;
    }

    /// �������� �������� ������� �� ������ ������.
    bool isJrnDef() const {
        return (m_eJrn == GB_DEVICE_K400_DEF);
    }

    /// �������� �������� ������� �� ������ �������.
    bool isJrnEvent() const {
        return (m_eJrn == GB_DEVICE_K400_GLB);
    }

    /// �������� �������� ������� �� ������ ���������.
    bool isJrnPrm() const {
        // TODO �������� �� ��� ��������� ������� ���������
        return (m_eJrn == GB_DEVICE_K400_PRM1);
    }

    /// �������� �������� ������� �� ������ �����������.
    bool isJrnPrd() const {
        return (m_eJrn == GB_DEVICE_K400_PRD);
    }

    /// ��������� �������� ��������� � ������� ����� ������.
    void setReadyToSend() {
        m_eState = STATE_REC_OK;
    }

    /// ���������� �������� ��������� � ���������� � ����� ������.
    void setReadyToRead() {
        m_eState = STATE_REC;
    }

    /// ��������� �������� ��������� � ���������� � ����� ������ + ���������� ���� ��������.
    void setReadyToEvent() {
        m_eState = STATE_TR_OK;
    }

    /// ��������� �������� ��������� � ��������.
    void setIdle() {
        m_eState = STATE_IDLE;
    }

    /// �������� �������� ��������� �� ������ ������ ���������.
    bool isReadyToWrite() const {
        return ((m_eState == STATE_REC) || (m_eState == STATE_TR_OK));
    }

    /** �������� �������� ��������� �� ������� ������ ���������.
     *
     *  � ������ ������, ��������� ����� ����������� � �������� ������ �������.
     *
     *  @return true - ������� ����� ���������.
     */
    bool isReadyToSend() {
        if (m_eState >= STATE_MAX) {
            m_eState = STATE_REC;
        }

        return (m_eState == STATE_REC_OK);
    }

    /** ���������� ������� ���������.
     *
     *  @retval STATE_IDLE
     *  @retval STATE_REC
     *  @retval STATE_TR_OK
     */
    uint8_t getState() const {
        return (m_eState >= STATE_REC_OK) ? STATE_IDLE : m_eState;
    }

private:
    eGB_DEVICE_K400 m_eJrn; /// ������.
    uint8_t m_u8Event;      /// C������.
    uint8_t m_u8Com;        /// ����� �������.
    uint8_t m_u8ComSource;  /// �������� ������� (0 - ��, 1 - ���).

    state_t m_eState;       /// ������� ���������.
    uint8_t m_step;         /// ������� ������.
};

#endif /* GLBDEF_H_ */
