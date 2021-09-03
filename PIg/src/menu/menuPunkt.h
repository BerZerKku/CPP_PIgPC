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
    static const uint8_t MAX_NUM_PUNKTS = 20;

public:
    /**	Конструктор.
     */
    TMenuPunkt() { clear(); }

    /**	Добавление пункта меню, через указатель на имя.
     *
     * 	@param name Имя пункта.
     * 	@param com Команда для запроса из БСП. По умолчанию - нет.
     * 	@retval True В случае успешного добавления.
     * 	@retval False В случае ошибки добавления. Например, переполнен массив.
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

    /**	Добавление пункта меню, через значение.
     *
     * 	@param name Имя пункта.
     * 	@param com Команда для запроса из БСП. По умолчанию - нет.
     * 	@retval True В случае успешного добавления.
     * 	@retval False В случае ошибки добавления. Например, переполнен массив.
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

    /** Очистка текущего списка пунктов меню.
     */
    void clear() { cnt_ = 0; }

    /** Замена указанного номера пункта меню.
     *
     * 	@param name Имя пунка.
     * 	@param num Номер пункта.
     * 	@retval True В случае успешного добавления.
     * 	@retval False В случае ошибки добавления. Например, не верный номер.
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

    /** Выбор одного из указанных номеров пункта меню.
     *
     *  @param[in] first Первый пункт.
     *  @param[in] second Второй пункт.
     *  @param[in] is_first Если true, то надо выбрать первый пункт, иначе второй.
     *  @return true если пункт для замены был найден, иначе false.
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

    /** Замена указанного номера пункта меню.
     *
     * 	@param name Имя пункта.
     * 	@param num Номер пункта.
     * 	@retval True В случае успешного добавления.
     * 	@retval False В случае ошибки добавления. Например, не верный номер.
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

    /** Возвращает название укзанного пункта меню.
     *
     *	@param num	Номер пункта.
     * 	@return Название пункта.
     */
    PGM_P getName(uint8_t num)
    {
        static char unknown[] PROGMEM = "?";
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
    PGM_P name_[MAX_NUM_PUNKTS];

    /// номер пункта, используется с массивами
    uint8_t number_[MAX_NUM_PUNKTS];
};


#endif /* MENUPUNKT_H_ */
