/*
 * protocolBspS.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLBSPS_H_
#define PROTOCOLBSPS_H_

#include "protocolS.h"


class clProtocolBspS : public clProtocolS
{
public:
	clProtocolBspS(uint8_t *buf, uint8_t size, stGBparam *sParam);
	/// Обработка принятого сообщения. В случае неудачи возвращает False.
	bool getData(bool pc);

	/// Отправка команды
	uint8_t sendData(eGB_COM com);

private:
	bool getDefCommand(eGB_COM com, bool pc);
	bool getPrmCommand(eGB_COM com, bool pc);
	bool getPrdCommand(eGB_COM com, bool pc);
	bool getGlbCommand(eGB_COM com, bool pc);

	uint8_t sendModifGlbCommand(eGB_COM com);

	uint8_t sendReadJrnCommand(eGB_COM com);

    /** Обрабатывает ответ на принятую команду чтения "Сетевой адрес"
     *
     *  @return true всегда
     */
    bool hdlrComGetNetAdr();

    /** Обрабатывает ответ на принятую команду записи "Сетевой адрес"
     *
     *  @return true всегда
     */
    bool hdlrComSetNetAdr();

    /** Считывание параметров для команд GB_COM_GET_NET_ADR и SET.
     *
     *  @param pos Номер параметра.
     *  @param buf Начальный адрес буфера.
     *  @param len Количество оставшихся байт в буфере.
     *  @return Количество считанных байт данных.
     *  @retval len если в буфере не достаточно байт для параметра.
     */
    uint8_t getComNetAdr(posComNetAdr_t pos, const uint8_t *buf, uint8_t len);

#ifdef TEST_FRIENDS
    TEST_FRIENDS;
#endif
};


#endif /* PROTOCOLBSPS_H_ */
