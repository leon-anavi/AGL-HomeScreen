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
    m_currentLayout(-1)
{
    qDebug("WindowManager");
    // publish windowmanager interface
    mp_windowManagerAdaptor = new WindowmanagerAdaptor((QObject*)this);

    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/windowmanager", this);
    dbus.registerService("org.agl.windowmanager");

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
    delete mp_windowManagerAdaptor;
#ifdef __arm__
    delete mp_surfaces;

    ilm_destroy();
#endif
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
    ilmErrorTypes err;

    t_ilm_uint screenID = 0;
    t_ilm_uint width;
    t_ilm_uint height;

    err = ilm_getScreenResolution(screenID, &width, &height);

    t_ilm_layer newLayerId = layerId;
    err = ilm_layerCreateWithDimension(&newLayerId, width, height);
    qDebug("ilm_layerCreateWithDimension = %d", err);
    qDebug("layerIdWallpaper = %d", newLayerId);

    err = ilm_layerSetVisibility(newLayerId, true);
    qDebug("ilm_layerSetVisibility = %d", err);

    t_ilm_float opacity = 1.0;
    err =  ilm_layerSetOpacity(newLayerId, opacity);

    ilm_commitChanges();
}

void WindowManager::addSurfaceToLayer(int surfaceId, int layerId)
{
    t_ilm_int length;
    t_ilm_layer* pArray;

    ilm_getLayerIDs(&length, &pArray);
    bool layerFound(false);
    for (int i = 0; i< length; ++i)
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
    qDebug("  origSourceWidth : %d", surfaceProperties.origSourceWidth);
    qDebug("  origSourceHeight: %d", surfaceProperties.origSourceHeight);

    ilm_surfaceSetDestinationRectangle(surfaceId, 0, 0, surfaceProperties.origSourceWidth, surfaceProperties.origSourceHeight);
    ilm_surfaceSetSourceRectangle(surfaceId, 0, 0, surfaceProperties.origSourceWidth, surfaceProperties.origSourceHeight);
    ilm_surfaceSetOpacity(surfaceId, 1.0);
    ilm_surfaceSetVisibility(surfaceId, true);

    ilm_layerAddSurface(layerId, surfaceId);

    ilm_commitChanges();
}

void WindowManager::notificationFunc_non_static(ilmObjectType object,
                                    t_ilm_uint id,
                                    t_ilm_bool created)
{
    if (ILM_SURFACE == object)
    {
        struct ilmSurfaceProperties surfaceProperties;

        if (created)
        {
            qDebug("Surface created, ID: %d", id);
            ilm_getPropertiesOfSurface(id, &surfaceProperties);
            qDebug("  origSourceWidth : %d", surfaceProperties.origSourceWidth);
            qDebug("  origSourceHeight: %d", surfaceProperties.origSourceHeight);

            addSurfaceToLayer(id, surfaceProperties.creatorPid);

            t_ilm_int length;
            t_ilm_surface* pArray;
            ilm_getSurfaceIDs(&length, &pArray);
            ilm_layerSetRenderOrder(42, pArray, length);

            ilm_commitChanges();

            SurfaceInfo surfaceInfo;
            surfaceInfo.pid = surfaceProperties.creatorPid;
            QString procInfoFileName = QString("/proc/") + QString::number(surfaceInfo.pid) + QString("/comm");
            QFile procInfo(procInfoFileName);
            if (procInfo.open(QIODevice::ReadOnly))
            {
                QTextStream in(&procInfo);
                surfaceInfo.processName = in.readLine();
                qDebug("surface id %d, pid %d: %s", id, surfaceInfo.pid, surfaceInfo.processName.toStdString().c_str());
            }

            mp_surfaces->insert(id, surfaceInfo);
            ilm_surfaceAddNotification(id, surfaceCallbackFunction_static);
        }
        else
        {
            qDebug("Surface destroyed, ID: %d", id);
            mp_surfaces->erase(mp_surfaces->find(id));
            ilm_surfaceRemoveNotification(id);
        }

        // rearrange surfaces on screen
        t_ilm_uint screenID = 0;
        t_ilm_uint width;
        t_ilm_uint height;
        ilm_getScreenResolution(screenID, &width, &height);

        qDebug("%d surfaces to show", mp_surfaces->count());

        QMap<t_ilm_uint, SurfaceInfo>::const_iterator i = mp_surfaces->constBegin();
        int counter(0);
        while (i != mp_surfaces->constEnd())
        {
            qDebug("place surface %d at x: %f, y: %d, width: %f, height: %d",
                   i.key(),
                   counter * (width / (1.0 * mp_surfaces->count())),
                   0,
                   width / (1.0 * mp_surfaces->count()),
                   height);
            ilm_surfaceSetDestinationRectangle(i.key(),
                    counter * (width / (1.0 * mp_surfaces->count())),
                    0,
                    width / (1.0 * mp_surfaces->count()),
                    height);

            ++i;
            ++counter;
        }


        ilm_commitChanges();
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

int WindowManager::addLayout(int layoutId, const QString &layoutName, const QList<SimpleRect> &surfaceAreas)
{
    m_layouts.insert(layoutId, surfaceAreas);
    m_layoutNames.insert(layoutId, layoutName);
    qDebug("addLayout %d %s, size %d", layoutId, layoutName.toStdString().c_str(), surfaceAreas.size());

    dumpScene();

    return true;
}

QList<int> WindowManager::getAvailableLayouts(int numberOfAppSurfaces)
{
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
    return m_currentLayout;
}

QString WindowManager::getLayoutName(int layoutId)
{
    return m_layoutNames.find(layoutId).value();
}

void WindowManager::setLayoutById(int layoutId)
{
    m_currentLayout = layoutId;

    dumpScene();
}

void WindowManager::setLayoutByName(const QString &layoutName)
{
    QMap<int, QString>::iterator i = m_layoutNames.begin();
    while (i != m_layoutNames.constEnd())
    {
        if (i.value() == layoutName)
        {
            m_currentLayout = i.key();
        }
        ++i;
    }

    dumpScene();
}

void WindowManager::setSurfaceToLayoutArea(int surfaceId, int layoutAreaId)
{
    dumpScene();
}
