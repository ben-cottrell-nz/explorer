import QtQuick
import QtQuick.Controls

Dialog {
    id: dialog
    title: "Warning"
    width: 800
    height: 600
    standardButtons: Dialog.Yes | Dialog.No
    Row {
        spacing: 18
    Image {
        source: "qrc:/icons/alert.svg"
        width: 32
        height: 32
    }
    Label {
        text: "Files already exist in the target directory.<br>Overwrite existing files?"
        wrapMode: Text.Wrap
        width: implicitWidth - 200
    }
    }
}
