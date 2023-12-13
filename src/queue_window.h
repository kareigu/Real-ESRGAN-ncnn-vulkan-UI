#ifndef QUEUE_WINDOW_H
#define QUEUE_WINDOW_H

#include "queue.h"
#include <QScrollArea>
#include <QWidget>

class QueueWindow final : public QWidget {
  Q_OBJECT

public:
  QueueWindow();
  ~QueueWindow() final = default;

  void show_window();
  void update_window();
signals:
  void queue_item_removed();

private:
  void rebuild_scroll_list();

  QPointer<QScrollArea> m_scroll_area = nullptr;
};

class QueueObject final : public QWidget {
  Q_OBJECT
  friend class QueueWindow;

public:
  ~QueueObject() final = default;

signals:
  void remove_clicked();

private:
  QueueObject(Queue::Item const& queue_item, QWidget* parent);
};


#endif// QUEUE_WINDOW_H
