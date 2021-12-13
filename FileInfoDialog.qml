import QtQuick
import QtQuick.Controls

Dialog {
    id: dialog
    title: "Info"

    ScrollView {
        Label {
           text: {
               var str = "";
               UIData.selectedFiles().forEach((a,i) => {
                   str += a + "<br/>"
               });
               return str;
           }
        }
    }


    footer: Button {
        text: "OK"
        onClicked: dialog.close()
    }
}
