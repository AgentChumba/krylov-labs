#include "../include/MainWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QLabel>
#include <QTextStream>
#include <QMessageBox>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Лабораторная работа 3");

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QLabel *titleLabel = new QLabel("Лабораторная работа 3");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; text-align: center;");

    QHBoxLayout *inputLayout = new QHBoxLayout();
    scriptInput = new QTextEdit();
    scriptOutput = new QTextEdit();
    scriptInput->setPlaceholderText("Введите скрипт...");
    scriptOutput->setPlaceholderText("Результат выполнения...");
    scriptOutput->setReadOnly(true);
    inputLayout->addWidget(scriptInput);
    inputLayout->addWidget(scriptOutput);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    openButton = new QPushButton("Открыть скрипт");
    executeButton = new QPushButton("Выполнить скрипт");
    buttonLayout->addWidget(openButton);
    buttonLayout->addWidget(executeButton);

    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(inputLayout);

    connect(openButton, &QPushButton::clicked, this, &MainWindow::openScript);
    connect(executeButton, &QPushButton::clicked, this, &MainWindow::executeScript);
}

void MainWindow::openScript() {
    scriptInput->clear();
    scriptOutput->clear();
    
    QString filePath = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Text Files (*.txt)");
    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл.");
        return;
    }

    QTextStream stream(&file);
    scriptInput->setText(stream.readAll());
    file.close();
}

void MainWindow::executeScript() {
    scriptOutput->clear();
    QString inputText = scriptInput->toPlainText();
    std::string result = interpreter.interpret(inputText.toStdString().c_str());
    scriptOutput->setText(QString::fromStdString(result));
}