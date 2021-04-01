/*
 * enterParam.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef TX_COM_HPP_
#define TX_COM_HPP_

#include "src/glbDefine.h"

// класс для передачи команд
class TTxCom {
    static const uint8_t BUFFER_SIZE = 16;

   public:
    TTxCom() {
        clear();
    }

    // очистка буфера
    void clear();

    /** Запись команды в буфер 1.
     *
     * 	Если num > 0, то заменяется последняя команда в буфере;
     * 	Если num = 0 команда добавляется.
     * 	@param com Код команды.
     * 	@param num Индекс элемента в буфере.
     * 	@retval True - в случае успешной записи.
     * 	@retval False - если не удалось поместить команду в буфер.
     */
    bool addCom1(eGB_COM com, uint8_t num = 0);

    /** Последовательная выдача имеющихся в буфере 1 команд (по кругу).
     * 	@return Код текущей команды.
     */
    eGB_COM getCom1();

    /** Запись команды в буфер 2.
     * 	@param com Код команды.
     * 	@retval True - в случае успешной записи.
     * 	@retval False - если не удалось поместить команду в буфер.
     */
    bool addCom2(eGB_COM com);

    /** Последовательная выдача имеющихся в буфере 2 команд (по кругу).
     * 	@return Код текущей команды.
     */
    eGB_COM getCom2();

    /**	Запись срочной команды в конец очереди.
     *
     *	При добавлении срочной команды все содержимое буфера передачи сохраняется.
     *
     * 	@param com Код срочной команды
     */
    void addFastCom(eGB_COM com);

    /**	Считывание срочной команды. При этом идет копирование данных
     *
     *	При извлечении срочной команды в буфер передачи копируются сохраненные
     *	для данной команды данные.
     *
     * 	@return Код срочной команды.
     */
    eGB_COM getFastCom();

    /**	Запись байт данных в буфер.
     * 	@param byte	Байт данных.
     * 	@param num Индекс элемента массива.
     */
    void setInt8(uint8_t byte, uint8_t num = 0);

    /** Считывание байта данных.
     * 	@param num Индекс элемента массива.
     * 	@return Байт данных.
     */
    uint8_t getInt8(uint8_t num = 0) const;

    /**	Запись 2-х байтного числа (uint16_t) в буфер.
     * 	В буфере данные записываются в 1 и 2 элемент массива uint8_t.
     * 	@param val Данные.
     */
    void setInt16(uint16_t val);

    /** Считывание 2-х абйтного числа (uint16_t) из буфера.
     * 	Данные хранятся в 1 и 2 элементах массива uint8_t.
     * 	@return Данные.
     */
    uint16_t getInt16() const;

    /**	Возвращает указатель на буфер данных.
     * 	@return Указатель на буфер данных.
     */
    uint8_t* getBuferAddress();

    /**	Возвращает тип команды на передачу.
     *
     * 	@return Тип команды на передачу
     */
    eGB_SEND_TYPE getSendType() const;

    /**	Установка типа команды на передачу.
     *
     * 	@param sendType Тип команды на передачу.
     */
    void setSendType(eGB_SEND_TYPE sendType);

   private:
    // тип передаваемой команды
    eGB_SEND_TYPE sendType;
    // срочная команда (на изменение)
    eGB_COM comFast_[MAX_NUM_FAST_COM];
    // номер текущей срочной команды
    uint8_t cntComFast;
    // первый буфер команд
    eGB_COM com1_[MAX_NUM_COM_BUF1];
    // кол-во команд в первом буфере
    uint8_t numCom1_;
    // номер текущей команды в первом буфере
    uint8_t cnt1_;
    // второй буфер команд
    eGB_COM com2_[MAX_NUM_COM_BUF2];
    // кол-во команд во втором буфере
    uint8_t numCom2_;
    // номер текущей команды во втором буфере
    uint8_t cnt2_;
    // буфер данных (для каждой быстрой команды и основной )
    uint8_t buf_[MAX_NUM_FAST_COM + 1] [BUFFER_SIZE];
};

#endif /* TX_COM_HPP_ */
