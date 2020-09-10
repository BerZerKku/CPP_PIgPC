QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    PIg/src \
    PIg/src/debug \
    PIg/src/drivers \
    PIg/src/menu \
    PIg/src/parameter \
    PIg/src/protocols \

SOURCES += \
    PIg/src/menu/enterParam.cpp \
    PIg/src/menu/menu.cpp \
    PIg/src/parameter/LocalParams.cpp \
    PIg/src/protocols/iec101/CIec101.cpp \
    PIg/src/protocols/iec101/protocolPcI.cpp \
    PIg/src/protocols/modbus/protocolModbus.cpp \
    PIg/src/protocols/modbus/protocolPcM.cpp \
    PIg/src/protocols/standart/protocolBspS.cpp \
    PIg/src/protocols/standart/protocolPcS.cpp \
    PIg/src/protocols/standart/protocolS.cpp \
    main.cpp \
    mainwindow.cpp \
    qkeyboard.cpp \
    wrapper.cpp

HEADERS += \
    PIg/src/drivers/keyboard.h \
    PIg/src/drivers/ks0108.h \
    PIg/src/drivers/tmp75.h \
    PIg/src/drivers/uart.h \
    PIg/src/flash.h \
    PIg/src/flashParams.h \
    PIg/src/glbDefine.h \
    PIg/src/menu/enterParam.h \
    PIg/src/menu/menu.h \
    PIg/src/menu/menuPunkt.h \
    PIg/src/paramBsp.h \
    PIg/src/paramDef.h \
    PIg/src/paramGlb.h \
    PIg/src/paramIS.h \
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
    avr/io.h \
    avr/pgmspace.h \
    mainwindow.h \
    qkeyboard.h \
    wrapper.hpp

FORMS += \
    mainwindow.ui \
    qkeyboard.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
