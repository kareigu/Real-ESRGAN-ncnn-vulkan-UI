#ifndef OPTIONS_WINDOW_H
#define OPTIONS_WINDOW_H

#include "download_manager.h"
#include "options.h"
#include "path_picker.h"
#include <QCheckBox>
#include <QCloseEvent>
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

signals:
  void options_changed(Options::Types);

private:
  QPointer<QLineEdit> m_fetch_url = nullptr;
  QPointer<PathPicker> m_cli_location = nullptr;
};

class GeneralOptions final : public QGroupBox {
  Q_OBJECT
  friend class OptionsWindow;

public:
  explicit GeneralOptions(QWidget* parent);
  ~GeneralOptions() final = default;

signals:
  void options_changed(Options::Types);

private:
  QPointer<QCheckBox> m_auto_set_output_check = nullptr;
  QPointer<QCheckBox> m_generate_output_name_check = nullptr;
};

class OptionsWindow final : public QWidget {
  Q_OBJECT
public:
  OptionsWindow();
  ~OptionsWindow() final = default;

  void closeEvent(QCloseEvent* event) final {
    if (handle_close())
      event->accept();
    else
      event->ignore();
  }

public slots:
  bool handle_close();
  void handle_save();
  void handle_defaults();
  void handle_dirty_check(Options::Types option_type = Options::Types::COUNT);
  void show_window();

private:
  class DirtySettingsContainer {
  public:
    DirtySettingsContainer() {
      for (bool& value : m_values)
        value = false;
    }

    template<Options::Types OptionType>
    void set_dirty(bool dirty) {
      m_values[static_cast<size_t>(OptionType)] = dirty;
    }

    operator bool() const {
      bool is_dirty = false;
      for (const bool& value : m_values)
        if (value)
          is_dirty = true;

      return is_dirty;
    }

  private:
    bool m_values[static_cast<size_t>(Options::Types::COUNT)]{};
  };

  DirtySettingsContainer m_dirty_settings{};

  QPointer<FetchingOptions> m_fetching_options = nullptr;
  QPointer<GeneralOptions> m_general_options = nullptr;

  QPointer<QPushButton> m_save_button = nullptr;
  QPointer<QPushButton> m_close_button = nullptr;
  QPointer<QPushButton> m_defaults_button = nullptr;
};

#endif// OPTIONS_WINDOW_H
