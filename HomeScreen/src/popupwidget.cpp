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

PopupWidget::PopupWidget(QWidget *parent) :
    QWidget(parent),
    mp_ui(new Ui::PopupWidget),
    m_dayNightMode(SystemDayNight::DAYNIGHTMODE_DAY), // TODO: read from system
    mp_dayNightModeProxy(0),
    mp_popupAdaptor(0)
{
    // this has to be adopted to the system setup
    mp_dayNightModeProxy = new org::agl::daynightmode("org.agl.homescreen.simulator", //"org.agl.systeminfoprovider"
                                                      "/",
                                                      QDBusConnection::sessionBus(),
                                                      0);
    QObject::connect(mp_dayNightModeProxy, SIGNAL(dayNightMode(int)), this, SLOT(dayNightModeSlot(int)));

    // publish dbus popup interface
    mp_popupAdaptor = new PopupAdaptor((QObject*)this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/Popup", this);
    dbus.registerService("org.agl.homescreen");

    mp_ui->setupUi(this);
    this->close();
}

PopupWidget::~PopupWidget()
{
    delete mp_dayNightModeProxy;
    delete mp_popupAdaptor;
    delete mp_ui;
}

void PopupWidget::dayNightModeSlot(int mode)
{
    switch (mode)
    {
    case SystemDayNight::DAYNIGHTMODE_DAY:
        m_dayNightMode = SystemDayNight::DAYNIGHTMODE_DAY;
        mp_ui->widget_Popup->setStyleSheet(QString("QWidget { \
                                                   border: 1px solid #D3D3D3; \
                                                   border-radius: 8px; \
                                                   background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(242, 242, 249, 255), stop:1 rgba(255, 255, 255, 255)); \
                                                   color: #333; \
                                                   padding: 0px; \
                                                   }  \
                                                   QWidget:on { \
                                                   background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #D5D5D5, stop: 1 #EEEEEE); \
                                                   }"));
        mp_ui->label_Text->setStyleSheet(QString("background-color: rgba(109, 109, 109, 0); \
                                                 background-image: url(:/images/transparency.png); \
                                                 border-image: url(:/images/transparency.png);"));
        break;
    case SystemDayNight::DAYNIGHTMODE_NIGHT:
        m_dayNightMode = SystemDayNight::DAYNIGHTMODE_NIGHT;
        mp_ui->widget_Popup->setStyleSheet(QString("QWidget { \
                                                   border: 1px solid #D3D3D3; \
                                                   border-radius: 8px; \
                                                   background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(147, 147, 151, 255), stop:1 rgba(255, 255, 255, 255)); \
                                                   color: #333; \
                                                   padding: 0px; \
                                                   }  \
                                                   QWidget:on { \
                                                   background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #D5D5D5, stop: 1 #EEEEEE); \
                                                   }"));
        mp_ui->label_Text->setStyleSheet(QString("background-color: rgba(109, 109, 109, 0); \
                                                 background-image: url(:/images/transparency.png); \
                                                 border-image: url(:/images/transparency.png);"));
        break;
    default:
        m_dayNightMode = SystemDayNight::DAYNIGHTMODE_UNDEFINED;
    }
}

void PopupWidget::showPopup(int type, const QString &text)
{
    this->show();
    this->raise();
    qDebug("PopupWidget::showPopup: type %d, text %s", type, text.toStdString().c_str());
    mp_ui->label_Text->setText(text);
}

void PopupWidget::on_pushButton_OK_clicked()
{
    this->close();
}
