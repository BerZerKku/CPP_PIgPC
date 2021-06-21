/*
 * flashParams.h
 *
 *  Created on: 27.03.2015
 *      Author: Shcheblykin
 */

#ifndef FLASHPARAMS_H_
#define FLASHPARAMS_H_

#include "glbDefine.h"
#include "parameter/param.h"
#include "paramDef.h"
#include "paramUart.h"
#include "paramGlb.h"

extern size_t getSizeOfParam();

/// ������ ����� ������������, ������ � Param::DIMENSION
static const char fcDimension[] [5] PROGMEM = {
    "",     // Param::DIM_NO
    "��",   // Param::DIM_MA
    "�",    // Param::DIM_A
    "��",   // Param::DIM_KA
    "�",    // Param::DIM_V
    "��",   // Param::DIM_KV
    "��",   // Param::MSEC
    "c",    // Param::DIM_SEC
    "���",  // Param::DIM_HOUR
    "��",   // Param::DIM_DB
    "��",   // Param::DIM_HZ
    "���",  // Param::DIM_KHZ
    "����"  // Param::DIM_DEG
};

/// ��������� �������� ���������.
static const char fcOnOff[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "����.",        ///< 0
    "���."          ///< 1
};

/// ��� ������ (�������� ������).
static const char fcDefType[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "���-����",     ///< 0
    "���-��1",      ///< 1
    "���-��2",      ///< 2
    "���-L60",      ///< 3
    "��-����",      ///< 4
    "��-��",        ///< 5
    "���-����",     ///< 6
    "���-��"        ///< 7
};

/// ��� ����� (�������� �����).
static const char fcNumDevices[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "2 ��������",   ///< 1
    "3 ��������"    ///< 2
};

/// ��� ��������� (�������� �����).
static const char fcPrmType[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "���+����",     ///< 0
    "��������",     ///< 1
    "���������"     ///< 2
};

/// ��� ������������� (�������� ������). ������ � eGB_COMPATIBILITY.
static const char fcCompatibility[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "����� �400",   ///< 0
    "���-90",       ///< 1
    "����-80",      ///< 2
    "����-�",       ///< 3
    "����",         ///< 4
    "�����-�",      ///< 5
    "����"          ///< 6
};

/// ��� ������������� (�������� �400). ������ � eGB_COMP_K400.
static const char fcCompK400[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "�����",        ///< 0
    "����� ���",    ///< 1
    "����� ���",    ///< 2
    "���� ���",     ///< 3
    "���� ���",     ///< 4
    "���� ���",     ///< 5
    "���� ���",     ///< 6
    "���-� ���",    ///< 7
    "���-� ���",    ///< 8
    "���� ���",     ///< 9
    "���� ���",     ///< 10
    "���� ���",     ///< 11
    "���� ���"      ///< 12
};

/// ��� ������������� (�������� ����). ������ � eGB_COMP_RZSK.
static const char fcCompRZSK[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "����",         ///< 0
    "�����",        ///< 1
    "����-3 8�"     ///< 2
};

/// ������� ��� / ��� (�������� ������).
static const char fcPvzlFreq[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "-1000��",      ///< 0
    "-500��",       ///< 1
    "0��",          ///< 2
    "+500��",       ///< 3
    "+1000��"       ///< 4
};

/// �������� ������ (�������� �����).
static const char fcPvzueProtocol[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "�������",      ///< 1
    "���������"     ///< 2
};

/// ��� ����������� (�������� �����).
static char fcPvzueTypeAC[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "���������",    ///< 1
    "����������",   ///< 2
    "���������"     ///< 3
};

/// ������� �������� (�������� �����).
static char fcPvzueParity[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "���.",         ///< 1
    "����."         ///< 2
};

/// ��������� ����� (�������� ���������).
static char fcInterface[TInterface::MAX][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "USB",          ///< TInterface::USB
    "RS485"         ///< TInterface::RS485
};

// �������� (�������� ���������).
static char fcProtocol[TProtocol::MAX ][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "��������",     ///< TProtocol::STANDART
    "MODBUS",       ///< TProtocol::MODBUS
    "���-101"       ///< TProtocol::IEC_101
};

// ��������, ���/� (�������� ���������).
static char fcBaudRate[TBaudRate::MAX][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "600",          ///< TBaudRate::_600
    "1200",         ///< TBaudRate::_1200
    "2400",         ///< TBaudRate::_2400
    "4800",         ///< TBaudRate::_4800
    "9600",         ///< TBaudRate::_9600
    "19200"         ///< TBaudRate::_19200
};

/// ���������� ��� ������ (�������� ���������).
static char fcDataBits[TDataBits::MAX][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "8"             ///< TDataBits::_8
};

/// �������� (�������� ���������).
static char fcParity[TParity::MAX][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "���",          ///< TParity::NONE
    "���",          ///< TParity::EVEN
    "�����"         ///< TParity::ODD
};

/// �������� ���� (�������� ���������).
static char fcStopBits[TStopBits::MAX][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "1",            ///< TStopBits::ONE
    "2"             ///< TStopBits::TWO
};

/// �������� ��
static char fcTMSpeed[][STRING_LENGHT] PROGMEM = {
    //       1234567890
    "200",
    "400"
};

/// ������� ��
static char fcCF[][STRING_LENGHT] PROGMEM = {
    "��1",
    "��2"
};

/// �������������� ������ � ������
static char fcRingRenumber[][STRING_LENGHT] PROGMEM = {
    "0",
    "1A","2A","3A","4A","5A","6A","7A","8A","9A","10A",
    "11A","12A","13A","14A","15A","16A","17A","18A","19A","20A",
    "21A","22A","23A","24A","25A","26A","27A","28A","29A","30A","31A","32A",
    "1B","2B","3B","4B","5B","6B","7B","8B","9B","10B",
    "11B","12B","13B","14B","15B","16B","17B","18B","19B","20B",
    "21B","22B","23B","24B","25B","26B","27B","28B","29B","30B","31B","32B",
    "1C","2C","3C","4C","5C","6C","7C","8C","9C","10C",
    "11C","12C","13C","14C","15C","16C","17C","18C","19C","20C",
    "21C","22C","23C","24C","25C","26C","27C","28C","29C","30C","31C","32C"
};

// ������������� �������
static char fcTymeSynchSrc[][STRING_LENGHT] PROGMEM = {
    //   1234567890
    "����.",
    "����� ��.",
    "��� ��"
};

/// ������� � ������ ���/���.
static const char fcTest1K400[GB_SIGNAL_MAX + 1][STRING_LENGHT] PROGMEM = {
    //234567890
    "����.",        ///<
    "��",           ///<
    "��1",          ///<
    "��2",          ///<
    "��3",          ///<
    "��4",          ///<
    "��1",          ///< GB_SIGNAL_CF_NO_RZ
    "��1 � ����",   ///< GB_SIGNAL_CF_RZ
    "��2",          ///< GB_SIGNAL_CF2_NO_RZ
    "��2 � ����",   ///< GB_SIGNAL_CF2_RZ
    "��",           ///<
    "�������1",     ///< GB_SIGNAL_COM1_NO_RZ
    "�������2",     ///< GB_SIGNAL_COM2_NO_RZ
    "�������3",     ///< GB_SIGNAL_COM3_NO_RZ
    "�������4",     ///< GB_SIGNAL_COM4_NO_RZ
    "�������5",     ///< GB_SIGNAL_COM5_NO_RZ
    "�������6",     ///< GB_SIGNAL_COM6_NO_RZ
    "�������7",     ///< GB_SIGNAL_COM7_NO_RZ
    "�������8",     ///< GB_SIGNAL_COM8_NO_RZ
    "���1+����",    ///< GB_SIGNAL_COM1_RZ
    "���2+����",    ///< GB_SIGNAL_COM2_RZ
    "���3+����",    ///< GB_SIGNAL_COM3_RZ
    "���4+����",    ///< GB_SIGNAL_COM4_RZ
    "���5+����",    ///< GB_SIGNAL_COM5_RZ
    "���6+����",    ///< GB_SIGNAL_COM6_RZ
    "���7+����",    ///< GB_SIGNAL_COM7_RZ
    "���8+����",    ///< GB_SIGNAL_COM8_RZ
    "�������1",     ///< GB_SIGNAL_COM1
    "�������2",
    "�������3",
    "�������4",
    "�������5",
    "�������6",
    "�������7",
    "�������8",
    "�������9",
    "�������10",
    "�������11",
    "�������12",
    "�������13",
    "�������14",
    "�������15",
    "�������16",
    "�������17",
    "�������18",
    "�������19",
    "�������20",
    "�������21",
    "�������22",
    "�������23",
    "�������24",
    "�������25",
    "�������26",
    "�������27",
    "�������28",
    "�������29",
    "�������30",
    "�������31",
    "�������32",
    "�� � ��",      ///< GB_SIGNAL_CF_RZ_R400M
    "��1 � ��",     ///< GB_SIGNAL_CF1_RZ_R400M
    "��2 � ��",     ///< GB_SIGNAL_CF2_RZ_R400M
    "��3 � ��",     ///< GB_SIGNAL_CF3_RZ_R400M
    "��4 � ��",     ///< GB_SIGNAL_CF4_RZ_R400M
    "��",           ///< � ������ ������ ��
    "�������1A",    ///< ������ ���������������� vvv
    "�������2A",
    "�������3A",
    "�������4A",
    "�������5A",
    "�������6A",
    "�������7A",
    "�������8A",
    "�������9A",
    "�������10A",
    "�������11A",
    "�������12A",
    "�������13A",
    "�������14A",
    "�������15A",
    "�������16A",
    "�������17A",
    "�������18A",
    "�������19A",
    "�������20A",
    "�������21A",
    "�������22A",
    "�������23A",
    "�������24A",
    "�������25A",
    "�������26A",
    "�������27A",
    "�������28A",
    "�������29A",
    "�������30A",
    "�������31A",
    "�������32A",
    "�������1B",
    "�������2B",
    "�������3B",
    "�������4B",
    "�������5B",
    "�������6B",
    "�������7B",
    "�������8B",
    "�������9B",
    "�������10B",
    "�������11B",
    "�������12B",
    "�������13B",
    "�������14B",
    "�������15B",
    "�������16B",
    "�������17B",
    "�������18B",
    "�������19B",
    "�������20B",
    "�������21B",
    "�������22B",
    "�������23B",
    "�������24B",
    "�������25B",
    "�������26B",
    "�������27B",
    "�������28B",
    "�������29B",
    "�������30B",
    "�������31B",
    "�������32B",
    "�������1C",
    "�������2C",
    "�������3C",
    "�������4C",
    "�������5C",
    "�������6C",
    "�������7C",
    "�������8C",
    "�������9C",
    "�������10C",
    "�������11C",
    "�������12C",
    "�������13C",
    "�������14C",
    "�������15C",
    "�������16C",
    "�������17C",
    "�������18C",
    "�������19C",
    "�������20C",
    "�������21C",
    "�������22C",
    "�������23C",
    "�������24C",
    "�������25C",
    "�������26C",
    "�������27C",
    "�������28C",
    "�������29C",
    "�������30C",
    "�������31C",
    "�������32C",   ///< ������ ���������������� ^^^
    "������"        ///< ��������� ��������
};

#endif /* FLASHPARAMS_H_ */
