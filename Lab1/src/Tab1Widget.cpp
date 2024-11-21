#include "../include/Tab1Widget.h"

#include <QHBoxLayout>
#include <QGroupBox>
#include <QLineEdit>

Tab1Widget::~Tab1Widget() {
    if (serialPort && serialPort->isOpen()) serialPort->close();
    delete serialPort;
}

Tab1Widget::Tab1Widget(QWidget *parent)
    : QWidget(parent) {
    
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    QGroupBox *initGroup = new QGroupBox("Инициализация канала управления", this);
    QVBoxLayout *initVLayout = new QVBoxLayout(initGroup);
    QHBoxLayout *initHLayout = new QHBoxLayout();

    QGroupBox *portGroup = new QGroupBox("Порт:", initGroup);
    QVBoxLayout *portLayout = new QVBoxLayout(portGroup);

    com1Button = new QRadioButton("COM1", portGroup);
    com2Button = new QRadioButton("COM2", portGroup);
    com3Button = new QRadioButton("COM3", portGroup);
    com4Button = new QRadioButton("COM4", portGroup);
    customPortButton = new QRadioButton("COM", portGroup);
    customPortSpin = new QSpinBox(portGroup);
    customPortSpin->setRange(5, 255);
    customPortSpin->setValue(5);
    customPortSpin->setEnabled(false);

    connect(customPortButton, &QRadioButton::toggled, customPortSpin, &QSpinBox::setEnabled);

    portLayout->addWidget(com1Button);
    portLayout->addWidget(com2Button);
    portLayout->addWidget(com3Button);
    portLayout->addWidget(com4Button);
    QHBoxLayout *customPortLayout = new QHBoxLayout();
    customPortLayout->addWidget(customPortButton);
    customPortLayout->addWidget(customPortSpin);
    portLayout->addLayout(customPortLayout);

    QGroupBox *paramGroup = new QGroupBox("Параметры обмена:", initGroup);
    QVBoxLayout *paramLayout = new QVBoxLayout(paramGroup);

    QHBoxLayout *speedLayout = new QHBoxLayout();
    QLabel *speedLabel = new QLabel("Скорость", paramGroup);
    speedCombo = new QComboBox(paramGroup);
    speedCombo->addItems({"9600", "14400", "19200", "28800", "38400"});
    speedLayout->addWidget(speedLabel);
    speedLayout->addWidget(speedCombo);

    QHBoxLayout *dataBitsLayout = new QHBoxLayout();
    QLabel *dataBitsLabel = new QLabel("Data Bits", paramGroup);
    dataBitsCombo = new QComboBox(paramGroup);
    dataBitsCombo->addItems({"7", "8"});
    dataBitsLayout->addWidget(dataBitsLabel);
    dataBitsLayout->addWidget(dataBitsCombo);

    QHBoxLayout *stopBitsLayout = new QHBoxLayout();
    QLabel *stopBitsLabel = new QLabel("Stop Bits", paramGroup);
    stopBitsCombo = new QComboBox(paramGroup);
    stopBitsCombo->addItems({"1", "2"});
    stopBitsLayout->addWidget(stopBitsLabel);
    stopBitsLayout->addWidget(stopBitsCombo);

    QHBoxLayout *parityLayout = new QHBoxLayout();
    QLabel *parityLabel = new QLabel("Четность", paramGroup);
    parityCombo = new QComboBox(paramGroup);
    parityCombo->addItems({"none", "even", "odd"});
    parityLayout->addWidget(parityLabel);
    parityLayout->addWidget(parityCombo);

    paramLayout->addLayout(speedLayout);
    paramLayout->addLayout(dataBitsLayout);
    paramLayout->addLayout(stopBitsLayout);
    paramLayout->addLayout(parityLayout);

    QPushButton *initButton = new QPushButton("Инициализация порта", initGroup);
    connect(initButton, &QPushButton::clicked, this, &Tab1Widget::onInitPortClicked);

    initHLayout->addWidget(portGroup);
    initHLayout->addSpacing(60);
    initHLayout->addWidget(paramGroup);
    initVLayout->addLayout(initHLayout);
    initVLayout->addWidget(initButton);
    initVLayout->addStretch();

    QGroupBox *controlGroup = new QGroupBox("Посылка сигналов управления", this);
    QVBoxLayout *controlLayout = new QVBoxLayout(controlGroup);

    QHBoxLayout *sensorLayout = new QHBoxLayout();
    QLabel *sensorLabel = new QLabel("Номер кюветы сенсора:", controlGroup);
    sensorLineEdit = new QLineEdit("1", controlGroup);
    sensorLayout->addWidget(sensorLabel);
    sensorLayout->addWidget(sensorLineEdit);

    QHBoxLayout *registerLayout = new QHBoxLayout();
    QLabel *registerLabel = new QLabel("Индекс регистра:", controlGroup);
    registerLineEdit = new QLineEdit("100", controlGroup);
    registerLayout->addWidget(registerLabel);
    registerLayout->addWidget(registerLineEdit);

    sendSignalButton = new QPushButton("Отправить сигнал");
    connect(sendSignalButton, &QPushButton::clicked, this, &Tab1Widget::onSendSignal);

    QHBoxLayout *resultLayout = new QHBoxLayout();
    QLabel *resultLabel = new QLabel("Результат:", controlGroup);
    resultField = new QLineEdit("Не запущено", controlGroup);
    resultField->setReadOnly(true);
    resultLayout->addWidget(resultLabel);
    resultLayout->addWidget(resultField);

    controlLayout->addLayout(sensorLayout);
    controlLayout->addLayout(registerLayout);
    controlLayout->addWidget(sendSignalButton);
    controlLayout->addLayout(resultLayout);
    controlLayout->addStretch();

    mainLayout->addWidget(initGroup);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(controlGroup);
    mainLayout->addStretch();
    setLayout(mainLayout);
}

void Tab1Widget::onInitPortClicked()
{
    QString selectedPort;

    #ifdef Q_OS_WIN
        if      (com1Button->isChecked()) selectedPort = "COM1";
        else if (com2Button->isChecked()) selectedPort = "COM2";
        else if (com3Button->isChecked()) selectedPort = "COM3";
        else if (com4Button->isChecked()) selectedPort = "COM4";
        else if (customPortButton->isChecked()) selectedPort = QString("COM%1").arg(customPortSpin->value());
        else {
            resultField->setText("Порт не выбран!");
            return;
        }
    #else
        selectedPort = "/dev/pts/";
        if      (com1Button->isChecked()) selectedPort += "1";
        else if (com2Button->isChecked()) selectedPort += "2";
        else if (com3Button->isChecked()) selectedPort += "3";
        else if (com4Button->isChecked()) selectedPort += "4";
        else if (customPortButton->isChecked()) selectedPort += QString("%1").arg(customPortSpin->value());
        else {
            resultField->setText("Порт не выбран!");
            return;
        }
    #endif

    if (serialPort == nullptr) serialPort = new QSerialPort(selectedPort, this);

    serialPort->setBaudRate(speedCombo->currentText().toInt());
    serialPort->setDataBits(static_cast<QSerialPort::DataBits>(dataBitsCombo->currentText().toInt()));
    serialPort->setStopBits(static_cast<QSerialPort::StopBits>(stopBitsCombo->currentText().toInt()));
    serialPort->setParity(static_cast<QSerialPort::Parity>(parityCombo->currentIndex()));

    if (!serialPort->open(QIODevice::ReadWrite)) {
        qDebug(QString("Не удалось открыть порт: %1").arg(serialPort->errorString()).toUtf8());
        resultField->setText("Ошибка открытия порта: " + serialPort->errorString());
        return;
    }
    
    qDebug(QString("Порт %1 успешно инициализирован!").arg(selectedPort).toUtf8());
    resultField->setText("Порт " + selectedPort + " успешно инициализирован!");
}

void Tab1Widget::onSendSignal()
{
    QString sensorNumber = sensorLineEdit->text();
    QString registerIndex = registerLineEdit->text();

    if (sensorNumber.isEmpty() || registerIndex.isEmpty()) {
        resultField->setText("Ошибка: Укажите все параметры!");
        return;
    }

    QByteArray signalCommand;
    signalCommand.append(sensorNumber.toUtf8());
    signalCommand.append(registerIndex.toUtf8());

    if (serialPort && serialPort->isOpen()) {
        qint64 bytesWritten = serialPort->write(signalCommand);
        if (bytesWritten == -1) {
            qDebug(QString("Ошибка при отправке данных: %1").arg(serialPort->errorString()).toUtf8());
            resultField->setText("Ошибка при отправке сигнала.");
        } else {
            qDebug(QString("Сигнал отправлен, количество байт: %1").arg(bytesWritten).toUtf8());
            resultField->setText("Сигнал успешно отправлен!");
        }
    } else {
        resultField->setText("Порт не открыт или не выбран!");
    }
}
