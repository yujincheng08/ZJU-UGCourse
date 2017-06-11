TEMPLATE += lib
QT       -= gui

include(../../MiniSQL.pri)

TARGET = BufferManager
TEMPLATE = lib

DESTDIR = ../../

HEADERS += \
    IndexManager.h

SOURCES += \
    IndexManager.cpp

OTHER_FILES += \
    README.md

CONFIG(test) {
}
