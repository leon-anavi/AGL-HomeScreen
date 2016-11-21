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

TEMPLATE = app
TARGET = HomeScreenSimulator
QT = widgets dbus

include(../interfaces/interfaces.pri)

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS  += \
    src/mainwindow.h

FORMS    += \
    resources/mainwindow.ui

OTHER_FILES += \
    README.md
