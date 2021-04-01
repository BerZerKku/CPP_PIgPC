/** @brief Последовательный порт.
 *
 *  Класс работы с последовательным портом.
 */

#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include "src/paramUart.h"

class TUart {
public:

	/**	Обработчик прерывания опустошения буфера передачика.
	 *
	 *	При передаче последнего байта из буфера, запрещается прерывание
	 *	по опустошению буфера.
	 *
	 * 	Должен распологаться в прерывании!!!
	 */
	void isrUDR() {
		if (cnt_ < numTrByte_) {
			*udr_ = buf_[cnt_];
			buf_[cnt_++] = 0;
		} else {
			*ucsrb_ &= ~(1 << UDRIE1);
		}
	}

	/**	Обработчик прерывания окончания передачи.
	 *
	 * 	Запрещается прерывание на передачу, разрешается прерывание на прием.
	 *
	 * 	Должен распологаться в прерывании!!!
	 */
	void isrTX() {
		cnt_ = 0;
		numTrByte_ = 0;
		*ucsrb_ |= (1 << RXCIE);
		*ucsrb_ &= ~(1 << TXCIE);
	}

	/// Имя последовательного порта
	enum ePORT {
		PORT_UART0, 				///< Последовательный порт UART0
		PORT_UART1					///< Последовательный порт UART1
	};

	/** Конструктор
	 *
	 * 	Порты ввода-вывода должны быть настроены заранее.
	 *
	 * 	@param port Имя порта
	 * 	@see ePORT
	 * 	@param *buf Указатель на буфер данных
	 * 	@param size Размер буфера
	 */
	TUart(TUart::ePORT port, uint8_t * const buf, uint8_t size);

	/**	Открывает порт.
	 *
	 *	В случае ошибочных данных настроек порта, будут применены параметры
	 *	по умолчанию:
	 *	- скорость 19200
	 *	- байт данных 8
	 *	- проверки четности нет
	 *	- два стоп байта
	 *
	 * 	@param baud	Скорость работы порта.
	 * 	@param databits	Количество битов данных.
	 * 	@param parity Протокол контроля четности.
	 * 	@param stopbits Количество стоп битов.
	 * 	@return True - в случае успешного открытия
	 */
	bool open(uint16_t baud, TDataBits::DATA_BITS databits,
			TParity::PARITY parity, TStopBits::STOP_BITS stopbits);

	/** Закрывает порт.
	 *
	 * 	Запрещается работа приемника и передатчика UART, а таке же сбрасываются
	 * 	все прерывания.
	 */
	void close();

	/**	Передача заданного кол-ва данных, заранее помещенных в буфер.
	 *
	 * 	@param size Кол-во байт данных для передачи.
	 * 	@return Кол-во отправляемых байт данных
	 */
	void trData(uint8_t size);

private:
	/// кол-во принятых/полученных данных
	uint8_t cnt_;
	/// указатель на буфер
	uint8_t * const buf_;
	/// размер буфера
	const uint8_t size_;
	/// кол-во данных на передачу
	uint8_t numTrByte_;

	// регистр данных
	volatile uint8_t *udr_;

	// регистры настройки
	volatile uint8_t *ubbrh_;
	volatile uint8_t *ubbrl_;
	volatile uint8_t *ucsrc_;
	volatile uint8_t *ucsrb_;
	volatile uint8_t *ucsra_;
};

#endif /* UART_H_ */
