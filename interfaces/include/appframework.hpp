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

#ifndef APPFRAMEWORK_HPP
#define APPFRAMEWORK_HPP

#include <QObject>
#include <QtDBus>
#include <QString>

class AppInfo : public QObject
{
    Q_OBJECT
public:
    explicit AppInfo(QObject *parent = 0);
    AppInfo(const AppInfo &other);
    AppInfo& operator=(const AppInfo &other);
    ~AppInfo();

    //register Message with the Qt type system
    static void registerMetaType();

    friend QDBusArgument &operator<<(QDBusArgument &argument, const AppInfo &appInfo);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, AppInfo &appInfo);

    void setName(const QString name) {this->name = name;}
    QString getName() const {return name;}
    void setIconPath(const QString iconPath) {this->iconPath = iconPath;}
    QString getIconPath() const {return iconPath;}
    void setDescription(const QString description) {this->description = description;}
    QString getDescription() const {return description;}

private:
    QString name;
    QString iconPath;
    QString description;
};

Q_DECLARE_METATYPE(AppInfo)

#endif // APPFRAMEWORK_HPP
