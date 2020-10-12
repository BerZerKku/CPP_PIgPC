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
    MY_TESTS \
    QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    PIg/ \
    PIg/src \
    PIg/src/debug \
    PIg/src/drivers \
    PIg/src/menu \
    PIg/src/parameter \
    PIg/src/protocols \
    PIg/src/security

SOURCES += \
    PIg/src/flashParam.cpp \
    PIg/src/menu/base.cpp \
    PIg/src/menu/dateTime.cpp \
    PIg/src/menu/enterParam.cpp \
    PIg/src/menu/menu.cpp \
    PIg/src/menu/txCom.cpp \
    PIg/src/paramGlb.cpp \
    PIg/src/parameter/LocalParams.cpp \
    PIg/src/parameter/param.cpp \
    PIg/src/protocols/iec101/CIec101.cpp \
    PIg/src/protocols/iec101/protocolPcI.cpp \
    PIg/src/protocols/modbus/protocolModbus.cpp \
    PIg/src/protocols/modbus/protocolPcM.cpp \
    PIg/src/protocols/standart/protocolBspS.cpp \
    PIg/src/protocols/standart/protocolPcS.cpp \
    PIg/src/protocols/standart/protocolS.cpp \
    PIg/src/security/infosecurity.cpp \
    PIg/src/security/isevent.cpp \
    PIg/src/security/pwd.cpp \
    PIg/src/security/user.cpp \
    main.cpp \
    mainwindow.cpp \
    qkeyboard.cpp \
    serialport.cpp \
    wrapper.cpp

HEADERS += \
    PIg/src/avr.h \
    PIg/src/debug/debug.hpp \
    PIg/src/drivers/keyboard.h \
    PIg/src/drivers/ks0108.h \
    PIg/src/drivers/tmp75.h \
    PIg/src/drivers/uart.h \
    PIg/src/flash.h \
    PIg/src/flashParams.h \
    PIg/src/glbDefine.h \
    PIg/src/menu/base.hpp \
    PIg/src/menu/dateTime.hpp \
    PIg/src/menu/deviceStatus.hpp \
    PIg/src/menu/enterParam.h \
    PIg/src/menu/journalEntry.hpp \
    PIg/src/menu/jrnScada.hpp \
    PIg/src/menu/menu.h \
    PIg/src/menu/menuPunkt.h \
    PIg/src/menu/txCom.hpp \
    PIg/src/paramBsp.h \
    PIg/src/paramDef.h \
    PIg/src/paramGlb.h \
    PIg/src/paramPrd.h \
    PIg/src/paramPrm.h \
    PIg/src/paramUart.h \
    PIg/src/parameter/LocalParams.h \
    PIg/src/parameter/param.h \
    PIg/src/protocols/iec101/CIec101.h \
    PIg/src/protocols/iec101/protocolPcI.h \
    PIg/src/protocols/modbus/protocolModbus.h \
    PIg/src/protocols/modbus/protocolPcM.h \
    PIg/src/protocols/standart/protocolBspS.h \
    PIg/src/protocols/standart/protocolPcS.h \
    PIg/src/protocols/standart/protocolS.h \
    PIg/src/security/infosecurity.h \
    PIg/src/security/isevent.h \
    PIg/src/security/pwd.h \
    PIg/src/security/user.h \
    mainwindow.h \
    qkeyboard.h \
    serialport.h \
    wrapper.hpp

FORMS += \
    mainwindow.ui \
    qkeyboard.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
