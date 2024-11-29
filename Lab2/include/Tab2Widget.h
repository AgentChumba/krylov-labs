#ifndef TAB2WIDGET_H
#define TAB2WIDGET_H

#include <QProcess>

#ifdef Q_OS_WIN
#include <QCustomPlot/qcustomplot.h>
#else
#include <qcustomplot.h>
#endif

class Tab2Widget : public QWidget {
    Q_OBJECT
    
public:
    explicit Tab2Widget(QWidget *parent = nullptr);
    ~Tab2Widget();

private slots:
    void initComSubsystem();
    void initDevice();
    void fetchData();
    void stopFetching();

private:
    QComboBox *comboSamplingRate;
    QPushButton *btnInitCom;
    QPushButton *btnInitDevice;
    QPushButton *btnFetchData;
    QPushButton *btnStop;
    QCustomPlot *plot;
};

#endif // TAB2WIDGET_H
