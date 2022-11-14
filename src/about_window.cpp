#include "about_window.h"
#include "message_log.h"
#include <QFile>
#include <QVBoxLayout>

AboutWindow::AboutWindow(QWidget* parent) : QDialog(parent) {
  setWindowTitle(tr("About"));
  setModal(true);
  setFixedSize(QSize(500, 600));

  setLayout(new QVBoxLayout);

  m_about_text = new QTextBrowser(this);
  m_about_text->setOpenExternalLinks(true);

  QString about_text_contents{};
  QFile about_file(":/about.md");
  if (about_file.open(QIODeviceBase::OpenMode::enum_type::ReadOnly))
    about_text_contents = QString(about_file.readAll());
  else
    about_text_contents = "# Failed reading about.md";
  about_file.close();

  m_about_text->setMarkdown(about_text_contents);
  layout()->addWidget(m_about_text);

  m_close_button = new QPushButton(this);
  m_close_button->setText("Close");
  connect(m_close_button, &QPushButton::released, this, [&] { this->close(); });
  layout()->addWidget(m_close_button);
}
