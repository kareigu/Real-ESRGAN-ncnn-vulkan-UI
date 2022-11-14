#include "options.h"

constexpr auto KEY_FETCH_URL = "fetch_url";
constexpr auto KEY_AUTO_RENAME = "auto_rename";
constexpr auto KEY_GENERATE_FILENAME = "generate_filename";

QString Options::fetch_url() {
  return m_create_settings().value(KEY_FETCH_URL, QVariant(default_fetch_url)).toString();
}

void Options::set_fetch_url(const QString& new_url) {
  auto settings = m_create_settings();
  settings.setValue(KEY_FETCH_URL, QVariant(new_url));
}

bool Options::auto_rename() {
  return m_create_settings().value(KEY_AUTO_RENAME, QVariant(default_auto_rename)).toBool();
}

void Options::set_auto_rename(bool auto_rename) {
  auto settings = m_create_settings();
  settings.setValue(KEY_AUTO_RENAME, QVariant(auto_rename));
}

bool Options::generate_filename() {
  auto settings = m_create_settings();
  return settings.value(KEY_GENERATE_FILENAME, QVariant(default_generate_filename)).toBool();
}

void Options::set_generate_filename(bool generate_filename) {
  auto settings = m_create_settings();
  settings.setValue(KEY_GENERATE_FILENAME, QVariant(generate_filename));
}