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
                }
                ToolSeparator {}
                TextField {
                    id: textAddress
                    Layout.fillWidth: true
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
                }
                ToolButton {
                    icon.source: "qrc:/icons/copy.svg"
                    text: "Copy"
                }
                ToolButton {
                    icon.source: "qrc:/icons/cut.svg"
                    text: "Cut"
                }
                ToolButton {
                    icon.source: "qrc:/icons/paste.svg"
                    text: "Paste"
                }
                ToolButton {
                    icon.source: "qrc:/icons/info.svg"
                    text: "Information"
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
            Component {
                id: itemDelegate
                Item {
                    width: view.width
                    Rectangle {
                        MouseArea {
                            anchors.fill: parent
                            onClicked: if (model.isDirectory) {
                                           FileModel.openDirectory(model.fileName,true)
                                       } else {
                                           FileModel.openFile(model.fileName)
                                       }
                        }
                        width: view.width
                        height: 48
                        color: model.isSelected ? "blue" : "transparent"
                            Image {
                                id: img
                                source: model.isDirectory ? "qrc:/icons/folder.svg" : "qrc:/icons/file-text.svg"
                                width: 48
                                height: 48
                            }

                            Label {
                                anchors.left: img.right
                                textFormat: Label.RichText
                                text: `${model.fileName}<br><b>${FileModel.readableFileSize(model.fileSize)}</b>`
                                elide: Label.ElideRight
                            }
                    }
                }
            }

            GridView {
                model: FileModel
                cellWidth: view.width
                cellHeight: 48
                anchors.fill: parent
                delegate: itemDelegate
                ScrollBar.vertical: ScrollBar {

                }
            }
        }
    }

}
