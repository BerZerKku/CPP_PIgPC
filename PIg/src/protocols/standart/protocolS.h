/*
 * protocolS.h
 *
 *  Created on: 28.06.2012
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLS_H_
#define PROTOCOLS_H_

#include <stdint.h>
#include "glbDefine.h"
#include "paramBsp.h"

// состояния протокола
enum ePRTS_STATUS {
	PRTS_STATUS_OFF = 0,	///< выключен
	PRTS_STATUS_NO,			///< состояние ожидания сообщения
	PRTS_STATUS_READ,		///< идет считывание сообщения
	PRTS_STATUS_READ_OK,	///< соообщение считано полностью, но КС не проверена
	PRTS_STATUS_WAIT_ANSWER,///< ожидание ответа
	PRTS_STATUS_WRITE_PC,	///< надо передать сообщене на(с) ПК
	PRTS_STATUS_WRITE		///< передается сообщение
};

// номера байт данных в протоколе
enum ePRTS_DATA_BYTE_NAME {
	COM =  2,
	NUM,// 3
	B1,	// 4
	B2,	// 5
	B3,	// 6
	B4,	// 7
	B5,	// 8
	B6,	// 9
	B7,	// 10
	B8,	// 11
	B9,	// 12
	B10,// 13
	B11,// 14
	B12,// 15
	B13,// 16
	B14,// 17
	B15,// 18
	B16,// 19
	B17,// 20
	B18,// 21
	B19,// 22
	B20,// 23
	B21,// 24
	B22	// 25
};

class clProtocolS {
	/** Кол-во циклов, после которого залипшее текущее состояние будет сброшено в состояние по-умолчанию
	 */
	static const uint8_t MAX_CYCLE_TO_REST_SOST = 5;

public:
	clProtocolS(uint8_t *buf, uint8_t size, stGBparam *sParam);
//	uint8_t addCom	(uint8_t com, ePRTS_ACTION act);
//	bool getData	(stMNUparam *param);

	// буфер принятых/передаваемых данных
	uint8_t * const buf;

	/// Проверка флага наличия принятой посылки
	bool checkReadData();

	/// Отправка сообщения, возвращает длинну передаваемой посылки
	uint8_t trCom();

	/// Запуск работы данного протокола
	void setEnable(ePRTS_STATUS stat) {
		statDef_ = stat;
		setCurrentStatus(stat);
		cntCycle_ = 0;
	}

	/// Остановка работы данного протокола
	void setDisable() {
		setCurrentStatus(PRTS_STATUS_OFF);
	}

	/// Проверка текущего состояния протокола
	bool isEnable() const {
		return (stat_ != PRTS_STATUS_OFF);
	}

	/// Текущая команда в буфере
	uint8_t getCurrentCom() const {
		return buf[2];
	}

	/// Текущий статус работы протокола
	ePRTS_STATUS getCurrentStatus() const {
		return stat_;
	}

	/// Смена текущего статуса работы протокола
	/// В случае установки состояния ожидания, сбрасываетс счетчик байт
	void setCurrentStatus(ePRTS_STATUS stat) {
		if (stat == PRTS_STATUS_NO) {
			cnt_ = 0;
		}

		stat_ = stat;
	}

	/// Копирование посылки из другого буфера
	bool copyCommandFrom(uint8_t * const bufSource);

	/// Обработка принятого сообщения. В случае неудачи возвращает False.
	virtual bool getData() {
		stat_ = statDef_;
		return false;
	}

	/// Проверка текущего состояния
	void checkStat();

	/**	Проверка принятого байта на соответствие протоколу
	 *
	 * 	!! Помещается в прерывание по приему.
	 * 	Байт проверяется только в состоянии \a PRTS_STATUS_READ.
	 *
	 *	@see PRTS_STATUS_READ
	 *
	 * 	@param byte Байт данных.
	 * 	@return Текущая позиция в проверке протокола.
	 * 	@retval Ожидание первого синхробайта.
	 */
	uint8_t checkByte(uint8_t byte) {
		uint8_t cnt = cnt_;

		if ((cnt == 0) && (byte == 0x55)) {
			setCurrentStatus(PRTS_STATUS_READ);
		}

		if (stat_ == PRTS_STATUS_READ) {
			buf[cnt] = byte;
			switch (cnt) {
			case 0:
				// первый синхробайт
				if (byte == 0x55)
					cnt++;
				break;
			case 1:
				// второй синхробайт
				cnt = (byte == 0xAA) ? 2 : 0;
				break;
			case 2:
				// принят байт команды
				cnt++;
				break;
			case 3:
				// проверка на наличие в буфере достаточного места для посылки
				cnt = (byte < (size_ - 5)) ? (maxLen_ = byte + 5, cnt + 1) : 0;
				break;
			default:
				// ожидание приема заявленного количества байт данных
				cnt++;
				if (cnt >= maxLen_) {
					setCurrentStatus(PRTS_STATUS_READ_OK);
				}
				break;
			}
			cnt_ = cnt;
		}
		return cnt;
	}

protected:
	// структура параметров
	stGBparam * const sParam_;

	// текущий статус работы протокола
	ePRTS_STATUS stat_;

	// Счетчик принятого байта по протоколу
	uint8_t cnt_;

	// Заявленное кол-во байт данных в посылке
	uint8_t maxLen_;

	// размер буфера
	const uint8_t size_;

	// состояние работы протокола по-умолчанию
	ePRTS_STATUS statDef_;

	// предыдущее состояние протокола
	ePRTS_STATUS old_;

	// счетчик одинаковых состояний
	uint8_t cntCycle_;

	// Подготовка к отправке команды (сама команда, кол-во данных и данные
	// уже должны лежать в буфере)
	uint8_t addCom();
	// Подготовка к отправке команды без данных (заполнение буфера)
	uint8_t addCom(uint8_t com);
	// Подготовка к отправке команды с 1 байтом данных (заполнение буфера)
	uint8_t addCom(uint8_t com, uint8_t byte);
	// Подготовка к отправке команды с 2 байтами данных (заполнение буфера)
	uint8_t addCom(uint8_t com, uint8_t byte1, uint8_t byte2);
	// Подготовка к отправке команды с данными (заполнение буфера)
	uint8_t addCom(uint8_t com, uint8_t size, uint8_t buf[]);

	// Проверка принятой контрольной суммы
	bool checkCRC() const;

	// Вычисление контрольной суммы содержимого буфера
	uint8_t getCRC() const;
};

#endif /* PROTOCOLS_H_ */
