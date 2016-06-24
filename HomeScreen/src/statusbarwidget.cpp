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
    mp_dayNightModeProxy(0),
    mp_statusbarAdaptor(0),
    mp_statusbarIconURIs(new QMap<int, QString>)
{
    // this has to be adopted to the system setup
    mp_dayNightModeProxy = new org::agl::daynightmode("org.agl.homescreen.simulator", //"org.agl.systeminfoprovider"
                                                      "/",
                                                      QDBusConnection::sessionBus(),
                                                      0);
    QObject::connect(mp_dayNightModeProxy, SIGNAL(dayNightMode(int)), this, SLOT(dayNightModeSlot(int)));

    // publish statusbar interface
    mp_statusbarAdaptor = new StatusbarAdaptor((QObject*)this);

  QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/StatusBar", this);
    dbus.registerService("org.agl.homescreen");

    mp_ui->setupUi(this);
}

StatusBarWidget::~StatusBarWidget()
{
    delete mp_statusbarAdaptor;
    delete mp_dayNightModeProxy;
    delete mp_ui;
}

void StatusBarWidget::dayNightModeSlot(int mode)
{
    switch (mode)
    {
    case SystemDayNight::DAYNIGHTMODE_DAY:
        m_dayNightMode = SystemDayNight::DAYNIGHTMODE_DAY;
        mp_ui->widget->setStyleSheet(QString("background-image: url(:/images/backgrounds/bg_stripes_day.png)"));
        mp_ui->label_1->setStyleSheet(QString("color: rgb(238, 238, 238); background-image: url(") + mp_statusbarIconURIs->value(1) + QString("); background-repeat: no-repeat;"));
        mp_ui->label_2->setStyleSheet(QString("color: rgb(238, 238, 238); background-image: url(") + mp_statusbarIconURIs->value(2) + QString("); background-repeat: no-repeat;"));
        mp_ui->label_3->setStyleSheet(QString("color: rgb(238, 238, 238); background-image: url(") + mp_statusbarIconURIs->value(3) + QString("); background-repeat: no-repeat;"));
        mp_ui->label_4->setStyleSheet(QString("color: rgb(238, 238, 238); background-image: url(") + mp_statusbarIconURIs->value(4) + QString("); background-repeat: no-repeat;"));
        mp_ui->label_5->setStyleSheet(QString("color: rgb(238, 238, 238); background-image: url(") + mp_statusbarIconURIs->value(5) + QString("); background-repeat: no-repeat;"));
        break;
    case SystemDayNight::DAYNIGHTMODE_NIGHT:
        m_dayNightMode = SystemDayNight::DAYNIGHTMODE_NIGHT;
        mp_ui->widget->setStyleSheet(QString("background-image: url(:/images/backgrounds/bg_stripes_night.png)"));
        mp_ui->label_1->setStyleSheet(QString("color: rgb(177, 177, 177); background-image: url(") + mp_statusbarIconURIs->value(1) + QString("); background-repeat: no-repeat;"));
        mp_ui->label_2->setStyleSheet(QString("color: rgb(177, 177, 177); background-image: url(") + mp_statusbarIconURIs->value(2) + QString("); background-repeat: no-repeat;"));
        mp_ui->label_3->setStyleSheet(QString("color: rgb(177, 177, 177); background-image: url(") + mp_statusbarIconURIs->value(3) + QString("); background-repeat: no-repeat;"));
        mp_ui->label_4->setStyleSheet(QString("color: rgb(177, 177, 177); background-image: url(") + mp_statusbarIconURIs->value(4) + QString("); background-repeat: no-repeat;"));
        mp_ui->label_5->setStyleSheet(QString("color: rgb(177, 177, 177); background-image: url(") + mp_statusbarIconURIs->value(5) + QString("); background-repeat: no-repeat;"));
        break;
    default:
        m_dayNightMode = SystemDayNight::DAYNIGHTMODE_UNDEFINED;
    }
}

QList<int> StatusBarWidget::getAvailablePlaceholders()
{
    QList<int> result;

    // for now, all are available
    result.append(1);
    result.append(2);
    result.append(3);
    result.append(4);
    result.append(5);

    return result;
}

QString StatusBarWidget::getStatusIcon(int placeholderIndex)
{
    return mp_statusbarIconURIs->value(placeholderIndex);
}

QString StatusBarWidget::getStatusText(int placeholderIndex)
{
    QString result = "";

    switch (placeholderIndex)
    {
    case 1:
        result = mp_ui->label_1->text();
        break;
    case 2:
        result = mp_ui->label_2->text();
        break;
    case 3:
        result = mp_ui->label_3->text();
        break;
    case 4:
        result = mp_ui->label_4->text();
        break;
    case 5:
        result = mp_ui->label_5->text();
        break;
    default:
        break;
    }

    return result;
}

void StatusBarWidget::setStatusIcon(int placeholderIndex, const QString &iconURI)
{
    mp_statusbarIconURIs->insert(placeholderIndex, iconURI);

    switch (m_dayNightMode)
    {
    case SystemDayNight::DAYNIGHTMODE_DAY:
        switch (placeholderIndex)
        {
        case 1:
            mp_ui->label_1->setStyleSheet(QString("color: rgb(238, 238, 238); background-image: url(") + mp_statusbarIconURIs->value(placeholderIndex) + QString("); background-repeat: no-repeat;"));
            break;
        case 2:
            mp_ui->label_2->setStyleSheet(QString("color: rgb(238, 238, 238); background-image: url(") + mp_statusbarIconURIs->value(placeholderIndex) + QString("); background-repeat: no-repeat;"));
            break;
        case 3:
            mp_ui->label_3->setStyleSheet(QString("color: rgb(238, 238, 238); background-image: url(") + mp_statusbarIconURIs->value(placeholderIndex) + QString("); background-repeat: no-repeat;"));
            break;
        case 4:
            mp_ui->label_4->setStyleSheet(QString("color: rgb(238, 238, 238); background-image: url(") + mp_statusbarIconURIs->value(placeholderIndex) + QString("); background-repeat: no-repeat;"));
            break;
        case 5:
            mp_ui->label_5->setStyleSheet(QString("color: rgb(238, 238, 238); background-image: url(") + mp_statusbarIconURIs->value(placeholderIndex) + QString("); background-repeat: no-repeat;"));
            break;
        default:
            break;
        }
        break;
    case SystemDayNight::DAYNIGHTMODE_NIGHT:
        switch (placeholderIndex)
        {
        case 1:
            mp_ui->label_1->setStyleSheet(QString("color: rgb(177, 177, 177); background-image: url(") + mp_statusbarIconURIs->value(placeholderIndex) + QString("); background-repeat: no-repeat;"));
            break;
        case 2:
            mp_ui->label_2->setStyleSheet(QString("color: rgb(177, 177, 177); background-image: url(") + mp_statusbarIconURIs->value(placeholderIndex) + QString("); background-repeat: no-repeat;"));
            break;
        case 3:
            mp_ui->label_3->setStyleSheet(QString("color: rgb(177, 177, 177); background-image: url(") + mp_statusbarIconURIs->value(placeholderIndex) + QString("); background-repeat: no-repeat;"));
            break;
        case 4:
            mp_ui->label_4->setStyleSheet(QString("color: rgb(177, 177, 177); background-image: url(") + mp_statusbarIconURIs->value(placeholderIndex) + QString("); background-repeat: no-repeat;"));
            break;
        case 5:
            mp_ui->label_5->setStyleSheet(QString("color: rgb(177, 177, 177); background-image: url(") + mp_statusbarIconURIs->value(placeholderIndex) + QString("); background-repeat: no-repeat;"));
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void StatusBarWidget::setStatusText(int placeholderIndex, const QString &text)
{
    switch (placeholderIndex)
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
        mp_ui->label_4->setText(text);
        break;
    case 5:
        mp_ui->label_5->setText(text);
        break;
    default:
        break;
    }
}


