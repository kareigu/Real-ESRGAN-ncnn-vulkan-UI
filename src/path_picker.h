#ifndef PATH_PICKER_H
#define PATH_PICKER_H

#include <QLabel>
#include <QLineEdit>
#include <QPointer>
#include <QPushButton>
#include <QWidget>
#include <functional>

class PathPicker : public QWidget {
  Q_OBJECT
public:
  enum class PathPickerMode {
    SaveFile,
    OpenFile,
    SelectFolder,
  };

  inline static const char* path_picker_mode_strings[3] = {
          "SaveFile",
          "OpenFile",
          "SelectFolder",
  };

  using PathPickerMode::OpenFile;
  using PathPickerMode::SaveFile;
  using PathPickerMode::SelectFolder;

  explicit PathPicker(const QString& title, QWidget* parent = nullptr);
  explicit PathPicker(QWidget* parent = nullptr);
  ~PathPicker() override = default;

  void set_to_open_mode() { m_mode = OpenFile; }
  void set_to_save_mode() { m_mode = SaveFile; }
  void set_to_folder_mode() { m_mode = SelectFolder; }

  PathPickerMode mode() { return m_mode; }

  [[nodiscard]] QString path() const { return m_path->text(); }
  void set_path(const QString& path) { m_path->setText(path); }

signals:
  void path_updated();

private:
  void m_init();

  PathPickerMode m_mode = OpenFile;

  QPointer<QLabel> m_title = nullptr;
  QPointer<QLineEdit> m_path = nullptr;
  QPointer<QPushButton> m_browse_button = nullptr;
};


#endif// PATH_PICKER_H