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

#include "homescreenappframeworkbinderagl.h"


HomeScreenAppFrameworkBinderAgl::HomeScreenAppFrameworkBinderAgl(QObject *parent) :
    QObject(parent),
    mp_appframeworkAdaptor(0),
    m_apps(),
    mp_dBusAfmUserProxy(0)
{
    qDebug("D-Bus: register as org.agl.homescreenappframeworkbinder");

    //m_apps.clear();

    // publish app framework interface
    mp_appframeworkAdaptor = new AppframeworkAdaptor((QObject*)this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/AppFramework", this);
    dbus.registerService("org.agl.homescreenappframeworkbinder");

    qDebug("D-Bus: connect to org.AGL.afm.user /org/AGL/afm/user");
    mp_dBusAfmUserProxy = new org::AGL::afm::user("org.AGL.afm.user",
                                              "/org/AGL/afm/user",
                                              QDBusConnection::sessionBus(),
                                              0);

#ifdef HAVE_IVI_LAYERMANAGEMENT_API
    QString runnables = mp_dBusAfmUserProxy->runnables("{\"dummy\": \"entry\"}");
    qDebug("runnables: %s", runnables.toStdString().c_str());

    QJsonDocument jsonResponse = QJsonDocument::fromJson(runnables.toUtf8());
    QJsonArray appsArray = jsonResponse.array();
    for (int i = 0; i < appsArray.size(); ++i)
    {
        qDebug("new app %d", i);
        QJsonObject appObject = appsArray[i].toObject();
        AppInfo appInfo;
        appInfo.read(appObject);
        qDebug("name %s", appInfo.name().toStdString().c_str());
        m_apps.append(appInfo);

    }
#else
    // for the simulation
    /*AppInfo ai;
    ai.setName("org.test.app1");
    ai.setDescription("testdesc1");
    ai.setIconPath("https://www.automotivelinux.org/sites/cpstandard/files/logo.png");
    m_apps.append(ai);
    ai.setName("org.test.app2");
    ai.setDescription("testdesc2");
    m_apps.append(ai);*/
#endif
}

HomeScreenAppFrameworkBinderAgl::~HomeScreenAppFrameworkBinderAgl()
{
    delete mp_appframeworkAdaptor;
}

QList<AppInfo> HomeScreenAppFrameworkBinderAgl::getAvailableApps()
{
    return m_apps;
}

int HomeScreenAppFrameworkBinderAgl::launchApp(const QString &name)
{
    int pid = -1;
#ifdef HAVE_IVI_LAYERMANAGEMENT_API
    qDebug("launchApp name: %s", name.toStdString().c_str());
    QString jsonLaunch = "{\"id\":\"" + name + "\", \"mode\":\"local\"}";
    qDebug("jsonLaunch %s", jsonLaunch.toStdString().c_str());
    QString stateString = mp_dBusAfmUserProxy->once(jsonLaunch);
    qDebug("stateString %s", stateString.toStdString().c_str());

    QJsonDocument jsonResponse = QJsonDocument::fromJson(stateString.toUtf8());
    QJsonObject obj = jsonResponse.object();
    QJsonArray pidArray = obj["pids"].toArray();
    for (int i = 0; i < pidArray.size(); ++i)
    {
        qDebug("pid %d", pidArray[i].toInt());
    }
    if (1 == pidArray.size())
    {
        pid = pidArray[0].toInt();
    }
    if (2 == pidArray.size())
    {
        pid = pidArray[1].toInt();
    }

    qDebug("launchApp pid: %d", pid);
#else
    qDebug("launchApp name: %s", name.toStdString().c_str());
    pid = qrand();
#endif

    return pid;
}

void HomeScreenAppFrameworkBinderAgl::clearAppList()
{
    //m_apps.clear();
}

void HomeScreenAppFrameworkBinderAgl::appendAppName(const char* name)
{
    /*AppInfo newApp;
    newApp.setName(QString(name));
    m_apps.append(newApp);*/
}
