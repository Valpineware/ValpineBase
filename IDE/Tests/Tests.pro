QT += testlib
QT += core
TARGET = Terrain3DTests
CONFIG   += console \
            c++14
CONFIG   -= app_bundle

QMAKE_CFLAGS_WARN_ON += -Wno-unknown-pragmas
QMAKE_CXXFLAGS_WARN_ON += -Wno-unknown-pragmas

TEMPLATE = app

DESTDIR += ../../Deployment/Bin/

INCLUDEPATH += ../../Src/ \
                ../../Ext/ \
                ../../Ext/gmock/gtest/include/

unix|win32: LIBS += -L$$OUT_PWD/../../Deployment/Bin/ -lgtest
unix|win32: LIBS += -L$$OUT_PWD/../../Deployment/Bin/ -lValpineBase

SOURCES += \
    ../../Src/Tests/Main.cpp \
    ../../Src/Tests/Test_Property.cpp
