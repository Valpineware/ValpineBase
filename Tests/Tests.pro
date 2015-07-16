TEMPLATE = app
TARGET = ValpineBaseTests

CONFIG   += console c++14 c++11
CONFIG   -= app_bundle
QT += testlib core

include(../Root.pri)

INCLUDEPATH += ../

HEADERS += \
    Main.h \
    Test_Loadable.h \
    Test_Property.h \
    Test_Property_Operators.h

SOURCES += \
    Main.cpp

# ValpineBase
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ValpineBase/release/ -lValpineBase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ValpineBase/debug/ -lValpineBase
else:unix: LIBS += -L$$OUT_PWD/../ValpineBase/ -lValpineBase

INCLUDEPATH += $$PWD/../ValpineBase
DEPENDPATH += $$PWD/../ValpineBase

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ValpineBase/release/libValpineBase.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ValpineBase/debug/libValpineBase.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ValpineBase/release/ValpineBase.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ValpineBase/debug/ValpineBase.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../ValpineBase/libValpineBase.a
