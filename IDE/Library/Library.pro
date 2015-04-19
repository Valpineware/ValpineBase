TARGET = ValpineBase
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14
CONFIG += object_parallel_to_source
QT += core

QMAKE_CFLAGS_WARN_ON += -Wno-unknown-pragmas
QMAKE_CXXFLAGS_WARN_ON += -Wno-unknown-pragmas

INCLUDEPATH += ../../Src/Library \
			../../Ext/

DESTDIR += ../../Deployment/Bin/

unix {
	target.path = /usr/lib
	INSTALLS += target
}
