TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    calledbyqml.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

INCLUDEPATH += $$PWD/../libhomescreen/include/

LIBS += -L$$OUT_PWD/../libhomescreen -lhomescreen

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    calledbyqml.h
