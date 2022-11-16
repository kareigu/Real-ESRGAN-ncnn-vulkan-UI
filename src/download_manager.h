#ifndef DOWNLOAD_MANAGER_H
#define DOWNLOAD_MANAGER_H

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>

class DownloadManager final : public QObject {
  Q_OBJECT
public:
  explicit DownloadManager(const QString& fetch_url, QObject* parent = nullptr);
  ~DownloadManager() final = default;

  [[nodiscard]] size_t downloaded_bytes_count() const { return m_downloaded_bytes.count(); }

  void start_download();

signals:
  void download_complete();

private slots:
  void download_finished(QNetworkReply* reply);
  static void handle_error(QNetworkReply::NetworkError error);

private:
  QUrl m_fetch_url{};
  QNetworkAccessManager m_network_manager;
  QByteArray m_downloaded_bytes;
};


#endif// DOWNLOAD_MANAGER_H
