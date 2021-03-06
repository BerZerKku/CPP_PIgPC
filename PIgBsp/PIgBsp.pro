include(../PIg.pri)

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

CONFIG(debug, debug|release) {
    message(===Debug===)

#    DEFINES += \
#        QT_MESSAGELOGCONTEXT
}

CONFIG(release, debug|release) {
    message(===Release===)

    DEFINES += \
        NDEBUG
}

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += \
    MY_TESTS \
    QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH +=

SOURCES += \
    $${PIG_PATH}/src/flashParam.cpp \
	$${PIG_PATH}/src/parameter/param.cpp \
	$${PIG_PATH}/src/security/infosecurity.cpp \
	$${PIG_PATH}/src/security/pwd.cpp \
	$${PIG_PATH}/src/security/securityevent.cpp \
	$${PIG_PATH}/src/security/user.cpp \
    bsp.cpp \
    main.cpp \
    mainwindow.cpp \
    serial.cpp \
    serial/serialport.cpp \
    widget/combobox.cpp \
    widget/qledindicator.cpp \
    widget/qpush1.cpp \
    wrapper.cpp

HEADERS += \
    $${PIG_PATH}/src/avr.h \
	$${PIG_PATH}/src/debug/debug.hpp \
	$${PIG_PATH}/src/flash.h \
	$${PIG_PATH}/src/flashParams.h \
	$${PIG_PATH}/src/glbDefine.h \
	$${PIG_PATH}/src/menu/deviceStatus.hpp \
	$${PIG_PATH}/src/paramIS.h \
	$${PIG_PATH}/src/paramUart.h \
	$${PIG_PATH}/src/parameter/param.h \
	$${PIG_PATH}/src/protocols/standart/protocolBspS.h \
	$${PIG_PATH}/src/protocols/standart/protocolPcS.h \
	$${PIG_PATH}/src/protocols/standart/protocolS.h \
	$${PIG_PATH}/src/security/infosecurity.h \
	$${PIG_PATH}/src/security/pwd.h \
	$${PIG_PATH}/src/security/securityevent.h \
	$${PIG_PATH}/src/security/user.h \
    bsp.h \
    mainwindow.h \
    serial.h \
    serial/serialport.h \
    widget/combobox.h \
    widget/qledindicator.h \
    widget/qpush1.h \
    wrapper.hpp

FORMS += \
    mainwindow.ui \
    serial.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
