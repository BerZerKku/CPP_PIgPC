/**
 * *****************************************************************************
 *
 *  @file flashArrays.cpp
 *
 *  VERSION_1v52:
 *      - �����.
 *
 * *****************************************************************************
 */

#include "flashArrays.h"

/// ��� ������������� �400� (�������� ������)
const char fcCompR400m[][VALUE_STRING_SIZE] PROGMEM = {
    //                        1234567890
    [GB_COMP_R400M_AVANT]  = "�400�",    //
    [GB_COMP_R400M_PVZ90]  = "���-90",   //
    [GB_COMP_R400M_AVZK80] = "����-80",  //
    [GB_COMP_R400M_PVZUE]  = "����-�",   //
    [GB_COMP_R400M_PVZL]   = "����",     //
    [GB_COMP_R400M_PVZK]   = "����",     //
    [GB_COMP_R400M_LINER]  = "�����-�",  //
    [GB_COMP_R400M_PVZU]   = "����",     //
    [GB_COMP_R400M_PVZ]    = "���",      //
    [GB_COMP_R400M_R400]   = "�400",     //
};

/// ��� ������������� (�������� �400)
const char fcCompK400[][VALUE_STRING_SIZE] PROGMEM = {
    //                          1234567890
    [GB_COMP_K400_AVANT]     = "�����",      //
    [GB_COMP_K400_AVANT_PRD] = "����� ���",  //
    [GB_COMP_K400_AVANT_PRM] = "����� ���",  //
    [GB_COMP_K400_AKPA_PRD]  = "���� ���",   //
    [GB_COMP_K400_AKPA_PRM]  = "���� ���",   //
    [GB_COMP_K400_KEDR_PRD]  = "���� ���",   //
    [GB_COMP_K400_KEDR_PRM]  = "���� ���",   //
    [GB_COMP_K400_UPKC_PRD]  = "���-� ���",  //
    [GB_COMP_K400_UPKC_PRM]  = "���-� ���",  //
    [GB_COMP_K400_VCTO_PRD]  = "���� ���",   //
    [GB_COMP_K400_VCTO_PRM]  = "���� ���",   //
    [GB_COMP_K400_ANKA_PRD]  = "���� ���",   //
    [GB_COMP_K400_ANKA_PRM]  = "���� ���"    //
};

/// ��� ������������� (�������� ����)
const char fcCompRZSK[][VALUE_STRING_SIZE] PROGMEM = {
    //                    1234567890
    [GB_COMP_RZSK]     = "����",       //
    [GB_COMP_RZSK_M]   = "�����",      //
    [GB_COMP_RZSK_2E5] = "����-2 5�",  //
    [GB_COMP_RZSK_3E8] = "����-3 8�"   //
};

static const char fcEventRemote000[] PROGMEM = "";
static const char fcEventRemote001[] PROGMEM = "1";
static const char fcEventRemote010[] PROGMEM = "2";
static const char fcEventRemote011[] PROGMEM = "1,2";
static const char fcEventRemote100[] PROGMEM = "3";
static const char fcEventRemote101[] PROGMEM = "1,3";
static const char fcEventRemote110[] PROGMEM = "2,3";

/// ������ ��������� ��������� ��� ������� �������
PGM_P const fcEventRemote[] PROGMEM = {
    fcEventRemote000,  ///< 000
    fcEventRemote001,  ///< 001
    fcEventRemote010,  ///< 010
    fcEventRemote011,  ///< 011
    fcEventRemote100,  ///< 100
    fcEventRemote101,  ///< 101
    fcEventRemote110   ///< 110
};
