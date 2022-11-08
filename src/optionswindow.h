#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QCheckBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QMainWindow>
#include <QPointer>
#include <QPushButton>
#include <QWidget>
#include <QWindow>

class OptionsWindow;

class FetchingOptions final : public QGroupBox {
  Q_OBJECT
  friend class OptionsWindow;

public:
  explicit FetchingOptions(QWidget* parent);
  ~FetchingOptions() final = default;

private:
  QPointer<QLineEdit> m_fetch_url = nullptr;
  QPointer<QPushButton> m_redownload_button = nullptr;
};

class GeneralOptions final : public QGroupBox {
  Q_OBJECT
  friend class OptionsWindow;

public:
  GeneralOptions(QWidget* parent);
  ~GeneralOptions() final = default;

private:
  QPointer<QCheckBox> m_auto_rename_check = nullptr;
  QPointer<QCheckBox> m_generate_output_name_check = nullptr;
};

class OptionsWindow final : public QWidget {
  Q_OBJECT
public:
  OptionsWindow();
  ~OptionsWindow() final = default;

public slots:
  void handle_close();
  void handle_save();
  void handle_defaults();

private:
  QPointer<FetchingOptions> m_fetching_options = nullptr;
  QPointer<GeneralOptions> m_general_options = nullptr;

  QPointer<QPushButton> m_save_button = nullptr;
  QPointer<QPushButton> m_close_button = nullptr;
  QPointer<QPushButton> m_defaults_button = nullptr;
};

#endif// OPTIONSWINDOW_H
