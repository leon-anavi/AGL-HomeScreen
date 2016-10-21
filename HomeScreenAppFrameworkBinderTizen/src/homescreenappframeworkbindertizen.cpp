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

#include "homescreenappframeworkbindertizen.h"


HomeScreenAppFrameworkBinderTizen::HomeScreenAppFrameworkBinderTizen(QObject *parent) :
    QObject(parent),
    mp_appframeworkAdaptor(0)
{
    qDebug("D-Bus: register as org.agl.homescreenappframeworkbinder");

    m_apps.clear();

    // publish app framework interface
    mp_appframeworkAdaptor = new AppframeworkAdaptor((QObject*)this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/AppFramework", this);
    dbus.registerService("org.agl.homescreenappframeworkbinder");

#ifdef __arm__
    pkgmgrinfo_pkginfo_get_list(HomeScreenAppFrameworkBinderTizen::pkg_list_cb_static, this);
    //list_pkgs(this);
#endif
#ifdef __i386__
    // for the simulation
    AppInfo ai;
    ai.setName("org.test.app1");
    ai.setDescription("testdesc1");
    ai.setIconPath("https://www.automotivelinux.org/sites/cpstandard/files/logo.png");
    m_apps.append(ai);
    ai.setName("org.test.app2");
    ai.setDescription("testdesc2");
    m_apps.append(ai);
#endif
}

HomeScreenAppFrameworkBinderTizen::~HomeScreenAppFrameworkBinderTizen()
{
    delete mp_appframeworkAdaptor;
}

QStringList HomeScreenAppFrameworkBinderTizen::getAvailableAppNames()
{
    QStringList result;
    for (int i = 0; i < m_apps.size(); ++i)
    {
        result.append(m_apps.at(i).getName());
    }
    qDebug("size: %d", result.size());
    return result;
}

int HomeScreenAppFrameworkBinderTizen::launchApp(const QString &name)
{
    int pid = -1;
#ifdef __arm__
    qDebug("launchApp name: %s", name.toStdString().c_str());
    pid = aul_launch_app(name.toStdString().c_str(), 0);
    qDebug("launchApp pid: %d", pid);
#endif
#ifdef __i386__
    qDebug("launchApp name: %s", name.toStdString().c_str());
    pid = qrand();
#endif

    return pid;
}

void HomeScreenAppFrameworkBinderTizen::clearAppList()
{
    m_apps.clear();
}

void HomeScreenAppFrameworkBinderTizen::appendAppName(const char* name)
{
    AppInfo newApp;
    newApp.setName(QString(name));
    m_apps.append(newApp);
}

#ifdef __arm__
void HomeScreenAppFrameworkBinderTizen::pkg_list_cb_non_static(pkgmgrinfo_pkginfo_h handle)
{
    qDebug("pkg_list_cb_non_static");
    char *pkgid2 = NULL;
    pkgmgrinfo_pkginfo_get_pkgid(handle, &pkgid2);
    printf("pkg_list_cb_non_static: %s\n", pkgid2);
    appendAppName(pkgid2);
}

int HomeScreenAppFrameworkBinderTizen::pkg_list_cb_static(pkgmgrinfo_pkginfo_h handle, void *user_data)
{
    qDebug("pkg_list_cb_static");
    static_cast<HomeScreenAppFrameworkBinderTizen*>(user_data)->pkg_list_cb_non_static(handle);
}
#endif
