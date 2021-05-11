/*
 * deviceGlb.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMGLB_H_
#define PARAMGLB_H_

#include "src/glbDefine.h"
#include "menu/deviceStatus.hpp"

/// �����
/// ����� ��������
#define GLB_DEV_NUM_MIN		1
#define GLB_DEV_NUM_MAX 	3
#define GLB_DEV_NUM_DISC 	1
#define GLB_DEV_NUM_FRACT	1
#define GLB_DEV_NUM_MIN_F	(GLB_DEV_NUM_MIN / GLB_DEV_NUM_FRACT)
#define GLB_DEV_NUM_MAX_F 	(GLB_DEV_NUM_MAX / GLB_DEV_NUM_FRACT)
#define GLB_DEV_NUM_DISC_F	(GLB_DEV_NUM_DISC / GLB_DEV_NUM_FRACT)
/// ������� �����
#define GLB_NET_ADR_MIN		1
#define GLB_NET_ADR_MAX		247
#define GLB_NET_ADR_DISC 	1
#define GLB_NET_ADR_FRACT	1
#define GLB_NET_ADR_MIN_F	(GLB_NET_ADR_MIN / GLB_NET_ADR_FRACT)
#define GLB_NET_ADR_MAX_F	(GLB_NET_ADR_MAX / GLB_NET_ADR_FRACT)
#define GLB_NET_ADR_DISC_F	(GLB_NET_ADR_DISC / GLB_NET_ADR_FRACT)

/// ���������� ������� ��������
enum eGB_IC {
	GB_IC_MIN = 0,
	GB_IC_BSP_MCU = 0,
	GB_IC_BSP_DSP,
	GB_IC_BSP_DSP_PLIS,
	GB_IC_PI_MCU,
	GB_IC_BSK_PLIS_PRD1,
	GB_IC_BSK_PLIS_PRD2,
	GB_IC_BSK_PLIS_PRM1,
	GB_IC_BSK_PLIS_PRM2,
	GB_IC_BSZ_PLIS,
    GB_IC_BVP_STM32,
    GB_IC_VP,
	GB_IC_MAX
};

/// �������������
enum eGB_COMPATIBILITY {
	GB_COMPATIBILITY_MIN 	= 0,
	GB_COMPATIBILITY_AVANT 	= 0,
	GB_COMPATIBILITY_PVZ90 	= 1,
	GB_COMPATIBILITY_AVZK80	= 2,
	GB_COMPATIBILITY_PVZUE 	= 3,
	GB_COMPATIBILITY_PVZL 	= 4,
	GB_COMPATIBILITY_LINER 	= 5,
	GB_COMPATIBILITY_PVZK	= 6,	// TODO ����� �� �������. ������ �� ���� �� �������.
	GB_COMPATIBILITY_MAX
};

/// ������������� ��� �400
enum eGB_COMP_K400 {
	GB_COMP_K400_MIN 		= 0,
	GB_COMP_K400_AVANT 		= 0,
	GB_COMP_K400_AVANT_PRD 	= 1,
	GB_COMP_K400_AVANT_PRM	= 2,
	GB_COMP_K400_AKPA_PRD 	= 3,
	GB_COMP_K400_AKPA_PRM 	= 4,
	GB_COMP_K400_KEDR_PRD	= 5,
	GB_COMP_K400_KEDR_PRM 	= 6,
	GB_COMP_K400_UPKC_PRD	= 7,
	GB_COMP_K400_UPKC_PRM	= 8,
	GB_COMP_K400_VCTO_PRD	= 9,
	GB_COMP_K400_VCTO_PRM	= 10,
	GB_COMP_K400_ANKA_PRD	= 11,	///< ����-����
	GB_COMP_K400_ANKA_PRM	= 12,	///< ����-����
	GB_COMP_K400_MAX
};

/// ����� ��� ����� ���������� � ��������
class TDeviceGlb {
	/// ���������� �����
	enum __attribute__((__packed__)) dInput_t {
		DINPUT_PUSK_PRM 	= 0x01,	///< ���� ���
		DINPUT_RESET_IND 	= 0x02,	///< ����� ���
		DINPUT_TM_CONTROL 	= 0x04	///< ��� ��
	};

public:
	TDeviceGlb() {
		typeDevice_ = AVANT_NO;
		typeOpto_ = TYPE_OPTO_MAX;
		numDevices_ = GB_NUM_DEVICES_MAX;
		typeLine_ = GB_TYPE_LINE_MAX;
		compatibility_ = GB_COMPATIBILITY_MAX;
		compK400_ = GB_COMP_K400_MAX;

		deviceNum_ = GLB_DEV_NUM_MIN_F;
		netAdr_ = 0;
		ledOn = false;

		for(uint_fast8_t i = 0; i < GB_IC_MAX; i++) {
			versProgIC_[i] = 0;
		}

		dInputState = 0x00;
	}

	TDeviceStatus status;

	/**	���������� ��� ��������.
	 *
	 * 	@return ��� ��������.
	 */
	eGB_TYPE_DEVICE getTypeDevice() const {
		return typeDevice_;
	}

	/**	��������� ���� ��������.
	 *
	 * 	� ������ ������, �������� �� ����������.
	 *
	 * 	@param ��� ��������.
	 * 	@return ������ ���������.
	 */
	uint8_t setTypeDevice(eGB_TYPE_DEVICE val) {
		uint8_t act = GB_ACT_NO;

		if ((val < AVANT_NO) || (val > AVANT_MAX)) {
			act = GB_ACT_ERROR;
		} else {
			if ((val == AVANT_K400) && (typeLine_ == GB_TYPE_LINE_OPTO)) {
				val = AVANT_OPTO;
			}

			if ((val == AVANT_RZSK) && (typeLine_ == GB_TYPE_LINE_OPTO)) {
				val = AVANT_OPTO;
			}

			if ((val == AVANT_R400) && (typeLine_ == GB_TYPE_LINE_OPTO)) {
				val = AVANT_OPTO;
			}

			if (typeDevice_ == val) {
				act = GB_ACT_OLD;
			} else {
				typeDevice_ = val;
				act = GB_ACT_NEW;
			}
		}

		return act;
	}

	/**	���������� ��� ����������� ��������.
	 *
	 * 	@return ��� ��������.
	 */
	eGB_TYPE_OPTO getTypeOpto() const {
		return typeOpto_;
	}

    /**	��������� ���� ��������.
     *
     * 	� ������ ������, �������� �� ����������.
     *
     * 	@param ��� ��������.
     * 	@return ������ ���������.
     */
    uint8_t setTypeOpto(eGB_TYPE_OPTO val);

	/**	���������� ������������ ���-�� ��������� � �����.
	 *
	 * 	@retval 3 � ������ 3-� �������� ������
	 * 	@retval 2 �� ���� ��������� �������.
	 */
	uint8_t getMaxNumDevices() const {
		return (numDevices_ == GB_NUM_DEVICES_3) ? 3 : 2;
	}

	/**	���������� ���-�� ��������� � ����� (2-�, 3-� ��������).
	 *
	 * 	@return ���-�� ��������� � �����.
	 */
	eGB_NUM_DEVICES getNumDevices() const {
		return numDevices_;
	}

	/** ��������� ���-�� ��������� � ����� (2-�, 3-� ��������.).
	 *
	 * 	� ������ ������, �������� �� ����������.
	 * 	@param val ���-�� ��������� � �����.
	 * 	@return ������ ��������� (eGB_ACT - �������� ��������).
	 */
	uint8_t setNumDevices(eGB_NUM_DEVICES val) {
		uint8_t act = GB_ACT_NO;

		if ((val < GB_NUM_DEVICES_MIN) || (val >= GB_NUM_DEVICES_MAX)) {
			act = GB_ACT_ERROR;
		} else {
			if (numDevices_ == val) {
				act = GB_ACT_OLD;
			} else {
				numDevices_ = val;
				act = GB_ACT_NEW;
			}
		}

		return act;
	}

	/**	���������� ������� ��� ����� (��/������ � �.�.).
	 *
	 * 	@return ��� �����.
	 */
	eGB_TYPE_LINE getTypeLine() const {
		return typeLine_;
	}

	/** ��������� ���� ����� (��/������ � �.�.).
	 *
	 * 	� ������ ������, �������� �� ����������.
	 *
	 * 	@param val ��� �����.
	 * 	@return ������ ��������� (eGB_ACT - �������� ��������).
	 */
	uint8_t setTypeLine(eGB_TYPE_LINE val) {
		uint8_t act = GB_ACT_NO;

		if ((val < GB_TYPE_LINE_MIN) || (val >= GB_TYPE_LINE_MAX)) {
			act = GB_ACT_ERROR;
		} else {
			if (typeLine_ == val) {
				act = GB_ACT_OLD;
			} else {
				typeLine_ = val;
				act = GB_ACT_NEW;
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
	void setVersProgIC16(uint16_t vers, eGB_IC ic) {
		if (ic < GB_IC_MAX) {
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
	void setVersProgIC8(uint8_t vers, eGB_IC ic) {
		if (ic < GB_IC_MAX) {
			uint8_t hi = vers >> 4;
			uint8_t low = vers & 0x0F;
			versProgIC_[ic] = TO_INT16(hi, low);
		}
	}

	/**	���������� ������ �������� ��� ��������� ���������� ������.
	 * 	������� ���� int16_t - ������ ��������, ������� - �������.
	 * 	��������: 2.3, ��� 2 - ������, 3 - �������.
	 * 	@return ������ �������� ����������.
	 * 	@retval 0 � ������ ���������� ������ ����������.
	 */
	uint16_t getVersProgIC(eGB_IC ic) {
		return ((ic < GB_IC_MAX) ? versProgIC_[ic] : 0);
	}


	/**	��������� ������������� (��� ���������� ��������).
	 * 	@param val �������������.
	 * 	@return ������ ��������� (eGB_ACT - �������� ��������).
	 */
	uint8_t setCompatibility(eGB_COMPATIBILITY val) {
		uint8_t act = GB_ACT_NO;

		if ((val < GB_COMPATIBILITY_MIN) || (val >= GB_COMPATIBILITY_MAX)) {
			val = GB_COMPATIBILITY_MAX;
			act = GB_ACT_ERROR;
		}

		if (compatibility_ == val) {
			act |= GB_ACT_OLD;
		} else {
			compatibility_ = val;
			act |= GB_ACT_NEW;
		}

		return act;
	}

	/**	���������� ������� �������������.
	 * 	@return �������������.
	 */
	eGB_COMPATIBILITY getCompatibility() const {
		return compatibility_;
	}

	/**	��������� ������������� ��� �400 (��� ���������� ��������).
	 * 	@param val �������������.
	 * 	@return ������ ��������� (eGB_ACT - �������� ��������).
	 */
	uint8_t setCompK400(eGB_COMP_K400 val) {
		uint8_t act = GB_ACT_NO;

		if ((val < GB_COMP_K400_MIN) || (val >= GB_COMP_K400_MAX)) {
			val = GB_COMP_K400_MAX;
			act = GB_ACT_ERROR;
		}

		if (compK400_ == val) {
			act |= GB_ACT_OLD;
		} else {
			compK400_ = val;
			act |= GB_ACT_NEW;
		}

		return act;
	}

	/**	���������� ������� ������������� � �400�
	 *
	 * 	@return ������������� � �400.
	 */
	eGB_COMP_K400 getCompK400() const {
		return compK400_;
	}

	// ����� ��������
	bool setDeviceNum(uint8_t val) {
		bool stat = false;
		val = (val / GLB_DEV_NUM_DISC_F) * GLB_DEV_NUM_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
		if (val >= GLB_DEV_NUM_MIN_F) {
			if (val <= getMaxNumDevices()) {
				deviceNum_ = val;
				stat = true;
			}
		}
		return stat;
	}
	uint8_t getDeviceNum() const {
		return (deviceNum_ * GLB_DEV_NUM_FRACT);
	}

	// ������� �����
	bool setNetAddress(uint8_t val) {
		bool stat = false;
		val = (val / GLB_NET_ADR_DISC_F) * GLB_NET_ADR_DISC_F;

		// �������� � ����� ���� �.�. ����� �������� ���������� � Eclipse
		if (val >= GLB_NET_ADR_MIN_F) {
			if (val <= GLB_NET_ADR_MAX_F) {
				netAdr_ = val;
				stat = true;
			}

		}
		return stat;
	}
	uint8_t getNetAddress() const {
		return (netAdr_ * GLB_NET_ADR_FRACT);
	}

	// �������� ����� ��������� ���������
	bool isLedOn() const {
		return ledOn;
	}

	// ��������� �������� ����� ��������� ���������
	void setLedOn(bool ledOn) {
		this->ledOn = ledOn;
	}

	// ��������� ������ ������� ��������� ���������� ������
	void setDInputState(uint8_t val) {
		dInputState = val;
	}

	// ���������� ������� ��������� �� "���� ���"
	bool getDInputPuskPrm() const {
		return ((dInputState & DINPUT_PUSK_PRM) > 0) ? true : false;
	}
	// ���������� ������� ��������� �� "����� ���"
	bool getDInputResetInd() const {
		return ((dInputState & DINPUT_RESET_IND) > 0) ? true : false;
	}

	// ���������� ������� ��������� �� "��� ��"
	bool getDInputTmControl() const {
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
	eGB_COMPATIBILITY compatibility_;

	// ������������� � �400
	eGB_COMP_K400 compK400_;

	// ����� ��������
	uint8_t deviceNum_;

	// ������� �����
	uint8_t netAdr_;

	// ���� ��������� ���������
	bool ledOn;

	// ��������� ���������� ������ (�������, ����(0)/���(1)).
	uint8_t dInputState;

};


#endif /* PARAMGLB_H_ */
