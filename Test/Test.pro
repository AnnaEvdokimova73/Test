QT += quick core

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        MainHandler.cpp \
        TextHandler.cpp \
        main.cpp

HEADERS += \
        MainHandler.h \
        TextHandler.h

RESOURCES += qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
