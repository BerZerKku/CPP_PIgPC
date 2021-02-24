COMMON_PATH = $$system(pwd)/common

INCLUDEPATH += \
    $$COMMON_PATH

HEADERS += \
    $$COMMON_PATH/qwintexttounicode.h

SOURCES += \
    $$COMMON_PATH/qwintexttounicode.cpp
