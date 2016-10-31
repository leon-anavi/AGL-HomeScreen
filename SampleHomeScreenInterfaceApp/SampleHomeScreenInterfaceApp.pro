TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/sampleclass.cpp

HEADERS += \
    src/sampleclass.hpp

INCLUDEPATH += $$PWD/../libhomescreen/include/

LIBS += -L$$OUT_PWD/../libhomescreen -lhomescreen

# remove generated files
QMAKE_CLEAN += -r \
    $$OUT_PWD/*
