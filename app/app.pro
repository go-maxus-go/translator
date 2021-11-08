QT += core gui network widgets

CONFIG += c++17
CONFIG += object_parallel_to_source

SOURCES += main.cpp


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

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
