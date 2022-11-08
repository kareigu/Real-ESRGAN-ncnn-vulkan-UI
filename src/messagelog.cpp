#include "messagelog.h"
#include <QDateTime>
#include <QScrollBar>
#include <QVBoxLayout>

MessageLog::MessageLog(QWidget* parent) : QWidget(parent) {
  m_message_box = new QTextEdit(this);
  m_message_box->setReadOnly(true);

  setLayout(new QVBoxLayout);
  layout()->addWidget(m_message_box);
}

void MessageLog::log(const QString& message) {
  if (message.isEmpty())
    return;

  QDateTime time = QDateTime::currentDateTime();
  m_messages.append(
          QString("[%1] %2\n").arg(time.toString("hh:mm:ss"), message));

  QString log_text;
  for (const auto& old_message : m_messages)
    log_text += old_message;

  m_message_box->setText(log_text);
  m_message_box->verticalScrollBar()->setValue(
          m_message_box->verticalScrollBar()->maximum());
}
