/*
 * menu.h
 *
 *  Created on: 01.05.2012
 *      Author: Хозяин
 */

#ifndef MENU_H_
#define MENU_H_

#include "drivers/keyboard.h"
#include "glbDefine.h"
#include "menu/control.h"
#include "menu/enterParam.h"
#include "menu/menuPunkt.h"
#include "paramBsp.h"
#include "parameter/LocalParams.h"


/// время до первой инициализации дисплея, мс
#define TIME_TO_INIT_LCD (200 / MENU_TIME_CYLCE)

/// время до переинициализации дисплея, мс
#define TIME_TO_REINIT_LCD (10000 / MENU_TIME_CYLCE)

/// максимальное кол-во отображаемых на экране параметров
#define MAX_NUM_MEAS_PARAM 6

/// максимальное кол-во отображаемых параметров в меню "Измерение"
#define MAX_NUM_MEAS_PARAM_LVL 10

/// время вывода измеряемого параметра на экран, мс
#define TIME_MEAS_PARAM (3000 / MENU_TIME_CYLCE)

/// время вывода текста на экран, мс (например авария/код аварии)
#define TIME_TEXT (1000 / MENU_TIME_CYLCE)

/// время до выхода из Тест 1 / Тест 2 и т.д. если режим != данному тесту, мс
#define TIME_TEST_EXIT (1000 / MENU_TIME_CYLCE)

/// время до возврата на начальный уровень, с
#define TIME_RETURN_LVL_START (5 * 60 * (1000 / MENU_TIME_CYLCE))

/// Измеряемые параметры
enum eMENU_MEAS_PARAM
{
    MENU_MEAS_PARAM_NO,    ///< нет измеряемого параметра
    MENU_MEAS_PARAM_DATE,  ///< дата
    MENU_MEAS_PARAM_TIME,  ///< время
    MENU_MEAS_PARAM_UZ,    ///< запас по зщаите
    MENU_MEAS_PARAM_UZ1,   ///< запас по защите 1 (для 3-х концевой)
    MENU_MEAS_PARAM_UZ2,   ///< запас по защите 2 (для 3-х концевой)
    MENU_MEAS_PARAM_UC,    ///< запас по КЧ
    MENU_MEAS_PARAM_UC1,   ///< запас по КЧ 1 (для 3-х концевой)
    MENU_MEAS_PARAM_UC2,   ///< запас по КЧ 2 (для 3-х концевой)
    MENU_MEAS_PARAM_UN,    ///< уровень шумов
    MENU_MEAS_PARAM_UN1,   ///< уровень шумов 1 (для 3-х концевой)
    MENU_MEAS_PARAM_UN2,   ///< уровень шумов 2 (для 3-х концевой)
    MENU_MEAS_PARAM_UOUT,  ///< напряжени выхода
    MENU_MEAS_PARAM_IOUT,  ///< ток выхода
    MENU_MEAS_PARAM_ROUT,  ///< сопротивление линии
    MENU_MEAS_PARAM_SD,    ///< просечки в сигнале
    MENU_MEAS_PARAM_D,  ///< Запас по тест.команде (двухчаст) или Отношение сигнал/помеха (одночаст)
    MENU_MEAS_PARAM_TEMPERATURE,  ///< Температура на плате БСП-ПИ
    MENU_MEAS_PARAM_DF            ///< Отклонение частоты КС на ПРМ (К400)
};


// класс меню
class clMenu
{
public:
    /** Конструктор.
     *
     * Устнанавливается по умолчанию неопределеныый тип аппарата.
     * Заполняются необходимые для работы массивы.
     * Производится переход на стартовый уровень меню.
     */
    clMenu();

    /// Работа с меню.
    void proc();

    /** Установка типа аппарата и настройка меню с его учетом.
     *
     * По умолчанию будет сделан выбор исходя из меющейся информации в
     * настройках отдельных устройств.
     *
     * @param device Тип устройства
     * @return False в случае ошибки, иначе True
     */
    bool setDevice(eGB_TYPE_DEVICE device = AVANT_NO);

    /** Возвращает имеющуюся команду на исполнение.
     *
     * Сначала проверяется срочная команда, если ее нет идет перебор текущих.
     *
     * Каждый цикл состоящий из 8 команд опрашиваются:
     * - текущее состояние;
     * - время;
     * - команда из буфера 2
     *   - первым всегда идет текущий параметр, если есть
     *   - затем команды необходимые для настройки меню (совместимость и т.д.)
     * - команда из буфера 1
     *   -команды опроса остальных параметров в данном уровне меню
     * - если команд нет, то счетчик цикла сбрасывается.
     *
     * 	@param Нет
     * 	@return Команда
     */
    eGB_COM getTxCommand();

    /** Параметры
     */
    stGBparam sParam;

private:
    // код кнопки
    eKEY key_;

    // флаг мигания надписей
    bool blink_;

    // флаг смены измеряемых параметров
    bool blinkMeasParam_;

    // true - необходимо вывести на экран курсор
    bool cursorEnable_;

    // текущее положение курсора (номер строки)
    uint8_t cursorLine_;

    // кол-во отображаемых параметров
    uint8_t lineParam_;

    // текущий подпункт (например номер текущей команды параметра, начиная с 1)
    uint8_t curCom_;

    // если true - необходимо создать уровень меню
    bool lvlCreate_;

    // время вывода доп.сообщения на экран (например сообщения об ошибке)
    uint8_t delay_;

    // измеряемые параметры
    eMENU_MEAS_PARAM measParam[MAX_NUM_MEAS_PARAM * 2];

    // измеряемые параметры для уровня меню "Измерения"
    eMENU_MEAS_PARAM measParamLvl[MAX_NUM_MEAS_PARAM_LVL];

    // текущие пункты меню
    TMenuPunkt Punkts_;

    // параметры для ввода новых значений
    TEnterParam EnterParam;

    TControl mControl;

    //  настройки для соответствующих аппаратов
    bool setDeviceK400();
    bool setDeviceRZSK();
    bool setDeviceR400M();
    bool setDeviceOPTO();

    // очистка текстового буфера
    void clearTextBuf();

    // очистка строки
    void clearLine(uint8_t line);

    /// вывод сообщения на экран
    void printMessage() { delay_ = 0; }

    /// возвращает true - в случае необходимости вывода сообщения
    bool isMessage() const { return (delay_ < TIME_MESSAGE); }

    /// вывод на экран измеряемого параметра
    void printMeasParam(uint8_t poz, eMENU_MEAS_PARAM par);

    /// вывод на экран текущего состояния устройств
    void printDevicesStatus(uint8_t poz, TDeviceStatus *device);

    /// вывод в пунтке меню "Режим" текущего режима устройств
    void printDevicesRegime(uint8_t poz, TDeviceStatus *device);

    /// Уровни меню
    void lvlError();
    void lvlStart();
    void lvlFirst();
    void lvlInfo();
    void lvlJournal();
    void lvlJournalEvent();
    void lvlJournalDef();
    void lvlJournalPrm();
    void lvlJournalPrd();
    void lvlControl();
    void lvlSetup();
    void lvlRegime();
    void lvlRegimeAc();
    void lvlSetupParam();
    void lvlSetupParamDef();
    void lvlSetupParamPrm();
    void lvlSetupParamPrd();
    void lvlSetupParamGlb();
    void lvlSetupParamRing();
    void lvlSetupDT();
    void lvlMeasure();
    void lvlTest();
    void lvlTest1();
    void lvlTest2();
    void lvlSetupInterface();

    /// ввод параметра
    eMENU_ENTER_PARAM enterValue();
    /// ввод пароля
    eMENU_ENTER_PARAM enterPassword();

    /// перемещение курсора вверх
    void cursorLineUp()
    {
        cursorLine_ = (cursorLine_ > 1) ? cursorLine_ - 1 : Punkts_.getMaxNumPunkts();
    }

    /// пермещение курсора вниз
    void cursorLineDown()
    {
        cursorLine_ = (cursorLine_ < Punkts_.getMaxNumPunkts()) ? cursorLine_ + 1 : 1;
    }

    /// вывод на экран текущих пунктов меню и курсора
    void printPunkts();

    /// Вывод на экран текущего параметра.
    void printParam();

    /// Вывод на экран названия параметра.
    void printName();

    ///	Вывод на экран текущего номера и их колчиество для однотипных пар-ов.
    void printSameNumber(uint8_t pos);

    ///	Вывод на экран диапазона значений параметра.
    void printRange(uint8_t pos);

    /**	Вывод на экран текущего значения параметра.
     *
     * 	Если значение параметра записано с ошибкой, будет поочердено выводится
     * 	предупреждающая надпись и текущее значение параметра.
     *
     * 	Для строковых параметров учитывается минимальное значение. Т.е. из
     * 	текущего значения параметра вычитается минимальное значение, а затем
     * 	осуществляется выбор из массива строк значений параметра.
     */
    void printValue(uint8_t pos);

    /** Вывод на экран размерности параметра.
     *
     * @param[in] pos Текущая позиция.
     * @param[in] size Максимальное количество символов.
     * @param[in] pn Параметр.
     * @return Количество символов выведенных на экран.
     */
    uint8_t printDimension(uint8_t pos, uint8_t size, eGB_PARAM pn);

    /** Вывод на экран режима АК и времени до следующей проверки.
     *
     * Время выводится если соблюдаются условия:
     * - АК не выключен
     * - Режим "Введен"
     * - Состояние "Контроль"
     *
     * В Р400м производится подмена названий режимов АК:
     * - В совместимости ЛинияР: АК-норм меняется на АК-авт;
     * - В совместимости АВЗК-80 и ПВЗ-90: АК-бегл меняется на АК-пров.
     *
     * @param[in] pos Начальная позиция в буфере.
     */
    void printAc(uint8_t pos);

    /** Настройка параметров для ввода значения с клавиатуры.
     *
     * Для параметров типа \a Param::PARAM_NO функция ввода значения не
     * вызывается.
     *
     * Для параметров типа \a Param::PARAM_LIST вызывается функция выбора
     * значения из списка.
     *
     * Для параметров типа \a Param::PARAM_INT вызывается функция ввода
     * целого числа.
     *
     * Для параметров типа \a Param::PARAM_U_COR вызывается функция ввода
     * коррекции напряжения, а также минимальное значение подменяется на 0.
     *
     * Для параметров типа \a Param::PARAM_I_COR вызывается функция ввода целого
     * числа, при этом минимальное значение подменяется на 0.
     *
     * Для параметров типа \a Param::PARAM_BITES вызывается функция выбора
     * значения из списка.
     */
    void enterParameter();

    /// Формирование списка локальных параметров для Защиты.
    void crtListOfParamsDef();

    /// Формирование списка локальных параметров для Приемника.
    void crtListOfParamsPrm();

    /// Формирование списка локальных параметров для Передатчика.
    void crtListOfParamsPrd();

    /// Формирование списка локальных параметров для Общие.
    void crtListOfParamsGlb();

    // текущая функция ввода
    eMENU_ENTER_PARAM (clMenu::*enterFunc)();

    // текущий уровень меню
    void (clMenu::*lvlMenu)();

    /** Работа в меню настройки параметров.
     *
     * Если текущее количество параметров равно 0, то будет осуществлен возврат
     * к предыдущему уровню.
     */
    void setupParam();

    /** Проверка необходимости в подсветке.
     *
     * Необходимость включения подсветки проводится для каждого имеющегося
     * устройства.
     *
     * ОБЩ:
     * - режим не "Введен";
     * - имеется неисправность или предупреждение.
     * ЗАЩ:
     * - состояние не "Контроль";
     * - имеется неисправность или предупреждение.
     * ПРМ:
     * - состояние не "ПРМ КЧ";
     * - имеется неисправность или предупреждение.
     * ПРД:
     * - состояние не "ПРД КЧ";
     * - имеется неисправность или предупреждение.
     *
     * @retval True Включить подсветку.
     * @retval False Необходимости в подсветке нет.
     */
    bool checkLedOn();

    /** Проверка текущего аппарата РСЗК и совместимости РСЗКм.
     *
     * @return true если РСЗК и совместимость РЗСКм, иначе false.
     */
    bool isRzskM() const;

    /** Обработчик дополнительных функций кнопок клавиатуры.
     *
     * @param[in/out] key Нажатая кнопка. Если обработана то возвращается \a KEY_NO.
     * @return Сигнал управления.
     * @return \a TControl::CTRL_NO если сигнала нет.
     */
    TControl::ctrl_t onFnButton(eKEY &key);

    /**
     * @brief Возвращает расскладку клавиатуры.
     *
     * В К400 ВЧ всегда расскладка К400.
     * В Р400/Р400м ВЧ всегда расскладка Р400м.
     * В РЗСК ВЧ расскладка зависит от текущей совместимости:
     * - РЗСК для РЗСК;
     * - Р400м для РЗСКм;
     * - К400 для РЗСК-3 8к.
     * В Оптике расскладка зависит от наличия устройств:
     * - Р400м если только защита;
     * - РЗСК если есть защита и команды
     * - К400 если нет защиты.
     *
     * @return Расскладка устройства.
     */
    eGB_TYPE_DEVICE getKeyboardLayout();

    /**
     * @brief Добавляет команду для передачи сигнала управления.
     * @param[in] ctrl Сигнал управления.
     */
    void addControlToSend(TControl::ctrl_t ctrl);

    bool fillLvlControl(eGB_TYPE_DEVICE device);
    bool fillLvlControlR400m(eGB_COMP_R400M comp, eGB_NUM_DEVICES num_devices);
    bool fillLvlControlRzsk(eGB_NUM_DEVICES num_devices, bool def, bool comp_rzskm);
    bool fillLvlControlK400();
    bool fillLvlControlOpto(eGB_TYPE_OPTO type, bool def, bool prd, bool prm);


#ifdef TEST_FRIENDS
    TEST_FRIENDS
#endif
};

#endif /* MENU_H_ */
