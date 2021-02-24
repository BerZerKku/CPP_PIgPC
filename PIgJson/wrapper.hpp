#ifndef WRAPPER_HPP
#define WRAPPER_HPP

#include <QDebug>

#define COMPILE_TIME_ASSERT(expression) \
    Q_STATIC_ASSERT_X(expression, "Error")

#endif // WRAPPER_HPP
