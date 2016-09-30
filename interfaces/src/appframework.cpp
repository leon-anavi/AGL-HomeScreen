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

#include "include/appframework.hpp"

AppInfo::AppInfo(QObject *parent) :
    QObject(parent)
{
}

AppInfo::AppInfo(const AppInfo &other) :
    QObject(other.parent()),
    name(other.getName()),
    iconPath(other.getIconPath()),
    description(other.getDescription())
{
}

AppInfo& AppInfo::operator=(const AppInfo &other)
{
    setParent(other.parent());
    name = other.getName();
    iconPath = other.getIconPath();
    description = other.getDescription();

    return *this;
}

AppInfo::~AppInfo()
{
}

void AppInfo::registerMetaType()
{
    qRegisterMetaType<AppInfo>("AppInfo");
    qDBusRegisterMetaType<AppInfo>();
}


// Marshall the MyStructure data into a D-Bus argument
QDBusArgument &operator<<(QDBusArgument &argument, const AppInfo &appInfo)
{
    argument.beginStructure();
    argument << appInfo.name << appInfo.iconPath << appInfo.description;
    argument.endStructure();
    qDebug("appInfo.name:<< %s", appInfo.name.toStdString().c_str());
    return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, AppInfo &appInfo)
{
    argument.beginStructure();
    argument >> appInfo.name >> appInfo.iconPath >> appInfo.description;
    argument.endStructure();
    qDebug("appInfo.name:>> %s", appInfo.name.toStdString().c_str());
    return argument;
}
