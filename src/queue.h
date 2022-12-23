#ifndef QUEUE_H
#define QUEUE_H

#include <QString>
#include "settings_panel.h"

class Queue {
public:
  struct Item {
    QString input_path;
    QString output_path;
    ModelOptions model;
    SizeOptions size;
  };

  [[nodiscard]] static QList<Item> const& get_items() { return m_queue; }
  static void add_to_queue(Item& item) { m_queue.append(item); }

private:
  inline static QList<Item> m_queue{};
};


#endif// QUEUE_H
