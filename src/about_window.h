#ifndef ABOUT_WINDOW_H
#define ABOUT_WINDOW_H

#include <QDialog>
#include <QLabel>
#include <QPointer>
#include <QPushButton>
#include <QTextBrowser>

class AboutWindow final : public QDialog {
  Q_OBJECT
public:
  explicit AboutWindow(QWidget* parent);
  ~AboutWindow() final = default;

private slots:
  void handle_close();

private:
  QPointer<QPushButton> m_close_button = nullptr;
  QPointer<QPushButton> m_about_qt_button = nullptr;
  QPointer<QTextBrowser> m_about_text = nullptr;
  QPointer<QLabel> m_header = nullptr;
};

#endif// ABOUT_WINDOW_H
