#include "useragentmanager.h"
#include <QFile>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QStandardPaths>
#include <QtConcurrent>

#include <KIO/JobUiDelegateFactory>
#include <KIO/OpenUrlJob>

using namespace Qt::Literals::StringLiterals;

UserAgentManager::UserAgentManager(bool start, QObject *parent)
    : QAbstractListModel(parent)
    , m_networkAccessManager(new QNetworkAccessManager(this))
    , m_start(start)
{
    connect(m_networkAccessManager, &QNetworkAccessManager::finished, this, &UserAgentManager::saveUserAgentsFile);

    connect(this, &UserAgentManager::userAgentsChanged, this, &UserAgentManager::fetchUserAgents);

    connect(&m_watcher, &QFutureWatcher<QList<QUserAgent *>>::finished, this, [this] {
        auto result = m_watcher.result();
        addUserAgents(result);
    });

    if (QFile(m_jsonPath).exists())
        fetchUserAgents();
    else
        getUserAgentsFile();
}

int UserAgentManager::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_userAgents.count();
}

QVariant UserAgentManager::data(const QModelIndex &index, int role) const
{
    auto userAgent = m_userAgents.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
        return userAgent->browser()->toString().append(QChar::Space).append(userAgent->os()->family());
    case Qt::DecorationRole:
        return userAgent->browser()->family();
    case Qt::UserRole:
        return QVariant::fromValue(&userAgent);
    }
    return QVariant();
}

void UserAgentManager::saveUserAgentsFile(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        Q_EMIT error(u"Network Error: "_s.append(reply->errorString()));
        reply->deleteLater();
        m_isFetching = !m_isFetching;
        Q_EMIT isFetchingChanged(m_isFetching);
        return;
    }

    QByteArray data = reply->readAll();
    reply->deleteLater();

    QFile file(m_jsonPath);
    QString fileName = file.fileName();

    if (!file.open(QIODevice::WriteOnly)) {
        m_isFetching = !m_isFetching;
        Q_EMIT error(u"Failed to open file:"_s.append(fileName));
        Q_EMIT isFetchingChanged(m_isFetching);
    } else {
        file.write(data);
        file.close();
        m_isFetching = !m_isFetching;
        Q_EMIT userAgentsChanged(u"Saved to "_s.append(fileName));
        Q_EMIT isFetchingChanged(m_isFetching);
    }
}

void UserAgentManager::addUserAgents(QList<QUserAgent *> userAgents)
{
    beginResetModel();
    m_userAgents = userAgents;
    endResetModel();
}

int UserAgentManager::containsUserAgent(const QString userAgent)
{
    for (int i = 0; i < m_userAgents.count(); i++) {
        auto ua = m_userAgents.at(i);
        if (ua->agentString() == userAgent)
            return i;
    }
    return -1;
}

QUserAgent *UserAgentManager::getUserAgent(int index)
{
    return m_userAgents.at(index);
}

void UserAgentManager::open()
{
    auto url = QUrl::fromLocalFile(m_jsonPath);
    auto job = new KIO::OpenUrlJob(url);
    job->setUiDelegate(KIO::createDefaultJobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, nullptr));
    job->start();
}

void UserAgentManager::getUserAgentsFile()
{
    m_isFetching = !m_isFetching;
    Q_EMIT isFetchingChanged(m_isFetching);

    m_networkAccessManager->get(QNetworkRequest(QUrl("https://jnrbsn.github.io/user-agents/user-agents.json"_L1)));
}

void UserAgentManager::fetchUserAgents()
{
    QFile file(UserAgentManager::m_jsonPath);

    if (!file.exists()) {
        qWarning() << "User Agents File doesn't Exist";
        Q_EMIT error(u"User Agents File doesn't Exist"_s);
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << file.errorString();
        Q_EMIT error(u"Failed to open file: "_s.append(file.errorString()));
        return;
    }

    auto data = file.readAll();
    auto jsonArray = QJsonDocument::fromJson(data).array();

    QFuture<QList<QUserAgent *>> userAgents = QtConcurrent::filteredReduced(
        jsonArray,
        [](QJsonValue value) {
            if (value.toString().contains("Linux"_L1)) {
                return true;
            };
            return false;
        },
        [](QList<QUserAgent *> &list, QJsonValue value) {
            QUserAgentParser parser = QUserAgentParser(UserAgentManager::m_regexPath);
            list.append(parser.parse(value.toString()));
            std::sort(list.begin(), list.end(), [](QUserAgent *a, QUserAgent *b) {
                if (a->browser()->family() == b->browser()->family()) {
                    return a->browser()->major() < b->browser()->major();
                }
                return a->browser()->family() < b->browser()->family();
            });
        });

    if (m_start) {
        m_start = !m_start;
        userAgents.waitForFinished();
        auto result = userAgents.result();
        addUserAgents(result);
    } else {
        m_watcher.setFuture(userAgents);
    }
}
