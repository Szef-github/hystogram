#-------------------------------------------------
#
# Project created by QtCreator 2015-03-12T21:21:42
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = hystogram
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    src/main.cpp \
    src/hist_image.cpp \
    src/params.cpp \
    src/input_image.cpp

HEADERS += \
    src/hist_image.hpp \
    src/hist_data.hpp \
    src/luminance.hpp \
    src/params.hpp \
    src/input_image.hpp \
    src/logger.hpp
