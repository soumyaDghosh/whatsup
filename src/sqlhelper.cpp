#include "sqlhelper.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

using namespace Qt::Literals::StringLiterals;

QSqlDatabase SqlHelper::db()
{
    static QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE"_L1);
    if (!database.isOpen()) {
        database.setDatabaseName(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).append("/downloads.db"_L1));
        database.open();
        init();
    }
    return database;
}

void SqlHelper::init()
{
    QSqlQuery query(db());
    query.exec(
        "CREATE TABLE IF NOT EXISTS downloads ("
        "filePath TEXT, mimeType TEXT, "
        "fileSize INTEGER, timestamp TEXT)"_L1);
}

void SqlHelper::insertEntry(const DownloadEntry &e)
{
    QSqlQuery q(db());
    q.prepare("INSERT INTO downloads VALUES (?, ?, ?, ?)"_L1);
    q.addBindValue(e.filePath);
    q.addBindValue(e.mimeType);
    q.addBindValue(e.fileSize);
    q.addBindValue(e.timestamp.toString(Qt::ISODate));
    q.exec();
}

QList<DownloadEntry> SqlHelper::loadHistory()
{
    QList<DownloadEntry> list;
    QSqlQuery q(db());
    q.exec("SELECT * FROM downloads ORDER BY timestamp DESC"_L1);

    while (q.next()) {
        DownloadEntry e;
        e.filePath = q.value(0).toString();
        e.mimeType = q.value(1).toString();
        e.fileSize = q.value(2).toLongLong();
        e.timestamp = QDateTime::fromString(q.value(3).toString(), Qt::ISODate);
        list.append(e);
    }
    return list;
}

void SqlHelper::clearHistory()
{
    QSqlQuery q(db());
    q.exec("DELETE FROM downloads"_L1);
}
