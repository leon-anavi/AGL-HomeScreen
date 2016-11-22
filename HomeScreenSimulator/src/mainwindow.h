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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <include/daynightmode.hpp>
#include "daynightmode_adaptor.h"

#include "statusbar_proxy.h"
#include "popup_proxy.h"
#include "proximity_proxy.h"
#include "homescreen_proxy.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

// day/night mode
Q_SIGNALS:
    void dayNightMode(int mode);

private slots:
    void on_radioButton_DayMode_toggled(bool checked);

    void on_radioButton_NightMode_toggled(bool checked);

    void on_pushButton_Update_clicked();

    void on_pushButton_Exit_clicked();

    void on_tableWidget_Status_cellChanged(int row, int column);

    void on_pushButton_Send_clicked();

    void on_pushButton_IconExample_clicked();

    void on_pushButton_PopupExample_clicked();

    void on_pushButton_comboBoxExample_clicked();

    void on_checkBox_ObjectDetected_clicked();

private:
    Ui::MainWindow *mp_ui;
    DaynightmodeAdaptor *mp_dBusDayNightModeAdaptor;
    org::agl::statusbar *mp_dBusStatusBarProxy;
    org::agl::popup *mp_dBusPopupProxy;
    org::agl::proximity *mp_dBusProximityProxy;
    org::agl::homescreen *mp_dBusHomeScreenProxy;
};

#endif // MAINWINDOW_H
