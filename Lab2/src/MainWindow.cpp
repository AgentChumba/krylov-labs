#include "../include/MainWindow.h"
#include "../include/Tab1Widget.h"
#include "../include/Tab2Widget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      centralWidget(new QWidget(this)),
      mainLayout(new QVBoxLayout),
      title(new QLabel("Лабораторная работа 2", this)),
      tabWidget(new QTabWidget(this)) {

    // Настройка заголовков
    title->setStyleSheet("font-size: 24px; font-weight: bold; text-align: center;");

    // Добавление вкладок через отдельные классы
    tabWidget->addTab(new Tab1Widget(this), "1");
    tabWidget->addTab(new Tab2Widget(this), "2");

    // Компоновка
    mainLayout->addWidget(title);
    title->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(tabWidget);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Настройка главного окна
    setWindowTitle("Лабораторная 2");
    resize(800, 600);
}

MainWindow::~MainWindow() {}
