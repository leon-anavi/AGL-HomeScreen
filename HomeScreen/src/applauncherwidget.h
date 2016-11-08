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

#ifndef APPLAUNCHERWIDGET_H
#define APPLAUNCHERWIDGET_H

#include <QWidget>
#include <QList>
#include <QTableWidget>
#include <include/appframework.hpp>
#include <appframework_proxy.h>

namespace Ui {
class AppLauncherWidget;
}

class AppLauncherWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AppLauncherWidget(QWidget *parent = 0);
    ~AppLauncherWidget();

    void populateAppList();

public slots:
    void updateColorScheme();

private slots:
    void on_tableView_clicked(int row, int col);

signals:
    void newRequestsToBeVisibleApp(int pid);
    void showAppLayer();

private:
    Ui::AppLauncherWidget *mp_ui;
    QList<AppInfo> m_appList;
    QTableWidget *mp_appTable;
    org::agl::appframework *mp_dBusAppFrameworkProxy;
};

#endif // APPLAUNCHERWIDGET_H
