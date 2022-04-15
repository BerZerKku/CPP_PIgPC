/*
 * menu.h
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */

#ifndef MENU_H_
#define MENU_H_

#include "drivers/keyboard.h"
#include "drivers/ks0108.h"

/// Время работы одного цикла (зависит от настройки таймеров), мс
#define TIME_CYLCE 100

// класс меню
class clMenu
{
    static constexpr uint8_t k_lcd_init_counter_max = (200 / TIME_CYLCE);
    static constexpr uint8_t k_blink_counter_max    = (1000 / TIME_CYLCE);

public:
    clMenu();
    void proc();

    static char vLCDbuf[SIZE_BUF_STRING + 1];

    /**
     * *************************************************************************
     *
     * @brief Установка состояния соединения.
     * @param[in] enabled True если есть соединение, иначе false.
     *
     * *************************************************************************
     */
    void SetConnection(bool enabled) { m_connection = enabled; }

    /**
     * *************************************************************************
     *
     * @brief Проверяет наличие связи с БСП.
     * @return True если есть соединение, иначе false.
     *
     * *************************************************************************
     */
    bool IsConnection() const { return m_connection; }


    /**
     * *************************************************************************
     *
     * @brief Возвращает коды нажатых кнопок.
     * @return Коды нажатых кнопок.
     *
     * *************************************************************************
     */
    uint16_t GetKeys() const { return m_key; }

private:
    bool     m_connection;    ///< Флаг наличия связи с БСП.
    uint16_t m_key;           ///< коды нажатых кнопок
    bool     m_blink;         ///< флаг мигания символов
    uint8_t m_blink_counter;  ///< счетчик циклов для мигания символов
    uint8_t m_lcd_init_counter;  ///< счетчик циклов для инициализации дисплея

    // true - необходимо вывести на экран курсор
    bool cursorEnable_;

    // текущее положение курсора (номер строки)
    uint8_t cursorLine_;

    // кол-во отображаемых параметров
    uint8_t lineParam_;


#ifdef TEST_FRIENDS
    TEST_FRIENDS
#endif
};

#endif /* MENU_H_ */
