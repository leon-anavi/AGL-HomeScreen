TEMPLATE = lib
TARGET = homescreen
CONFIG -= qt

SOURCES += \
        src/libhomescreen.cpp

HEADERS += \
        include/libhomescreen.hpp

CONFIG += link_pkgconfig
PKGCONFIG += glib-2.0 gio-unix-2.0

XMLSOURCES = \
    ../interfaces/homescreen.xml

gen_proxy_skeleton_h.input = XMLSOURCES
gen_proxy_skeleton_h.commands = \
    gdbus-codegen --generate-c-code ${QMAKE_FILE_IN_BASE} --c-namespace LibHomeScreen --interface-prefix org.agl. ${QMAKE_FILE_IN}
gen_proxy_skeleton_h.output = ${QMAKE_FILE_IN_BASE}.h
gen_proxy_skeleton_h.variable_out = HEADERS
gen_proxy_skeleton_h.clean = ${QMAKE_FILE_IN_BASE}.h

gen_proxy_skeleton_c.input = XMLSOURCES
gen_proxy_skeleton_c.commands = \
    gdbus-codegen --generate-c-code ${QMAKE_FILE_IN_BASE} --c-namespace LibHomeScreen --interface-prefix org.agl. ${QMAKE_FILE_IN}
gen_proxy_skeleton_c.output = ${QMAKE_FILE_IN_BASE}.c
gen_proxy_skeleton_c.variable_out = SOURCES
gen_proxy_skeleton_c.clean = ${QMAKE_FILE_IN_BASE}.c

QMAKE_EXTRA_COMPILERS += gen_proxy_skeleton_h gen_proxy_skeleton_c


gen_docbook.target = doc
gen_docbook.commands = \
    gdbus-codegen --generate-docbook doc $$PWD/../interfaces/homescreen.xml

QMAKE_EXTRA_TARGETS += gen_docbook
PRE_TARGETDEPS += doc

#    ../interfaces/appframework.xml \
#    ../interfaces/daynightmode.xml \
#    ../interfaces/homescreen.xml \
#    ../interfaces/inputevent.xml \
#    ../interfaces/popup.xml \
#    ../interfaces/proximity.xml \
#    ../interfaces/statusbar.xml \
#    ../interfaces/windowmanager.xml
