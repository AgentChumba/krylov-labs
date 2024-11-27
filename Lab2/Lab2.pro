QT       += core gui widgets
CONFIG   += c++17

INCLUDEPATH += /usr/include/qcustomplot
LIBS += -L/usr/lib -lqcustomplot

TARGET = Lab2
TEMPLATE = app

SOURCES += src/main.cpp \
           src/MainWindow.cpp \
           src/Tab1Widget.cpp \
           src/Tab2Widget.cpp

HEADERS += include/MainWindow.h \
           include/Tab1Widget.h \
           include/Tab2Widget.h
