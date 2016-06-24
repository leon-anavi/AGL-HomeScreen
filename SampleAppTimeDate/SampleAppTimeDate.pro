#-------------------------------------------------
#
# Project created by QtCreator 2016-06-21T13:46:39
#
#-------------------------------------------------

QT       += core dbus
QT       -= gui

TARGET = SampleAppTimeDate
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += src/main.cpp \
    src/timedateprovider.cpp
GENERATED_SOURCES += \
    $$OUT_PWD/statusbar_proxy.cpp             #generated dbus proxy

HEADERS += \
    $$OUT_PWD/statusbar_proxy.h \           #generated dbus proxy
    src/timedateprovider.h

OTHER_FILES += \
    ../interfaces/statusbar.xml \
    README.md

# remove generated files
QMAKE_CLEAN += -r \
    $$OUT_PWD/HomeScreen \
    $$OUT_PWD/Makefile \
    $$OUT_PWD/statusbar_proxy.h \
    $$OUT_PWD/statusbar_proxy.cpp \
    $$OUT_PWD/statusbar_proxy.moc

# Create custom targets for generating dbus proxy
# for statusbar
statusbargenerateproxy.target = $$OUT_PWD/statusbar_proxy.cpp
unix:statusbargenerateproxy.commands = "$(OE_QMAKE_PATH_EXTERNAL_HOST_BINS)qdbusxml2cpp -m -p statusbar_proxy $$_PRO_FILE_PWD_/../interfaces/statusbar.xml; $(OE_QMAKE_PATH_EXTERNAL_HOST_BINS)moc $$OUT_PWD/statusbar_proxy.h -o $$OUT_PWD/statusbar_proxy.moc"
PRE_TARGETDEPS += \
    $$OUT_PWD/statusbar_proxy.cpp
QMAKE_EXTRA_TARGETS += \
    statusbargenerateproxy
