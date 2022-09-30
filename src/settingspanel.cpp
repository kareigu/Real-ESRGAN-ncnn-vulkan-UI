#include "settingspanel.h"
#include <QButtonGroup>
#include <QVBoxLayout>

static QStringList MODEL_OPTIONS = {
	"realesr-animevideov3",
	"realesrgan-x4plus",
	"realesrgan-x4plus-anime",
};

enum class ModelOptions {
	RealESR_AnimeVideoV3,
	RealESRGAN_x4plus,
	RealESRGAN_x4plus_anime,
	Size,
};

QString model_option_to_string(ModelOptions option) {
	switch (option) {
	case ModelOptions::RealESR_AnimeVideoV3:
		return "realesr-animevideov3";
	case ModelOptions::RealESRGAN_x4plus:
		return "realesrgan-x4plus";
	case ModelOptions::RealESRGAN_x4plus_anime:
		return "realesrgan-x4plus-anime";
	default:
		return "Invalid";
	}
}

static QStringList SIZE_OPTIONS = {
	"4",
	"3",
	"2",
};

enum class SizeOptions {
	X4 = 4,
	X3 = 3,
	X2 = 2,
};


SettingsPanel::SettingsPanel(QWidget* parent) : QGroupBox(parent) {
	m_model_options = new QGroupBox(this);
	m_model_options->setLayout(new QVBoxLayout);
	m_model_options->setTitle("Model");

	for (int i = 0; i < (int)ModelOptions::Size; i++) {
		auto button = new QRadioButton(m_model_options);
		button->setText(model_option_to_string((ModelOptions)i));

		if (i == 0)
			button->setChecked(true);

		m_model_options->layout()->addWidget(button);
	}

	m_size_options = new QGroupBox(this);
	m_size_options->setLayout(new QVBoxLayout);
	m_size_options->setTitle("Size");

	for (int i = 4; i > 1; i--) {
		auto button = new QRadioButton(m_size_options);
		button->setText(QString("%1x").arg(i));

		if (i == (int)SizeOptions::X4)
			button->setChecked(true);

		m_size_options->layout()->addWidget(button);
	}

	setTitle("Settings");
	setLayout(new QHBoxLayout);
	layout()->addWidget(m_model_options);
	layout()->addWidget(m_size_options);
}

SettingsPanel::~SettingsPanel() {

}