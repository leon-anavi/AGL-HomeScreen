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

#include "inputeventdistributor.h"
#include <QEvent>

InputEventDistributor::InputEventDistributor(QObject *parent) :
    QObject(parent),
    mp_inputEventAdaptor(0)
{
    // publish dbus popup interface
    mp_inputEventAdaptor = new InputeventAdaptor((QObject*)this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/InputEventDistributor", this);
    dbus.registerService("org.agl.main");
}

InputEventDistributor::~InputEventDistributor()
{
    delete mp_inputEventAdaptor;
}

void InputEventDistributor::keyEvent(int type, int zone, int key)
{
    qDebug("InputEventDistributor::keyEvent type 0x%x, key 0x%x", type, key);

    //QEvent *event;
    switch (type)
    {
    case SystemInputEvent::INPUTEVENTTYPE_KEY_PRESSED:

        break;
    default:
        qDebug("InputEventDistributor::keyEvent: Type 0x%x undefined", type);
    }

}
