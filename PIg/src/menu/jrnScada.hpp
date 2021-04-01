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

    /// ��������� �������.
    typedef enum __attribute__ ((__packed__)) {
        COM_OFF = 0,	///< ��������.
        COM_ON,			///< ��������.
        COM_MAX			///< ������������ ��������.
    } com_t;

    /// ������� ��������� ������.
    typedef enum __attribute__ ((__packed__)) {
        STATE_IDLE = 0,	///< ��������.
        STATE_REC,		///< ������� ����� ������ �������.
        STATE_TR_OK,	///< ������� ������ �������� � SCADA, ������� ����� ������ �������.
        STATE_REC_OK,	///< ������ ������� ������� � ���� �������� � SCADA.
        STATE_MAX		///< ������������ ��������.
    } state_t;

    /// ���� �������� �������� ������.
    enum __attribute__ ((__packed__)) defSignal_t {
        DEF_SIGNAL_PUSK = 0,	///< ����.
        DEF_SIGNAL_STOP,		///< ����.
        DEF_SIGNAL_MAN,			///< ���.
        DEF_SIGNAL_PRM,			///< ���. ���.
        DEF_SIGNAL_PRD,			///< ���. ���.
        DEF_SIGNAL_OUT,			///< ����� ���������.
        //
        DEF_SIGNAL_MAX			///< ���������� �������� � ������.
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

    /// ��������� �������.
    void setEvent(uint8_t val) {
        m_u8Event= val;
    }

    /// ���������� �������.
    uint8_t getEvent() const {
        return m_u8Event;
    }

    /** ���������� ������� ��� ������� ������.
     *
     *	@param[out] val �������� �������.
     * 	@return ������� ��� ������� ������.
     * 	@retval 0 ���� ������� ������ ���.
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

    /// ��������� ������ �������.
    void setCom(uint8_t val) {
        m_u8Com = val;
    }

    /// ���������� ����� �������.
    uint8_t getCom() const {
        return m_u8Com;
    }

    /// ��������� ��������� �������
    void setComSource(uint8_t source) {
        m_u8ComSource = source;
    }

    /** ���������� �������� �������
     *
     * 	@retval 0 - ���������� ����.
     * 	@retval 1 - �������� ���������.
     */
    uint8_t getComSource() const {
        return m_u8ComSource;
    }

    /// ��������� �������� �������.
    void setJrn(uint8_t val) {
        if ((val >= GB_DEVICE_K400_MIN) && (val <GB_DEVICE_K400_MAX)) {
            m_eJrn = static_cast<eGB_DEVICE_K400> (val);
        } else {
            m_eJrn = GB_DEVICE_K400_MAX;
        }
        m_step = 0;
    }

    /// �������� �������� ������� �� ������ ������.
    bool isJrnDef() const {
        return (m_eJrn == GB_DEVICE_K400_DEF);
    }

    /// �������� �������� ������� �� ������ �������.
    bool isJrnEvent() const {
        return (m_eJrn == GB_DEVICE_K400_GLB);
    }

    /// �������� �������� ������� �� ������ ���������.
    bool isJrnPrm() const {
        // TODO �������� �� ��� ��������� ������� ���������
        return (m_eJrn == GB_DEVICE_K400_PRM1);
    }

    /// �������� �������� ������� �� ������ �����������.
    bool isJrnPrd() const {
        return (m_eJrn == GB_DEVICE_K400_PRD);
    }

    /// ��������� �������� ��������� � ������� ����� ������.
    void setReadyToSend() {
        m_eState = STATE_REC_OK;
    }

    /// ���������� �������� ��������� � ���������� � ����� ������.
    void setReadyToRead() {
        m_eState = STATE_REC;
    }

    /// ��������� �������� ��������� � ���������� � ����� ������ + ���������� ���� ��������.
    void setReadyToEvent() {
        m_eState = STATE_TR_OK;
    }

    /// ��������� �������� ��������� � ��������.
    void setIdle() {
        m_eState = STATE_IDLE;
    }

    /// �������� �������� ��������� �� ������ ������ ���������.
    bool isReadyToWrite() const {
        return ((m_eState == STATE_REC) || (m_eState == STATE_TR_OK));
    }

    /** �������� �������� ��������� �� ������� ������ ���������.
     *
     *  � ������ ������, ��������� ����� ����������� � �������� ������ �������.
     *
     *  @return true - ������� ����� ���������.
     */
    bool isReadyToSend() {
        if (m_eState >= STATE_MAX) {
            m_eState = STATE_REC;
        }

        return (m_eState == STATE_REC_OK);
    }

    /**	���������� ������� ���������.
     *
     * 	@retval STATE_IDLE
     * 	@retval STATE_REC
     * 	@retval STATE_TR_OK
     */
    uint8_t getState() const {
        return (m_eState >= STATE_REC_OK) ? STATE_IDLE : m_eState;
    }

   private:
    eGB_DEVICE_K400	m_eJrn;	/// ������.
    uint8_t m_u8Event;		/// C������.
    uint8_t m_u8Com;		/// ����� �������.
    uint8_t m_u8ComSource;	/// �������� ������� (0 - ��, 1 - ���).

    state_t m_eState;		/// ������� ���������.
    uint8_t m_step;			/// ������� ������.
};

#endif /* JRN_SCADA_HPP_ */
