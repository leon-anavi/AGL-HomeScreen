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
    mp_appList(new QList<AppInfo>()),
    mp_appTable(0),
    mp_dBusAppFrameworkProxy()
{
    mp_ui->setupUi(this);

    AppInfo ai;
    for (int i = 0; i < 100; ++i)
    {
        ai.setName("test" + QString::number(i));
        mp_appList->append(ai);
    }

    qDebug("D-Bus: connect to org.agl.homescreenappframeworkbindertizen /AppFramework");
    mp_dBusAppFrameworkProxy = new org::agl::appframework("org.agl.homescreenappframeworkbindertizen",
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
    delete mp_appList;
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

#ifdef __arm__
    QStringList apps = mp_dBusAppFrameworkProxy->getAvailableAppNames();
#endif
#ifdef __i386__
    QStringList apps;
    apps.append(QString("/usr/bin/gnome-terminal"));
#endif
    mp_appList->clear();

    mp_appTable->setRowCount((apps.size() + (APP_LIST_COLUMN_COUNT - 1)) / APP_LIST_COLUMN_COUNT);

    if (apps.size() >= (9 * APP_LIST_COLUMN_COUNT))
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

    AppInfo ai;
    for (i = 0; i < apps.size(); ++i)
    {
        qDebug("new app: %s", apps.at(i).toStdString().c_str());
        ai.setName(apps.at(i));
        mp_appList->append(ai);
    }

    for (i = 0; i < mp_appList->size(); i++)
    {
       mp_appTable->setItem(i / APP_LIST_COLUMN_COUNT,
                            i % APP_LIST_COLUMN_COUNT,
                            new QTableWidgetItem(mp_appList->at(i).getName()));
       mp_appTable->item(i / APP_LIST_COLUMN_COUNT,
                         i % APP_LIST_COLUMN_COUNT)->setFlags(Qt::ItemIsEnabled);
       mp_appTable->item(i / APP_LIST_COLUMN_COUNT,
                         i % APP_LIST_COLUMN_COUNT)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
}

void AppLauncherWidget::on_tableView_clicked(int row, int col)
{
    if (mp_appList->size() > row * APP_LIST_COLUMN_COUNT + col)
    {
#ifdef __arm__
        int pid = mp_dBusAppFrameworkProxy->launchApp(mp_appList->at(row * APP_LIST_COLUMN_COUNT + col).getName());
#endif
#ifdef __i386__
        QProcess *myProcess = new QProcess();
        myProcess->start(mp_appList->at(row * APP_LIST_COLUMN_COUNT + col).getName(), NULL);
        int pid = myProcess->pid();
#endif
        qDebug("%d, %d: start app %s", row, col, mp_appList->at(row * APP_LIST_COLUMN_COUNT + col).getName().toStdString().c_str());
        qDebug("pid: %d", pid);

        // the new app wants to be visible by default
        newRequestsToBeVisibleApp(pid);
    }
}
