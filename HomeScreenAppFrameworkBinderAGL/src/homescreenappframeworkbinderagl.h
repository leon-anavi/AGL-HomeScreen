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

#ifndef HOMESCREENAPPFRAMEWORKBINDERAGL_H
#define HOMESCREENAPPFRAMEWORKBINDERAGL_H

#include <QObject>
#include <include/appframework.hpp>
#include "appframework_adaptor.h"
#include "afm_user_daemon_proxy.h"


class HomeScreenAppFrameworkBinderAgl : public QObject
{
    Q_OBJECT
public:
    explicit HomeScreenAppFrameworkBinderAgl(QObject *parent = 0);
    ~HomeScreenAppFrameworkBinderAgl();
private:
    AppframeworkAdaptor *mp_appframeworkAdaptor;
    QList<AppInfo> m_apps;

    //from appframework_adaptor.h
public Q_SLOTS: // METHODS
    QList<AppInfo> getAvailableApps();
    int launchApp(const QString &name);

public:
    void clearAppList();
    void appendAppName(const char* name);

private:
    org::AGL::afm::user *mp_dBusAfmUserProxy;

};

#endif // HOMESCREENAPPFRAMEWORKBINDERAGL_H
