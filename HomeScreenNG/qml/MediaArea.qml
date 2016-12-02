import QtQuick 2.7
import QtQuick.Controls 2.0

StackView {
    id: root
    width: 1080
    height: 215

    initialItem: blank

    Component {
        id: blank
        MediaAreaBlank {
            MouseArea {
                anchors.fill: parent
                onClicked: root.push(mouse.x < 540 ? music : radio)
            }
        }
    }

    Component {
        id: music
        MediaAreaMusic {
            MouseArea {
                anchors.fill: parent
                onClicked: root.pop()
            }
        }
    }

    Component {
        id: radio
        MediaAreaRadio {
            MouseArea {
                anchors.fill: parent
                onClicked: root.pop()
            }
        }
    }
}
