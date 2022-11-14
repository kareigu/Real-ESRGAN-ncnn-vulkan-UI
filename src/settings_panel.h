#ifndef SETTINGS_PANEL_H
#define SETTINGS_PANEL_H

#include <QGroupBox>
#include <QPointer>
#include <QRadioButton>

enum class ModelOptions {
  RealESR_AnimeVideoV3,
  RealESRGAN_x4plus,
  RealESRGAN_x4plus_anime,
  Size,
};

enum class SizeOptions {
  X4 = 4,
  X3 = 3,
  X2 = 2,
};

QString model_option_to_string(ModelOptions option);

class SettingsPanel : public QGroupBox {
  Q_OBJECT
public:
  explicit SettingsPanel(QWidget* parent);

  ~SettingsPanel() override = default;

  QString model() { return model_option_to_string(raw_model()); }

  ModelOptions raw_model() { return m_model; }

  QString up_size() { return QString("%1").arg(static_cast<int>(raw_up_size())); }

  SizeOptions raw_up_size() { return m_size; }

signals:
  void settings_changed();

private:
  ModelOptions m_model = ModelOptions::RealESR_AnimeVideoV3;
  SizeOptions m_size = SizeOptions::X4;

  QPointer<QGroupBox> m_model_options = nullptr;
  QPointer<QGroupBox> m_size_options = nullptr;
};


#endif// SETTINGS_PANEL_H
