/*
 * protocolPcI.h
 *
 *  Created on: 14.05.2015
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLPCI_H_
#define PROTOCOLPCI_H_

#include "CIec101.h"
#include "glbDefine.h"
#include "paramBsp.h"

#ifdef AVR
	#include <avr/pgmspace.h>
#else
	#define PROGMEM
	#define pgm_read_byte(a) (*a)
#endif

/**	\defgroup Notation Стиль оформления.
 *
 *	Префикс:
 *	- b - булева переменная, bError;
 *	- cl - класс, clIec101;
 *	- e - перечисление, eTypeId;
 *	- i8 - целочисленная переменная размером 8 бит, i8Error;
 *	- i16 - целочисленная переменная размером 16 бит, i16Error;
 *	- p - указатель, pArray;
 *	- r - ссылка, rArray;
 *	- s - строка в RAM, sDate;
 *	- sf - строка в FLASH, sfDate;
 *	- st - структура, SData;
 *	- u - объединение union, uAsdu;
 *	- u8 - беззнаковая переменная размером 8 бит, u8Size;
 *	- u16 - беззнаковая переменная размером 8 бит, u16Size;
 *	- v - отсутствие типа void, vReserved.
 *
 *	Префикс:
 *	- c_ - статическая переменная функции, c_u8Address.
 *	- g_ - глобальная переменная, g_u8Address;
 *	- m_ - переменная-член, m_u8Address;
 *	- s_ - статическая переменная-член, s_u8AddressMax;
 *
 *	Префикс:
 *	- C - класс, CIec101;
 *	- E - перечисление, ECot;
 *	- S - структура, SDate;
 *	- T - новый тип данных (typedef);
 *	- U - объединение, UAsdu.
 *
 *	Имена локальным переменным даются без данных префиксов.
 */

class TProtocolPcI : public CIec101{

	// Адрес первого значения в журнале событий (данные класс 1).
	static const uint16_t c_adrIe1Event1 = 700;

	// Адрес первой команды передатчика (ДВ) (данные класс 1).
	static const uint16_t c_adrIe1PrdDCom1 = 740;

	// Адрес первой команды передатчика (ЦПП) (данные класс 1).
	static const uint16_t c_adrIe1PrdCCom1 = 772;

	// Адрес первой команды приемника (данные класс 1).
	static const uint16_t c_adrIe1PrmCom1 = 804;

	// Адрес первого сигнала для защиты
	static const uint16_t c_adrIe1DefSignal1 = 836;

	/// Список элементов информации класса 2.
	typedef enum __attribute__ ((__packed__)) {
		IE2_ERROR = 0,
		IE2_WARNING,
		IE2_PRD_COM,
		IE2_PRM_COM,
		IE2_DI_PUSK_PRM,
		IE2_DI_RESET_IND,
		IE2_DI_TM_CONTROL,
		IE2_GLB_ERROR_H0001,
		IE2_GLB_ERROR_H0002,
		IE2_GLB_ERROR_H0004,
		IE2_GLB_ERROR_H0008,
		IE2_GLB_ERROR_H0010,
		IE2_GLB_ERROR_H0020,
		IE2_GLB_ERROR_H0040,
		IE2_GLB_ERROR_H0080,
		IE2_GLB_ERROR_H0100,
		IE2_GLB_ERROR_H0200,
		IE2_GLB_ERROR_H0400,
		IE2_GLB_ERROR_H0800,
		IE2_GLB_ERROR_H1000,
		IE2_GLB_ERROR_H2000,
		IE2_GLB_ERROR_H4000,
		IE2_GLB_ERROR_H8000,
		IE2_GLB_WARNING_H0001,
		IE2_GLB_WARNING_H0002,
		IE2_GLB_WARNING_H0004,
		IE2_GLB_WARNING_H0008,
		IE2_GLB_WARNING_H0010,
		IE2_GLB_WARNING_H0020,
		IE2_GLB_WARNING_H0040,
		IE2_GLB_WARNING_H0080,
		IE2_GLB_WARNING_H0100,
		IE2_GLB_WARNING_H0200,
		IE2_GLB_WARNING_H0400,
		IE2_GLB_WARNING_H0800,
		IE2_GLB_WARNING_H1000,
		IE2_GLB_WARNING_H2000,
		IE2_GLB_WARNING_H4000,
		IE2_GLB_WARNING_H8000,
		IE2_PRD_ON,
		IE2_PRD_ERROR_H0001,
		IE2_PRD_ERROR_H0002,
		IE2_PRD_ERROR_H0004,
		IE2_PRD_ERROR_H0008,
		IE2_PRD_ERROR_H0010,
		IE2_PRD_ERROR_H0020,
		IE2_PRD_ERROR_H0040,
		IE2_PRD_ERROR_H0080,
		IE2_PRD_ERROR_H0100,
		IE2_PRD_ERROR_H0200,
		IE2_PRD_ERROR_H0400,
		IE2_PRD_ERROR_H0800,
		IE2_PRD_ERROR_H1000,
		IE2_PRD_ERROR_H2000,
		IE2_PRD_ERROR_H4000,
		IE2_PRD_ERROR_H8000,
		IE2_PRD_WARNING_H0001,
		IE2_PRD_WARNING_H0002,
		IE2_PRD_WARNING_H0004,
		IE2_PRD_WARNING_H0008,
		IE2_PRD_WARNING_H0010,
		IE2_PRD_WARNING_H0020,
		IE2_PRD_WARNING_H0040,
		IE2_PRD_WARNING_H0080,
		IE2_PRD_WARNING_H0100,
		IE2_PRD_WARNING_H0200,
		IE2_PRD_WARNING_H0400,
		IE2_PRD_WARNING_H0800,
		IE2_PRD_WARNING_H1000,
		IE2_PRD_WARNING_H2000,
		IE2_PRD_WARNING_H4000,
		IE2_PRD_WARNING_H8000,
		IE2_PRD_COM_01,
		IE2_PRD_COM_02,
		IE2_PRD_COM_03,
		IE2_PRD_COM_04,
		IE2_PRD_COM_05,
		IE2_PRD_COM_06,
		IE2_PRD_COM_07,
		IE2_PRD_COM_08,
		IE2_PRD_COM_09,
		IE2_PRD_COM_10,
		IE2_PRD_COM_11,
		IE2_PRD_COM_12,
		IE2_PRD_COM_13,
		IE2_PRD_COM_14,
		IE2_PRD_COM_15,
		IE2_PRD_COM_16,
		IE2_PRD_COM_17,
		IE2_PRD_COM_18,
		IE2_PRD_COM_19,
		IE2_PRD_COM_20,
		IE2_PRD_COM_21,
		IE2_PRD_COM_22,
		IE2_PRD_COM_23,
		IE2_PRD_COM_24,
		IE2_PRD_COM_25,
		IE2_PRD_COM_26,
		IE2_PRD_COM_27,
		IE2_PRD_COM_28,
		IE2_PRD_COM_29,
		IE2_PRD_COM_30,
		IE2_PRD_COM_31,
		IE2_PRD_COM_32,
		IE2_PRD_DISABLED,
		IE2_PRD_ENABLED,
		IE2_PRD_TEST,
		IE2_PRM_ON,
		IE2_PRM_ERROR_H0001,
		IE2_PRM_ERROR_H0002,
		IE2_PRM_ERROR_H0004,
		IE2_PRM_ERROR_H0008,
		IE2_PRM_ERROR_H0010,
		IE2_PRM_ERROR_H0020,
		IE2_PRM_ERROR_H0040,
		IE2_PRM_ERROR_H0080,
		IE2_PRM_ERROR_H0100,
		IE2_PRM_ERROR_H0200,
		IE2_PRM_ERROR_H0400,
		IE2_PRM_ERROR_H0800,
		IE2_PRM_ERROR_H1000,
		IE2_PRM_ERROR_H2000,
		IE2_PRM_ERROR_H4000,
		IE2_PRM_ERROR_H8000,
		IE2_PRM_WARNING_H0001,
		IE2_PRM_WARNING_H0002,
		IE2_PRM_WARNING_H0004,
		IE2_PRM_WARNING_H0008,
		IE2_PRM_WARNING_H0010,
		IE2_PRM_WARNING_H0020,
		IE2_PRM_WARNING_H0040,
		IE2_PRM_WARNING_H0080,
		IE2_PRM_WARNING_H0100,
		IE2_PRM_WARNING_H0200,
		IE2_PRM_WARNING_H0400,
		IE2_PRM_WARNING_H0800,
		IE2_PRM_WARNING_H1000,
		IE2_PRM_WARNING_H2000,
		IE2_PRM_WARNING_H4000,
		IE2_PRM_WARNING_H8000,
		IE2_PRM_COM_01,
		IE2_PRM_COM_02,
		IE2_PRM_COM_03,
		IE2_PRM_COM_04,
		IE2_PRM_COM_05,
		IE2_PRM_COM_06,
		IE2_PRM_COM_07,
		IE2_PRM_COM_08,
		IE2_PRM_COM_09,
		IE2_PRM_COM_10,
		IE2_PRM_COM_11,
		IE2_PRM_COM_12,
		IE2_PRM_COM_13,
		IE2_PRM_COM_14,
		IE2_PRM_COM_15,
		IE2_PRM_COM_16,
		IE2_PRM_COM_17,
		IE2_PRM_COM_18,
		IE2_PRM_COM_19,
		IE2_PRM_COM_20,
		IE2_PRM_COM_21,
		IE2_PRM_COM_22,
		IE2_PRM_COM_23,
		IE2_PRM_COM_24,
		IE2_PRM_COM_25,
		IE2_PRM_COM_26,
		IE2_PRM_COM_27,
		IE2_PRM_COM_28,
		IE2_PRM_COM_29,
		IE2_PRM_COM_30,
		IE2_PRM_COM_31,
		IE2_PRM_COM_32,
		IE2_PRM_DISABLED,
		IE2_PRM_READY,
		IE2_PRM_ENABLED,
		IE2_PRM_TEST,
		IE2_DEF_ON,
		IE2_DEF_ERROR_H0001,
		IE2_DEF_ERROR_H0002,
		IE2_DEF_ERROR_H0004,
		IE2_DEF_ERROR_H0008,
		IE2_DEF_ERROR_H0010,
		IE2_DEF_ERROR_H0020,
		IE2_DEF_ERROR_H0040,
		IE2_DEF_ERROR_H0080,
		IE2_DEF_ERROR_H0100,
		IE2_DEF_ERROR_H0200,
		IE2_DEF_ERROR_H0400,
		IE2_DEF_ERROR_H0800,
		IE2_DEF_ERROR_H1000,
		IE2_DEF_ERROR_H2000,
		IE2_DEF_ERROR_H4000,
		IE2_DEF_ERROR_H8000,
		IE2_DEF_WARNING_H0001,
		IE2_DEF_WARNING_H0002,
		IE2_DEF_WARNING_H0004,
		IE2_DEF_WARNING_H0008,
		IE2_DEF_WARNING_H0010,
		IE2_DEF_WARNING_H0020,
		IE2_DEF_WARNING_H0040,
		IE2_DEF_WARNING_H0080,
		IE2_DEF_WARNING_H0100,
		IE2_DEF_WARNING_H0200,
		IE2_DEF_WARNING_H0400,
		IE2_DEF_WARNING_H0800,
		IE2_DEF_WARNING_H1000,
		IE2_DEF_WARNING_H2000,
		IE2_DEF_WARNING_H4000,
		IE2_DEF_WARNING_H8000,
		IE2_MAX
	} EInfoElement2;

public:
	TProtocolPcI(stGBparam *sParam, uint8_t *buf, uint8_t size);

	/**	Функция отправки сообщения.
	 *
	 * 	@return Кол-во байт данных для передачи.
	 */
	uint8_t send();

	/**	Проверка наличия данных класса 1 на передачу.
	 *
	 *	@param adr[out] Адрес.
	 *	@param val[out] Значение.
	 *	@param time[out] Дата и время.
	 * 	@retval False Нет данных на передачу.
	 * 	@retval True Есть данные на передачу.
	 */
	virtual bool checkEventClass1(uint16_t &adr, bool &val, SCp56Time2a &time);

	/**	Проверка наличия данных класса 2 на передачу.
	 *
	 *	@param adr[out] Адрес.
	 *	@param val[out] Значение.
	 *	@param time[out] Дата и время.
	 * 	@retval False Нет данных на передачу.
	 * 	@retval True Есть данные на передачу.
	 */
	virtual bool checkEventClass2(uint16_t &adr, bool &val, SCp56Time2a &time);

private:

	/// Структура параметров.
	stGBparam * const sParam_;

	/// Состояние элементов информации (EInfoElement2).
	bool m_flags[IE2_MAX];

	/// Адреса элементов информации (EInfoElement2).
	static const uint16_t c_adrIE2[];

	/**	Обработка ответа на команду опроса.
	 *
	 *	При корректном адресе подготавливается кадр C_IC_NA_1 и возвращается \a
	 *	True, иначе False.
	 *
	 *	@param adr[out] Адрес флага.
	 *	@param val[out] Значение флага.
	 *	@retval True Подготовлены данные для передачи.
	 *	@retval False Данных нет.
	 */
	virtual bool procInterrog(uint16_t &adr, bool &val);

	/**	Установка времени.
	 *
	 *	Вызывается автоматически, при получении кадра синхронизации времени.
	 *
	 *	Подгатавливается команда для передачи времени в БСП.
	 *
	 * 	@retval True Время установлено.
	 * 	@retval False Время не установлено.
	 */
	virtual bool procSetTime();

	/**	Установка времени, сообщение об окочнании.
	 *
	 * 	Вызывается автоматически при принятии пакетов фиксированой длины, просле
	 * 	кадра синхронизации времени.
	 *
	 * 	Если до этого было получен кадр синхронизации времени, будет сформирован
	 * 	кадр сообщения об окончании.
	 *
	 * 	@retval True Если был получен ответ от БСП с временем во время синхронизации.
	 * 	@retval False Если ответа небыло.
	 */
	virtual bool procSetTimeEnd();

	/**	Возвращает состояние элемента информации.
	 *
	 * 	@param[in] ei Элемент информации.
	 * 	@retval True Флаг установлен.
	 * 	@retval False Флаг сброшен.
	 *
	 */
	bool getValue(EInfoElement2 ei) const;

	/**	Возвращает состояние флага информации аппарата.
	 *
	 * 	@param[in] ei Элемент информации.
	 *	@retval True Флаг установлен.
	 *	@retval False Флаг сброшен.
	 */
	bool getDevice(EInfoElement2 ei) const;

	/**	Возвращает состояние флага общей информации.
	 *
	 * 	@param[in] ei Адрес флага.
	 *	@retval True Флаг установлен.
	 *	@retval False Флаг сброшен.
	 */
	bool getGlb(EInfoElement2 ei) const;

	/**	Возвращает состояние флага передатчика.
	 *
	 * 	@param[in] ei Адрес флага.
	 *	@retval True Флаг установлен.
	 *	@retval False Флаг сброшен.
	 */
	bool getPrd(EInfoElement2 ei) const;

	/**	Возвращает состояние флага приемника.
	 *
	 * 	@param[in] ei Адрес флага.
	 *	@retval True Флаг установлен.
	 *	@retval False Флаг сброшен.
	 */
	bool getPrm(EInfoElement2 ei) const;

	/**	Возвращает состояние флага защиты.
	 *
	 * 	@param[in] ei Адрес флага.
	 *	@retval True Флаг установлен.
	 *	@retval False Флаг сброшен.
	 */
	bool getDef(EInfoElement2 ei) const;
};

#endif /* PROTOCOLPCI_H_ */
