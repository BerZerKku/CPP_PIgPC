include(../PIg.pri)

QT += testlib         #Include QtTest to use SignalSpy, QTest::mouseClick, etc

TARGET = Tests
TEMPLATE = app

CONFIG += c++14
CONFIG += testcase
CONFIG -= debug_and_release
CONFIG += cmdline

DEFINES += \
    MY_TESTS \
	NDEBUG

INCLUDEPATH += \
    googletest \
	$${PIG_PATH}/src

HEADERS += \

SOURCES += \
    googletest/gtest-all.cc \
	main.cpp \
	CIec101Test.cpp \
	TFifoTest.cpp \	
	TProtocolModbusTest.cpp \
	TPwdTest.cpp \
	TSecurityEventTest.cpp \
	TUserTest.cpp \	
	$${PIG_PATH}/src/protocols/iec101/CIec101.cpp \
	$${PIG_PATH}/src/protocols/modbus/protocolModbus.cpp \
	$${PIG_PATH}/src/security/pwd.cpp \
	$${PIG_PATH}/src/security/securityevent.cpp \
	$${PIG_PATH}/src/security/user.cpp
