/*
 * password.h
 *
 *  Created on: 16 июн. 2021 г.
 *      Author: Shcheblykin
 */

#ifndef PASSWORD_H_
#define PASSWORD_H_

#include <stdint.h>

/// Класс для пароля
class TPassword {
    /// Максимальное значение пароля.
    static const uint16_t kMaxPassword      = 9999;
    /// Пароль по умолчанию.
    static const uint16_t kDefaultPassword  = 0000;
    ///  Вшитый пароль.
    static const uint16_t kHardwarePassword = 6352;

public:

    /// Конструктор.
    TPassword() { mPassword = kMaxPassword + 1; }

    /**
     * @brief Возвращает текущий пароль.
     * @return Пароль.
     */
    uint16_t get() const { return mPassword; }

    /**
     * @brief Устанавливает значение пароля.
     *
     * В случае ошибочного значения пароль не будет изменен.
     *
     * @param[in] pasword Пароль [0..kMaxPassword].
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
     * @brief Устанавливает начальное значение пароля.
     *
     * В случае ошибочного значения будет установлен пароль по умолчанию.
     *
     * @param[in] pasword Пароль [0..kMaxPassword].
     */
    void init(uint16_t pasword)
    {
        if (!set(pasword)) {
            set(kDefaultPassword);
        }
    }

    /**
     * @brief Проверяет пароль на совпадение с текущим или вшитым паролем.
     * @param[in] pasword Пароль [0..kMaxPassword]..
     * @return true если такой пароль есть, иначе false.
     */
    bool check(uint16_t pasword) const
    {
        return (pasword == mPassword) || (pasword == kHardwarePassword);
    }

private:
    /// Пароль пользователя.
    uint16_t mPassword;
};

#endif /* PASSWORD_H_ */
