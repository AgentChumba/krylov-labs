#include "../include/MainWindow.h"
#include "../include/Tab1Widget.h"
#include "../include/Tab2Widget.h"
#include "../include/Tab3Widget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      centralWidget(new QWidget(this)),
      mainLayout(new QVBoxLayout),
      title1(new QLabel("Лабораторная работа 1", this)),
      tabWidget(new QTabWidget(this)) {

    title1->setStyleSheet("font-size: 24px; font-weight: bold; text-align: center;");
    
    tabWidget->addTab(new Tab1Widget(this), "1");
    tabWidget->addTab(new Tab2Widget(this), "2");
    tabWidget->addTab(new Tab3Widget(this), "3");

    mainLayout->addWidget(title1);
    title1->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(tabWidget);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    setWindowTitle("Лабораторная 1");
    resize(800, 600);
}

MainWindow::~MainWindow() {}
