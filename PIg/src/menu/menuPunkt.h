/*
 * menuPunkt.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef MENUPUNKT_H_
#define MENUPUNKT_H_

#include "glbDefine.h"

/// Структура пункта меню
class TMenuPunkt
{
    /// максимальное кол-во пунктов в меню
    static const uint8_t kSize = 20;

public:
    TMenuPunkt() { clear(); }

    bool    addName(PGM_P const name, uint8_t number = 0);
    bool    addNumber(uint8_t number);
    uint8_t choose(PGM_P const first, PGM_P const second, bool is_first, uint8_t start = 0);


    /** Очистка текущего списка пунктов меню.
     */
    void clear() { cnt_ = 0; }

    /** Возвращает название укзанного пункта меню.
     *
     *	@param num	Номер пункта.
     * 	@return Название пункта.
     */
    PGM_P getName(uint8_t num)
    {
        static const char unknown[] PROGMEM = "?";
        return ((num < cnt_) ? name_[num] : unknown);
    }

    /**	Возвращает номер эелемента массива с именем указанного пункта меню.
     *
     *	@param num	Номер пункта.
     * 	@return Название пункта.
     */
    uint8_t getNumber(uint8_t num) { return ((num < cnt_) ? number_[num] : 0); }

    /** Возвращает текущее кол-во пункто меню.
     *
     * 	@return Текущее кол-во пунктов меню.
     * 	@retval 0 - Пусто.
     */
    uint8_t getMaxNumPunkts() { return cnt_; }

private:
    /// текущее кол-во пунктов
    uint8_t cnt_;

    /// указатель на имя пункта
    PGM_P name_[kSize];

    /// номер пункта, используется с массивами
    uint8_t number_[kSize];
};


/**
 * *****************************************************************************
 *
 * Добавление пункта меню, через указатель на имя.
 *
 * @param[in] name Имя пункта.
 * @param[in] number Номер пункта или дополнительное значение.
 * @return Результат добавления.
 * @retval True В случае успешного добавления.
 * @retval False В случае ошибки добавления. Например, переполнен массив.
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
 * Добавление пункта меню, через значение.
 *
 * @param[in] number Номер пункта.
 * @return Результат добавления.
 * @retval True В случае успешного добавления.
 * @retval False В случае ошибки добавления. Например, переполнен массив.
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
 * Выбор одного из указанных номеров пункта меню.
 *
 * @param[in] first Первый пункт.
 * @param[in] second Второй пункт.
 * @param[in] is_first Если true, то надо выбрать первый пункт, иначе второй.
 * @param[in] start Начальная позиция для поиска.
 * @return Позиция на которой был найден параметр.
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
