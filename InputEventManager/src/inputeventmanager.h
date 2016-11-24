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

#ifndef INPUTEVENTMANAGER_H
#define INPUTEVENTMANAGER_H

#include "inputevent_adaptor.h"
#include <include/homescreen.hpp>
#include "homescreen_proxy.h"

/*
#ifdef __arm__
extern "C" {
#include "aul/aul.h"
#include "pkgmgr-info.h"
}
#endif
*/

class InputEventManager : public QObject
{
    Q_OBJECT
public:
    explicit InputEventManager(QObject *parent = 0);
    ~InputEventManager();
private:
    InputeventAdaptor *mp_inputEventAdaptor;
    org::agl::homescreen *mp_dBusHomeScreenProxy;

    //from inputevent_adapter.h
public Q_SLOTS: // METHODS
    void hardKeyPressed(int key);

public:


public slots:

};

#endif // INPUTEVENTMANAGER_H
