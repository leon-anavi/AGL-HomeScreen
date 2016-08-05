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

#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <QObject>


#ifdef __arm__
extern "C" {
#include "ilm/ilm_control.h"
}
#endif


class WindowManager : public QObject
{
    Q_OBJECT
public:
    explicit WindowManager(QObject *parent = 0);
    ~WindowManager();
private:


public:

#ifdef __arm__
    void notificationFunc_non_static(ilmObjectType object,
                                        t_ilm_uint id,
                                        t_ilm_bool created);
    static void notificationFunc_static(ilmObjectType object,
                                        t_ilm_uint id,
                                        t_ilm_bool created,
                                        void* user_data);
#endif
public slots:

};

#endif // WINDOWMANAGER_HPP
