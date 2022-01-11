#ifndef __GLB_TYPES_H__
#define __GLB_TYPES_H__

// TODO ������� STRING_LENGTH � ��������������� ��� header
// ����� �������� ������ (+1 - ������ ����� ������)
#define STRING_LENGHT (11 + 1)

/// ������������� �400�
enum eGB_COMP_R400M
{
    GB_COMP_R400M_MIN    = 0,
    GB_COMP_R400M_AVANT  = 0,
    GB_COMP_R400M_PVZ90  = 1,
    GB_COMP_R400M_AVZK80 = 2,
    GB_COMP_R400M_PVZUE  = 3,
    GB_COMP_R400M_PVZL   = 4,
    GB_COMP_R400M_LINER  = 5,
    GB_COMP_R400M_PVZK   = 6,  // TODO ����� �� �������
    GB_COMP_R400M_PVZU   = 7,
    GB_COMP_R400M_PVZ    = 8,
    GB_COMP_R400M_P400   = 9,
    GB_COMP_R400M_MAX
};

#endif  // __GLB_TYPES_H__
