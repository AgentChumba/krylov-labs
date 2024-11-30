QT += core gui widgets

unix: {
    INCLUDEPATH +=  /usr/local/Wolfram/Wolfram/14.1/SystemFiles/Links/WSTP/DeveloperKit/Linux-x86-64/CompilerAdditions \
                    /usr/include/qcustomplot
    LIBS += -L/usr/local/Wolfram/Wolfram/14.1/SystemFiles/Links/WSTP/DeveloperKit/Linux-x86-64/CompilerAdditions -lWSTP64i4 \
            -L/usr/lib -lqcustomplot
}

win32: {
    INCLUDEPATH +=  C:/Program Files/Wolfram Research/Mathematica/13.3/SystemFiles/Links/WSTP/DeveloperKit/Windows-x86-64/CompilerAdditions \
                    C:/Qt/6.8.0/msvc2022_64/include/
    LIBS += -LC:/Program Files/Wolfram Research/Mathematica/13.3/SystemFiles/Links/WSTP/DeveloperKit/Windows-x86-64/CompilerAdditions -lWSTP64i4 \
            -LC:/Qt/6.8.0/msvc2022_64/lib -lqcustomplotd2
}

TARGET = Lab5
TEMPLATE = app

SOURCES +=  src/main.cpp \
            src/MainWindow.cpp
            
HEADERS +=  include/MainWindow.h