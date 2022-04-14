include(../PIg.pri)

QT += testlib         #Include QtTest to use SignalSpy, QTest::mouseClick, etc

TARGET = Tests
TEMPLATE = app

CONFIG += c++14
CONFIG += testcase
CONFIG -= debug_and_release
CONFIG += cmdline

DEFINES += \
    NDEBUG \
    MY_TESTS \
    QT_NO_DEBUG

INCLUDEPATH += \
    googletest/ \
    $${PIG_PATH}/src/ \
    $${PIG_PATH}/src/menu

HEADERS += \
    wrapper.h

SOURCES += \
    googletest/gtest-all.cc \
    main.cpp \
    menu/test_clMenu.cpp \
    wrapper.cpp

