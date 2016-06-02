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

#include "settingswidget.h"
#include "ui_settingswidget.h"
#include "include/daynightmode.h"
#include <QSettings>

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    mp_ui(new Ui::SettingsWidget),
    m_dayNightMode(SystemDayNight::DAYNIGHTMODE_DAY), // TODO: read from system
    mp_daynightmodeAdaptor(0),
    mp_translator(0)
{
    // publish dbus day night mode interface
    mp_daynightmodeAdaptor = new DaynightmodeAdaptor((QObject*)this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/SettingsWidget", this);
    dbus.registerService("org.agl.mainwindow");

    // multi-language support
    mp_translator = new QTranslator();
    mp_translator->load("homescreen_en_US.qm", ":/translations"); // TODO: read from system
    QApplication::instance()->installTranslator(mp_translator);

    mp_ui->setupUi(this);

    mp_ui->comboBoxLanguage->addItem(QString("English"), QVariant("homescreen_en_US.qm")); // TODO: make this configurable?
    mp_ui->comboBoxLanguage->addItem(QString("Deutsch"), QVariant("homescreen_de_DE.qm"));
    mp_ui->comboBoxLanguage->addItem(QString("日本語"), QVariant("homescreen_ja_JP.qm"));

    QSettings settings;
    mp_ui->comboBoxLanguage->setCurrentIndex(settings.value("systemsettings/language", 0).toInt());
}

SettingsWidget::~SettingsWidget()
{
    delete mp_translator;
    delete mp_daynightmodeAdaptor;

    QSettings settings;
    settings.setValue("systemsettings/language", mp_ui->comboBoxLanguage->currentIndex());
    delete mp_ui;
}

void SettingsWidget::setDayNightMode(int mode)
{
    switch (mode)
    {
    case SystemDayNight::DAYNIGHTMODE_DAY:
        m_dayNightMode = SystemDayNight::DAYNIGHTMODE_DAY;
        mp_ui->widget_Background->setStyleSheet(QString("background-image: url(:/images/backgrounds/bg_blue_day.png);"));
        mp_ui->comboBoxLanguage->setStyleSheet(QString("QComboBox { \
            border: 1px solid #D3D3D3; \
            border-radius: 8px; \
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(242, 242, 249, 255), stop:1 rgba(255, 255, 255, 255)); \
            color: #333; \
            padding: 0px; \
            } \
            QComboBox:on { \
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #D5D5D5, stop: 1 #EEEEEE); \
            } \
            QComboBox::drop-down { \
            border: 0px solid; \
            border-radius: 0px; \
            } \
            QComboBox::down-arrow:on { \
            }"));
        // settings icon
        mp_ui->widget_Settings_Icon->setStyleSheet(QString("border-image: url(:/icons/settings_day.png) 0 0 0 0 stretch stretch;"));
        break;
    case SystemDayNight::DAYNIGHTMODE_NIGHT:
        m_dayNightMode = SystemDayNight::DAYNIGHTMODE_NIGHT;
        mp_ui->widget_Background->setStyleSheet(QString("background-image: url(:/images/backgrounds/bg_blue_night.png);"));
        mp_ui->comboBoxLanguage->setStyleSheet(QString("QComboBox { \
            border: 1px solid #D3D3D3; \
            border-radius: 8px; \
            background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(147, 147, 151, 255), stop:1 rgba(255, 255, 255, 255)); \
            color: #333; \
            padding: 0px; \
            } \
            QComboBox:on { \
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #D5D5D5, stop: 1 #EEEEEE); \
            } \
            QComboBox::drop-down { \
            border: 0px solid; \
            border-radius: 0px; \
            } \
            QComboBox::down-arrow:on { \
            }"));
        // settings icon
        mp_ui->widget_Settings_Icon->setStyleSheet(QString("border-image: url(:/icons/settings_night.png) 0 0 0 0 stretch stretch;"));
        break;
    default:
        m_dayNightMode = SystemDayNight::DAYNIGHTMODE_UNDEFINED;
    }
}

void SettingsWidget::changeEvent(QEvent* event)
{
    if (QEvent::LanguageChange == event->type())
    {
        mp_ui->retranslateUi(this);
    }

    QWidget::changeEvent(event);
}

void SettingsWidget::on_comboBoxLanguage_currentIndexChanged(const QString &)
{
    if (0 != mp_translator)
        mp_translator->load(mp_ui->comboBoxLanguage->currentData().toString(), ":/translations");
}
