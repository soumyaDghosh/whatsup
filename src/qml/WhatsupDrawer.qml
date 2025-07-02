/**
 * SPDX-FileCopyrightText: 2025 Soumyadeep Ghosh (সৌম্যদীপ ঘোষ) <soumyadeepghosh2004@zohomail.in>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import QtWebEngine

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.delegates as Delegates

import io.github.soumyadghosh.whatsup

Kirigami.GlobalDrawer {
    id: drawer
    modal: false
    visible: false
    property WebEngineView webEngineView
    readonly property bool drawerExpanded: Config.drawerExpanded
    readonly property int slimWidth: Kirigami.Units.iconSizes.large
    readonly property int wideWidth: Kirigami.Units.gridUnit * 10
    width: drawerExpanded ? wideWidth : slimWidth

    Kirigami.Theme.colorSet: Kirigami.Theme.View
    Kirigami.Theme.inherit: false

    contentItem: ColumnLayout {
        width: parent.width
        Behavior on width {
            NumberAnimation {
                easing.type: Easing.OutCubic
                duration: 150
            }
        }

        Controls.ButtonGroup {
            id: navGroup
            exclusive: true
        }

        Delegates.RoundedItemDelegate {
            Layout.fillWidth: true
            Controls.ToolTip.text: i18n("Toggle View")
            Controls.ToolTip.visible: hovered
            text: drawerExpanded ? Controls.ToolTip.text : ""
            icon.name: webEngineView.visible ? "password-show-off" : "password-show-on"
            icon.width: Kirigami.Units.iconSizes.medium
            icon.height: Kirigami.Units.iconSizes.medium
            onClicked: webEngineView.visible = !webEngineView.visible
        }

        Delegates.RoundedItemDelegate {
            Layout.fillWidth: true
            Controls.ToolTip.visible: hovered
            Controls.ToolTip.text: i18n("Reload the current page")
            text: drawerExpanded ? i18n("Reload") : ""
            icon.name: "view-refresh"
            icon.width: Kirigami.Units.iconSizes.medium
            icon.height: Kirigami.Units.iconSizes.medium
            onClicked: webEngineView.reload()
        }

        Delegates.RoundedItemDelegate {
            Layout.fillWidth: true
            Controls.ToolTip.visible: hovered
            Controls.ToolTip.text: i18n("Zoom In")
            text: drawerExpanded ? Controls.ToolTip.text : ""
            icon.name: "zoom-in-map"
            icon.width: Kirigami.Units.iconSizes.medium
            icon.height: Kirigami.Units.iconSizes.medium
            onClicked: webEngineView.zoomFactor === 5.0 ? webEngineView.zoomFactor : webEngineView.zoomFactor += 0.10
        }

        Delegates.RoundedItemDelegate {
            Layout.fillWidth: true
            Controls.ToolTip.visible: hovered
            Controls.ToolTip.text: i18n("Zoom Out")
            text: drawerExpanded ? Controls.ToolTip.text : ""
            icon.name: "zoom-out-map"
            icon.width: Kirigami.Units.iconSizes.medium
            icon.height: Kirigami.Units.iconSizes.medium
            onClicked: webEngineView.zoomFactor === 0.25 ? webEngineView.zoomFactor : webEngineView.zoomFactor -= 0.05
        }

        Delegates.RoundedItemDelegate {
            Layout.fillWidth: true
            Controls.ToolTip.visible: hovered
            Controls.ToolTip.text: i18n("Reset the zoom factor to the default value")
            text: drawerExpanded ? i18n("Reset Zoom") : ""
            icon.name: "edit-reset"
            icon.width: Kirigami.Units.iconSizes.medium
            icon.height: Kirigami.Units.iconSizes.medium
            onClicked: webEngineView.zoomFactor = Config.zoomFactor / 100;
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
            }
        }

        Item {
            Layout.fillHeight: true
        }

        Component {
            id: settings
            SettingsPage {}
        }

        Delegates.RoundedItemDelegate {
            autoExclusive: true
            Controls.ButtonGroup.group: navGroup
            Layout.fillWidth: true
            Controls.ToolTip.visible: hovered
            Controls.ToolTip.text: i18n("Settings")
            text: drawerExpanded ? Controls.ToolTip.text : ""
            icon.name: "settings-configure"
            icon.width: Kirigami.Units.iconSizes.medium
            icon.height: Kirigami.Units.iconSizes.medium
            onClicked: pageStack.layers.push(settings);
        }

        Delegates.RoundedItemDelegate {
            Controls.ButtonGroup.group: navGroup
            Layout.fillWidth: true
            Controls.ToolTip.visible: hovered
            Controls.ToolTip.text: drawerExpanded ? i18n("Collapse Sidebar") : i18n("Expand Sidebar")
            text: drawerExpanded ? i18n("Collapse Sidebar") : ""
            icon.name: drawerExpanded ? "sidebar-collapse" : "sidebar-expand"
            icon.width: Kirigami.Units.iconSizes.medium
            icon.height: Kirigami.Units.iconSizes.medium
            onClicked: {
                Config.drawerExpanded = !Config.drawerExpanded;
                Config.save();
            }
        }
    }
}
