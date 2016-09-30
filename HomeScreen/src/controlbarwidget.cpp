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
#include <include/daynightmode.hpp>
#include <include/inputevent.hpp>
#include <QSettings>

ControlBarWidget::ControlBarWidget(QWidget *parent) :
    QWidget(parent),
    mp_ui(new Ui::ControlBarWidget),
    mp_dBusInputEventProxy()
{
    mp_ui->setupUi(this);

    qDebug("D-Bus: connect to org.agl.homescreenappframeworkbindertizen /AppFramework");
    mp_dBusInputEventProxy = new org::agl::inputevent("org.agl.inputeventmanager",
                                              "/InputEvent",
                                              QDBusConnection::sessionBus(),
                                              0);
}

ControlBarWidget::~ControlBarWidget()
{
    delete mp_dBusInputEventProxy;
    delete mp_ui;
}

void ControlBarWidget::updateColorScheme()
{
    QSettings settings;
    QSettings settings_cs(QApplication::applicationDirPath() +
                          "/colorschemes/" +
                          settings.value("systemsettings/colorscheme", "default").toString() +
                          "/" +
                          QString::number(settings.value("systemsettings/daynightmode", SystemDayNight::DAYNIGHTMODE_DAY).toInt()) +
                          ".ini",
                          QSettings::IniFormat);

    QIcon icon;
    mp_ui->widget_Background->setStyleSheet(settings_cs.value(QString("ControlBarWidget/widget_Background")).toString());

    icon.addFile(settings_cs.value(QString("ControlBarWidget/pushButton_Home")).toString(), QSize(), QIcon::Normal, QIcon::Off);
    mp_ui->pushButton_Home->setIcon(icon);
    icon.addFile(settings_cs.value(QString("ControlBarWidget/pushButton_Settings")).toString(), QSize(), QIcon::Normal, QIcon::Off);
    mp_ui->pushButton_Settings->setIcon(icon);
    icon.addFile(settings_cs.value(QString("ControlBarWidget/pushButton_Nav")).toString(), QSize(), QIcon::Normal, QIcon::Off);
    mp_ui->pushButton_Nav->setIcon(icon);
}

void ControlBarWidget::on_pushButton_Settings_clicked()
{
    settingsButtonPressed();
}

void ControlBarWidget::on_pushButton_Home_clicked()
{
    homeButtonPressed();
}

void ControlBarWidget::on_pushButton_Nav_clicked()
{
    mp_dBusInputEventProxy->hardKeyPressed(InputEvent::HARDKEY_NAV);
}
