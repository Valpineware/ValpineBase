TEMPLATE = app
TARGET = Terrain3DTests

CONFIG   += console c++14 c++11
CONFIG   -= app_bundle
QT += testlib core

include(../Root.pri)

INCLUDEPATH += ../../Src/ \
                ../../Ext/ \


unix {
    SOURCES += $$system("find ../../Src/Tests/ -name '*.cpp'")
    HEADERS += $$system("find ../../Src/Tests/ -name '*.h'")
}
win32 {
	SOURCES += $$system("dir ..\..\Src\Tests\*.cpp /b /s")
	HEADERS += $$system("dir ..\..\Src\Tests\*.h /b /s")
}



Release: CONFIG_DIR = release
Debug: CONFIG_DIR = debug

LIBS += -L../ValpineBase/$$CONFIG_DIR/ -lValpineBase

PRE_TARGETDEPS += ../ValpineBase/$$CONFIG_DIR/libValpineBase.a
