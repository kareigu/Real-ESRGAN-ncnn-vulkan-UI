#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QGroupBox>

#include "pathpicker.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  QPointer<QGroupBox> m_main_view = nullptr;

  QPointer<PathPicker> m_input_select = nullptr;
  QPointer<PathPicker> m_output_select = nullptr;

};
#endif // MAINWINDOW_H
