/*
 * paramUart.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMUART_H_
#define PARAMUART_H_

#include <stdint.h>

/// Интерфейс связи
class TInterface {
public:

    // интерфейс связи
    enum INTERFACE {
        MIN = 0,	//
        USB = 0,	// подключение через USB на передней панели
        RS485,		// подключение через 485 интерфейс на задней панели
        MAX			//
    };

    /**	Конструктор.
     * 	По умолчанию устанваливает связь по USB.
     */
    TInterface()
    {
        changed = false;
        interface_ = MAX;
    }

    /**	Запись.
     * 	@param val Интерфейс связи.
     * 	@return False в случае ошибочного значения.
     */
    bool set(TInterface::INTERFACE val) {
        bool stat = false;
        if ((val >= MIN) && (val < MAX)) {
            changed |= (interface_ != val);
            interface_ = val;
            stat = true;
        }
        return stat;
    }

    /**	Чтение.
     * 	@return Интерфейс связи.
     */
    INTERFACE get() const {
        return interface_;
    }

    /** Проверяет изменение параметра.
     *
     *  После проверки флаг изменения сбрасывается.
     *
     *  @return true если параметр был изменен, иначе false.
     */
    bool isChanged() {
        bool tchanged = changed;
        changed = false;
        return tchanged;
    }

private:
    // флаг изменения параметра
    bool changed;
    // Кол-во битов данных
    INTERFACE interface_;
};

/// Протокол связи.
class TProtocol{
public:
    enum PROTOCOL {
        MIN 	= 0,	//
        STANDART= 0,	// наш внутренний протокол
        MODBUS,			// MODBUS
        IEC_101,		// МЭК-101
        MAX				//
    };

    /** Контруктор.
     * 	По умолчанию устанавливает MODBUS.
     */
    TProtocol() {
        protocol_ = MAX;

#ifdef  DEBUG
        protocol_ = IEC_101;
#endif
    }

    /**	Запись.
     * 	@param val Протокол связи.
     * 	@return False в случае ошибочного значения.
     */
    bool set(PROTOCOL val) {
        bool stat = false;
        if ((val >= MIN) && (val < MAX)) {
            changed |= (protocol_ != val);
            protocol_ = val;
            stat = true;
        }
        return stat;
    }

    /** Чтение.
     * 	@return Протокол связи
     */
    PROTOCOL get() const {
        return protocol_;
    }
    /** Проверяет изменение параметра.
     *
     *  После проверки флаг изменения сбрасывается.
     *
     *  @return true если параметр был изменен, иначе false.
     */
    bool isChanged() {
        bool tchanged = changed;
        changed = false;
        return tchanged;
    }

private:
    // флаг изменения параметра
    bool changed;
    // Протокол связи
    PROTOCOL protocol_;
};

/// Скорость передачи.
class TBaudRate {
public:
    /// скорость передачи
    enum BAUD_RATE {
        MIN = 0,	//
        _600 = 0,	//
        _1200,		//
        _2400,		//
        _4800,		//
        _9600,		//
        _19200,		//
        MAX			//
    };

    /**	Конструктор.
     * 	По умолчанию устанавливает скорость 19200 бит/с.
     */
    TBaudRate() {
        baudRate_ = MAX;
    }

    /**	Запись.
     * 	@param val Скорость передачи.
     * 	@return
     * 	@retval False в случае ошибочного значения.
     */
    bool set(TBaudRate::BAUD_RATE val) {
        bool stat = false;
        if ((val >= MIN) && (val < MAX)) {
            changed |= (baudRate_ != val);
            baudRate_ = val;
            stat = true;
        }
        return stat;
    }

    /**	Чтение.
     * 	@return Скорость передачи.
     */
    TBaudRate::BAUD_RATE get() const {
        return baudRate_;
    }

    /**	Возвращает численное значение текущей скорости передачи.
     * 	@return Скорость передачи.
     * 	@retval 0 В случае ошибки
     */
    uint16_t getValue() const {
        uint16_t ibaud = 0;
        switch(baudRate_) {
            case TBaudRate::_600:
                ibaud = 600;
                break;
            case TBaudRate::_1200:
                ibaud = 1200;
                break;
            case TBaudRate::_2400:
                ibaud = 2400;
                break;
            case TBaudRate::_4800:
                ibaud = 4800;
                break;
            case TBaudRate::_9600:
                ibaud = 9600;
                break;
            case TBaudRate::_19200:
                ibaud = 19200;
                break;
            case TBaudRate::MAX:	// заглушка
                break;
        }
        return ibaud;
    }

    /** Проверяет изменение параметра.
     *
     *  После проверки флаг изменения сбрасывается.
     *
     *  @return true если параметр был изменен, иначе false.
     */
    bool isChanged() {
        bool tchanged = changed;
        changed = false;
        return tchanged;
    }

private:
    // флаг изменения параметра
    bool changed;
    // Скорость передачи
    BAUD_RATE baudRate_;
};


/// Количество битов данных
class TDataBits {
public:
    /// кол-во байт данных
    enum DATA_BITS {
        MIN = 0,		//
        _8 = 0,			// 8 бит данных
        MAX				//
    };

    /**	Конструктор.
     * 	По умолчанию устанваливает 8 бит данных.
     */
    TDataBits() {
        dataBits_ = MAX;
    }

    /**	Запись
     * 	@param val Кол-во битов данных.
     * 	@return False в случае ошибочного значения.
     */
    bool set(DATA_BITS val) {
        bool stat = false;
        if ((val >= MIN) && (val < MAX)) {
            changed |= (dataBits_ != val);
            dataBits_ = val;
            stat = true;
        }
        return stat;
    }

    /**	Чтение
     * 	@param Нет
     * 	@return Кол-во битов данных
     */
    DATA_BITS get() const {
        return dataBits_;
    }

    /** Проверяет изменение параметра.
     *
     *  После проверки флаг изменения сбрасывается.
     *
     *  @return true если параметр был изменен, иначе false.
     */
    bool isChanged() {
        bool tchanged = changed;
        changed = false;
        return tchanged;
    }

private:
    // флаг изменения параметра
    bool changed;
    // Кол-во битов данных
    DATA_BITS dataBits_;
};

///	Протокол контроля четности
class TParity {
public:
    /// четность
    enum PARITY {
        MIN = 0,		//
        NONE = 0,	// нет
        EVEN,		// число установленных битов всегда четно
        ODD,			// число установленных битов всегда нечетно
        MAX
    };

    /**	Конструктор.
     * 	По умолчанию проверка четности отключена.
     */
    TParity() {
        parity_ = MAX;
    }

    /**	Запись
     * 	@param val Контроль четности
     * 	@return False в случае ошибочного значения
     */
    bool set(TParity::PARITY val) {
        bool stat = false;
        if ((val >= MIN) && (val < MAX)) {
            changed |= (parity_ != val);
            parity_ = val;
            stat = true;
        }
        return stat;
    }

    /**	Чтение
     * 	@param Нет
     * 	@return Контроль четности
     */
    TParity::PARITY get() const {
        return parity_;
    }

    /** Проверяет изменение параметра.
     *
     *  После проверки флаг изменения сбрасывается.
     *
     *  @return true если параметр был изменен, иначе false.
     */
    bool isChanged() {
        bool tchanged = changed;
        changed = false;
        return tchanged;
    }

private:
    // флаг изменения параметра
    bool changed;
    // Контроль четности
    PARITY parity_;
};

///	Число стоповых битов
class TStopBits {
public:
    /// кол-во стоп-бит
    enum STOP_BITS {
        MIN = 0,		//
        ONE = 0,		// один стоп-бит
        TWO,			// два стоп-бита
        MAX				//
    };

    /**	Конструктор.
     * 	По умолчанию 2 стоп бита.
     */
    TStopBits() {
        stopBits_ = MAX;
    }

    /**	Запись
     * 	@param val Число стоповых битов
     * 	@return False в случае ошибочного значения
     */
    bool set(STOP_BITS val) {
        bool stat = false;
        if ((val >= MIN) && (val < MAX)) {
            changed |= (stopBits_ != val);
            stopBits_ = val;
            stat = true;
        }
        return stat;
    }

    /**	Чтение
     * 	@param Нет
     * 	@return  Число стоповых битов
     */
    STOP_BITS get() const {
        return stopBits_;
    }

    /** Проверяет изменение параметра.
     *
     *  После проверки флаг изменения сбрасывается.
     *
     *  @return true если параметр был изменен, иначе false.
     */
    bool isChanged() {
        bool tchanged = changed;
        changed = false;
        return tchanged;
    }

private:
    // флаг изменения параметра
    bool changed;
    // Число стоповых битов
    STOP_BITS stopBits_;
};

/// Сетевой адрес.
class TNetAddress {
public:
    /// Диапазон сетевых адресов.
    enum NET_ADDRESS {
        MIN = 1,
        MAX = 247
    };

    TNetAddress() {
        netAddress_ = MAX+1;
        changed = false;
    }

    /**	Устанавливает сетевой адрес.
     *
     * 	@param[in] val Сетевой адрес.
     * 	@return true если сетевой адрес установлен, иначе false.
     */
    bool set(uint8_t val) {
        bool stat = false;
        if ((val >= MIN) && (val <= MAX)) {
            changed |= (netAddress_ != val);
            netAddress_ = val;
            stat = true;
        }
        return stat;
    }

    /**	Возвращает сетевой адрес.
     *
     * 	@return Сетевой адрес.
     */
    uint8_t get() const {
        return netAddress_;
    }

    /** Проверяет изменение параметра.
     *
     *  После проверки флаг изменения сбрасывается.
     *
     *  @return true если параметр был изменен, иначе false.
     */
    bool isChanged() {
        bool tchanged = changed;
        changed = false;
        return tchanged;
    }

private:
    // флаг изменения параметра
    bool changed;
    // Сетевой адрес.
    uint8_t netAddress_;
};

/// структура параметров работы с последовательным портом
class TUartData {
public:
    TUartData() {
        Interface.set(TInterface::USB);
        Protocol.set(TProtocol::STANDART);
        BaudRate.set(TBaudRate::_19200);
        DataBits.set(TDataBits::_8);
        Parity.set(TParity::NONE);
        StopBits.set(TStopBits::TWO);
        NetAddress.set(1);
    }

    /// Интерфейс связи
    TInterface Interface;
    /// Протокол связи
    TProtocol Protocol;
    /// Скорость передачи
    TBaudRate BaudRate;
    /// Количество битов данных
    TDataBits DataBits;
    /// Протокол контроля четности
    TParity Parity;
    /// Количество стоп битов
    TStopBits StopBits;
    /// Сетевой адрес.
    TNetAddress NetAddress;
};

#endif /* PARAMUART_H_ */
