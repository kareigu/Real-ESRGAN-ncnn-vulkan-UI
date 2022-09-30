#ifndef SETTINGSPANEL_H
#define SETTINGSPANEL_H

#include <QGroupBox>
#include <QPointer>
#include <QRadioButton>

class SettingsPanel : public QGroupBox {
	Q_OBJECT
public:
	SettingsPanel(QWidget* parent);
	~SettingsPanel();

private:

	QPointer<QGroupBox> m_model_options = nullptr;
	QPointer<QGroupBox> m_size_options = nullptr;
};

#endif
