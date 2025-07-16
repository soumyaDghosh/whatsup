import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import Qt.labs.platform

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.delegates as Delegates
import org.kde.kirigamiaddons.formcard as FormCard
import org.kde.kitemmodels as KItemModels

import io.github.soumyadghosh.whatsup

Kirigami.ScrollablePage {
    id: root
    title: qsTr("Download History")
    header: Kirigami.SearchField {
        id: filterField
        KeyNavigation.tab: listView
        KeyNavigation.down: listView
        autoAccept: false
    }

    ListView {
        id: listView
        model: KItemModels.KSortFilterProxyModel {
            id: filteredSnapsModel
            sourceModel: downloadManager
            filterString: filterField.text
        }
        clip: true
        spacing: Kirigami.Units.largeSpacing
        currentIndex: -1
        section.property: "date"
        section.delegate: Kirigami.ListSectionHeader {
            text: section
            width: ListView.view.width - ListView.view.leftMargin - ListView.view.rightMargin
        }
        delegate: Delegates.RoundedItemDelegate {
            id: downloadDelegate
            width: ListView.view.width - ListView.view.leftMargin - ListView.view.rightMargin
            required property string file
            required property string filePath
            required property string fileName
            required property string mimeType
            required property string fileSize
            required property string time
            required property int index

            text: fileName
            icon.name: mimeType
            icon.width: Kirigami.Units.iconSizes.medium
            icon.height: Kirigami.Units.iconSizes.medium
            contentItem: RowLayout {
                Delegates.SubtitleContentItem {
                    subtitle: `${filePath} - ${formatBytes(fileSize)} - ${time}`
                    itemDelegate: downloadDelegate
                }

                Controls.Button {
                    text: i18n("Open in File Manager")
                    icon.name: "document-open-folder-symbolic"
                    visible: downloadDelegate.hovered
                    onClicked: downloadManager.highlight(file)
                }

                Controls.Button {
                    id: button
                    text: i18n("Open")
                    icon.name: "document-open-symbolic"
                    visible: downloadDelegate.hovered
                    onClicked: downloadManager.open(file)
                }
            }
            onClicked: ListView.view.currentIndex = index
        }
    }

    function formatBytes(bytes) {
        const units = ["B", "KB", "MB", "GB", "TB"];
        let size = bytes;
        let unitIndex = 0;
        while (size >= 1024 && unitIndex < units.length - 1) {
            size /= 1024;
            ++unitIndex;
        }
        return size.toFixed(2) + " " + units[unitIndex];
    }
}
