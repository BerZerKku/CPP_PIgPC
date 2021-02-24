include(../PIg.pri)

QT += testlib         #Include QtTest to use SignalSpy, QTest::mouseClick, etc

TARGET = Tests
TEMPLATE = app

CONFIG += c++14
CONFIG += testcase
CONFIG -= debug_and_release
CONFIG += cmdline

DEFINES += \
	NDEBUG

INCLUDEPATH += \
    googletest \

HEADERS += \

SOURCES += \
    googletest/gtest-all.cc \
	main.cpp \
	CIec101Test.cpp \
	TProtocolModbusTest.cpp \
	$${PIG_PATH}/src/protocols/iec101/CIec101.cpp \
	$${PIG_PATH}/src/protocols/modbus/protocolModbus.cpp \
