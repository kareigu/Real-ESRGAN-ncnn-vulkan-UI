#include "queue_window.h"
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

QueueWindow::QueueWindow() {
  setWindowTitle(tr("Queue"));
  setFixedSize(700, 500);
  setAttribute(Qt::WA_DeleteOnClose);

  auto layout = new QVBoxLayout;
  setLayout(layout);

  auto scroll_area = new QScrollArea(this);
  scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scroll_area->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  scroll_area->setWidgetResizable(true);
  layout->addWidget(scroll_area);

  if (Queue::is_empty()) {
    auto empty = new QLabel(scroll_area);
    empty->setText("Queue is empty");
    empty->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    scroll_area->setWidget(empty);
  } else {
    auto object_list = new QWidget(scroll_area);
    object_list->setLayout(new QVBoxLayout);
    for (const auto& item : Queue::get_items()) {
      auto object = new QueueObject(item, object_list);
      object_list->layout()->addWidget(object);
    }
    scroll_area->setWidget(object_list);
  }


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


constexpr int QUEUE_OBJECT_INPUT_WIDTH = 350;
constexpr int QUEUE_OBJECT_BUTTON_BASE_SIZE = 24;

QueueObject::QueueObject(Queue::Item const& queue_item, QWidget* parent) : QWidget(parent) {
  setFixedHeight(80);
  auto layout = new QHBoxLayout;
  setLayout(layout);

  auto paths_container = new QWidget(this);
  paths_container->setLayout(new QVBoxLayout);
  auto input_path = new QLabel(paths_container);
  input_path->setText(queue_item.input_path.isEmpty() ? "No input path" : queue_item.input_path);
  input_path->setFixedWidth(QUEUE_OBJECT_INPUT_WIDTH);
  auto output_path = new QLabel(paths_container);
  output_path->setText(queue_item.output_path.isEmpty() ? "No output path" : queue_item.output_path);
  output_path->setFixedWidth(QUEUE_OBJECT_INPUT_WIDTH);
  paths_container->layout()->addWidget(input_path);
  paths_container->layout()->addWidget(output_path);
  layout->addWidget(paths_container);

  auto settings_container = new QWidget(this);
  settings_container->setLayout(new QVBoxLayout);
  auto model = new QLabel(settings_container);
  model->setText("Model: placeholder");
  auto size = new QLabel(settings_container);
  size->setText("Size: X4");
  settings_container->layout()->addWidget(model);
  settings_container->layout()->addWidget(size);
  layout->addWidget(settings_container);

  auto edit_button = new QPushButton(this);
  edit_button->setText(tr("Edit"));
  edit_button->setFixedSize(50, QUEUE_OBJECT_BUTTON_BASE_SIZE);
  layout->addWidget(edit_button);

  auto remove_button = new QPushButton(this);
  remove_button->setText("X");
  remove_button->setFixedSize(QUEUE_OBJECT_BUTTON_BASE_SIZE, QUEUE_OBJECT_BUTTON_BASE_SIZE);
  layout->addWidget(remove_button);
}