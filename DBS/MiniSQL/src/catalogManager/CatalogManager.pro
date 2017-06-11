TEMPLATE += lib
QT       -= gui

include(../../MiniSQL.pri)

TARGET = BufferManager
# TEMPLATE = lib

DESTDIR = ../../

HEADERS += \
    CatalogManager.h

SOURCES += \
    CatalogManager.cpp

OTHER_FILES += \
    README.md

CONFIG(test) {
}
