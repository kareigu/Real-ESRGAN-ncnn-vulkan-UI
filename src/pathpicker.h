#ifndef PATHPICKER_H
#define PATHPICKER_H

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

private:
	void m_init();

	bool m_select_folder = false;

	QPointer<QLabel> m_title = nullptr;
	QPointer<QLineEdit> m_path = nullptr;
	QPointer<QPushButton> m_browse_button = nullptr;
};


#endif