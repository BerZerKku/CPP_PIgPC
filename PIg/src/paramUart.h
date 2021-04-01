/*
 * paramUart.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMUART_H_
#define PARAMUART_H_

#include <stdint.h>

/// ��������� �����
class TInterface {
public:

	// ��������� �����
	enum INTERFACE {
		MIN = 0,	//
        USB = MIN,	// ����������� ����� USB �� �������� ������
        BVP,		// ����������� � ����� ���
		MAX			//
	};

	TInterface () {
        interface_ =  MAX;
		changed = false;
	}

	/**	������.
	 * 	@param val ��������� �����.
	 * 	@return False � ������ ���������� ��������.
	 */
	bool set(TInterface::INTERFACE val) {
		bool stat = false;
		if ((val >= MIN) && (val < MAX)) {
            if (val != interface_) {
                changed = true;
                interface_ = val;
            }
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

    /** ��������� ��������� ���������.
     *
     *  ����� �������� ���� ��������� ������������.
     *
     *  @return true ���� �������� ��� �������, ����� false.
     */
    bool isChanged() {
        bool tchanged = changed;
        changed = false;
        return tchanged;
    }

private:
    // ���� ��������� ���������
    bool changed;
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

	//
	TProtocol() {
		protocol_ = MAX;
		changed = false;
	}

	/**	������.
	 * 	@param val �������� �����.
	 * 	@return False � ������ ���������� ��������.
	 */
	bool set(PROTOCOL val) {
		bool stat = false;
		if ((val >= MIN) && (val < MAX)) {
            if (protocol_ != val) {
                protocol_ = val;
                changed = true;
            }
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

    /** ��������� ��������� ���������.
     *
     *  ����� �������� ���� ��������� ������������.
     *
     *  @return true ���� �������� ��� �������, ����� false.
     */
    bool isChanged() {
        bool tchanged = changed;
        changed = false;
        return tchanged;
    }

private:
    // ���� ��������� ���������
    bool changed;
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

	TBaudRate() {
		baudRate_ = MAX;
		changed = false;
	}

	/**	������.
	 * 	@param val �������� ��������.
	 * 	@return
	 * 	@retval False � ������ ���������� ��������.
	 */
	bool set(TBaudRate::BAUD_RATE val) {
		bool stat = false;
		if ((val >= MIN) && (val < MAX)) {
            if (baudRate_ != val) {
                baudRate_ = val;
                changed = true;
            }
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

    /** ��������� ��������� ���������.
     *
     *  ����� �������� ���� ��������� ������������.
     *
     *  @return true ���� �������� ��� �������, ����� false.
     */
    bool isChanged() {
        bool tchanged = changed;
        changed = false;
        return tchanged;
    }

private:
    // ���� ��������� ���������
    bool changed;
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

	TDataBits() {
		dataBits_ = MAX;
		changed = false;
	}

	/**	������
	 * 	@param val ���-�� ����� ������.
	 * 	@return False � ������ ���������� ��������.
	 */
	bool set(DATA_BITS val) {
		bool stat = false;
		if ((val >= MIN) && (val < MAX)) {
            if (dataBits_ != val) {
                dataBits_ = val;
                changed = true;
            }
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

    /** ��������� ��������� ���������.
     *
     *  ����� �������� ���� ��������� ������������.
     *
     *  @return true ���� �������� ��� �������, ����� false.
     */
    bool isChanged() {
        bool tchanged = changed;
        changed = false;
        return tchanged;
    }

private:
    // ���� ��������� ���������
    bool changed;
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

	TParity() {
		parity_ = MAX;
		changed = false;
	}

	/**	������
	 * 	@param val �������� ��������
	 * 	@return False � ������ ���������� ��������
	 */
	bool set(TParity::PARITY val) {
		bool stat = false;
		if ((val >= MIN) && (val < MAX)) {
            if (parity_ != val) {
                parity_ = val;
                changed = true;
            }
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

    /** ��������� ��������� ���������.
     *
     *  ����� �������� ���� ��������� ������������.
     *
     *  @return true ���� �������� ��� �������, ����� false.
     */
    bool isChanged() {
        bool tchanged = changed;
        changed = false;
        return tchanged;
    }

private:
    // ���� ��������� ���������
    bool changed;
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

	TStopBits() {
		stopBits_ = MAX;
		changed = false;
	}

	/**	������
	 * 	@param val ����� �������� �����
	 * 	@return False � ������ ���������� ��������
	 */
	bool set(STOP_BITS val) {
		bool stat = false;
		if ((val >= MIN) && (val < MAX)) {
            if (stopBits_ != val) {
                stopBits_ = val;
                changed = true;
            }
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

    /** ��������� ��������� ���������.
     *
     *  ����� �������� ���� ��������� ������������.
     *
     *  @return true ���� �������� ��� �������, ����� false.
     */
    bool isChanged() {
        bool tchanged = changed;
        changed = false;
        return tchanged;
    }

private:
    // ���� ��������� ���������
    bool changed;
	// ����� �������� �����
    STOP_BITS stopBits_;
};

/// ������� �����.
class TNetAddress {
public:
    /// �������� ������� �������.
    enum NET_ADDRESS {
        MIN = 1,
        MAX = 247
    };

    TNetAddress() {
    	netAddress_ = MIN;
    	changed = false;
    }

    /**	������������� ������� �����.
     *
     * 	@param[in] val ������� �����.
     * 	@return true ���� ������� ����� ����������, ����� false.
     */
    bool set(uint8_t val) {
        bool stat = false;
        if ((val >= MIN) && (val <= MAX)) {
            if (netAddress_ != val) {
                netAddress_ = val;
                changed = true;
            }
            stat = true;
        }
        return stat;
    }

    /**	���������� ������� �����.
     *
     * 	@return ������� �����.
     */
    uint8_t get() const {
        return netAddress_;
    }

    /** ��������� ��������� ���������.
     *
     *  ����� �������� ���� ��������� ������������.
     *
     *  @return true ���� �������� ��� �������, ����� false.
     */
    bool isChanged() {
        bool tchanged = changed;
        changed = false;
        return tchanged;
    }

private:
    // ���� ��������� ���������
    bool changed;
    // ������� �����.
    uint8_t netAddress_;
};


/// ��������� ���������� ������ � ���������������� ������
class TUartData {
public:
    TUartData() {
        Interface.set(TInterface::BVP);
        Protocol.set(TProtocol::STANDART);
        BaudRate.set(TBaudRate::_9600);
        DataBits.set(TDataBits::_8);
        Parity.set(TParity::EVEN);
        StopBits.set(TStopBits::ONE);
        NetAddress.set(1);

    };

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
    /// ������� �����.
    TNetAddress NetAddress;
};

#endif /* PARAMUART_H_ */
