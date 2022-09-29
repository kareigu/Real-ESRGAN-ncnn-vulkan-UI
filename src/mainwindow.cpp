#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent) {

  this->setMinimumSize(QSize(500, 230));
  this->setWindowTitle("realesrgan-nccn-vulkan");

  m_main_view = new QGroupBox();

  m_input_select = new PathPicker();
  m_output_select = new PathPicker();

  m_main_view->setLayout(new QVBoxLayout);
  m_main_view->layout()->addWidget(m_input_select);
  m_main_view->layout()->addWidget(m_output_select);
  this->setCentralWidget(m_main_view);
}

MainWindow::~MainWindow() {
}

