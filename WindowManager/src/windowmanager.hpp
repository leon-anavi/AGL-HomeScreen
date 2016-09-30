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

#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <QObject>
#include <QList>
#include <QMap>

#include "windowmanager_adapter.h"


typedef struct
{
    int pid;
    QString processName;
} SurfaceInfo;

#ifdef __arm__
extern "C" {
#include "ilm/ilm_control.h"
}
#endif

class WindowManager : public QObject
{
    Q_OBJECT

public:
    explicit WindowManager(QObject *parent = 0);
    QMutex callbackMutex;

    ~WindowManager();
private:
    WindowmanagerAdaptor *mp_windowManagerAdaptor;
    QMap<int, QList<SimpleRect> > m_layouts;
    QMap<int, QString> m_layoutNames;
    int m_currentLayout;
    void dumpScene();

#ifdef __arm__
    void createNewLayer(int layerId);
    void addSurfaceToLayer(int surfaceId, int layerId);

    QMap<t_ilm_uint, SurfaceInfo> *mp_surfaces;
    /* one layer per pid is created
    where the surfaces are added that are created by the process */
    QList<int> *mp_processLayers;
#endif

public:
    static void* myThis;

#ifdef __arm__
    // for general notifications
    void notificationFunc_non_static(ilmObjectType object,
                                        t_ilm_uint id,
                                        t_ilm_bool created);
    static void notificationFunc_static(ilmObjectType object,
                                        t_ilm_uint id,
                                        t_ilm_bool created,
                                        void* user_data);


    // for surface notifications
    void surfaceCallbackFunction_non_static(t_ilm_surface surface,
                                        struct ilmSurfaceProperties* surfaceProperties,
                                        t_ilm_notification_mask mask);
    static void surfaceCallbackFunction_static(t_ilm_surface surface,
                                        struct ilmSurfaceProperties* surfaceProperties,
                                        t_ilm_notification_mask mask);

#endif
public slots:


// from windowmanager_adapter.h
public Q_SLOTS: // METHODS
    int addLayout(int layoutId, const QString &layoutName, const QList<SimpleRect> &surfaceAreas);
    QList<int> getAvailableLayouts(int numberOfAppSurfaces);
    QList<SimplePoint> getAvailableSurfaces();
    int getLayout();
    QString getLayoutName(int layoutId);
    void setLayoutById(int layoutId);
    void setLayoutByName(const QString &layoutName);
    void setSurfaceToLayoutArea(int surfaceId, int layoutAreaId);

};


#endif // WINDOWMANAGER_HPP
