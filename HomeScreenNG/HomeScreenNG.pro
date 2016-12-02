TEMPLATE = app
QT = quick dbus

include(../interfaces/interfaces.pri)

HEADERS += \
    statusbarmodel.h \
    statusbarserver.h \
    applicationlauncher.h

SOURCES += main.cpp \
    statusbarmodel.cpp \
    statusbarserver.cpp \
    applicationlauncher.cpp

RESOURCES += \
    qml/images/MediaPlayer/mediaplayer.qrc \
    qml/images/MediaMusic/mediamusic.qrc \
    qml/images/Weather/weather.qrc \
    qml/images/Shortcut/shortcut.qrc \
    qml/images/Status/status.qrc \
    qml/images/images.qrc \
    qml/qml.qrc

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

target.path = /opt/$${TARGET}/bin
INSTALLS += target

