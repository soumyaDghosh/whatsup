#include "profilemanager.h"
#include "config.h"
#include "downloadmanager.h"

ProfileManager::ProfileManager(QObject *parent)
    : QObject(parent)
{
    m_profile = new QQuickWebEngineProfile(QStringLiteral("whatsup"), parent);
    if (Config::self()->storePerm()) {
        m_profile->setPersistentPermissionsPolicy(QQuickWebEngineProfile::PersistentPermissionsPolicy::StoreOnDisk);
    }
}

void ProfileManager::setProfile(const QUserAgent *agent)
{
    if (agent == nullptr || agent->agentString().isEmpty()) {
        m_profile->setHttpUserAgent("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/138.0.0.0 Safari/537.36"_L1);
        return;
    }
    if (m_profile->httpUserAgent() == agent->agentString()) {
        return;
    }
    m_profile->setHttpUserAgent(agent->agentString());
}
