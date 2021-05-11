/*
 * enterParam.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef JOURNAL_ENTRY_HPP_
#define JOURNAL_ENTRY_HPP_

#include "src/glbDefine.h"
#include "src/menu/dateTime.hpp"

class TJournalEntry {
   public:
    TJournalEntry() {
        clear();
    }

    void clear() {
        currentDevice_ = GB_DEVICE_MAX;
        deviceJrn_ = GB_DEVICE_K400_MAX;
        eventType_ = MAX_JRN_EVENT_VALUE - MIN_JRN_EVENT_VALUE + 1;
        regime_ = GB_REGIME_MAX;
        numCom_ = 0;
        sourceCom_ = GB_SOURCE_COM_MAX;
        srcCom_ = 0;
        signalPusk_ = false;
        signalStop_ = false;
        signalMan_ = false;
        signalPrm_ = false;
        signalPrd_ = false;
        signalOut_ = false;
        signals_ = 0;

        numJrnEntries_ = 0;
        maxNumJrnEntry_ = 0;
        overflow_ = false;
        addressFirstEntry_ = 0;

        currentEntry_ = 1;
        ready_ = false;

        val = false;
    }

    // время для записи журнала
    TDateTime dateTime;

    // время для передачи по протоколам
    TDateTime dateTimeTr;

    // значение для передачи по протоколу
    bool val;

    bool setCurrentDevice(eGB_DEVICE device) {
        bool stat = false;
        if ((device >= GB_DEVICE_MIN) && (device < GB_DEVICE_MAX)) {
            currentDevice_ = device;
            stat = true;
        } else
            currentDevice_ = GB_DEVICE_MAX;
        return stat;
    }
    eGB_DEVICE getCurrentDevice() const {
        return currentDevice_;
    }

    // запись\считывание устройства для которого сделана запись
    bool setDeviceJrn(eGB_DEVICE_K400 device) {
        bool stat = false;
        if ((device >= GB_DEVICE_K400_MIN) && (device < GB_DEVICE_K400_MAX)) {
            deviceJrn_ = device;
            stat = true;
        } else
            deviceJrn_ = GB_DEVICE_K400_MAX;
        return stat;
    }
    eGB_DEVICE_K400 getDeviceJrn() const {
        return deviceJrn_;
    }

    // тип события
    bool setEventType(uint8_t val) {
        bool state = false;

        uint8_t min = 255;
        uint8_t max = 0;
        // установка мин/макс значения события, в зависимости от
        // текущего журнала
        if (currentDevice_ == GB_DEVICE_GLB) {
            min = MIN_JRN_EVENT_VALUE;
            max = MAX_JRN_EVENT_VALUE;
        } else if (currentDevice_ == GB_DEVICE_PRD) {
            min = GB_STATE_COM_MIN;
            max = GB_STATE_COM_MAX;
        } else if (currentDevice_ == GB_DEVICE_PRM) {
            min = GB_STATE_COM_MIN;
            max = GB_STATE_COM_MAX;
        } else if (currentDevice_ == GB_DEVICE_DEF) {
            min = 0;
            max = MAX_NUM_DEVICE_STATE;
        }

        if ((val >= min) && (val <= max)) {
            eventType_ = val;
            state = true;
        } else
            eventType_ = max;

        return state;
    }
    uint8_t getEventType() const {
        return eventType_;
    }

    // источник команды
    bool setSrcCom(uint8_t val) {
        srcCom_ = val & 0x07;
        return true;
    }
    uint8_t getSrcCom() const {
        return srcCom_;
    }


    /**	Установка записей для журналов ОПТИКИ.
     * 	Считываются 4 байта, старшим вперед.
     * 	Каждый установленный бит в них отвечает за свое событие.
     * 	@param buf Указатель на массив из 4 байт данных.
     * 	@retval True - всегда.
     */
    bool setOpticEntry(uint8_t *buf) {
        uint8_t num = 0;

        // В каждом байте записи считается кол-во установленных битов
        for(uint_fast8_t i = 0; i <= 3; i++) {
            uint8_t byte = *buf++;
            opticEntry_[i] = byte;
            for(uint_fast8_t j = 1; j > 0; j <<= 1) {
                if (byte & j) {
                    num++;
                }
            }
        }
        numOpticEntries_ = num;

        return true;
    }

    /** Возвращает кол-во активных событий в журнале ОПТИКИ.
     * 	Для журнала событий это кол-во событий, для команд - команд.
     * 	@return Кол-во активных собыйтив журнале ОПТИКИ
     */
    uint8_t getNumOpticsEntries() const {
        return numOpticEntries_;
    }

    /** Ищет и возвращает код события, исходя из номера события.
     * 	Т.е. если у нас в записи 5 активных событий, для каждого из них
     * 	можно получить код записи (например номер команды).
     *
     * 	@param num Номер события, начиная с 1.
     * 	@return Код события (0 - в случае ошибки).
     */
    uint8_t getOpticEntry(uint8_t num) {
        uint8_t val = 0;

        // проверка на допустимое значение
        if ((num >= 1) && (num <= numOpticEntries_)) {
            for(uint_fast8_t i = 0; i <= 3; i++) {
                // перебор 4-х байт, начиная с младшего
                uint8_t byte = opticEntry_[3 - i];
                for(uint_fast8_t j = 0; j < 8; j++) {
                    if (byte & (1 << j)) {
                        if (--num == 0) {
                            // номер установленного бита, от 1 до 32
                            val = 1 + ((i * 8) + j);
                        }
                    }
                }
            }
        }
        return val;
    }

    /**	Установка записей для журналов ОПТИКИ ЦПП.
     *
     * 	Считываются 4 байта, СТАРШИМ вперед.
     * 	Каждый установленный бит в них отвечает за свое событие.
     *
     * 	@param buf Указатель на массив из 4 байт данных.
     * 	@retval True - всегда.
     */
    bool setOpticEntryDR(uint8_t *buf) {
        uint8_t num = 0;

        // В каждом байте записи считается кол-во установленных битов
        for(uint_fast8_t i = 0; i <= 3; i++) {
            uint8_t byte = *buf++;
            opticEntryDR_[i] = byte;
            for(uint_fast8_t j = 1; j > 0; j <<= 1) {
                if (byte & j) {
                    num++;
                }
            }
        }
        numOpticEntriesDR_ = num;

        return true;
    }

    /** Возвращает флаг команды ЦПП.
     *
     * 	@param com Номер команды для которой проверяется флаг 1..32.
     * 	@return 1 - если команда передана по ЦПП, 0 - с дискретного входа.
     */
    uint8_t getOpticEntryDR(uint8_t com) {
        eGB_SOURCE_COM val = GB_SOURCE_COM_DI;

        if ((com > 0) && (com < MAX_NUM_COM_PRD)) {
            com--;

            uint8_t i = (MAX_NUM_COM_PRD - 1) / 8;
            while(com > 8) {
                com -= 8;
                i--;
            }

            if (opticEntryDR_[i] & (1 << com)) {
                val = GB_SOURCE_COM_DR;
            }
        }

        return val;
    }

    // режим
    bool setRegime(eGB_REGIME regime) {
        bool state = false;
        if ((regime >= GB_REGIME_MIN) && (regime <= GB_REGIME_MAX)) {
            regime_ = regime;
            state = true;
        } else
            regime_ = GB_REGIME_MAX;
        return state;
    }
    eGB_REGIME getRegime() const {
        return regime_;
    }

    // сигналы для журнала защиты
    bool setSignalDef(uint8_t val) {
        bool state = true;

        signalPusk_ = (val & 0x10) ? 1 : 0;
        signalStop_ = (val & 0x20) ? 1 : 0;
        signalMan_ = (val & 0x40) ? 1 : 0;
        signalPrm_ = (val & 0x01) ? 1 : 0;
        signalPrd_ = (val & 0x02) ? 1 : 0;
        signalOut_ = (val & 0x04) ? 1 : 0;
        signals_ = val;

        return state;
    }
    uint8_t getSignalPusk() const {
        return signalPusk_;
    }
    uint8_t getSignalStop() const {
        return signalStop_;
    }
    uint8_t getSignalMan() const {
        return signalMan_;
    }
    uint8_t getSignalPrm() const {
        return signalPrm_;
    }
    uint8_t getSignalPrd() const {
        return signalPrd_;
    }
    uint8_t getSignalOut() const {
        return signalOut_;
    }
    uint8_t getSignals() const {
        return signals_;
    }

    // номер команды
    bool setNumCom(uint8_t num) {
        bool stat = false;
        if ((num > 0) && (num <= MAX_NUM_COM_PRD)) {
            numCom_ = num;
            stat = true;
        }
        return stat;
    }
    uint8_t getNumCom() const {
        return numCom_;
    }

    /** Установка источника передаваемой команды.
     *	В случае не корректного значения, в параметр будет записано значение
     *	\a GB_SOURCE_COM_MAX.
     *
     *	@see eGB_SOURCE_COM
     *	@param source Источник передаваемой команды.
     *	@return True - в случае корректного значения, False - иначе.
     */
    bool setSourceCom(eGB_SOURCE_COM source) {
        bool stat = false;

        switch(source) {
            case GB_SOURCE_COM_DI:
            case GB_SOURCE_COM_DR:
            case GB_SOURCE_COM_MAX: {
                stat = true;
            }
            break;
        }

        if (!stat) {
            source = GB_SOURCE_COM_MAX;
        }

        sourceCom_ = source;
        return stat;
    }

    /** Считывание источника передаваемой команды.
     *
     *	@return Источник передаваемой команды.
     */
    eGB_SOURCE_COM getSourceCom() const {
        return sourceCom_;
    }

    // количество записей в журнале
    bool setNumJrnEntry(uint16_t val) {
        bool stat = false;

        overflow_ = (val & 0xC000) != 0;

        val &= 0x3FFF;

        if (val <= maxNumJrnEntry_) {
            if (overflow_) {
                numJrnEntries_ = maxNumJrnEntry_;
                addressFirstEntry_ = val;
            } else {
                numJrnEntries_ = val;
                addressFirstEntry_ = 0;
            }
            stat = true;
        }
        return stat;
    }
    uint16_t getNumJrnEntries() const {
        return numJrnEntries_;
    }

    // максимальное кол-во записей в журнале
    bool setMaxNumJrnEntries(uint16_t max) {
        bool stat = false;
        if (max <= GLB_JRN_MAX) {
            stat = true;
            maxNumJrnEntry_ = max;
        }
        return stat;
    }
    uint16_t getMaxNumJrnEntry() const {
        return maxNumJrnEntry_;
    }

    // переполнение журнала
    bool isOverflow() const {
        return overflow_;
    }

    /** Возвращает абсолютный адрес текущей записи в журнале.
     *
     *  Журнал перезаписывается по кругу, поэтому начало и конец журнала
     *  постоянно перезаписываются.
     *  В данной функции вычисляется абсолютный адрес в журнале, чтобы при
     *  добавлении новой записи в журнал, текущая запись на экране не менялась.
     *
     *  @return Абсолютный адрес записи в журнале.
     */
    uint16_t getEntryAdress() const {
        uint16_t address = 0;

        if (numJrnEntries_ > 0) {
            address= (currentEntry_ + addressFirstEntry_ - 1) % numJrnEntries_;
        }

        return address;
    }

    // текущая запись
    uint16_t getCurrentEntry() const {
        return currentEntry_;
    }
    // следующая/предыдущая запись возвращает true если новое значение
    // отличается от предыдущего
    bool setNextEntry() {
        bool stat = false;
        uint16_t tmp = currentEntry_;
        tmp = (tmp < numJrnEntries_) ? currentEntry_ + 1 : 1;
        if (tmp != currentEntry_) {
            currentEntry_ = tmp;
            ready_ = false;
            stat = true;
        }
        return stat;
    }
    bool setPreviousEntry() {
        bool stat = false;
        uint16_t tmp = currentEntry_;
        tmp = (tmp > 1) ? tmp - 1 : numJrnEntries_;
        if (tmp != currentEntry_) {
            currentEntry_ = tmp;
            ready_ = false;
            stat = true;
        }
        return stat;
    }

    // Установка номера записи
    bool setNumEntry(uint16_t num) {
        bool stat = false;
        if (num <= numJrnEntries_) {
            currentEntry_ = num;
            ready_ = false;
            stat = true;
        }
        return stat;
    }

    // утстановка и считывание флага получения информации о текущей записи
    bool setReady() {
        return (ready_ = true);
    }
    bool isReady() const {
        return ready_;
    }

   private:
    // текущий журнал
    eGB_DEVICE currentDevice_;

    // устройство
    eGB_DEVICE_K400 deviceJrn_;

    // тип события
    uint8_t eventType_;

    // источник команды (номер аппарата с которого пришла команда)
    uint8_t srcCom_;

    // режим
    eGB_REGIME regime_;

    // сигналы для журнала защиты
    bool signalPusk_;
    bool signalStop_;
    bool signalMan_;
    bool signalPrm_;
    bool signalPrd_;
    bool signalOut_;	// Выход приемника (ПРМ2 или РЗвых)
    uint8_t signals_;

    // буфер записи для журналов ОПТИКИ
    uint8_t opticEntry_[4];

    // буфер записи для журналов ОПТИКИ ЦПП
    uint8_t opticEntryDR_[4];

    // кол-во событий в одной записи журнала ОПТИКИ
    uint8_t numOpticEntries_;

    // кол-во событий в одной записи журнала ОПТИКИ ЦПП
    uint8_t numOpticEntriesDR_;

    // кол-во записей в журнале
    uint16_t numJrnEntries_;

    // максимальное кол-во записей в журнале
    uint16_t maxNumJrnEntry_;

    // переполнение журнала
    bool overflow_;

    // адрес первой записи
    uint16_t addressFirstEntry_;

    // адрес текущей записи (отображаемой на экране)
    uint16_t currentEntry_;

    // номер команды в текущей записи
    uint16_t numCom_;

    // источник передаваемой команды
    eGB_SOURCE_COM sourceCom_;

    // флаг получения информации о текущей записи
    bool ready_;
};

#endif /* JOURNAL_ENTRY_HPP_ */
