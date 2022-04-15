PIG_PATH = $$system(pwd)/PIg

INCLUDEPATH += \
    $$system(pwd) \
    $${PIG_PATH}/src

HEADERS += \
    $${PIG_PATH}/src/avr.h \
    $${PIG_PATH}/src/debug/debug.hpp \
    $${PIG_PATH}/src/drivers/keyboard.h \
    $${PIG_PATH}/src/drivers/ks0108.h \
    $${PIG_PATH}/src/drivers/tmp75.h \
    $${PIG_PATH}/src/drivers/uart.h \
    $${PIG_PATH}/src/menu/base.h \
    $${PIG_PATH}/src/menu/menu.h \
    $${PIG_PATH}/src/protocols/standart/protocolS.h

SOURCES += \
    $${PIG_PATH}/src/debug/debug.cpp \
    $${PIG_PATH}/src/menu/base.cpp \
    $${PIG_PATH}/src/menu/menu.cpp \
    $${PIG_PATH}/src/protocols/standart/protocolS.cpp
