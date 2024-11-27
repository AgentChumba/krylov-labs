#include "../include/Tab1Widget.h"
#include <QHBoxLayout>

#ifdef Q_OS_WIN
#include <windows.h>
#else
#include <dlfcn.h>
#include <QProcess>
#include <QStringList>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#endif

Tab1Widget::Tab1Widget(QWidget *parent)
    : QWidget(parent) {
    setupUi();
}

Tab1Widget::~Tab1Widget() {
    if (libraryHandle) {
#ifdef Q_OS_WIN
        FreeLibrary(libraryHandle);
#else
        dlclose(libraryHandle);
#endif
    }
}

void Tab1Widget::setupUi() {
    
    libraryNameEdit = new QLineEdit();
    functionNameBox = new QComboBox();
    functionNameBox->setMinimumWidth(200);
    
    paramXSpinBox = new QDoubleSpinBox();
    paramXSpinBox->setRange(-100.0, 100.0);
    paramXSpinBox->setDecimals(2);
    paramXSpinBox->setSingleStep(1.0);
    paramXSpinBox->setValue(7.0);

    paramYSpinBox = new QDoubleSpinBox();
    paramYSpinBox->setRange(-100.0, 100.0);
    paramYSpinBox->setDecimals(2);
    paramYSpinBox->setSingleStep(1.0);
    paramYSpinBox->setValue(3.0);

    resultEdit = new QLineEdit();
    resultEdit->setReadOnly(true);
    
    loadLibraryButton = new QPushButton("Загрузить библиотеку");
    invokeFunctionButton = new QPushButton("Вызвать функцию");

    browseButton = new QPushButton("...");
    browseButton->setFixedWidth(30);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *bybleLayout = new QHBoxLayout();
    bybleLayout->addWidget(new QLabel("Имя библиотеки:"));
    bybleLayout->addWidget(libraryNameEdit);
    bybleLayout->addWidget(browseButton);
    bybleLayout->addStretch();
    bybleLayout->addWidget(loadLibraryButton);
    loadLibraryButton->setMinimumWidth(250);
    layout->addLayout(bybleLayout);
    
    QHBoxLayout *funLayout = new QHBoxLayout();
    funLayout->addWidget(new QLabel("Имя функции:"));
    funLayout->addWidget(functionNameBox);
    funLayout->addStretch();
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
    connect(loadLibraryButton, &QPushButton::clicked, this, &Tab1Widget::loadLibrary);
    connect(invokeFunctionButton, &QPushButton::clicked, this, &Tab1Widget::invokeFunction);

}

void Tab1Widget::browseLibraryPath() {
#ifdef Q_OS_WIN
    QString filter = "DLL Files (*.dll);;All Files (*)";
#else
    QString filter = "Shared Object Files (*.so);;All Files (*)";
#endif

    QString filePath = QFileDialog::getOpenFileName(this, "Выберите библиотеку", "", filter);
    if (!filePath.isEmpty()) {
        libraryNameEdit->setText(filePath);
    }
}

void Tab1Widget::loadLibrary() {
    QString libraryPath = libraryNameEdit->text();

    if (libraryPath.isEmpty()) {
        resultEdit->setText("Путь к библиотеке не указан");
        return;
    }

    clearFunctionList();

#ifdef Q_OS_WIN
    libraryHandle = LoadLibraryW(reinterpret_cast<LPCWSTR>(libraryPath.utf16()));
    if (!libraryHandle) {
        resultEdit->setText(Ошибка загрузки библиотеки!);
        return;
    }

    QStringList functionNames;

    QProcess dumpbinProcess;
    dumpbinProcess.start("dumpbin", QStringList() << "/exports" << libraryPath);
    if (!dumpbinProcess.waitForFinished()) {
        resultEdit->setText("Не удалось выполнить dumpbin!");
        return;
    }

    QString dumpbinOutput = dumpbinProcess.readAllStandardOutput();
    QRegularExpression regex(R"(\s+(\w+)\s+0x[0-9a-fA-F]+)");
    QRegularExpressionMatchIterator it = regex.globalMatch(dumpbinOutput);
    bool found = false;
    
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        if (match.hasMatch()) {
            QString functionName = match.captured(1);
            functionNameBox->addItem(functionName);
            found = true;
        }
    }

    if (!found) {
        resultEdit->setText("Функции не найдены в библиотеке!");
    } else {
        resultEdit->setText("Библиотека загружена!");
    }

#else
    libraryHandle = dlopen(libraryPath.toStdString().c_str(), RTLD_LAZY);
    if (!libraryHandle) {
        resultEdit->setText("Ошибка загрузки библиотеки!");
        return;
    }

    QProcess nmProcess;
    nmProcess.start("nm", QStringList() << "-D" << libraryPath);
    if (!nmProcess.waitForFinished()) {
        resultEdit->setText("Не удалось выполнить nm!");
        return;
    }

    QString nmOutput = nmProcess.readAllStandardOutput();

    QRegularExpression regex(R"(^[0-9a-fA-F]+ T (\w+)$)", QRegularExpression::MultilineOption);
    QRegularExpressionMatchIterator it = regex.globalMatch(nmOutput);
    bool found = false;

    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        if (match.hasMatch()) {
            QString mangledName = match.captured(1);

            QProcess filtProcess;
            filtProcess.start("c++filt", QStringList() << mangledName);
            if (filtProcess.waitForFinished()) {
                QString demangledName = filtProcess.readAllStandardOutput().trimmed();

                int parenIndex = demangledName.indexOf('(');
                QString functionName = (parenIndex != -1) 
                                   ? demangledName.left(parenIndex) 
                                   : demangledName;

                functionNameBox->addItem(functionName);
                functionNameMap.insert(functionName, mangledName);
                found = true;
            } else {
                functionNameBox->addItem(mangledName);
                functionNameMap.insert(mangledName, mangledName);
            }
        }
    }

    if (!found) resultEdit->setText("Функции не найдены!");
    else resultEdit->setText("Библиотека загружена!");
#endif
}

void Tab1Widget::clearFunctionList() {
    functionNameBox->clear();

    if (libraryHandle) {
#ifdef Q_OS_WIN
        FreeLibrary(libraryHandle);
#else
        dlclose(libraryHandle);
#endif
        libraryHandle = nullptr;
    }
}

void Tab1Widget::invokeFunction() {
    if (!libraryHandle) {
        resultEdit->setText("Библиотека не загружена!");
        return;
    }

    QString functionName = functionNameBox->currentText();
    if (functionName.isEmpty()) {
        resultEdit->setText("Функция не выбрана!");
        return;
    }
#ifdef Q_OS_LINUX
    QString mangledName = functionNameMap.value(functionName);
    if (mangledName.isEmpty()) {
        resultEdit->setText("Не удалось найти mangled имя для выбранной функции!");
    return;
    }
#endif

    double paramX = paramXSpinBox->value();
    double paramY = paramYSpinBox->value();

    typedef double (*FunctionPointer)(double, double);
    FunctionPointer func = nullptr;

#ifdef Q_OS_WIN
    func = reinterpret_cast<FunctionPointer>(GetProcAddress(libraryHandle, functionName.toStdString().c_str()));
#else
    func = reinterpret_cast<FunctionPointer>(dlsym(libraryHandle, mangledName.toStdString().c_str()));
#endif

    if (!func) {
        resultEdit->setText("Функция не найдена в библиотеке!");
        return;
    }

    try {
        double result = func(paramX, paramY);
        resultEdit->setText(QString::number(result));
    } catch (...) {
        resultEdit->setText("Ошибка при выполнении функции!");
    }
}