#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QDialog>
#include <QPointer>
#include <QPushButton>
#include <QTextBrowser>

class AboutWindow final : public QDialog {
  Q_OBJECT
public:
  explicit AboutWindow(QWidget* parent);
  ~AboutWindow() final = default;

private:
  QPointer<QPushButton> m_close_button = nullptr;
  QPointer<QTextBrowser> m_about_text = nullptr;
};

#endif// ABOUTWINDOW_H
