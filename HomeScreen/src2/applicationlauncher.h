/*
 * Copyright (C) 2016 The Qt Company Ltd.
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

#ifndef APPLICATIONLAUNCHER_H
#define APPLICATIONLAUNCHER_H

#include <QtCore/QObject>
#include <include/appframework.hpp>
#include <appframework_proxy.h>

class ApplicationLauncher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString current READ current WRITE setCurrent NOTIFY currentChanged)
public:
    explicit ApplicationLauncher(QObject *parent = NULL);
    ~ApplicationLauncher();

    QString current() const;

signals:
    void newAppRequestsToBeVisible(int pid);
    void currentChanged(const QString &current);

public slots:
    int launch(const QString &application);
    void setCurrent(const QString &current);

private:
    org::agl::appframework *mp_dBusAppFrameworkProxy;
    QString m_current;
};

#endif // APPLICATIONLAUNCHER_H
