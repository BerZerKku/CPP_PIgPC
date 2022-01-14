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
 * *****************************************************************************
 *
 * @brief Класс для работы с сигналами управления, в том числе и с АК.
 *
 * *****************************************************************************
 */
class TControl
{
public:
    /// Сигналы управления.
    enum ctrl_t
    {
        CTRL_NO = -1,  ///< Нет сигнала
        //
        CTRL_Call = 0,        ///< Вызов
        CTRL_AcAuto,          ///< АК автоматический
        CTRL_AcAccelerated,   ///< АК ускоренный
        CTRL_AcCtrlCheck,     ///< АК контрольная проверка
        CTRL_AcNormal,        ///< АК нормальный
        CTRL_AcOff,           ///< АК выключен
        CTRL_AcOn,            ///< АК включен
        CTRL_AcPusk,          ///< Пуск АК
        CTRL_AcPuskSelf,      ///< Пуск АК свой
        CTRL_AcQuick,         ///< АК беглый
        CTRL_AcRegime,        ///< Смена режима АК
        CTRL_AcRequest,       ///< АК запрос
        CTRL_AcReset,         ///< Сброс АК
        CTRL_AcTest,          ///< АК тест
        CTRL_IndReset,        ///< Сброс индикации
        CTRL_PuskAdjOn,       ///< Пуск наладочный вкл.
        CTRL_PuskAdjOff,      ///< Пуск наладочный выкл.
        CTRL_PuskPrd,         ///< Пуск передатчика
        CTRL_PuskPrm,         ///< Пуск приемника
        CTRL_RemoteAcPusk,    ///< Пуск АК удаленного
        CTRL_RemoteMan,       ///< Пуск удаленной манипуляции
        CTRL_RemoteMan1,      ///< Пуск удаленной манипуляции 1
        CTRL_RemoteMan2,      ///< Пуск удаленной манипуляции 2
        CTRL_RemoteMan3,      ///< Пуск удаленной манипуляции 3
        CTRL_RemoteManAll,    ///< Пуск удаленных манипуляций
        CTRL_RemotePusk,      ///< Пуск удаленного
        CTRL_RemotePusk1,     ///< Пуск удаленного 1
        CTRL_RemotePusk2,     ///< Пуск удаленного 2
        CTRL_RemotePusk3,     ///< Пуск удаленного 3
        CTRL_RemotePuskAll,   ///< Пуск удаленных
        CTRL_RemoteReset,     ///< Сброс удаленного
        CTRL_RemoteReset1,    ///< Сброс удаленного 1
        CTRL_RemoteReset2,    ///< Сброс удаленного 2
        CTRL_RemoteReset3,    ///< Сброс удаленного 3
        CTRL_RemoteResetAll,  ///< Сброс удаленных
        CTRL_Reset,           ///< Сброс своего
        CTRL_ResetAll,        ///< Сброс всех
        CTRL_SingleOff,       ///< Односторонний режим выкл.
        CTRL_SingleOn,        ///< Односторонний режим вкл.
        //
        CTRL_MAX
    };

    PGM_P getText(ctrl_t ctrl) const;
    bool  getData(ctrl_t ctrl, eGB_COM &com, bool &isbyte, uint8_t &byte) const;

private:
    /// Структруа сигнала управления.
    struct data_t
    {
        PGM_P   text;    ///< Название синала, не более 16 символов.
        eGB_COM com;     ///< Команда стандратного протокола АВАНТ.
        bool    isbyte;  ///< Наличие байта данных в команде.
        uint8_t byte;    ///< Байт данных.
    };

    /// Массив данных.
    static const data_t m_data[TControl::CTRL_MAX] PROGMEM;

    bool checkCtrl(ctrl_t ctrl) const;
};

#endif /* TCONTROL_H_ */
