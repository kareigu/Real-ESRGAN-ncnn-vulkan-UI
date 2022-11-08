#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QPushButton>
#include <QWidget>
#include <QWindow>

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
  QPointer<QPushButton> m_save_button = nullptr;
  QPointer<QPushButton> m_close_button = nullptr;
  QPointer<QPushButton> m_defaults_button = nullptr;
};

#endif// OPTIONSWINDOW_H
