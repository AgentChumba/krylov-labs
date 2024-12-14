#ifndef TAB2WIDGET_H
#define TAB2WIDGET_H

#include <QProcess>

#ifdef Q_OS_WIN
#include <QCustomPlot/qcustomplot.h>
#else
#include <QWidget>
#endif

class Tab2Widget : public QWidget {
    Q_OBJECT
    
public:
    explicit Tab2Widget(QWidget *parent = nullptr);
    ~Tab2Widget();

private slots:
#ifdef Q_OS_WIN
    void initComSubsystem();
    void initDevice();
    void fetchData();
    void stopFetching();
    void onTimeout();
    void ShowImage();
    void onComboSamplingRateChanged();

private:
    QComboBox *comboSamplingRate;
    QPushButton *btnInitCom;
    QPushButton *btnInitDevice;
    QPushButton *btnFetchData;
    QPushButton *btnStop;
    QCustomPlot *plot;
    QTimer *timer;

    std::vector <double> data;
#endif
};

#endif // TAB2WIDGET_H
