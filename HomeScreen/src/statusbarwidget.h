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

#ifndef STATUSBARWIDGET_H
#define STATUSBARWIDGET_H

#include <QWidget>
#include "../interfaces/daynightmode.h"
#include "daynightmode_proxy.h"

#include "statusbar_adapter.h"

namespace Ui {
class StatusBarWidget;
}

class StatusBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatusBarWidget(QWidget *parent = 0);
    ~StatusBarWidget();

// day/night mode
public Q_SLOTS:
    void dayNightModeSlot(int mode);

// from statusbar_adapter.h
public Q_SLOTS: // METHODS
    QList<int> getAvailablePlaceholders();
    QString getStatusIcon(int placeholderIndex);
    QString getStatusText(int placeholderIndex);
    void setStatusIcon(int placeholderIndex, const QString &iconURI);
    void setStatusText(int placeholderIndex, const QString &text);

private:
    Ui::StatusBarWidget *mp_ui;

    SystemDayNight::eDayNightMode m_dayNightMode;
    org::agl::daynightmode *mp_dayNightModeProxy;
    StatusbarAdaptor *mp_statusbarAdaptor;

    QMap<int, QString> *mp_statusbarIconURIs;
};

#endif // STATUSBARWIDGET_H
