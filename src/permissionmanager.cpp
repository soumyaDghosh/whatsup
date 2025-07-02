/**
 * SPDX-FileCopyrightText: 2025 Soumyadeep Ghosh (সৌম্যদীপ ঘোষ) <soumyadeepghosh2004@zohomail.in>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "permissionmanager.h"
#include "config.h"

using namespace Qt::Literals::StringLiterals;

QString PermissionManager::handlePermission(QString origin, QWebEnginePermission permission) {
    QString result;

    Q_ASSERT(QUrl(origin) == permission.origin());

    switch (permission.permissionType()) {
        case QWebEnginePermission::PermissionType::MediaAudioCapture:
            result = u"Microphone"_s;
            break;
        case QWebEnginePermission::PermissionType::MediaVideoCapture:
                result = u"Camera"_s;
            break;
        default:
            break;
    }

    return result;
}

void PermissionManager::load() {
    auto config = Config::self();
    config->read();
    config->load();
}

void PermissionManager::saveConfig(QWebEnginePermission permission, bool allow) {
    auto config = Config::self();
    switch (permission.permissionType()) {
        case QWebEnginePermission::PermissionType::MediaAudioCapture:
            config->setAllowMicrophone(allow);
            break;
        case QWebEnginePermission::PermissionType::MediaVideoCapture:
            config->setAllowCamera(allow);
            break;
        default:
            break;
    }
}
