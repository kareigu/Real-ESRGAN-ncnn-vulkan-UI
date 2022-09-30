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
	enum class LogLevel {
		Info,
		Debug,
	};

	static void init(MessageLog* message_log_widget) {
		m_message_log_widget = message_log_widget;
		log("Initialised Log");
	}

	static void log(const QString& message) {
		if (m_message_log_widget)
			m_message_log_widget->log(message);
	}

	static void debug(const QString& message) {
		if (m_log_level >= LogLevel::Debug)
			log(QString("DEBUG : %1").arg(message));
	}

	static void set_log_level(LogLevel log_level) {
		m_log_level = log_level;
		debug(QString("Set logging level to %1").arg(m_log_level_to_name(log_level)));
	}

private:
	Log() {}

	inline static const char* m_log_level_names[] = {
		"Info",
		"Debug",
	};

	static const char* m_log_level_to_name(LogLevel log_level) { return m_log_level_names[(size_t)log_level]; }

	inline static QPointer<MessageLog> m_message_log_widget = nullptr;
	inline static LogLevel m_log_level = LogLevel::Info;
};

#endif