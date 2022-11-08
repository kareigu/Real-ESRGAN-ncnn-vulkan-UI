#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGroupBox>
#include <QMainWindow>
#include <QPointer>
#include <QProcess>
#include <QPushButton>
#include <QTextEdit>

#include "messagelog.h"
#include "pathpicker.h"
#include "settingspanel.h"

class MainWindow final : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() final = default;

private:
  QPointer<QProcess> m_cli = nullptr;

  QPointer<QWidget> m_main_view = nullptr;

  QPointer<QGroupBox> m_main_controls = nullptr;
  QPointer<QWidget> m_path_selects = nullptr;
  QPointer<PathPicker> m_input_select = nullptr;
  QPointer<PathPicker> m_output_select = nullptr;

  QPointer<QWidget> m_main_buttons = nullptr;
  QPointer<QPushButton> m_start_button = nullptr;
  QPointer<QPushButton> m_cancel_button = nullptr;

  QPointer<SettingsPanel> m_settings_panel = nullptr;

  QPointer<MessageLog> m_message_log = nullptr;
};
#endif// MAINWINDOW_H
