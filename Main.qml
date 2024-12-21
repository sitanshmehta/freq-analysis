import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
//import QtQuick.Window

ApplicationWindow {
    width: 640
    height: 480
    visible: true

    FileDialog {
        id: fileDialog
        nameFilters: ["Audio files (*.wav *.mp3 *.ogg *.m4a)"]
        onAccepted: {
            audio.load_from_file(selectedFile)
        }
    }

    Button {
        text: "Load Audio File"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        onClicked: fileDialog.open()
    }
}
