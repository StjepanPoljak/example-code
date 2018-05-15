TEMPLATE = app
TARGET = QuadTree

QT = core gui

greaterThan (QT_MAJOR_VERSION, 4) : QT += widgets

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    quadnode.cpp \
    ahcdrawingframe.cpp

HEADERS += \
    mainwindow.h \
    quadnode.h \
    ahcdrawingframe.h
