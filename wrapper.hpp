#ifndef WRAPPER_HPP
#define WRAPPER_HPP

#include "avr/io.h"
#include "avr/pgmspace.h"
#include <QDebug>

#define COMPILE_TIME_ASSERT(expression) \
    Q_STATIC_ASSERT_X(expression, "Error")

#endif // WRAPPER_HPP
