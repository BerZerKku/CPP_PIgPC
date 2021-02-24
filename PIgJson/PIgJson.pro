include(../common.pri)
include(../PIg.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

message($$COMMON_PATH)

INCLUDEPATH += \
    $$PIG_PATH/src

SOURCES += \
    $$PIG_PATH/src/flashParam.cpp \
    $$PIG_PATH/src/parameter/param.cpp \
    configJson.cpp \
    main.cpp \
    pigjson.cpp \
    tab.cpp

HEADERS += \
    $$PIG_PATH/src/flashParams.h \
    $$PIG_PATH/src/glbDefine.h \
    $$PIG_PATH/src/parameter/param.h \
    configJson.h \
    pigjson.h \
    tab.h \
    wrapper.hpp

FORMS += \
    pigjson.ui \
    tab.ui

OTHER_FILES += \
    $$PWD/config/config.json

copydata.commands = $(COPY_DIR) $$PWD/config $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
