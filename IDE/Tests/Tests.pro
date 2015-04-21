TEMPLATE = app
TARGET = Terrain3DTests

CONFIG   += console c++14 c++11
CONFIG   -= app_bundle
QT += testlib core

include(../Root.pri)

INCLUDEPATH += ../../Src/ \
				../../Ext/ \
				../../Ext/gmock/gtest/include/


DESTDIR += ../../Deployment/Bin/

unix|win32: LIBS += -L$$OUT_PWD/../../Deployment/Bin/ -lgtest
unix|win32: LIBS += -L$$OUT_PWD/../../Deployment/Bin/ -lgmock
unix|win32: LIBS += -L$$OUT_PWD/../../Deployment/Bin/ -lValpineBase

SOURCES += \
    ../../Src/Tests/Main.cpp \
    ../../Src/Tests/Test_Property.cpp
