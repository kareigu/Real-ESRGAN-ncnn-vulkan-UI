#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QCloseEvent>
#include <QDialog>
#include <QGroupBox>
#include <QMainWindow>
#include <QMenuBar>
#include <QPointer>
#include <QProcess>
#include <QPushButton>
#include <QTextEdit>

#include "about_window.h"
#include "message_log.h"
#include "options_window.h"
#include "path_picker.h"
#include "settings_panel.h"

class MainWindow final : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() final = default;

  void closeEvent(QCloseEvent* event) override;

private slots:
  void update_output_filepath();
  void start_processing();
  void cancel_processing();

private:
  QPointer<QProcess> m_cli = nullptr;

  QPointer<QWidget> m_main_view = nullptr;

  QPointer<QMenuBar> m_menu_bar = nullptr;
  QPointer<AboutWindow> m_about_window = nullptr;
  QPointer<OptionsWindow> m_options_window = nullptr;

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
#endif// MAIN_WINDOW_H
