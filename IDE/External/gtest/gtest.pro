QT       -= core gui

TARGET = gtest
TEMPLATE = lib

include(../External.pri)

SOURCES += ../../../Ext/gmock/gtest/src/gtest-all.cc

unix {
    target.path = /usr/lib
    INSTALLS += target
}
