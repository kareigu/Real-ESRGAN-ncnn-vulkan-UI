#include "main_window.h"
#include "palette.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLineEdit>
#include <QThread>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {

  setMinimumSize(QSize(650, 490));
  setMaximumSize(QSize(750, 650));
  setWindowTitle("Real-ESRGAN-ncnn-Vulkan");


  m_menu_bar = new QMenuBar(this);
  m_options_window = new OptionsWindow();
  auto options_action = m_menu_bar->addAction(tr("&Options"));
  connect(options_action, &QAction::triggered, m_options_window, &OptionsWindow::show_window);
  m_about_window = new AboutWindow(this);
  m_menu_bar->addAction(tr("&About"), [&] { m_about_window->show(); });
  setMenuBar(m_menu_bar);

  m_main_view = new QWidget(this);

  m_main_controls = new QGroupBox(m_main_view);
  m_main_controls->setLayout(new QVBoxLayout);

  m_path_selects = new QWidget(m_main_controls);
  m_path_selects->setLayout(new QVBoxLayout);

  m_input_select = new PathPicker(tr("Input"));
  connect(m_input_select, &PathPicker::path_updated, this, &MainWindow::update_output_filepath);
  m_output_select = new PathPicker(tr("Output"));
  m_output_select->set_save_mode();

  m_path_selects->layout()->addWidget(m_input_select);
  m_path_selects->layout()->addWidget(m_output_select);

  m_main_buttons = new QWidget(m_main_controls);
  m_main_buttons->setLayout(new QHBoxLayout);
  m_main_buttons->layout()->setAlignment(Qt::AlignCenter);

  QSize button_size(60, 25);
  m_start_button = new QPushButton(m_main_buttons);
  m_start_button->setText("&Start");
  m_start_button->setPalette(primary_button_palette());
  m_start_button->setFixedSize(button_size);
  m_start_button->setDisabled(true);
  connect(m_start_button, &QPushButton::released, this, &MainWindow::start_processing);

  m_cancel_button = new QPushButton(m_main_buttons);
  m_cancel_button->setDisabled(true);
  m_cancel_button->setText("&Cancel");
  m_cancel_button->setFixedSize(button_size);
  connect(m_cancel_button, &QPushButton::released, this, &MainWindow::cancel_processing);


  m_main_buttons->layout()->addWidget(m_start_button);
  m_main_buttons->layout()->addWidget(m_cancel_button);

  m_main_controls->layout()->addWidget(m_path_selects);
  m_main_controls->layout()->addWidget(m_main_buttons);

  m_settings_panel = new SettingsPanel(m_main_view);
  connect(m_settings_panel, &SettingsPanel::settings_changed, this, &MainWindow::update_output_filepath);

  m_message_log = new MessageLog(m_main_view);
  Log::init(m_message_log);
#ifndef NDEBUG
  Log::set_log_level(Log::LogLevel::Debug);
#endif

  m_main_view->setLayout(new QVBoxLayout);
  m_main_view->layout()->addWidget(m_main_controls);
  m_main_view->layout()->addWidget(m_settings_panel);
  m_main_view->layout()->addWidget(m_message_log);
  setCentralWidget(m_main_view);

  logln("Ready");
}

void MainWindow::update_output_filepath() {
  if (!Options::auto_rename()) {
    debugln("Skipping automatic renaming");
    return;
  }

  auto path = m_input_select->path();
  if (path.isEmpty()) {
    m_start_button->setDisabled(true);
    return;
  }

  if (Options::generate_filename()) {
    QFileInfo file_info(path);
    QDir folder = file_info.dir();
    QString filename_ext = QString("%1_%2x")
                                   .arg(m_settings_panel->model(), m_settings_panel->up_size());
    QString filename = QString("%1_%2.%3")
                               .arg(file_info.baseName(), filename_ext, file_info.completeSuffix());
    m_output_select->set_path(folder.filePath(filename));
  } else {
    debugln("Skipping output name generation");
    m_output_select->set_path(path);
  }

  m_start_button->setDisabled(false);
}

void MainWindow::start_processing() {
  m_start_button->setDisabled(true);
  m_cancel_button->setDisabled(false);
  if (m_cli) {
    debugln("Already running");
    m_cancel_button->setDisabled(true);
    m_start_button->setDisabled(false);
    return;
  }

#ifdef WIN32
  QString program_path(Options::cli_location() + "/realesrgan-ncnn-vulkan.exe");
#else
  QString program_path(Options::cli_location() + "/realesrgan-ncnn-vulkan");
#endif

  QStringList arguments;
  arguments << "-i" << m_input_select->path()
            << "-o" << m_output_select->path()
            << "-n" << m_settings_panel->model()
            << "-s" << m_settings_panel->up_size();

  QFile program(program_path);

  if (!program.exists()) {
    debugln("CLI doesn't exist");
    debugln(program_path);
    logln(tr("Missing CLI files"));
    m_cancel_button->setDisabled(true);
    m_start_button->setDisabled(false);
    return;
  }

  m_cli = new QProcess();
  m_cli->setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);

  connect(m_cli, &QProcess::readyRead, this, [&]() {
    auto output = QString(m_cli->readAll()).split("\n");
    for (const auto& line : output)
      logln(line);
  });

  connect(m_cli, &QProcess::finished, this, [&]() {
    logln("Finished");
    delete m_cli;
    m_cli.clear();
    m_cancel_button->setDisabled(true);
    m_start_button->setDisabled(false);
  });

  m_cli->start(program_path, arguments);
}

void MainWindow::cancel_processing() {
  if (!m_cli) {
    debugln("Not running");
    return;
  }
  m_cli->kill();
  logln(tr("Cancelled"));

  m_cli.clear();
}

void MainWindow::closeEvent(QCloseEvent* event) {
  m_options_window->close();
  event->accept();
}