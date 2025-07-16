/**
 * SPDX-FileCopyrightText: 2025 Soumyadeep Ghosh (সৌম্যদীপ ঘোষ) <soumyadeepghosh2004@zohomail.in>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "downloadmanager.h"
#include "config.h"
#include "sqlhelper.h"

#include <QFileDialog>
#include <QMimeDatabase>
#include <QStandardPaths>

#include <KIO/JobUiDelegateFactory>
#include <KIO/OpenFileManagerWindowJob>
#include <KIO/OpenUrlJob>

using namespace Qt::Literals::StringLiterals;

DownloadManager::DownloadManager(QObject *parent)
    : QAbstractListModel(parent)
{
    m_downloads = SqlHelper::loadHistory();
}

int DownloadManager::rowCount(const QModelIndex &) const
{
    return m_downloads.count();
}

QVariant DownloadManager::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_downloads.size())
        return {};

    const auto &entry = m_downloads.at(index.row());
    QLocale locale;
    switch (role) {
    case FileRole:
        return QFileInfo(entry.filePath).absoluteFilePath();
    case FileNameRole:
        return QFileInfo(entry.filePath).fileName();
    case FilePathRole:
        return QFileInfo(entry.filePath).absoluteDir().path();
    case MimeTypeRole:
        return QMimeDatabase().mimeTypeForFile(entry.filePath).iconName();
    case FileSizeRole:
        return QFileInfo(entry.filePath).size();
    case TimestampRole:
        return QFileInfo(entry.filePath).lastModified();
    case DateRole:
        return locale.toString(QFileInfo(entry.filePath).lastModified().date());
    case TimeRole:
        return locale.toString(QFileInfo(entry.filePath).lastModified().toLocalTime().time(), QLocale::ShortFormat);
    default:
        return {};
    }
}

QHash<int, QByteArray> DownloadManager::roleNames() const
{
    return {{FileRole, "file"},
            {FileNameRole, "fileName"},
            {FilePathRole, "filePath"},
            {MimeTypeRole, "mimeType"},
            {FileSizeRole, "fileSize"},
            {TimestampRole, "timestamp"},
            {DateRole, "date"},
            {TimeRole, "time"}};
}

void DownloadManager::downloadFile(QWebEngineDownloadRequest *request)
{
    auto config = Config::self();
    QString savePath;
    if (!config->askDownloadPath()) {
        request->setDownloadDirectory(config->downloadPath());
        savePath = config->downloadPath().append("/"_L1).append(request->downloadFileName());
        request->accept();
    } else {
        savePath = askDownloadPath(request);
        if (savePath.isEmpty()) {
            request->cancel();
            return;
        }
        request->setDownloadDirectory(QFileInfo(savePath).dir().path());
        request->setDownloadFileName(QFileInfo(savePath).fileName());
        request->accept();
    }

    connect(request, &QWebEngineDownloadRequest::isFinishedChanged, this, [this, request, savePath]() {
        if (request->isFinished() && QFile::exists(savePath)) {
            DownloadEntry entry;
            entry.filePath = savePath;
            entry.mimeType = getMimeType(request->mimeType()).name();
            entry.fileSize = request->totalBytes();
            entry.timestamp = QDateTime::currentDateTime();

            addToHistory(entry);
        }
    });
}

void DownloadManager::addToHistory(const DownloadEntry &entry)
{
    beginInsertRows(QModelIndex(), m_downloads.count(), m_downloads.count());
    m_downloads.append(entry);
    endInsertRows();

    SqlHelper::insertEntry(entry);
}

void DownloadManager::clearHistory()
{
    beginResetModel();
    m_downloads.clear();
    endResetModel();
    SqlHelper::clearHistory();
}

void DownloadManager::open(QString file)
{
    auto url = QUrl::fromLocalFile(file);
    auto job = new KIO::OpenUrlJob(url);
    job->setUiDelegate(KIO::createDefaultJobUiDelegate());
    job->start();
}

void DownloadManager::highlight(QString file)
{
    KIO::highlightInFileManager({QUrl::fromLocalFile(file)});
}

QString DownloadManager::askDownloadPath(QWebEngineDownloadRequest *request)
{
    auto [filter, defaultDir] = getDownloadPath(request->mimeType());
    QString baseName = request->downloadFileName();
    QString filePath = QDir(defaultDir).filePath(baseName);

    int i = 1;
    while (QFile::exists(filePath))
        filePath = QDir(defaultDir).filePath(baseName + QStringLiteral(" (%1)").arg(i++));

    return QFileDialog::getSaveFileName(nullptr, u"Save File"_s, filePath, filter);
}

QMimeType DownloadManager::getMimeType(const QString &mimeString)
{
    QMimeDatabase db;
    return db.mimeTypeForName(mimeString);
}

QPair<QString, QString> DownloadManager::getDownloadPath(const QString &mimeString)
{
    auto type = getMimeType(mimeString);
    QString path = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    if (type.name().startsWith("image"_L1)) {
        path = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    } else if (type.name().startsWith("video"_L1)) {
        path = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    } else if (type.name().startsWith("audio"_L1)) {
        path = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    } else if (type.name().startsWith("text"_L1) || type.name().startsWith("application"_L1)) {
        path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    }

    return {type.filterString(), path};
}
