#include "optionswindow.h"
#include "messagelog.h"
#include "palette.h"
#include <QBoxLayout>
#include <QGroupBox>
#include <QLabel>

OptionsWindow::OptionsWindow() : QWidget() {
  setWindowTitle("Options");
  setMinimumSize(QSize(600, 450));
  setMaximumSize(QSize(800, 600));

  auto main_layout = new QVBoxLayout();
  setLayout(main_layout);

  auto settings_container = new QWidget(this);
  settings_container->setLayout(new QVBoxLayout);

  m_fetching_options = new FetchingOptions(settings_container);
  settings_container->layout()->addWidget(m_fetching_options);
  m_general_options = new GeneralOptions(settings_container);
  settings_container->layout()->addWidget(m_general_options);

  auto button_container = new QWidget(this);
  auto button_container_layout = new QHBoxLayout();
  button_container_layout->addStretch();
  button_container->setLayout(button_container_layout);
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

FetchingOptions::FetchingOptions(QWidget* parent) : QGroupBox(parent) {
  setTitle("Downloading");
  setLayout(new QVBoxLayout);
  auto fetch_url_container = new QWidget(this);
  fetch_url_container->setLayout(new QHBoxLayout);
  auto fetch_url_label = new QLabel(fetch_url_container);
  fetch_url_label->setText("Download URL: ");
  m_fetch_url = new QLineEdit(fetch_url_container);
  m_fetch_url->setText("https://github.com/xinntao/Real-ESRGAN/releases/download/v0.2.5.0/realesrgan-ncnn-vulkan-20220424-windows.zip");
  fetch_url_container->layout()->addWidget(fetch_url_label);
  fetch_url_container->layout()->addWidget(m_fetch_url);

  auto buttons_container = new QWidget(this);
  buttons_container->setLayout(new QHBoxLayout);
  m_redownload_button = new QPushButton(this);
  m_redownload_button->setText("Redownload");
  m_redownload_button->setFixedWidth(80);
  connect(m_redownload_button, &QPushButton::released, this, [] { debugln("Redownloading CLI (not)"); });
  buttons_container->layout()->addWidget(m_redownload_button);
  layout()->addWidget(fetch_url_container);
  layout()->addWidget(buttons_container);
}

GeneralOptions::GeneralOptions(QWidget* parent) : QGroupBox(parent) {
  setTitle("General");
  setLayout(new QVBoxLayout);

  auto auto_rename_check_container = new QWidget(this);
  auto_rename_check_container->setLayout(new QHBoxLayout);
  auto auto_rename_check_label = new QLabel(auto_rename_check_container);
  auto_rename_check_label->setText("Automatically rename output file: ");
  m_auto_rename_check = new QCheckBox(auto_rename_check_container);
  auto_rename_check_container->layout()->addWidget(auto_rename_check_label);
  auto_rename_check_container->layout()->addWidget(m_auto_rename_check);

  layout()->addWidget(auto_rename_check_container);

  auto generate_output_name_check_container = new QWidget(this);
  generate_output_name_check_container->setLayout(new QHBoxLayout);
  auto generate_output_name_check_label = new QLabel(generate_output_name_check_container);
  generate_output_name_check_label->setText("Automatically generate output filename: ");
  m_generate_output_name_check = new QCheckBox(generate_output_name_check_container);
  generate_output_name_check_container->layout()->addWidget(generate_output_name_check_label);
  generate_output_name_check_container->layout()->addWidget(m_generate_output_name_check);

  layout()->addWidget(generate_output_name_check_container);
}