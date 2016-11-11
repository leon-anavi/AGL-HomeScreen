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

#include "inputeventmanager.h"
#include "include/inputevent.hpp"


InputEventManager::InputEventManager(QObject *parent) :
    QObject(parent),
    mp_inputEventAdaptor(0),
    mp_dBusHomeScreenProxy(0)
{
    qDebug("D-Bus: register as org.agl.inputeventmanager");

    // publish app framework interface
    mp_inputEventAdaptor = new InputeventAdaptor((QObject*)this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/InputEvent", this);
    dbus.registerService("org.agl.inputeventmanager");


    qDebug("D-Bus: connect to org.agl.homescreen /HomeScreen");
    mp_dBusHomeScreenProxy = new org::agl::homescreen("org.agl.homescreen",
                                              "/HomeScreen",
                                              QDBusConnection::sessionBus(),
                                              0);
}

InputEventManager::~InputEventManager()
{
    delete mp_dBusHomeScreenProxy;
    delete mp_inputEventAdaptor;
}

void InputEventManager::hardKeyPressed(int key)
{
    switch (key)
    {
    case InputEvent::HARDKEY_NAV:
        mp_dBusHomeScreenProxy->hardKeyPressed(key);
        qDebug("hardKeyPressed NAV key pressed!");
        break;
    case InputEvent::HARDKEY_MEDIA:
        mp_dBusHomeScreenProxy->hardKeyPressed(key);
        qDebug("hardKeyPressed MEDIA key pressed!");
        break;
    default:
        qDebug("hardKeyPressed %d", key);
        break;
    }
}
