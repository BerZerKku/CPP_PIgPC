/**
 * *****************************************************************************
 *
 *  @file flashArrays.cpp
 *
 *  VERSION_1v52:
 *      - Вновь.
 *
 * *****************************************************************************
 */

#include "flashArrays.h"

/// Тип совместимости Р400м (параметр Защиты)
const char fcCompR400m[][VALUE_STRING_SIZE] PROGMEM = {
    //                        1234567890
    [GB_COMP_R400M_AVANT]  = "АВАНТ Р400",  //
    [GB_COMP_R400M_PVZ90]  = "ПВЗ-90",      //
    [GB_COMP_R400M_AVZK80] = "АВЗК-80",     //
    [GB_COMP_R400M_PVZUE]  = "ПВЗУ-Е",      //
    [GB_COMP_R400M_PVZL]   = "ПВЗЛ",        //
    [GB_COMP_R400M_PVZK]   = "ПВЗК",        //
    [GB_COMP_R400M_LINER]  = "ЛИНИЯ-Р",     //
    [GB_COMP_R400M_PVZU]   = "ПВЗУ",        //
    [GB_COMP_R400M_PVZ]    = "ПВЗ",         //
    [GB_COMP_R400M_R400]   = "Р400",        //
};

/// Тип совместимости (параметр К400)
const char fcCompK400[][VALUE_STRING_SIZE] PROGMEM = {
    //                          1234567890
    [GB_COMP_K400_AVANT]     = "АВАНТ",      //
    [GB_COMP_K400_AVANT_PRD] = "АВАНТ ПРД",  //
    [GB_COMP_K400_AVANT_PRM] = "АВАНТ ПРМ",  //
    [GB_COMP_K400_AKPA_PRD]  = "АКПА ПРД",   //
    [GB_COMP_K400_AKPA_PRM]  = "АКПА ПРМ",   //
    [GB_COMP_K400_KEDR_PRD]  = "КЕДР ПРД",   //
    [GB_COMP_K400_KEDR_PRM]  = "КЕДР ПРМ",   //
    [GB_COMP_K400_UPKC_PRD]  = "УПК-Ц ПРД",  //
    [GB_COMP_K400_UPKC_PRM]  = "УПК-Ц ПРМ",  //
    [GB_COMP_K400_VCTO_PRD]  = "ВЧТО ПРД",   //
    [GB_COMP_K400_VCTO_PRM]  = "ВЧТО ПРМ",   //
    [GB_COMP_K400_ANKA_PRD]  = "АНКА ПРД",   //
    [GB_COMP_K400_ANKA_PRM]  = "АНКА ПРМ"    //
};

/// Тип совместимости (параметр РЗСК)
const char fcCompRZSK[][VALUE_STRING_SIZE] PROGMEM = {
    //                    1234567890
    [GB_COMP_RZSK]     = "РЗСК",      //
    [GB_COMP_RZSK_M]   = "РЗСКм",     //
    [GB_COMP_RZSK_3E8] = "РЗСК-3 8к"  //
};
