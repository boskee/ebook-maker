#-------------------------------------------------
#
# Project created by QtCreator 2014-04-04T22:41:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = eBookMaker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    RectItem.cpp \
    PageItem.cpp \
    arrowheadgraphic.cpp \
    cornergrabber.cpp \
    PageView.cpp

HEADERS  += mainwindow.h \
    RectItem.h \
    PageItem.h \
    arrowheadgraphic.h \
    cornergrabber.h \
    PageView.h

FORMS    += mainwindow.ui

INCLUDEPATH  += /usr/include/poppler/qt5

LIBS     += -L/usr/lib -lpoppler-qt5
