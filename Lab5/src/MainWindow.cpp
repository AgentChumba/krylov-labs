#include "../include/MainWindow.h"
#include <stdio.h>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainlayout = new QVBoxLayout(centralWidget);

    QLabel *title = new QLabel("Лабораторная работа 2");
    title->setStyleSheet("font-size: 24px; font-weight: bold; text-align: center;");

    QHBoxLayout *inputLayout = new QHBoxLayout();
    QLabel *inputLabel = new QLabel("X[t] = ");
    inputEdit = new QLineEdit();
    inputEdit->setPlaceholderText("Уравнение");
    inputLayout->addWidget(inputLabel);
    inputLayout->addWidget(inputEdit);    

    QHBoxLayout *paramsLayout = new QHBoxLayout();
    QLabel *t0Label = new QLabel("t0 = ");
    QLabel *tMaxLabel = new QLabel("tMax = ");
    QLabel *dTLabel = new QLabel("dT = ");
    t0Edit = new QLineEdit();
    tMaxEdit = new QLineEdit();
    dTEdit = new QLineEdit();
    paramsLayout->addWidget(t0Label);
    paramsLayout->addWidget(t0Edit);
    paramsLayout->addWidget(tMaxLabel);
    paramsLayout->addWidget(tMaxEdit);
    paramsLayout->addWidget(dTLabel);
    paramsLayout->addWidget(dTEdit);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    initializeBtn = new QPushButton("Инициализация");
    calculateBtn = new QPushButton("Построить");
    clearBtn = new QPushButton("Очистить");
    btnLayout->addWidget(initializeBtn);
    btnLayout->addWidget(calculateBtn);
    btnLayout->addWidget(clearBtn);

    QHBoxLayout *debugLayout = new QHBoxLayout();
    QLabel *debugLabel = new QLabel("Строка состояния:");
    debugEdit = new QLineEdit("Ожидание действий");
    debugEdit->setReadOnly(true);
    debugLayout->addWidget(debugLabel);
    debugLayout->addWidget(debugEdit);

    QHBoxLayout *graphLayout = new QHBoxLayout();
    viewSignal = new QCustomPlot();
    viewEnvelope = new QCustomPlot();
    viewSpectrum = new QCustomPlot();

    viewSignal->addGraph();
    viewSignal->xAxis->setLabel("t");
    viewSignal->yAxis->setLabel("X(t)");
    viewSignal->setMinimumSize(300, 300);    

    viewEnvelope->addGraph();
    viewEnvelope->xAxis->setLabel("t");
    viewEnvelope->yAxis->setLabel("Amplitude");
    viewEnvelope->setMinimumSize(300, 300);

    viewSpectrum->addGraph();
    viewSpectrum->xAxis->setLabel("Frequency");
    viewSpectrum->yAxis->setLabel("Amplitude");
    viewSpectrum->setMinimumSize(300, 300);

    graphLayout->addWidget(viewSignal);
    graphLayout->addWidget(viewEnvelope);
    graphLayout->addWidget(viewSpectrum);

    mainlayout->addWidget(title);
    mainlayout->addLayout(inputLayout);
    mainlayout->addLayout(paramsLayout);
    mainlayout->addLayout(btnLayout);
    mainlayout->addLayout(debugLayout);
    mainlayout->addLayout(graphLayout);
    mainlayout->addStretch();

    setCentralWidget(centralWidget);

    connect(initializeBtn, &QPushButton::clicked, this, &MainWindow::initializeMath);
    connect(calculateBtn, &QPushButton::clicked, this, &MainWindow::calculate);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::clearGraphs);
}

MainWindow::~MainWindow() {
    if (link) WSClose(link);
    if (env) WSDeinitialize(env);
}

void MainWindow::initializeMath() {
    const char* argv[] = { "-linkmode", "launch", "-linkname", "MathKernel" };
    int argc = sizeof(argv) / sizeof(argv[0]);
    int err;

    env = WSInitialize(NULL);
    if (!env) {
        qWarning() << "Ошибка: не удалось инициализировать MathLink.";
        return;
    }

    link = WSOpenArgcArgv(env, argc, (char**)argv, &err);
    if (!link || err != WSEOK) {
        qWarning() << "Ошибка: не удалось подключиться к MathKernel. Код ошибки: " << err;
        return;
    }

    debugEdit->setText("Соединение с MathLink установлено");
}

void MainWindow::calculate() {
    if (!env) {
        debugEdit->setText("Ошибка: не инициализирован MathLink.");
        return;
    }

    QString formula = inputEdit->text();
    QString t0 = t0Edit->text();
    QString tMax = tMaxEdit->text();
    QString dT = dTEdit->text();

    if (formula.isEmpty() || t0.isEmpty() || tMax.isEmpty() || dT.isEmpty()) {
        debugEdit->setText("Ошибка: все поля должны быть заполнены!");
        return;
    }

    QString command = QString("Table[%1, {t, %2, %3, %4}]")
                    .arg(formula)
                    .arg(t0)
                    .arg(tMax)
                    .arg(dT);

    if (!WSPutFunction(link, "ToExpression", 1) || !WSPutString(link, command.toUtf8().constData())) {
        debugEdit->setText("Ошибка отправки команды (График 1) в MathLink.");
        return;
    }
    WSEndPacket(link);

    int packet;
    while ((packet = WSNextPacket(link)) && packet != RETURNPKT) {
        WSNewPacket(link);
    }

    double* resultArray;
    int length;
    if (!WSGetReal64List(link, &resultArray, &length)) {
        debugEdit->setText("Ошибка получения данных (График 1) от MathLink.");
        return;
    }

    QString envelopeCommand = QString("Table[Abs[%1], {t, %2, %3, %4}]")
                             .arg(formula)
                             .arg(t0)
                             .arg(tMax)
                             .arg(dT);

    if (!WSPutFunction(link, "ToExpression", 1) || !WSPutString(link, envelopeCommand.toUtf8().constData())) {
        debugEdit->setText("Ошибка отправки команды (График 2) в MathLink.");
        WSReleaseReal64List(link, resultArray, length);
        return;
    }
    WSEndPacket(link);

    while ((packet = WSNextPacket(link)) && packet != RETURNPKT) {
        WSNewPacket(link);
    }

    double* envelopeArray;
    int envelopeLength;
    if (!WSGetReal64List(link, &envelopeArray, &envelopeLength)) {
        debugEdit->setText("Ошибка получения данных (График 2) от MathLink.");
        WSReleaseReal64List(link, resultArray, length);
        return;
    }

    QString spectrumCommand = QString("Abs[Fourier[Table[%1, {t, %2, %3, %4}]]]")
                             .arg(formula)
                             .arg(t0)
                             .arg(tMax)
                             .arg(dT);

    if (!WSPutFunction(link, "ToExpression", 1) || !WSPutString(link, spectrumCommand.toUtf8().constData())) {
        debugEdit->setText("Ошибка отправки команды (График 3) в MathLink.");
        WSReleaseReal64List(link, resultArray, length);
        WSReleaseReal64List(link, envelopeArray, envelopeLength);
        return;
    }
    WSEndPacket(link);

    while ((packet = WSNextPacket(link)) && packet != RETURNPKT) {
        WSNewPacket(link);
    }

    double* spectrumArray = nullptr;
    int spectrumLength = 0;
    if (!WSGetReal64List(link, &spectrumArray, &spectrumLength)) {
        debugEdit->setText("Ошибка получения данных (Спектр) от MathLink.");
        WSReleaseReal64List(link, resultArray, length);
        WSReleaseReal64List(link, envelopeArray, envelopeLength);
        return;
    }

    debugEdit->setText("Вычисления завершены");

    QVector<double> xData, yData, envelopeData, spectrumData, freqData;
    for (int i = 0; i < length; ++i) {
        xData.append(i);
        yData.append(resultArray[i]);
        envelopeData.append(envelopeArray[i]);
    }

    for (int i = 0; i < spectrumLength; ++i) {
        freqData.append(i);
        spectrumData.append(spectrumArray[i]);
    }

    WSReleaseReal64List(link, resultArray, length);
    WSReleaseReal64List(link, envelopeArray, envelopeLength);
    WSReleaseReal64List(link, spectrumArray, spectrumLength);

    viewSignal->addGraph();
    viewSignal->graph(viewSignal->graphCount() - 1)->setData(xData, yData);
    viewSignal->rescaleAxes();
    viewSignal->replot();

    viewEnvelope->addGraph();
    viewEnvelope->graph(viewEnvelope->graphCount() - 1)->setData(xData, envelopeData);
    viewEnvelope->rescaleAxes();
    viewEnvelope->replot();

    viewSpectrum->addGraph();
    viewSpectrum->graph(viewSpectrum->graphCount() - 1)->setData(freqData, spectrumData);
    viewSpectrum->rescaleAxes();
    viewSpectrum->replot();
}

void MainWindow::clearGraphs() {
    viewSignal->clearGraphs();
    viewSignal->addGraph();
    viewSignal->replot();
    
    viewEnvelope->clearGraphs();
    viewEnvelope->addGraph();
    viewEnvelope->replot();

    viewSpectrum->clearGraphs();
    viewSpectrum->addGraph();
    viewSpectrum->replot();

    debugEdit->setText("Графики очищены");
}