#ifndef TAB1WIDGET_H
#define TAB1WIDGET_H

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QPushButton>
#include <QWidget>

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
  HMODULE libraryHandle = nullptr;
#else
  void *libraryHandle = nullptr;
#endif

  void setupUi();
  void clearFunctionList();
  void invokeFunction();
};

#endif // TAB1WIDGET_H
