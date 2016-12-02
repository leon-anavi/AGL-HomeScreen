import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1

Window {
    visible: true
    flags: Qt.FramelessWindowHint
    width: container.width * container.scale
    height: container.height * container.scale
    title: 'HomeScreen'

    Image {
        id: container
        anchors.centerIn: parent
        width: 1080
        height: 1920
        scale: 0.7
        source: './images/AGL_HMI_Background_NoCar-01.png'

        ColumnLayout {
            anchors.fill: parent
            spacing: 0
            TopArea {
                id: topArea
                Layout.fillWidth: true
                Layout.preferredHeight: 218
            }

            Item {
                id: applicationArea
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight: 1920 - 218 - 215
            }

            MediaArea {
                id: mediaArea
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight: 215
            }
        }
    }
}
