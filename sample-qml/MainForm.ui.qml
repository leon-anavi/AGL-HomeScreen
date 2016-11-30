import QtQuick 2.6

Rectangle {
    id: rectangle1
    property alias mouseArea: mouseArea

    width: 360
    height: 360
    color: "#dfe259"
    property alias layout_permission: layout_permission
    z: 2147483646
    property alias image1: image1

    MouseArea {
        id: mouseArea
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent

        Rectangle {
            id: layout_permission
            x: 35
            y: 53
            width: 160
            height: 47
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#ffffff"
                }

                GradientStop {
                    position: 1
                    color: "#ecfcec"
                }
            }
            signal buttonClick()


            Text {
                id: text1
                x: 5
                y: 9
                width: 151
                height: 30
                color: "#f25728"
                text: qsTr("Ask layout permission")
                style: Text.Normal
                styleColor: "#86ecae"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.NoWrap
                font.pixelSize: 12
            }
        }

        Image {
            id: image1
            x: -2
            y: 90
            width: 360
            height: 267
            opacity: 0.2
            z: 0
            source: "agl-image.png"
        }
    }

    Text {
        text: "AGL sample app"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 326
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 17
        font.italic: true
        font.bold: true
        anchors.horizontalCenterOffset: -104
    }
}
