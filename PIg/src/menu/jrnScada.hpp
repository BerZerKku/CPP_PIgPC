/*
 * enterParam.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef JRN_SCADA_HPP_
#define JRN_SCADA_HPP_

#include "src/glbDefine.h"
#include "src/menu/dateTime.hpp"

class TJrnSCADA {
   public:

    /// Состояние команды.
    typedef enum __attribute__ ((__packed__)) {
        COM_OFF = 0,	///< Окончена.
        COM_ON,			///< Началась.
        COM_MAX			///< Максимальное значение.
    } com_t;

    /// Текущее состояние записи.
    typedef enum __attribute__ ((__packed__)) {
        STATE_IDLE = 0,	///< Ожидание.
        STATE_REC,		///< Считать новую запись журнала.
        STATE_TR_OK,	///< Текущая запись передана в SCADA, считать новую запись журнала.
        STATE_REC_OK,	///< Запись журнала считана и ждет отправки в SCADA.
        STATE_MAX		///< Максимальное значение.
    } state_t;

    /// Шаги проверки сигналов защиты.
    enum __attribute__ ((__packed__)) defSignal_t {
        DEF_SIGNAL_PUSK = 0,	///< ПУСК.
        DEF_SIGNAL_STOP,		///< СТОП.
        DEF_SIGNAL_MAN,			///< МАН.
        DEF_SIGNAL_PRM,			///< Осц. ПРМ.
        DEF_SIGNAL_PRD,			///< Осц. ПРД.
        DEF_SIGNAL_OUT,			///< Выход приемника.
        //
        DEF_SIGNAL_MAX			///< Количество сигналов в списке.
    };

    TJrnSCADA() {
        m_u8Com = 0;
        m_u8ComSource = 0;
        m_u8Event = 0;
        m_eJrn = GB_DEVICE_K400_MAX;

        m_eState = STATE_IDLE;
        m_step = 0;
    };

    TDateTime dtime;

    /// Установка события.
    void setEvent(uint8_t val) {
        m_u8Event= val;
    }

    /// Возвращает событие.
    uint8_t getEvent() const {
        return m_u8Event;
    }

    /** Возвращает событие для журнала защиты.
     *
     *	@param[out] val Значение события.
     * 	@return Событие для журнала защиты.
     * 	@retval 0 если событий больше нет.
     */
    uint8_t getDefEvent(bool &val) {
        uint8_t ev = 0;

        switch(m_step) {
            case DEF_SIGNAL_PUSK:
                ev = ++m_step;
                val = m_u8Com & 0x01 ? true : false;
                break;
            case DEF_SIGNAL_STOP:
                ev = ++m_step;
                val = m_u8Com & 0x02 ? true : false;
                break;
            case DEF_SIGNAL_MAN:
                ev = ++m_step;
                val = m_u8Com & 0x04 ? true : false;
                break;
            case DEF_SIGNAL_PRM:
                ev = ++m_step;
                val = m_u8ComSource & 0x01 ? true : false;
                break;
            case DEF_SIGNAL_PRD:
                ev = ++m_step;
                val = m_u8ComSource & 0x02 ? true : false;
                break;
            case DEF_SIGNAL_OUT:
                ev = ++m_step;
                val = m_u8ComSource & 0x04 ? true : false;
                break;
        }


        return ev;
    }

    /// Установка номера команды.
    void setCom(uint8_t val) {
        m_u8Com = val;
    }

    /// Возвращает номер команды.
    uint8_t getCom() const {
        return m_u8Com;
    }

    /// Установка источника сигнала
    void setComSource(uint8_t source) {
        m_u8ComSource = source;
    }

    /** Возвращает источник сигнала
     *
     * 	@retval 0 - дискретный вход.
     * 	@retval 1 - цифровой переприем.
     */
    uint8_t getComSource() const {
        return m_u8ComSource;
    }

    /// Установка текущего журнала.
    void setJrn(uint8_t val) {
        if ((val >= GB_DEVICE_K400_MIN) && (val <GB_DEVICE_K400_MAX)) {
            m_eJrn = static_cast<eGB_DEVICE_K400> (val);
        } else {
            m_eJrn = GB_DEVICE_K400_MAX;
        }
        m_step = 0;
    }

    /// Проверка текущего журнала на журнал защиты.
    bool isJrnDef() const {
        return (m_eJrn == GB_DEVICE_K400_DEF);
    }

    /// Проверка текущего журнала на журнал событий.
    bool isJrnEvent() const {
        return (m_eJrn == GB_DEVICE_K400_GLB);
    }

    /// Проверка текущего журнала на журнал приемника.
    bool isJrnPrm() const {
        // TODO проверка на все остальные журналы приемника
        return (m_eJrn == GB_DEVICE_K400_PRM1);
    }

    /// Проверка текущего журнала на журнал передатчика.
    bool isJrnPrd() const {
        return (m_eJrn == GB_DEVICE_K400_PRD);
    }

    /// Установка текущего состояния в наличие новой записи.
    void setReadyToSend() {
        m_eState = STATE_REC_OK;
    }

    /// Уситановка текущего состояния в готовность к новой записи.
    void setReadyToRead() {
        m_eState = STATE_REC;
    }

    /// Установка текущего состояния в готовность к новой записи + предыдущая была передана.
    void setReadyToEvent() {
        m_eState = STATE_TR_OK;
    }

    /// Установка текущего состояния в ожидание.
    void setIdle() {
        m_eState = STATE_IDLE;
    }

    /// Проверка текущего состояния на запись нового сообщения.
    bool isReadyToWrite() const {
        return ((m_eState == STATE_REC) || (m_eState == STATE_TR_OK));
    }

    /** Проверка текущего состояния на наличие нового сообщения.
     *
     *  В случае ошибки, состояние будет установлено в ожидание нового события.
     *
     *  @return true - имеется новое сообщение.
     */
    bool isReadyToSend() {
        if (m_eState >= STATE_MAX) {
            m_eState = STATE_REC;
        }

        return (m_eState == STATE_REC_OK);
    }

    /**	Возвращает текущее состояние.
     *
     * 	@retval STATE_IDLE
     * 	@retval STATE_REC
     * 	@retval STATE_TR_OK
     */
    uint8_t getState() const {
        return (m_eState >= STATE_REC_OK) ? STATE_IDLE : m_eState;
    }

   private:
    eGB_DEVICE_K400	m_eJrn;	/// Журнал.
    uint8_t m_u8Event;		/// Cобытие.
    uint8_t m_u8Com;		/// Номер команды.
    uint8_t m_u8ComSource;	/// Источник команды (0 - ДВ, 1 - ЦПП).

    state_t m_eState;		/// Текущее состояние.
    uint8_t m_step;			/// Счетчик опроса.
};

#endif /* JRN_SCADA_HPP_ */
