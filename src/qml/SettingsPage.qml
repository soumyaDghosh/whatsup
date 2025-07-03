/**
 * SPDX-FileCopyrightText: 2025 Soumyadeep Ghosh (সৌম্যদীপ ঘোষ) <soumyadeepghosh2004@zohomail.in>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import Qt.labs.platform

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

import io.github.soumyadghosh.whatsup

FormCard.FormCardPage {
    id: root
    property var webEngineView

    title: i18nc("@title:window", "Settings")

    FormCard.FormHeader {
        title: i18n("General Settings")
    }

    FormCard.FormCard {
        FormCard.AbstractFormDelegate {
            id: pageZoomFactor
            contentItem: RowLayout {
                Controls.Label {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter

                    text: i18n("Page Zoom Factor:")
                    elide: Text.ElideRight
                    wrapMode: Text.Wrap
                    maximumLineCount: 2
                }
                Controls.SpinBox {
                    id: spinbox
                    value: Config.zoomFactor
                    from: 25
                    to: 500
                    stepSize: 5
                    readonly property int decimals: 2
                    textFromValue: (v, l) => Number(v/100).toLocaleString(l, 'f', 2)
                    validator: DoubleValidator {
                        bottom: Math.min(spinbox.from, spinbox.to)
                        top:  Math.max(spinbox.from, spinbox.to)
                    }
                    valueFromText: (t, l) => Math.round(Number.fromLocaleString(l, t) * 100)
                    onValueChanged: {
                        Config.zoomFactor = value;
                        Config.save();
                    }
                    Keys.onReturnPressed: {
                        spinbox.focus = false;
                    }
                }
            }
        }

        FormCard.FormDelegateSeparator {}

        FormCard.FormSwitchDelegate {
            id: askDownload
            text: i18n("Ask before download")
            description: i18n("Always ask where to download files")
            checked: Config.askDownloadPath
            onCheckedChanged: {
                Config.askDownloadPath = checked;
                Config.save();
            }
        }
        
        FormCard.FormDelegateSeparator {
            above: askDownload
            below: downloadPath
            visible: !Config.askDownloadPath
        }

        FormCard.FormTextDelegate {
            id: downloadPath
            visible: !Config.askDownloadPath
            text: i18n("Auto-Download Path")
            description: Config.downloadPath
            trailing: Controls.Button {
                icon.name: "folder-add"
                onClicked: folderDialog.open();
            }
        }
    }

    FormCard.FormHeader {
        title: i18n("Website Permissions")
    }

    FormCard.FormCard {
        FormCard.FormSwitchDelegate {
            id: storeInDisk
            text: i18n("Store in Disk")
            description: i18n("Store permissions in disk")
            checked: Config.storePerm
            onCheckedChanged: {
                Config.storePerm = checked;
                Config.save();
            }
        }

        FormCard.FormDelegateSeparator {
            above: storeInDisk
            below: allowMicCheck
        }

        FormCard.FormSwitchDelegate {
            id: allowMicCheck
            text: i18n("Allow Microphone")
            description: i18n("Allow the website to use microphone")
            checked: Config.allowMicrophone
            onCheckedChanged: {
                Config.allowMicrophone = checked;
                Config.save();
            }
        }

        FormCard.FormDelegateSeparator {
            above: allowMicCheck
            below: allowCameraCheck
        }

        FormCard.FormSwitchDelegate {
            id: allowCameraCheck
            text: i18n("Allow Camera")
            description: i18n("Allow the website to use camera")
            checked: Config.allowCamera
            onCheckedChanged: {
                Config.allowCamera = checked;
                Config.save();
            }
        }
    }

    FormCard.FormCard {
        Layout.topMargin: Kirigami.Units.gridUnit

        FormCard.FormButtonDelegate {
            text: i18n("About Whatsup")
            icon.name: "xchat"
            onClicked: {
                Controls.ApplicationWindow.window.pageStack.layers.push(aboutPage);
            }

            Component {
                id: aboutPage
                FormCard.AboutPage {
                    aboutData: AboutData
                }
            }
        }

        FormCard.FormDelegateSeparator {}

        FormCard.FormButtonDelegate {
            text: i18n("About KDE")
            icon.name: "kde"
            onClicked: {
                Controls.ApplicationWindow.window.pageStack.layers.push(aboutKDE);
            }

            Component {
                id: aboutKDE
                FormCard.AboutKDEPage {}
            }
        }
    }

    Connections {
        target: Config

        function onConfigChanged() {
            webEngineView.reload();
        }
    }

    FolderDialog {
        id: folderDialog
        currentFolder: "file://" + Config.downloadPath
        onAccepted: {
            var path = URL(folder);
            Config.downloadPath = path.pathname;
            Config.save();
        }
    }
}
