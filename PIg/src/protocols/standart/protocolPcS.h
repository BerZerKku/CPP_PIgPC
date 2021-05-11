/*
 * protocolBspS.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLPCS_H_
#define PROTOCOLPCS_H_

#include "protocolS.h"

class clProtocolPcS : public clProtocolS
{
    eGB_COM lastCom;

public:
	clProtocolPcS(uint8_t *buf, uint8_t size, stGBparam *sParam);

	/** Обработка принятого сообщения.
	 *
	 * 	!!! override не понимает WinAVR.
	 *
	 * 	@return true если сообщение обработано, иначе false.
	 */
    bool getData();

    /**	Изменение команды при передаче ее на ПК.
     *
     *  - GB_COM_GET_VERS
     *      В посылку добавляется версия прошивки БСП-ПИ.
     *      Если кол-во байт данных меньше, чем необходимо, т.е. нет резерва
     *      под прошивку, происходит дополнение кол-ва байт данных до
     *      необходимого. Все новые байты данных, за исключением самой
     *      прошивки, будут 0х00.
     *  - GB_COM_SET_NET_ADR
     *      В случае если "оригинальная" команда пришедшая с ПК была
     *      GB_COM_SET_PASSWORD и в этой команде находится пароль,
     *      то посылка будет преобразована в ответ на команду
     *      B_COM_SET_PASSWORD.
	 *
     *	@return true если команда была изменена, иначе false.
	 */
    bool modifyCommand();

    /**
     * @brief Обработка команд.
     * @param[in] com Команда.
     * @return true если команда обработана, false если надо передать дальше.
     */
    bool procCommand(eGB_COM com);

    /**
     * @brief Обработка команды чтения пароля.
     * @return true если команда обработана, false если надо передать дальше.
     */
    bool hdlrGetPassword();

    /**
     * @brief Обработка команды записи пароля.
     * Если в команде два байта, то команда будет изменена на команду записи
     * пароля в БСП.
     * Если в команде не два байта, то команда будет отправлена обратно.
     * @returntrue если команда обработана, false если надо передать дальше..
     */
    bool hdlrSetPassword();

    /**
     * @brief Обработка команды чтения настроек локальной сети.
     * @return true если команда обработана, false если надо передать дальше.
     */
    bool hdlrGetNetAdr();
};


#endif /* PROTOCOLPC_H_ */
