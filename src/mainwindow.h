#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QGroupBox>
#include <QTextEdit>

#include "messagelog.h"
#include "pathpicker.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  QPointer<QWidget> m_main_view = nullptr;

  QPointer<QGroupBox> m_path_selects = nullptr;
  QPointer<PathPicker> m_input_select = nullptr;
  QPointer<PathPicker> m_output_select = nullptr;

  QPointer<QGroupBox> m_settings_box = nullptr;

  QPointer<MessageLog> m_message_log = nullptr;
};
#endif // MAINWINDOW_H
