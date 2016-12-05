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

#ifndef APPLAUNCHER_H
#define APPLAUNCHER_H

#include <include/appframework.hpp>
#include <appframework_proxy.h>

class AppLauncher : public QObject
{
    Q_OBJECT

public:
    explicit AppLauncher(QObject *parent);
    ~AppLauncher();

    void populateAppList();

public slots:
    void updateColorScheme();

private slots:
    void on_tableView_clicked(int row, int col);

signals:
    void newRequestsToBeVisibleApp(int pid);
    void showAppLayer();

private:
    QList<AppInfo> m_appList;
    org::agl::appframework *mp_dBusAppFrameworkProxy;
};

#endif // APPLAUNCHER_H
