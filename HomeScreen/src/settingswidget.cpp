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
#include <QSettings>
#include <include/daynightmode.hpp>

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    mp_ui(new Ui::SettingsWidget),
    mp_translator(0)
{
    // no window decoration
    setWindowFlags(Qt::FramelessWindowHint);

    // multi-language support
    mp_translator = new QTranslator();
    mp_translator->load("homescreen_en_US.qm", ":/translations"); // TODO: read from system
    QApplication::instance()->installTranslator(mp_translator);

    mp_ui->setupUi(this);

    mp_ui->comboBox_language->addItem(QString("English"), QVariant("homescreen_en_US.qm")); // TODO: make this configurable
    mp_ui->comboBox_language->addItem(QString("Deutsch"), QVariant("homescreen_de_DE.qm"));
    mp_ui->comboBox_language->addItem(QString("日本語"), QVariant("homescreen_ja_JP.qm"));

    mp_ui->comboBox_colorScheme->addItem(QString("Default"), QVariant("default")); // TODO: make this configurable
    mp_ui->comboBox_colorScheme->addItem(QString("Demo 1"), QVariant("demo1"));
    mp_ui->comboBox_colorScheme->addItem(QString("Demo 2"), QVariant("demo2"));

    QSettings settings;
    mp_ui->comboBox_language->setCurrentIndex(settings.value("systemsettings/language", 0).toInt());
    mp_ui->comboBox_colorScheme->setCurrentIndex(settings.value("systemsettings/colorschemeindex", 0).toInt());
}

SettingsWidget::~SettingsWidget()
{
    delete mp_translator;
    delete mp_ui;
}

void SettingsWidget::updateColorScheme()
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

    mp_ui->widget_background->setStyleSheet(settings_cs.value("SettingsWidget/widget_background_css").toString());
    mp_ui->comboBox_language->setStyleSheet(settings_cs.value("SettingsWidget/comboBox_language_css").toString());
    mp_ui->comboBox_colorScheme->setStyleSheet(settings_cs.value("SettingsWidget/comboBox_colorScheme_css").toString());
    mp_ui->widget_settingsIcon->setStyleSheet(settings_cs.value("SettingsWidget/widget_settingsIcon_css").toString());
}

void SettingsWidget::changeEvent(QEvent* event)
{
    if (QEvent::LanguageChange == event->type())
    {
        mp_ui->retranslateUi(this);
    }

    QWidget::changeEvent(event);
}

void SettingsWidget::on_comboBox_language_currentIndexChanged(const QString &)
{
    if (0 != mp_translator)
        mp_translator->load(mp_ui->comboBox_language->currentData().toString(), ":/translations");

    QSettings settings;
    settings.setValue("systemsettings/language", mp_ui->comboBox_language->currentIndex());
}

void SettingsWidget::on_comboBox_colorScheme_currentIndexChanged(const QString &)
{
    QSettings settings;
    settings.setValue("systemsettings/colorscheme", mp_ui->comboBox_colorScheme->currentData().toString());
    settings.setValue("systemsettings/colorschemeindex", mp_ui->comboBox_colorScheme->currentIndex());
    // make sure that everything is written to the settings file before continuing
    settings.sync();

    emit colorSchemeChanged();
}
