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

#include "controlbarwidget.h"
#include "ui_controlbarwidget.h"

ControlBarWidget::ControlBarWidget(QWidget *parent) :
    QWidget(parent),
    mp_ui(new Ui::ControlBarWidget),
    mp_settingsWidget(0),
    mp_dBusDayNightMode_SettingsWidget(0),
    m_dayNightMode(SystemDayNight::DAYNIGHTMODE_DAY), // TODO: read from system
    mp_daynightmodeAdaptor(0)
{
    // publish dbus day night mode interface
    mp_daynightmodeAdaptor = new DaynightmodeAdaptor((QObject*)this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/ControlBarWidget", this);
    dbus.registerService("org.agl.mainwindow");

    mp_ui->setupUi(this);
}

ControlBarWidget::~ControlBarWidget()
{
    delete mp_daynightmodeAdaptor;
    if (0 != mp_settingsWidget)
    {
        delete mp_settingsWidget;
    }
    if (0 == mp_dBusDayNightMode_SettingsWidget)
    {
        delete mp_dBusDayNightMode_SettingsWidget;
    }
    delete mp_ui;
}

void ControlBarWidget::setDayNightMode(int mode)
{
    QIcon icon;
    switch (mode)
    {
    case SystemDayNight::DAYNIGHTMODE_DAY:
        m_dayNightMode = SystemDayNight::DAYNIGHTMODE_DAY;
        mp_ui->widget_Background->setStyleSheet(QString("background-image: url(:/images/backgrounds/bg_green_day.png)"));

        icon.addFile(QStringLiteral(":/icons/home_day.png"), QSize(), QIcon::Normal, QIcon::Off);
        mp_ui->pushButton_Home->setIcon(icon);
        icon.addFile(QStringLiteral(":/icons/settings_day.png"), QSize(), QIcon::Normal, QIcon::Off);
        mp_ui->pushButton_Settings->setIcon(icon);
        break;
    case SystemDayNight::DAYNIGHTMODE_NIGHT:
        m_dayNightMode = SystemDayNight::DAYNIGHTMODE_NIGHT;
        mp_ui->widget_Background->setStyleSheet(QString("background-image: url(:/images/backgrounds/bg_green_night.png)"));

        icon.addFile(QStringLiteral(":/icons/home_night.png"), QSize(), QIcon::Normal, QIcon::Off);
        mp_ui->pushButton_Home->setIcon(icon);
        icon.addFile(QStringLiteral(":/icons/settings_night.png"), QSize(), QIcon::Normal, QIcon::Off);
        mp_ui->pushButton_Settings->setIcon(icon);
        break;
    default:
        m_dayNightMode = SystemDayNight::DAYNIGHTMODE_UNDEFINED;
    }

    // settings widget if present
    if (0 != mp_dBusDayNightMode_SettingsWidget)
    {
        mp_dBusDayNightMode_SettingsWidget->setDayNightMode(m_dayNightMode);
    }
}

void ControlBarWidget::on_pushButton_Settings_clicked()
{
    if (0 == mp_settingsWidget)
    {
        mp_settingsWidget = new SettingsWidget((QWidget*)parent());
    }

    mp_settingsWidget->move(0, 60);
    mp_settingsWidget->show();

    if (0 == mp_dBusDayNightMode_SettingsWidget)
    {
        // connect to the dBus interface provided by the settings widget
        mp_dBusDayNightMode_SettingsWidget = new org::agl::daynightmode("org.agl.mainwindow",
                                                  "/SettingsWidget",
                                                  QDBusConnection::sessionBus(),
                                                  0);
        mp_dBusDayNightMode_SettingsWidget->setDayNightMode(m_dayNightMode);
    }
}

void ControlBarWidget::on_pushButton_Home_clicked()
{
    if (0 != mp_settingsWidget)
    {
        mp_settingsWidget->hide();
    }
}
