#include "about_window.h"
#include "message_log.h"
#include "palette.h"
#include <QApplication>
#include <QFile>
#include <QVBoxLayout>

#ifndef PROJECT_VERSION
  #define PROJECT_VERSION "unknown version"
#endif

AboutWindow::AboutWindow(QWidget* parent) : QDialog(parent) {
  setWindowTitle(tr("About"));
  setModal(true);
  setFixedSize(QSize(500, 600));

  setLayout(new QVBoxLayout);

  m_header = new QLabel(this);
  m_header->setAlignment(Qt::AlignCenter);
  auto header_font = m_header->font();
  header_font.setPointSize(16);
  header_font.setBold(true);
  m_header->setFont(header_font);
  m_header->setText(QString("Real-ESRGAN-ncnn-Vulkan-UI - %1").arg(PROJECT_VERSION));
  layout()->addWidget(m_header);

  m_about_text = new QTextBrowser(this);
  m_about_text->setOpenExternalLinks(true);

  QString about_text_contents{};
  QFile about_file(":/about.md");
  if (about_file.open(QIODeviceBase::OpenMode::enum_type::ReadOnly))
    about_text_contents = QString(about_file.readAll());
  else
    about_text_contents = "# Failed reading about.md";
  about_file.close();

  m_about_text->setHtml(about_text_contents);
  layout()->addWidget(m_about_text);

  auto button_container = new QWidget(this);
  auto button_container_layout = new QHBoxLayout;
  button_container->setLayout(button_container_layout);
  button_container_layout->addStretch();

  int button_width = 80;
  m_close_button = new QPushButton(button_container);
  m_close_button->setText("Close");
  m_close_button->setFixedWidth(button_width);
  connect(m_close_button, &QPushButton::released, this, [&] { this->close(); });
  button_container_layout->addWidget(m_close_button);

  m_about_qt_button = new QPushButton(button_container);
  m_about_qt_button->setPalette(primary_button_palette());
  m_about_qt_button->setText("About Qt");
  m_about_qt_button->setFixedWidth(button_width);
  connect(m_about_qt_button, &QPushButton::released, this, [] { QApplication::aboutQt(); });
  button_container_layout->addWidget(m_about_qt_button);

  layout()->addWidget(button_container);
}
