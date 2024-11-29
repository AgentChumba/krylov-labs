QT += core gui widgets

CONFIG += c++17

TEMPLATE = app
TARGET = Bonus

INCLUDEPATH += include

LIBS += -L/usr/lib -lqcustomplot

HEADERS +=  include/mainwindow.h \
            include/solver.h
SOURCES +=  src/main.cpp \
            src/mainwindow.cpp \
            src/solver.cpp
