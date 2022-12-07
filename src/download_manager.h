#ifndef DOWNLOAD_MANAGER_H
#define DOWNLOAD_MANAGER_H

#include <QByteArray>
#include <QDialog>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPointer>
#include <QProgressBar>
#include <QPushButton>
#include <QUrl>

class DownloadManager final : public QDialog {
  Q_OBJECT
public:
  explicit DownloadManager(QWidget* parent);
  ~DownloadManager() final = default;

  [[nodiscard]] size_t downloaded_bytes_count() const { return m_downloaded_bytes.count(); }

  void start_download();

signals:
  void download_complete();

private slots:
  void download_finished(QNetworkReply* reply);
  void handle_error(QNetworkReply::NetworkError error);

private:
  QNetworkAccessManager m_network_manager;
  QByteArray m_downloaded_bytes;

  QPointer<QLabel> m_info_text = nullptr;
  QPointer<QProgressBar> m_progress_bar = nullptr;
  QPointer<QPushButton> m_close_button = nullptr;
};


#endif// DOWNLOAD_MANAGER_H
