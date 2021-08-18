/*
 * enterParam.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef ENTERPARAM_H_
#define ENTERPARAM_H_

#include "glbDefine.h"
#include "paramGlb.h"

// Настройки необходимые для ввода нового значения параметра.
class TEnterParam
{
public:
    /**	Конструктор.
     */
    TEnterParam() { setDisable(); }

    /**	Проверка текущего статуса работы с параметром.
     *
     * 	@param Нет
     * 	@return True - если идет ввод нового значения, иначе False.
     */
    bool isEnable();

    /** Старт ввода нового значения параметра.
     * 	 Если ввод из списка, автоматически дискретность и делитель равны 1.
     * 	Для пароля автоматически выставляются диапазон ввода и начальное значение.
     *
     * 	@param s Режим работы функции ввода параметров.
     */
    void setEnable(eMENU_ENTER_PARAM s = MENU_ENTER_PARAM_INT);

    /**	Окончание ввода нового значения параметра.
     *
     * 	@param Нет
     * 	@return Нет
     */
    void setDisable();

    // Установка диапазона возможных значений.
    void setValueRange(int16_t min, int16_t max)
    {
        max_ = max;
        min_ = min;
    }

    // Возвращает минимальное значение параметра.
    int16_t getValueMin() const { return min_; }

    // Возвращает максимальное значение параметра.
    int16_t getValueMax() const { return max_; }

    // установка текущего значения, диапазон значений должен быть задан до !
    void setValue(int16_t val) { val_ = (val < min_) || (val > max_) ? min_ : val; }

    // возвращает текущее значение
    int16_t getValue() const { return val_; }

    // возвращает введеное значение с учетом дискретности и делителя
    int16_t getValueEnter() const { return ((val_ / disc_) * disc_) / fract_; }

    /** Увеличение текущего значения.
     *
     * 	@param velocity Скорость изменения значения (для ввода целых значений
     * 	и коррекции напряжения).
     * 	@argval 0 Увеличение на шаг заданный дискретностью.
     * 	@argval 1 Увеличение на шаг в 10 раз больше заданной дискретности.
     * 	@argval 2 Увеличение на шаг в 50 раз больше заданной дискретности.
     */
    uint16_t incValue(uint8_t velocity = 0);

    /** Уменьшение текущего значения.
     *
     * 	@param velocity Скорость изменения значения (для ввода целых значений
     * 	и коррекции напряжения).
     * 	@argval 0 Увеличение на шаг заданный дискретностью.
     * 	@argval 1 Увеличение на шаг в 10 раз больше заданной дискретности.
     * 	@argval 2 Увеличение на шаг в 50 раз больше заданной дискретности.
     */
    uint16_t decValue(uint8_t velocity = 0);

    // установка номера параметра
    void setParam(eGB_PARAM param) { param_ = param; }

    // возвращает номер параметра
    eGB_PARAM getParam() const { return param_; }

    // установка дополнительного байта
    void setDopValue(uint16_t byte) { dopValue_ = byte; }

    // возвращает дополнительный байт
    uint16_t getDopValue() const { return dopValue_; }

    // установка дискретности
    void setDisc(uint16_t disc) { disc_ = disc; }

    // возвращает дискретность
    uint16_t getDisc() const { return disc_; }

    // установка делителя
    void setFract(uint8_t fract) { fract_ = fract; }

    // возвращает делитель
    uint8_t getFract() const { return fract_; }

    // вывод сообщения на экран
    // по умолчанию работает для функции ввода параметра
    void printMessage()
    {
        status_ = MENU_ENTER_PARAM_MESSAGE;
        cnt_    = 0;
    }

    // возвращает текущий статус
    eMENU_ENTER_PARAM getStatus() const { return status_; }

    // установка флага окончания ввода параметра
    void setEnterValueReady(eMENU_ENTER_PARAM status = MENU_ENTER_PARAM_READY) { status_ = status; }

    // указатель на первый элемент списка
    PGM_P list;

    // указатель на массив значений
    uint8_t *listValue;

    // команда на передачу
    eGB_COM com;

    // счетчик времени
    uint8_t cnt_;

private:
    // текущее значение
    int16_t val_;

    // максимальное значение
    int16_t max_;

    // минимальное значение
    int16_t min_;

    // байт с дополнительой информацией
    uint16_t dopValue_;

    // дискретность
    uint16_t disc_;

    // делитель
    uint8_t fract_;

    // текущий статус устройства
    eMENU_ENTER_PARAM status_;

    // текущий параметр
    eGB_PARAM param_;
};


#endif /* ENTERPARAM_H_ */
