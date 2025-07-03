/**
 * SPDX-FileCopyrightText: 2025 Soumyadeep Ghosh (সৌম্যদীপ ঘোষ) <soumyadeepghosh2004@zohomail.in>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <QApplication>
#include <QtWebEngineQuick>
#include <QQuickWebEngineProfile>
#include <KLocalizedQmlContext>
#include <KLocalizedString>
#include <QtQml>
#include <QQuickStyle>
#include <KAboutData>

#include "config.h"
#include "downloadmanager.h"

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


    auto *profile = new QQuickWebEngineProfile(QStringLiteral("whatsup"), &app);

    QObject::connect(profile, &QQuickWebEngineProfile::downloadRequested, DownloadManager::self(), &DownloadManager::downloadFile);

    auto config = Config::self();

    if (config->storePerm()) {
        profile->setPersistentPermissionsPolicy(QQuickWebEngineProfile::PersistentPermissionsPolicy::StoreOnDisk);
    }

    profile->setHttpUserAgent(
        QStringLiteral("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/125.0.0.0 Safari/537.36")
    );

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("webProfile"), profile);

    KLocalization::setupLocalizedContext(&engine);
    engine.loadFromModule(QStringLiteral("io.github.soumyadghosh.whatsup"), QStringLiteral("Main"));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
