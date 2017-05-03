#-------------------------------------------------
#
# Project created by QtCreator 2013-11-23T15:06:58
#
#-------------------------------------------------

QT += core gui multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += link_pkgconfig
PKGCONFIG += opencv
QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder
TARGET = ../AutoFaceCapture
TEMPLATE = app
SOURCES += main.cpp\
        mainwindow.cpp \
    preference.cpp \
    capturelistwidget.cpp \
    suredialog.cpp \
    mycamera.cpp \
    cameraview.cpp \
    facedetector.cpp \
    thread.cpp \
    autocapture.cpp
HEADERS  += mainwindow.h \
    preference.h \
    capturelistwidget.h \
    suredialog.h \
    mycamera.h \
    cameraview.h \
    facedetector.h \
    thread.h \
    autocapture.h

TRANSLATIONS += trans/my_zh_CN.ts \
   trans/my_en_US.ts

FORMS    += mainwindow.ui \
    preference.ui \
    suredialog.ui
RC_FILE = icon.rc
RESOURCES += resource.qrc
ICON = icon.icns

DISTFILES += \
    faceRecognize.xml
