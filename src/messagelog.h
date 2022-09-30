#ifndef MESSAGELOG_H
#define MESSAGELOG_H

#include <QPointer>
#include <QString>
#include <QTextEdit>
#include <QVector>
#include <QWidget>


class MessageLog : public QWidget {
	Q_OBJECT

public:
	MessageLog(QWidget* parent);
	~MessageLog();

	void log(const QString& message);

private:
	QVector<QString> m_messages;

	QPointer<QTextEdit> m_message_box = nullptr;
};

class Log {
public:
	static void init(MessageLog* message_log_widget) {
		m_message_log_widget = message_log_widget;
		log("Initialised Log");
	}

	static void log(const QString& message) {
		if (m_message_log_widget)
			m_message_log_widget->log(message);
	}

private:
	Log() {}

	inline static QPointer<MessageLog> m_message_log_widget = nullptr;
};

#endif