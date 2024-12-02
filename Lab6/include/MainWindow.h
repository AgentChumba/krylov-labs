#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QLineEdit *portNameEdit;
    QLineEdit *byteInput;
    QPushButton *btnOpenSync;
    QPushButton *btnOpenAsync;
    QPushButton *btnClose;
    QPushButton *btnSend;
    QTextEdit *logView;

    QSerialPort *serialPort;
    QTimer *sendTimer;
    QByteArray longData;
    int longDataIndex;

    enum Mode { None, Sync, Async };
    Mode currentMode;

    void logMessage(const QString &message);

private slots:
    void openPortSync();
    void openPortAsync();
    void closePort();

    void sendData();
    void sendDataSync(const QByteArray &data);
    void sendDataAsync(const QByteArray &data);
    void sendNextByte();
    void onReadyRead();
    void setUIEnabled(bool enabled);
};

#endif // MAINWINDOW_H
