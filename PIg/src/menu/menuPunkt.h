/*
 * menuPunkt.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef MENUPUNKT_H
#define MENUPUNKT_H

#include "glbDefine.h"

/// ��������� ������ ����
class TMenuPunkt
{
    /// ������������ ���-�� ������� � ����
    static const uint8_t kSize = 20;

    struct punkt_t
    {
        PGM_P   name;
        uint8_t data;
    };

public:
    TMenuPunkt() { Clear(); }

    bool Add(PGM_P const name, uint8_t data = 0);
    void Change(uint8_t index, PGM_P const name, uint8_t data = 0);

    /**
     * *************************************************************************
     *
     *  @brief ������� ������ ������� ����.
     *
     * *************************************************************************
     */
    void Clear() { mLen = 0; }

    /**
     * *************************************************************************
     *
     * @brief ���������� �������� ��������� ������ ����.
     *
     * @param[in] index ������ ������ ����.
     * @return �������� ������.
     *
     * *************************************************************************
     */
    PGM_P GetName(uint8_t index) { return ((index < mLen) ? mPunkts[index].name : nullptr); }

    /**
     * *************************************************************************
     *
     * @brief ���������� ������ ��� ���������� ������ ����.
     *
     * @param[in] index ������ ������ ����.
     * @return ������.
     *
     * *************************************************************************
     */
    uint8_t GetData(uint8_t index) { return ((index < mLen) ? mPunkts[index].data : 0); }

    /**
     *  *************************************************************************
     *
     * @brief ���������� ������� ���������� ������ ����.
     *
     * @return ������� ���-�� ������� ����.
     * @retval 0 - �����.
     *
     * *************************************************************************
     */
    uint8_t GetLen() { return mLen; }

    /**
     * *************************************************************************
     *
     * @brief ���������� ������������ ���������� ������� ����.
     * @return
     *
     * *************************************************************************
     */
    uint8_t GetSize() { return kSize; }

private:
    punkt_t mPunkts[kSize];  ///< ������ ����.
    uint8_t mLen;            ///< ���������� ������� ����.
};


/**
 * *****************************************************************************
 *
 * ���������� ������ ����, ����� ��������� �� ���.
 *
 * @param[in] name �������� ������.
 * @param[in] data ����� ������ ��� �������������� ��������.
 * @return ��������� ����������.
 * @retval True � ������ ��������� ����������.
 * @retval False � ������ ������ ����������. ��������, ���������� ������.
 *
 * *****************************************************************************
 */
inline bool TMenuPunkt::Add(PGM_P const name, uint8_t data)
{
    bool stat = false;
    if (mLen < kSize)
    {
        mPunkts[mLen].name = name;
        mPunkts[mLen].data = data;

        mLen++;
        stat = true;
    }
    return stat;
}


/**
 * *****************************************************************************
 *
 * @brief �������� ��������� ����� ����.
 * @param[in] index ������ ����� ����.
 * @param[in] name �������� ������.
 * @param[in] data ����� ������ ��� �������������� ��������.
 *
 * *****************************************************************************
 */
inline void TMenuPunkt::Change(uint8_t index, const char* const name, uint8_t data)
{
    if (index < mLen)
    {
        mPunkts[index].name = name;
        mPunkts[index].data = data;
    }
}

#endif /* MENUPUNKT_H */
