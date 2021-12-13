import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: view.width
    property string fileName
    property int fileSize
    property int isChecked
    property bool isRenaming
    property bool isDirectory
    onIsCheckedChanged: {
        console.log('isCheckedChanged')
        console.log(`isRenaming: ${isRenaming}`)
    }

    onIsRenamingChanged: {
        console.log(`renaming: ${isRenaming}`)
    }
    Connections {
        target: FileModel
        function onAllDataChanged() {
            isRenaming = model.isRenaming
            fileName = model.fileName
        }
    }

    CheckBox {
        id: checkbox
        visible: UIData.selectingMode
        onCheckStateChanged: {
            //isChecked = checkState == Qt.Checked
            console.log(isChecked)
            if (checkState == Qt.Checked) {
                UIData.setFileSelected(index, true)
                model.isChecked = true
            } else {
                UIData.setFileSelected(index, false)
                model.isChecked = true
            }
        }
    }
    Rectangle {
        x: UIData.selectingMode ? checkbox.width : 0
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
        color: "transparent"
        //color: isSelected ? "blue" : "transparent"
            Image {
                id: img
                source: isDirectory ? "qrc:/icons/folder.svg" : "qrc:/icons/file-text.svg"
                width: 48
                height: 48
            }
            TextField {
                text: `${fileName}`
                anchors.left: img.right
                visible: isRenaming
                onEditingFinished: {
                    model.isRenaming = false
                    FileModel.renameFile(index, text)
                }
            }

            Label {
                id: labelName
                anchors.left: img.right
                textFormat: Label.RichText
                text: `${fileName}<br><b>${FileModel.readableFileSize(fileSize)}</b>`
                visible: !isRenaming
                elide: Label.ElideRight
            }
    }
}
