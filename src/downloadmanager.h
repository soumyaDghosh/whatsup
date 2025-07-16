/**
 * SPDX-FileCopyrightText: 2025 Soumyadeep Ghosh (সৌম্যদীপ ঘোষ) <soumyadeepghosh2004@zohomail.in>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <QAbstractListModel>
#include <QWebEngineDownloadRequest>
#include <QtQml>

struct DownloadEntry {
    QString filePath;
    QString mimeType;
    qint64 fileSize;
    QDateTime timestamp;
};

class DownloadManager : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    enum Roles {
        FileNameRole = Qt::UserRole + 1,
        FileRole,
        FilePathRole,
        MimeTypeRole,
        FileSizeRole,
        TimestampRole,
        DateRole,
        TimeRole
    };
    Q_ENUM(Roles)

    explicit DownloadManager(QObject *parent = nullptr);

    Q_SLOT void downloadFile(QWebEngineDownloadRequest *request);
    Q_INVOKABLE void clearHistory();
    Q_INVOKABLE void open(QString file);
    Q_INVOKABLE void highlight(QString file);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<DownloadEntry> m_downloads;

    static QString askDownloadPath(QWebEngineDownloadRequest *request);
    static QPair<QString, QString> getDownloadPath(const QString &mimeString);
    static QMimeType getMimeType(const QString &mimeString);
    void addToHistory(const DownloadEntry &entry);
};
