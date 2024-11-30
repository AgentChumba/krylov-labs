QT += core gui widgets

unix: {
    INCLUDEPATH +=  /usr/local/Wolfram/Wolfram/14.1/SystemFiles/Links/MathLink/DeveloperKit/Linux-x86-64/CompilerAdditions \
                    /usr/local/Wolfram/Wolfram/14.1/SystemFiles/Links/WSTP/DeveloperKit/Linux-x86-64/CompilerAdditions \
                    /usr/include/qcustomplot
    LIBS += -L/usr/local/Wolfram/Wolfram/14.1/SystemFiles/Links/MathLink/DeveloperKit/Linux-x86-64/CompilerAdditions -lML64i4 \
            -L/usr/local/Wolfram/Wolfram/14.1/SystemFiles/Links/WSTP/DeveloperKit/Linux-x86-64/CompilerAdditions -lWSTP64i4 \
            -L/usr/lib -lqcustomplot
}

TARGET = Lab5
TEMPLATE = app

SOURCES +=  src/main.cpp \
            src/MainWindow.cpp
            
HEADERS +=  include/MainWindow.h