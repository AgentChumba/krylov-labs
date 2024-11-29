#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>

#include "qcustomplot.h"
#include "solver.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSolveButtonClicked();

private:
    QWidget *centralWidget;

    QLineEdit *plateWidthInput;
    QLineEdit *plateHeightInput;
    QLineEdit *distanceInput;
    QLineEdit *potentialInput;
    QLineEdit *fixedPotentialInput;

    QPushButton *calculateButton;

    QComboBox *boundaryConditionSelector;

    QLabel *chargeLabel;
    QLabel *capacityLabel;

    QCustomPlot *potentialPlot;
    void plotPotential(const std::vector<std::vector<double>> &phi);

    void setupUI();

    Solver *solver = nullptr;
};

#endif // MAINWINDOW_H