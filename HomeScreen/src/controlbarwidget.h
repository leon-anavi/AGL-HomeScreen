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

#ifndef CONTROLBARWIDGET_H
#define CONTROLBARWIDGET_H

#include <QWidget>
#include "inputevent_proxy.h"

namespace Ui {
class ControlBarWidget;
}

class ControlBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlBarWidget(QWidget *parent = 0);
    ~ControlBarWidget();
public slots:
    void updateColorScheme();
signals:
    void hideAppLayer();

    void settingsButtonPressed();
    void homeButtonPressed();

private slots:
    void on_pushButton_home_clicked();
    void on_pushButton_settings_clicked();
    void on_pushButton_nav_clicked();

private:
    Ui::ControlBarWidget *mp_ui;
    org::agl::inputevent *mp_dBusInputEventProxy;
};

#endif // CONTROLBARWIDGET_H
