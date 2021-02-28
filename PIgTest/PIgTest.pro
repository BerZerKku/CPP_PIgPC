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
    googletest \

HEADERS += \
    wrapper.hpp

SOURCES += \
    googletest/gtest-all.cc \
	main.cpp \
	CIec101Test.cpp \
    clMenuTest.cpp \
	TProtocolModbusTest.cpp \
    wrapper.cpp



#    $${PIG_PATH}/src/menu/menu.cpp \
#	$${PIG_PATH}/src/protocols/iec101/CIec101.cpp \
#	$${PIG_PATH}/src/protocols/modbus/protocolModbus.cpp \
