import QtQuick 2.7
import QtQuick.Layouts 1.1
import HomeScreen 1.0

Item {
    id: root
    width: 785
    height: 218

    ApplicationLauncher {
        id: launcher
    }

    ListModel {
        id: applicationModel
        ListElement {
            name: 'Home'
            application: 'launcher'
        }
        ListElement {
            name: 'Multimedia'
            application: 'musicplayer'
        }
        ListElement {
            name: 'HVAC'
            application: 'hvac'
        }
        ListElement {
            name: 'Navigation'
            application: 'navigation'
        }
    }

    property int currentIndex: -1 // TODO: to be moved to whereever right

    RowLayout {
        anchors.fill: parent
        spacing: 2
        Repeater {
            model: applicationModel
            delegate: ShortcutIcon {
                Layout.fillWidth: true
                Layout.fillHeight: true
                name: model.name
                active: model.index === root.currentIndex
                onClicked: {
                    root.currentIndex = active ? -1 : model.index
                    launcher.launch(model.application)
                }
            }
        }
    }
}
