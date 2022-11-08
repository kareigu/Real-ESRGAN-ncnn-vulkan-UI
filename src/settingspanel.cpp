#include "settingspanel.h"
#include "messagelog.h"
#include <QButtonGroup>
#include <QVBoxLayout>


SettingsPanel::SettingsPanel(QWidget* parent) : QGroupBox(parent) {
  m_model_options = new QGroupBox(this);
  m_model_options->setLayout(new QVBoxLayout);
  m_model_options->setTitle("Model");

  for (int i = 0; i < (int) ModelOptions::Size; i++) {
    auto model = static_cast<ModelOptions>(i);
    auto button = new QRadioButton(m_model_options);
    button->setText(model_option_to_string(model));

    if (model == m_model)
      button->setChecked(true);

    connect(button, &QRadioButton::toggled, this, [this, button, model]() {
      if (button->isChecked()) {
        m_model = model;
        debugln(QString("Set model to %1").arg(model_option_to_string(model)));
        emit settingsChanged();
      }
    });

    m_model_options->layout()->addWidget(button);
  }

  m_size_options = new QGroupBox(this);
  m_size_options->setLayout(new QVBoxLayout);
  m_size_options->setTitle("Size");

  for (int i = 4; i > 1; i--) {
    auto size = static_cast<SizeOptions>(i);
    auto button = new QRadioButton(m_size_options);
    button->setText(QString("%1x").arg(static_cast<int>(size)));

    if (size == m_size)
      button->setChecked(true);

    connect(button, &QRadioButton::toggled, this, [this, button, size]() {
      if (button->isChecked()) {
        m_size = size;
        debugln(QString("Set size to %1").arg((int) size));
        emit settingsChanged();
      }
    });

    m_size_options->layout()->addWidget(button);
  }

  setTitle("Settings");
  setLayout(new QHBoxLayout);
  layout()->addWidget(m_model_options);
  layout()->addWidget(m_size_options);
}

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
