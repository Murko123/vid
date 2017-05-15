#-------------------------------------------------
#
# Project created by QtCreator 2017-03-12T23:37:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vid2
TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
LIBS+= -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_videoio -lopencv_highgui -lopencv_imgproc


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
