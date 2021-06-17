/*
 * paramUart.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMUART_H_
#define PARAMUART_H_

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
	TInterface() {

		interface_ = USB;
#ifdef  DEBUG
		interface_ = RS485;
#endif
	}

	/**	Запись.
	 * 	@param val Интерфейс связи.
	 * 	@return False в случае ошибочного значения.
	 */
	bool set(TInterface::INTERFACE val) {
		bool stat = false;
		if ((val >= MIN) && (val < MAX)) {
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

private:
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
		protocol_ = STANDART;

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

private:
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
		baudRate_ = _19200;
	}

	/**	Запись.
	 * 	@param val Скорость передачи.
	 * 	@return
	 * 	@retval False в случае ошибочного значения.
	 */
	bool set(TBaudRate::BAUD_RATE val) {
		bool stat = false;
		if ((val >= MIN) && (val < MAX)) {
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

private:
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
		dataBits_ = _8;
	}

	/**	Запись
	 * 	@param val Кол-во битов данных.
	 * 	@return False в случае ошибочного значения.
	 */
	bool set(DATA_BITS val) {
		bool stat = false;
		if ((val >= MIN) && (val < MAX)) {
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

private:
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
		parity_ = NONE;
	}

	/**	Запись
	 * 	@param val Контроль четности
	 * 	@return False в случае ошибочного значения
	 */
	bool set(TParity::PARITY val) {
		bool stat = false;
		if ((val >= MIN) && (val < MAX)) {
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

private:
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
		stopBits_ = TWO;
	}

	/**	Запись
	 * 	@param val Число стоповых битов
	 * 	@return False в случае ошибочного значения
	 */
	bool set(STOP_BITS val) {
		bool stat = false;
		if ((val >= MIN) && (val < MAX)) {
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

private:
	// Число стоповых битов
	STOP_BITS stopBits_;
};


/// структура параметров работы с последовательным портом
class TUartData {
public:
	TUartData() {
		Interface.set(TInterface::USB);
		Protocol.set(TProtocol::MODBUS);
		BaudRate.set(TBaudRate::_19200);
		DataBits.set(TDataBits::_8);
		Parity.set(TParity::EVEN);
		StopBits.set(TStopBits::ONE);
#ifdef IEC101
		Interface.set(TInterface::RS485);
		Protocol.set(TProtocol::IEC_101);
#endif
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
};

#endif /* PARAMUART_H_ */
