/*
 * enterParam.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef DATE_TIME_HPP_
#define DATE_TIME_HPP_

#include <stdint.h>

/// ����� ��� ���� � �������
class TDateTime {
public:
    /// ���� ��������� ������ �������.
	bool setTimeBsp_;

    /// �����������.
	TDateTime() {
		msSecond_ = 0;
		second_ = 0;
		minute_ = 0;
		hour_ = 0;
		day_ = 1;
		month_ = 1;
		year_ = 0;
        dayWeek_ = 1;

		setTimeBsp_ = false;
	}

    /// ���������� �������.
    uint8_t getSecond() const;

    /** ������������� �������.
     *
     *  � BCD ���� �� ����������� ������������ ��������,
     *  ������ �������� ��������.
     *
     *  @param[in] val �������.
     *  @param[in] bcd true ���� �������� � bcd ����, ����� false.
     *  @return true ���� �������� �����������, ����� false.
     */
    bool setSecond(uint8_t val, bool bcd);

    /// ���������� ������.
    uint8_t getMinute() const;

    /** ������������� ������.
     *
     *  � BCD ���� �� ����������� ������������ ��������,
     *  ������ �������� ��������.
     *
     *  @param[in] val ������.
     *  @param[in] bcd true ���� �������� � bcd ����, ����� false.
     *  @return true ���� �������� �����������, ����� false.
     */
    bool setMinute(uint8_t val, bool bcd);

    /// ���������� ����.
    uint8_t getHour() const;

    /** ������������� ����.
     *
     *  � BCD ���� �� ����������� ������������ ��������,
     *  ������ �������� ��������.
     *
     *  @param[in] val ����.
     *  @param[in] bcd true ���� �������� � bcd ����, ����� false.
     *  @return true ���� �������� �����������, ����� false.
     */
    bool setHour(uint8_t val, bool bcd);

    /// ���������� ����.
    uint8_t getDay() const;

    /** ������������� ���� ������.
     *
     *  � BCD ���� �� ����������� ������������ ��������,
     *  ������ �������� ��������.
     *
     *  @param[in] val ���� ������.
     *  @param[in] bcd true ���� �������� � bcd ����, ����� false.
     *  @return true ���� �������� �����������, ����� false.
     */
    bool setDay(uint8_t val, bool bcd);

    /** ���������� ���������� ���� ��� ��������� ������ � ����.
     *
     *  ������������� ���������� �����:
     *  - ���, ����� �������� ������ 400, � ����������;
     *  - ��������� ����, ����� ������� ������ 100, � ������������ (2100 � �.�.)
     *  - ��������� ����, ����� ������� ������ 4, � ����������.
     *
     *  @param[in] month ����� (���� 0, �� ������� �������).
     *  @param[in] year ��� (���� 0, �� ������� �������).
     *  @return ���������� ���� � ������.
     */
    uint8_t getNumDaysInMonth(uint8_t month = 0, uint8_t year = 0) const;

    /// ���������� �����.
    uint8_t getMonth() const;

    /** ������������� �����.
     *
     *  � BCD ���� �� ����������� ������������ ��������,
     *  ������ �������� ��������.
     *
     *  @param[in] val �����.
     *  @param[in] bcd true ���� �������� � bcd ����, ����� false.
     *  @return true ���� �������� �����������, ����� false.
     */
    bool setMonth(uint8_t val, bool bcd);

    /// ���������� ���.
    uint8_t getYear() const;

    /** ������������� ���.
     *
     *  �������� �������� [20..99].
     *
     *  @param[in] val ���.
     *  @param[in] bcd true ���� �������� � bcd ����, ����� false.
     *  @return true ���� �������� �����������, ����� false.
     */
    bool setYear(uint8_t val, bool bcd);

    /// ���������� ������������.
    uint16_t getMsSecond() const;

    /** ������������� ������������
     *
     *  @param[in] val ������������.
     *  @return true ���� �������� �����������, ����� false.
     */
	bool setMsSecond(uint16_t val);

    /// ���������� ���� ������.
    uint8_t getDayOfWeek() const;

    /** ������������� ���� ������.
     *
     *  @param[in] val �������� � BCD ����.
     *  @return true ���� �������� �����������, ����� false.
     */
    bool setDayOfWeek(uint8_t val);

    /** ����������� ����� �������� � bcd ���.
     *
     *  FIXME ������� �������� ��������� ������� ��������.
     *
     *  @param val ����� �������� [0..99].
     *  @return �������� � bcd ����.
     */
    uint8_t binToBcd(uint8_t val) const;

private:
	uint16_t msSecond_;
	uint8_t second_;
	uint8_t minute_;
	uint8_t hour_;
	uint8_t dayWeek_;
	uint8_t day_;
	uint8_t month_;
	uint8_t year_;

    /** ����������� �� �������� �� bcd � �����.
     *
     *  ���� �������� �� ���������, �.�. �������� ���� �� ����� �������
     *  ������ '9', �� �������� ������� �� ��������.
     *
     *  @param[in/out] val �������� � bcd ���� �� �����, �� ������ bin.
     *  @return ����� ��������.
     */
    bool bcdToBin(uint8_t &val) const;
};

#endif /* DATE_TIME_HPP_ */
