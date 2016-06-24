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
    m_dayNightMode(SystemDayNight::DAYNIGHTMODE_DAY), // TODO: read from system
    mp_dayNightModeProxy(0)
{
    // this has to be adopted to the system setup
    mp_dayNightModeProxy = new org::agl::daynightmode("org.agl.homescreen.simulator", //"org.agl.systeminfoprovider"
                                                      "/",
                                                      QDBusConnection::sessionBus(),
                                                      0);
    QObject::connect(mp_dayNightModeProxy, SIGNAL(dayNightMode(int)), this, SLOT(dayNightModeSlot(int)));

    mp_ui->setupUi(this);
}

ControlBarWidget::~ControlBarWidget()
{
    delete mp_dayNightModeProxy;
    if (0 != mp_settingsWidget)
    {
        delete mp_settingsWidget;
    }
    delete mp_ui;
}

void ControlBarWidget::dayNightModeSlot(int mode)
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
}

void ControlBarWidget::on_pushButton_Settings_clicked()
{
    if (0 == mp_settingsWidget)
    {
        mp_settingsWidget = new SettingsWidget((QWidget*)parent());
    }

    mp_settingsWidget->move(0, 60);
    mp_settingsWidget->show();
}

void ControlBarWidget::on_pushButton_Home_clicked()
{
    if (0 != mp_settingsWidget)
    {
        mp_settingsWidget->hide();
    }
}
