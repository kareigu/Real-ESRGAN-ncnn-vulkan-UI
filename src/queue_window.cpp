#include "queue_window.h"
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

QueueWindow::QueueWindow() {
  setWindowTitle(tr("Queue"));
  setFixedSize(500, 500);
  setAttribute(Qt::WA_DeleteOnClose);

  auto layout = new QVBoxLayout;
  setLayout(layout);

  auto scroll_area = new QScrollArea(this);
  layout->addWidget(scroll_area);

  auto button_container = new QWidget(this);
  auto button_container_layout = new QHBoxLayout;
  button_container->setLayout(button_container_layout);
  button_container_layout->addStretch();
  auto close_button = new QPushButton(button_container);
  close_button->setText(tr("Close"));
  connect(close_button, &QPushButton::released, this, &QWidget::close);
  button_container_layout->addWidget(close_button);
  layout->addWidget(button_container);

}

void QueueWindow::show_window() {
  if (isVisible())
    activateWindow();
  else
    show();
}