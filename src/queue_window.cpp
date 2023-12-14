#include "queue_window.h"
#include "message_log.h"
#include "src/settings_panel.h"
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

  m_scroll_area = new QScrollArea(this);
  m_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  m_scroll_area->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  m_scroll_area->setWidgetResizable(true);
  layout->addWidget(m_scroll_area);

  rebuild_scroll_list();

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

void QueueWindow::rebuild_scroll_list() {
  if (m_scroll_area->widget())
    delete m_scroll_area->widget();

  if (Queue::is_empty()) {
    auto empty = new QLabel(m_scroll_area);
    empty->setText("Queue is empty");
    empty->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_scroll_area->setWidget(empty);
  } else {
    auto object_list = new QWidget(m_scroll_area);
    object_list->setLayout(new QVBoxLayout);

    for (const auto& item : Queue::get_items().asKeyValueRange()) {
      auto object = new QueueObject(item.second, object_list);
      auto uuid = item.first;
      connect(object, &QueueObject::remove_clicked, this, [this, object, uuid] {
        Queue::remove(uuid);
        delete object;
        rebuild_scroll_list();
        emit queue_item_removed();
      });
      object_list->layout()->addWidget(object);
    }
    m_scroll_area->setWidget(object_list);
  }
}

void QueueWindow::update_window() {
  debugln("Updating QueueWindow");
  rebuild_scroll_list();
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
  model->setText(QString("Model: %1").arg(model_option_to_string(queue_item.model)));
  auto size = new QLabel(settings_container);
  size->setText(QString("Size: %1x").arg(static_cast<int>(queue_item.size)));
  settings_container->layout()->addWidget(model);
  settings_container->layout()->addWidget(size);
  layout->addWidget(settings_container);

  // TODO: Disabled for now, possibly implement later
  /* auto edit_button = new QPushButton(this); */
  /* edit_button->setText(tr("Edit")); */
  /* edit_button->setFixedSize(50, QUEUE_OBJECT_BUTTON_BASE_SIZE); */
  /* layout->addWidget(edit_button); */

  auto remove_button = new QPushButton(this);
  remove_button->setText("X");
  remove_button->setFixedSize(QUEUE_OBJECT_BUTTON_BASE_SIZE, QUEUE_OBJECT_BUTTON_BASE_SIZE);
  connect(remove_button, &QPushButton::released, this, [&] { emit remove_clicked(); });
  layout->addWidget(remove_button);
}
