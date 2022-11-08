#ifndef PATHPICKER_H
#define PATHPICKER_H

#include <QLabel>
#include <QLineEdit>
#include <QPointer>
#include <QPushButton>
#include <QWidget>
#include <functional>

class PathPicker : public QWidget {
  Q_OBJECT
public:
  explicit PathPicker(const QString& title, QWidget* parent = nullptr);
  explicit PathPicker(QWidget* parent = nullptr);
  ~PathPicker() override = default;

  void set_mode(bool save) { m_save = save; }
  void set_save_mode() { m_save = true; }
  void set_open_mode() { m_save = false; }

  bool mode() { return m_save; }

  [[nodiscard]] QString path() const { return m_path->text(); }
  void set_path(const QString& path) { m_path->setText(path); }

  void run_select_callback() {
    if (m_select_callback && !m_path->text().isEmpty())
      m_select_callback(m_path->text());
  };
  void set_select_callback(std::function<void(const QString& path)> callback) { m_select_callback = callback; }

private:
  void m_init();

  bool m_save = false;

  std::function<void(const QString& path)> m_select_callback;

  QPointer<QLabel> m_title = nullptr;
  QPointer<QLineEdit> m_path = nullptr;
  QPointer<QPushButton> m_browse_button = nullptr;
};


#endif