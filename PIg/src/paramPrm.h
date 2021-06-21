/*
 * devicePrm.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMPRM_H_
#define PARAMPRM_H_

#include "glbDefine.h"

/// ����� ��� ���������� � �������� ���������
class TDevicePrm {
public:
	TDevicePrm() {
		numCom_ = 0;

		for (uint_fast8_t i = 0; i < (MAX_NUM_COM_PRM / 8); i++) {
			indCom_[i] = 0;
		}

		numJrnEntry_ = 0;
		maxNumJrnEntry_ = 0;
		overflow_ = false;
	}

	TDeviceStatus status;

	/**	��������� �������� ���-�� ������ � ���.
	 *
	 * 	���� ����� �������� ������� �� �������� ���������� ��������, ���������
	 * 	������ 0. ����� ���� ������ ���, ��� �����������. ����� ����������.
	 *
	 * 	@param val ���������� ������ �� ���.
	 * 	@return ������ ��������� (eGB_ACT - �������� ��������).
	 */
	uint8_t setNumCom(uint8_t val) {
		uint8_t act = GB_ACT_NO;

		if (val > MAX_NUM_COM_PRM) {
			act = GB_ACT_ERROR;
		}

		if (numCom_ == val) {
			act |= GB_ACT_OLD;
		} else {
			numCom_ = val;
			// ���� ���-�� ������ �� ����� 0, �� ���������� ���
			this->status.setEnable(val != 0);
			act |= GB_ACT_NEW;
		}

		return act;
	}

	uint8_t getNumCom() const {
		return numCom_;
	}

	/** ��������� �������� ���������� ����������� �� ����� ���
	 *
	 *	@param num ����� ��������� ������ (0 - 8..1 �������, 1 - 16..9 � �.�.).
	 *	@param val ��������� �����������.
	 *	@return True - � ������ �������� ������, False - �����.
	 */
	bool setIndCom8(uint8_t num, uint8_t val) {
		bool stat = false;
		if (num < (MAX_NUM_COM_PRD / 8)) {
			indCom_[num] = val;
			stat = true;
		}
		return stat;
	}

	/**	���������� ������� ��������� ���������� ������� �� ����� ���.
	 *
	 * 	@param num ����� �������.
	 * 	@return True- ��������� �����, False - �����.
	 */
	bool getIndCom(uint8_t num) const {
		// ����� ���������
		uint8_t pl = num / 8;
		// ����� ������ ���������
		num = num % 8;
		return (indCom_[pl] & (1 << num)) != 0;
	}

	/**	������������� ������� ��������� ����������� ������ �� ����� ���.
	 *
	 * 	������ ��� �������� ������ ��� ��������������� �������, ������� ��� -
	 * 	������� �������. ������������� ��� �������� ��� ��������� �����.
	 *
	 * 	@param num ����� ��������� ������ (0 - 8..1 �������, 1 - 16..9 � �.�.).
	 * 	@return ������� ��������� ������ ���������� ������.
	 */
	uint8_t getIndCom8(uint8_t num) const {
		uint8_t val = 0;
		if (num < (MAX_NUM_COM_PRD / 8))
			val = indCom_[num];
		return val;
	}

	/**	�������� ������� ������� ����������� ��������� ������ �� ����� ���,
	 *
	 * 	@retval True - ���� ����� ���������� ���� �� ����� �������.
	 * 	@retval False - ������� ����������� ������ ���.
	 */
	bool isIndCom() const {
		uint8_t ind = 0;

		for(uint8_t i = 0; i < (MAX_NUM_COM_PRM / 8); i++) {
			ind |= indCom_[i];
		}

		return (ind != 0);
	}

	// ���������� ������� � �������
	bool setNumJrnEntry(uint16_t val) {
		bool stat = false;

		overflow_ = (val & 0xC000) != 0;
		val &= 0x3FFF;

		if (val <= maxNumJrnEntry_) {
			numJrnEntry_ = val;
			stat = true;
		}
		return stat;
	}

	uint16_t getNumJrnEntry() const {
		return numJrnEntry_;
	}

	// ������������ ���-�� ������� � �������
	bool setMaxNumJrnEntry(uint16_t max) {
		bool stat = false;
		if (max <= 1024) {
			stat = true;
			maxNumJrnEntry_ = max;
		}
		return stat;
	}

	uint16_t getMaxNumJrnEntry() const {
		return maxNumJrnEntry_;
	}

private:
	// ���-�� ������ ���������
	uint8_t numCom_;

	// ��������� ��������� ����������� �� ����� ���
	uint8_t indCom_[MAX_NUM_COM_PRM / 8];

	// ���-�� ������� � �������
	uint16_t numJrnEntry_;

	// ������������ ���-�� ������� � �������
	uint16_t maxNumJrnEntry_;

	// ���� ������������ �������
	bool overflow_;
};

#endif /* PARAMPRM_H_ */
