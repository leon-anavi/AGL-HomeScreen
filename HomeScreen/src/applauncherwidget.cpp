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

#include "applauncherwidget.h"
#include "ui_applauncherwidget.h"
#include <include/daynightmode.hpp>
#include <QSettings>
#ifdef __i386__
    #include <QProcess>
#endif

#define APP_LIST_COLUMN_COUNT 5

AppLauncherWidget::AppLauncherWidget(QWidget *parent) :
    QWidget(parent),
    mp_ui(new Ui::AppLauncherWidget),
    m_appList(),
    mp_appTable(0),
    mp_dBusAppFrameworkProxy()
{
    mp_ui->setupUi(this);

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

AppLauncherWidget::~AppLauncherWidget()
{
    delete mp_dBusAppFrameworkProxy;
    if (0 != mp_appTable)
    {
        delete mp_appTable;
    }
    delete mp_ui;
}

void AppLauncherWidget::updateColorScheme()
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

    mp_ui->widget_background->setStyleSheet(settings_cs.value("AppLauncherWidget/widget_Background").toString());
    mp_ui->widget_homeIcon->setStyleSheet(settings_cs.value("AppLauncherWidget/widget_Home_Icon").toString());

    setStyleSheet(settings_cs.value("AppLauncherWidget/common_css").toString());
}

void AppLauncherWidget::populateAppList()
{
    if (0 == mp_appTable)
    {
        mp_appTable = new QTableWidget(this);
        QObject::connect(mp_appTable, SIGNAL(cellClicked(int,int)), this, SLOT(on_tableView_clicked(int,int)));
    }
    else
    {
        mp_appTable->clear();
    }

    mp_appTable->setShowGrid(false);
    mp_appTable->setFrameShape(QFrame::NoFrame);
    mp_appTable->move(40, 40);
    mp_appTable->resize(1000, 1920 - 40 - 40 - 60 - 60);
    mp_appTable->horizontalHeader()->setVisible(false);
    mp_appTable->verticalHeader()->setVisible(false);
    mp_appTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mp_appTable->setRowCount(100);
    mp_appTable->setColumnCount(APP_LIST_COLUMN_COUNT);

    int i;

    m_appList = mp_dBusAppFrameworkProxy->getAvailableApps();

    mp_appTable->setRowCount((m_appList.size() + (APP_LIST_COLUMN_COUNT - 1)) / APP_LIST_COLUMN_COUNT);

    if (m_appList.size() >= (9 * APP_LIST_COLUMN_COUNT))
    {
        mp_appTable->resize(1000, 1920 - 40 - 40 - 60 - 60);
    }
    else
    {
        mp_appTable->resize(1000, mp_appTable->rowCount() * 190);
    }


    for (i = 0; i < (mp_appTable->rowCount() * APP_LIST_COLUMN_COUNT); i++)
    {
        mp_appTable->verticalHeader()->resizeSection(i, 190);
        mp_appTable->horizontalHeader()->resizeSection(i, 190);
    }

    for (i = 0; i < m_appList.size(); i++)
    {
       mp_appTable->setItem(i / APP_LIST_COLUMN_COUNT,
                            i % APP_LIST_COLUMN_COUNT,
                            new QTableWidgetItem(m_appList.at(i).name()));
       mp_appTable->item(i / APP_LIST_COLUMN_COUNT,
                         i % APP_LIST_COLUMN_COUNT)->setFlags(Qt::ItemIsEnabled);
       mp_appTable->item(i / APP_LIST_COLUMN_COUNT,
                         i % APP_LIST_COLUMN_COUNT)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
}

void AppLauncherWidget::on_tableView_clicked(int row, int col)
{
    if (m_appList.size() > row * APP_LIST_COLUMN_COUNT + col)
    {
        int pid = mp_dBusAppFrameworkProxy->launchApp(m_appList.at(row * APP_LIST_COLUMN_COUNT + col).id());
        qDebug("%d, %d: start app %s", row, col, m_appList.at(row * APP_LIST_COLUMN_COUNT + col).id().toStdString().c_str());
        qDebug("pid: %d", pid);

        // the new app wants to be visible by default
        newRequestsToBeVisibleApp(pid);

        showAppLayer();
    }
}
