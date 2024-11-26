#include "../include/Tab3Widget.h"
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

Tab3Widget::~Tab3Widget() {}

Tab3Widget::Tab3Widget(QWidget *parent)
    : QWidget(parent) {

    QLabel *titleLabel = new QLabel("Поиск драйвера в реестре", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");

    QLabel *sectionLabel = new QLabel("Имя раздела:", this);
    sectionInput = new QLineEdit(this);

    QPushButton *searchButton = new QPushButton("Поиск", this);
    connect(searchButton, &QPushButton::clicked, this, &Tab3Widget::onSearchButtonClicked);

    QLabel *resultLabel = new QLabel("Результат:", this);
    resultOutput = new QTextEdit(this);
    resultOutput->setReadOnly(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(sectionLabel);
    layout->addWidget(sectionInput);
    layout->addWidget(searchButton);
    layout->addWidget(resultLabel);
    layout->addWidget(resultOutput);

    setLayout(layout);
}

void Tab3Widget::onSearchButtonClicked()
{
#ifdef Q_OS_WIN
    QString section = sectionInput->text();
    if (section.isEmpty()) {
        resultOutput->setText("Ошибка: поле 'Имя раздела' не может быть пустым.");
        return;
    }

    HKEY hKey;
    LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, section.toStdWString().c_str(), 0, KEY_READ, &hKey);
    if (result == ERROR_SUCCESS) {
        resultOutput->setText("Раздел найден!");
        RegCloseKey(hKey);
    } else {
        resultOutput->setText("Ошибка: не удалось открыть указанный раздел реестра.");
    }
#endif

#ifdef Q_OS_LINUX
    QString moduleName = sectionInput->text();
    if (moduleName.isEmpty()) {
        resultOutput->setText("Ошибка: поле 'Имя раздела' не может быть пустым.");
        return;
    }

    QString modulePath = QString("/sys/module/%1").arg(moduleName);
    if (!QFile::exists(modulePath)) {
        resultOutput->setText("Модуль не найден в /sys/module.");
        return;
    }

    QString resultText = QString("Путь к модулю: %1\n\n").arg(modulePath);
    QDir moduleDir(modulePath);

    QStringList files = moduleDir.entryList(QDir::Files);
    for (const QString &file : files) {
        QFile infoFile(modulePath + "/" + file);
        if (infoFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&infoFile);
            resultText += QString("%1: %2\n").arg(file, in.readAll().trimmed());
        }
    }

    if (resultText == "Путь к модулю: %1\n\n") resultOutput->setText("Нет доступной информации о модуле.");
    else resultOutput->setText(resultText);
#endif
}
