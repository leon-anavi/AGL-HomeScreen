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
    src/inputdevicesimulator.cpp \
    src/systemsettingssimulator.cpp \
    $$OUT_PWD/daynightmode_adapter.cpp \        #generated dbus adapter
    $$OUT_PWD/daynightmode_proxy.cpp \             #generated dbus proxy
    $$OUT_PWD/popup_adapter.cpp \        #generated dbus adapter
    $$OUT_PWD/popup_proxy.cpp \             #generated dbus proxy
    $$OUT_PWD/inputevent_adapter.cpp \        #generated dbus adapter
    $$OUT_PWD/inputevent_proxy.cpp \             #generated dbus proxy
    $$OUT_PWD/statusbar_adapter.cpp \        #generated dbus adapter
    $$OUT_PWD/statusbar_proxy.cpp \             #generated dbus proxy
    src/settingswidget.cpp \
    src/popupwidget.cpp \
    src/inputeventdistributor.cpp \
    src/controlbarwidget.cpp \
    src/statusbarwidget.cpp

HEADERS  += \
    include/daynightmode.h \
    include/inputevent.h \
    include/popup.h \
    src/mainwindow.h \
    src/inputdevicesimulator.h \
    src/systemsettingssimulator.h \
    $$OUT_PWD/daynightmode_adapter.h \      #generated dbus adapter
    $$OUT_PWD/daynightmode_proxy.h \           #generated dbus proxy
    $$OUT_PWD/popup_adapter.h \      #generated dbus adapter
    $$OUT_PWD/popup_proxy.h \           #generated dbus proxy
    $$OUT_PWD/inputevent_adapter.h \      #generated dbus adapter
    $$OUT_PWD/inputevent_proxy.h \           #generated dbus proxy
    $$OUT_PWD/statusbar_adapter.h \      #generated dbus adapter
    $$OUT_PWD/statusbar_proxy.h \           #generated dbus proxy
    src/settingswidget.h \
    src/popupwidget.h \
    src/inputeventdistributor.h \
    src/controlbarwidget.h \
    src/statusbarwidget.h

FORMS    += \
    resources/mainwindow.ui \
    resources/inputdevicesimulator.ui \
    resources/systemsettingssimulator.ui \
    resources/settingswidget.ui \
    resources/popupwidget.ui \
    resources/controlbarwidget.ui \
    resources/statusbarwidget.ui

TRANSLATIONS = \
    resources/translations/homescreen_en_US.ts \
    resources/translations/homescreen_de_DE.ts \
    resources/translations/homescreen_ja_JP.ts

OTHER_FILES += \
    dbus-introspections/daynightmode.xml \
    dbus-introspections/inputevent.xml \
    dbus-introspections/popup.xml \
    dbus-introspections/statusbar.xml

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
    $$OUT_PWD/inputevent_adapter.h \
    $$OUT_PWD/inputevent_adapter.cpp \
    $$OUT_PWD/inputevent_adapter.moc \
    $$OUT_PWD/inputevent_proxy.h \
    $$OUT_PWD/inputevent_proxy.cpp \
    $$OUT_PWD/inputevent_proxy.moc \
    $$OUT_PWD/statusbar_adapter.h \
    $$OUT_PWD/statusbar_adapter.cpp \
    $$OUT_PWD/statusbar_adapter.moc \
    $$OUT_PWD/statusbar_proxy.h \
    $$OUT_PWD/statusbar_proxy.cpp \
    $$OUT_PWD/statusbar_proxy.moc

# Create custom targets for generating dbus proxy and adapter
# for daynightmode
daynightmodegenerateadapter.target = $$OUT_PWD/daynightmode_adapter.cpp
unix:daynightmodegenerateadapter.commands = "qdbusxml2cpp -m -a daynightmode_adapter $$_PRO_FILE_PWD_/dbus-introspections/daynightmode.xml; moc $$OUT_PWD/daynightmode_adapter.h -o $$OUT_PWD/daynightmode_adapter.moc"
daynightmodegenerateproxy.target = $$OUT_PWD/daynightmode_proxy.cpp
unix:daynightmodegenerateproxy.commands = "qdbusxml2cpp -m -p daynightmode_proxy $$_PRO_FILE_PWD_/dbus-introspections/daynightmode.xml; moc $$OUT_PWD/daynightmode_proxy.h -o $$OUT_PWD/daynightmode_proxy.moc"
PRE_TARGETDEPS += \
    $$OUT_PWD/daynightmode_adapter.cpp \
    $$OUT_PWD/daynightmode_proxy.cpp
QMAKE_EXTRA_TARGETS += \
    daynightmodegenerateadapter \
    daynightmodegenerateproxy
# for popup
popupgenerateadapter.target = $$OUT_PWD/popup_adapter.cpp
unix:popupgenerateadapter.commands = "qdbusxml2cpp -m -a popup_adapter $$_PRO_FILE_PWD_/dbus-introspections/popup.xml; moc $$OUT_PWD/popup_adapter.h -o $$OUT_PWD/popup_adapter.moc"
popupgenerateproxy.target = $$OUT_PWD/popup_proxy.cpp
unix:popupgenerateproxy.commands = "qdbusxml2cpp -m -p popup_proxy $$_PRO_FILE_PWD_/dbus-introspections/popup.xml; moc $$OUT_PWD/popup_proxy.h -o $$OUT_PWD/popup_proxy.moc"
PRE_TARGETDEPS += \
    $$OUT_PWD/popup_adapter.cpp \
    $$OUT_PWD/popup_proxy.cpp
QMAKE_EXTRA_TARGETS += \
    popupgenerateadapter \
    popupgenerateproxy
# for inputevent
inputeventgenerateadapter.target = $$OUT_PWD/inputevent_adapter.cpp
unix:inputeventgenerateadapter.commands = "qdbusxml2cpp -m -a inputevent_adapter $$_PRO_FILE_PWD_/dbus-introspections/inputevent.xml; moc $$OUT_PWD/inputevent_adapter.h -o $$OUT_PWD/inputevent_adapter.moc"
inputeventgenerateproxy.target = $$OUT_PWD/inputevent_proxy.cpp
unix:inputeventgenerateproxy.commands = "qdbusxml2cpp -m -p inputevent_proxy $$_PRO_FILE_PWD_/dbus-introspections/inputevent.xml; moc $$OUT_PWD/inputevent_proxy.h -o $$OUT_PWD/inputevent_proxy.moc"
PRE_TARGETDEPS += \
    $$OUT_PWD/inputevent_adapter.cpp \
    $$OUT_PWD/inputevent_proxy.cpp
QMAKE_EXTRA_TARGETS += \
    inputeventgenerateadapter \
    inputeventgenerateproxy
# for statusbar
statusbargenerateadapter.target = $$OUT_PWD/statusbar_adapter.cpp
unix:statusbargenerateadapter.commands = "qdbusxml2cpp -m -a statusbar_adapter $$_PRO_FILE_PWD_/dbus-introspections/statusbar.xml; moc $$OUT_PWD/statusbar_adapter.h -o $$OUT_PWD/statusbar_adapter.moc"
statusbargenerateproxy.target = $$OUT_PWD/statusbar_proxy.cpp
unix:statusbargenerateproxy.commands = "qdbusxml2cpp -m -p statusbar_proxy $$_PRO_FILE_PWD_/dbus-introspections/statusbar.xml; moc $$OUT_PWD/statusbar_proxy.h -o $$OUT_PWD/statusbar_proxy.moc"
PRE_TARGETDEPS += \
    $$OUT_PWD/statusbar_adapter.cpp \
    $$OUT_PWD/statusbar_proxy.cpp
QMAKE_EXTRA_TARGETS += \
    statusbargenerateadapter \
    statusbargenerateproxy

