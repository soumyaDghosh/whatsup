/**
 * SPDX-FileCopyrightText: 2025 Soumyadeep Ghosh (সৌম্যদীপ ঘোষ) <soumyadeepghosh2004@zohomail.in>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <KAboutData>
#include <KLocalizedQmlContext>
#include <KLocalizedString>
#include <QApplication>
#include <QQuickStyle>
#include <QQuickWebEngineProfile>
#include <QUaParser>
#include <QtQml>
#include <QtWebEngineQuick>

#include "config.h"
#include "downloadmanager.h"
#include "profilemanager.h"
#include "useragentmanager.h"

using namespace Qt::Literals::StringLiterals;

int main(int argc, char *argv[]) {
    QtWebEngineQuick::initialize();
    
    // Workaround for https://bugs.kde.org/show_bug.cgi?id=438345
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu-compositing");

    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("whatsup");

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }

    KAboutData about(QStringLiteral("whatsup"),
                     i18n("WhatsUp"),
                     QStringLiteral("0.1"),
                     i18n("Whatsapp wrapper"),
                     KAboutLicense::GPL_V3,
                     i18n("© 2025 Soumyadeep Ghosh (সৌম্যদীপ ঘোষ)"));
    about.addAuthor(i18n("Soumyadeep Ghosh (সৌম্যদীপ ঘোষ)"), i18n("Maintainer"), QStringLiteral("soumyadghosh@ubuntu.com"));
    about.setOrganizationDomain("soumyadghosh.github.io");

    KAboutData::setApplicationData(about);

    auto *profile = new ProfileManager(&app);
    auto downloadManager = new DownloadManager(&app);
    QObject::connect(profile->getProfile(), &QQuickWebEngineProfile::downloadRequested, downloadManager, &DownloadManager::downloadFile);

    auto config = Config::self();

    if (config->downloadPath().isEmpty()) {
        config->setDownloadPath(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation).append("/WhatsUp"_L1));
    }

    QUserAgent *userAgent = nullptr;
    auto uaManager = new UserAgentManager(true);

    if (uaManager->userAgents().count() > 0) {
        userAgent = uaManager->userAgents().at(config->uAIndex());
    }

    profile->setProfile(userAgent);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("webProfile"), profile);
    engine.rootContext()->setContextProperty(QStringLiteral("uaManager"), uaManager);
    engine.rootContext()->setContextProperty(QStringLiteral("downloadManager"), downloadManager);

    KLocalization::setupLocalizedContext(&engine);
    engine.loadFromModule(QStringLiteral("io.github.soumyadghosh.whatsup"), QStringLiteral("Main"));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
