#include "flashArrays.h"

/// Тип совместимости Р400м (параметр Защиты)
const char fcCompR400m[][STRING_LENGHT] PROGMEM = {
    //                        1234567890
    [GB_COMP_R400M_AVANT]  = "АВАНТ Р400",  //
    [GB_COMP_R400M_PVZ90]  = "ПВЗ-90",      //
    [GB_COMP_R400M_AVZK80] = "АВЗК-80",     //
    [GB_COMP_R400M_PVZUE]  = "ПВЗУ-Е",      //
    [GB_COMP_R400M_PVZL]   = "ПВЗЛ",        //
    [GB_COMP_R400M_LINER]  = "ЛИНИЯ-Р",     //
    [GB_COMP_R400M_PVZU]   = "ПВЗУ",        //
    [GB_COMP_R400M_PVZ]    = "ПВЗ",         //
    [GB_COMP_R400M_P400]   = "Р400",        //
};
