include(../../MiniSQL.pri)

TARGET = BufferManager

DESTDIR = ../../

HEADERS += \
    Buffer.h \
    BufferManager.h \
    File.h \
    WriteThread.h \
    ReadThread.h \
    BufferList.h \
    BufferListItem.h \
    FixString.h

SOURCES += \
    Buffer.cpp \
    BufferManager.cpp \
    File.cpp \
    WriteThread.cpp \
    ReadThread.cpp \
    BufferListItem.cpp

OTHER_FILES += \
    README.md
TEMPLATE = lib
#CONFIG(test) {
#TEMPLATE = app
#SOURCES += \
#    test.cpp
#}
#else{
#TEMPLATE = lib
#}
