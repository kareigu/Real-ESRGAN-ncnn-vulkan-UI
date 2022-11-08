#include "optionswindow.h"
#include "messagelog.h"
#include "palette.h"
#include <QBoxLayout>
#include <QSpacerItem>

OptionsWindow::OptionsWindow() : QWidget() {
  setWindowTitle("Options");
  setMinimumSize(QSize(400, 450));
  setMaximumSize(QSize(450, 600));

  auto main_layout = new QVBoxLayout();
  setLayout(main_layout);

  auto settings_container = new QWidget(this);
  settings_container->setLayout(new QVBoxLayout);


  auto button_container = new QWidget(this);
  button_container->setLayout(new QHBoxLayout);
  m_close_button = new QPushButton(button_container);
  m_close_button->setText("Close");
  connect(m_close_button, &QPushButton::released, this, &OptionsWindow::handle_close);

  m_save_button = new QPushButton(button_container);
  m_save_button->setText("Save");
  m_save_button->setPalette(primary_button_palette());
  m_save_button->setEnabled(false);
  connect(m_save_button, &QPushButton::released, this, &OptionsWindow::handle_save);

  m_defaults_button = new QPushButton(button_container);
  m_defaults_button->setText("Defaults");
  connect(m_defaults_button, &QPushButton::released, this, &OptionsWindow::handle_defaults);
  button_container->layout()->addWidget(m_close_button);
  button_container->layout()->addWidget(m_save_button);
  button_container->layout()->addWidget(m_defaults_button);

  main_layout->addWidget(settings_container);
  main_layout->addStretch();
  main_layout->addWidget(button_container);
}

void OptionsWindow::handle_close() {
  close();
}

void OptionsWindow::handle_save() {
  debugln("Saved settings (not)");
}

void OptionsWindow::handle_defaults() {
  debugln("Restored settings to default (not)");
}