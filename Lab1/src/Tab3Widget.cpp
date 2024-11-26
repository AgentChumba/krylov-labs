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
    QString searchTerm = sectionInput->text();
    if (searchTerm.isEmpty()) {
        resultOutput->setText("Ошибка: поле 'Имя раздела' не может быть пустым.");
        return;
    }

    QString resultText = "Поиск результатов:\n\n";

    // Начинаем поиск с корня HKEY_LOCAL_MACHINE
    HKEY hKey = HKEY_LOCAL_MACHINE;
    searchRegistry(hKey, searchTerm, resultText); // Поиск в текущем ключе
    searchRegistryInSubKeys(hKey, searchTerm, resultText); // Поиск во всех подкаталогах

    if (resultText == "Поиск результатов:\n\n") {
        resultOutput->setText("Ничего не найдено.");
    } else {
        resultOutput->setText(resultText);
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

#ifdef Q_OS_WIN
void searchRegistry(HKEY hKey, const QString &searchTerm, QString &resultText, DWORD index = 0)
{
    TCHAR valueName[256];
    BYTE data[256];
    DWORD valueNameSize, dataSize, type;
    
    while (true) {
        valueNameSize = 256;
        dataSize = 256;
        LONG result = RegEnumValue(hKey, index, valueName, &valueNameSize, nullptr, &type, data, &dataSize);
        if (result == ERROR_NO_MORE_ITEMS) break;
        if (result != ERROR_SUCCESS) {
            resultText += QString("Ошибка при чтении значения #%1\n").arg(index);
            continue;
        }

        QString valueString;
        if (type == REG_SZ || type == REG_EXPAND_SZ) {
            valueString = QString::fromWCharArray(reinterpret_cast<wchar_t *>(data));
        } else if (type == REG_DWORD) {
            DWORD value = *reinterpret_cast<DWORD *>(data);
            valueString = QString::number(value);
        } else {
            valueString = QString("Данные в неподдерживаемом формате (тип %1)").arg(type);
        }

        // Проверяем на совпадение с поисковым запросом
        if (QString::fromWCharArray(valueName).contains(searchTerm, Qt::CaseInsensitive) || 
            valueString.contains(searchTerm, Qt::CaseInsensitive)) {
            resultText += QString("Ключ: %1, Значение: %2\n")
                              .arg(QString::fromWCharArray(valueName))
                              .arg(valueString);
        }
        ++index;
    }
}

void searchRegistryInSubKeys(HKEY hKey, const QString &searchTerm, QString &resultText)
{
    TCHAR subKey[256];
    DWORD subKeySize;
    LONG result;
    DWORD index = 0;

    while (true) {
        subKeySize = 256;
        result = RegEnumKeyEx(hKey, index, subKey, &subKeySize, nullptr, nullptr, nullptr, nullptr);
        if (result == ERROR_NO_MORE_ITEMS) break;
        if (result != ERROR_SUCCESS) {
            resultText += QString("Ошибка при чтении подкаталога #%1\n").arg(index);
            continue;
        }

        HKEY hSubKey;
        result = RegOpenKeyEx(hKey, subKey, 0, KEY_READ, &hSubKey);
        if (result == ERROR_SUCCESS) {
            // Рекурсивно ищем в текущем подкаталоге
            searchRegistry(hSubKey, searchTerm, resultText);
            RegCloseKey(hSubKey);
        }
        ++index;
    }
}
#endif