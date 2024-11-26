QT       += core gui widgets serialport
CONFIG   += c++17

unix {
    LIBS     += -lusb-1.0
}

TARGET = Lab1
TEMPLATE = app

SOURCES += src/main.cpp \
           src/MainWindow.cpp \
           src/Tab1Widget.cpp \
           src/Tab2Widget.cpp \
           src/Tab3Widget.cpp

HEADERS += include/MainWindow.h \
           include/Tab1Widget.h \
           include/Tab2Widget.h \
           include/Tab3Widget.h
