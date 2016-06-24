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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mp_ui(new Ui::MainWindow),
    mp_statusBarWidget(0),
    mp_controlBarWidget(0),
    m_dayNightMode(SystemDayNight::DAYNIGHTMODE_DAY), // TODO: read from system
    mp_dayNightModeProxy(0),
    mp_popupWidget(0)
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

    mp_popupWidget = new PopupWidget(this);
    mp_controlBarWidget->raise();
    // apply layout
    mp_popupWidget->move(0, 0);

    setWindowIcon(QIcon(":/icons/home_day.png"));
}

MainWindow::~MainWindow()
{
    delete mp_popupWidget;
    delete mp_dayNightModeProxy;
    delete mp_statusBarWidget;
    delete mp_ui;
}

void MainWindow::dayNightModeSlot(int mode)
{
    switch (mode)
    {
    case SystemDayNight::DAYNIGHTMODE_DAY:
        m_dayNightMode = SystemDayNight::DAYNIGHTMODE_DAY;
        mp_ui->widget_Background->setStyleSheet(QString("background-image: url(:/images/backgrounds/bg_blue_day.png)"));
        // home icon
        mp_ui->widget_Home_Icon->setStyleSheet(QString("border-image: url(:/icons/home_day.png) 0 0 0 0 stretch stretch;"));

        break;
    case SystemDayNight::DAYNIGHTMODE_NIGHT:
        m_dayNightMode = SystemDayNight::DAYNIGHTMODE_NIGHT;
        mp_ui->widget_Background->setStyleSheet(QString("background-image: url(:/images/backgrounds/bg_blue_night.png)"));
        // home icon
        mp_ui->widget_Home_Icon->setStyleSheet(QString("border-image: url(:/icons/home_night.png) 0 0 0 0 stretch stretch;"));

        break;
    default:
        m_dayNightMode = SystemDayNight::DAYNIGHTMODE_UNDEFINED;
    }
}

void MainWindow::changeEvent(QEvent* event)
{
    if (QEvent::LanguageChange == event->type())
    {
        mp_ui->retranslateUi(this);
    }

    QMainWindow::changeEvent(event);
}

