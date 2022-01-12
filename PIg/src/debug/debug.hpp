/*
 * debug.hpp
 *
 *  Created on: 27.04.2012
 *      Author: ’оз€ин
 */

#ifndef DEBUG_HPP_
#define DEBUG_HPP_

#include <stdint.h>

#if defined(AVR)

    #include <avr/io.h>

    // тестовые пины
    #define DDR_DBG  DDRE
    #define PORT_DBG PORTE
    #define PIN_TP1  (1 << 6)
    #define PIN_TP2  (1 << 7)

    #define SET_TP1 PORT_DBG |= PIN_TP1
    #define CLR_TP1 PORT_DBG &= ~PIN_TP1
    #define TOG_TP1 PORT_DBG ^= PIN_TP1

    #define SET_TP2 PORT_DBG |= PIN_TP2
    #define CLR_TP2 PORT_DBG &= ~PIN_TP2
    #define TOG_TP2 PORT_DBG ^= PIN_TP2

    #define Q_ASSERT(x)  (void(0))
    #define QDEBUG(x)    (void(0))
    #define QDINFO(x)    (void(0))
    #define QWARNING(x)  (void(0))
    #define QCRITICAL(x) (void(0))
    #define COMPILE_TIME_ASSERT(expression) \
        switch (0)                          \
        {                                   \
        case 0:                             \
        case (expression):;                 \
        }

#elif defined(QT_CORE_LIB)

    #include <QDebug>

    #define QDEBUG(x)                       (qDebug() << x)
    #define QINFO(x)                        (qInfo() << x)
    #define QWARNING(x)                     (qWarning() << x)
    #define QCRITICAL(x)                    (qCritical() << x)
    #define COMPILE_TIME_ASSERT(expression) Q_STATIC_ASSERT_X(expression, "Error")

#endif

//
#if defined(NDEBUG)

    #define SET_DEBUG_BYTE(number, value) (void(0))
    #define INC_DEBUG_BYTE(number)        (void(0))

#else  // defined(NDEBUG)

// вывод отладочной информации на экран
//#define VIEW_DEBUG_PARAM
//#define IEC101

// отладочна€ информаци€
struct stDebug
{
    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;
    uint8_t byte4;
    uint8_t byte5;
    uint8_t byte6;
    uint8_t byte7;
    uint8_t byte8;
};

extern stDebug sDebug;

    #define SET_DEBUG_BYTE(number, value) (sDebug.byte##number = value)
    #define INC_DEBUG_BYTE(number)        (sDebug.byte##number = sDebug.byte##number + 1)

#endif  // defined(NDEBUG)

#endif /* DEBUG_HPP_ */
