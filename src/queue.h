#ifndef QUEUE_H
#define QUEUE_H

#include "settings_panel.h"
#include <QMap>
#include <QString>
#include <QUuid>
#include <functional>


class Queue {
public:
  struct Item {
    QString input_path;
    QString output_path;
    ModelOptions model;
    SizeOptions size;
  };

  [[nodiscard]] static QMap<QUuid, Item> const& get_items() { return m_queue; }
  static qsizetype length() { return m_queue.count(); }
  static void add(Item& item);
  static void remove(QUuid uuid);

  static bool is_empty();
  static void clear();


private:
  inline static QMap<QUuid, Item> m_queue{};
};


#endif// QUEUE_H
