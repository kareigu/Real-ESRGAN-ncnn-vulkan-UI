#include "pathpicker.h"
#include <QFileDialog>
#include <QHBoxLayout>

#include "messagelog.h"

PathPicker::PathPicker(const QString& title, QWidget* parent)
    : QWidget(parent) {
  m_title = new QLabel();
  m_title->setText(QString("%1:").arg(title));
  m_title->setFixedWidth(45);
  m_init();
}

PathPicker::PathPicker(QWidget* parent) : QWidget(parent) { m_init(); }

void PathPicker::m_init() {
  m_path = new QLineEdit();
  m_browse_button = new QPushButton();
  m_browse_button->setText(tr("Browse"));

  connect(m_browse_button, &QPushButton::released, this, [&] {
    auto current_path = m_path->text();

    auto path = m_save
                      ? QFileDialog::getSaveFileName(this, tr("Select folder"), current_path)
                      : QFileDialog::getOpenFileName(this, tr("Select image"), current_path, tr("Image Files (*.png *.jpg *.bmp)"));

    debugln(QString("Path selected: { m_select_folder = %1, path = %2 }")
                    .arg(m_save)
                    .arg(path));

    if (path.isEmpty())
      return;

    m_path->setText(path);
    emit path_updated();
  });

  setLayout(new QHBoxLayout);

  if (m_title)
    this->layout()->addWidget(m_title);

  layout()->addWidget(m_path);
  layout()->addWidget(m_browse_button);
}
