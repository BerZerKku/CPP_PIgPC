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

    /** ������ ���������� ������ ������ ����.
     *
     * 	@param name ��� �����.
     * 	@param num ����� ������.
     * 	@retval True � ������ ��������� ����������.
     * 	@retval False � ������ ������ ����������. ��������, �� ������ �����.
     */
    bool change(PGM_P const name, uint8_t num)
    {
        bool stat = false;
        if (num < cnt_)
        {
            name_[num] = name;
            stat       = true;
        }
        return stat;
    }

    /** ����� ������ �� ��������� ������� ������ ����.
     *
     *  @param[in] first ������ �����.
     *  @param[in] second ������ �����.
     *  @param[in] is_first ���� true, �� ���� ������� ������ �����, ����� ������.
     *  @return true ���� ����� ��� ������ ��� ������, ����� false.
     */
    bool choose(PGM_P const first, PGM_P const second, bool is_first)
    {
        for (uint8_t i = 0; i < cnt_; i++)
        {
            if ((name_[i] == first) || (name_[i] == second))
            {
                name_[i] = is_first ? first : second;
                return true;
            }
        }

        return false;
    }

    /** ������ ���������� ������ ������ ����.
     *
     * 	@param name ��� ������.
     * 	@param num ����� ������.
     * 	@retval True � ������ ��������� ����������.
     * 	@retval False � ������ ������ ����������. ��������, �� ������ �����.
     */
    bool changeName(PGM_P const dest, PGM_P const src)
    {
        for (uint8_t i = 0; i < cnt_; i++)
        {
            if (name_[i] == dest)
            {
                name_[i] = src;
                return true;
            }
        }
        return false;
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
