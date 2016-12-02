import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import HomeScreen 1.0

Item {
    id: root
    width: 295
    height: 218

    property date now: new Date
    Timer {
        interval: 100; running: true; repeat: true;
        onTriggered: root.now = new Date
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 295 - 76
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 40
                spacing: 0
                Label {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    text: Qt.formatDate(now, 'dddd').toUpperCase()
                    font.family: 'Roboto'
                    font.pixelSize: 13
                    color: 'white'
                    verticalAlignment:  Text.AlignVCenter
//                    Rectangle {
//                        anchors.fill: parent
//                        anchors.margins: 5
//                        color: 'red'
//                        border.color: 'blue'
//                        border.width: 1
//                        z: -1
//                    }
                }
                Label {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    text: Qt.formatTime(now, 'h:mm ap').toUpperCase()
                    font.family: 'Roboto'
                    font.pixelSize: 40
                    color: 'white'
                    horizontalAlignment:  Text.AlignHCenter
                    verticalAlignment:  Text.AlignVCenter
                }
                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight: 20
                    Image {
                        source: './images/Weather/WeatherIcons_Rain-01.png'
                    }
                    Label {
                        text: '64°F'
                        color: 'white'
                        font.family: 'Helvetica'
                        font.pixelSize: 32
                    }
                }
            }
        }
        ColumnLayout {
            id: icons
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 76
            spacing: -10
            Repeater {
                model: StatusBarModel {}
                delegate: Image {
                    source: model.modelData
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
    }
}
