# This part of "choopers" under license GNU GPL (c) 2020 Kirill Pshenichnyi

QT += core gui widgets printsupport

QTPLUGIN += qsvg

TARGET = choopers
TEMPLATE = app

SOURCES += \
            src/interactiveplot.cpp \
            src/main.cpp \
            src/mainwindow.cpp \
            src/widgets.cpp \
            src/external/qcustomplot.cpp \
            src/calculate.cpp \
            src/dialogs.cpp

HEADERS += \
            src/interactiveplot.h \
            src/mainwindow.h \
            src/widgets.h \
            src/external/qcustomplot.h \
            src/calculate.h \
            src/dialogs.h

INCLUDEPATH += \
            src/ \
            src/external

RESOURCES += resources.qrc

