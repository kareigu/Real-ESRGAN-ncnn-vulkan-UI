#ifndef PATHPICKER_H
#define PATHPICKER_H

#include <functional>
#include <QLabel>
#include <QLineEdit>
#include <QPointer>
#include <QPushButton>
#include <QWidget>

class PathPicker : public QWidget {
	Q_OBJECT
public:
	PathPicker(const QString& title, QWidget* parent = nullptr);
	PathPicker(QWidget* parent = nullptr);
	~PathPicker();

	void set_mode(bool select_folder) { m_select_folder = select_folder; }
	void set_folder_mode() { m_select_folder = true; }
	void set_file_mode() { m_select_folder = false; }

	bool mode() { return m_select_folder; }

	const QString path() const { return m_path->text(); }
	void set_path(const QString& path) { m_path->setText(path); }

	void set_select_callback(std::function<void(const QString& path)> callback) { m_select_callback = callback; }

private:
	void m_init();

	bool m_select_folder = false;

	std::function<void(const QString& path)> m_select_callback;

	QPointer<QLabel> m_title = nullptr;
	QPointer<QLineEdit> m_path = nullptr;
	QPointer<QPushButton> m_browse_button = nullptr;
};


#endif