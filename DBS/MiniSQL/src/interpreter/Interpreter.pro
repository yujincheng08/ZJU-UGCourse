TEMPLATE += lib
QT       -= gui

CONFIG += warn_off

include(../../MiniSQL.pri)

TARGET = Interpreter
TEMPLATE = lib

DESTDIR = ../../

HEADERS += \
    Action.h \
    BaseInterpreter.h \
    Column.h \
    Condition.h \
    Constraint.h \
    Interpreter.h \
    parser/parser.h \
    scanner/Scanner.h \
    scanner/Scanner.ih \
    scanner/Scannerbase.h

SOURCES += \
    BaseInterpreter.cpp \
    Interpreter.cpp \
    parser/parser.cpp \
    scanner/Scanner.cpp

OTHER_FILES += \
    scanner/Scanner.l \
    parser/parser.y \
    README.md

CONFIG(test) {
    SOURCES += test.cpp
}
