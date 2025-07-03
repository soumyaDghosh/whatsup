/**
 * SPDX-FileCopyrightText: 2025 Soumyadeep Ghosh (সৌম্যদীপ ঘোষ) <soumyadeepghosh2004@zohomail.in>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <QDir>
#include <QStandardPaths>

#include "downloadmanager.h"

using namespace Qt::Literals::StringLiterals;

void DownloadManager::downloadFile(QWebEngineDownloadRequest *request)
{
    auto requestDir = QDir(request->downloadDirectory());
    auto fileName = request->downloadFileName();

    if (requestDir == QDir::currentPath()) {
        requestDir = QDir(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    }

    QString filePath = requestDir.filePath(fileName);
    int i = 1;
    while (QFile::exists(filePath)) {
        filePath = requestDir.filePath(fileName.append(u" (%1)"_s.arg(i++)));
    }

    QMimeDatabase db;
    QMimeType mimeType = db.mimeTypeForName(request->mimeType());
    QString saveLocation = QFileDialog::getSaveFileName(nullptr, u"Save File"_s, filePath, mimeType.filterString());

    if (saveLocation.isEmpty()) {
        request->cancel();
        return;
    }

    request->setDownloadFileName(saveLocation);
    request->accept();
}

DownloadManager *DownloadManager::self()
{
    static DownloadManager instance;
    return &instance;
}
