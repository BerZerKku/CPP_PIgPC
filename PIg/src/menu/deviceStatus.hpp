/*
 * enterParam.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef DEVICE_STATUS_HPP_
#define DEVICE_STATUS_HPP_

#include <stdint.h>
#include "src/glbDefine.h"

/// Класс для текущего состояния аппарата
class TDeviceStatus {
   public:
    /// Максимальное кол-во вариантов для номеров удаленных аппаратов
    // т.е. используются 3 бита, 0 бит - 1 аппарат, ... 2 биит - 3 аппарат.
    // если установлены все три - ошибка.
    static const uint8_t GB_MAX_REM_NUM = 7;

    TDeviceStatus() {
        // присваивание иемени по умолчанию
        static const char nameDev[] PROGMEM = "НЕТ";
        name = nameDev;
        enable_ = false;
        fault_ = 0;
        faults_ = 0;
        numFaults_ = 0;
        warning_ = 0;
        warnings_ = 0;
        numWarnings_ = 0;
        remoteNumber_ = 0;
        regime_ = GB_REGIME_MAX;
        state_ = MAX_NUM_DEVICE_STATE;
        dopByte_ = 0;
    }

    /**	Установка номера удаленного аппарата.
     *
     * 	Используется для вывода номеров удаленных аппаратов, для которых
     * 	сформировалась ошибка (АВАНТ Р400(М) совместимость с ПВЗУ-Е).
     *
     * 	@param val Номер аппарата(ов). Три младших бита отвечают за соответсвую-
     * 	щий аппарат.
     * 	@retval True - в случае успешной записи параметра.
     * 	@retval False - если была обнаружена ошибка данных
     */
    bool setRemoteNumber(uint8_t val) {
        bool state = false;

        if (val > GB_MAX_REM_NUM) {
            remoteNumber_ = GB_MAX_REM_NUM;
        } else {
            remoteNumber_ = val;
            state = true;
        }

        return state;
    }

    /** Возвращает номер удаленного аппарата.
     *
     * 	Используется для вывода номеров удаленных аппаратов, для которых
     * 	сформировалась ошибка (АВАНТ Р400(М) совместимость с ПВЗУ-Е).
     *
     * 	@param Номер аппарата(ов), а точнее строка в массиве  fcRemoteNum.
     */
    uint8_t getRemoteNumber() const {
        return remoteNumber_;
    }

    // аварии
    bool setFault(uint16_t faults) {
        fault_ = getFirstSetBit(faults);
        faults_ = faults;
        numFaults_ = getNumSetBits(faults);
        return true;
    }
    uint8_t getFault() const {
        return fault_;
    }
    uint16_t getFaults() const {
        return faults_;
    }
    uint8_t getNumFaults() const {
        return numFaults_;
    }

    // неисправности
    bool setWarning(uint16_t warnings) {
        warning_ = getFirstSetBit(warnings);
        warnings_ = warnings;
        numWarnings_ = getNumSetBits(warnings);
        return true;
    }
    uint8_t getWarning() const {
        return warning_;
    }
    uint16_t getWarnings() const {
        return warnings_;
    }
    uint8_t getNumWarnings() const {
        return numWarnings_;
    }

    /**	Возвращает текущее состояние указанного номера предупреждения.
     *
     *	При значении аргумента 0xFF возвращает true если есть хотя бы одно
     *	предупреждение. При значениях от 0
     *
     *	@param shift Номер предупреждения.
     *	@return Состояние предупреждения.
     */
    bool isWarning(uint8_t shift=0xFF) const {
        bool s = false;
        if (shift == 0xFF) {
            s = (warnings_ != 0);
        } else if (shift < 16) {
            s = ((warnings_ & (1 << shift)) != 0);
        }
        return s;
    }

    /**	Возвращает текущее состояние указанного номера неисправности.
     *
     *	При значении аргумента 0xFF возвращает true если есть хотя бы одно
     *	предупреждение. При значениях от 0
     *
     *	@param shift Номер неисправности.
     *	@return Состояние неисправности.
     */
    bool isFault(uint8_t shift=0xFF) const {
        bool s = false;
        if (shift == 0xFF) {
            s = (faults_ != 0);
        } else if (shift < 16) {
            s = ((faults_ & (1 << shift)) != 0);
        }
        return s;
    }

    // режим работы
    // для GLB возвращается GB_REGIME_DISABLED, в случае если все имеющиеся
    // устройства выведены, иначе GB_REGIME_MAX
    bool setRegime(eGB_REGIME regime) {
        bool stat = false;
        if ((regime >= GB_REGIME_MIN) && (regime <= GB_REGIME_MAX)) {
            stat = true;
        } else
            regime = GB_REGIME_MAX;
        regime_ = regime;
        return stat;
    }
    eGB_REGIME getRegime() const {
        return regime_;
    }

    //состояние
    bool setState(uint8_t state) {
        bool stat = false;
        if (state < MAX_NUM_DEVICE_STATE) {
            stat = true;
        } else {
            state = MAX_NUM_DEVICE_STATE;
        }
        state_ = state;
        return stat;
    }
    uint8_t getState() const {
        return state_;
    }

    uint8_t getDopByte() const {
        return dopByte_;
    }
    bool setDopByte(uint8_t byte) {
        dopByte_ = byte;
        return true;
    }

    // работа с флагом наличия устройства
    // возвращает true если новое значение отличается от текущего
    bool setEnable(bool val) {
        uint8_t act = GB_ACT_NO;

        if (enable_ == val) {
            act |= GB_ACT_OLD;
        } else {
            act |= GB_ACT_NEW;
        }
        enable_ = val;

        return act;
    }
    bool isEnable() const {
        return enable_;
    }

    // массивы расшифровок аварий и предупреждений
    PGM_P faultText[MAX_NUM_FAULTS];
    PGM_P warningText[MAX_NUM_WARNINGS];
    PGM_P stateText[MAX_NUM_DEVICE_STATE + 1];
    PGM_P name;

   private:
    // текущая приоритетная неисправность, неисправности и кол-во неисправностей
    uint8_t fault_;
    uint16_t faults_;
    uint8_t numFaults_;

    // текущее приоритетное предупреждение, предупреждения и кол-во предупрежд.
    uint8_t warning_;
    uint16_t warnings_;
    uint8_t numWarnings_;

    // Номер удаленного аппарата для которого была сформирована ошибка/предупр.
    // используется в Р400(М) в совместимости с ПВЗУ-Е
    uint8_t remoteNumber_;

    eGB_REGIME regime_;
    uint8_t state_;
    uint8_t dopByte_;

    // True - означает наличие данного устройства в текущем исполнении АВАНТа
    bool enable_;


    // возвращает кол-во установленных в 1 бит
    uint8_t getNumSetBits(uint16_t val)
    {
        uint8_t cnt = 0;
        for(; val > 0; val >>= 1)
        {
            if (val & 0x0001)
                cnt++;
        }
        return cnt;
    }

    // возвращает номер младшего установленного бита
    uint8_t getFirstSetBit(uint16_t val)
    {
        uint8_t cnt = 0;
        for(; val > 0; val >>= 1)
        {
            if (val & 0x0001)
                break;
            cnt++;
        }
        return cnt;
    }
};

#endif /* DEVICE_STATUS_HPP_ */
