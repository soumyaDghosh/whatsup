/**
 * SPDX-FileCopyrightText: 2025 Soumyadeep Ghosh (সৌম্যদীপ ঘোষ) <soumyadeepghosh2004@zohomail.in>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

pragma ComponentBehavior: Bound

import QtQuick
import QtWebEngine

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.components as Components

import io.github.soumyadghosh.whatsup

Components.MessageDialog {
    property string name
    property webEnginePermission permission
    padding: Kirigami.Units.largeSpacing
    dialogType: Components.MessageDialog.Warning
    iconName: "dialog-question"
    standardButtons: Kirigami.Dialog.Yes | Kirigami.Dialog.No
    onAccepted: {
        permission.grant();
        PermissionManager.load();
        close();
    }
    onRejected: {
        permission.deny();
        PermissionManager.load();
        close();
    }
    title: i18n("Permission Request")
    dontShowAgainName: `Allow${name}`
    configGroupName: "Permissions"
    Kirigami.SelectableLabel {
        id: requestLabel
        text: i18n("WhatsApp requires access to %1,\nDo you want to allow it?", name)
        Kirigami.FormData.label: ""
        textFormat: Text.StyledText
        wrapMode: Text.Wrap
    }
}
