/*
 * dateTime.h
 *
 *  Created on: 15 ���. 2021 �.
 *      Author: Shcheblykin
 */

#ifndef DATE_TIME_H_
#define DATE_TIME_H_

#include <stdint.h>

/**
 * @brief ����� ���� � �������.
 */
class TDateTime
{
public:
    // ���� ��������� ������ �������
    bool setTimeBsp_;

    /// �����������.
    TDateTime();

    /**
     * @brief ������������� �������.
     *
     * � ������ ���������� �������� ����� ����������� 60.
     *
     * @param[in] val [0..59]
     * @param[in] bcd true ���� �������� � bcd �������, ����� false.
     * @return true ���� �������� �����������, ����� false.
     */
    bool setSecond(uint8_t val, bool bcd);

    /**
     * @brief ���������� �������.
     * @return ������� [0..60].
     */
    uint8_t getSecond() const { return second_; }

    /**
     * @brief ������������� ������.
     *
     * � ������ ���������� �������� ����� ����������� 60.
     *
     * @param[in] val [0..59]
     * @param[in] bcd true ���� �������� � bcd �������, ����� false.
     * @return true ���� �������� �����������, ����� false.
     */
    bool setMinute(uint8_t val, bool bcd);

    /**
     * @brief ���������� ������.
     * @return ������ [0..60].
     */
    uint8_t getMinute() const { return minute_; }

    /**
     * @brief ������������� ����.
     *
     * � ������ ���������� �������� ����� ����������� 24.
     *
     * @param[in] val [0..23]
     * @param[in] bcd true ���� �������� � bcd �������, ����� false.
     * @return true ���� �������� �����������, ����� false.
     */
    bool setHour(uint8_t val, bool bcd);

    /**
     * @brief ���������� ����.
     * @return ���� [0..24].
     */
    uint8_t getHour() const { return hour_; }

    /**
     * @brief ������������� ����.
     *
     * ���������� �������� �� ���������� ���� � ������� ������.
     * � ������ ���������� �������� ����� ����������� 32.
     *
     * @param[in] val [1..31]
     * @param[in] bcd true ���� �������� � bcd �������, ����� false.
     * @return true ���� �������� �����������, ����� false.
     */
    bool setDay(uint8_t val, bool bcd);

    /**
     * @brief ���������� ����.
     * @return ���� [0..32].
     */
    uint8_t getDay() const { return day_; }

    /**
     * @brief ������������� �����.
     *
     * � ������ ���������� �������� ����� ����������� 13.
     *
     * @param[in] val [1..12]
     * @param[in] bcd true ���� �������� � bcd �������, ����� false.
     * @return true ���� �������� �����������, ����� false.
     */
    bool setMonth(uint8_t val, bool bcd);

    /**
     * @brief ���������� �����.
     * @return ���� [1..13].
     */
    uint8_t getMonth() const { return month_; }

    /**
     * @brief ������������� ���.
     *
     * � ������ ���������� �������� ����� ���������� 0.
     *
     * @param[in] val [0..99]
     * @param[in] bcd true ���� �������� � bcd �������, ����� false.
     * @return true ���� �������� �����������, ����� false.
     */
    bool setYear(uint8_t val, bool bcd);

    /**
     * @brief ���������� ���.
     * @return ���� [0..99].
     */
    uint8_t getYear() const { return year_; }

    /**
     * @brief ������������� ������������.
     *
     * � ������ ���������� �������� ����� ����������� 1000.
     *
     * @param[in] val [0..999]
     * @return true ���� �������� �����������, ����� false.
     */
    bool setMsSecond(uint16_t val);

    /**
     * @brief ���������� ������������.
     * @return ���� [0..1000].
     */
    uint16_t getMsSecond() const { return msSecond_; }

    /**
     * @brief ������������� ���� ������.
     *
     * � ������ ���������� �������� ����� ���������� 0.
     *
     * @param[in] val [1..7]
     * @return true ���� �������� �����������, ����� false.
     */
    bool setDayOfWeek(uint8_t val);

    /**
     * @brief ���������� ���� ������.
     * @return ���� [1..8].
     */
    uint8_t getDayOfWeek() const { return dayWeek_; }

    /**
     * @brief ���������� ���������� ���� � ������ ����.
     *
     * � ������ ���������� �������� ������ ��� ���� ����� ��������� 0.
     * ���� �������� ������ ��� ���� ����� 0, �� ����� ����������� ���������
     * ��������.
     *
     * @param[in] month ����� [1..12].
     * @param[in] year ��� [1..99].
     * @return ���������� ���� [0..31].
     */
    uint8_t getNumDaysInMonth(uint8_t month = 0, uint8_t year = 0) const;

    /**
     * @brief ����������� ����� �������� � bcd ���.
     *
     * � ������ ������ �������� �������� �� ��������.
     *
     * @param[in/out] val ����� �������� �� ����� [0..99] � bcd �� ������.
     * @return true ���� �������� �������������, ����� false.
     */
    bool binToBcd(uint8_t &val) const;

    /**
     * @brief ����������� �������� bcd � �����.
     *
     * ���� �������� �� ���������, �.�. �������� ���� �� ����� �������
     *  ������ '9', �� �������� ������� �� ��������.
     *
     * @param[in/out] val �������� bcd �� ����� [0x00..0x99] � ����� �� ������.
     * @return true ���� �������� �������������, ����� false.
     */
    bool bcdToBin(uint8_t &val) const;

private:
    uint16_t msSecond_;
    uint8_t  second_;
    uint8_t  minute_;
    uint8_t  hour_;
    uint8_t  dayWeek_;
    uint8_t  day_;
    uint8_t  month_;
    uint8_t  year_;
};

#endif /* DATE_TIME_H_ */
