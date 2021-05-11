#ifndef PARAM_H_
#define PARAM_H_

#include <stdint.h>
#include "src/avr.h"
#include "src/glbDefine.h"

struct Param {
	/// Размернось параметра (связана с fcDimension)
	typedef enum {
		DIM_NO = 0,		///< Нет
		DIM_MA,			///< мА
		DIM_A,			///< А
		DIM_KA,			///< кА
		DIM_V,			///< В
		DIM_KV,			///< кВ
		DIM_MSEC,		///< мс
		DIM_SEC,		///< с
		DIM_HOUR,		///< час
		DIM_DB,			///< дБ
		DIM_HZ,			///< Гц
		DIM_KHZ,		///< кГц
		DIM_DEG			///< град
	} DIMENSION;

	/// Тип строки для отображения диапазона значений параметра.
	typedef enum {
		RANGE_LIST = 0,	///< Список.
		RANGE_ON_OFF,	///< Список вкл./выкл.
		RANGE_INT,		///< <min>..<max><dim>
		RANGE_INT_NO_DIM,///< <min>..<max>
		RANGE_U_COR,	///< 0..±<max/10><dim>
        RANGE_I_COR,	///< 0..±<max><dim>
        RANGE_PWD       ///< <max password value>
	} RANGE_TYPE;

	/// Тип параметра, в плане ввода / отображения.
	typedef enum {
		PARAM_NO = 0,	///< Неизменяемый параметр.
		PARAM_LIST,		///< Список.
        PARAM_INT,		///< Целое значение от -32768..32767.
		PARAM_U_COR,	///< Коррекция напряжения ±ХХ.Х.
		PARAM_I_COR,	///< Коррекция тока ±XXX.
        PARAM_BITES,	///< Набор битовых значений.
        PARAM_PWD       ///< Пароль.
	} PARAM_TYPE;

	/// Зависимость максимума параметра.
	typedef enum {
        DEPEND_MAX_NO = 0,			///< Нет зависимости.
		DEPEND_MAX_ON_COM_PRD,		///< Зависит от количества команд на ПРД.
		DEPEND_MAX_ON_NUM_DEVS		///< Зависит от количества команд на ПРМ.
	} DEPEND_MAX;

	/// Зависимость повторов параметра.
	typedef enum {
        DEPEND_SAME_NO = 0,			///< Нет зависимости.
		DEPEND_SAME_ON_NUM_DEVS,	///< Зависит от количества аппаратов в линии.
		DEPEND_SAME_ON_COM_PRD,		///< Зависит от количества команд на ПРД.
		DEPEND_SAME_ON_COM_PRM		///< Зависит от количества команд на ПРМ.
	} DEPEND_SAME;

    /// Необходимые режим для изменения параметра.
	typedef enum {
        CHANGE_REG_NO = 0,  ///< Нет условий.
        CHANGE_REG_DISABLE	///< Изменить можно только в режиме "Выведен".
    } CHANGE_REG;

	char name[NAME_PARAM_LENGHT];	///< Имя параметра.
	eGB_COM com;		///< Команда считывания для стандартного протокола.

	PARAM_TYPE param;	///< Тип параметра.
	RANGE_TYPE range;	///< Тип строки для вывода диапазона значений.
	DIMENSION dim;		///< Размерность параметра.
	PGM_P listValues;	///< Массив параметров для строковых переменных.

	uint8_t num;		///< Количество подобных параметров.
	int16_t min;		///< Минимальное значение параметра.
	int16_t max;		///< Максимальное значение параметра или кол-во элементов в списке
	uint8_t disc;		///< Дискретность.
	uint8_t fract;		///< Множитель для обмена по протоколу с БСП.

	eGB_SEND_TYPE send;		///< Тип параметра для сохранения нового значения.
	uint8_t sendDop;		///< Байт дополнительной информации для сохранения.
	DEPEND_MAX dependMax;	///< Зависимость максимума текущего параметра.
	DEPEND_SAME dependSame;	///< Зависимость повторений текущего параметра.
    CHANGE_REG changeReg;	///< Необходимый режим для изменения параметра.
};

extern const Param *fParams[] PROGMEM;

// Возвращает указатель на структуру параметра.
extern Param const *getPtrParam(eGB_PARAM pn);

// Возвращает минимум значения параметра.
extern int16_t getAbsMin(eGB_PARAM pn);
// Возвращает абсолютный максимум значения параметра.
extern int16_t getAbsMax(eGB_PARAM pn);
// Возвращает абсолютный максимум количества однотипных параметров.
extern uint8_t getAbsMaxNumOfSameParams(eGB_PARAM pn);
// Возвращает требование к режиму для изменения параметра.
extern Param::CHANGE_REG getChangeReg(eGB_PARAM pn);
// Возвращает команду стандартного протокола для параметр.
extern eGB_COM getCom(eGB_PARAM pn);
// Возвращает зависимость максимума для параметра.
extern Param::DEPEND_MAX getDependMax(eGB_PARAM pn);
// Возвращает зависимость повторений для параметра.
extern Param::DEPEND_SAME getDependSame(eGB_PARAM pn);
// Возвращает размерность параметра.
extern Param::DIMENSION getDim(eGB_PARAM pn);
// Возвращает дискретность для значения параметра.
extern uint8_t getDisc(eGB_PARAM pn);
// Возвращает множитель значения для параметра.
extern uint8_t getFract(eGB_PARAM pn);
// Возвращает указатель на начало списка значений параметра.
extern PGM_P getListOfValues(eGB_PARAM pn);
// Возвращает указатель на строку с названием параметра.
extern PGM_P getNameOfParam(eGB_PARAM pn);
// Возвращает тип параметра.
extern Param::PARAM_TYPE getParamType(eGB_PARAM pn);
// Возвращает тип диапазона значений для параметра.
extern Param::RANGE_TYPE getRangeType(eGB_PARAM pn);
// Возвращает значение байта доп. информации для сохранения нового значения.
extern uint8_t getSendDop(eGB_PARAM pn);
// Возвращает тип параметра для сохранения нового значения.
extern eGB_SEND_TYPE getSendType(eGB_PARAM pn);

#endif /* PARAM_H_ */
