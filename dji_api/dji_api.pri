INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/qtosdk.hpp

SOURCES += \
    $$PWD/qtosdk.cpp

QT += serialport

#---------------------------------------------
#
# Project management
#
#---------------------------------------------

DJILIB += ONBOARDSDK \
        DJILIBCORE

DEPENDENCE += QT \
            DJILIB \
            DEPENDENCE

DEFINES += $$DEPENDENCE

message("DJILIB:"$$DJILIB)

contains(DJILIB, ONBOARDSDK) {
QT += serialport

ONBOARDSDK_SRC += \
    $$PWD/api/src/*.cpp \
    $$PWD/hal/src/*.cpp \
    $$PWD/platform/default/src/*.cpp \
    $$PWD/platform/qt/src/*.cpp \
    $$PWD/protocol/src/*.cpp \
    $$PWD/utility/src/*.cpp

ONBOARDSDK_INC += \
    $$PWD/api/inc/*.hpp \
    $$PWD/hal/inc/*.hpp \
    $$PWD/platform/default/inc/*.hpp \
    $$PWD/platform/qt/inc/*.hpp \
    $$PWD/protocol/inc/*.hpp \
    $$PWD/utility/inc/*.hpp

SOURCES += $$ONBOARDSDK_SRC
HEADERS += $$ONBOARDSDK_INC
}

INCLUDEPATH += \
    $$PWD/api/inc \
    $$PWD/hal/inc \
    $$PWD/platform/default/inc \
    $$PWD/platform/qt/inc \
    $$PWD/protocol/inc \
    $$PWD/utility/inc

message("finish compile")

RESOURCES += \
    $$PWD/dji_resources.qrc
