#ifndef CONFIRM_DIALOG_H
#define CONFIRM_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPointer>
#include <QPushButton>

class ConfirmDialog final : public QDialog {
  Q_OBJECT
public:
  explicit ConfirmDialog(QWidget* parent);
  ~ConfirmDialog() final = default;

  void set_title(const QString&);
  void set_text(const QString&);
  void set_confirm_text(const QString&);
  void set_decline_text(const QString&);

private:
  QPointer<QLabel> m_text = nullptr;
  QPointer<QPushButton> m_confirm_button = nullptr;
  QPointer<QPushButton> m_decline_button = nullptr;
};


#endif// CONFIRM_DIALOG_H
