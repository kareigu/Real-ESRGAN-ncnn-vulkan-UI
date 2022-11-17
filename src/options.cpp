#include "options.h"

static constexpr auto KEY_FETCH_URL = "fetch_url";
static constexpr auto KEY_AUTO_SET_OUTPUT = "auto_set_output";
static constexpr auto KEY_GENERATE_FILENAME = "generate_filename";
static constexpr auto KEY_CLI_LOCATION = "cli_location";

QString Options::fetch_url() {
  return m_create_settings().value(KEY_FETCH_URL, QVariant(default_fetch_url)).toString();
}

void Options::set_fetch_url(const QString& new_url) {
  auto settings = m_create_settings();
  settings.setValue(KEY_FETCH_URL, QVariant(new_url));
}

bool Options::auto_set_output() {
  return m_create_settings().value(KEY_AUTO_SET_OUTPUT, QVariant(default_auto_set_output)).toBool();
}

void Options::set_auto_set_output(bool auto_rename) {
  auto settings = m_create_settings();
  settings.setValue(KEY_AUTO_SET_OUTPUT, QVariant(auto_rename));
}

bool Options::generate_filename() {
  auto settings = m_create_settings();
  return settings.value(KEY_GENERATE_FILENAME, QVariant(default_generate_filename)).toBool();
}

void Options::set_generate_filename(bool generate_filename) {
  auto settings = m_create_settings();
  settings.setValue(KEY_GENERATE_FILENAME, QVariant(generate_filename));
}

QString Options::cli_location() {
  return m_create_settings().value(KEY_CLI_LOCATION, QVariant(default_cli_location())).toString();
}

void Options::set_cli_location(const QString& new_location) {
  auto settings = m_create_settings();
  settings.setValue(KEY_CLI_LOCATION, QVariant(new_location));
}
