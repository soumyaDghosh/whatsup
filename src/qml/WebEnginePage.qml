/**
 * SPDX-FileCopyrightText: 2025 Soumyadeep Ghosh (সৌম্যদীপ ঘোষ) <soumyadeepghosh2004@zohomail.in>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import QtWebEngine

import io.github.soumyadghosh.whatsup

ColumnLayout {
    property alias view: webEngineView
    readonly property string originUrl: "https://web.whatsapp.com"
    WebEngineView {
        id: webEngineView
        focus: true
        Layout.fillWidth: true
        Layout.fillHeight: true
        url: originUrl
        lifecycleState: WebEngineView.LifecycleState.Active
        profile: webProfile
        zoomFactor: Config.zoomFactor / 100

        onRenderProcessTerminated: (status, code) => {
            if (status !== WebEngineView.NormalTerminationStatus)
                reloadLater();
        }

        onPermissionRequested: permission => {
            var result = PermissionManager.handlePermission(originUrl, permission);
            requestPermDialog.name = result;
            requestPermDialog.permission = permission;
            requestPermDialog.openDialog();
        }

        onContextMenuRequested: request => request.accepted = true

        onLoadingChanged: info => {
            if (info.status === WebEngineView.LoadFailedStatus) {
                webEngineView.visible = false;
                reloadButton.visible = true;
            }
        }

        function reloadLater() {
            Qt.callLater(() => webEngineView.reload());
        }

        Shortcut {
            sequences: [StandardKey.ZoomIn]
            onActivated: (webEngineView.zoomFactor += 0.10)
        }

        Shortcut {
            sequences: [StandardKey.ZoomOut]
            onActivated: (webEngineView.zoomFactor -= 0.10)
        }

        onVisibleChanged: {
            webEngineView.lifecycleState = visible ? WebEngineView.LifecycleState.Active : WebEngineView.LifecycleState.Frozen;
        }
    }

    Controls.Button {
        id: reloadButton
        text: qsTr("Reload")
        icon.name: "view-refresh"
        Layout.alignment: Qt.AlignHCenter
        visible: false
        onClicked: {
            visible = false;
            webEngineView.reload();
            webEngineView.visible = true;
        }
    }
}

// WebEngineView {
//     id: webEngineView
//     anchors.fill: parent
//     url: "https://web.whatsapp.com"
//     // profile.httpUserAgent: "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123.0.0.0 Safari/537.36"
//     profile: webProfile

//     onRenderProcessTerminated: (status, code) => {
//         console.log("WebEngine process terminated:", status, "Exit code:", code);
//         if (status !== WebEngineView.NormalTerminationStatus) {
//             webview.reload(); // or show an error page
//         }
//     }

//     onContextMenuRequested: (request) => {request.accepted = true;}

//     onLoadingChanged: (loadingInfo) => {
//         console.log(loadingInfo);
//         if(loadingInfo.status === WebEngineView.LoadFailedStatus) {
//             console.log(webview.visible);
//             reloadButton.visible = true;
//         }
//     }

//     Component.onCompleted: {
//         console.log(webProfile.persistentStoragePath);
//     }
// }
