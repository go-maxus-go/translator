TEMPLATE = app

CONFIG += staticlib c++17
CONFIG -= QT

SOURCES = $$files(*.cpp, true)

INCLUDEPATH += \
    ../3rdparty/Catch2/single_include

# src library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/release/ -lsrc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/debug/ -lsrc
else:unix: LIBS += -L$$OUT_PWD/../src/ -lsrc

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/release/libsrc.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/debug/libsrc.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/release/src.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../src/debug/src.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../src/libsrc.a
