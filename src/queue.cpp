#include "queue.h"
#include "message_log.h"


void Queue::add(Item& item) {
  auto uuid = QUuid::createUuid();
  m_queue[uuid] = std::move(item);
}

void Queue::remove(QUuid uuid) {
  debugln(QString("Remove queue from queue: %1").arg(uuid.toString()));
  m_queue.remove(uuid);
}

bool Queue::is_empty() {
  return m_queue.isEmpty();
}

void Queue::clear() {
  m_queue.clear();
}