#include <QtQml>
// #include <QAbstractListModel>
#include <QQuickWebEngineProfile>
#include <QStandardPaths>
#include <QUaParser>

using namespace Qt::Literals::StringLiterals;

class ProfileManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QQuickWebEngineProfile *profile MEMBER m_profile CONSTANT)

public:
    ProfileManager(QObject *parent = nullptr);

    Q_INVOKABLE void setProfile(const QUserAgent *agent);
    Q_INVOKABLE QQuickWebEngineProfile *getProfile()
    {
        return m_profile;
    }

private:
    QQuickWebEngineProfile *m_profile;
};
