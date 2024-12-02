#include "../include/MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QThread>

MainWindow::~MainWindow() {}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      serialPort(new QSerialPort(this)),
      sendTimer(new QTimer(this)),
      currentMode(None) {

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QLabel *title = new QLabel("Лабораторная работа 6");
    title->setStyleSheet("font-size: 24px; font-weight: bold; text-align: center;");

    QHBoxLayout *portLayout = new QHBoxLayout();
    portNameEdit = new QLineEdit();
    portNameEdit->setPlaceholderText("Введите номер порта...");
    portLayout->addWidget(new QLabel("COM-порт:"));
    portLayout->addWidget(portNameEdit);

    QHBoxLayout *dataLayout = new QHBoxLayout();
    byteInput = new QLineEdit();
    byteInput->setPlaceholderText("Введите данные для отправки...");
    dataLayout->addWidget(new QLabel("Данные:"));
    dataLayout->addWidget(byteInput);

    btnOpenSync = new QPushButton("Открыть (Sync)");
    btnOpenAsync = new QPushButton("Открыть (Async)");
    btnClose = new QPushButton("Закрыть порт");
    btnSend = new QPushButton("Отправить данные");

    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->addWidget(btnOpenSync);
    controlLayout->addWidget(btnOpenAsync);
    controlLayout->addWidget(btnClose);
    controlLayout->addWidget(btnSend);

    logView = new QTextEdit();
    logView->setReadOnly(true);

    mainLayout->addWidget(title);
    mainLayout->addLayout(portLayout);
    mainLayout->addLayout(dataLayout);
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(logView);

    connect(btnOpenSync, &QPushButton::clicked, this, &MainWindow::openPortSync);
    connect(btnOpenAsync, &QPushButton::clicked, this, &MainWindow::openPortAsync);
    connect(btnClose, &QPushButton::clicked, this, &MainWindow::closePort);
    connect(btnSend, &QPushButton::clicked, this, &MainWindow::sendData);
    connect(sendTimer, &QTimer::timeout, this, &MainWindow::sendNextByte);
    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::onReadyRead);
}

void MainWindow::logMessage(const QString &message) {
    logView->append(message);
}

void MainWindow::openPortSync() {
    #ifdef Q_OS_WIN
        serialPort->setPortName(QString("COM%1").arg(portNameEdit->text()));
    #else
        serialPort->setPortName(QString("/dev/pts/%1").arg(portNameEdit->text()));
    #endif
    if (serialPort->open(QIODevice::ReadWrite)) {
        logView->clear();
        logMessage("Порт открыт в синхронном режиме");
        currentMode = Sync;
    } else {
        logView->clear();
        logMessage("Ошибка открытия порта: " + serialPort->errorString());
    }
}

void MainWindow::openPortAsync() {
    #ifdef Q_OS_WIN
        serialPort->setPortName(QString("COM%1").arg(portNameEdit->text()));
    #else
        serialPort->setPortName(QString("/dev/pts/%1").arg(portNameEdit->text()));
    #endif
    if (serialPort->open(QIODevice::ReadWrite)) {
        logView->clear();
        logMessage("Порт открыт в асинхронном режиме");
        currentMode = Async;
    } else {
        logView->clear();
        logMessage("Ошибка открытия порта: " + serialPort->errorString());
    }
}

void MainWindow::closePort() {
    if (serialPort->isOpen()) {
        serialPort->close();
        logView->clear();
        logMessage("Порт закрыт");
        currentMode = None;
    } else {
        logView->clear();
        logMessage("Порт уже закрыт");
    }
}

void MainWindow::sendData() {
    if (!serialPort->isOpen()) {
        logView->clear();
        logMessage("Порт не открыт");
        return;
    }

    QByteArray dataToSend = byteInput->text().toUtf8();
    if (dataToSend.isEmpty()) {
        logView->clear();
        logMessage("Нет данных для отправки");
        return;
    }

    if (currentMode == Sync) {
        sendDataSync(dataToSend);
    } else if (currentMode == Async) {
        sendDataAsync(dataToSend);
    } else {
        logView->clear();
        logMessage("GG... Mode не инициализирован... Где ошибка?");
        return;
    }
}

void MainWindow::sendDataSync(const QByteArray &data) {
    logView->clear();
    logMessage("Начинается отправка в синхронном режиме...");
    setUIEnabled(false);
    
    for (int i = 0; i < data.size(); ++i) {
        serialPort->write(data.mid(i, 1));
        serialPort->flush();
        QThread::msleep(500);
        logMessage("Отправлен байт: " + QString(data.mid(i, 1)));
    }
    setUIEnabled(true);
    logMessage("Отправка в синхронном режиме завершена");
}

void MainWindow::sendDataAsync(const QByteArray &data) {
    logView->clear();
    logMessage("Начинается отправка в асинхронном режиме...");
    longData = data;
    longDataIndex = 0;
    sendTimer->start(500);
}

void MainWindow::sendNextByte() {
    if (longDataIndex < longData.size()) {
        serialPort->write(longData.mid(longDataIndex, 1));
        logMessage("Отправлен байт: " + QString(longData.mid(longDataIndex, 1)));
        ++longDataIndex;
    } else {
        sendTimer->stop();
        logMessage("Отправка в ансинхронном режиме завершена");
    }
}

void MainWindow::onReadyRead() {
    QByteArray data;
    if (currentMode == Sync) {
        setUIEnabled(false);
        
        if (serialPort->waitForReadyRead(10000)) {
            setUIEnabled(true);
            data = serialPort->readAll();
            logMessage("Получены данные: " + QString(data));
        } else {
            setUIEnabled(true);
            logMessage("Ошибка получения данных или истёк тайм-аут");
        }
    } else {
        data = serialPort->readAll();
        logMessage("Получены данные: " + QString(data));
    }
}

void MainWindow::setUIEnabled(bool enabled) {
    btnOpenSync->setEnabled(enabled);
    btnOpenAsync->setEnabled(enabled);
    btnClose->setEnabled(enabled);
    btnSend->setEnabled(enabled);
}
