#include "../include/Tab1Widget.h"
#include <QHBoxLayout>

Tab1Widget::Tab1Widget(QWidget *parent)
    : QWidget(parent) {
    setupUi();
}

Tab1Widget::~Tab1Widget() {}

void Tab1Widget::setupUi() {
    
    libraryNameEdit = new QLineEdit();
    functionNameEdit = new QLineEdit();
    
    paramXSpinBox = new QSpinBox();
    paramXSpinBox->setValue(5);

    paramYSpinBox = new QSpinBox();
    paramYSpinBox->setValue(5);

    resultEdit = new QLineEdit();
    resultEdit->setReadOnly(true);
    
    checkLibraryButton = new QPushButton("Загрузить библиотеку");
    checkFunctionButton = new QPushButton("Загрузить функцию");
    invokeFunctionButton = new QPushButton("Вызвать функцию");

    browseButton = new QPushButton("...");
    browseButton->setFixedWidth(30);

    // Создание и настройка макета
    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *bybleLayout = new QHBoxLayout();
    bybleLayout->addWidget(new QLabel("Имя библиотеки:"));
    bybleLayout->addWidget(libraryNameEdit);
    bybleLayout->addWidget(browseButton);
    bybleLayout->addStretch();
    bybleLayout->addWidget(checkLibraryButton);
    checkLibraryButton->setMinimumWidth(250);
    layout->addLayout(bybleLayout);
    
    QHBoxLayout *funLayout = new QHBoxLayout();
    funLayout->addWidget(new QLabel("Имя функции:"));
    funLayout->addWidget(functionNameEdit);
    funLayout->addStretch();
    funLayout->addWidget(checkFunctionButton);
    checkFunctionButton->setMinimumWidth(250);
    layout->addLayout(funLayout);

    layout->addWidget(new QLabel("Параметры вызова:"));

    QHBoxLayout *paramsLayout = new QHBoxLayout();
    paramsLayout->addWidget(new QLabel("X = "));
    paramsLayout->addWidget(paramXSpinBox);
    paramsLayout->addSpacing(30);
    paramsLayout->addWidget(new QLabel("Y = "));
    paramsLayout->addWidget(paramYSpinBox);
    paramsLayout->addStretch();
    paramsLayout->addWidget(invokeFunctionButton);
    invokeFunctionButton->setMinimumWidth(250);
    layout->addLayout(paramsLayout);

    QHBoxLayout *resultLayout = new QHBoxLayout();
    resultLayout->addWidget(new QLabel("Строка состояния:"));
    resultLayout->addWidget(resultEdit);
    resultLayout->addStretch();
    layout->addLayout(resultLayout);
    layout->addStretch();

    setLayout(layout);

    connect(browseButton, &QPushButton::clicked, this, &Tab1Widget::browseLibraryPath);
}

void Tab1Widget::browseLibraryPath() {
    QString filePath = QFileDialog::getOpenFileName(this, "Выберите библиотеку", "", "DLL Files (*.dll);;Все файлы (*)");
    if (!filePath.isEmpty()) {
        libraryNameEdit->setText(filePath);
    }
}
