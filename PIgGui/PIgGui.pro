include(../PIg.pri)

QT       += core gui serialport testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

CONFIG(debug, debug|release) {
    message(===Debug===)
}

CONFIG(release, debug|release) {
    message(===Release===)

    CONFIG -= console

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

VPATH

INCLUDEPATH += \
    $$system(pwd)

HEADERS += \
    mainwindow.h \
    qkeyboard.h \
    qprotocolviewer.h \
    serial.h \
    bsp/bsp.h \
    wrapper.h \
    serial/serialport.h \
    widget/combobox.h \
    widget/qledindicator.h \
    widget/qpush1.h \
    widget/qhexspinbox.h


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qkeyboard.cpp \
    qprotocolviewer.cpp \
    wrapperGui.cpp \
    bsp/bsp.cpp \
    serial.cpp \
    serial/serialport.cpp \
    widget/combobox.cpp \
    widget/qledindicator.cpp \
    widget/qpush1.cpp


FORMS += \
    mainwindow.ui \
    qkeyboard.ui \
    serial.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
