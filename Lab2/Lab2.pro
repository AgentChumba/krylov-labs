QT       += core gui widgets
CONFIG   += c++17

unix: {
INCLUDEPATH += /usr/include/qcustomplot
LIBS += -L/usr/lib -lqcustomplot
}

win32: {
INCLUDEPATH += "C:/Qt/6.8.0/msvc2022_64/include/"
LIBS += -L"C:/Qt/6.8.0/msvc2022_64/lib" -lqcustomplotd2
}

TARGET = Lab2
TEMPLATE = app

SOURCES += src/main.cpp \
           src/MainWindow.cpp \
           src/Tab1Widget.cpp \
           src/Tab2Widget.cpp \
           src/OscB322Lib_TLB.cpp

HEADERS += include/MainWindow.h \
           include/Tab1Widget.h \
           include/Tab2Widget.h \
           include/OscB322Lib_TLB.h
