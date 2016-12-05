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
#include "daynightmode_proxy.h"
#include "proximity_adaptor.h"

#include "homescreencontrolinterface.h"

#include "statusbarwidget.h"
#include "controlbarwidget.h"
#include "settingswidget.h"
//#include "applauncher.h"
#include "popupwidget.h"

#include "layouthandler.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

// day/night mode
public Q_SLOTS:
    void dayNightModeSlot(int mode);

// from proximity_adaptor.h
public Q_SLOTS:
    void setObjectDetected(bool detected);

public slots:
    void updateColorScheme();

protected:
    // called when the translator loaded a new language set
    void changeEvent(QEvent* event);


private:
    Ui::MainWindow *mp_ui;

    StatusBarWidget *mp_statusBarWidget;
    org::agl::daynightmode *mp_dBusDayNightMode_StatusBarWidget;
    ControlBarWidget *mp_controlBarWidget;
    org::agl::daynightmode *mp_dBusDayNightMode_ControlBarWidget;
    SettingsWidget *mp_settingsWidget;
    //AppLauncherWidget *mp_applauncherwidget;
    PopupWidget *mp_popupWidget;

    LayoutHandler *mp_layoutHandler;

    org::agl::daynightmode *mp_dBusDayNightModeProxy;

    ProximityAdaptor *mp_proximityAdaptor;

    HomeScreenControlInterface *mp_homeScreenControlInterface;
};

#endif // MAINWINDOW_H
