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
#include "../interfaces/daynightmode.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mp_ui(new Ui::MainWindow),
    mp_statusBarWidget(0),
    mp_controlBarWidget(0),
    mp_settingsWidget(0),
    mp_popupWidget(0),
    mp_dayNightModeProxy(0)
{
    // this has to be adopted to the system setup
    mp_dayNightModeProxy = new org::agl::daynightmode("org.agl.homescreen.simulator", //"org.agl.systeminfoprovider"
                                                      "/",
                                                      QDBusConnection::sessionBus(),
                                                      0);
    QObject::connect(mp_dayNightModeProxy, SIGNAL(dayNightMode(int)), this, SLOT(dayNightModeSlot(int)));

    // dbus setup
    QDBusConnection dbus = QDBusConnection::sessionBus();

    dbus.registerObject("/MainWindow", this);
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
    mp_settingsWidget->hide();

    mp_popupWidget = new PopupWidget(this);
    mp_controlBarWidget->raise();
    // apply layout
    mp_popupWidget->move(0, 0);

    QObject::connect(mp_settingsWidget, SIGNAL(colorSchemeChanged()), this, SLOT(updateColorScheme()));
    QObject::connect(mp_settingsWidget, SIGNAL(colorSchemeChanged()), mp_statusBarWidget, SLOT(updateColorScheme()));
    QObject::connect(mp_settingsWidget, SIGNAL(colorSchemeChanged()), mp_controlBarWidget, SLOT(updateColorScheme()));
    QObject::connect(mp_settingsWidget, SIGNAL(colorSchemeChanged()), mp_settingsWidget, SLOT(updateColorScheme()));
    QObject::connect(mp_settingsWidget, SIGNAL(colorSchemeChanged()), mp_popupWidget, SLOT(updateColorScheme()));

    QObject::connect(mp_controlBarWidget, SIGNAL(settingsButtonPressed()), mp_settingsWidget, SLOT(show()));
    QObject::connect(mp_controlBarWidget, SIGNAL(homeButtonPressed()), mp_settingsWidget, SLOT(hide()));

    // apply color scheme
    updateColorScheme();
    mp_statusBarWidget->updateColorScheme();
    mp_controlBarWidget->updateColorScheme();
    mp_settingsWidget->updateColorScheme();
    mp_popupWidget->updateColorScheme();

    setWindowIcon(QIcon(":/icons/home_day.png"));
}

MainWindow::~MainWindow()
{
    delete mp_dayNightModeProxy;
    delete mp_popupWidget;
    delete mp_settingsWidget;
    delete mp_controlBarWidget;
    delete mp_statusBarWidget;
    delete mp_ui;
}

void MainWindow::dayNightModeSlot(int mode)
{
    QSettings settings;
    settings.setValue("systemsettings/daynightmode", mode);
    // make sure that everything is written to the settings file before continuing
    settings.sync();

    updateColorScheme();

    mp_statusBarWidget->updateColorScheme();
    mp_controlBarWidget->updateColorScheme();
    mp_settingsWidget->updateColorScheme();
    mp_popupWidget->updateColorScheme();
}

void MainWindow::updateColorScheme()
{
    QSettings settings;
    QSettings settings_cs(QApplication::applicationDirPath() +
                          "/colorschemes/" +
                          settings.value("systemsettings/colorscheme", "default").toString() +
                          "/" +
                          QString::number(settings.value("systemsettings/daynightmode", SystemDayNight::DAYNIGHTMODE_DAY).toInt()) +
                          ".ini",
                          QSettings::IniFormat);

    mp_ui->widget_Background->setStyleSheet(settings_cs.value("MainWindow/widget_Background").toString());
    mp_ui->widget_Home_Icon->setStyleSheet(settings_cs.value("MainWindow/widget_Home_Icon").toString());
}

void MainWindow::changeEvent(QEvent* event)
{
    if (QEvent::LanguageChange == event->type())
    {
        mp_ui->retranslateUi(this);
    }

    QMainWindow::changeEvent(event);
}
