include(../PIg.pri)

QT       += core gui serialport testlib
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
    QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH +=

SOURCES += \
    $${PIG_PATH}/src/drivers/kbdLayout.cpp \
    $${PIG_PATH}/src/flashParam.cpp \
    $${PIG_PATH}/src/menu/base.cpp \
    $${PIG_PATH}/src/menu/dateTime.cpp \
    $${PIG_PATH}/src/menu/enterParam.cpp \
    $${PIG_PATH}/src/menu/menu.cpp \
    $${PIG_PATH}/src/menu/txCom.cpp \
    $${PIG_PATH}/src/paramGlb.cpp \
    $${PIG_PATH}/src/parameter/LocalParams.cpp \
    $${PIG_PATH}/src/parameter/param.cpp \
    $${PIG_PATH}/src/protocols/iec101/CIec101.cpp \
    $${PIG_PATH}/src/protocols/iec101/protocolPcI.cpp \
    $${PIG_PATH}/src/protocols/modbus/protocolModbus.cpp \
    $${PIG_PATH}/src/protocols/modbus/protocolPcM.cpp \
    $${PIG_PATH}/src/protocols/standart/protocolBspS.cpp \
    $${PIG_PATH}/src/protocols/standart/protocolPcS.cpp \
    $${PIG_PATH}/src/protocols/standart/protocolS.cpp \
    $${PIG_PATH}/src/security/infosecurity.cpp \
    $${PIG_PATH}/src/security/pwd.cpp \
    $${PIG_PATH}/src/security/securityevent.cpp \
    $${PIG_PATH}/src/security/user.cpp \
    main.cpp \
    mainwindow.cpp \
    qkeyboard.cpp \
    serial.cpp \
    serial/serialport.cpp \
    widget/combobox.cpp \
    widget/qledindicator.cpp \
    widget/qpush1.cpp \
    wrapper.cpp

HEADERS += \
    $${PIG_PATH}/src/avr.h \
    $${PIG_PATH}/src/debug/debug.hpp \
    $${PIG_PATH}/src/drivers/kbdLayout.h \
    $${PIG_PATH}/src/drivers/keyboard.h \
    $${PIG_PATH}/src/drivers/ks0108.h \
    $${PIG_PATH}/src/drivers/tmp75.h \
    $${PIG_PATH}/src/drivers/uart.h \
    $${PIG_PATH}/src/fifo.h \
    $${PIG_PATH}/src/flash.h \
    $${PIG_PATH}/src/flashParams.h \
    $${PIG_PATH}/src/glbDefine.h \
    $${PIG_PATH}/src/menu/base.hpp \
    $${PIG_PATH}/src/menu/dateTime.hpp \
    $${PIG_PATH}/src/menu/deviceStatus.hpp \
    $${PIG_PATH}/src/menu/enterParam.h \
    $${PIG_PATH}/src/menu/journalEntry.hpp \
    $${PIG_PATH}/src/menu/jrnScada.hpp \
    $${PIG_PATH}/src/menu/menu.h \
    $${PIG_PATH}/src/menu/menuPunkt.h \
    $${PIG_PATH}/src/menu/txCom.hpp \
    $${PIG_PATH}/src/paramBsp.h \
    $${PIG_PATH}/src/paramDef.h \
    $${PIG_PATH}/src/paramGlb.h \
    $${PIG_PATH}/src/paramPrd.h \
    $${PIG_PATH}/src/paramPrm.h \
    $${PIG_PATH}/src/paramUart.h \
    $${PIG_PATH}/src/parameter/LocalParams.h \
    $${PIG_PATH}/src/parameter/param.h \
    $${PIG_PATH}/src/protocols/iec101/CIec101.h \
    $${PIG_PATH}/src/protocols/iec101/protocolPcI.h \
    $${PIG_PATH}/src/protocols/modbus/protocolModbus.h \
    $${PIG_PATH}/src/protocols/modbus/protocolPcM.h \
    $${PIG_PATH}/src/protocols/standart/protocolBspS.h \
    $${PIG_PATH}/src/protocols/standart/protocolPcS.h \
    $${PIG_PATH}/src/protocols/standart/protocolS.h \
    $${PIG_PATH}/src/security/infosecurity.h \
    $${PIG_PATH}/src/security/pwd.h \
    $${PIG_PATH}/src/security/securityevent.h \
    $${PIG_PATH}/src/security/user.h \
    mainwindow.h \
    qkeyboard.h \
    serial.h \
    serial/serialport.h \
    widget/combobox.h \
    widget/qledindicator.h \
    widget/qpush1.h \
    wrapper.hpp

FORMS += \
    mainwindow.ui \
    qkeyboard.ui \
    serial.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
