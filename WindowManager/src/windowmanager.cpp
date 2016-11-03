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

// three layers will be defined. The HomeScreen will be placed
// full screen in the background.
// On top all applications in one layer.
// On top of that, the popup layer.
#define WINDOWMANAGER_LAYER_POPUP 100
#define WINDOWMANAGER_LAYER_APPLICATIONS 101
#define WINDOWMANAGER_LAYER_HOMESCREEN 102

#define WINDOWMANAGER_LAYER_NUM 3

// the HomeScreen app has to have the surface id 1000
#define WINDOWMANAGER_HOMESCREEN_MAIN_SURFACE_ID 1000


void* WindowManager::myThis = 0;

WindowManager::WindowManager(QObject *parent) :
    QObject(parent),
    m_layouts(),
    m_surfaces(),
    mp_layoutAreaToSurfaceIdAssignment(0),
    m_currentLayout(-1)
{
    qDebug("-=[WindowManager]=-");
    // publish windowmanager interface
    mp_windowManagerAdaptor = new WindowmanagerAdaptor((QObject*)this);

    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/windowmanager", this);
    dbus.registerService("org.agl.windowmanager");
}

void WindowManager::start()
{
    qDebug("-=[start]=-");
    mp_layoutAreaToSurfaceIdAssignment = new QMap<int, unsigned int>;
#ifdef __arm__
    ilmErrorTypes err;

    err = ilm_init();
    qDebug("ilm_init = %d", err);

    myThis = this;
    err =  ilm_registerNotification(WindowManager::notificationFunc_static, this);

    createNewLayer(WINDOWMANAGER_LAYER_POPUP);
    createNewLayer(WINDOWMANAGER_LAYER_APPLICATIONS);
    createNewLayer(WINDOWMANAGER_LAYER_HOMESCREEN);
#endif
}

WindowManager::~WindowManager()
{
    qDebug("-=[~WindowManager]=-");
    delete mp_windowManagerAdaptor;
#ifdef __arm__
    ilm_destroy();
#endif
    delete mp_layoutAreaToSurfaceIdAssignment;
}

void WindowManager::dumpScene()
{
    qDebug("\n");
    qDebug("current layout   : %d", m_currentLayout);
    qDebug("available layouts: %d", m_layouts.size());
    QList<Layout>::const_iterator i = m_layouts.begin();

    while (i != m_layouts.constEnd())
    {
        qDebug("--[id: %d]--[%s]--", i->id, i->name.toStdString().c_str());
        qDebug("  %d surface areas", i->layoutAreas.size());
        for (int j = 0; j < i->layoutAreas.size(); ++j)
        {
            qDebug("  -area %d", j);
            qDebug("    -x     : %d", i->layoutAreas.at(j).x);
            qDebug("    -y     : %d", i->layoutAreas.at(j).y);
            qDebug("    -width : %d", i->layoutAreas.at(j).width);
            qDebug("    -height: %d", i->layoutAreas.at(j).height);
        }

        ++i;
    }
}

#ifdef __arm__

void WindowManager::createNewLayer(int layerId)
{
    qDebug("-=[createNewLayer]=-");
    qDebug("layerId %d", layerId);

    t_ilm_uint screenID = 0;
    t_ilm_uint width;
    t_ilm_uint height;

    ilm_getScreenResolution(screenID, &width, &height);

    t_ilm_layer newLayerId = layerId;
    ilm_layerCreateWithDimension(&newLayerId, width, height);
    ilm_layerSetOpacity(newLayerId, 1.0);
    ilm_layerSetVisibility(newLayerId, ILM_TRUE);
    ilm_layerSetSourceRectangle(newLayerId,
                                    0,
                                    0,
                                    width,
                                    height);
    ilm_layerSetDestinationRectangle(newLayerId,
                                    0,
                                    0,
                                    width,
                                    height);

    ilm_commitChanges();
}

void WindowManager::addSurfaceToLayer(int surfaceId, int layerId)
{
    qDebug("-=[addSurfaceToLayer]=-");
    qDebug("surfaceId %d", surfaceId);
    qDebug("layerId %d", layerId);

    if (layerId == WINDOWMANAGER_LAYER_HOMESCREEN)
    {
        struct ilmSurfaceProperties surfaceProperties;
        ilm_getPropertiesOfSurface(surfaceId, &surfaceProperties);

        qDebug("sourceX %d", surfaceProperties.sourceX);
        qDebug("sourceY %d", surfaceProperties.sourceY);
        qDebug("sourceWidth %d", surfaceProperties.sourceWidth);
        qDebug("sourceHeight %d", surfaceProperties.sourceHeight);

        // homescreen app always fullscreen in the back
        t_ilm_uint screenID = 0;
        t_ilm_uint width;
        t_ilm_uint height;

        ilm_getScreenResolution(screenID, &width, &height);

        ilm_surfaceSetDestinationRectangle(surfaceId, 0, 0, width, height);
        ilm_surfaceSetSourceRectangle(surfaceId, 0, 0, width, height);
        ilm_surfaceSetOpacity(surfaceId, 1.0);
        ilm_surfaceSetVisibility(surfaceId, ILM_TRUE);

        ilm_layerAddSurface(layerId, surfaceId);
    }

    if (layerId == WINDOWMANAGER_LAYER_APPLICATIONS)
    {
        struct ilmSurfaceProperties surfaceProperties;
        ilm_getPropertiesOfSurface(surfaceId, &surfaceProperties);

        ilm_surfaceSetDestinationRectangle(surfaceId, 0, 0, surfaceProperties.origSourceWidth, surfaceProperties.origSourceHeight);
        ilm_surfaceSetSourceRectangle(surfaceId, 0, 0, surfaceProperties.origSourceWidth, surfaceProperties.origSourceHeight);
        ilm_surfaceSetOpacity(surfaceId, 0.0);
        ilm_surfaceSetVisibility(surfaceId, ILM_FALSE);

        ilm_layerAddSurface(layerId, surfaceId);
    }

    if (layerId == WINDOWMANAGER_LAYER_POPUP)
    {
        struct ilmSurfaceProperties surfaceProperties;
        ilm_getPropertiesOfSurface(surfaceId, &surfaceProperties);

        ilm_surfaceSetDestinationRectangle(surfaceId, 0, 0, surfaceProperties.origSourceWidth, surfaceProperties.origSourceHeight);
        ilm_surfaceSetSourceRectangle(surfaceId, 0, 0, surfaceProperties.origSourceWidth, surfaceProperties.origSourceHeight);
        ilm_surfaceSetOpacity(surfaceId, 0.0);
        ilm_surfaceSetVisibility(surfaceId, ILM_FALSE);

        ilm_layerAddSurface(layerId, surfaceId);
    }

    ilm_commitChanges();
}

#endif

void WindowManager::updateScreen()
{
    qDebug("-=[updateScreen]=-");

#ifdef __arm__
    if (-1 != m_currentLayout)
    {

        // hide all surfaces
        for (int i = 0; i < m_surfaces.size(); ++i)
        {
            ilm_surfaceSetVisibility(m_surfaces.at(i), ILM_FALSE);
            ilm_surfaceSetOpacity(m_surfaces.at(i), 0.0);
        }

        // find the current used layout
        QList<Layout>::const_iterator ci = m_layouts.begin();

        Layout currentLayout;
        while (ci != m_layouts.constEnd())
        {
            if (ci->id == m_currentLayout)
            {
                currentLayout = *ci;
            }

            ++ci;
        }

        qDebug("show %d apps", mp_layoutAreaToSurfaceIdAssignment->size());
        for (int j = 0; j < mp_layoutAreaToSurfaceIdAssignment->size(); ++j)
        {
            int surfaceToShow = mp_layoutAreaToSurfaceIdAssignment->find(j).value();
            qDebug("  surface no. %d: %d", j, surfaceToShow);

            ilm_surfaceSetVisibility(surfaceToShow, ILM_TRUE);
            ilm_surfaceSetOpacity(surfaceToShow, 1.0);

            qDebug("  layout area %d", j);
            qDebug("    x: %d", currentLayout.layoutAreas[j].x);
            qDebug("    y: %d", currentLayout.layoutAreas[j].y);
            qDebug("    w: %d", currentLayout.layoutAreas[j].width);
            qDebug("    h: %d", currentLayout.layoutAreas[j].height);

            ilm_surfaceSetDestinationRectangle(surfaceToShow,
                                             currentLayout.layoutAreas[j].x,
                                             currentLayout.layoutAreas[j].y,
                                             currentLayout.layoutAreas[j].width,
                                             currentLayout.layoutAreas[j].height);
        }

        ilm_commitChanges();
    }

    t_ilm_layer renderOrder[WINDOWMANAGER_LAYER_NUM];
    renderOrder[0] = WINDOWMANAGER_LAYER_HOMESCREEN;
    renderOrder[1] = WINDOWMANAGER_LAYER_APPLICATIONS;
    renderOrder[2] = WINDOWMANAGER_LAYER_POPUP;

    ilm_displaySetRenderOrder(0, renderOrder, WINDOWMANAGER_LAYER_NUM);

    ilm_commitChanges();

#endif
}

#ifdef __arm__
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
            ilm_getPropertiesOfSurface(id, &surfaceProperties);
            qDebug("  origSourceWidth : %d", surfaceProperties.origSourceWidth);
            qDebug("  origSourceHeight: %d", surfaceProperties.origSourceHeight);

            if (WINDOWMANAGER_HOMESCREEN_MAIN_SURFACE_ID == id)
            {
                qDebug("HomeScreen app detected");
                addSurfaceToLayer(id, WINDOWMANAGER_LAYER_HOMESCREEN);
                updateScreen();
            }
            else
            {
                addSurfaceToLayer(id, WINDOWMANAGER_LAYER_APPLICATIONS);

                m_surfaces.append(id);
            }
            ilm_surfaceAddNotification(id, surfaceCallbackFunction_static);

            ilm_commitChanges();
        }
        else
        {
            qDebug("Surface destroyed, ID: %d", id);
            m_surfaces.removeAt(m_surfaces.indexOf(id));
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
    if (ILM_NOTIFICATION_CONTENT_AVAILABLE & mask)
    {
        qDebug("ILM_NOTIFICATION_CONTENT_AVAILABLE");
    }
    if (ILM_NOTIFICATION_CONTENT_REMOVED & mask)
    {
        qDebug("ILM_NOTIFICATION_CONTENT_REMOVED");
    }
    if (ILM_NOTIFICATION_CONFIGURED & mask)
    {
        qDebug("ILM_NOTIFICATION_CONFIGURED");
        updateScreen();
    }
}

void WindowManager::surfaceCallbackFunction_static(t_ilm_surface surface,
                                    struct ilmSurfaceProperties* surfaceProperties,
                                    t_ilm_notification_mask mask)

{
    static_cast<WindowManager*>(WindowManager::myThis)->surfaceCallbackFunction_non_static(surface, surfaceProperties, mask);
}
#endif

int WindowManager::layoutId() const
{
    return m_currentLayout;
}

QString WindowManager::layoutName() const
{
    QList<Layout>::const_iterator i = m_layouts.begin();

    QString result = "not found";
    while (i != m_layouts.constEnd())
    {
        if (i->id == m_currentLayout)
        {
            result = i->name;
        }

        ++i;
    }

    return result;
}


int WindowManager::addLayout(int layoutId, const QString &layoutName, const QList<LayoutArea> &surfaceAreas)
{
    qDebug("-=[addLayout]=-");
    m_layouts.append(Layout(layoutId, layoutName, surfaceAreas));

    qDebug("addLayout %d %s, size %d",
           layoutId,
           layoutName.toStdString().c_str(),
           surfaceAreas.size());

    dumpScene();

    return WINDOWMANAGER_NO_ERROR;
}

QList<Layout> WindowManager::getAllLayouts()
{
    qDebug("-=[getAllLayouts]=-");

    return m_layouts;
}

QList<int> WindowManager::getAvailableLayouts(int numberOfAppSurfaces)
{
    qDebug("-=[getAvailableLayouts]=-");
    QList<Layout>::const_iterator i = m_layouts.begin();

    QList<int> result;
    while (i != m_layouts.constEnd())
    {
        if (i->layoutAreas.size() == numberOfAppSurfaces)
        {
            result.append(i->id);
        }

        ++i;
    }

    return result;
}

QList<int> WindowManager::getAvailableSurfaces()
{
    qDebug("-=[getAvailableSurfaces]=-");

    return m_surfaces;
}

QString WindowManager::getLayoutName(int layoutId)
{
    qDebug("-=[getLayoutName]=-");
    QList<Layout>::const_iterator i = m_layouts.begin();

    QString result = "not found";
    while (i != m_layouts.constEnd())
    {
        if (i->id == layoutId)
        {
            result = i->name;
        }

        ++i;
    }

    return result;
}


int WindowManager::setLayoutById(int layoutId)
{
    qDebug("-=[setLayoutById]=-");
    int result = WINDOWMANAGER_NO_ERROR;
    m_currentLayout = layoutId;

    mp_layoutAreaToSurfaceIdAssignment->clear();

    dumpScene();

    return result;
}

int WindowManager::setLayoutByName(const QString &layoutName)
{
    qDebug("-=[setLayoutByName]=-");
    int result = WINDOWMANAGER_NO_ERROR;

    QList<Layout>::const_iterator i = m_layouts.begin();

    while (i != m_layouts.constEnd())
    {
        if (i->name == layoutName)
        {
            m_currentLayout = i->id;

            mp_layoutAreaToSurfaceIdAssignment->clear();

            dumpScene();
        }

        ++i;
    }

    return result;
}

int WindowManager::setSurfaceToLayoutArea(int surfaceId, int layoutAreaId)
{
    qDebug("-=[setSurfaceToLayoutArea]=-");
    int result = WINDOWMANAGER_NO_ERROR;

    qDebug("surfaceId %d", surfaceId);
    qDebug("layoutAreaId %d", layoutAreaId);
    mp_layoutAreaToSurfaceIdAssignment->insert(layoutAreaId, surfaceId);

    updateScreen();

    dumpScene();

    return result;
}
