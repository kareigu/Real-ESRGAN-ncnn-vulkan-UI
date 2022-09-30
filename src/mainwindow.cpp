#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent) {

  setMinimumSize(QSize(650, 300));
  setMaximumSize(QSize(750, 300));
  setWindowTitle("realesrgan-nccn-vulkan");

  m_main_view = new QWidget(this);

  m_path_selects = new QGroupBox(m_main_view);
  m_path_selects->setLayout(new QVBoxLayout);
  m_input_select = new PathPicker(tr("Input"));
  m_output_select = new PathPicker(tr("Output"));
  m_path_selects->layout()->addWidget(m_input_select);
  m_path_selects->layout()->addWidget(m_output_select);

  m_settings_box = new QGroupBox(m_main_view);

  m_message_log = new MessageLog(m_main_view);
  Log::init(m_message_log);
#ifndef NDEBUG
  Log::set_log_level(Log::LogLevel::Debug);
#endif

  m_main_view->setLayout(new QVBoxLayout);
  m_main_view->layout()->addWidget(m_path_selects);
  m_main_view->layout()->addWidget(m_settings_box);
  m_main_view->layout()->addWidget(m_message_log);
  setCentralWidget(m_main_view);

  logln("Started");
}

MainWindow::~MainWindow() {

}

