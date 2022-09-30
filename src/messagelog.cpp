#include "messagelog.h"
#include <QDateTime>
#include <QVBoxLayout>
#include <QScrollBar>

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
		QString("[%1] %2\n")
			.arg(time.toString("hh:mm:ss"))
			.arg(message)
	);
	
	QString log_text;
	for (const auto& message : m_messages)
		log_text += message;

	m_message_box->setText(log_text);
	m_message_box->verticalScrollBar()->setValue(m_message_box->verticalScrollBar()->maximum());
}

MessageLog::~MessageLog() {

}