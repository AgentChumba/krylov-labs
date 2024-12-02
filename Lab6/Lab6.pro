QT += core gui widgets serialport

TARGET = Lab6
TEMPLATE = app

CONFIG += c++17

LIBS += -lQt5SerialPort

SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp

HEADERS += \
    include/MainWindow.h

