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
#include "../interfaces/daynightmode.h"

StatusBarWidget::StatusBarWidget(QWidget *parent) :
    QWidget(parent),
    mp_ui(new Ui::StatusBarWidget),
    mp_statusbarAdaptor(0),
    mp_statusbarIconURIs(new QMap<int, QString>)
{
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
    delete mp_ui;
}

void StatusBarWidget::updateColorScheme()
{
    QSettings settings;
    QSettings settings_cs(QApplication::applicationDirPath() +
                          "/colorschemes/" +
                          settings.value("systemsettings/colorscheme", "default").toString() +
                          "/" +
                          QString::number(settings.value("systemsettings/daynightmode", SystemDayNight::DAYNIGHTMODE_DAY).toInt()) +
                          ".ini",
                          QSettings::IniFormat);

    mp_ui->widget->setStyleSheet(settings_cs.value(QString("StatusBarWidget/widget")).toString());
    mp_ui->label_1->setStyleSheet(settings_cs.value(QString("StatusBarWidget/label_1")).toString());
    mp_ui->label_2->setStyleSheet(settings_cs.value(QString("StatusBarWidget/label_2")).toString());
    mp_ui->label_3->setStyleSheet(settings_cs.value(QString("StatusBarWidget/label_3")).toString());
    mp_ui->label_4->setStyleSheet(settings_cs.value(QString("StatusBarWidget/label_4")).toString());
    mp_ui->label_5->setStyleSheet(settings_cs.value(QString("StatusBarWidget/label_5")).toString());

    mp_ui->label_1_icon->setStyleSheet(settings_cs.value(QString("StatusBarWidget/label_1_icon")).toString()
                                       + QString(" border-image: url(") + mp_statusbarIconURIs->value(1) + QString(") 0 0 0 0 stretch stretch;"));
    mp_ui->label_2_icon->setStyleSheet(settings_cs.value(QString("StatusBarWidget/label_2_icon")).toString()
                                       + QString(" border-image: url(") + mp_statusbarIconURIs->value(2) + QString(") 0 0 0 0 stretch stretch;"));
    mp_ui->label_3_icon->setStyleSheet(settings_cs.value(QString("StatusBarWidget/label_3_icon")).toString()
                                       + QString(" border-image: url(") + mp_statusbarIconURIs->value(3) + QString(") 0 0 0 0 stretch stretch;"));
    mp_ui->label_4_icon->setStyleSheet(settings_cs.value(QString("StatusBarWidget/label_4_icon")).toString()
                                       + QString(" border-image: url(") + mp_statusbarIconURIs->value(4) + QString(") 0 0 0 0 stretch stretch;"));
    mp_ui->label_5_icon->setStyleSheet(settings_cs.value(QString("StatusBarWidget/label_5_icon")).toString()
                                       + QString(" border-image: url(") + mp_statusbarIconURIs->value(5) + QString(") 0 0 0 0 stretch stretch;"));
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

    updateColorScheme();
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
