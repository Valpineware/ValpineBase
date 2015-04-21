QT       -= core gui

TARGET = gmock
TEMPLATE = lib

include(../External.pri)

SOURCES += ../../../Ext/gmock/src/gmock-all.cc

unix {
    target.path = /usr/lib
    INSTALLS += target
}
