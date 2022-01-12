/*
 * control.h
 *
 *  Created on: 22 июн. 2021 г.
 *      Author: Shcheblykin
 */

#ifndef TCONTROL_H_
#define TCONTROL_H_

#include "glbDefine.h"

/**
 * @brief Класс для работы с сигналами управления, в том числе и с АК.
 */
class TControl
{

public:
    /// Сигналы управления.
    enum ctrl_t
    {
        CTRL_NO = -1,  ///< Нет сигнала
        //
        CTRL_Call = 0,       ///< Вызов
        CTRL_AcPusk,         ///< Пуск АК
        CTRL_AcPuskSelf,     ///< Пуск АК свой
        CTRL_AcRegime,       ///< Смена режима АК
        CTRL_AcReset,        ///< Сброс АК
        CTRL_IndReset,       ///< Сброс индикации
        CTRL_PuskAdjOn,      ///< Пуск своего вкл.
        CTRL_PuskAdjOff,     ///< Пуск своего выкл.
        CTRL_PuskPrm,        ///< Пуск приемника
        CTRL_RemoteAcPusk,   ///< Пуск АК удаленного
        CTRL_RemotePusk1,    ///< Пуск удаленного 1
        CTRL_RemotePuskAll,  ///< Пуск удаленных
        CTRL_Reset,          ///< Сброс
        //
        CTRL_MAX
    };


    /**
     * @brief Возвращает текст для сигнала управления.
     * @param[in] ctrl Сигнал управления.
     * @return Текст.
     * @return "" в случе ошибки.
     */
    PGM_P getText(ctrl_t ctrl) const;

    /**
     * @brief Возвращает данные для сигнала управления.
     * @param[in] ctrl Сигнал управления.
     * @param[out] com Команда.
     * @param[out] isbyte Наличие байта данных.
     * @param[out] byte Байт данных.
     * @return true если сигнал корректный, иначе false.
     */
    bool getData(ctrl_t ctrl, eGB_COM &com, bool &isbyte, uint8_t &byte) const;

private:
    /// Структруа сигнала управления.
    struct data_t
    {
        char    text[20];  ///< Название синала.
        eGB_COM com;     ///< Команда стандратного протокола АВАНТ.
        bool    isbyte;  ///< Наличие байта данных в команде.
        uint8_t byte;    ///< Байт данных.
    };

    static const data_t mData[TControl::CTRL_MAX] PROGMEM;

    /**
     * @brief Проверяет корректность сигнала управления.
     * @param[in] ctrl Сигнал управления.
     * @return true если сигнал корректный, иначе false.
     */
    bool checkCtrl(ctrl_t ctrl) const;
};

#endif /* TCONTROL_H_ */
