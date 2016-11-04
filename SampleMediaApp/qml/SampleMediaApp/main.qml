import QtQuick 2.0

Rectangle {
    width: 400
    height: 300


    Rectangle {
        width: parent.width / 2
        height: parent.height
        color: "yellow"

        Text {
            text: qsTr("SampleMediaApp")
            anchors.centerIn: parent
        }
    }

    Rectangle {
        width: parent.width / 2
        height: parent.height
        x: parent.width / 2
        color: "yellow"

        Text {
            text: qsTr("default size: 400x300px")
            anchors.centerIn: parent
        }
    }
}
