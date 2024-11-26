#ifndef TAB2WIDGET_H
#define TAB2WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>

#ifdef Q_OS_LINUX
#include <libusb-1.0/libusb.h>
#endif

class Tab2Widget : public QWidget {
    Q_OBJECT

public:
    explicit Tab2Widget(QWidget *parent = nullptr);
    ~Tab2Widget();

private slots:
    void onStartClicked();

private:
    QLabel *titleLabel;
    QPushButton *startButton;
    QListWidget *mouseList;
    QListWidget *keyboardList;
    QListWidget *hidList;

    void populateUsbDevices();
};

#endif // TAB2WIDGET_H
