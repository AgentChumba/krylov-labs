#ifndef TAB3WIDGET_H
#define TAB3WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>

class Tab3Widget : public QWidget {
    Q_OBJECT

public:
    explicit Tab3Widget(QWidget *parent = nullptr);
    ~Tab3Widget();

private slots:
    void onSearchButtonClicked();

private:
    QLineEdit *sectionInput;
    QTextEdit *resultOutput;
};

#endif // TAB3WIDGET_H
