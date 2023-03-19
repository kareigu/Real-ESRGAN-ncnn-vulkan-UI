#include "path_picker.h"
#include "message_log.h"
#include "options.h"
#include <QFileDialog>
#include <QHBoxLayout>

PathPicker::PathPicker(const QString& title, QWidget* parent)
    : QWidget(parent) {
  m_title = new QLabel();
  m_title->setText(QString("%1:").arg(title));
  m_title->setMinimumWidth(45);
  m_init();
}

PathPicker::PathPicker(QWidget* parent) : QWidget(parent) { m_init(); }

void PathPicker::m_init() {
  m_path = new QLineEdit();
  connect(m_path, &QLineEdit::textChanged, this, [&] { emit path_updated(); });

  m_browse_button = new QPushButton();
  m_browse_button->setText(tr("Browse"));
  connect(m_browse_button, &QPushButton::released, this, [&] {
    auto current_path = m_path->text();

    auto path = ([&] {
      switch (m_mode) {
        case PathPicker::SelectFolder:
          return QFileDialog::getExistingDirectory(this, tr("Select folder"), Options::cli_location());
        case PathPicker::SaveFile:
          return QFileDialog::getSaveFileName(this, tr("Save as..."), current_path);
        case PathPicker::OpenFile:
          return QFileDialog::getOpenFileName(this, tr("Select image"), current_path, tr("Image Files (*.png *.jpg *.bmp)"));
      }
    }());

    debugln(QString("Path selected: { m_mode = %1, path = %2 }")
                    .arg(PathPicker::path_picker_mode_strings[static_cast<size_t>(m_mode)], path));

    if (path.isEmpty())
      return;

    m_path->setText(path);
  });

  setLayout(new QHBoxLayout);

  if (m_title)
    this->layout()->addWidget(m_title);

  layout()->addWidget(m_path);
  layout()->addWidget(m_browse_button);
}
