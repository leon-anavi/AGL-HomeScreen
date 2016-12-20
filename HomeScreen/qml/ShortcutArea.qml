/*
 * Copyright (C) 2016 The Qt Company Ltd.
 * Copyright (C) 2016 Mentor Graphics Development (Deutschland) GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import QtQuick 2.2
import QtQuick.Layouts 1.1

Item {
    id: root
    width: 785
    height: 218


    ListModel {
        id: applicationModel
        ListElement {
            name: 'Home'
            application: ''
        }
        ListElement {
            name: 'Multimedia'
            application: 'mediaplayer@0.1'
        }
        ListElement {
            name: 'HVAC'
            application: 'hvac@0.1'
        }
        ListElement {
            name: 'Navigation'
            application: 'navigation@0.1'
        }
    }

    property int pid: -1

    RowLayout {
        anchors.fill: parent
        spacing: 2
        Repeater {
            model: applicationModel
            delegate: ShortcutIcon {
                Layout.fillWidth: true
                Layout.fillHeight: true
                name: model.name
                active: model.application === launcher.current
                onClicked: {
                    if (0 === model.index) {
                        appLauncherAreaLauncher.visible = true
                        applicationArea.visible = false
                        layoutHandler.hideAppLayer()
                        launcher.current = ''
                    }
                    else {
                        pid = launcher.launch(model.application)
                        if (1 < pid) {
                            applicationArea.visible = true
                            appLauncherAreaLauncher.visible = false
                            layoutHandler.makeMeVisible(pid)
                            layoutHandler.showAppLayer(pid)
                        }
                        else {
                            console.warn("app cannot be launched!")
                        }
                    }
                }
            }
        }
    }
}
