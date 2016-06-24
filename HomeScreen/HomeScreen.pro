# Copyright (C) 2016 Mentor Graphics Development (Deutschland) GmbH
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#TODO: compile switches for ARM/x86 architecture

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HomeScreen
TEMPLATE = app


SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/settingswidget.cpp \
    src/popupwidget.cpp \
    src/controlbarwidget.cpp \
    src/statusbarwidget.cpp
GENERATED_SOURCES += \
    $$OUT_PWD/daynightmode_adapter.cpp \        #generated dbus adapter
    $$OUT_PWD/daynightmode_proxy.cpp \             #generated dbus proxy
    $$OUT_PWD/popup_adapter.cpp \        #generated dbus adapter
    $$OUT_PWD/popup_proxy.cpp \             #generated dbus proxy
    $$OUT_PWD/statusbar_adapter.cpp \        #generated dbus adapter
    $$OUT_PWD/statusbar_proxy.cpp             #generated dbus proxy

HEADERS  += \
    ../interfaces/daynightmode.h \
    ../interfaces/popup.h \
    src/mainwindow.h \
    src/settingswidget.h \
    src/popupwidget.h \
    src/controlbarwidget.h \
    src/statusbarwidget.h \
    $$OUT_PWD/daynightmode_adapter.h \      #generated dbus adapter
    $$OUT_PWD/daynightmode_proxy.h \           #generated dbus proxy
    $$OUT_PWD/popup_adapter.h \      #generated dbus adapter
    $$OUT_PWD/popup_proxy.h \           #generated dbus proxy
    $$OUT_PWD/statusbar_adapter.h \      #generated dbus adapter
    $$OUT_PWD/statusbar_proxy.h           #generated dbus proxy

FORMS    += \
    resources/mainwindow.ui \
    resources/settingswidget.ui \
    resources/popupwidget.ui \
    resources/controlbarwidget.ui \
    resources/statusbarwidget.ui

TRANSLATIONS = \
    resources/translations/homescreen_en_US.ts \
    resources/translations/homescreen_de_DE.ts \
    resources/translations/homescreen_ja_JP.ts

OTHER_FILES += \
    ../interfaces/daynightmode.xml \
    ../interfaces/inputevent.xml \
    ../interfaces/popup.xml \
    ../interfaces/statusbar.xml \
    README.md

RESOURCES += \
    resources/homescreen.qrc

# remove generated files
QMAKE_CLEAN += -r \
    $$OUT_PWD/HomeScreen \
    $$OUT_PWD/Makefile \
    $$OUT_PWD/daynightmode_adapter.h \
    $$OUT_PWD/daynightmode_adapter.cpp \
    $$OUT_PWD/daynightmode_adapter.moc \
    $$OUT_PWD/daynightmode_proxy.h \
    $$OUT_PWD/daynightmode_proxy.cpp \
    $$OUT_PWD/daynightmode_proxy.moc \
    $$OUT_PWD/popup_adapter.h \
    $$OUT_PWD/popup_adapter.cpp \
    $$OUT_PWD/popup_adapter.moc \
    $$OUT_PWD/popup_proxy.h \
    $$OUT_PWD/popup_proxy.cpp \
    $$OUT_PWD/popup_proxy.moc \
    $$OUT_PWD/statusbar_adapter.h \
    $$OUT_PWD/statusbar_adapter.cpp \
    $$OUT_PWD/statusbar_adapter.moc \
    $$OUT_PWD/statusbar_proxy.h \
    $$OUT_PWD/statusbar_proxy.cpp \
    $$OUT_PWD/statusbar_proxy.moc

# Create custom targets for generating dbus proxy and adapter
# OE_QMAKE_PATH_EXTERNAL_HOST_BINS is only defined in the yocto environment
isEmpty(OE_QMAKE_PATH_EXTERNAL_HOST_BINS){
  OE_QMAKE_PATH_EXTERNAL_HOST_BINS = "/"
}
# for daynightmode
daynightmodegenerateadapter.target = $$OUT_PWD/daynightmode_adapter.cpp
unix:daynightmodegenerateadapter.commands = "$(OE_QMAKE_PATH_EXTERNAL_HOST_BINS)qdbusxml2cpp -m -a daynightmode_adapter $$_PRO_FILE_PWD_/../interfaces/daynightmode.xml; $(OE_QMAKE_PATH_EXTERNAL_HOST_BINS)moc $$OUT_PWD/daynightmode_adapter.h -o $$OUT_PWD/daynightmode_adapter.moc"
daynightmodegenerateproxy.target = $$OUT_PWD/daynightmode_proxy.cpp
unix:daynightmodegenerateproxy.commands = "$(OE_QMAKE_PATH_EXTERNAL_HOST_BINS)qdbusxml2cpp -m -p daynightmode_proxy $$_PRO_FILE_PWD_/../interfaces/daynightmode.xml; $(OE_QMAKE_PATH_EXTERNAL_HOST_BINS)moc $$OUT_PWD/daynightmode_proxy.h -o $$OUT_PWD/daynightmode_proxy.moc"
PRE_TARGETDEPS += \
    $$OUT_PWD/daynightmode_adapter.cpp \
    $$OUT_PWD/daynightmode_proxy.cpp
QMAKE_EXTRA_TARGETS += \
    daynightmodegenerateadapter \
    daynightmodegenerateproxy
# for popup
popupgenerateadapter.target = $$OUT_PWD/popup_adapter.cpp
unix:popupgenerateadapter.commands = "$(OE_QMAKE_PATH_EXTERNAL_HOST_BINS)qdbusxml2cpp -m -a popup_adapter $$_PRO_FILE_PWD_/../interfaces/popup.xml; $(OE_QMAKE_PATH_EXTERNAL_HOST_BINS)moc $$OUT_PWD/popup_adapter.h -o $$OUT_PWD/popup_adapter.moc"
popupgenerateproxy.target = $$OUT_PWD/popup_proxy.cpp
unix:popupgenerateproxy.commands = "$(OE_QMAKE_PATH_EXTERNAL_HOST_BINS)qdbusxml2cpp -m -p popup_proxy $$_PRO_FILE_PWD_/../interfaces/popup.xml; $(OE_QMAKE_PATH_EXTERNAL_HOST_BINS)moc $$OUT_PWD/popup_proxy.h -o $$OUT_PWD/popup_proxy.moc"
PRE_TARGETDEPS += \
    $$OUT_PWD/popup_adapter.cpp \
    $$OUT_PWD/popup_proxy.cpp
QMAKE_EXTRA_TARGETS += \
    popupgenerateadapter \
    popupgenerateproxy
# for statusbar
statusbargenerateadapter.target = $$OUT_PWD/statusbar_adapter.cpp
unix:statusbargenerateadapter.commands = "$(OE_QMAKE_PATH_EXTERNAL_HOST_BINS)qdbusxml2cpp -m -a statusbar_adapter $$_PRO_FILE_PWD_/../interfaces/statusbar.xml; $(OE_QMAKE_PATH_EXTERNAL_HOST_BINS)moc $$OUT_PWD/statusbar_adapter.h -o $$OUT_PWD/statusbar_adapter.moc"
statusbargenerateproxy.target = $$OUT_PWD/statusbar_proxy.cpp
unix:statusbargenerateproxy.commands = "$(OE_QMAKE_PATH_EXTERNAL_HOST_BINS)qdbusxml2cpp -m -p statusbar_proxy $$_PRO_FILE_PWD_/../interfaces/statusbar.xml; $(OE_QMAKE_PATH_EXTERNAL_HOST_BINS)moc $$OUT_PWD/statusbar_proxy.h -o $$OUT_PWD/statusbar_proxy.moc"
PRE_TARGETDEPS += \
    $$OUT_PWD/statusbar_adapter.cpp \
    $$OUT_PWD/statusbar_proxy.cpp
QMAKE_EXTRA_TARGETS += \
    statusbargenerateadapter \
    statusbargenerateproxy

