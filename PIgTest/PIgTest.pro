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
    MY_TESTS

INCLUDEPATH += \
    googletest/ \
    $${PIG_PATH}/src/ \
    $${PIG_PATH}/src/menu \
    $${PIG_PATH}/src/parameter \
    $${PIG_PATH}/src/protocols/iec101 \
    $${PIG_PATH}/src/protocols/modbus

HEADERS += \
    wrapper.h

SOURCES += \
    googletest/gtest-all.cc \
    main.cpp \
    menu/test_clMenu.cpp \
    test_flash.cpp \
    test_flashParam.cpp \
    wrapper.cpp \
    menu/test_control.cpp \
    menu/test_dateTime.cpp \
    menu/test_measure.cpp \
    menu/test_menuPunkt.cpp \
    menu/test_password.cpp \
    menu/test_test.cpp \
    menu/test_txCom.cpp \
    parameter/test_LocalParams.cpp \
    protocols/iec101/test_CIec101.cpp \
    protocols/modbus/test_protocolModbus.cpp \
    protocols/standart/test_protocolBspS.cpp

