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
    src/statusbarwidget.cpp \
    src/applauncherwidget.cpp \
    src/homescreencontrolinterface.cpp \
    src/layouthandler.cpp

HEADERS  += \
    src/mainwindow.h \
    src/settingswidget.h \
    src/popupwidget.h \
    src/controlbarwidget.h \
    src/statusbarwidget.h \
    src/applauncherwidget.h \
    src/homescreencontrolinterface.h \
    src/layouthandler.h

INCLUDEPATH += $$OUT_PWD/../interfaces
INCLUDEPATH += ../interfaces/

LIBS += -L$$OUT_PWD/../interfaces -linterfaces


FORMS    += \
    resources/mainwindow.ui \
    resources/settingswidget.ui \
    resources/popupwidget.ui \
    resources/controlbarwidget.ui \
    resources/statusbarwidget.ui \
    resources/applauncherwidget.ui

TRANSLATIONS = \
    resources/translations/homescreen_en_US.ts \
    resources/translations/homescreen_de_DE.ts \
    resources/translations/homescreen_ja_JP.ts

OTHER_FILES += \
    README.md

RESOURCES += \
    resources/homescreen.qrc

copydata.commands = $(COPY_DIR) $$PWD/resources/colorschemes $$OUT_PWD
HomeScreen.depends = $(HomeScreen) copydata
export(HomeScreen.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += HomeScreen copydata

# remove generated files
QMAKE_CLEAN += -r \
    $$OUT_PWD/colorschemes \
    $$OUT_PWD/HomeScreen \
    $$OUT_PWD/Makefile
