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
import QtQuick.Controls 1.0

Image {
    width: 1080
    height: 215
    source: './images/Utility_Music_Background-01.png'

    ColumnLayout {
        anchors.fill: parent
        spacing: 1
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 107
            spacing: 10
            Image {
                source: './images/MediaMusic/AlbumArtwork.png'
                width: 105.298
                height: 110.179
                fillMode: Image.PreserveAspectFit
            }
            Label {
                text: 'Come Together - The Beatles'
                font.family: 'Roboto'
                font.pixelSize: 32
                color: 'white'
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 107
            Image {
                source: './images/MediaPlayer/AGL_MediaPlayer_BackArrow.png'
            }
            Image {
                source: './images/MediaPlayer/AGL_MediaPlayer_Player_Pause.png'
            }
            Image {
                source: './images/MediaPlayer/AGL_MediaPlayer_ForwardArrow.png'
            }

            ProgressBar {
                Layout.fillWidth: true
                Layout.preferredWidth: 2
            }

            Label {
                text: '2:12/4:19'
                font.family: 'Roboto'
                font.pixelSize: 20
            }
            Image {
                source: './images/MediaPlayer/AGL_MediaPlayer_Shuffle_Active.png'
            }
            Image {
                source: './images/MediaPlayer/AGL_MediaPlayer_Shuffle_Active.png'
            }
            ProgressBar {
                Layout.fillWidth: true
                Layout.preferredWidth: 1
            }
        }
    }
}
