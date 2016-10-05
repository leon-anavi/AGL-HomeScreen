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

#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QTranslator>

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = 0);
    ~SettingsWidget();
public slots:
    void updateColorScheme();

protected:
    // called when the translator loaded a new language set
    void changeEvent(QEvent* event);

private slots:
    void on_comboBox_language_currentIndexChanged(const QString &);
    void on_comboBox_colorScheme_currentIndexChanged(const QString &);

signals:
    void colorSchemeChanged(void);

private:
    Ui::SettingsWidget *mp_ui;
    QTranslator *mp_translator;
};

#endif // SETTINGSWIDGET_H
