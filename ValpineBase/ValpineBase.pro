TARGET = ValpineBase
TEMPLATE = lib

QT += core

CONFIG += staticlib
CONFIG += c++14 c++11

#CONFIG += object_parallel_to_source
#NOTE: I decided the best way to get around object files with the same name
#is to make sub projects when collisions occur.
# http://stackoverflow.com/questions/7765147/how-to-use-qmake-with-two-source-files-which-have-the-same-name

INCLUDEPATH += ../../Ext/ \
                ../../Ext/gmock/gtest/include

HEADERS += \
    Test/Test.h \
    Helper.h \
    Loadable.h \
    MutexTryLocker.h \
    Property.h \
    System.h

SOURCES += \
    Helper.cpp \
    System.cpp

