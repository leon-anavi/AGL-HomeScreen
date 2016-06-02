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

#include "statusbarwidget.h"
#include "ui_statusbarwidget.h"

StatusBarWidget::StatusBarWidget(QWidget *parent) :
    QWidget(parent),
    mp_ui(new Ui::StatusBarWidget),
    m_dayNightMode(SystemDayNight::DAYNIGHTMODE_DAY), // TODO: read from system
    mp_daynightmodeAdaptor(0),
    mp_statusbarAdaptor(0),
    m_secondsTimerId(-1)
{
    // publish dbus day night mode interface
    mp_daynightmodeAdaptor = new DaynightmodeAdaptor((QObject*)this);
    // publish statusbar interface
    mp_statusbarAdaptor = new StatusbarAdaptor((QObject*)this);

    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/StatusBarWidget", this);
    dbus.registerService("org.agl.mainwindow");

    mp_ui->setupUi(this);

    // callback every second
    m_secondsTimerId = startTimer(1000);
}

StatusBarWidget::~StatusBarWidget()
{
    delete mp_statusbarAdaptor;
    delete mp_daynightmodeAdaptor;
    delete mp_ui;
}

void StatusBarWidget::setDayNightMode(int mode)
{
    switch (mode)
    {
    case SystemDayNight::DAYNIGHTMODE_DAY:
        m_dayNightMode = SystemDayNight::DAYNIGHTMODE_DAY;
        mp_ui->widget->setStyleSheet(QString("background-image: url(:/images/backgrounds/bg_stripes_day.png)"));
        mp_ui->label_1->setStyleSheet(QString("color: rgb(238, 238, 238); background-image: url(:/images/transparency.png);"));
        mp_ui->label_2->setStyleSheet(QString("color: rgb(238, 238, 238); background-image: url(:/images/transparency.png);"));
        mp_ui->label_3->setStyleSheet(QString("color: rgb(238, 238, 238); background-image: url(:/images/transparency.png);"));
        mp_ui->label_4_Time->setStyleSheet(QString("color: rgb(238, 238, 238); background-image: url(:/images/transparency.png);"));
        mp_ui->label_5->setStyleSheet(QString("color: rgb(238, 238, 238); background-image: url(:/images/transparency.png);"));
        mp_ui->label_6->setStyleSheet(QString("color: rgb(238, 238, 238); background-image: url(:/images/transparency.png);"));
        mp_ui->label_7->setStyleSheet(QString("color: rgb(238, 238, 238); background-image: url(:/images/transparency.png);"));
        break;
    case SystemDayNight::DAYNIGHTMODE_NIGHT:
        m_dayNightMode = SystemDayNight::DAYNIGHTMODE_NIGHT;
        mp_ui->widget->setStyleSheet(QString("background-image: url(:/images/backgrounds/bg_stripes_night.png)"));
        mp_ui->label_1->setStyleSheet(QString("color: rgb(177, 177, 177); background-image: url(:/images/transparency.png);"));
        mp_ui->label_2->setStyleSheet(QString("color: rgb(177, 177, 177); background-image: url(:/images/transparency.png);"));
        mp_ui->label_3->setStyleSheet(QString("color: rgb(177, 177, 177); background-image: url(:/images/transparency.png);"));
        mp_ui->label_4_Time->setStyleSheet(QString("color: rgb(177, 177, 177); background-image: url(:/images/transparency.png);"));
        mp_ui->label_5->setStyleSheet(QString("color: rgb(177, 177, 177); background-image: url(:/images/transparency.png);"));
        mp_ui->label_6->setStyleSheet(QString("color: rgb(177, 177, 177); background-image: url(:/images/transparency.png);"));
        mp_ui->label_7->setStyleSheet(QString("color: rgb(177, 177, 177); background-image: url(:/images/transparency.png);"));
        break;
    default:
        m_dayNightMode = SystemDayNight::DAYNIGHTMODE_UNDEFINED;
    }
}

void StatusBarWidget::setStatus(int index, const QString &text)
{
    switch (index)
    {
    case 1:
        mp_ui->label_1->setText(text);
        break;
    case 2:
        mp_ui->label_2->setText(text);
        break;
    case 3:
        mp_ui->label_3->setText(text);
        break;
    case 4:
        qDebug("reserved for the time status");
        break;
    case 5:
        mp_ui->label_5->setText(text);
        break;
    case 6:
        mp_ui->label_6->setText(text);
        break;
    case 7:
        mp_ui->label_7->setText(text);
        break;
    default:
        break;
    }
}

void StatusBarWidget::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == m_secondsTimerId)
    {
        mp_ui->label_4_Time->setText(QDateTime::currentDateTime().toString("hh:mm"));
    }
}

