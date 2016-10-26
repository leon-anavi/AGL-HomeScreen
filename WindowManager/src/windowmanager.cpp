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

#include "windowmanager.hpp"
#include <wayland-client.h>
#include <QFile>

//////////////////////////////////////////
// THIS IS STILL UNDER HEAVY DEVELOPMENT!
// DO NOT JUDGE THE SOURCE CODE :)
//////////////////////////////////////////

void* WindowManager::myThis = 0;

WindowManager::WindowManager(QObject *parent) :
    QObject(parent),
    m_layouts(),
    m_layoutNames(),
    m_layoutFullScreen(),
    m_layoutFullScreenAssociated(),
    m_currentLayout(-1),
    m_homeScreenPid(-1),
#ifdef __arm__
    mp_surfaces(0),
    mp_processLayers(0),
#endif
    mp_layoutAreaToPidAssignment(0)
{
    qDebug("-=[WindowManager]=-");
    qDebug("WindowManager");
    // publish windowmanager interface
    mp_windowManagerAdaptor = new WindowmanagerAdaptor((QObject*)this);

    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/windowmanager", this);
    dbus.registerService("org.agl.windowmanager");
}

void WindowManager::start()
{
    qDebug("-=[start]=-");
    mp_layoutAreaToPidAssignment = new QMap<int, unsigned int>;
#ifdef __arm__
    mp_processLayers = new QList<int>;
    mp_surfaces = new QMap<t_ilm_uint, SurfaceInfo>;

    ilmErrorTypes err;

    err = ilm_init();
    qDebug("ilm_init = %d", err);

    myThis = this;
    err =  ilm_registerNotification(WindowManager::notificationFunc_static, this);
#endif
}

WindowManager::~WindowManager()
{
    qDebug("-=[~WindowManager]=-");
    delete mp_windowManagerAdaptor;
#ifdef __arm__
    delete mp_surfaces;
    delete mp_processLayers;

    ilm_destroy();
#endif
    delete mp_layoutAreaToPidAssignment;
}

void WindowManager::dumpScene()
{
    qDebug("\n");
    qDebug("current layout   : %d", m_currentLayout);
    qDebug("available layouts: %d", m_layouts.size());
    QMap<int, QList<SimpleRect> >::iterator i = m_layouts.begin();

    QList<int> result;
    while (i != m_layouts.constEnd())
    {
        qDebug("--[id: %d]--[%s]--", i.key(), m_layoutNames.find(i.key()).value().toStdString().c_str());
        qDebug("  %d surface areas", i.value().size());
        for (int j = 0; j < i.value().size(); ++j)
        {
            qDebug("  -area %d", j);
            qDebug("    -x     : %d", i.value().at(j).x);
            qDebug("    -y     : %d", i.value().at(j).y);
            qDebug("    -width : %d", i.value().at(j).width);
            qDebug("    -height: %d", i.value().at(j).height);
        }

        ++i;
    }
}

#ifdef __arm__

void WindowManager::createNewLayer(int layerId)
{
    qDebug("-=[createNewLayer]=-");
    qDebug("layerId %d", layerId);
    ilmErrorTypes err;

    t_ilm_uint screenID = 0;
    t_ilm_uint width;
    t_ilm_uint height;

    err = ilm_getScreenResolution(screenID, &width, &height);

    t_ilm_layer newLayerId = layerId;
    err = ilm_layerCreateWithDimension(&newLayerId, width, height);

    t_ilm_float opacity = 0.0;
    err =  ilm_layerSetOpacity(newLayerId, opacity);

    ilm_layerSetVisibility(newLayerId, ILM_FALSE);

    ilm_commitChanges();
}

void WindowManager::addSurfaceToLayer(int surfaceId, int layerId)
{
    qDebug("-=[addSurfaceToLayer]=-");
    qDebug("surfaceId %d", surfaceId);
    qDebug("layerId %d", layerId);
    t_ilm_int length;
    t_ilm_layer* pArray;

    ilm_getLayerIDs(&length, &pArray);
    bool layerFound(false);
    for (int i = 0; i < length; ++i)
    {
        if (layerId == pArray[i])
        {
            layerFound = true;
        }
    }

    if (!layerFound)
    {
        createNewLayer(layerId);
    }

    struct ilmSurfaceProperties surfaceProperties;
    ilm_getPropertiesOfSurface(surfaceId, &surfaceProperties);
    //qDebug("  origSourceWidth : %d", surfaceProperties.origSourceWidth);
    //qDebug("  origSourceHeight: %d", surfaceProperties.origSourceHeight);

    ilm_layerSetSourceRectangle(layerId,
                                     0,
                                     0,
                                     surfaceProperties.origSourceWidth,
                                     surfaceProperties.origSourceHeight);
    ilm_commitChanges();

    ilm_surfaceSetDestinationRectangle(surfaceId, 0, 0, surfaceProperties.origSourceWidth, surfaceProperties.origSourceHeight);
    ilm_surfaceSetSourceRectangle(surfaceId, 0, 0, surfaceProperties.origSourceWidth, surfaceProperties.origSourceHeight);
    ilm_surfaceSetOpacity(surfaceId, 1.0);
    ilm_surfaceSetVisibility(surfaceId, true);

    ilm_layerAddSurface(layerId, surfaceId);

    ilm_commitChanges();
}

void WindowManager::updateScreen()
{
    qDebug("-=[updateScreen]=-");
    int numberOfLayersToShow = 0;

    QMap<int, QList<SimpleRect> >::iterator i = m_layouts.find(m_currentLayout);
    if (m_layouts.end() != i)
    {
        numberOfLayersToShow += i.value().size();
        qDebug("the current layout provides %d render areas", i.value().size());
    }
    else
    {
        qDebug("the current layout provides no render areas!");
    }


    t_ilm_layer renderOrder[numberOfLayersToShow];
    int renderOrderCounter = 0;

    qDebug("show home screen app");
    if (-1 != m_homeScreenPid)
    {
        renderOrder[renderOrderCounter] = m_homeScreenPid;
        ++renderOrderCounter;

        ilm_layerSetVisibility(m_homeScreenPid, ILM_TRUE);
        t_ilm_float opacity = 1.0;
        ilm_layerSetOpacity(m_homeScreenPid, opacity);

        // homescreen app always fullscreen in the back
        t_ilm_uint screenID = 0;
        t_ilm_uint width;
        t_ilm_uint height;

        ilm_getScreenResolution(screenID, &width, &height);

        ilm_layerSetDestinationRectangle(m_homeScreenPid,
                                         0,
                                         0,
                                         width,
                                         height);
    }


    qDebug("show %d apps", numberOfLayersToShow);
    for (int j = 0; j < numberOfLayersToShow; ++j)
    {
        int layerToShow = mp_layoutAreaToPidAssignment->find(j).value();
        if (layerToShow != m_homeScreenPid)
        {
            qDebug("  app no. %d: %d", j, layerToShow);
            renderOrder[renderOrderCounter] = layerToShow;
            ++renderOrderCounter;

            ilm_layerSetVisibility(layerToShow, ILM_TRUE);
            t_ilm_float opacity = 1.0;
            ilm_layerSetOpacity(layerToShow, opacity);

            qDebug("  layout area %d", j);
            qDebug("    x: %d", m_layouts.find(m_currentLayout).value()[j].x);
            qDebug("    x: %d", m_layouts.find(m_currentLayout).value()[j].y);
            qDebug("    w: %d", m_layouts.find(m_currentLayout).value()[j].width);
            qDebug("    h: %d", m_layouts.find(m_currentLayout).value()[j].height);

            ilm_layerSetDestinationRectangle(layerToShow,
                                             m_layouts.find(m_currentLayout).value()[j].x,
                                             m_layouts.find(m_currentLayout).value()[j].y,
                                             m_layouts.find(m_currentLayout).value()[j].width,
                                             m_layouts.find(m_currentLayout).value()[j].height);
        }
    }

    qDebug("renderOrder");
    for (int j = 0; j < renderOrderCounter; ++j)
    {
        qDebug("  %d: %d", j, renderOrder[j]);
    }

    ilm_displaySetRenderOrder(0, renderOrder, renderOrderCounter);

    ilm_commitChanges();
}

void WindowManager::notificationFunc_non_static(ilmObjectType object,
                                    t_ilm_uint id,
                                    t_ilm_bool created)
{
    qDebug("-=[notificationFunc_non_static]=-");
    qDebug("Notification from weston!");
    if (ILM_SURFACE == object)
    {
        struct ilmSurfaceProperties surfaceProperties;

        if (created)
        {
            qDebug("Surface created, ID: %d", id);
            //qDebug("Surface created, ID: 0x%s", QString(QByteArray::number(id,16)).toStdString().c_str());
            ilm_getPropertiesOfSurface(id, &surfaceProperties);
            qDebug("  origSourceWidth : %d", surfaceProperties.origSourceWidth);
            qDebug("  origSourceHeight: %d", surfaceProperties.origSourceHeight);
            SurfaceInfo surfaceInfo;
            surfaceInfo.pid = surfaceProperties.creatorPid;
            QString procInfoFileName = QString("/proc/") + QString::number(surfaceInfo.pid) + QString("/comm");
            QFile procInfo(procInfoFileName);
            if (procInfo.open(QIODevice::ReadOnly))
            {
                QTextStream in(&procInfo);
                surfaceInfo.processName = in.readLine();
                qDebug("  creator pid %d %s", surfaceInfo.pid, surfaceInfo.processName.toStdString().c_str());
                addSurfaceToLayer(id, surfaceProperties.creatorPid);

                mp_surfaces->insert(id, surfaceInfo);
                ilm_surfaceAddNotification(id, surfaceCallbackFunction_static);

                ilm_commitChanges();

                if (0 == QString::compare("weston-fullscre", surfaceInfo.processName))
                {
                    qDebug("HomeScreen app detected");
                    m_homeScreenPid = surfaceInfo.pid;
                    updateScreen();
                }
            }
            else
            {
                qDebug("no creator pid found. Ignoring surface!");
            }
        }
        else
        {
            qDebug("Surface destroyed, ID: %d", id);
            mp_surfaces->erase(mp_surfaces->find(id));
            ilm_surfaceRemoveNotification(id);

            ilm_commitChanges();
        }
    }
    if (ILM_LAYER == object)
    {
        //qDebug("Layer.. we don't care...");
    }
}

void WindowManager::notificationFunc_static(ilmObjectType object,
                                            t_ilm_uint id,
                                            t_ilm_bool created,
                                            void* user_data)
{
    static_cast<WindowManager*>(WindowManager::myThis)->notificationFunc_non_static(object, id, created);
}




void WindowManager::surfaceCallbackFunction_non_static(t_ilm_surface surface,
                                    struct ilmSurfaceProperties* surfaceProperties,
                                    t_ilm_notification_mask mask)
{
    qDebug("-=[surfaceCallbackFunction_non_static]=-");
    qDebug("surfaceCallbackFunction_non_static changes for surface %d", surface);
    if (ILM_NOTIFICATION_VISIBILITY & mask)
    {
        qDebug("ILM_NOTIFICATION_VISIBILITY");
    }

    if (ILM_NOTIFICATION_OPACITY & mask)
    {
        qDebug("ILM_NOTIFICATION_OPACITY");
    }

    if (ILM_NOTIFICATION_ORIENTATION & mask)
    {
        qDebug("ILM_NOTIFICATION_ORIENTATION");
    }

    if (ILM_NOTIFICATION_SOURCE_RECT & mask)
    {
        qDebug("ILM_NOTIFICATION_SOURCE_RECT");
    }

    if (ILM_NOTIFICATION_DEST_RECT & mask)
    {
        qDebug("ILM_NOTIFICATION_DEST_RECT");
    }
}

void WindowManager::surfaceCallbackFunction_static(t_ilm_surface surface,
                                    struct ilmSurfaceProperties* surfaceProperties,
                                    t_ilm_notification_mask mask)

{
    static_cast<WindowManager*>(WindowManager::myThis)->surfaceCallbackFunction_non_static(surface, surfaceProperties, mask);
}
#endif

int WindowManager::addLayout(int layoutId, const QString &layoutName, bool isFullScreen, int associatedFullScreenLayout, const QList<SimpleRect> &surfaceAreas)
{
    qDebug("-=[addLayout]=-");
    m_layouts.insert(layoutId, surfaceAreas);
    m_layoutNames.insert(layoutId, layoutName);
    m_layoutFullScreen.insert(layoutId, isFullScreen);
    m_layoutFullScreenAssociated.insert(layoutId, associatedFullScreenLayout);
    qDebug("addLayout %d %s %s, %d, size %d",
           layoutId,
           layoutName.toStdString().c_str(),
           isFullScreen ? "true" : "false",
           associatedFullScreenLayout,
           surfaceAreas.size());

    dumpScene();

    return true;
}

int WindowManager::getAssociatedFullScreenLayout(int layoutId)
{
    qDebug("-=[getAssociatedFullScreenLayout]=-");
    return m_layoutFullScreenAssociated.find(layoutId).value();
}

QList<int> WindowManager::getAvailableLayouts(int numberOfAppSurfaces)
{
    qDebug("-=[getAvailableLayouts]=-");
    QMap<int, QList<SimpleRect> >::iterator i = m_layouts.begin();

    QList<int> result;
    while (i != m_layouts.constEnd())
    {
        if (i.value().size() == numberOfAppSurfaces)
        {
            result.append(i.key());
        }

        ++i;
    }

    return result;
}

// maybe not needed anymore
QList<SimplePoint> WindowManager::getAvailableSurfaces()
{
    qDebug("-=[getAvailableSurfaces]=-");
    QList<SimplePoint> points;
    SimplePoint point;
    point.x = 1;
    point.y = 2;
    points.append(point);
    point.x = 11;
    point.y = 22;
    points.append(point);
    point.x = 111;
    point.y = 222;
    points.append(point);

    return points;
}

int WindowManager::getLayout()
{
    qDebug("-=[getLayout]=-");
    return m_currentLayout;
}

QString WindowManager::getLayoutName(int layoutId)
{
    qDebug("-=[getLayoutName]=-");
    return m_layoutNames.find(layoutId).value();
}

bool WindowManager::isLayoutFullScreen(int layoutId)
{
    qDebug("-=[isLayoutFullScreen]=-");
    return m_layoutFullScreen.find(layoutId).value();
}

void WindowManager::setLayoutById(int layoutId)
{
    qDebug("-=[setLayoutById]=-");
    m_currentLayout = layoutId;

    mp_layoutAreaToPidAssignment->clear();

    dumpScene();
}

void WindowManager::setLayoutByName(const QString &layoutName)
{
    qDebug("-=[setLayoutByName]=-");
    QMap<int, QString>::iterator i = m_layoutNames.begin();
    while (i != m_layoutNames.constEnd())
    {
        if (i.value() == layoutName)
        {
            m_currentLayout = i.key();
        }
        ++i;
    }

    mp_layoutAreaToPidAssignment->clear();

    dumpScene();
}

void WindowManager::setPidToLayoutArea(int pid, int layoutAreaId)
{
    qDebug("-=[setPidToLayoutArea]=-");
    qDebug("pid %d", pid);
    qDebug("layoutAreaId %d", layoutAreaId);
    mp_layoutAreaToPidAssignment->insert(layoutAreaId, pid);

#ifdef __arm__
    updateScreen();
#endif

    dumpScene();
}
