#ifndef __GLB_TYPES_H__
#define __GLB_TYPES_H__

// TODO Вынести STRING_LENGTH в соответствующий ему header
// длина половины строки (+1 - символ конца строки)
#define STRING_LENGHT (11 + 1)

/// Совместимость Р400м
enum eGB_COMP_R400M
{
    GB_COMP_R400M_MIN    = 0,
    GB_COMP_R400M_AVANT  = 0,
    GB_COMP_R400M_PVZ90  = 1,
    GB_COMP_R400M_AVZK80 = 2,
    GB_COMP_R400M_PVZUE  = 3,
    GB_COMP_R400M_PVZL   = 4,
    GB_COMP_R400M_LINER  = 5,
    GB_COMP_R400M_PVZK   = 6,  // TODO задел на будущее
    GB_COMP_R400M_PVZU   = 7,
    GB_COMP_R400M_PVZ    = 8,
    GB_COMP_R400M_P400   = 9,
    GB_COMP_R400M_MAX
};

#endif  // __GLB_TYPES_H__
