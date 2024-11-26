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
    QString sectionName = sectionInput->text();
    if (sectionName.isEmpty()) {
        resultOutput->setText("Пожалуйста, введите имя раздела.");
        return;
    }

    HKEY hKey;
    LONG openRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
                            ("SYSTEM\\ControlSet001\\Services\\" + sectionName).toStdWString().c_str(),
                            0, KEY_READ, &hKey);

    if (openRes == ERROR_SUCCESS) {
    resultOutput->setText("Драйвер найден в реестре!");

    DWORD bufferSize = 1024;
    wchar_t buffer[1024];
    DWORD dwType = 0;

    LONG queryRes = RegQueryValueEx(hKey, L"ImagePath", NULL, &dwType, (LPBYTE)buffer, &bufferSize);
    if (queryRes == ERROR_SUCCESS) {
        QString driverPath = QString::fromWCharArray(buffer);
        resultOutput->append("\nПуть к драйверу: " + driverPath);
    } else {
        resultOutput->append("\nНе удалось получить путь к драйверу.");
    }

    bufferSize = 1024;
    queryRes = RegQueryValueEx(hKey, L"DisplayName", NULL, &dwType, (LPBYTE)buffer, &bufferSize);
    if (queryRes == ERROR_SUCCESS) {
        QString driverDescription = QString::fromWCharArray(buffer);
        resultOutput->append("\nОписание драйвера: " + driverDescription);
    } else {
        resultOutput->append("\nНе удалось получить описание драйвера.");
    }

    RegCloseKey(hKey);
    } else {
        resultOutput->setText("Ошибка: не удалось найти драйвер в реестре.");
    }
#endif

#ifdef Q_OS_LINUX
    QString moduleName = sectionInput->text();
    if (moduleName.isEmpty()) {
        resultOutput->setText("Ошибка! Пожалуйста, введите имя раздела.");
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
