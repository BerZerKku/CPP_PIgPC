#ifndef GLB_TYPES_H
#define GLB_TYPES_H

// TODO ������� STRING_LENGTH � ��������������� ��� header
// ����� �������� ������ (+1 - ������ ����� ������)
#define VALUE_STRING_SIZE (11 + 1)

// ������������ �������� "������ ���������" � ������� �������
#define EVENT_REMOTES_MAX 6

/** ������������� �400�
 *
 *  VERSION_1v52:
 *  - ��������� ������������� PVZU, PVZ, P400.
 */
enum eGB_COMP_R400M
{
    GB_COMP_R400M_MIN   = 0,
    GB_COMP_R400M_AVANT = GB_COMP_R400M_MIN,
    GB_COMP_R400M_PVZ90,
    GB_COMP_R400M_AVZK80,
    GB_COMP_R400M_PVZUE,
    GB_COMP_R400M_PVZL,
    GB_COMP_R400M_LINER,
    GB_COMP_R400M_PVZK,  // TODO ����� �� �������
    GB_COMP_R400M_PVZU,
    GB_COMP_R400M_PVZ,
    GB_COMP_R400M_R400,
    GB_COMP_R400M_MAX
};

/// ������������� ��� �400
enum eGB_COMP_K400
{
    GB_COMP_K400_MIN   = 0,
    GB_COMP_K400_AVANT = GB_COMP_K400_MIN,
    GB_COMP_K400_AVANT_PRD,
    GB_COMP_K400_AVANT_PRM,
    GB_COMP_K400_AKPA_PRD,
    GB_COMP_K400_AKPA_PRM,
    GB_COMP_K400_KEDR_PRD,
    GB_COMP_K400_KEDR_PRM,
    GB_COMP_K400_UPKC_PRD,
    GB_COMP_K400_UPKC_PRM,
    GB_COMP_K400_VCTO_PRD,
    GB_COMP_K400_VCTO_PRM,
    GB_COMP_K400_ANKA_PRD,  ///< ����-����
    GB_COMP_K400_ANKA_PRM,  ///< ����-����
    GB_COMP_K400_MAX
};

/// ������������� ��� ����
enum eGB_COMP_RZSK
{
    GB_COMP_RZSK_MIN = 0,
    GB_COMP_RZSK     = GB_COMP_RZSK_MIN,
    GB_COMP_RZSK_M,
    GB_COMP_RZSK_3E8,
    //
    GB_COMP_RZSK_MAX
};

#endif  // GLB_TYPES_H
