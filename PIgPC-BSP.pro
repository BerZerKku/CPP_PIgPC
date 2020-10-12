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

INCLUDEPATH += \
    PIg/ \
    PIg/src \
    PIg/src/debug \
    PIg/src/parameter \
    PIg/src/menu \
    PIg/src/security

SOURCES += \
    PIg/src/flashParam.cpp \
    PIg/src/parameter/param.cpp \
    bsp.cpp \
    main.cpp \
    mainwindow.cpp \
    serialport.cpp \
    wrapper.cpp

HEADERS += \
    PIg/src/avr.h \
    PIg/src/debug/debug.hpp \
    PIg/src/flash.h \
    PIg/src/flashParams.h \
    PIg/src/glbDefine.h \
    PIg/src/menu/deviceStatus.hpp \
    PIg/src/paramIS.h \
    PIg/src/paramUart.h \
    PIg/src/parameter/param.h \
    bsp.h \
    mainwindow.h \
    serialport.h \
    wrapper.hpp

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
