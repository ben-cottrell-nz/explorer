import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: item
    property string title: "?"
    property int group: ["Device","Common","Internet"].indexOf(title)
    property bool expanded: false
    property var items
    height: txtTitle.height + listView.height
    anchors.margins: 16
    clip: true
    Label {
        anchors.margins: 16
        height: 30
        x: 16
        id: txtTitle
        elide: Text.ElideRight
        text: title
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        width: parent.width - 16
        MouseArea {
            anchors.fill: parent
            onClicked: expanded = !expanded
        }
        ToolButton {
            id: imgExpander
            width: 24
            height: 24
            anchors.right: parent.right
            rotation: expanded ? 90 : -90
            icon.source: "qrc:/icons/chevron.svg"
            display: "IconOnly"
            onClicked: expanded = !expanded
            Behavior on rotation {
                NumberAnimation { duration: 300 }
            }
        }
    }
    Rectangle {
        id: r
        color: "transparent"
        width: parent.width
        height: expanded ? listView.contentHeight : 0
        anchors.top: txtTitle.bottom
        Component.onCompleted: {
            console.log(listView.implicitHeight)
        }
        Behavior on height {
            NumberAnimation { easing.type: Easing.InCubic; duration: 300 }
        }

        ListView {
            id: listView
            interactive: false
            visible: r.height > 0
            height: r.height
            width: parent.width
            model: items
            transitions: []
            delegate:
                Rectangle {
                color: modelData.getPlaceId() === UIData.selectedPlaceId
                                                      ? "#454545" : "transparent"
                width: listView.width
                height: 30
                id: rect
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        UIData.selectedPlaceId = modelData.getPlaceId()
                    }
                }
                Label {
                    text: modelData != null ? modelData.getPlaceName() : ''
                    elide: Text.ElideRight
                    height: rect.height
                    x: 16
                    verticalAlignment: Text.AlignVCenter
                }
            }
            highlightResizeDuration: 0
            focus: true
        }
    }
}
