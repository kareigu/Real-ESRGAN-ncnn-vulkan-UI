#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QLineEdit>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent) {

  setMinimumSize(QSize(650, 450));
  setMaximumSize(QSize(750, 650));
  setWindowTitle("realesrgan-nccn-vulkan");

  m_main_view = new QWidget(this);

  m_main_controls = new QGroupBox(m_main_view);
  m_main_controls->setLayout(new QVBoxLayout);

  m_path_selects = new QWidget(m_main_controls);
  m_path_selects->setLayout(new QVBoxLayout);

  m_input_select = new PathPicker(tr("Input"));
  m_output_select = new PathPicker(tr("Output"));
  m_output_select->set_save_mode();
  m_input_select->set_select_callback([&](const QString& path) {
    QFileInfo file_info(path);
    QDir folder = file_info.dir();
    QString filename_ext = QString("%1_%2x")
      .arg(m_settings_panel->model())
      .arg(m_settings_panel->up_size());
    QString filename = QString("%1_%2.%3")
      .arg(file_info.baseName())
      .arg(filename_ext)
      .arg(file_info.completeSuffix());
    m_output_select->set_path(folder.filePath(filename));
  });

  m_path_selects->layout()->addWidget(m_input_select);
  m_path_selects->layout()->addWidget(m_output_select);

  m_main_buttons = new QWidget(m_main_controls);
  m_main_buttons->setLayout(new QHBoxLayout);
  m_main_buttons->layout()->setAlignment(Qt::AlignCenter);

  QSize button_size(60, 25);
  m_start_button = new QPushButton(m_main_buttons);
  m_start_button->setText("&Start");
  m_start_button->setFixedSize(button_size);
  connect(m_start_button, &QPushButton::released, this, [&]() {
    if (m_cli) {
      debugln("Already running");
      return;
    }
    m_cli = new QProcess();

#ifdef WIN32
    QString program_path(QCoreApplication::applicationDirPath() + "/cli/realesrgan-ncnn-vulkan.exe");
#else
    QString program_path(QCoreApplication::applicationDirPath() + "cli/realesrgan-ncnn-vulkan");
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
      return;
    }

    m_cli->setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);

    connect(m_cli, &QProcess::readyRead, this, [&]() {
      auto output = QString(m_cli->readAll()).split("\n");
      for (const auto& line : output)
        logln(line);
    });

    connect(m_cli, &QProcess::finished, this, [&]() {
      logln("Finished");
      delete m_cli;
      m_cli = nullptr;
    });

    m_cli->start(program_path, arguments);
  });

  m_cancel_button = new QPushButton(m_main_buttons);
  m_cancel_button->setText("&Cancel");
  m_cancel_button->setFixedSize(button_size);
  connect(m_cancel_button, &QPushButton::released, this, [&]() {
    if (!m_cli) {
      debugln("Not running");
      return;
    }
    m_cli->kill();
    logln(tr("Cancelled"));

    m_cli = nullptr;
  });

  
  m_main_buttons->layout()->addWidget(m_start_button);
  m_main_buttons->layout()->addWidget(m_cancel_button);

  m_main_controls->layout()->addWidget(m_path_selects);
  m_main_controls->layout()->addWidget(m_main_buttons);

  m_settings_panel = new SettingsPanel(m_main_view);

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

MainWindow::~MainWindow() {

}

