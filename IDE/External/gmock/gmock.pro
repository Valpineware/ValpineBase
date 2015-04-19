QT       -= core gui

TARGET = gmock
TEMPLATE = lib

CONFIG += staticlib
CONFIG += c++14

QMAKE_CFLAGS_WARN_ON += -w
QMAKE_CXXFLAGS_WARN_ON += -w

INCLUDEPATH += ../../../Ext/gmock/include/ \
                ../../../Ext/gmock/gtest/include/ \
               ../../../Ext/gmock/

DESTDIR += ../../../Deployment/Bin/

SOURCES += ../../../Ext/gmock/src/gmock-all.cc

unix {
    target.path = /usr/lib
    INSTALLS += target
}
