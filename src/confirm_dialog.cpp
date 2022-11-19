#include "confirm_dialog.h"
#include <QVBoxLayout>

ConfirmDialog::ConfirmDialog(QWidget* parent) : QDialog(parent) {
  setWindowTitle("Confirm");
  setLayout(new QVBoxLayout);
  setFixedSize(280, 120);

  m_text = new QLabel(this);
  m_text->setText("Confirm action");
  m_text->setAlignment(Qt::AlignCenter);
  layout()->addWidget(m_text);

  auto button_container = new QWidget(this);
  auto button_container_layout = new QHBoxLayout(button_container);
  button_container->setLayout(button_container_layout);
  m_confirm_button = new QPushButton(button_container);
  m_confirm_button->setText("Yes");
  connect(m_confirm_button, &QPushButton::released, this, &ConfirmDialog::accept);
  m_decline_button = new QPushButton(button_container);
  m_decline_button->setText("No");
  connect(m_decline_button, &QPushButton::released, this, &ConfirmDialog::reject);
  button_container_layout->addWidget(m_confirm_button);
  button_container_layout->addWidget(m_decline_button);
  layout()->addWidget(button_container);
}

void ConfirmDialog::set_title(const QString& title) {
  setWindowTitle(title);
}

void ConfirmDialog::set_text(const QString& text) {
  m_text->setText(text);
  m_text->adjustSize();
  m_text->resize(m_text->sizeHint());
  setFixedWidth(m_text->size().width() + 40);
}

void ConfirmDialog::set_confirm_text(const QString& text) {
  m_confirm_button->setText(text);
}

void ConfirmDialog::set_decline_text(const QString& text) {
  m_decline_button->setText(text);
}
