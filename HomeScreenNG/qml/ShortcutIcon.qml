import QtQuick 2.6

MouseArea {
    id: root
    width: 195
    height: 216.8
    property string name: 'Home'
    property bool active: false
    Image {
        id: icon
        anchors.fill: parent
        source: './images/Shortcut/HMI_Shortcut_%1-01.png'.arg(root.name)
    }
    Image {
        anchors.fill: parent
        source: './images/Shortcut/HMI_Shortcut_%1_Active-01.png'.arg(root.name)
        opacity: 1.0 - icon.opacity
    }
    states: [
        State {
            when: root.active
            PropertyChanges {
                target: icon
                opacity: 0.0
            }
        }
    ]

    transitions: [
        Transition {
            NumberAnimation {
                properties: 'opacity'
                easing.type: Easing.OutExpo
            }
        }
    ]
}
