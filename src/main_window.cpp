#include "main_window.h"
#include "confirm_dialog.h"
#include "palette.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLineEdit>
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {

  setMinimumSize(QSize(650, 490));
  setMaximumSize(QSize(750, 650));
  setWindowTitle("Real-ESRGAN-ncnn-Vulkan");


  m_menu_bar = new QMenuBar(this);
  auto file_menu = m_menu_bar->addMenu(tr("&File"));
  auto redownload_action = file_menu->addAction(tr("Redownload CLI"));
  connect(redownload_action, &QAction::triggered, this, [&] {
    if (!m_download_manager)
      m_download_manager = new DownloadManager(this);

    m_download_manager->start_download();
  });

  auto queue_action = file_menu->addAction(tr("Queue"));
  connect(queue_action, &QAction::triggered, this, [&] {
    if (!m_queue_window)
      m_queue_window = new QueueWindow();

    m_queue_window->show_window();
  });

  auto options_action = file_menu->addAction(tr("&Options"));
  connect(options_action, &QAction::triggered, this, &MainWindow::open_options_window);

  auto help_menu = m_menu_bar->addMenu(tr("&Help"));
  auto about_action = help_menu->addAction(tr("&About"));
  connect(about_action, &QAction::triggered, this, &MainWindow::open_about_window);
  setMenuBar(m_menu_bar);

  m_main_view = new QWidget(this);

  m_main_controls = new QGroupBox(m_main_view);
  m_main_controls->setLayout(new QVBoxLayout);

  m_path_selects = new QWidget(m_main_controls);
  m_path_selects->setLayout(new QVBoxLayout);
  m_main_controls->layout()->addWidget(m_path_selects);

  m_input_select = new PathPicker(tr("Input"));
  connect(m_input_select, &PathPicker::path_updated, this, &MainWindow::update_output_filepath);
  m_output_select = new PathPicker(tr("Output"));
  m_output_select->set_save_mode();
  connect(m_output_select, &PathPicker::path_updated, this, [&] { update_start_button(); });

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

  auto add_to_queue_button = new QPushButton(m_main_buttons);
  add_to_queue_button->setText(tr("Add to queue"));
  add_to_queue_button->setFixedHeight(button_size.height());
  add_to_queue_button->setFixedWidth(110);
  connect(add_to_queue_button, &QPushButton::released, this, &MainWindow::add_to_queue);


  m_main_buttons->layout()->addWidget(m_start_button);
  m_main_buttons->layout()->addWidget(add_to_queue_button);
  m_main_buttons->layout()->addWidget(m_cancel_button);

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

  connect(this, &MainWindow::cli_files_missing, this, &MainWindow::ask_to_download_cli);

  logln("Ready");

  if (!check_for_cli())
    QTimer::singleShot(500, Qt::TimerType::CoarseTimer, [&] { emit cli_files_missing(); });
}

void MainWindow::update_output_filepath() {
  if (!Options::auto_set_output()) {
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

  update_start_button();
}

void MainWindow::start_processing() {
  m_start_button->setDisabled(true);
  m_cancel_button->setDisabled(false);
  if (m_cli) {
    debugln("Already running");
    m_cancel_button->setDisabled(true);
    update_start_button();
    return;
  }


  QString program_path(Options::cli_location() + PLATFORM_EXECUTABLE_NAME);

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
    update_start_button();
    ask_to_download_cli();
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
    update_start_button();
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
  if (m_options_window)
    m_options_window->close();
  event->accept();
}

void MainWindow::update_start_button() {
  m_start_button->setDisabled(m_input_select->path().isEmpty() || m_output_select->path().isEmpty());
}

void MainWindow::open_options_window() {
  if (!m_options_window)
    m_options_window = new OptionsWindow();

  m_options_window->show_window();
}

void MainWindow::open_about_window() {
  if (!m_about_window)
    m_about_window = new AboutWindow(this);

  m_about_window->show();
}

bool MainWindow::check_for_cli() {
  auto cli_folder = Options::cli_location();
  auto executable = QFile(cli_folder + PLATFORM_EXECUTABLE_NAME);
  auto models_folder = QDir(cli_folder + "/models");

  return executable.exists() && models_folder.exists();
}

void MainWindow::ask_to_download_cli() {
  auto confirm_dialog = new ConfirmDialog(this);
  confirm_dialog->set_title("CLI missing");
  confirm_dialog->set_text("You're missing required files to use this software.\nWould you like to download them?");
  int result = confirm_dialog->exec();
  delete confirm_dialog;

  if (result != 1)
    return;

  if (!m_download_manager)
    m_download_manager = new DownloadManager(this);

  m_download_manager->start_download();
}
void MainWindow::add_to_queue() {
  auto queue_item = Queue::Item{
          .input_path = m_input_select->path(),
          .output_path = m_output_select->path(),
          .model = m_settings_panel->raw_model(),
          .size = m_settings_panel->raw_up_size()};
  Queue::add(queue_item);
}
