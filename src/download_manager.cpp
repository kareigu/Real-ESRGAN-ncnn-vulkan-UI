#include "download_manager.h"

#include "message_log.h"
#include "options.h"
#include <QDir>
#include <QFile>
#include <QNetworkRequest>
#include <QVBoxLayout>
#include <elzip/elzip.hpp>

DownloadManager::DownloadManager(QWidget* parent) : QDialog(parent) {
  setWindowTitle("Downloading CLI");
  setAttribute(Qt::WA_DeleteOnClose);
  auto layout = new QVBoxLayout;
  setLayout(layout);
  setFixedSize(300, 100);
  m_info_text = new QLabel(this);
  m_info_text->setText("Starting download...");
  layout->addWidget(m_info_text);

  m_progress_bar = new QProgressBar(this);
  layout->addWidget(m_progress_bar);

  m_close_button = new QPushButton(this);
  m_close_button->setText("Close");
  m_close_button->setFixedSize(80, 25);
  m_close_button->setDisabled(true);
  auto button_layout = new QHBoxLayout;
  button_layout->addWidget(m_close_button);
  layout->addLayout(button_layout);
  connect(m_close_button, &QPushButton::released, this, &DownloadManager::close);

  connect(&m_network_manager, &QNetworkAccessManager::finished, this, &DownloadManager::download_finished);
}

void DownloadManager::start_download() {
  if (m_download_in_progress)
    return;

  m_download_in_progress = true;

  QNetworkRequest req(Options::fetch_url());
  auto reply = m_network_manager.get(req);
  connect(reply, &QNetworkReply::errorOccurred, this, &DownloadManager::handle_error);
  connect(reply, &QNetworkReply::downloadProgress, this, [&](qint64 rec, qint64 total) {
    auto info = QString("Download progress: %1/%2").arg(rec).arg(total);
    m_info_text->setText("Downloading zip file");
    m_progress_bar->setMaximum(static_cast<int>(total));
    m_progress_bar->setValue(static_cast<int>(rec));
    logln(info);
  });
  show();
}

void DownloadManager::download_finished(QNetworkReply* reply) {
  m_info_text->setText("Writing zip file to disk");
  m_downloaded_bytes = reply->readAll();
  reply->deleteLater();
  m_close_button->setEnabled(true);

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

  m_info_text->setText("Extracting zip file");

  try {
    elz::extractZip(zip_file.filesystemFileName(), cli_folder.toStdString());
    debugln("Extracted temporary zip file");
  } catch (const elz::zip_exception& e) {
    logln(e.what());
  }

  m_info_text->setText("Removing zip file");
  if (zip_file.remove())
    debugln("Removed temporary zip file");

  m_info_text->setText("Complete");
  m_download_in_progress = false;
  logln("Download complete");
}

void DownloadManager::handle_error(QNetworkReply::NetworkError error) {
  auto error_info = QString("Error while downloading files: %1").arg(error);
  logln(error_info);
  m_close_button->setEnabled(true);
}
