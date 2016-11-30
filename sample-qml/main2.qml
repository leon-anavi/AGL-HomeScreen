import QtQuick 2.6
import QtQuick.Window 2.2

Window {
    visible: true
    width: 1080
    height: 1920
    title: qsTr("AGL sample qml app")

    MainForm {
        anchors.fill: parent
        mouseArea.onClicked: {
            Qt.quit();
        }
    }
    onVisibleChanged: {
        if(visible == true)
        {
            console.log("2:visible true")
            console.log("maybe it is okay")
        }
        else{
            console.log("visible2 false")
        }
    }

}
