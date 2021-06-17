/*
 * dateTime.h
 *
 *  Created on: 15 июн. 2021 г.
 *      Author: Shcheblykin
 */

#ifndef DATE_TIME_H_
#define DATE_TIME_H_

#include <stdint.h>

/**
 * @brief Класс даты и времени.
 */
class TDateTime
{
public:
    // флаг установки нового времени
    bool setTimeBsp_;

    /// Конструктор.
    TDateTime();

    /**
     * @brief Устанавливает секунды.
     *
     * В случае ошибочного значения будет установлено 60.
     *
     * @param[in] val [0..59]
     * @param[in] bcd true если значение в bcd формате, иначе false.
     * @return true если значение установлено, иначе false.
     */
    bool setSecond(uint8_t val, bool bcd);

    /**
     * @brief Возвращает секунды.
     * @return Секунды [0..60].
     */
    uint8_t getSecond() const { return second_; }

    /**
     * @brief Устанавливает минуты.
     *
     * В случае ошибочного значения будет установлено 60.
     *
     * @param[in] val [0..59]
     * @param[in] bcd true если значение в bcd формате, иначе false.
     * @return true если значение установлено, иначе false.
     */
    bool setMinute(uint8_t val, bool bcd);

    /**
     * @brief Возвращает минуты.
     * @return Минуты [0..60].
     */
    uint8_t getMinute() const { return minute_; }

    /**
     * @brief Устанавливает часы.
     *
     * В случае ошибочного значения будет установлено 24.
     *
     * @param[in] val [0..23]
     * @param[in] bcd true если значение в bcd формате, иначе false.
     * @return true если значение установлено, иначе false.
     */
    bool setHour(uint8_t val, bool bcd);

    /**
     * @brief Возвращает часы.
     * @return Часы [0..24].
     */
    uint8_t getHour() const { return hour_; }

    /**
     * @brief Устанавливает день.
     *
     * Проводится проверка на количество дней в текущем месяце.
     * В случае ошибочного значения будет установлено 32.
     *
     * @param[in] val [1..31]
     * @param[in] bcd true если значение в bcd формате, иначе false.
     * @return true если значение установлено, иначе false.
     */
    bool setDay(uint8_t val, bool bcd);

    /**
     * @brief Возвращает день.
     * @return День [0..32].
     */
    uint8_t getDay() const { return day_; }

    /**
     * @brief Устанавливает месяц.
     *
     * В случае ошибочного значения будет установлено 13.
     *
     * @param[in] val [1..12]
     * @param[in] bcd true если значение в bcd формате, иначе false.
     * @return true если значение установлено, иначе false.
     */
    bool setMonth(uint8_t val, bool bcd);

    /**
     * @brief Возвращает месяц.
     * @return День [1..13].
     */
    uint8_t getMonth() const { return month_; }

    /**
     * @brief Устанавливает год.
     *
     * В случае ошибочного значения будет установлен 0.
     *
     * @param[in] val [0..99]
     * @param[in] bcd true если значение в bcd формате, иначе false.
     * @return true если значение установлено, иначе false.
     */
    bool setYear(uint8_t val, bool bcd);

    /**
     * @brief Возвращает год.
     * @return День [0..99].
     */
    uint8_t getYear() const { return year_; }

    /**
     * @brief Устанавливает миллисекунды.
     *
     * В случае ошибочного значения будет установлена 1000.
     *
     * @param[in] val [0..999]
     * @return true если значение установлено, иначе false.
     */
    bool setMsSecond(uint16_t val);

    /**
     * @brief Возвращает миллисекунды.
     * @return День [0..1000].
     */
    uint16_t getMsSecond() const { return msSecond_; }

    /**
     * @brief Устанавливает день недели.
     *
     * В случае ошибочного значения будет установлен 0.
     *
     * @param[in] val [1..7]
     * @return true если значение установлено, иначе false.
     */
    bool setDayOfWeek(uint8_t val);

    /**
     * @brief Возвращает день недели.
     * @return День [1..8].
     */
    uint8_t getDayOfWeek() const { return dayWeek_; }

    /**
     * @brief Возвращает количество дней в месяце года.
     *
     * В случае ошибочного значения месяца или года будет возвращен 0.
     * Если значение месяца или года равно 0, то будет подставлено имеющееся
     * значение.
     *
     * @param[in] month Месяц [1..12].
     * @param[in] year Год [1..99].
     * @return Количество дней [0..31].
     */
    uint8_t getNumDaysInMonth(uint8_t month = 0, uint8_t year = 0) const;

    /**
     * @brief Преобразует целое значение в bcd вид.
     *
     * В случае ошибки принятое значение не меняется.
     *
     * @param[in/out] val Целое значение на входе [0..99] и bcd на выходе.
     * @return true если значение преобразовано, иначе false.
     */
    bool binToBcd(uint8_t &val) const;

    /**
     * @brief Преобразует значение bcd в целое.
     *
     * Если значение не корректно, т.е. значение хотя бы одной тетрады
     *  больше '9', то принятое значние не меняется.
     *
     * @param[in/out] val Значение bcd на входе [0x00..0x99] и целое на выходе.
     * @return true если значение преобразовано, иначе false.
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
