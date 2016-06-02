/*
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "include/daynightmode.h"
#include "daynightmode_adapter.h"
#include "daynightmode_proxy.h"
#include "popupwidget.h"
#include "include/popup.h"
#include "popup_adapter.h"
#include "popup_proxy.h"

#include "statusbarwidget.h"
#include "controlbarwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

// from daynightmode_adapter.h
public Q_SLOTS:
    void setDayNightMode(int mode);
    inline int getDayNightMode()
    {
        return (int)m_dayNightMode;
    }

// from popup_adapter.h
public Q_SLOTS: // METHODS
    void showPopup(int type, const QString &text);

protected:
    // called when the translator loaded a new language set
    void changeEvent(QEvent* event);

private:
    Ui::MainWindow *mp_ui;

    StatusBarWidget *mp_statusBarWidget;
    org::agl::daynightmode *mp_dBusDayNightMode_StatusBarWidget;
    ControlBarWidget *mp_controlBarWidget;
    org::agl::daynightmode *mp_dBusDayNightMode_ControlBarWidget;

    // dbus daynightmode
    SystemDayNight::eDayNightMode m_dayNightMode;
    DaynightmodeAdaptor *mp_daynightmodeAdaptor;

    // dbus popup
    PopupAdaptor *mp_popupAdaptor;
    org::agl::popup *mp_dBusPopup;

    PopupWidget *mp_popupWidget;
};

#endif // MAINWINDOW_H
