/**
 * SPDX-FileCopyrightText: 2025 Soumyadeep Ghosh (সৌম্যদীপ ঘোষ) <soumyadeepghosh2004@zohomail.in>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <QDir>
#include <QStandardPaths>

#include "config.h"
#include "downloadmanager.h"

using namespace Qt::Literals::StringLiterals;

void DownloadManager::downloadFile(QWebEngineDownloadRequest *request)
{
    auto config = Config::self();

    if (!config->askDownloadPath()) {
        request->setDownloadDirectory(config->downloadPath());
        request->accept();
        return;
    }

    auto saveLocation = askDownloadPath(request);

    if (saveLocation.isEmpty()) {
        request->cancel();
        return;
    }

    request->setDownloadFileName(saveLocation);
    request->accept();
}

const QString DownloadManager::askDownloadPath(QWebEngineDownloadRequest *request)
{
    auto downloadPath = getDownloadPath(request->mimeType());
    auto requestDir = QDir(downloadPath.second);
    auto fileName = request->downloadFileName();

    QString filePath = requestDir.filePath(fileName);
    int i = 1;
    while (QFile::exists(filePath)) {
        filePath = requestDir.filePath(fileName.append(u" (%1)"_s.arg(i++)));
    }

    QString saveLocation = QFileDialog::getSaveFileName(nullptr, u"Save File"_s, filePath, downloadPath.first);

    return saveLocation;
}

const QPair<QString, QString> DownloadManager::getDownloadPath(const QString &mimeString)
{
    QMimeDatabase db;
    QMimeType mimeType = db.mimeTypeForName(mimeString);
    QString filterString = mimeType.filterString();
    QString location;

    location = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    if (mimeType.name().contains("image"_L1)) {
        location = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    }
    if (mimeType.name().contains("video"_L1)) {
        location = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    }
    if (mimeType.name().contains("audio"_L1)) {
        location = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    }
    if (mimeType.name().contains("text"_L1) || mimeType.name().contains("application"_L1)) {
        location = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    }

    return {filterString, location};
}

DownloadManager *DownloadManager::self()
{
    static DownloadManager instance;
    return &instance;
}
