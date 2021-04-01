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

};


#endif /* PROTOCOLPC_H_ */
