#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QProcess>

#ifdef Q_OS_WIN
#include <QCustomPlot/qcustomplot.h>
#include <WSTP/wstp.h>
#else
#include <qcustomplot.h>
#include "wstp.h"
#endif

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void initializeMath();
    void calculate();
    void clearGraphs();

private:
    WSENV env;
    WSLINK link;

    QLineEdit   *inputEdit,
                *t0Edit,
                *tMaxEdit,
                *dTEdit,
                *debugEdit;

    QPushButton *calculateBtn,
                *initializeBtn,
                *clearBtn;

    QCustomPlot *viewSignal,
                *viewEnvelope,
                *viewSpectrum;
};

#endif // MAIN_WINDOW_H
