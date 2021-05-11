/*
 * enterParam.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef DATE_TIME_HPP_
#define DATE_TIME_HPP_

#include <stdint.h>

/// Класс для даты и времени
class TDateTime {
public:
    /// Флаг установки нового времени.
	bool setTimeBsp_;

    /// Конструктор.
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

    /// Возвращает секунды.
    uint8_t getSecond() const;

    /** Устанавливает секунды.
     *
     *  В BCD коде не проверяется корректность разрядов,
     *  только конечное значение.
     *
     *  @param[in] val Секунды.
     *  @param[in] bcd true если значение в bcd коде, иначе false.
     *  @return true если значение установлено, иначе false.
     */
    bool setSecond(uint8_t val, bool bcd);

    /// Возвращает минуты.
    uint8_t getMinute() const;

    /** Устанавливает минуты.
     *
     *  В BCD коде не проверяется корректность разрядов,
     *  только конечное значение.
     *
     *  @param[in] val Минуты.
     *  @param[in] bcd true если значение в bcd коде, иначе false.
     *  @return true если значение установлено, иначе false.
     */
    bool setMinute(uint8_t val, bool bcd);

    /// Возвращает часы.
    uint8_t getHour() const;

    /** Устанавливает часы.
     *
     *  В BCD коде не проверяется корректность разрядов,
     *  только конечное значение.
     *
     *  @param[in] val Часы.
     *  @param[in] bcd true если значение в bcd коде, иначе false.
     *  @return true если значение установлено, иначе false.
     */
    bool setHour(uint8_t val, bool bcd);

    /// Возвращает день.
    uint8_t getDay() const;

    /** Устанавливает день месяца.
     *
     *  В BCD коде не проверяется корректность разрядов,
     *  только конечное значение.
     *
     *  @param[in] val День месяца.
     *  @param[in] bcd true если значение в bcd коде, иначе false.
     *  @return true если значение установлено, иначе false.
     */
    bool setDay(uint8_t val, bool bcd);

    /** Возвращает количество дней для заданного месяца и года.
     *
     *  Распределение високосных годов:
     *  - год, номер которого кратен 400, — високосный;
     *  - остальные годы, номер которых кратен 100, — невисокосные (2100 и т.д.)
     *  - остальные годы, номер которых кратен 4, — високосные.
     *
     *  @param[in] month Месяц (если 0, то берется текущий).
     *  @param[in] year Год (если 0, то берется текущий).
     *  @return Количество дней в месяце.
     */
    uint8_t getNumDaysInMonth(uint8_t month = 0, uint8_t year = 0) const;

    /// Возвращает месяц.
    uint8_t getMonth() const;

    /** Устанавливает месяц.
     *
     *  В BCD коде не проверяется корректность разрядов,
     *  только конечное значение.
     *
     *  @param[in] val Месяц.
     *  @param[in] bcd true если значение в bcd коде, иначе false.
     *  @return true если значение установлено, иначе false.
     */
    bool setMonth(uint8_t val, bool bcd);

    /// Возвращает год.
    uint8_t getYear() const;

    /** Устанавливает год.
     *
     *  Диапазон значений [20..99].
     *
     *  @param[in] val Год.
     *  @param[in] bcd true если значение в bcd коде, иначе false.
     *  @return true если значение установлено, иначе false.
     */
    bool setYear(uint8_t val, bool bcd);

    /// Возвращает миллисекунды.
    uint16_t getMsSecond() const;

    /** Устанавливает миллисекунды
     *
     *  @param[in] val Миллисекунды.
     *  @return true если значение установлено, иначе false.
     */
	bool setMsSecond(uint16_t val);

    /// Возвращает день недели.
    uint8_t getDayOfWeek() const;

    /** Устанавливает день недели.
     *
     *  @param[in] val Значение в BCD коде.
     *  @return true если значение установлено, иначе false.
     */
    bool setDayOfWeek(uint8_t val);

    /** Преобразует целое значение в bcd вид.
     *
     *  FIXME Сделать проверку диапазона входных значений.
     *
     *  @param val Целое значение [0..99].
     *  @return Значение в bcd виде.
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

    /** Преобразует из значение из bcd в целое.
     *
     *  Если значение не корректно, т.е. значение хотя бы одной тетрады
     *  больше '9', то принятое значние не меняется.
     *
     *  @param[in/out] val Значение в bcd виде на входе, на выходе bin.
     *  @return Целое значение.
     */
    bool bcdToBin(uint8_t &val) const;
};

#endif /* DATE_TIME_HPP_ */
