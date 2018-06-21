#-------------------------------------------------
#
# Project created by QtCreator 2018-06-18T16:51:16
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Matrice600DF
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        $$PWD\view\mainview.cpp \
    #presenter/presenterinterface.cpp
    presenter/matricedfpresenter.cpp \
    model/matricedfmodel.cpp \
    model/udpchannel.cpp \
    widgets/spectrumplot.cpp \
    widgets/amplitudespectrumplot.cpp \
    widgets/phasespectrumplot.cpp \
    widgets/twochannelstrategy.cpp \
    widgets/summarystrategy.cpp \

HEADERS += \
        $$PWD\view\mainview.hpp \
    view/mainiview.hpp \
    #presenter/presenterinterface.hpp
    presenter/matricedfpresenter.hpp \
    model/matricedfmodel.hpp \
    model/udpchannel.hpp \
    utils/constset.hpp \
    widgets/spectrumplot.hpp \
    widgets/amplitudespectrumplot.hpp \
    widgets/phasespectrumplot.hpp \
    widgets/amplitudedisplaystrategy.hpp \
    widgets/twochannelstrategy.hpp \
    widgets/summarystrategy.hpp \

FORMS += \
        $$PWD\view\mainview.ui

CONFIG += \
    console \
    qwt \
    qwtpolar
