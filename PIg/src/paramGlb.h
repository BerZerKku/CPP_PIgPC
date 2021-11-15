/*
 * deviceGlb.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMGLB_H_
#define PARAMGLB_H_

#include "glbDefine.h"

/// ���������� ������� ��������
enum eGB_IC
{
    GB_IC_MIN     = 0,
    GB_IC_BSP_MCU = 0,
    GB_IC_BSP_DSP,
    GB_IC_BSP_DSP_PLIS,
    GB_IC_PI_MCU,
    GB_IC_BSK_PLIS_PRD1,
    GB_IC_BSK_PLIS_PRD2,
    GB_IC_BSK_PLIS_PRM1,
    GB_IC_BSK_PLIS_PRM2,
    GB_IC_BSZ_PLIS,
    GB_IC_MAX
};

/// �������������
enum eGB_COMP_R400M
{
    GB_COMP_R400M_MIN    = 0,
    GB_COMP_R400M_AVANT  = 0,
    GB_COMP_R400M_PVZ90  = 1,
    GB_COMP_R400M_AVZK80 = 2,
    GB_COMP_R400M_PVZUE  = 3,
    GB_COMP_R400M_PVZL   = 4,
    GB_COMP_R400M_LINER  = 5,
    GB_COMP_R400M_PVZK = 6,  // TODO ����� �� �������. ������ �� ���� �� �������.
    GB_COMP_R400M_MAX
};

/// ������������� ��� �400
enum eGB_COMP_K400
{
    GB_COMP_K400_MIN       = 0,
    GB_COMP_K400_AVANT     = 0,
    GB_COMP_K400_AVANT_PRD = 1,
    GB_COMP_K400_AVANT_PRM = 2,
    GB_COMP_K400_AKPA_PRD  = 3,
    GB_COMP_K400_AKPA_PRM  = 4,
    GB_COMP_K400_KEDR_PRD  = 5,
    GB_COMP_K400_KEDR_PRM  = 6,
    GB_COMP_K400_UPKC_PRD  = 7,
    GB_COMP_K400_UPKC_PRM  = 8,
    GB_COMP_K400_VCTO_PRD  = 9,
    GB_COMP_K400_VCTO_PRM  = 10,
    GB_COMP_K400_ANKA_PRD  = 11,  ///< ����-����
    GB_COMP_K400_ANKA_PRM  = 12,  ///< ����-����
    GB_COMP_K400_MAX
};

/// ������������� ��� ����
enum eGB_COMP_RZSK
{
    GB_COMP_RZSK_MIN = 0,
    GB_COMP_RZSK     = GB_COMP_RZSK_MIN,
    GB_COMP_RZSK_M,
    GB_COMP_RZSK_3E8,
    //
    GB_COMP_RZSK_MAX
};

/// ����� ��� ����� ���������� � ��������
class TDeviceGlb
{
    /// ���������� �����
    enum __attribute__((__packed__)) dInput_t
    {
        DINPUT_PUSK_PRM   = 0x01,  ///< ���� ���
        DINPUT_RESET_IND  = 0x02,  ///< ����� ���
        DINPUT_TM_CONTROL = 0x04   ///< ��� ��
    };

public:
    TDeviceGlb()
    {
        typeDevice_ = AVANT_NO;
        typeOpto_   = TYPE_OPTO_MAX;
        numDevices_ = GB_NUM_DEVICES_MAX;
        typeLine_   = GB_TYPE_LINE_MAX;
        compR400m_  = GB_COMP_R400M_MAX;
        compK400_   = GB_COMP_K400_MAX;
        compRZSK_   = GB_COMP_RZSK_MAX;

        deviceNum_ = 1;
        ledOn      = false;

        for (uint_fast8_t i = 0; i < GB_IC_MAX; i++)
        {
            versProgIC_[i] = 0;
        }

        dInputState = 0x00;
    }

    TDeviceStatus status;

    /**	���������� ��� ��������.
     *
     * 	@return ��� ��������.
     */
    eGB_TYPE_DEVICE getTypeDevice() const { return typeDevice_; }

    /**	��������� ���� ��������.
     *
     * 	� ������ ������, �������� �� ����������.
     *
     * 	@param ��� ��������.
     * 	@return ������ ���������.
     */
    uint8_t setTypeDevice(eGB_TYPE_DEVICE val)
    {
        uint8_t act = GB_ACT_NO;

        if ((val < AVANT_NO) || (val > AVANT_MAX))
        {
            act |= GB_ACT_ERROR;
            val = typeDevice_;
        }

        // NOTE �400� �� ����������� ����������
        if (typeLine_ == GB_TYPE_LINE_OPTO)
        {
            val = AVANT_OPTO;
        }

        act |= (typeDevice_ == val) ? GB_ACT_OLD : GB_ACT_NEW;
        typeDevice_ = val;

        return act;
    }

    /**	���������� ��� ����������� ��������.
     *
     * 	@return ��� ��������.
     */
    eGB_TYPE_OPTO getTypeOpto() const { return typeOpto_; }

    /**	��������� ���� ��������.
     *
     * 	� ������ ������, �������� �� ����������.
     *
     * 	@param ��� ��������.
     * 	@return ������ ���������.
     */
    uint8_t setTypeOpto(eGB_TYPE_OPTO val)
    {
        uint8_t act = GB_ACT_NO;

        if (val > TYPE_OPTO_MAX)
        {
            act |= GB_ACT_ERROR;
        }
        else
        {
            if (typeLine_ == GB_TYPE_LINE_OPTO)
            {
                if (typeOpto_ == val)
                {
                    act = GB_ACT_OLD;
                }
                else
                {
                    typeOpto_ = val;
                    act       = GB_ACT_NEW;
                }
            }
        }

        return act;
    }


    /**	���������� ������������ ���-�� ��������� � �����.
     *
     * 	@retval 3 � ������ 3-� �������� ������
     * 	@retval 2 �� ���� ��������� �������.
     */
    uint8_t getMaxNumDevices() const { return (numDevices_ == GB_NUM_DEVICES_3) ? 3 : 2; }

    /**	���������� ���-�� ��������� � ����� (2-�, 3-� ��������).
     *
     * 	@return ���-�� ��������� � �����.
     */
    eGB_NUM_DEVICES getNumDevices() const { return numDevices_; }

    /** ��������� ���-�� ��������� � ����� (2-�, 3-� ��������.).
     *
     * 	� ������ ������, �������� �� ����������.
     * 	@param val ���-�� ��������� � �����.
     * 	@return ������ ��������� (eGB_ACT - �������� ��������).
     */
    uint8_t setNumDevices(eGB_NUM_DEVICES val)
    {
        uint8_t act = GB_ACT_NO;

        if ((val < GB_NUM_DEVICES_MIN) || (val >= GB_NUM_DEVICES_MAX))
        {
            act = GB_ACT_ERROR;
        }
        else
        {
            if (numDevices_ == val)
            {
                act = GB_ACT_OLD;
            }
            else
            {
                numDevices_ = val;
                act         = GB_ACT_NEW;
            }
        }

        return act;
    }

    /**	���������� ������� ��� ����� (��/������ � �.�.).
     *
     * 	@return ��� �����.
     */
    eGB_TYPE_LINE getTypeLine() const { return typeLine_; }

    /** ��������� ���� ����� (��/������ � �.�.).
     *
     * 	� ������ ������, �������� �� ����������.
     *
     * 	@param val ��� �����.
     * 	@return ������ ��������� (eGB_ACT - �������� ��������).
     */
    uint8_t setTypeLine(eGB_TYPE_LINE val)
    {
        uint8_t act = GB_ACT_NO;

        if ((val < GB_TYPE_LINE_MIN) || (val >= GB_TYPE_LINE_MAX))
        {
            act = GB_ACT_ERROR;
        }
        else
        {
            if (typeLine_ == val)
            {
                act = GB_ACT_OLD;
            }
            else
            {
                typeLine_ = val;
                act       = GB_ACT_NEW;
            }
        }

        return act;
    }

    /**	������ ������ �������� ��� ��������� ������.
     * 	������ �������� � ���������� int16_t.
     * 	������� ���� - ������ ��������. ������� - �������.
     * 	��������: 2.3, ��� 2 - ������, 3 - �������.
     * 	@param vers ������ ��������.
     * 	@param ic	����������.
     */
    void setVersProgIC16(uint16_t vers, eGB_IC ic)
    {
        if (ic < GB_IC_MAX)
        {
            versProgIC_[ic] = vers;
        }
    }

    /**	������ ������ �������� ��� ���������� ������.
     * ������ �������� � ���������� int16_t.
     * 	������� ���� - ������ ��������. ������� - �������.
     * 	��������: 2.3, ��� 2 - ������, 3 - �������.
     * 	@param vers ������ ��������.
     * 	@param ic	����������.
     */
    void setVersProgIC8(uint8_t vers, eGB_IC ic)
    {
        if (ic < GB_IC_MAX)
        {
            uint8_t hi      = vers >> 4;
            uint8_t low     = vers & 0x0F;
            versProgIC_[ic] = TO_INT16(hi, low);
        }
    }

    /**	���������� ������ �������� ��� ��������� ���������� ������.
     * 	������� ���� int16_t - ������ ��������, ������� - �������.
     * 	��������: 2.3, ��� 2 - ������, 3 - �������.
     * 	@return ������ �������� ����������.
     * 	@retval 0 � ������ ���������� ������ ����������.
     */
    uint16_t getVersProgIC(eGB_IC ic) { return ((ic < GB_IC_MAX) ? versProgIC_[ic] : 0); }


    /** ��������� �������������.
     *  @param[in] val �������������.
     *  @return ������ ��������� (eGB_ACT - �������� ��������).
     *  @retval GB_ACT_OLD
     *  @retval GB_ACT_NEW
     */
    uint8_t setCompatibility(uint8_t compatibility)
    {
        uint8_t act = GB_ACT_NO;

        switch (typeDevice_)
        {
        case AVANT_R400:  // DOWN
        case AVANT_R400M:
            {
                act = setCompR400m(eGB_COMP_R400M(compatibility));
            }
            break;
        case AVANT_RZSK:
            {
                act = setCompRZSK(eGB_COMP_RZSK(compatibility));
            }
            break;
        case AVANT_K400:
            {
                act = setCompK400(eGB_COMP_K400(compatibility));
            }
            break;
        case AVANT_OPTO: break;

        case AVANT_NO:  // DOWN
        case AVANT_MAX:
            {
                act |= setCompR400m(eGB_COMP_R400M(compatibility));
                act |= setCompRZSK(eGB_COMP_RZSK(compatibility));
                act |= setCompK400(eGB_COMP_K400(compatibility));
            }
            break;
        }

        return act;
    }

    /**	���������� ������� �������������.
     * 	@return �������������.
     */
    eGB_COMP_R400M getCompR400m() const { return compR400m_; }

    /**	���������� ������� ������������� � �400�
     *
     * 	@return ������������� � �400.
     */
    eGB_COMP_K400 getCompK400() const { return compK400_; }

    /** ���������� ������� ������������� � �400�
     *
     *  @return ������������� � �400.
     */
    eGB_COMP_RZSK getCompRZSK() const { return compRZSK_; }

    // ����� ��������
    bool setDeviceNum(uint8_t val)
    {
        bool stat = false;

        // �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
        if (val >= 1)
        {
            if (val <= getMaxNumDevices())
            {
                deviceNum_ = val;
                stat       = true;
            }
        }
        return stat;
    }
    uint8_t getDeviceNum() const { return (deviceNum_); }

    // �������� ����� ��������� ���������
    bool isLedOn() const { return ledOn; }

    // ��������� �������� ����� ��������� ���������
    void setLedOn(bool ledOn) { this->ledOn = ledOn; }

    // ��������� ������ ������� ��������� ���������� ������
    void setDInputState(uint8_t val) { dInputState = val; }

    // ���������� ������� ��������� �� "���� ���"
    bool getDInputPuskPrm() const { return ((dInputState & DINPUT_PUSK_PRM) > 0) ? true : false; }
    // ���������� ������� ��������� �� "����� ���"
    bool getDInputResetInd() const { return ((dInputState & DINPUT_RESET_IND) > 0) ? true : false; }

    // ���������� ������� ��������� �� "��� ��"
    bool getDInputTmControl() const
    {
        return ((dInputState & DINPUT_TM_CONTROL) > 0) ? true : false;
    }

private:
    // ������ �������� ���������
    uint16_t versProgIC_[GB_IC_MAX];

    // ��� ��������
    eGB_TYPE_DEVICE typeDevice_;

    // ��� ����������� ��������
    eGB_TYPE_OPTO typeOpto_;

    // ���-�� ��������� � ����� 2 ��� 3
    eGB_NUM_DEVICES numDevices_;

    // ��� �����
    eGB_TYPE_LINE typeLine_;

    // �������������
    eGB_COMP_R400M compR400m_;

    // ������������� � �400
    eGB_COMP_K400 compK400_;

    // ������������� � ����
    eGB_COMP_RZSK compRZSK_;

    // ����� ��������
    uint8_t deviceNum_;

    // ���� ��������� ���������
    bool ledOn;

    // ��������� ���������� ������ (�������, ����(0)/���(1)).
    uint8_t dInputState;

    /**	��������� ������������� (��� ���������� ��������).
     * 	@param val �������������.
     * 	@return ������ ��������� (eGB_ACT - �������� ��������).
     */
    uint8_t setCompR400m(eGB_COMP_R400M val)
    {
        uint8_t act = GB_ACT_NO;

        if ((val < GB_COMP_R400M_MIN) || (val >= GB_COMP_R400M_MAX))
        {
            val = GB_COMP_R400M_MAX;
            act = GB_ACT_ERROR;
        }

        act |= (compR400m_ == val) ? GB_ACT_OLD : GB_ACT_NEW;
        compR400m_ = val;

        return act;
    }

    /** ��������� ������������� ��� RZSK (��� ���������� ��������).
     *  @param val �������������.
     *  @return ������ ��������� (eGB_ACT - �������� ��������).
     */
    uint8_t setCompRZSK(eGB_COMP_RZSK val)
    {
        uint8_t act = GB_ACT_NO;

        if ((val < GB_COMP_RZSK_MIN) || (val >= GB_COMP_RZSK_MAX))
        {
            val = GB_COMP_RZSK_MAX;
            act = GB_ACT_ERROR;
        }

        act |= (compRZSK_ == val) ? GB_ACT_OLD : GB_ACT_NEW;
        compRZSK_ = val;

        return act;
    }

    /**	��������� ������������� ��� �400 (��� ���������� ��������).
     * 	@param val �������������.
     * 	@return ������ ��������� (eGB_ACT - �������� ��������).
     */
    uint8_t setCompK400(eGB_COMP_K400 val)
    {
        uint8_t act = GB_ACT_NO;

        if ((val < GB_COMP_K400_MIN) || (val >= GB_COMP_K400_MAX))
        {
            val = GB_COMP_K400_MAX;
            act = GB_ACT_ERROR;
        }

        act |= (compK400_ == val) ? GB_ACT_OLD : GB_ACT_NEW;
        compK400_ = val;

        return act;
    }
};


#endif /* PARAMGLB_H_ */
