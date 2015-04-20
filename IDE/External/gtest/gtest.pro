QT       -= core gui

TARGET = gtest
TEMPLATE = lib

CONFIG += staticlib
CONFIG += c++14

QMAKE_CFLAGS_WARN_ON += -w
QMAKE_CXXFLAGS_WARN_ON += -w

INCLUDEPATH += ../../../Ext/gmock/gtest/include/ \
               ../../../Ext/gmock/gtest/

DESTDIR += ../../../Deployment/Bin/

SOURCES += ../../../Ext/gmock/gtest/src/gtest-all.cc

unix {
    target.path = /usr/lib
    INSTALLS += target
}
