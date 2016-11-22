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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mp_ui(new Ui::MainWindow),
    mp_dBusDayNightModeAdaptor(0),
    mp_dBusStatusBarProxy(0),
    mp_dBusPopupProxy(0),
    mp_dBusProximityProxy(0),
    mp_dBusHomeScreenProxy(0)
{
    mp_ui->setupUi(this);

    // dbus setup
    QDBusConnection dbus = QDBusConnection::sessionBus();

    // publish dbus day night mode interface
    mp_dBusDayNightModeAdaptor = new DaynightmodeAdaptor((QObject*)this);

    dbus.registerService("org.agl.homescreen.simulator");
    dbus.registerObject("/", this);

    mp_dBusStatusBarProxy = new org::agl::statusbar("org.agl.homescreen",
                                              "/StatusBar",
                                              QDBusConnection::sessionBus(),
                                              0);
    mp_dBusPopupProxy = new org::agl::popup("org.agl.homescreen",
                                              "/Popup",
                                              QDBusConnection::sessionBus(),
                                              0);
    mp_dBusProximityProxy = new org::agl::proximity("org.agl.homescreen",
                                              "/Proximity",
                                              QDBusConnection::sessionBus(),
                                              0);
    mp_dBusHomeScreenProxy = new org::agl::homescreen("org.agl.homescreen",
                                                     "/HomeScreen",
                                                     QDBusConnection::sessionBus(),
                                                     0);

    QSettings settings;
    this->move(settings.value("homescreensimulator/pos").toPoint());
    mp_ui->radioButton_DayMode->setChecked(settings.value("homescreensimulator/daymode", true).toBool()); // if nothing is stored, use "true"
    mp_ui->radioButton_NightMode->setChecked(settings.value("homescreensimulator/nightmode", false).toBool()); // if nothing is stored, use "false"
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.setValue("homescreensimulator/pos", this->pos());
    settings.setValue("homescreensimulator/daymode", mp_ui->radioButton_DayMode->isChecked());
    settings.setValue("homescreensimulator/nightmode", mp_ui->radioButton_NightMode->isChecked());

    delete mp_dBusHomeScreenProxy;
    delete mp_dBusProximityProxy;
    delete mp_dBusPopupProxy;
    delete mp_dBusStatusBarProxy;

    if (0 == mp_dBusDayNightModeAdaptor)
    {
        delete mp_dBusDayNightModeAdaptor;
    }
    delete mp_ui;
}

void MainWindow::on_radioButton_DayMode_toggled(bool checked)
{
    if (checked)
    {
        emit(dayNightMode(SystemDayNight::DAYNIGHTMODE_DAY));
    }
}

void MainWindow::on_radioButton_NightMode_toggled(bool checked)
{
    if (checked)
    {
        emit(dayNightMode(SystemDayNight::DAYNIGHTMODE_NIGHT));
    }
}

void MainWindow::on_pushButton_Update_clicked()
{
    QList<int> availablePlaceholders = mp_dBusStatusBarProxy->getAvailablePlaceholders();
    mp_ui->tableWidget_Status->verticalHeader()->setVisible(false);
    mp_ui->tableWidget_Status->clear();
    mp_ui->tableWidget_Status->setRowCount(availablePlaceholders.size());
    mp_ui->tableWidget_Status->setColumnCount(3);

    QList<QString> horizontalHeaderLabels;
    horizontalHeaderLabels.append("Placeholder #");
    horizontalHeaderLabels.append("Text");
    horizontalHeaderLabels.append("iconURI");
    mp_ui->tableWidget_Status->setHorizontalHeaderLabels(horizontalHeaderLabels);

    // disable trigger for the update
    mp_ui->tableWidget_Status->blockSignals(true);

    for (int i = 0; i < availablePlaceholders.size(); ++i)
    {
        mp_ui->tableWidget_Status->setItem(i, 0, new QTableWidgetItem(QString::number(availablePlaceholders[i])));
        mp_ui->tableWidget_Status->item(i, 0)->setFlags(Qt::ItemIsSelectable);
        mp_ui->tableWidget_Status->setItem(i, 1, new QTableWidgetItem(mp_dBusStatusBarProxy->getStatusText(availablePlaceholders[i])));
        mp_ui->tableWidget_Status->setItem(i, 2, new QTableWidgetItem(mp_dBusStatusBarProxy->getStatusIcon(availablePlaceholders[i])));
    }

    // enable trigger again
    mp_ui->tableWidget_Status->blockSignals(false);
}

void MainWindow::on_pushButton_Exit_clicked()
{
    QApplication::exit();
}

void MainWindow::on_tableWidget_Status_cellChanged(int row, int column)
{
    // text
    if (1 == column)
    {
        mp_dBusStatusBarProxy->setStatusText(mp_ui->tableWidget_Status->item(row, 0)->text().toInt(), mp_ui->tableWidget_Status->item(row, 1)->text());
    }

    // icon
    if (2 == column)
    {
        mp_dBusStatusBarProxy->setStatusIcon(mp_ui->tableWidget_Status->item(row, 0)->text().toInt(), mp_ui->tableWidget_Status->item(row, 2)->text());
    }
}

void MainWindow::on_pushButton_Send_clicked()
{
    mp_dBusPopupProxy->showPopup(0, mp_ui->plainTextEdit_Popup->toPlainText());
}

void MainWindow::on_pushButton_IconExample_clicked()
{
    on_pushButton_Update_clicked();

    // THIS IS JUST AN EXAMPLE!!! It only works, because the alias for the pic is defined in the
    // Home Screen app resources...
    mp_ui->tableWidget_Status->setItem(0, 2, new QTableWidgetItem(":/icons/home_day.png"));
}

void MainWindow::on_pushButton_PopupExample_clicked()
{
    mp_dBusPopupProxy->showPopup(0, "Hello Popup!\nNice to see you!");
}

void MainWindow::on_pushButton_comboBoxExample_clicked()
{
    QStringList choices;
    choices.append("Item1");
    choices.append("Item2");
    choices.append("Item3");
    choices.append("Item4");
    mp_dBusPopupProxy->showPopupComboBox("Make your choice!", choices);
}

void MainWindow::on_checkBox_ObjectDetected_clicked()
{
    mp_dBusProximityProxy->setObjectDetected(Qt::Checked == mp_ui->checkBox_ObjectDetected->checkState());
}
