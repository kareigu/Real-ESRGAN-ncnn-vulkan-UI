#include "options_window.h"
#include "message_log.h"
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

  // TODO: Implement dirty settings handling
  //       Wire up events to both options that get notified when changes happen
  //       Should compare to previously saved settings
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
  // FIXME: Uncomment this once dirty settings handling is implemented
  // m_save_button->setEnabled(false);
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
  // FIXME: Check for dirty settings and ask to save before closing
  close();
}

void OptionsWindow::handle_save() {
  // TODO: Implement saving the settings
  debugln("Saved settings (not)");
  Options::set_fetch_url(m_fetching_options->m_fetch_url->text());
  debugln("Saved fetch_url");
  Options::set_auto_rename(m_general_options->m_auto_rename_check->isChecked());
  debugln("Saved auto_rename");
  Options::set_generate_filename(m_general_options->m_generate_output_name_check->isChecked());
  debugln("Save generate_filename");
}

void OptionsWindow::handle_defaults() {
  // TODO: Implement resetting settings to default
  debugln("Restored settings to default (not)");
  m_fetching_options->m_fetch_url->setText(Options::default_fetch_url);
  m_general_options->m_auto_rename_check->setChecked(Options::default_auto_rename);
  m_general_options->m_generate_output_name_check->setChecked(Options::default_generate_filename);
}

void OptionsWindow::show_window() {
  if (isVisible()) {
    debugln("OptionsWindow visible");
    activateWindow();
  } else {
    debugln("Opening OptionsWindow");
    show();
  }
}

FetchingOptions::FetchingOptions(QWidget* parent) : QGroupBox(parent) {
  setTitle("Downloading");
  setLayout(new QVBoxLayout);
  auto fetch_url_container = new QWidget(this);
  fetch_url_container->setLayout(new QHBoxLayout);
  auto fetch_url_label = new QLabel(fetch_url_container);
  fetch_url_label->setText("Download URL: ");
  m_fetch_url = new QLineEdit(fetch_url_container);
  m_fetch_url->setText(Options::fetch_url());
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
  m_auto_rename_check->setChecked(Options::auto_rename());
  auto_rename_check_container->layout()->addWidget(auto_rename_check_label);
  auto_rename_check_container->layout()->addWidget(m_auto_rename_check);

  layout()->addWidget(auto_rename_check_container);

  auto generate_output_name_check_container = new QWidget(this);
  generate_output_name_check_container->setLayout(new QHBoxLayout);
  auto generate_output_name_check_label = new QLabel(generate_output_name_check_container);
  generate_output_name_check_label->setText("Automatically generate output filename: ");
  m_generate_output_name_check = new QCheckBox(generate_output_name_check_container);
  m_generate_output_name_check->setChecked(Options::generate_filename());
  generate_output_name_check_container->layout()->addWidget(generate_output_name_check_label);
  generate_output_name_check_container->layout()->addWidget(m_generate_output_name_check);

  layout()->addWidget(generate_output_name_check_container);
}