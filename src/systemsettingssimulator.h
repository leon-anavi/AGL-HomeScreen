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

#ifndef SYSTEMSETTINGSSIMULATOR_H
#define SYSTEMSETTINGSSIMULATOR_H

#include <QDialog>
#include "include/daynightmode.h"
#include "daynightmode_proxy.h"

namespace Ui {
class SystemSettingsSimulator;
}

class SystemSettingsSimulator : public QDialog
{
    Q_OBJECT

public:
    explicit SystemSettingsSimulator(QWidget *parent = 0);
    ~SystemSettingsSimulator();

private slots:
    void on_pushButton_Exit_clicked();
    void on_radioButton_DayMode_toggled(bool checked);

    void on_radioButton_NightMode_toggled(bool checked);

private:
    Ui::SystemSettingsSimulator *mp_ui;
    org::agl::daynightmode *mp_dBusDayNightMode;
};

#endif // SYSTEMSETTINGSSIMULATOR_H
