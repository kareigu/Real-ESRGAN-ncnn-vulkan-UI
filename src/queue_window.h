#ifndef QUEUE_WINDOW_H
#define QUEUE_WINDOW_H

#include "queue.h"
#include <QWidget>

class QueueWindow final : public QWidget {
  Q_OBJECT

public:
  QueueWindow();
  ~QueueWindow() final = default;

  void show_window();

private:
};

class QueueObject final : public QWidget {
  Q_OBJECT
  friend class QueueWindow;

public:
  ~QueueObject() final = default;

private:
  QueueObject(Queue::Item const& queue_item, QWidget* parent);
};


#endif// QUEUE_WINDOW_H
