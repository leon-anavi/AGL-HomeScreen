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
#include <include/popup.hpp>
#include "popup_adaptor.h"

namespace Ui {
class PopupWidget;
}

class PopupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PopupWidget(QWidget *parent = 0);
    ~PopupWidget();
public slots:
    void updateColorScheme();

    // from popup_adaptor.h
public Q_SLOTS: // METHODS
    void showPopup(int /*type*/, const QString &text);
    void showPopupComboBox(const QString &text, const QStringList &choices);

private slots:
    void on_pushButton_OK_clicked();

signals:
    void comboBoxResult(QString choice);

private:
    Ui::PopupWidget *mp_ui;

    PopupAdaptor *mp_popupAdaptor;
    bool m_sendComboBoxChoice;
    // for showPupupFor LayoutSelection
    //QList<QPushButton> m_pushButtons;
};

#endif // POPUPWIDGET_H
