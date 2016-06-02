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

#include "systemsettingssimulator.h"
#include "ui_systemsettingssimulator.h"
#include "include/daynightmode.h"
#include <QSettings>

SystemSettingsSimulator::SystemSettingsSimulator(QWidget *parent) :
    QDialog(parent),
    mp_ui(new Ui::SystemSettingsSimulator),
    mp_dBusDayNightMode(0)
{
    mp_ui->setupUi(this);

    // connect to the dBus interface provided by the main window
    mp_dBusDayNightMode = new org::agl::daynightmode("org.agl.mainwindow",
                                              "/MainWindow",
                                              QDBusConnection::sessionBus(),
                                              0);

    QSettings settings;
    this->restoreGeometry(settings.value("systemsettingssimulator/geometry").toByteArray());
    mp_ui->radioButton_DayMode->setChecked(settings.value("systemsettingssimulator/daymode", true).toBool()); // if nothing is stored, use "true"
    mp_ui->radioButton_NightMode->setChecked(settings.value("systemsettingssimulator/nightmode", false).toBool()); // if nothing is stored, use "false"
}

SystemSettingsSimulator::~SystemSettingsSimulator()
{
    QSettings settings;
    settings.setValue("systemsettingssimulator/geometry", saveGeometry());
    settings.setValue("systemsettingssimulator/daymode", mp_ui->radioButton_DayMode->isChecked());
    settings.setValue("systemsettingssimulator/nightmode", mp_ui->radioButton_NightMode->isChecked());

    if (0 == mp_dBusDayNightMode)
    {
        delete mp_dBusDayNightMode;
    }
    delete mp_ui;
}

void SystemSettingsSimulator::on_pushButton_Exit_clicked()
{
    QApplication::exit();
}

void SystemSettingsSimulator::on_radioButton_DayMode_toggled(bool checked)
{
    if (checked)
    {
        mp_dBusDayNightMode->setDayNightMode(SystemDayNight::DAYNIGHTMODE_DAY);
    }
}

void SystemSettingsSimulator::on_radioButton_NightMode_toggled(bool checked)
{
    if (checked)
    {
        mp_dBusDayNightMode->setDayNightMode(SystemDayNight::DAYNIGHTMODE_NIGHT);
    }
}
