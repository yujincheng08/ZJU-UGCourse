#-------------------------------------------------
#
# Project created by QtCreator 2017-01-26T15:10:12
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Assembler
TEMPLATE = app
INCLUDEPATH += RamTable
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    editor.cpp \
    linenumwidget.cpp \
    highlighter.cpp \
    RamTable/ramtable.cpp \
    RamTable/ramtabledelegate.cpp \
    RamTable/ramtablemodel.cpp \
    instructionlist.cpp \
    register.cpp \
    assembler.cpp \
    phraser.cpp

HEADERS  += mainwindow.h \
    editor.h \
    linenumwidget.h \
    highlighter.h \
    RamTable/ramtable.h \
    RamTable/ramtabledelegate.h \
    RamTable/ramtablemodel.h \
    instructionlist.h \
    register.h \
    assembler.h \
    phraser.h

RESOURCES += \
    resources.qrc
