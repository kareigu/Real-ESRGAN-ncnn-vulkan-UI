#ifndef SETTINGS_H
#define SETTINGS_H

#include <QApplication>
#include <QCoreApplication>
#include <QSettings>
#include <QString>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  #define PLATFORM_DEFAULT_FETCH_URL "https://github.com/xinntao/Real-ESRGAN/releases/download/v0.2.5.0/realesrgan-ncnn-vulkan-20220424-windows.zip"
#elif __APPLE__
  #define PLATFORM_DEFAULT_FETCH_URL "https://github.com/xinntao/Real-ESRGAN/releases/download/v0.2.5.0/realesrgan-ncnn-vulkan-20220424-macos.zip"
#else
  #define PLATFORM_DEFAULT_FETCH_URL "https://github.com/xinntao/Real-ESRGAN/releases/download/v0.2.5.0/realesrgan-ncnn-vulkan-20220424-ubuntu.zip"
#endif

class Options {
public:
  [[nodiscard]] static QString fetch_url();
  static bool auto_set_output();
  static bool generate_filename();
  [[nodiscard]] static QString cli_location();

  static void set_fetch_url(const QString&);
  static void set_auto_set_output(bool);
  static void set_generate_filename(bool);
  static void set_cli_location(const QString&);

  static constexpr auto default_fetch_url = PLATFORM_DEFAULT_FETCH_URL;
  static constexpr bool default_auto_set_output = true;
  static constexpr bool default_generate_filename = true;
  static auto default_cli_location() { return QString(QCoreApplication::applicationDirPath() + "/cli"); }

private:
  static inline QSettings m_create_settings() { return {QSettings::Format::IniFormat, QSettings::Scope::UserScope, QApplication::organizationName(), QApplication::applicationName()}; }
};


#endif// SETTINGS_H
