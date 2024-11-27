#ifndef TAB1WIDGET_H
#define TAB1WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QFileDialog>
#include <QComboBox>
#include <QMap>

class Tab1Widget : public QWidget {
    Q_OBJECT

public:
    explicit Tab1Widget(QWidget *parent = nullptr);
    ~Tab1Widget();

private slots:
    void browseLibraryPath();
    void loadLibrary();

private:
    QLineEdit *libraryNameEdit;
    QLineEdit *resultEdit;
    QDoubleSpinBox *paramXSpinBox;
    QDoubleSpinBox *paramYSpinBox;
    QComboBox *functionNameBox;

    QPushButton *loadLibraryButton;
    QPushButton *invokeFunctionButton;
    QPushButton *browseButton;

    QMap<QString, QString> functionNameMap;

#ifdef Q_OS_WIN
    HMODULE libraryHandle;
#else
    void *libraryHandle;
#endif

    void setupUi();
    void clearFunctionList();
    void invokeFunction();
};

#endif // TAB1WIDGET_H
