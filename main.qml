import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Explorer")
    property int placeId: UIData.selectedPlaceId
    property var currentPath: FileModel.currentPath
    onPlaceIdChanged: {
        textAddress.text = UIData.getLocation()
        FileModel.openDirectory(UIData.getLocation(),false)
    }
    onCurrentPathChanged: {
        textAddress.text = currentPath
    }

    OverwriteDialog {
        id: overwriteDialog
        modal: true
        visible: false
        width: 0.5 * root.width
        height: 0.5 * root.height
        anchors.centerIn: parent
        onAccepted: {
            UIData.overwriteApproved(true)
        }
        onRejected: {
            UIData.overwriteApproved(false)
        }
    }

    Connections {
        target: UIData
        function onOverwriteApprovalRequested() {
            overwriteDialog.visible = true
        }
    }

    header: ToolBar {
        Column {
            anchors.fill: parent
            RowLayout {
                width: parent.width
                ToolButton {
                    icon.source: "qrc:/icons/chevron-left.svg"
                    text: "Back"
                    onClicked: FileModel.openDirectory("..",true)
                }
                ToolButton {
                    icon.source: "qrc:/icons/chevron-right.svg"
                    text: "Forward"
                }
                ToolButton {
                    icon.source: "qrc:/icons/refresh.svg"
                    text: "Refresh"
                    onClicked: FileModel.refresh()
                }
                ToolSeparator {}
                TextField {
                    id: textAddress
                    text: "/home/ben/Documents/test/folderA"
                    Layout.fillWidth: true
                    onEditingFinished: {
                        FileModel.openDirectory(textAddress.text,false)
                    }
                }
                ToolButton {
                    text: "Go"
                    onClicked: FileModel.openDirectory(textAddress.text,false)
                }
                ToolButton {
                    icon.source: "qrc:/icons/menu.svg"
                    text: "Menu"
                }
            }

            RowLayout {
                ToolButton {
                    icon.source: "qrc:/icons/selection.svg"
                    text: "Selecting Mode"
                    onClicked: {
                        UIData.selectingMode = !UIData.selectingMode
                    }
                }
                ToolButton {
                    icon.source: "qrc:/icons/copy.svg"
                    text: "Copy"
                    enabled: UIData.selectingMode && UIData.selectedFilesCount > 0
                    onClicked: {
                        //copy
                        UIData.setOperation(1)
                    }

                }
                ToolButton {
                    icon.source: "qrc:/icons/cut.svg"
                    text: "Cut"
                    enabled: UIData.selectingMode && UIData.selectedFilesCount > 0
                    onClicked: {
                        //move
                        UIData.setOperation(2)
                    }
                }
                ToolButton {
                    icon.source: "qrc:/icons/paste.svg"
                    text: "Paste"
                    enabled: UIData.selectedFilesCount > 0
                    onClicked: {
                        var op = UIData.getOperation();
                        console.log(`op: ${op}`)
                        if (op == 1) {
                            UIData.copySelectedFilesToCurrentPath()
                        } else if (op == 2) {
                            UIData.moveSelectedFilesToCurrentPath()
                        }
                    }
                }
                ToolButton {
                    icon.source: "qrc:/icons/rename.svg"
                    text: "Rename"
                    enabled: UIData.selectingMode && UIData.selectedFilesCount == 1
                    onClicked: {
                        FileModel.renameForSelectedFile()
                        FileModel.update()
                    }
                }

                ToolButton {
                    icon.source: "qrc:/icons/info.svg"
                    text: "Information"
                    enabled: UIData.selectingMode
                    onClicked: {
                        var d = Qt.createQmlObject('import QtQuick;
FileInfoDialog { visible: true; width: 0.5 * root.width; height: 0.6 * root.height; anchors.centerIn: parent }
', root, 'infoDialog');
                    }
                }
            }
        }
    }
    SplitView {
        anchors.fill: parent
        ScrollView {
            SplitView.preferredWidth: 200
            contentHeight: expandDevices.height + expandPlaces.height + expandCloud.height + 30
            height: root.height
            Expander {
                id: expandDevices
                title: "Device"
                items: PlaceDirPairsModel.getGroupPlaces(0)
                width: parent.width
                expanded: true
            }
            Expander {
                id: expandPlaces
                anchors.top: expandDevices.bottom
                title: "Common"
                items: PlaceDirPairsModel.getGroupPlaces(1)
                width: parent.width
                expanded: true
            }
            Expander {
                id: expandCloud
                anchors.top: expandPlaces.bottom
                title: "Online"
                items: PlaceDirPairsModel.getGroupPlaces(2)
                width: parent.width
                expanded: true
            }
        }
        Item {
            id: view
            GridView {
                model: FileModel
                cellWidth: view.width
                cellHeight: 48
                anchors.fill: parent
                delegate: FileEntry {
                    fileName: model.fileName
                    fileSize: model.fileSize
                    isChecked: model.isChecked
                    isDirectory: model.isDirectory
                    isRenaming: model.isRenaming
                }
                ScrollBar.vertical: ScrollBar {

                }
            }
        }
    }

}
