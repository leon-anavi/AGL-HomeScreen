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


AppInfo::AppInfo()
{
}

AppInfo::~AppInfo()
{
}

void AppInfo::read(const QJsonObject &json)
{
    id = json["id"].toString();
    version = json["version"].toString();
    width = json["width"].toInt();
    height = json["height"].toInt();
    name = json["name"].toString();
    description = json["description"].toString();
    shortname = json["shortname"].toString();
    author = json["author"].toString();
    iconPath = json["iconPath"].toString();
}

QDBusArgument &operator <<(QDBusArgument &argument, const AppInfo &mAppInfo)
{
    argument.beginStructure();
    argument << mAppInfo.id;
    argument << mAppInfo.version;
    argument << mAppInfo.width;
    argument << mAppInfo.height;
    argument << mAppInfo.name;
    argument << mAppInfo.description;
    argument << mAppInfo.shortname;
    argument << mAppInfo.author;
    argument << mAppInfo.iconPath;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator >>(const QDBusArgument &argument, AppInfo &mAppInfo)
{
    argument.beginStructure();
    argument >> mAppInfo.id;
    argument >> mAppInfo.version;
    argument >> mAppInfo.width;
    argument >> mAppInfo.height;
    argument >> mAppInfo.name;
    argument >> mAppInfo.description;
    argument >> mAppInfo.shortname;
    argument >> mAppInfo.author;
    argument >> mAppInfo.iconPath;
    argument.endStructure();
    return argument;
}
