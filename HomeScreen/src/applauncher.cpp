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

#include "applauncher.h"
#include <include/daynightmode.hpp>
#include <QSettings>
#include <QApplication>
#ifdef __i386__
    #include <QProcess>
#endif

AppLauncher::AppLauncher(QObject *parent) :
    QObject(parent),
    m_appList(),
    mp_dBusAppFrameworkProxy()
{
    /*AppInfo ai;
    for (int i = 0; i < 100; ++i)
    {
        ai.setName("test" + QString::number(i));
        mp_appList->append(ai);
    }*/

    qDebug("D-Bus: connect to org.agl.homescreenappframeworkbinder /AppFramework");
    mp_dBusAppFrameworkProxy = new org::agl::appframework("org.agl.homescreenappframeworkbinder",
                                              "/AppFramework",
                                              QDBusConnection::sessionBus(),
                                              0);
}

AppLauncher::~AppLauncher()
{
    delete mp_dBusAppFrameworkProxy;
}

void AppLauncher::updateColorScheme()
{
    QSettings settings;
    QSettings settings_cs(QApplication::applicationDirPath() +
                          "/colorschemes/" +
                          settings.value("systemsettings/colorscheme", "default").toString() +
                          "/" +
                          QString::number(settings.value("systemsettings/proximityobjectdetected", false).toBool()) +
                          "/" +
                          QString::number(settings.value("systemsettings/daynightmode", SystemDayNight::DAYNIGHTMODE_DAY).toInt()) +
                          ".ini",
                          QSettings::IniFormat);

    //mp_ui->widget_background->setStyleSheet(settings_cs.value("AppLauncherWidget/widget_Background").toString());
    //mp_ui->widget_homeIcon->setStyleSheet(settings_cs.value("AppLauncherWidget/widget_Home_Icon").toString());

    //setStyleSheet(settings_cs.value("AppLauncherWidget/common_css").toString());
}

void AppLauncher::populateAppList()
{
    int i;

    m_appList = mp_dBusAppFrameworkProxy->getAvailableApps();
}

void AppLauncher::on_tableView_clicked(int row, int col)
{
/*    if (m_appList.size() > row * APP_LIST_COLUMN_COUNT + col)
    {
        int pid = mp_dBusAppFrameworkProxy->launchApp(m_appList.at(row * APP_LIST_COLUMN_COUNT + col).id());
        qDebug("%d, %d: start app %s", row, col, m_appList.at(row * APP_LIST_COLUMN_COUNT + col).id().toStdString().c_str());
        qDebug("pid: %d", pid);

        // the new app wants to be visible by default
        emit newRequestsToBeVisibleApp(pid);

        emit showAppLayer();
    }*/
}
