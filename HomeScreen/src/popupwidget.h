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

#ifndef POPUPWIDGET_H
#define POPUPWIDGET_H

#include <QWidget>
#include "../interfaces/popup.h"
#include "popup_adapter.h"
#include "../interfaces/daynightmode.h"
#include "daynightmode_proxy.h"


namespace Ui {
class PopupWidget;
}

class PopupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PopupWidget(QWidget *parent = 0);
    ~PopupWidget();

// day/night mode
public Q_SLOTS:
    void dayNightModeSlot(int mode);

    // from popup_adapter.h
public Q_SLOTS: // METHODS
    void showPopup(int type, const QString &text);

private slots:
    void on_pushButton_OK_clicked();

private:
    Ui::PopupWidget *mp_ui;

    SystemDayNight::eDayNightMode m_dayNightMode;
    org::agl::daynightmode *mp_dayNightModeProxy;
    PopupAdaptor *mp_popupAdaptor;
};

#endif // POPUPWIDGET_H
