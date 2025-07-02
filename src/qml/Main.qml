/**
 * SPDX-FileCopyrightText: 2025 Soumyadeep Ghosh (সৌম্যদীপ ঘোষ) <soumyadeepghosh2004@zohomail.in>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick

import org.kde.kirigami as Kirigami

Kirigami.ApplicationWindow {
    id: root
    width: 1024
    height: 720
    title: qsTr("WhatsUp")

    WebEnginePage {
        id: webEnginePage
    }

    globalDrawer: WhatsupDrawer {
        webEngineView: webEnginePage.view
    }

    pageStack.initialPage: webEnginePage

    RequestPermission {
        id: requestPermDialog
    }
}
