TARGET = ValpineBase
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14
CONFIG += object_parallel_to_source
QT += core

INCLUDEPATH += ../../Src/Library \
               ../../Ext/ \
                ../../Ext/gmock/gtest/include

DESTDIR += ../../Deployment/Bin/

unix {
	target.path = /usr/lib
	INSTALLS += target
}

HEADERS += \
    ../../Src/ValpineBase/Test/Test.h \
    ../../Src/ValpineBase/Test/Assert.h \
    ../../Src/ValpineBase/Property.h
