#include "param.h"

/** Возвращает указатель на структуру параметра.
 *
 * @param[in] params Список параметров.
 * @param[in] pn Параметр.
 * @return Указатель на структуру параметра.
 */
Param const * getPtrParam(eGB_PARAM pn) {
    return (Param const*) (pgm_read_ptr(&fParams[pn]));
}

/** Возвращает минимальное значение параметра.
 *
 *  @param[in] pn Параметр.
 * 	@return Минимальное значение параметра.
 */
int16_t getAbsMin(eGB_PARAM pn) {
    return static_cast<int16_t> (pgm_read_word(&getPtrParam(pn)->min));
}

/** Возвращает абсолютный максимум значения параметра.
 *
 *  Текущий максимум может зависеть от других параметров.
 *
 *  @param[in] pn Параметр.
 * 	@return Абсолютный максимум значения параметра.
 */
int16_t getAbsMax(eGB_PARAM pn) {
    return static_cast<int16_t> (pgm_read_word(&getPtrParam(pn)->max));
}

/** Возвращает абсолютный максимум количества однотипных параметров.
 *
 *  Текущее количество может зависеть от других параметров.
 *
 *  @param[in] pn Параметр.
 * 	@return Абсолютный максимум количества однотипных параметров.
 */
uint8_t getAbsMaxNumOfSameParams(eGB_PARAM pn) {
    return pgm_read_byte(&getPtrParam(pn)->num);
}

/** Возвращает требование к режиму для изменения параметра.
 *
 *  @param[in] pn Параметр.
 *  @return Требование к режиму для изменения параметра.
 */
Param::CHANGE_REG getChangeReg(eGB_PARAM pn) {
    uint8_t value = pgm_read_byte(&getPtrParam(pn)->changeReg);
    return static_cast<Param::CHANGE_REG> (value);
}

/**	Возвращает команду стандартного протокола для параметр.
 *
 *  @param[in] pn Параметр.
 * 	@return Команда стандартного протокола.
 */
eGB_COM getCom(eGB_PARAM pn) {
    uint8_t value = pgm_read_byte(&getPtrParam(pn)->com);
    return static_cast<eGB_COM> (value);
}

/** Возвращает зависимость максимума для параметра.
 *
 *  @param[in] pn Параметр.
 *  @return Зависимость максимума для параметра.
 */
Param::DEPEND_MAX getDependMax(eGB_PARAM pn) {
    uint8_t value = pgm_read_byte(&getPtrParam(pn)->dependMax);
    return static_cast<Param::DEPEND_MAX> (value);
}

/**	Возвращает зависимость повторений для параметра.
 *
 *  @param[in] pn Параметр.
 * 	@return Зависимость повторений для параметра.
 */
Param::DEPEND_SAME getDependSame(eGB_PARAM pn) {
    uint8_t value = pgm_read_byte(&getPtrParam(pn)->dependSame);
    return static_cast<Param::DEPEND_SAME> (value);
}

/**	Возвращает размерность параметра.
 *
 *  @param[in] pn Параметр.
 * 	@return Размерность параметра.
 */
Param::DIMENSION getDim(eGB_PARAM pn) {
    uint8_t value = pgm_read_byte(&getPtrParam(pn)->dim);
    return static_cast<Param::DIMENSION> (value);
}

/** Возвращает дискретность для значения параметра.
 *
 *  @param[in] pn Параметр.
 * 	@return Дискретность для значения параметра.
 */
uint8_t getDisc(eGB_PARAM pn) {
    return pgm_read_byte(&getPtrParam(pn)->disc);
}

/** Возвращает множитель значения для параметра.
 *
 *  @param[in] pn Параметр.
 * 	@return Множитель для значения параметра.
 */
uint8_t getFract(eGB_PARAM pn) {
    return pgm_read_byte(&getPtrParam(pn)->fract);
}

/** Возвращает указатель на начало списка значений параметра.
 *
 *  @param[in] pn Параметр.
 * 	@return Указатель на начало списка значений.
 */
PGM_P getListOfValues(eGB_PARAM pn) {
    return static_cast<PGM_P> (pgm_read_ptr(&getPtrParam(pn)->listValues));
}

/** Возвращает указатель на строку с названием параметра.
 *
 *  @param[in] pn Параметр.
 * 	@return Указатель на строку с названием параметра.
 */
PGM_P getNameOfParam(eGB_PARAM pn) {
    return (PGM_P) &getPtrParam(pn)->name;
}

/** Возвращает тип параметра.
 *
 *  @param[in] pn Параметр.
 * 	@return Тип параметра.
 */
Param::PARAM_TYPE getParamType(eGB_PARAM pn) {
    uint8_t value = pgm_read_byte(&getPtrParam(pn)->param);
    return static_cast<Param::PARAM_TYPE> (value);
}

/**	Возвращает тип диапазона значений для параметра.
 *
 *  @param[in] pn Параметр.
 * 	@return Тип диапазона значений.
 */
Param::RANGE_TYPE getRangeType(eGB_PARAM pn) {
    uint8_t value = pgm_read_byte(&getPtrParam(pn)->range);
    return static_cast<Param::RANGE_TYPE> (value);
}

/**	Возвращает значение байта доп. информации для сохранения нового значения.
 *
 *  @param[in] pn Параметр.
 * 	@return Значение байта доп. информации.
 */
uint8_t getSendDop(eGB_PARAM pn) {
    return pgm_read_byte(&getPtrParam(pn)->sendDop);
}

/**	Возвращает тип параметра для сохранения нового значения.
 *
 *  @param[in] pn Параметр.
 * 	@return Тип параметра для сохранения.
 */
eGB_SEND_TYPE getSendType(eGB_PARAM pn) {
    uint8_t value = pgm_read_byte(&getPtrParam(pn)->send);
    return static_cast<eGB_SEND_TYPE> (value);
}
