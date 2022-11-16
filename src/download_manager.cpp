#include "download_manager.h"

#include "message_log.h"
#include <QNetworkRequest>

DownloadManager::DownloadManager(const QString& fetch_url, QObject* parent) : m_fetch_url(fetch_url), QObject(parent) {
  connect(&m_network_manager, &QNetworkAccessManager::finished, this, &DownloadManager::download_finished);
}

void DownloadManager::download_finished(QNetworkReply* reply) {
  m_downloaded_bytes = reply->readAll();
  reply->deleteLater();
  emit download_complete();
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
void DownloadManager::handle_error(QNetworkReply::NetworkError error) {
  auto error_info = QString("Error while downloading files: %1").arg(error);
  logln(error_info);
}
