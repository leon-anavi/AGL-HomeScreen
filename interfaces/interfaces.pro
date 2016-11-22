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

TEMPLATE = lib
TARGET = interfaces
QT = dbus
CONFIG += staticlib

HEADERS += \
    include/appframework.hpp \
    include/windowmanager.hpp

SOURCES += src/appframework.cpp \
    src/windowmanager.cpp

XMLSOURCES = \
    appframework.xml \
    afm_user_daemon.xml \
    daynightmode.xml \
    homescreen.xml \
    inputevent.xml \
    popup.xml \
    proximity.xml \
    statusbar.xml \
    windowmanager.xml

gen_adaptor_cpp.input = XMLSOURCES
gen_adaptor_cpp.commands = \
    qdbusxml2cpp -i include/${QMAKE_FILE_IN_BASE}.hpp -m -a ${QMAKE_FILE_IN_BASE}_adaptor ${QMAKE_FILE_IN}; \
    moc $$OUT_PWD/${QMAKE_FILE_IN_BASE}_adaptor.h -o $$OUT_PWD/${QMAKE_FILE_IN_BASE}_adaptor.moc
gen_adaptor_cpp.output = ${QMAKE_FILE_IN_BASE}_adaptor.cpp
gen_adaptor_cpp.variable_out = SOURCES
gen_adaptor_cpp.clean = ${QMAKE_FILE_IN_BASE}_adaptor.cpp

gen_proxy_cpp.input = XMLSOURCES
gen_proxy_cpp.commands = \
    qdbusxml2cpp -i include/${QMAKE_FILE_IN_BASE}.hpp -m -p ${QMAKE_FILE_IN_BASE}_proxy ${QMAKE_FILE_IN}; \
    moc $$OUT_PWD/${QMAKE_FILE_IN_BASE}_proxy.h -o $$OUT_PWD/${QMAKE_FILE_IN_BASE}_proxy.moc
gen_proxy_cpp.output = ${QMAKE_FILE_IN_BASE}_proxy.cpp
gen_proxy_cpp.variable_out = SOURCES
gen_proxy_cpp.clean = ${QMAKE_FILE_IN_BASE}_proxy.cpp

gen_adaptor_h.input = XMLSOURCES
gen_adaptor_h.commands = @echo Fake making the header for ${QMAKE_FILE_IN}
gen_adaptor_h.depends = ${QMAKE_FILE_IN_BASE}_adaptor.cpp
gen_adaptor_h.output = ${QMAKE_FILE_IN_BASE}_adaptor.h
gen_adaptor_h.clean = ${QMAKE_FILE_IN_BASE}_adaptor.h

gen_proxy_h.input = XMLSOURCES
gen_proxy_h.commands = @echo Fake making the header for ${QMAKE_FILE_IN}
gen_proxy_h.depends = ${QMAKE_FILE_IN_BASE}_proxy.cpp
gen_proxy_h.output = ${QMAKE_FILE_IN_BASE}_proxy.h
gen_proxy_h.clean = ${QMAKE_FILE_IN_BASE}_proxy.h

QMAKE_EXTRA_COMPILERS += gen_adaptor_cpp gen_proxy_cpp gen_adaptor_h gen_proxy_h
