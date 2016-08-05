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

TARGET = HomeScreenSimulator
TEMPLATE = app


SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS  += \
    ../interfaces/daynightmode.h \
    ../interfaces/popup.h \
    src/mainwindow.h

INCLUDEPATH += $$OUT_PWD/../interfaces
INCLUDEPATH += ../interfaces

LIBS += -L$$OUT_PWD/../interfaces -linterfaces

FORMS    += \
    resources/mainwindow.ui

OTHER_FILES += \
    README.md

RESOURCES +=

# remove generated files
QMAKE_CLEAN += -r \
    $$OUT_PWD/HomeScreenSimulator \
    $$OUT_PWD/Makefile
