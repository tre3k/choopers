# This part of "choopers" under license GNU GPL (c) 2020 Kirill Pshenichnyi

QT += core gui widgets printsupport

TARGET = gap
TEMPLATE = app

SOURCES += \
           src/main.cpp \
           src/mainwindow.cpp

HEADERS += \ \
    src/mainwindow.h

INCLUDEPATH += \
            src/ \
            src/external

RESOURCES += resources.qrc

