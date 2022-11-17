#ifndef SETTINGS_H
#define SETTINGS_H

#include <QApplication>
#include <QCoreApplication>
#include <QSettings>
#include <QString>

class Options {
public:
  [[nodiscard]] static QString fetch_url();
  static bool auto_rename();
  static bool generate_filename();
  [[nodiscard]] static QString cli_location();

  static void set_fetch_url(const QString&);
  static void set_auto_rename(bool);
  static void set_generate_filename(bool);
  static void set_cli_location(const QString&);

  static constexpr auto default_fetch_url = "https://github.com/xinntao/Real-ESRGAN/releases/download/v0.2.5.0/realesrgan-ncnn-vulkan-20220424-windows.zip";
  static constexpr bool default_auto_rename = true;
  static constexpr bool default_generate_filename = true;
  static auto default_cli_location() { return QString(QCoreApplication::applicationDirPath() + "/cli"); }

private:
  static inline QSettings m_create_settings() { return {QSettings::Format::IniFormat, QSettings::Scope::UserScope, QApplication::organizationName(), QApplication::applicationName()}; }
};


#endif// SETTINGS_H
