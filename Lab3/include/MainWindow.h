#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>

#include "Interpreter.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void openScript();
    void executeScript();

private:
    QTextEdit *scriptInput;
    QTextEdit *scriptOutput;
    QPushButton *openButton;
    QPushButton *executeButton;

    Interpreter interpreter;
};

#endif // MAINWINDOW_H
