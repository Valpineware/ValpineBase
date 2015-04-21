TEMPLATE = app
CONFIG   += console c++14
CONFIG   -= app_bundl
QT += testlib core

include(../Root.pri)

INCLUDEPATH += ../../Src/ \
				../../Ext/ \
				../../Ext/gmock/gtest/include/

TARGET = Terrain3DTests
DESTDIR += ../../Deployment/Bin/

unix|win32: LIBS += -L$$OUT_PWD/../../Deployment/Bin/ -lgtest
unix|win32: LIBS += -L$$OUT_PWD/../../Deployment/Bin/ -lValpineBase

SOURCES += \
    ../../Src/Tests/Main.cpp \
    ../../Src/Tests/Test_Property.cpp
