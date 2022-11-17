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
  Options::set_fetch_url(m_fetching_options->m_fetch_url->text());
  debugln("Saved fetch_url");
  Options::set_cli_location(m_fetching_options->m_cli_location->path());
  debugln("Saved cli_location");
  Options::set_auto_set_output(m_general_options->m_auto_set_output_check->isChecked());
  debugln("Saved auto_set_output");
  Options::set_generate_filename(m_general_options->m_generate_output_name_check->isChecked());
  debugln("Save generate_filename");
  debugln("Saved settings");
}

void OptionsWindow::handle_defaults() {
  m_fetching_options->m_fetch_url->setText(Options::default_fetch_url);
  m_fetching_options->m_cli_location->set_path(Options::default_cli_location());
  m_general_options->m_auto_set_output_check->setChecked(Options::default_auto_set_output);
  m_general_options->m_generate_output_name_check->setChecked(Options::default_generate_filename);
  debugln("Restored settings to default");
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
  layout()->addWidget(fetch_url_container);

  m_cli_location = new PathPicker(tr("CLI files location"), this);
  m_cli_location->set_path(Options::cli_location());
  m_cli_location->set_folder_mode();
  layout()->addWidget(m_cli_location);

  auto buttons_container = new QWidget(this);
  buttons_container->setLayout(new QHBoxLayout);
  m_redownload_button = new QPushButton(this);
  m_redownload_button->setText("Redownload");
  m_redownload_button->setFixedWidth(80);
  connect(m_redownload_button, &QPushButton::released, this, &FetchingOptions::redownload_files);
  buttons_container->layout()->addWidget(m_redownload_button);
  layout()->addWidget(buttons_container);
}

void FetchingOptions::redownload_files() {
  delete m_download_manager;
  m_download_manager = new DownloadManager(Options::fetch_url(), this);

  connect(m_download_manager, &DownloadManager::download_complete, this, &FetchingOptions::download_complete);
  m_download_manager->start_download();
}

void FetchingOptions::download_complete() {
  if (!m_download_manager) {
    debugln("No download manager present");
    return;
  }

  // TODO: Do something with the downloaded bytes
  auto info = QString("Download complete - %1 bytes").arg(m_download_manager->downloaded_bytes_count());
  logln(info);
}

GeneralOptions::GeneralOptions(QWidget* parent) : QGroupBox(parent) {
  setTitle("General");
  setLayout(new QVBoxLayout);

  auto auto_set_output_container = new QWidget(this);
  auto_set_output_container->setLayout(new QHBoxLayout);
  auto auto_set_output_check_label = new QLabel(auto_set_output_container);
  auto_set_output_check_label->setText("Automatically set output filepath: ");
  m_auto_set_output_check = new QCheckBox(auto_set_output_container);
  m_auto_set_output_check->setChecked(Options::auto_set_output());
  auto_set_output_container->layout()->addWidget(auto_set_output_check_label);
  auto_set_output_container->layout()->addWidget(m_auto_set_output_check);

  layout()->addWidget(auto_set_output_container);

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