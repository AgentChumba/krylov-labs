#include "../include/Tab2Widget.h"
#include <QDebug>

Tab2Widget::~Tab2Widget() {}

Tab2Widget::Tab2Widget(QWidget *parent)
    : QWidget(parent) {

    QLabel *labelSamplingRate = new QLabel("Частота дискретизации:");
    comboSamplingRate = new QComboBox();
    comboSamplingRate->addItems({"1000 Hz", "2000 Hz", "5000 Hz", "10000 Hz"});

    btnInitCom = new QPushButton("Инициализация COM подсистемы");
    btnInitDevice = new QPushButton("Инициализировать устройство");
    btnFetchData = new QPushButton("Получить данные");
    btnStop = new QPushButton("Стоп");

    plot = new QCustomPlot();
    plot->addGraph();
    plot->xAxis->setLabel("Время");
    plot->yAxis->setLabel("Амплитуда");
    plot->setMinimumSize(400, 400);

    QHBoxLayout *rateLayout = new QHBoxLayout();
    rateLayout->addWidget(labelSamplingRate);
    rateLayout->addWidget(comboSamplingRate);
    rateLayout->addStretch();

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(btnInitCom);
    buttonLayout->addWidget(btnInitDevice);
    buttonLayout->addWidget(btnFetchData);
    buttonLayout->addWidget(btnStop);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(rateLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(plot);
    mainLayout->addStretch();

    // Связь сигналов и слотов
    connect(btnInitCom, &QPushButton::clicked, this, &Tab2Widget::initComSubsystem);
    connect(btnInitDevice, &QPushButton::clicked, this, &Tab2Widget::initDevice);
    connect(btnFetchData, &QPushButton::clicked, this, &Tab2Widget::fetchData);
    connect(btnStop, &QPushButton::clicked, this, &Tab2Widget::stopFetching);
}

void Tab2Widget::initComSubsystem()
{
    qDebug("Инициализация COM подсистемы...");
}

void Tab2Widget::initDevice()
{
    qDebug("Инициализация устройства BORDO322...");
}

void Tab2Widget::fetchData()
{
    qDebug("Получение данных с устройства...");
    QVector<double> x(101), y(101);
    for (int i = 0; i < 101; ++i) {
        x[i] = i / 10.0;
        y[i] = qSin(x[i]);
    }
    plot->graph(0)->setData(x, y);
    plot->replot();
}

void Tab2Widget::stopFetching()
{
    qDebug("Останов получения данных.");
}
