TEMPLATE += lib
QT       -= gui

TARGET = BufferManager
TEMPLATE = lib

DESTDIR = ../../

HEADERS += \
    API.h

SOURCES += \
    API.cpp

OTHER_FILES += \
    README.md

CONFIG(test) {
}
