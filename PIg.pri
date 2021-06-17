PIG_PATH = $$system(pwd)/PIg

INCLUDEPATH += \
    $$system(pwd) \
	$${PIG_PATH}

HEADERS += \
    $${PIG_PATH}/src/avr.h \
    $${PIG_PATH}/src/debug/debug.hpp \
#    $${PIG_PATH}/src/drivers/kbdLayout.h \
    $${PIG_PATH}/src/drivers/keyboard.h \
    $${PIG_PATH}/src/drivers/ks0108.h \
    $${PIG_PATH}/src/drivers/tmp75.h \
    $${PIG_PATH}/src/drivers/uart.h \
    $${PIG_PATH}/src/flash.h \
    $${PIG_PATH}/src/flashParams.h \
    $${PIG_PATH}/src/glbDefine.h \
#    $${PIG_PATH}/src/menu/base.hpp \
    $${PIG_PATH}/src/menu/dateTime.h \
#    $${PIG_PATH}/src/menu/deviceStatus.h \
    $${PIG_PATH}/src/menu/enterParam.h \
#    $${PIG_PATH}/src/menu/journalEntry.h \
#    $${PIG_PATH}/src/menu/jrnScada.h \
    $${PIG_PATH}/src/menu/menu.h \
    $${PIG_PATH}/src/menu/menuPunkt.h \
    $${PIG_PATH}/src/menu/test.h \
    $${PIG_PATH}/src/menu/txCom.h \
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
    $${PIG_PATH}/src/protocols/standart/protocolS.h

SOURCES += \
#    $${PIG_PATH}/src/drivers/kbdLayout.cpp \
    $${PIG_PATH}/src/flashParams.cpp \
#    $${PIG_PATH}/src/menu/base.cpp \
    $${PIG_PATH}/src/menu/dateTime.cpp \
    $${PIG_PATH}/src/menu/enterParam.cpp \
#    $${PIG_PATH}/src/menu/menu.cpp \
    $${PIG_PATH}/src/menu/test.cpp \
    $${PIG_PATH}/src/menu/txCom.cpp \
#    $${PIG_PATH}/src/paramGlb.cpp \
#    $${PIG_PATH}/src/parameter/LocalParams.cpp \
    $${PIG_PATH}/src/parameter/param.cpp \
    $${PIG_PATH}/src/protocols/iec101/CIec101.cpp \
#    $${PIG_PATH}/src/protocols/iec101/protocolPcI.cpp \
    $${PIG_PATH}/src/protocols/modbus/protocolModbus.cpp \
#    $${PIG_PATH}/src/protocols/modbus/protocolPcM.cpp \
#    $${PIG_PATH}/src/protocols/standart/protocolBspS.cpp \
#    $${PIG_PATH}/src/protocols/standart/protocolPcS.cpp \
#    $${PIG_PATH}/src/protocols/standart/protocolS.cpp
