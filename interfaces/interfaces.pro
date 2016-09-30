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

QT  += core dbus
QT  -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
CONFIG += staticlib
TARGET = interfaces

HEADERS += \
    include/appframework.hpp \
    include/windowmanager.hpp

SOURCES += src/appframework.cpp \
    src/windowmanager.cpp

XMLSOURCES = \
    appframework.xml \
    daynightmode.xml \
    homescreen.xml \
    inputevent.xml \
    popup.xml \
    statusbar.xml \
    windowmanager.xml

gen_adapter_cpp.input = XMLSOURCES
gen_adapter_cpp.commands = \
    qdbusxml2cpp -i include/${QMAKE_FILE_IN_BASE}.hpp -m -a ${QMAKE_FILE_IN_BASE}_adapter ${QMAKE_FILE_IN}; \
    moc $$OUT_PWD/${QMAKE_FILE_IN_BASE}_adapter.h -o $$OUT_PWD/${QMAKE_FILE_IN_BASE}_adapter.moc
gen_adapter_cpp.output = ${QMAKE_FILE_IN_BASE}_adapter.cpp
gen_adapter_cpp.variable_out = SOURCES
gen_adapter_cpp.clean = ${QMAKE_FILE_IN_BASE}_adapter.cpp

gen_proxy_cpp.input = XMLSOURCES
gen_proxy_cpp.commands = \
    qdbusxml2cpp -i include/${QMAKE_FILE_IN_BASE}.hpp -m -p ${QMAKE_FILE_IN_BASE}_proxy ${QMAKE_FILE_IN}; \
    moc $$OUT_PWD/${QMAKE_FILE_IN_BASE}_proxy.h -o $$OUT_PWD/${QMAKE_FILE_IN_BASE}_proxy.moc
gen_proxy_cpp.output = ${QMAKE_FILE_IN_BASE}_proxy.cpp
gen_proxy_cpp.variable_out = SOURCES
gen_proxy_cpp.clean = ${QMAKE_FILE_IN_BASE}_proxy.cpp

gen_adapter_h.input = XMLSOURCES
gen_adapter_h.commands = @echo Fake making the header for ${QMAKE_FILE_IN}
gen_adapter_h.depends = ${QMAKE_FILE_IN_BASE}_adapter.cpp
gen_adapter_h.output = ${QMAKE_FILE_IN_BASE}_adapter.h
gen_adapter_h.clean = ${QMAKE_FILE_IN_BASE}_adapter.h

gen_proxy_h.input = XMLSOURCES
gen_proxy_h.commands = @echo Fake making the header for ${QMAKE_FILE_IN}
gen_proxy_h.depends = ${QMAKE_FILE_IN_BASE}_proxy.cpp
gen_proxy_h.output = ${QMAKE_FILE_IN_BASE}_proxy.h
gen_proxy_h.clean = ${QMAKE_FILE_IN_BASE}_proxy.h

QMAKE_EXTRA_COMPILERS += gen_adapter_cpp gen_proxy_cpp gen_adapter_h gen_proxy_h

# remove generated files
QMAKE_CLEAN += -r \
    $$OUT_PWD/*
