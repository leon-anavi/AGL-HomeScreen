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

#ifndef HOMESCREENAPPFRAMEWORKBINDERTIZEN_H
#define HOMESCREENAPPFRAMEWORKBINDERTIZEN_H

#include <QObject>
#include <include/appframework.hpp>
#include "appframework_adaptor.h"

#ifdef __arm__
extern "C" {
#include "aul/aul.h"
#include "pkgmgr-info.h"
}
#endif

class HomeScreenAppFrameworkBinderTizen : public QObject
{
    Q_OBJECT
public:
    explicit HomeScreenAppFrameworkBinderTizen(QObject *parent = 0);
    ~HomeScreenAppFrameworkBinderTizen();
private:
    AppframeworkAdaptor *mp_appframeworkAdaptor;
    QList<AppInfo> m_apps;

    //from appframework_adaptor.h
public Q_SLOTS: // METHODS
    QStringList getAvailableAppNames();
    int launchApp(const QString &name);

public:
    void clearAppList();
    void appendAppName(const char* name);

#ifdef __arm__
    void pkg_list_cb_non_static(pkgmgrinfo_pkginfo_h handle);
    static int pkg_list_cb_static(pkgmgrinfo_pkginfo_h handle, void *user_data);
#endif

public slots:

};

#endif // HOMESCREENAPPFRAMEWORKBINDERTIZEN_H
