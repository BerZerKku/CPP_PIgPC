/*
 * menuPunkt.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef MENUPUNKT_H_
#define MENUPUNKT_H_

#include "glbDefine.h"

/// ��������� ������ ����
class TMenuPunkt {

	/// ������������ ���-�� ������� � ����
	static const uint8_t  MAX_NUM_PUNKTS = 20;

public:
	/**	�����������.
	 */
	TMenuPunkt() {
		clear();

		for (uint_fast8_t i = 0; i < MAX_NUM_PUNKTS; i++)
			com_[i] = GB_COM_NO;
	}

	/**	���������� ������ ����, ����� ���������.
	 *
	 * 	@param name ��� ������.
	 * 	@param com ������� ��� ������� �� ���. �� ��������� - ���.
	 * 	@retval True � ������ ��������� ����������.
	 * 	@retval False � ������ ������ ����������. ��������, ���������� ������.
	 */
	bool add(PGM_P name, eGB_COM com=GB_COM_NO) {
		bool stat = false;
		if (cnt_ < MAX_NUM_PUNKTS) {
			name_[cnt_] = name;
			com_[cnt_] = com;
			cnt_++;
			stat = true;
		}
		return stat;
	}

	/**	���������� ������ ����, ����� ����� �������� �������.
	 *
	 * 	@param name	����� ������ (���).
	 * 	@param com ������� ��� ������� �� ���. �� ��������� - ���.
	 * 	@retval True � ������ ��������� ����������.
	 * 	@retval False � ������ ������ ����������. ��������, ���������� ������.
	 */
	bool add(uint8_t name, eGB_COM com=GB_COM_NO) {
		bool stat = false;
		if (cnt_ < MAX_NUM_PUNKTS) {
			number_[cnt_] = name;
			com_[cnt_] = com;
			cnt_++;
			stat = true;
		}
		return stat;
	}

	/** ������� �������� ������ ������� ����.
	 */
	void clear() {
		cnt_ = 0;
	}

	/** ������ ���������� ������ ������ ����.
	 *
	 * 	@param name ��� �����.
	 * 	@param com ������� ��� ������� �� ���.
	 * 	@param num ����� ������.
	 * 	@retval True � ������ ��������� ����������.
	 * 	@retval False � ������ ������ ����������. ��������, �� ������ �����.
	 */
	bool change(PGM_P name, eGB_COM com, uint8_t num) {
		bool stat = false;
		if (num < cnt_) {
			name_[num] = name;
			com_[num] = com;
			stat = true;
		}
		return stat;
	}

	/** ���������� �������� ��������� ������ ����.
	 *
	 *	@param num	����� ������.
	 * 	@return �������� ������.
	 */
	PGM_P getName(uint8_t num) {
//		if (num >= cnt_)
//			num = 0;
//		return name_[num];
		return ((num < cnt_) ? name_[num] : 0);
	}

	/**	���������� ����� ��������� ������� � ������ ���������� ������ ����.
	 *
	 *	@param num	����� ������.
	 * 	@return �������� ������.
	 */
	uint8_t getNumber(uint8_t num) {
		return ((num < cnt_) ? number_[num] : 0);
	}

	/** ���������� ������� ���������� ������ ����.
	 *
	 * 	@param num ����� ������.
	 * 	@return ������� ��� ������� �� ���.
	 */
	eGB_COM getCom(uint8_t num) {
		if (num >= cnt_)
			num = 0;
		return com_[num];
	}

	/** ���������� ������� ���-�� ������ ����.
	 *
	 * 	@return ������� ���-�� ������� ����.
	 * 	@retval 0 - �����.
	 */
	uint8_t getMaxNumPunkts() {
		return cnt_;
	}

private:
	/// ������� ���-�� �������
	uint8_t cnt_;

	/// ��������� �� ��� ������
	PGM_P name_[MAX_NUM_PUNKTS];

	/// ����� ������, ������������ � ���������
	uint8_t number_[MAX_NUM_PUNKTS];

	/// ������� ��� ������� �� ���, ����������� ��� ������� ������ ����
	eGB_COM com_[MAX_NUM_PUNKTS];
};


#endif /* MENUPUNKT_H_ */
