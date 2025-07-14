#include "useragentmanager.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QStandardPaths>
#include <QtConcurrent>

using namespace Qt::Literals::StringLiterals;

UserAgentManager::UserAgentManager(QObject *parent)
    : QAbstractListModel(parent)
    , m_networkAccessManager(new QNetworkAccessManager(this))
{
    connect(m_networkAccessManager, &QNetworkAccessManager::finished, this, &UserAgentManager::saveUserAgentsFile);

    connect(this, &UserAgentManager::userAgentsChanged, this, &UserAgentManager::fetchUserAgents);

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
    qDebug() << "Save User Agents File Called";
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Network Error:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    reply->deleteLater();

    QThreadPool::globalInstance()->start([=] {
        QFile file(m_jsonPath);
        QString fileName = file.fileName();

        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "Failed to open file:" << fileName;
            QMetaObject::invokeMethod(
                this,
                [fileName, this]() {
                    Q_EMIT userAgentsChanged(u"Failed to open file:"_s.append(fileName));
                },
                Qt::QueuedConnection);
        } else {
            file.write(data);
            file.close();
            qDebug() << "Saved to" << fileName;
            QMetaObject::invokeMethod(
                this,
                [fileName, this]() {
                    Q_EMIT userAgentsChanged(u"Saved to "_s.append(fileName));
                },
                Qt::QueuedConnection);
        }
    });
}

void UserAgentManager::addUserAgents(QList<QUserAgent *> userAgents)
{
    qDebug() << "Adding User Agents Called";
    m_userAgents.clear();
    for (auto ua : userAgents) {
        beginInsertRows(QModelIndex(), m_userAgents.count(), m_userAgents.count());
        m_userAgents.append(ua);
        endInsertRows();
    }
    qDebug() << "Count of User Agents:" << m_userAgents.count();
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

void UserAgentManager::getUserAgentsFile()
{
    m_networkAccessManager->get(QNetworkRequest(QUrl("https://jnrbsn.github.io/user-agents/user-agents.json"_L1)));
}

void UserAgentManager::fetchUserAgents()
{
    QFile file(UserAgentManager::m_jsonPath);

    if (!file.exists()) {
        qWarning() << "User Agents File doesn't Exist";
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << file.errorString();
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

    userAgents.waitForFinished();
    auto result = userAgents.result();
    addUserAgents(result);
}

// QUserAgent* UserAgentManager::validateUserAgent(QJsonArray *array) {
//     QtConcurrent::filteredReduced(array, []{

//     })
// }
