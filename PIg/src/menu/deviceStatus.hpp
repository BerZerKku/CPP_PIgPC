/*
 * enterParam.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef DEVICE_STATUS_HPP_
#define DEVICE_STATUS_HPP_

#include <stdint.h>
#include "src/glbDefine.h"

/// ����� ��� �������� ��������� ��������
class TDeviceStatus {
   public:
    /// ������������ ���-�� ��������� ��� ������� ��������� ���������
    // �.�. ������������ 3 ����, 0 ��� - 1 �������, ... 2 ���� - 3 �������.
    // ���� ����������� ��� ��� - ������.
    static const uint8_t GB_MAX_REM_NUM = 7;

    TDeviceStatus() {
        // ������������ ������ �� ���������
        static const char nameDev[] PROGMEM = "���";
        name = nameDev;
        enable_ = false;
        fault_ = 0;
        faults_ = 0;
        numFaults_ = 0;
        warning_ = 0;
        warnings_ = 0;
        numWarnings_ = 0;
        remoteNumber_ = 0;
        regime_ = GB_REGIME_MAX;
        state_ = MAX_NUM_DEVICE_STATE;
        dopByte_ = 0;
    }

    /**	��������� ������ ���������� ��������.
     *
     * 	������������ ��� ������ ������� ��������� ���������, ��� �������
     * 	�������������� ������ (����� �400(�) ������������� � ����-�).
     *
     * 	@param val ����� ��������(��). ��� ������� ���� �������� �� �����������-
     * 	��� �������.
     * 	@retval True - � ������ �������� ������ ���������.
     * 	@retval False - ���� ���� ���������� ������ ������
     */
    bool setRemoteNumber(uint8_t val) {
        bool state = false;

        if (val > GB_MAX_REM_NUM) {
            remoteNumber_ = GB_MAX_REM_NUM;
        } else {
            remoteNumber_ = val;
            state = true;
        }

        return state;
    }

    /** ���������� ����� ���������� ��������.
     *
     * 	������������ ��� ������ ������� ��������� ���������, ��� �������
     * 	�������������� ������ (����� �400(�) ������������� � ����-�).
     *
     * 	@param ����� ��������(��), � ������ ������ � �������  fcRemoteNum.
     */
    uint8_t getRemoteNumber() const {
        return remoteNumber_;
    }

    // ������
    bool setFault(uint16_t faults) {
        fault_ = getFirstSetBit(faults);
        faults_ = faults;
        numFaults_ = getNumSetBits(faults);
        return true;
    }
    uint8_t getFault() const {
        return fault_;
    }
    uint16_t getFaults() const {
        return faults_;
    }
    uint8_t getNumFaults() const {
        return numFaults_;
    }

    // �������������
    bool setWarning(uint16_t warnings) {
        warning_ = getFirstSetBit(warnings);
        warnings_ = warnings;
        numWarnings_ = getNumSetBits(warnings);
        return true;
    }
    uint8_t getWarning() const {
        return warning_;
    }
    uint16_t getWarnings() const {
        return warnings_;
    }
    uint8_t getNumWarnings() const {
        return numWarnings_;
    }

    /**	���������� ������� ��������� ���������� ������ ��������������.
     *
     *	��� �������� ��������� 0xFF ���������� true ���� ���� ���� �� ����
     *	��������������. ��� ��������� �� 0
     *
     *	@param shift ����� ��������������.
     *	@return ��������� ��������������.
     */
    bool isWarning(uint8_t shift=0xFF) const {
        bool s = false;
        if (shift == 0xFF) {
            s = (warnings_ != 0);
        } else if (shift < 16) {
            s = ((warnings_ & (1 << shift)) != 0);
        }
        return s;
    }

    /**	���������� ������� ��������� ���������� ������ �������������.
     *
     *	��� �������� ��������� 0xFF ���������� true ���� ���� ���� �� ����
     *	��������������. ��� ��������� �� 0
     *
     *	@param shift ����� �������������.
     *	@return ��������� �������������.
     */
    bool isFault(uint8_t shift=0xFF) const {
        bool s = false;
        if (shift == 0xFF) {
            s = (faults_ != 0);
        } else if (shift < 16) {
            s = ((faults_ & (1 << shift)) != 0);
        }
        return s;
    }

    // ����� ������
    // ��� GLB ������������ GB_REGIME_DISABLED, � ������ ���� ��� ���������
    // ���������� ��������, ����� GB_REGIME_MAX
    bool setRegime(eGB_REGIME regime) {
        bool stat = false;
        if ((regime >= GB_REGIME_MIN) && (regime <= GB_REGIME_MAX)) {
            stat = true;
        } else
            regime = GB_REGIME_MAX;
        regime_ = regime;
        return stat;
    }
    eGB_REGIME getRegime() const {
        return regime_;
    }

    //���������
    bool setState(uint8_t state) {
        bool stat = false;
        if (state < MAX_NUM_DEVICE_STATE) {
            stat = true;
        } else {
            state = MAX_NUM_DEVICE_STATE;
        }
        state_ = state;
        return stat;
    }
    uint8_t getState() const {
        return state_;
    }

    uint8_t getDopByte() const {
        return dopByte_;
    }
    bool setDopByte(uint8_t byte) {
        dopByte_ = byte;
        return true;
    }

    // ������ � ������ ������� ����������
    // ���������� true ���� ����� �������� ���������� �� ��������
    bool setEnable(bool val) {
        uint8_t act = GB_ACT_NO;

        if (enable_ == val) {
            act |= GB_ACT_OLD;
        } else {
            act |= GB_ACT_NEW;
        }
        enable_ = val;

        return act;
    }
    bool isEnable() const {
        return enable_;
    }

    // ������� ����������� ������ � ��������������
    PGM_P faultText[MAX_NUM_FAULTS];
    PGM_P warningText[MAX_NUM_WARNINGS];
    PGM_P stateText[MAX_NUM_DEVICE_STATE + 1];
    PGM_P name;

   private:
    // ������� ������������ �������������, ������������� � ���-�� ��������������
    uint8_t fault_;
    uint16_t faults_;
    uint8_t numFaults_;

    // ������� ������������ ��������������, �������������� � ���-�� ����������.
    uint8_t warning_;
    uint16_t warnings_;
    uint8_t numWarnings_;

    // ����� ���������� �������� ��� �������� ���� ������������ ������/�������.
    // ������������ � �400(�) � ������������� � ����-�
    uint8_t remoteNumber_;

    eGB_REGIME regime_;
    uint8_t state_;
    uint8_t dopByte_;

    // True - �������� ������� ������� ���������� � ������� ���������� ������
    bool enable_;


    // ���������� ���-�� ������������� � 1 ���
    uint8_t getNumSetBits(uint16_t val)
    {
        uint8_t cnt = 0;
        for(; val > 0; val >>= 1)
        {
            if (val & 0x0001)
                cnt++;
        }
        return cnt;
    }

    // ���������� ����� �������� �������������� ����
    uint8_t getFirstSetBit(uint16_t val)
    {
        uint8_t cnt = 0;
        for(; val > 0; val >>= 1)
        {
            if (val & 0x0001)
                break;
            cnt++;
        }
        return cnt;
    }
};

#endif /* DEVICE_STATUS_HPP_ */
