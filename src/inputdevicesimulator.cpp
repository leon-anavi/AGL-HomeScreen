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

#include "inputdevicesimulator.h"
#include "ui_inputdevicesimulator.h"
#include <QSettings>


InputDeviceSimulator::InputDeviceSimulator(QWidget *parent) :
    QDialog(parent),
    mp_ui(new Ui::InputDeviceSimulator),
    mp_dBusInputevent(0)
{
    mp_ui->setupUi(this);

    // connect to the dBus interface provided by the input device distributor
    mp_dBusInputevent = new org::agl::inputevent("org.agl.main",
                                              "/InputEventDistributor",
                                              QDBusConnection::sessionBus(),
                                              0);

    QSettings settings;
    this->restoreGeometry(settings.value("inputdevicesimulator/geometry").toByteArray());
}

InputDeviceSimulator::~InputDeviceSimulator()
{
    QSettings settings;
    settings.setValue("inputdevicesimulator/geometry", saveGeometry());

    if (0 == mp_dBusInputevent)
    {
        delete mp_dBusInputevent;
    }

    delete mp_ui;
}

void InputDeviceSimulator::on_pushButtonRight_clicked()
{
    qDebug("right");
    mp_dBusInputevent->keyEvent(SystemInputEvent::INPUTEVENTTYPE_KEY_PRESSED, SystemInputEvent::INPUTEVENTZONE_1, Qt::Key_Right);
}
