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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <include/daynightmode.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mp_ui(new Ui::MainWindow),
    mp_statusBarWidget(0),
    mp_controlBarWidget(0),
    mp_settingsWidget(0),
    //mp_applauncherwidget(0),
    mp_popupWidget(0),
    mp_layoutHandler(new LayoutHandler()),
    mp_dBusDayNightModeProxy(0),
    mp_proximityAdaptor(0),
    mp_homeScreenControlInterface(0)
{
    // this has to be adopted to the system setup
    mp_dBusDayNightModeProxy = new org::agl::daynightmode("org.agl.homescreen.simulator", //"org.agl.systeminfoprovider"
                                                      "/",
                                                      QDBusConnection::sessionBus(),
                                                      0);
    QObject::connect(mp_dBusDayNightModeProxy, SIGNAL(dayNightMode(int)), this, SLOT(dayNightModeSlot(int)));

    mp_proximityAdaptor = new ProximityAdaptor((QObject*)this);

    // dbus setup
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/Proximity", this);
    dbus.registerService("org.agl.homescreen");

    // no window decoration
    setWindowFlags(Qt::FramelessWindowHint);

    mp_ui->setupUi(this);

    mp_statusBarWidget = new StatusBarWidget(this);
    mp_statusBarWidget->raise();
    // apply layout
    mp_statusBarWidget->move(0, 0);

    mp_controlBarWidget = new ControlBarWidget(this);
    mp_controlBarWidget->raise();
    // apply layout
    mp_controlBarWidget->move(0, 1920-60);

    mp_settingsWidget = new SettingsWidget(this);
    mp_settingsWidget->raise();
    // apply layout
    mp_settingsWidget->move(0, 60);
    //mp_settingsWidget->hide();

    /*mp_applauncherwidget = new AppLauncherWidget(this);
    mp_applauncherwidget->raise();
    // apply layout
    mp_applauncherwidget->move(0, 60);*/


    mp_popupWidget = new PopupWidget();
    mp_controlBarWidget->raise();
    // apply layout
    mp_popupWidget->move(0, 0);


    QObject::connect(mp_settingsWidget, SIGNAL(colorSchemeChanged()), this, SLOT(updateColorScheme()));
    QObject::connect(mp_settingsWidget, SIGNAL(colorSchemeChanged()), mp_statusBarWidget, SLOT(updateColorScheme()));
    QObject::connect(mp_settingsWidget, SIGNAL(colorSchemeChanged()), mp_controlBarWidget, SLOT(updateColorScheme()));
    QObject::connect(mp_settingsWidget, SIGNAL(colorSchemeChanged()), mp_settingsWidget, SLOT(updateColorScheme()));
    QObject::connect(mp_settingsWidget, SIGNAL(colorSchemeChanged()), mp_popupWidget, SLOT(updateColorScheme()));

    QObject::connect(mp_controlBarWidget, SIGNAL(settingsButtonPressed()), mp_settingsWidget, SLOT(raise()));
    //QObject::connect(mp_controlBarWidget, SIGNAL(homeButtonPressed()), mp_applauncherwidget, SLOT(raise()));
    QObject::connect(mp_controlBarWidget, SIGNAL(hideAppLayer()), mp_layoutHandler, SLOT(hideAppLayer()));

    //QObject::connect(mp_applauncherwidget, SIGNAL(newRequestsToBeVisibleApp(int)), mp_layoutHandler, SLOT(makeMeVisible(int)));
    //QObject::connect(mp_applauncherwidget, SIGNAL(showAppLayer()), mp_layoutHandler, SLOT(showAppLayer()));


    // apply color scheme
    updateColorScheme();

    // this is only useful during development and will be removed later
    setWindowIcon(QIcon(":/icons/home_day.png"));

   // mp_applauncherwidget->populateAppList();
    //mp_layoutHandler->setUpLayouts();

    mp_homeScreenControlInterface = new HomeScreenControlInterface(this);
    //    QObject::connect(mp_homeScreenControlInterface, SIGNAL(newRequestGetAllSurfacesOfProcess(int)), mp_layoutHandler, SLOT(requestGetAllSurfacesOfProcess(int)));
    QObject::connect(mp_homeScreenControlInterface, SIGNAL(newRequestGetSurfaceStatus(int)), mp_layoutHandler, SLOT(requestGetSurfaceStatus(int)));
    QObject::connect(mp_homeScreenControlInterface, SIGNAL(newRequestsToBeVisibleApp(int)), mp_layoutHandler, SLOT(makeMeVisible(int)));
    QObject::connect(mp_homeScreenControlInterface, SIGNAL(newRequestRenderSurfaceToArea(int, int)), mp_layoutHandler, SLOT(requestRenderSurfaceToArea(int,int)));
    QObject::connect(mp_homeScreenControlInterface, SIGNAL(newRequestRenderSurfaceToAreaAllowed(int, int)), mp_layoutHandler, SLOT(requestRenderSurfaceToAreaAllowed(int,int)));
    QObject::connect(mp_homeScreenControlInterface, SIGNAL(newRequestSurfaceIdToFullScreen(int)), mp_layoutHandler, SLOT(requestSurfaceIdToFullScreen(int)));

    QObject::connect(mp_popupWidget, SIGNAL(comboBoxResult(QString)), mp_layoutHandler, SLOT(setLayoutByName(QString)));
}

MainWindow::~MainWindow()
{
    delete mp_homeScreenControlInterface;

    delete mp_dBusDayNightModeProxy;

    delete mp_layoutHandler;

    delete mp_popupWidget;
    //delete mp_applauncherwidget;
    delete mp_settingsWidget;
    delete mp_controlBarWidget;
    delete mp_statusBarWidget;
    delete mp_proximityAdaptor;
    delete mp_ui;
}

void MainWindow::dayNightModeSlot(int mode)
{
    QSettings settings;
    settings.setValue("systemsettings/daynightmode", mode);
    // make sure that everything is written to the settings file before continuing
    settings.sync();

    updateColorScheme();
}

void MainWindow::setObjectDetected(bool detected)
{
    qDebug("setObjectDetected %s", detected ? "true" : "false");
    QSettings settings;
    settings.setValue("systemsettings/proximityobjectdetected", detected);
    // make sure that everything is written to the settings file before continuing
    settings.sync();

    updateColorScheme();
}

void MainWindow::updateColorScheme()
{
    QSettings settings;
    QSettings settings_cs(QApplication::applicationDirPath() +
                          "/colorschemes/" +
                          settings.value("systemsettings/colorscheme", "default").toString() +
                          "/" +
                          QString::number(settings.value("systemsettings/proximityobjectdetected", false).toBool()) +
                          "/" +
                          QString::number(settings.value("systemsettings/daynightmode", SystemDayNight::DAYNIGHTMODE_DAY).toInt()) +
                          ".ini",
                          QSettings::IniFormat);

    mp_ui->widget_background->setStyleSheet(settings_cs.value("MainWindow/widget_background_css").toString());
    mp_ui->widget_homeIcon->setStyleSheet(settings_cs.value("MainWindow/widget_homeIcon_css").toString());

    // update children
    mp_statusBarWidget->updateColorScheme();
    mp_controlBarWidget->updateColorScheme();
    mp_settingsWidget->updateColorScheme();
    //mp_applauncherwidget->updateColorScheme();
    mp_popupWidget->updateColorScheme();
}

void MainWindow::changeEvent(QEvent* event)
{
    if (QEvent::LanguageChange == event->type())
    {
        mp_ui->retranslateUi(this);
    }

    QMainWindow::changeEvent(event);
}

