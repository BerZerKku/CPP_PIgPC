/*
 * menuPunkt.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef MENUPUNKT_H
#define MENUPUNKT_H

#include "glbDefine.h"

/// Структура пункта меню
class TMenuPunkt
{
    /// максимальное кол-во пунктов в меню
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
     *  @brief Очищает список пунктов меню.
     *
     * *************************************************************************
     */
    void Clear() { mLen = 0; }

    /**
     * *************************************************************************
     *
     * @brief Возвращает название укзанного пункта меню.
     *
     * @param[in] index Индекс пункта меню.
     * @return Название пункта.
     *
     * *************************************************************************
     */
    PGM_P GetName(uint8_t index) { return ((index < mLen) ? mPunkts[index].name : nullptr); }

    /**
     * *************************************************************************
     *
     * @brief Возвращает данные для указанного пункта меню.
     *
     * @param[in] index Индекс пункта меню.
     * @return Данные.
     *
     * *************************************************************************
     */
    uint8_t GetData(uint8_t index) { return ((index < mLen) ? mPunkts[index].data : 0); }

    /**
     *  *************************************************************************
     *
     * @brief Возвращает текущее количество пункто меню.
     *
     * @return Текущее кол-во пунктов меню.
     * @retval 0 - Пусто.
     *
     * *************************************************************************
     */
    uint8_t GetLen() { return mLen; }

    /**
     * *************************************************************************
     *
     * @brief Возвращает максимальное количество пунктов меню.
     * @return
     *
     * *************************************************************************
     */
    uint8_t GetSize() { return kSize; }

private:
    punkt_t mPunkts[kSize];  ///< Пункты меню.
    uint8_t mLen;            ///< Количество пунктов меню.
};


/**
 * *****************************************************************************
 *
 * Добавление пункта меню, через указатель на имя.
 *
 * @param[in] name Название пункта.
 * @param[in] data Номер пункта или дополнительное значение.
 * @return Результат добавления.
 * @retval True В случае успешного добавления.
 * @retval False В случае ошибки добавления. Например, переполнен массив.
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
 * @brief Заменяет указанный пункт меню.
 * @param[in] index Индекс пукта меню.
 * @param[in] name Название пункта.
 * @param[in] data Номер пункта или дополнительное значение.
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
