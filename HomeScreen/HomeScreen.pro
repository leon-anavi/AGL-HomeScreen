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
TARGET = HomeScreen
QT = qml quick dbus
CONFIG += c++11

include(../interfaces/interfaces.pri)
include(../ivi_layermanagement_api.pri)

SOURCES += \
    src/main.cpp \
    src/homescreencontrolinterface.cpp \
    src/layouthandler.cpp \
    src/applicationmodel.cpp \
    src/appinfo.cpp \
    src/statusbarmodel.cpp \
    src/statusbarserver.cpp \
    src/applicationlauncher.cpp

HEADERS  += \
    src/homescreencontrolinterface.h \
    src/layouthandler.h \
    src/statusbarmodel.h \
    src/statusbarserver.h \
    src/applicationlauncher.h \
    src/applicationmodel.h \
    src/appinfo.h

OTHER_FILES += \
    README.md

RESOURCES += \
    qml/images/Home/home.qrc \
    qml/images/MediaPlayer/mediaplayer.qrc \
    qml/images/MediaMusic/mediamusic.qrc \
    qml/images/Weather/weather.qrc \
    qml/images/Shortcut/shortcut.qrc \
    qml/images/Status/status.qrc \
    qml/images/images.qrc \
    qml/qml.qrc
