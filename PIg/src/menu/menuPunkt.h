/*
 * menuPunkt.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef MENUPUNKT_H_
#define MENUPUNKT_H_

#include "glbDefine.h"

/// ��������� ������ ����
class TMenuPunkt
{
    /// ������������ ���-�� ������� � ����
    static const uint8_t kSize = 20;

public:
    TMenuPunkt() { clear(); }

    bool    addName(PGM_P const name, uint8_t number = 0);
    bool    addNumber(uint8_t number);
    uint8_t choose(PGM_P const first, PGM_P const second, bool is_first, uint8_t start = 0);


    /** ������� �������� ������ ������� ����.
     */
    void clear() { cnt_ = 0; }

    /** ���������� �������� ��������� ������ ����.
     *
     *	@param num	����� ������.
     * 	@return �������� ������.
     */
    PGM_P getName(uint8_t num)
    {
        static const char unknown[] PROGMEM = "?";
        return ((num < cnt_) ? name_[num] : unknown);
    }

    /**	���������� ����� ��������� ������� � ������ ���������� ������ ����.
     *
     *	@param num	����� ������.
     * 	@return �������� ������.
     */
    uint8_t getNumber(uint8_t num) { return ((num < cnt_) ? number_[num] : 0); }

    /** ���������� ������� ���-�� ������ ����.
     *
     * 	@return ������� ���-�� ������� ����.
     * 	@retval 0 - �����.
     */
    uint8_t getMaxNumPunkts() { return cnt_; }

private:
    /// ������� ���-�� �������
    uint8_t cnt_;

    /// ��������� �� ��� ������
    PGM_P name_[kSize];

    /// ����� ������, ������������ � ���������
    uint8_t number_[kSize];
};


/**
 * *****************************************************************************
 *
 * ���������� ������ ����, ����� ��������� �� ���.
 *
 * @param[in] name ��� ������.
 * @param[in] number ����� ������ ��� �������������� ��������.
 * @return ��������� ����������.
 * @retval True � ������ ��������� ����������.
 * @retval False � ������ ������ ����������. ��������, ���������� ������.
 *
 * *****************************************************************************
 */
inline bool TMenuPunkt::addName(PGM_P const name, uint8_t number)
{
    bool stat = false;
    if (cnt_ < kSize)
    {
        name_[cnt_]   = name;
        number_[cnt_] = number;
        cnt_++;
        stat = true;
    }
    return stat;
}


/**
 * *****************************************************************************
 *
 * ���������� ������ ����, ����� ��������.
 *
 * @param[in] number ����� ������.
 * @return ��������� ����������.
 * @retval True � ������ ��������� ����������.
 * @retval False � ������ ������ ����������. ��������, ���������� ������.
 *
 * *****************************************************************************
 */
inline bool TMenuPunkt::addNumber(uint8_t number)
{
    bool stat = false;

    if (cnt_ < kSize)
    {
        name_[cnt_]   = nullptr;
        number_[cnt_] = number;
        cnt_++;
        stat = true;
    }

    return stat;
}


/**
 * *****************************************************************************
 *
 * ����� ������ �� ��������� ������� ������ ����.
 *
 * @param[in] first ������ �����.
 * @param[in] second ������ �����.
 * @param[in] is_first ���� true, �� ���� ������� ������ �����, ����� ������.
 * @param[in] start ��������� ������� ��� ������.
 * @return ������� �� ������� ��� ������ ��������.
 *
 * *****************************************************************************
 */
inline uint8_t
TMenuPunkt::choose(PGM_P const first, PGM_P const second, bool is_first, uint8_t start)
{
    for (uint8_t i = start; i < cnt_; i++)
    {
        if ((name_[i] == first) || (name_[i] == second))
        {
            name_[i] = (is_first) ? (first) : (second);
            return i;
        }
    }

    return 0;
}

#endif /* MENUPUNKT_H_ */
