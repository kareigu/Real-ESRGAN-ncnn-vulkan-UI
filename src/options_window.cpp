#include "options_window.h"
#include "confirm_dialog.h"
#include "message_log.h"
#include "palette.h"
#include <QBoxLayout>
#include <QGroupBox>
#include <QLabel>

OptionsWindow::OptionsWindow() : QWidget() {
  setWindowTitle("Options");
  setMinimumSize(QSize(600, 450));
  setMaximumSize(QSize(800, 600));
  setAttribute(Qt::WA_DeleteOnClose);

  auto main_layout = new QVBoxLayout();
  setLayout(main_layout);

  auto settings_container = new QWidget(this);
  settings_container->setLayout(new QVBoxLayout);

  m_fetching_options = new FetchingOptions(settings_container);
  connect(m_fetching_options, &FetchingOptions::options_changed, this, &OptionsWindow::handle_dirty_check);
  settings_container->layout()->addWidget(m_fetching_options);
  m_general_options = new GeneralOptions(settings_container);
  connect(m_general_options, &GeneralOptions::options_changed, this, &OptionsWindow::handle_dirty_check);
  settings_container->layout()->addWidget(m_general_options);

  auto button_container = new QWidget(this);
  auto button_container_layout = new QHBoxLayout();
  button_container_layout->addStretch();
  button_container->setLayout(button_container_layout);
  m_close_button = new QPushButton(button_container);
  m_close_button->setText("Close");
  connect(m_close_button, &QPushButton::released, this, &OptionsWindow::close);

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

bool OptionsWindow::handle_close() {
  if (!m_dirty_settings) {
    return true;
  }

  auto confirm_dialog = new ConfirmDialog(this);
  confirm_dialog->set_title("Close without saving?");
  confirm_dialog->set_text("You've made unsaved changes, do you want to close the options?");
  int result = confirm_dialog->exec();
  delete confirm_dialog;

  debugln(QString("Confirm result: %1").arg(result));

  return result == 1;
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
  handle_dirty_check();
  debugln("Saved settings");
}

void OptionsWindow::handle_defaults() {
  m_fetching_options->m_fetch_url->setText(Options::default_fetch_url);
  m_fetching_options->m_cli_location->set_path(Options::default_cli_location());
  m_general_options->m_auto_set_output_check->setChecked(Options::default_auto_set_output);
  m_general_options->m_generate_output_name_check->setChecked(Options::default_generate_filename);
  handle_dirty_check();
  debugln("Restored settings to default");
}

void OptionsWindow::handle_dirty_check(Options::Types option_type) {
  switch (option_type) {
    case Options::FETCH_URL:
      m_dirty_settings.set_dirty<Options::FETCH_URL>(m_fetching_options->m_fetch_url->text() != Options::fetch_url());
    case Options::CLI_LOCATION:
      m_dirty_settings.set_dirty<Options::CLI_LOCATION>(m_fetching_options->m_cli_location->path() != Options::cli_location());
    case Options::AUTO_SET_OUTPUT:
      m_dirty_settings.set_dirty<Options::AUTO_SET_OUTPUT>(m_general_options->m_auto_set_output_check->isChecked() != Options::auto_set_output());
    case Options::GENERATE_FILENAME:
      m_dirty_settings.set_dirty<Options::GENERATE_FILENAME>(m_general_options->m_generate_output_name_check->isChecked() != Options::generate_filename());
    default:
      debugln("Checking all options for dirty");
      m_dirty_settings.set_dirty<Options::FETCH_URL>(m_fetching_options->m_fetch_url->text() != Options::fetch_url());
      m_dirty_settings.set_dirty<Options::CLI_LOCATION>(m_fetching_options->m_cli_location->path() != Options::cli_location());
      m_dirty_settings.set_dirty<Options::AUTO_SET_OUTPUT>(m_general_options->m_auto_set_output_check->isChecked() != Options::auto_set_output());
      m_dirty_settings.set_dirty<Options::GENERATE_FILENAME>(m_general_options->m_generate_output_name_check->isChecked() != Options::generate_filename());
      break;
  }

  debugln(m_dirty_settings ? "Setting dirty" : "Settings clean");
  m_save_button->setEnabled(m_dirty_settings);
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
  connect(m_fetch_url, &QLineEdit::editingFinished, this, [&] { emit options_changed(Options::FETCH_URL); });
  fetch_url_container->layout()->addWidget(fetch_url_label);
  fetch_url_container->layout()->addWidget(m_fetch_url);
  layout()->addWidget(fetch_url_container);

  m_cli_location = new PathPicker(tr("CLI files location"), this);
  m_cli_location->set_path(Options::cli_location());
  m_cli_location->set_folder_mode();
  connect(m_cli_location, &PathPicker::path_updated, this, [&] { emit options_changed(Options::CLI_LOCATION); });
  layout()->addWidget(m_cli_location);
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
  connect(m_auto_set_output_check, &QCheckBox::stateChanged, this, [&] { emit options_changed(Options::AUTO_SET_OUTPUT); });
  auto_set_output_container->layout()->addWidget(auto_set_output_check_label);
  auto_set_output_container->layout()->addWidget(m_auto_set_output_check);

  layout()->addWidget(auto_set_output_container);

  auto generate_output_name_check_container = new QWidget(this);
  generate_output_name_check_container->setLayout(new QHBoxLayout);
  auto generate_output_name_check_label = new QLabel(generate_output_name_check_container);
  generate_output_name_check_label->setText("Automatically generate output filename: ");
  m_generate_output_name_check = new QCheckBox(generate_output_name_check_container);
  m_generate_output_name_check->setChecked(Options::generate_filename());
  connect(m_generate_output_name_check, &QCheckBox::stateChanged, this, [&] { emit options_changed(Options::GENERATE_FILENAME); });
  generate_output_name_check_container->layout()->addWidget(generate_output_name_check_label);
  generate_output_name_check_container->layout()->addWidget(m_generate_output_name_check);

  layout()->addWidget(generate_output_name_check_container);
}