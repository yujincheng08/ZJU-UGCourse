include(../../MiniSQL.pri)

TARGET = RecordManager

DESTDIR = ../../

HEADERS += \
    RecordManager.h \
    record.h

SOURCES += \
    RecordManager.cpp \
    record.cpp

OTHER_FILES += \
    README.md

CONFIG(test) {
    DEFINES += TEST
    TEMPLATE = lib
    SOURCES += \
        test.cpp
}
else{
    TEMPLATE = app
}
