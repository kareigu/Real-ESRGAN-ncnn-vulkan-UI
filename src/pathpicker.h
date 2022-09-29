#ifndef PATHPICKER_H
#define PATHPICKER_H

#include <QWidget>
#include <QPointer>
#include <QLineEdit>
#include <QPushButton>

class PathPicker : public QWidget {
	Q_OBJECT
public:
	PathPicker(QWidget* parent = nullptr);
	~PathPicker();

	void set_mode(bool select_folder) { m_select_folder = select_folder; }
	void set_folder_mode() { m_select_folder = true; }
	void set_file_mode() { m_select_folder = false; }

	bool mode() { return m_select_folder; }

private:
	bool m_select_folder = false;

	QPointer<QLineEdit> m_path = nullptr;
	QPointer<QPushButton> m_browse_button = nullptr;
};


#endif