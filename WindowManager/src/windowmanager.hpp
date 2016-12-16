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

#include "windowmanager_adaptor.h"


#ifdef HAVE_IVI_LAYERMANAGEMENT_API
#include <ilm/ilm_control.h>
#endif
class WindowManager : public QObject
{
    Q_OBJECT

public:
    explicit WindowManager(int displayId, QObject *parent = 0);
    ~WindowManager();

    void start();
private:
    WindowmanagerAdaptor *mp_windowManagerAdaptor;
    QList<Layout> m_layouts;
    QMap<int, unsigned int> *mp_layoutAreaToSurfaceIdAssignment;

    int m_currentLayout;

    int m_screenId;
    unsigned int m_screenWidth;
    unsigned int m_screenHeight;

    void dumpScene();

#ifdef HAVE_IVI_LAYERMANAGEMENT_API
    t_ilm_layer* m_showLayers;
    QMap<pid_t, t_ilm_layer> m_appLayers;
    int getLayerRenderOrder(t_ilm_layer* id_array);

    void createNewLayer(const int layerId);

    t_ilm_layer getAppLayerID(const pid_t pid);

    pid_t m_pending_to_show;

    void addSurfaceToAppLayer(const int surfaceID);
    void addSurfaceToLayer(const int surfaceId, const int layerId);
#endif
    void updateScreen();


public:
    static void* myThis;

#ifdef HAVE_IVI_LAYERMANAGEMENT_API
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


// from windowmanager_adaptor.h
public: // PROPERTIES
    Q_PROPERTY(int layoutId READ layoutId)
    int layoutId() const;

    Q_PROPERTY(QString layoutName READ layoutName)
    QString layoutName() const;

public Q_SLOTS: // METHODS
    int addLayout(int layoutId, const QString &layoutName, const QList<LayoutArea> &surfaceAreas);
    int deleteLayoutById(int layoutId);
    QList<Layout> getAllLayouts();
  //    QList<int> getAllSurfacesOfProcess(int pid);
    QList<int> getAvailableLayouts(int numberOfAppSurfaces);
  //    QList<int> getAvailableSurfaces();
    QString getLayoutName(int layoutId);
    void hideLayer(int layer);
    int setLayoutById(int layoutId);
    int setLayoutByName(const QString &layoutName);
    int setSurfaceToLayoutArea(int surfaceId, int layoutAreaId);
    void showLayer(int layer);
    void showAppLayer(int pid);

Q_SIGNALS: // SIGNALS
    void surfaceVisibilityChanged(int surfaceId, bool visible);
};


#endif // WINDOWMANAGER_HPP
