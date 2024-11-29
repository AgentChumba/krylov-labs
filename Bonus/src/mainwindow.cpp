#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    QVBoxLayout *settingsLayout = new QVBoxLayout();

    settingsLayout->addStretch();
    QHBoxLayout *plateWidthLayout = new QHBoxLayout();
    QLabel *plateWidthLabel = new QLabel("Ширина пластины:");
    plateWidthInput = new QLineEdit();
    plateWidthLayout->addWidget(plateWidthLabel);
    plateWidthLayout->addWidget(plateWidthInput);
    settingsLayout->addLayout(plateWidthLayout);

    QHBoxLayout *plateHeightLayout = new QHBoxLayout();
    QLabel *plateHeightLabel = new QLabel("Высота пластины:");
    plateHeightInput = new QLineEdit();
    plateHeightLayout->addWidget(plateHeightLabel);
    plateHeightLayout->addWidget(plateHeightInput);
    settingsLayout->addLayout(plateHeightLayout);

    QHBoxLayout *distanceLayout = new QHBoxLayout();
    QLabel *distanceLabel = new QLabel("Расстояние между пластинами:");
    distanceInput = new QLineEdit();
    distanceLayout->addWidget(distanceLabel);
    distanceLayout->addWidget(distanceInput);
    settingsLayout->addLayout(distanceLayout);

    QHBoxLayout *potentialLayout = new QHBoxLayout();
    QLabel *potentialLabel = new QLabel("Потенциал пластины (U):");
    potentialInput = new QLineEdit();
    potentialLayout->addWidget(potentialLabel);
    potentialLayout->addWidget(potentialInput);
    settingsLayout->addLayout(potentialLayout);

    QHBoxLayout *boundaryConditionLayout = new QHBoxLayout();
    QLabel *boundaryConditionLabel = new QLabel("Граничные условия:");
    boundaryConditionSelector = new QComboBox();
    boundaryConditionSelector->addItem("Нулевой поток (Neumann)");
    boundaryConditionSelector->addItem("Фиксированный потенциал (Dirichlet)");
    boundaryConditionLayout->addWidget(boundaryConditionLabel);
    boundaryConditionLayout->addWidget(boundaryConditionSelector);
    settingsLayout->addLayout(boundaryConditionLayout);

    QHBoxLayout *fixedPotentialLayout = new QHBoxLayout();
    QLabel *fixedPotentialLabel = new QLabel("Фиксированный потенциал (если выбран):");
    fixedPotentialInput = new QLineEdit();
    fixedPotentialLayout->addWidget(fixedPotentialLabel);
    fixedPotentialLayout->addWidget(fixedPotentialInput);
    settingsLayout->addLayout(fixedPotentialLayout);

    calculateButton = new QPushButton("Рассчитать");
    settingsLayout->addWidget(calculateButton);

    settingsLayout->addStretch();

    QHBoxLayout *chargeTextLayout = new QHBoxLayout();
    QLabel *chargeTextLabel = new QLabel("Заряд:");
    chargeLabel = new QLabel("Not calculated");
    chargeTextLayout->addWidget(chargeTextLabel);
    chargeTextLayout->addWidget(chargeLabel);
    settingsLayout->addLayout(chargeTextLayout);

    QHBoxLayout *capacityTextLayout = new QHBoxLayout();
    QLabel *capacityTextLabel = new QLabel("Ёмкость:");
    capacityLabel = new QLabel("Not calculated");
    capacityTextLayout->addWidget(capacityTextLabel);
    capacityTextLayout->addWidget(capacityLabel);
    settingsLayout->addLayout(capacityTextLayout);

    settingsLayout->addStretch();

    mainLayout->addStretch();
    mainLayout->addLayout(settingsLayout);

    QVBoxLayout *potentialPlotLayout = new QVBoxLayout();
    potentialPlot = new QCustomPlot(this);
    potentialPlot->xAxis->setLabel("X");
    potentialPlot->yAxis->setLabel("Y");
    potentialPlot->setMinimumSize(400, 400);
    potentialPlotLayout->addStretch();
    potentialPlotLayout->addWidget(potentialPlot);
    potentialPlotLayout->addStretch();
    mainLayout->addStretch();
    mainLayout->addLayout(potentialPlotLayout);
    mainLayout->addStretch();

    connect(calculateButton, &QPushButton::clicked, this, &MainWindow::onSolveButtonClicked);
}

void MainWindow::onSolveButtonClicked()
{
    int width = plateWidthInput->text().toInt();
    int height = plateHeightInput->text().toInt();
    int distance = distanceInput->text().toInt();
    double potential = potentialInput->text().toDouble();
    double fixedPotential = fixedPotentialInput->text().toDouble();
    int boundaryType = boundaryConditionSelector->currentIndex(); // 0 - Neumann, 1 - Dirichlet

    if (width <= 0 || height <= 0 || distance <= 0 || potential <= 0) {
        qDebug() << "Некорректные входные данные.";
        return;
    }
    qDebug() << "Aboba ?";
    if (solver) {
        delete solver;
        solver = nullptr;
    }
    qDebug() << "Aboba 1";
    solver = new Solver(width + 2, height + distance + 2, 1.0, 1.0, 1.0);
    qDebug() << "Aboba 2";
    solver->setBoundaryConditions(potential, fixedPotential, boundaryType);
    qDebug() << "Aboba 3";
    solver->setPlates(1, width, height + 1, height + distance, potential);
    qDebug() << "Aboba 4";
    solver->solve();
    qDebug() << "Aboba 5";
    double charge = solver->computeCharge();
    qDebug() << "Aboba 6";
    double capacitance = solver->computeCapacitance();

    chargeLabel->setText(QString("%1").arg(charge));
    capacityLabel->setText(QString("%1").arg(capacitance));

    plotPotential(solver->getSolution());
}

void MainWindow::plotPotential(const std::vector<std::vector<double>> &phi)
{
    potentialPlot->clearPlottables();

    int nx = phi.size();
    int ny = phi[0].size();

    QCPColorMap *colorMap = new QCPColorMap(potentialPlot->xAxis, potentialPlot->yAxis);
    colorMap->data()->setSize(nx, ny);
    colorMap->data()->setRange(QCPRange(0, nx - 1), QCPRange(0, ny - 1));

    for (int x = 0; x < nx; ++x)
    {
        for (int y = 0; y < ny; ++y)
        {
            colorMap->data()->setCell(x, y, phi[x][y]);
        }
    }

    QCPColorScale *colorScale = new QCPColorScale(potentialPlot);
    potentialPlot->plotLayout()->addElement(0, 1, colorScale);
    colorScale->setType(QCPAxis::atRight);
    colorMap->setColorScale(colorScale);
    colorMap->setGradient(QCPColorGradient::gpJet);
    colorMap->rescaleDataRange();

    potentialPlot->rescaleAxes();
    potentialPlot->replot();
}
