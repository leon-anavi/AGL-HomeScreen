TEMPLATE = app
TARGET = SampleHomeScreenInterfaceApp
CONFIG -= qt

SOURCES += src/main.cpp \
    src/sampleclass.cpp

HEADERS += \
    src/sampleclass.hpp

INCLUDEPATH += $$PWD/../libhomescreen/include/

LIBS += -L$$OUT_PWD/../libhomescreen -lhomescreen
