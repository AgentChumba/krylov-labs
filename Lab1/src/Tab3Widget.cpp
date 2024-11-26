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
    HKEY hKey;
    long lRes;
    TCHAR szSubKey[MAX_PATH];
    DWORD dwIndex = 0;
    DWORD dwSize = MAX_PATH;

    // Открываем реестр для поиска (например, в разделе HKEY_LOCAL_MACHINE)
    lRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\ControlSet001\\Services"), 0, KEY_READ, &hKey);
    
    if (lRes != ERROR_SUCCESS) {
        cout << "Не удалось открыть реестр!" << endl;
        return;
    }

    // Проходим по всем подкаталогам
    while (true) {
        lRes = RegEnumKeyEx(hKey, dwIndex, szSubKey, &dwSize, NULL, NULL, NULL, NULL);
        
        if (lRes == ERROR_NO_MORE_ITEMS) {
            // Нет больше ключей
            break;
        }
        
        if (lRes == ERROR_SUCCESS) {
            // Проверяем, соответствует ли подкаталог нашему запросу
            if (searchKey == szSubKey) {
                cout << "Найдено: " << szSubKey << endl;
                RegCloseKey(hKey);
                return;
            }
        }
        
        // Увеличиваем индекс и продолжаем искать
        dwIndex++;
        dwSize = MAX_PATH;
    }

    // Если не нашли
    cout << "Не найдено!" << endl;
    RegCloseKey(hKey);
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
