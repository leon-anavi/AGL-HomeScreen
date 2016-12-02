import QtQuick 2.7
import QtQuick.Layouts 1.1

Image {
    width: 1920
    height: 218
    source: './images/TopSection_NoText_NoIcons-01.png'
    fillMode: Image.PreserveAspectCrop

    RowLayout {
        anchors.fill: parent
        spacing: 0
        ShortcutArea {
            id: shortcutArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 785
        }
        StatusArea {
            id: statusArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 295
        }
    }
}
