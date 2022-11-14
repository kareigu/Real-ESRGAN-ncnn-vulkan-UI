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
  explicit PathPicker(const QString& title, QWidget* parent = nullptr);
  explicit PathPicker(QWidget* parent = nullptr);
  ~PathPicker() override = default;

  void set_mode(bool save) { m_save = save; }
  void set_save_mode() { m_save = true; }
  void set_open_mode() { m_save = false; }

  bool mode() { return m_save; }

  [[nodiscard]] QString path() const { return m_path->text(); }
  void set_path(const QString& path) { m_path->setText(path); }

signals:
  void path_updated();

private:
  void m_init();

  bool m_save = false;

  QPointer<QLabel> m_title = nullptr;
  QPointer<QLineEdit> m_path = nullptr;
  QPointer<QPushButton> m_browse_button = nullptr;
};


#endif// PATH_PICKER_H