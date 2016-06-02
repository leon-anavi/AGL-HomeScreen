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
    mp_dBusDayNightMode_StatusBarWidget(0),
    mp_controlBarWidget(0),
    mp_dBusDayNightMode_ControlBarWidget(0),
    m_dayNightMode(SystemDayNight::DAYNIGHTMODE_DAY), // TODO: read from system
    mp_daynightmodeAdaptor(0),
    mp_popupAdaptor(0),
    mp_dBusPopup(0),
    mp_popupWidget(0)
{
    // dbus setup
    QDBusConnection dbus = QDBusConnection::sessionBus();

    // publish dbus day night mode interface
    mp_daynightmodeAdaptor = new DaynightmodeAdaptor((QObject*)this);
    // publish dbus popup interface
    mp_popupAdaptor = new PopupAdaptor((QObject*)this);

    dbus.registerObject("/MainWindow", this);
    dbus.registerService("org.agl.mainwindow");

    // no window decoration
    setWindowFlags(Qt::FramelessWindowHint);
    mp_ui->setupUi(this);

    mp_statusBarWidget = new StatusBarWidget(this);
    mp_statusBarWidget->raise();
    // apply layout
    mp_statusBarWidget->setGeometry(0, 0, 800, 60);
    // connect to the dBus interface provided by the status bar widget
    mp_dBusDayNightMode_StatusBarWidget = new org::agl::daynightmode("org.agl.mainwindow",
                                              "/StatusBarWidget",
                                              QDBusConnection::sessionBus(),
                                              0);

    mp_controlBarWidget = new ControlBarWidget(this);
    mp_controlBarWidget->raise();
    // apply layout
    mp_controlBarWidget->setGeometry(0, 540, 800, 60);
    // connect to the dBus interface provided by the control bar widget
    mp_dBusDayNightMode_ControlBarWidget = new org::agl::daynightmode("org.agl.mainwindow",
                                              "/ControlBarWidget",
                                              QDBusConnection::sessionBus(),
                                              0);
}

MainWindow::~MainWindow()
{
    delete mp_dBusDayNightMode_ControlBarWidget;
    delete mp_dBusDayNightMode_StatusBarWidget;

    if (0 == mp_dBusPopup)
    {
        delete mp_dBusPopup;
    }
    if (0 != mp_popupWidget)
    {
        delete mp_popupWidget;
    }



    delete mp_popupAdaptor;
    delete mp_daynightmodeAdaptor;
    delete mp_statusBarWidget;
    delete mp_ui;
}

void MainWindow::setDayNightMode(int mode)
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

    mp_dBusDayNightMode_StatusBarWidget->setDayNightMode(m_dayNightMode);
    mp_dBusDayNightMode_ControlBarWidget->setDayNightMode(m_dayNightMode);
}

void MainWindow::showPopup(int type, const QString &text)
{
    if (0 == mp_popupWidget)
    {
        qDebug("0 == mp_popupWidget");
        mp_popupWidget = new PopupWidget(this);
    }

    mp_popupWidget->move(0, 0);
    mp_popupWidget->show();

    if (0 == mp_dBusPopup)
    {
        qDebug("0 == mp_dBusPopup");
        // connect to the dBus interface provided by the popup widget
        mp_dBusPopup = new org::agl::popup("org.agl.mainwindow",
                                                  "/PopupWidget",
                                                  QDBusConnection::sessionBus(),
                                                  0);
    }
    mp_dBusPopup->showPopup(type, text);
}

void MainWindow::changeEvent(QEvent* event)
{
    if (QEvent::LanguageChange == event->type())
    {
        mp_ui->retranslateUi(this);
    }

    QMainWindow::changeEvent(event);
}

