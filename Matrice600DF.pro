#-------------------------------------------------
#
# Project created by QtCreator 2018-04-24T14:24:51
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
    View/matricedfview.cpp \
    Utils/spectrumwaterfall.cpp \
    Utils/spectrumplot.cpp \
    Utils/amplitudespectrumplot.cpp \
    Utils/phasespectrumplot.cpp \
    Utils/polarplot.cpp \
    Controller/matricedfcontroller.cpp \
    Model/matricedfmodel.cpp \
    Utils/udpchannel.cpp \
    #Controller/controllerinterface.cpp \
    #Model/modelinterface.cpp

HEADERS += \
    View/matricedfview.hpp \
    Utils/spectrumwaterfall.hpp \
    Utils/spectrumplot.hpp \
    Utils/amplitudespectrumplot.hpp \
    Utils/phasespectrumplot.hpp \
    Utils/polarplot.hpp \
    Controller/matricedfcontroller.hpp \
    Model/matricedfmodel.hpp \
    Utils/udpchannel.hpp \
    Controller/controllerinterface.hpp \
    Model/modelinterface.hpp \
    Utils/constset.hpp

FORMS += \
    View/matricedfview.ui

CONFIG += \
    qwt \
    qwtpolar
