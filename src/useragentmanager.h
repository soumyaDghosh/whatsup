#include <QAbstractListModel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStandardPaths>
#include <QUaParser>
#include <QtQml>

class UserAgentManager : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("UserAgentManager must be created in C++ only")
    Q_PROPERTY(bool isFetching READ isFetching NOTIFY isFetchingChanged)

public:
    UserAgentManager(bool start, QObject *parent = nullptr);

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

    bool isFetching() const
    {
        return m_isFetching;
    }

public Q_SLOTS:
    void saveUserAgentsFile(QNetworkReply *reply);

Q_SIGNALS:
    void userAgentsChanged(QString message);
    void isFetchingChanged(bool isFetching);

private:
    QNetworkAccessManager *m_networkAccessManager;
    QList<QUserAgent *> m_userAgents;
    int m_progress;
    int m_index;
    bool m_isFetching = false;
    bool m_start;
    inline static const QString m_jsonPath =
        QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation).append(QStringLiteral("/whatsup/useragents.json"));
    inline static const QString m_regexPath = QStandardPaths::locate(QStandardPaths::AppDataLocation, QStringLiteral("whatsup/regexes.yaml"));
};
