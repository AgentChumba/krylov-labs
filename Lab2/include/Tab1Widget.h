#ifndef TAB1WIDGET_H
#define TAB1WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QFileDialog>

class Tab1Widget : public QWidget {
    Q_OBJECT

public:
    explicit Tab1Widget(QWidget *parent = nullptr);
    ~Tab1Widget();

private slots:
    void browseLibraryPath();

private:
    QLineEdit *libraryNameEdit;
    QLineEdit *functionNameEdit;
    QLineEdit *resultEdit;
    QSpinBox *paramXSpinBox;
    QSpinBox *paramYSpinBox;

    QPushButton *checkLibraryButton;
    QPushButton *checkFunctionButton;
    QPushButton *invokeFunctionButton;
    QPushButton *browseButton;

    void setupUi();
};

#endif // TAB1WIDGET_H
