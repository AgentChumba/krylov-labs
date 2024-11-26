#ifndef TAB1WIDGET_H
#define TAB1WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QRadioButton>
#include <QPushButton>
#include <QSerialPort>
#include <QSerialPortInfo>

class Tab1Widget : public QWidget {
    Q_OBJECT

public:
    explicit Tab1Widget(QWidget *parent = nullptr);
    ~Tab1Widget();

private:
    QRadioButton *com1Button;
    QRadioButton *com2Button;
    QRadioButton *com3Button;
    QRadioButton *com4Button;
    QRadioButton *customPortButton;

    QSpinBox *customPortSpin;

    QPushButton *sendSignalButton;

    QLineEdit *resultField;
    QLineEdit *sensorLineEdit;
    QLineEdit *registerLineEdit;

    QComboBox *speedCombo;
    QComboBox *dataBitsCombo;
    QComboBox *stopBitsCombo;
    QComboBox *parityCombo;

    QSerialPort *serialPort = nullptr;

private slots:
    void onInitPortClicked();
    void onDisconnectPortClicked();
    void onSendSignal();
};

#endif // TAB1WIDGET_H
