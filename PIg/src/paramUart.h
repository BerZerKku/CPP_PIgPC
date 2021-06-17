/*
 * paramUart.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMUART_H_
#define PARAMUART_H_

/// ��������� �����
class TInterface {
public:

	// ��������� �����
	enum INTERFACE {
		MIN = 0,	//
		USB = 0,	// ����������� ����� USB �� �������� ������
		RS485,		// ����������� ����� 485 ��������� �� ������ ������
		MAX			//
	};

	/**	�����������.
	 * 	�� ��������� ������������� ����� �� USB.
	 */
	TInterface() {

		interface_ = USB;
#ifdef  DEBUG
		interface_ = RS485;
#endif
	}

	/**	������.
	 * 	@param val ��������� �����.
	 * 	@return False � ������ ���������� ��������.
	 */
	bool set(TInterface::INTERFACE val) {
		bool stat = false;
		if ((val >= MIN) && (val < MAX)) {
			interface_ = val;
			stat = true;
		}
		return stat;
	}

	/**	������.
	 * 	@return ��������� �����.
	 */
	INTERFACE get() const {
		return interface_;
	}

private:
	// ���-�� ����� ������
	INTERFACE interface_;
};

/// �������� �����.
class TProtocol{
public:
	enum PROTOCOL {
		MIN 	= 0,	//
		STANDART= 0,	// ��� ���������� ��������
		MODBUS,			// MODBUS
		IEC_101,		// ���-101
		MAX				//
	};

	/** ����������.
	 * 	�� ��������� ������������� MODBUS.
	 */
	TProtocol() {
		protocol_ = STANDART;

#ifdef  DEBUG
		protocol_ = IEC_101;
#endif
	}

	/**	������.
	 * 	@param val �������� �����.
	 * 	@return False � ������ ���������� ��������.
	 */
	bool set(PROTOCOL val) {
		bool stat = false;
		if ((val >= MIN) && (val < MAX)) {
			protocol_ = val;
			stat = true;
		}
		return stat;
	}

	/** ������.
	 * 	@return �������� �����
	 */
	PROTOCOL get() const {
		return protocol_;
	}

private:
	// �������� �����
	PROTOCOL protocol_;
};

/// �������� ��������.
class TBaudRate {
public:
	/// �������� ��������
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

	/**	�����������.
	 * 	�� ��������� ������������� �������� 19200 ���/�.
	 */
	TBaudRate() {
		baudRate_ = _19200;
	}

	/**	������.
	 * 	@param val �������� ��������.
	 * 	@return
	 * 	@retval False � ������ ���������� ��������.
	 */
	bool set(TBaudRate::BAUD_RATE val) {
		bool stat = false;
		if ((val >= MIN) && (val < MAX)) {
			baudRate_ = val;
			stat = true;
		}
		return stat;
	}

	/**	������.
	 * 	@return �������� ��������.
	 */
	TBaudRate::BAUD_RATE get() const {
		return baudRate_;
	}

	/**	���������� ��������� �������� ������� �������� ��������.
	 * 	@return �������� ��������.
	 * 	@retval 0 � ������ ������
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
			case TBaudRate::MAX:	// ��������
			break;
		}
		return ibaud;
	}

private:
	// �������� ��������
	BAUD_RATE baudRate_;
};


/// ���������� ����� ������
class TDataBits {
public:
	/// ���-�� ���� ������
	enum DATA_BITS {
		MIN = 0,		//
		_8 = 0,			// 8 ��� ������
		MAX				//
	};

	/**	�����������.
	 * 	�� ��������� ������������� 8 ��� ������.
	 */
	TDataBits() {
		dataBits_ = _8;
	}

	/**	������
	 * 	@param val ���-�� ����� ������.
	 * 	@return False � ������ ���������� ��������.
	 */
	bool set(DATA_BITS val) {
		bool stat = false;
		if ((val >= MIN) && (val < MAX)) {
			dataBits_ = val;
			stat = true;
		}
		return stat;
	}

	/**	������
	 * 	@param ���
	 * 	@return ���-�� ����� ������
	 */
	DATA_BITS get() const {
		return dataBits_;
	}

private:
	// ���-�� ����� ������
	DATA_BITS dataBits_;
};

///	�������� �������� ��������
class TParity {
public:
	/// ��������
	enum PARITY {
		MIN = 0,		//
		NONE = 0,	// ���
		EVEN,		// ����� ������������� ����� ������ �����
		ODD,			// ����� ������������� ����� ������ �������
		MAX
	};

	/**	�����������.
	 * 	�� ��������� �������� �������� ���������.
	 */
	TParity() {
		parity_ = NONE;
	}

	/**	������
	 * 	@param val �������� ��������
	 * 	@return False � ������ ���������� ��������
	 */
	bool set(TParity::PARITY val) {
		bool stat = false;
		if ((val >= MIN) && (val < MAX)) {
			parity_ = val;
			stat = true;
		}
		return stat;
	}

	/**	������
	 * 	@param ���
	 * 	@return �������� ��������
	 */
	TParity::PARITY get() const {
		return parity_;
	}

private:
	// �������� ��������
	PARITY parity_;
};

///	����� �������� �����
class TStopBits {
public:
	/// ���-�� ����-���
	enum STOP_BITS {
		MIN = 0,		//
		ONE = 0,		// ���� ����-���
		TWO,			// ��� ����-����
		MAX				//
	};

	/**	�����������.
	 * 	�� ��������� 2 ���� ����.
	 */
	TStopBits() {
		stopBits_ = TWO;
	}

	/**	������
	 * 	@param val ����� �������� �����
	 * 	@return False � ������ ���������� ��������
	 */
	bool set(STOP_BITS val) {
		bool stat = false;
		if ((val >= MIN) && (val < MAX)) {
			stopBits_ = val;
			stat = true;
		}
		return stat;
	}

	/**	������
	 * 	@param ���
	 * 	@return  ����� �������� �����
	 */
	STOP_BITS get() const {
		return stopBits_;
	}

private:
	// ����� �������� �����
	STOP_BITS stopBits_;
};


/// ��������� ���������� ������ � ���������������� ������
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

	/// ��������� �����
	TInterface Interface;
	/// �������� �����
	TProtocol Protocol;
	/// �������� ��������
	TBaudRate BaudRate;
	/// ���������� ����� ������
	TDataBits DataBits;
	/// �������� �������� ��������
	TParity Parity;
	/// ���������� ���� �����
	TStopBits StopBits;
};

#endif /* PARAMUART_H_ */
