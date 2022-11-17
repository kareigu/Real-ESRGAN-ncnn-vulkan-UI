#include "download_manager.h"

#include "message_log.h"
#include "options.h"
#include <QDir>
#include <QFile>
#include <QNetworkRequest>
#include <elzip/elzip.hpp>

DownloadManager::DownloadManager(const QString& fetch_url, QObject* parent) : m_fetch_url(fetch_url), QObject(parent) {
  connect(&m_network_manager, &QNetworkAccessManager::finished, this, &DownloadManager::download_finished);
}

void DownloadManager::start_download() {
  QNetworkRequest req(m_fetch_url);
  auto reply = m_network_manager.get(req);
  connect(reply, &QNetworkReply::errorOccurred, this, &DownloadManager::handle_error);
  connect(reply, &QNetworkReply::downloadProgress, this, [](qint64 rec, qint64 total) {
    auto info = QString("Download progress: %1/%2").arg(rec).arg(total);
    logln(info);
  });
}

void DownloadManager::download_finished(QNetworkReply* reply) {
  m_downloaded_bytes = reply->readAll();
  reply->deleteLater();

  auto cli_folder = Options::cli_location();
  if (!QDir(cli_folder).exists())
    QDir().mkpath(cli_folder);

  constexpr auto temp_zip_filename = "cli.zip";
  QFile zip_file(cli_folder + "/" + temp_zip_filename);

  if (!zip_file.open(QIODevice::WriteOnly)) {
    auto error_info = QString("Couldn't open temporary zip file for writing - path = %1/%2").arg(cli_folder, temp_zip_filename);
    logln(error_info);
    return;
  }

  QDataStream output(&zip_file);
  output.writeRawData(m_downloaded_bytes, static_cast<int>(m_downloaded_bytes.length()));

  zip_file.close();
  debugln("Wrote temporary zip file");

  try {
    elz::extractZip(zip_file.filesystemFileName(), cli_folder.toStdString());
    debugln("Extracted temporary zip file");
  } catch (const elz::zip_exception& e) {
    logln(e.what());
    return;
  }

  if (zip_file.remove())
    debugln("Removed temporary zip file");

  emit download_complete();
}

void DownloadManager::handle_error(QNetworkReply::NetworkError error) {
  auto error_info = QString("Error while downloading files: %1").arg(error);
  logln(error_info);
}
