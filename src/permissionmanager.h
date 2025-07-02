/**
 * SPDX-FileCopyrightText: 2025 Soumyadeep Ghosh (সৌম্যদীপ ঘোষ) <soumyadeepghosh2004@zohomail.in>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <QObject>
#include <QtQml>
#include <QWebEnginePermission>

class PermissionManager : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    public:
        Q_INVOKABLE QString handlePermission(QString origin, QWebEnginePermission permission);

        Q_INVOKABLE void load();

        Q_INVOKABLE void saveConfig(QWebEnginePermission permission, bool allow);
};
