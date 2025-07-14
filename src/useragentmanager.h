#include <QAbstractListModel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStandardPaths>
#include <QUaParser>
#include <QtQml>
#include <qtmetamacros.h>
#include <quaparser.h>

class UserAgentManager : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    UserAgentManager(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void fetchUserAgents();
    Q_INVOKABLE void getUserAgentsFile();
    void addUserAgents(QList<QUserAgent *> userAgents);
    QList<QUserAgent *> userAgents() const
    {
        return m_userAgents;
    }
    bool jsonExists()
    {
        return QFile::exists(m_jsonPath);
    }
    Q_INVOKABLE int containsUserAgent(const QString userAgent);
    Q_INVOKABLE QUserAgent *getUserAgent(int index);

    Q_SLOT void saveUserAgentsFile(QNetworkReply *reply);

    Q_SIGNAL void userAgentsChanged(QString message);

private:
    QNetworkAccessManager *m_networkAccessManager;
    QList<QUserAgent *> m_userAgents;
    int m_index;
    inline static const QString m_jsonPath =
        QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation).append(QStringLiteral("/whatsup/useragents.json"));
    inline static const QString m_regexPath = QStandardPaths::locate(QStandardPaths::AppDataLocation, QStringLiteral("whatsup/regexes.yaml"));
};
