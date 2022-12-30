#ifndef QUEUE_H
#define QUEUE_H

#include "settings_panel.h"
#include <QString>

class Queue {
public:
  struct Item {
    QString input_path;
    QString output_path;
    ModelOptions model;
    SizeOptions size;
  };

  [[nodiscard]] static QList<Item> const& get_items() { return m_queue; }
  static void add(Item& item);
  static void remove(qsizetype id);

  static bool is_empty();
  static void clear();


private:
  inline static QList<Item> m_queue{};
};


#endif// QUEUE_H
