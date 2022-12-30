#include "queue.h"


void Queue::add(Item& item) {
  m_queue.append(std::move(item));
}

void Queue::remove(qsizetype id) {
  m_queue.remove(id);
}

bool Queue::is_empty() {
  return m_queue.isEmpty();
}

void Queue::clear() {
  m_queue.clear();
}