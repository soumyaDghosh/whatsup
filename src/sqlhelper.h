#pragma once

#include "downloadmanager.h"
#include <QList>
#include <QSqlDatabase>

class SqlHelper
{
public:
    static void init();
    static void insertEntry(const DownloadEntry &entry);
    static QList<DownloadEntry> loadHistory();
    static void clearHistory();

private:
    static QSqlDatabase db();
};
