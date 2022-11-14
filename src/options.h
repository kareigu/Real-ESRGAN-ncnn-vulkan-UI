#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

class Options {
public:
  [[nodiscard]] static QString fetch_url();
  static bool auto_rename();
  static bool generate_filename();

  static void set_fetch_url(const QString& new_url);
  static void set_auto_rename(bool auto_rename);
  static void set_generate_filename(bool generate_filename);

  static constexpr auto default_fetch_url = "https://github.com/xinntao/Real-ESRGAN/releases/download/v0.2.5.0/realesrgan-ncnn-vulkan-20220424-windows.zip";
  static constexpr bool default_auto_rename = true;
  static constexpr bool default_generate_filename = true;

private:
};


#endif// SETTINGS_H
