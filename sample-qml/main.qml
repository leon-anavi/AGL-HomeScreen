import QtQuick 2.6
import QtQuick.Window 2.2
import "apps/HVAC"
import "imports"
import "dummyimports"

Window {
    visible: true
    width: 1080
    height: 1920
    title: qsTr("AGL sample qml app")

    //MainForm {
    //    anchors.fill: parent
    //    mouseArea.onClicked: {
    //        Qt.quit();
    //    }
    //}
    HVAC {
        id: hvc
        width: 1080
        height: 1920
        visible: true
    }
    onVisibleChanged: {
        if(visible == true)
        {
            console.log("visible true")
            console.log("maybe it is okay")
            hsa.consoleout()
            hsa.refresh();
        }
        else{
            console.log("visible false")
        }
    }

}
