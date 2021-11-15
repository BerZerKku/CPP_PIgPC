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
    static const uint8_t MAX_NUM_PUNKTS = 20;

public:
    /**	�����������.
     */
    TMenuPunkt() { clear(); }

    /**	���������� ������ ����, ����� ��������� �� ���.
     *
     * 	@param name ��� ������.
     * 	@param com ������� ��� ������� �� ���. �� ��������� - ���.
     * 	@retval True � ������ ��������� ����������.
     * 	@retval False � ������ ������ ����������. ��������, ���������� ������.
     */
    bool addName(PGM_P const name)
    {
        bool stat = false;
        if (cnt_ < MAX_NUM_PUNKTS)
        {
            name_[cnt_] = name;
            cnt_++;
            stat = true;
        }
        return stat;
    }

    /**	���������� ������ ����, ����� ��������.
     *
     * 	@param name ��� ������.
     * 	@param com ������� ��� ������� �� ���. �� ��������� - ���.
     * 	@retval True � ������ ��������� ����������.
     * 	@retval False � ������ ������ ����������. ��������, ���������� ������.
     */
    bool addNumber(uint8_t number)
    {
        bool stat = false;
        if (cnt_ < MAX_NUM_PUNKTS)
        {
            number_[cnt_] = number;
            cnt_++;
            stat = true;
        }
        return stat;
    }

    /** ������� �������� ������ ������� ����.
     */
    void clear() { cnt_ = 0; }

    /** ����� ������ �� ��������� ������� ������ ����.
     *
     *  @param[in] first ������ �����.
     *  @param[in] second ������ �����.
     *  @param[in] is_first ���� true, �� ���� ������� ������ �����, ����� ������.
     *  @param[in] start ��������� ������� ��� ������.
     *  @return ������� �� ������� ��� ������ ��������.
     */
    uint8_t choose(PGM_P const first, PGM_P const second, bool is_first, uint8_t start = 0)
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

    /** ���������� �������� ��������� ������ ����.
     *
     *	@param num	����� ������.
     * 	@return �������� ������.
     */
    PGM_P getName(uint8_t num)
    {
        static char unknown[] PROGMEM = "?";
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
    PGM_P name_[MAX_NUM_PUNKTS];

    /// ����� ������, ������������ � ���������
    uint8_t number_[MAX_NUM_PUNKTS];
};


#endif /* MENUPUNKT_H_ */
