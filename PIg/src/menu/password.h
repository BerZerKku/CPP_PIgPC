/*
 * password.h
 *
 *  Created on: 16 ���. 2021 �.
 *      Author: Shcheblykin
 */

#ifndef PASSWORD_H_
#define PASSWORD_H_

#include <stdint.h>

/// ����� ��� ������
class TPassword {
    /// ������������ �������� ������.
    static const uint16_t kMaxPassword      = 9999;
    /// ������ �� ���������.
    static const uint16_t kDefaultPassword  = 0000;
    ///  ������ ������.
    static const uint16_t kHardwarePassword = 6352;

public:

    /// �����������.
    TPassword() { mPassword = kMaxPassword + 1; }

    /**
     * @brief ���������� ������� ������.
     * @return ������.
     */
    uint16_t get() const { return mPassword; }

    /**
     * @brief ������������� �������� ������.
     *
     * � ������ ���������� �������� ������ �� ����� �������.
     *
     * @param[in] pasword ������ [0..kMaxPassword].
     * @return
     */
    bool set(uint16_t pas) {
        bool check = (pas <= kMaxPassword);

        if (check) {
            mPassword = pas;
        }

        return check;
    }

    /**
     * @brief ������������� ��������� �������� ������.
     *
     * � ������ ���������� �������� ����� ���������� ������ �� ���������.
     *
     * @param[in] pasword ������ [0..kMaxPassword].
     */
    void init(uint16_t pasword)
    {
        if (!set(pasword)) {
            set(kDefaultPassword);
        }
    }

    /**
     * @brief ��������� ������ �� ���������� � ������� ��� ������ �������.
     * @param[in] pasword ������ [0..kMaxPassword]..
     * @return true ���� ����� ������ ����, ����� false.
     */
    bool check(uint16_t pasword) const
    {
        return (pasword == mPassword) || (pasword == kHardwarePassword);
    }

private:
    /// ������ ������������.
    uint16_t mPassword;
};

#endif /* PASSWORD_H_ */
