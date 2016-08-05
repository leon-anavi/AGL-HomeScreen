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

#include "popupwidget.h"
#include "ui_popupwidget.h"
#include <include/daynightmode.hpp>

PopupWidget::PopupWidget(QWidget *parent) :
    QWidget(parent),
    mp_ui(new Ui::PopupWidget),
    mp_popupAdaptor(0)
{
    // publish dbus popup interface
    mp_popupAdaptor = new PopupAdaptor((QObject*)this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/Popup", this);
    dbus.registerService("org.agl.homescreen");

    // no window decoration
    setWindowFlags(Qt::FramelessWindowHint);

    mp_ui->setupUi(this);

    this->close();
}

PopupWidget::~PopupWidget()
{
    delete mp_popupAdaptor;
    delete mp_ui;
}

void PopupWidget::updateColorScheme()
{
    QSettings settings;
    QSettings settings_cs(QApplication::applicationDirPath() +
                          "/colorschemes/" +
                          settings.value("systemsettings/colorscheme", "default").toString() +
                          "/" +
                          QString::number(settings.value("systemsettings/daynightmode", SystemDayNight::DAYNIGHTMODE_DAY).toInt()) +
                          ".ini",
                          QSettings::IniFormat);

    mp_ui->widget_Popup->setStyleSheet(settings_cs.value(QString("PopupWidget/widget_Popup")).toString());
    mp_ui->label_Text->setStyleSheet(settings_cs.value(QString("PopupWidget/label_Text")).toString());
}

void PopupWidget::showPopup(int /*type*/, const QString &text)
{
    this->show();
    this->raise();
    mp_ui->label_Text->setText(text);
}

void PopupWidget::on_pushButton_OK_clicked()
{
    this->close();
}