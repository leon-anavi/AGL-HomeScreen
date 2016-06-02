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

#ifndef INPUTDEVICESIMULATOR_H
#define INPUTDEVICESIMULATOR_H

#include <QDialog>
#include "include/inputevent.h"
#include "inputevent_proxy.h"

namespace Ui {
class InputDeviceSimulator;
}

class InputDeviceSimulator : public QDialog
{
    Q_OBJECT

public:
    explicit InputDeviceSimulator(QWidget *parent = 0);
    ~InputDeviceSimulator();

private slots:
    void on_pushButtonRight_clicked();

private:
    Ui::InputDeviceSimulator *mp_ui;
    org::agl::inputevent *mp_dBusInputevent;
};

#endif // INPUTDEVICESIMULATOR_H