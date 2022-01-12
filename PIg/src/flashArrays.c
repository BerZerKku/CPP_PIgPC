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
const char fcCompR400m[][STRING_LENGHT] PROGMEM = {
    //                        1234567890
    [GB_COMP_R400M_AVANT]  = "����� �400",  //
    [GB_COMP_R400M_PVZ90]  = "���-90",      //
    [GB_COMP_R400M_AVZK80] = "����-80",     //
    [GB_COMP_R400M_PVZUE]  = "����-�",      //
    [GB_COMP_R400M_PVZL]   = "����",        //
    [GB_COMP_R400M_LINER]  = "�����-�",     //
    [GB_COMP_R400M_PVZU]   = "����",        //
    [GB_COMP_R400M_PVZ]    = "���",         //
    [GB_COMP_R400M_P400]   = "�400",        //
};

/// ��� ������������� (�������� �400). ������ � eGB_COMP_K400.
const char fcCompK400[][STRING_LENGHT] PROGMEM = {
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

/// ��� ������������� (�������� ����). ������ � eGB_COMP_RZSK.
const char fcCompRZSK[][STRING_LENGHT] PROGMEM = {
    //                    1234567890
    [GB_COMP_RZSK]     = "����",      //
    [GB_COMP_RZSK_M]   = "�����",     //
    [GB_COMP_RZSK_3E8] = "����-3 8�"  //
};
