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


//////////////////////////////////////////
// THIS IS STILL UNDER HEAVY DEVELOPMENT!
// DO NOT JUDGE THE SOURCE CODE :)
//////////////////////////////////////////

// three layers will be defined. The HomeScreen will be placed
// full screen in the background.
// On top all applications in one layer.
// On top of that, the popup layer.
#define WINDOWMANAGER_LAYER_POPUP 100
#define WINDOWMANAGER_LAYER_HOMESCREEN_OVERLAY 101
#define WINDOWMANAGER_LAYER_APPLICATIONS 102
#define WINDOWMANAGER_LAYER_HOMESCREEN 103

#define WINDOWMANAGER_LAYER_NUM 4

#define WINDOWMANAGER_LAYER_ID_SHIFT 22

// the HomeScreen app has to have the surface id 1000
#define WINDOWMANAGER_HOMESCREEN_MAIN_SURFACE_ID 1000

// Quick hack for scaling layer to fit non-FHD(1920x1080) screen
//  * source rect of layer should be 1920x1080
//  * destination rect of layer should fit physical display resolution
//  * source rect of surface shoud be based on 1920x1080
//  * destination rect of surface should be based on 1920x1080
#define WINDOWMANAGER_HOMESCREEN_WIDTH  1080
#define WINDOWMANAGER_HOMESCREEN_HEIGHT 1920

void* WindowManager::myThis = 0;

WindowManager::WindowManager(int displayId, QObject *parent) :
    QObject(parent),
    m_layouts(),
    //    m_appSurfaces(),
    mp_layoutAreaToSurfaceIdAssignment(0),
    m_currentLayout(-1),
    m_screenId(displayId),
    m_screenWidth(0),
    m_screenHeight(0)
#ifdef HAVE_IVI_LAYERMANAGEMENT_API
  ,
    m_appLayers(),
    m_pending_to_show(-1)
#endif
{
#ifdef HAVE_IVI_LAYERMANAGEMENT_API
    m_showLayers = new t_ilm_layer[WINDOWMANAGER_LAYER_NUM];

    m_showLayers[0] = 0; /* POPUP is not shown by default */
    m_showLayers[1] = 0; /* HOMESCREEN_OVERLAY is not shown by default */
    m_showLayers[2] = 0; /* APPLICATIONS is not shown by default */
    m_showLayers[3] = WINDOWMANAGER_LAYER_HOMESCREEN; /* HOMESCREEN is shwon by default */

#endif
    qDebug("-=[WindowManager]=-");
}

void WindowManager::start()
{
    qDebug("-=[start]=-");
    mp_layoutAreaToSurfaceIdAssignment = new QMap<int, unsigned int>;
#ifdef HAVE_IVI_LAYERMANAGEMENT_API
    ilmErrorTypes err;

    err = ilm_init();
    qDebug("ilm_init = %d", err);
    if(ILM_SUCCESS != err)
    {
        qDebug("failed! Exiting!");
        exit(-1);
    }

    myThis = this;

    ilm_getScreenResolution(m_screenId, &m_screenWidth, &m_screenHeight);

    createNewLayer(WINDOWMANAGER_LAYER_POPUP);
    createNewLayer(WINDOWMANAGER_LAYER_HOMESCREEN_OVERLAY);
//  createNewLayer(WINDOWMANAGER_LAYER_APPLICATIONS);
    createNewLayer(WINDOWMANAGER_LAYER_HOMESCREEN);

    ilm_registerNotification(WindowManager::notificationFunc_static, this);
#endif

    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/windowmanager", this);
    dbus.registerService("org.agl.windowmanager");

    // publish windowmanager interface
    mp_windowManagerAdaptor = new WindowmanagerAdaptor((QObject*)this);
}

WindowManager::~WindowManager()
{
    qDebug("-=[~WindowManager]=-");
    delete mp_windowManagerAdaptor;
#ifdef HAVE_IVI_LAYERMANAGEMENT_API
    ilm_destroy();
#endif
    delete mp_layoutAreaToSurfaceIdAssignment;
}

#ifdef HAVE_IVI_LAYERMANAGEMENT_API
int WindowManager::getLayerRenderOrder(t_ilm_layer id_array[])
{
    int i, j;

    for (i = WINDOWMANAGER_LAYER_NUM - 1, j = 0; i >= 0; i--, j++) {
        if (m_showLayers[i] != 0) {
            id_array[j] = m_showLayers[i];
        }
    }

    return j;
}
#endif

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

#ifdef HAVE_IVI_LAYERMANAGEMENT_API

void WindowManager::createNewLayer(int layerId)
{
    qDebug("-=[createNewLayer]=-");
    qDebug("  layerId %d", layerId);

    t_ilm_layer newLayerId = layerId;
    ilm_layerCreateWithDimension(&newLayerId,
                                    WINDOWMANAGER_HOMESCREEN_WIDTH,
                                    WINDOWMANAGER_HOMESCREEN_HEIGHT);
    ilm_layerSetOpacity(newLayerId, 1.0);
    ilm_layerSetVisibility(newLayerId, ILM_TRUE);
    ilm_layerSetSourceRectangle(newLayerId,
                                    0,
                                    0,
                                    WINDOWMANAGER_HOMESCREEN_WIDTH,
                                    WINDOWMANAGER_HOMESCREEN_HEIGHT);
    ilm_layerSetDestinationRectangle(newLayerId,
                                    0,
                                    0,
                                    m_screenWidth,
                                    m_screenHeight);

    ilm_commitChanges();
}

t_ilm_layer WindowManager::getAppLayerID(pid_t pid)
{
    t_ilm_layer layer_id;

//    layer_id = pid + (WINDOWMANAGER_LAYER_APPLICATIONS << WINDOWMANAGER_LAYER_ID_SHIFT);
    layer_id = pid + (WINDOWMANAGER_LAYER_APPLICATIONS * 100000); /* for debug */

    return layer_id;
}

void WindowManager::addSurfaceToAppLayer(int surfaceId)
{
    struct ilmSurfaceProperties surfaceProperties;
    t_ilm_layer layer_id;
    int found = 0;
    pid_t pid;

    qDebug("-=[addSurfaceToAppLayer]=-");
    qDebug("  surfaceId %d", surfaceId);

    ilm_getPropertiesOfSurface(surfaceId, &surfaceProperties);
    pid = surfaceProperties.creatorPid;

    if (pid < 0) {
        /* No process */
        qDebug("addSurfaceToAppLayer(%d) got pid == -1", surfaceId);
        return;
    }

    QMap<pid_t, t_ilm_layer>::const_iterator i = m_appLayers.find(pid);
    if (i == m_appLayers.end()) {
        qDebug("No layer found, create new for app(pid=%d)", pid);

        /* not found, create new one */
        t_ilm_layer layer_id = getAppLayerID(pid);

        createNewLayer(layer_id);
        m_appLayers.insert(pid, layer_id);
    }
}

void WindowManager::addSurfaceToLayer(int surfaceId, int layerId)
{
    qDebug("-=[addSurfaceToLayer]=-");
    qDebug("  surfaceId %d", surfaceId);
    qDebug("  layerId %d", layerId);

    if (layerId == WINDOWMANAGER_LAYER_HOMESCREEN)
    {
        struct ilmSurfaceProperties surfaceProperties;
        ilm_getPropertiesOfSurface(surfaceId, &surfaceProperties);

        // homescreen app always fullscreen in the back
        ilm_surfaceSetDestinationRectangle(surfaceId, 0, 0,
                                           WINDOWMANAGER_HOMESCREEN_WIDTH,
                                           WINDOWMANAGER_HOMESCREEN_HEIGHT);
        //ilm_surfaceSetSourceRectangle(surfaceId, 0, 0, m_screenWidth, m_screenHeight);
        ilm_surfaceSetOpacity(surfaceId, 1.0);
        ilm_surfaceSetVisibility(surfaceId, ILM_TRUE);

        ilm_layerAddSurface(layerId, surfaceId);
    }
#if 0
    if (layerId == WINDOWMANAGER_LAYER_APPLICATIONS)
    {
        struct ilmSurfaceProperties surfaceProperties;
        ilm_getPropertiesOfSurface(surfaceId, &surfaceProperties);

        //ilm_surfaceSetDestinationRectangle(surfaceId, 0, 0, surfaceProperties.origSourceWidth, surfaceProperties.origSourceHeight);
        //ilm_surfaceSetSourceRectangle(surfaceId, 0, 0, surfaceProperties.origSourceWidth, surfaceProperties.origSourceHeight);
        //ilm_surfaceSetOpacity(surfaceId, 0.0);
        //ilm_surfaceSetVisibility(surfaceId, ILM_FALSE);

        ilm_layerAddSurface(layerId, surfaceId);
    }
#endif
    if (layerId == WINDOWMANAGER_LAYER_HOMESCREEN_OVERLAY)
    {
        struct ilmSurfaceProperties surfaceProperties;
        ilm_getPropertiesOfSurface(surfaceId, &surfaceProperties);

        //ilm_surfaceSetDestinationRectangle(surfaceId, 0, 0, surfaceProperties.origSourceWidth, surfaceProperties.origSourceHeight);
        //ilm_surfaceSetSourceRectangle(surfaceId, 0, 0, surfaceProperties.origSourceWidth, surfaceProperties.origSourceHeight);
        //ilm_surfaceSetOpacity(surfaceId, 0.5);
        //ilm_surfaceSetVisibility(surfaceId, ILM_TRUE);

        ilm_layerAddSurface(layerId, surfaceId);
    }

    if (layerId == WINDOWMANAGER_LAYER_POPUP)
    {
        struct ilmSurfaceProperties surfaceProperties;
        ilm_getPropertiesOfSurface(surfaceId, &surfaceProperties);

        //ilm_surfaceSetDestinationRectangle(surfaceId, 0, 0, surfaceProperties.origSourceWidth, surfaceProperties.origSourceHeight);
        //ilm_surfaceSetSourceRectangle(surfaceId, 0, 0, surfaceProperties.origSourceWidth, surfaceProperties.origSourceHeight);
        //ilm_surfaceSetOpacity(surfaceId, 0.0);
        //ilm_surfaceSetVisibility(surfaceId, ILM_FALSE);

        ilm_layerAddSurface(layerId, surfaceId);
    }

    ilm_commitChanges();
}

#endif

void WindowManager::updateScreen()
{
    qDebug("-=[updateScreen]=-");

#if 0
//#ifdef HAVE_IVI_LAYERMANAGEMENT_API
    if (-1 != m_currentLayout)
    {
        // hide all surfaces
        for (int i = 0; i < m_appSurfaces.size(); ++i)
        {
            ilm_layerRemoveSurface(WINDOWMANAGER_LAYER_APPLICATIONS, m_appSurfaces.at(i));
            //ilm_surfaceSetVisibility(m_appSurfaces.at(i), ILM_FALSE);
            //ilm_surfaceSetOpacity(m_appSurfaces.at(i), 0.0);
            ilm_commitChanges();
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

            addSurfaceToLayer(surfaceToShow, WINDOWMANAGER_LAYER_APPLICATIONS);

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
            ilm_commitChanges();
        }
    }

    // layer surface render order
    t_ilm_int length;
    t_ilm_surface* pArray;
    ilm_getSurfaceIDsOnLayer(WINDOWMANAGER_LAYER_HOMESCREEN, &length, &pArray);
    ilm_layerSetRenderOrder(WINDOWMANAGER_LAYER_HOMESCREEN, pArray, length);
    ilm_commitChanges();
    ilm_getSurfaceIDsOnLayer(WINDOWMANAGER_LAYER_APPLICATIONS, &length, &pArray);
    ilm_layerSetRenderOrder(WINDOWMANAGER_LAYER_APPLICATIONS, pArray, length);
    ilm_commitChanges();
    ilm_getSurfaceIDsOnLayer(WINDOWMANAGER_LAYER_HOMESCREEN_OVERLAY, &length, &pArray);
    ilm_layerSetRenderOrder(WINDOWMANAGER_LAYER_HOMESCREEN_OVERLAY, pArray, length);
    ilm_commitChanges();
    ilm_getSurfaceIDsOnLayer(WINDOWMANAGER_LAYER_POPUP, &length, &pArray);
    ilm_layerSetRenderOrder(WINDOWMANAGER_LAYER_POPUP, pArray, length);
    ilm_commitChanges();
#endif
#ifdef HAVE_IVI_LAYERMANAGEMENT_API
    if (m_pending_to_show != -1) {
        showAppLayer(m_pending_to_show);
    } else {
        // display layer render order
        t_ilm_layer renderOrder[WINDOWMANAGER_LAYER_NUM];
        int num_layers = getLayerRenderOrder(renderOrder);
        ilm_displaySetRenderOrder(m_screenId, renderOrder, num_layers);
        ilm_commitChanges();
    }
#endif
}

#ifdef HAVE_IVI_LAYERMANAGEMENT_API
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
                addSurfaceToAppLayer(id);
                //addSurfaceToLayer(id, WINDOWMANAGER_LAYER_APPLICATIONS);
                //m_appSurfaces.append(id);
            }
            ilm_surfaceAddNotification(id, surfaceCallbackFunction_static);

            ilm_commitChanges();
        }
        else
        {
            qDebug("Surface destroyed, ID: %d", id);
#if 0
            m_appSurfaces.removeAt(m_appSurfaces.indexOf(id));
            ilm_surfaceRemoveNotification(id);

            ilm_commitChanges();
#endif
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
        surfaceVisibilityChanged(surface, surfaceProperties->visibility);
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
        /* add surface to layer for the application */

        ilmErrorTypes result;
        pid_t pid = surfaceProperties->creatorPid;

        QMap<pid_t, t_ilm_layer>::const_iterator i = m_appLayers.find(pid);
        if (i != m_appLayers.end()) {
            t_ilm_layer layer_id = m_appLayers.value(pid);

            result = ilm_layerAddSurface(layer_id, surface);

            if (result != ILM_SUCCESS) {
                qDebug("ilm_layerAddSurface(%d,%d) failed.", layer_id, surface);
            }

            /* Dirty hack! cut & paste from HomeScreen/src/layouthandler.cpp */
            const int SCREEN_WIDTH = 1080;
            const int SCREEN_HEIGHT = 1920;

            const int TOPAREA_HEIGHT = 218;
            const int TOPAREA_WIDTH = SCREEN_WIDTH;
            const int TOPAREA_X = 0;
            const int TOPAREA_Y = 0;
            const int MEDIAAREA_HEIGHT = 215;
            const int MEDIAAREA_WIDTH = SCREEN_WIDTH;
            const int MEDIAAREA_X = 0;
            const int MEDIAAREA_Y = SCREEN_HEIGHT - MEDIAAREA_HEIGHT;

            ilm_surfaceSetDestinationRectangle(surface,
                                               0,
                                               TOPAREA_HEIGHT,
                                               SCREEN_WIDTH,
                                               SCREEN_HEIGHT - TOPAREA_HEIGHT - MEDIAAREA_HEIGHT);

            ilm_commitChanges();
        } else {
            qDebug("No layer for application(pid=%d)", surfaceProperties->creatorPid);
        }
    }
    if (ILM_NOTIFICATION_CONTENT_REMOVED & mask)
    {
        qDebug("ILM_NOTIFICATION_CONTENT_REMOVED");

        /* application being down */
        m_appLayers.remove(surfaceProperties->creatorPid);

        updateScreen();
    }
    if (ILM_NOTIFICATION_CONFIGURED & mask)
    {
        qDebug("ILM_NOTIFICATION_CONFIGURED");
        qDebug("  surfaceProperties %d", surface);
        qDebug("    surfaceProperties.origSourceWidth: %d", surfaceProperties->origSourceWidth);
        qDebug("    surfaceProperties.origSourceHeight: %d", surfaceProperties->origSourceHeight);

        ilm_surfaceSetSourceRectangle(surface,
                                      0,
                                      0,
                                      surfaceProperties->origSourceWidth,
                                      surfaceProperties->origSourceHeight);

        ilm_surfaceSetVisibility(surface, ILM_TRUE);

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

int WindowManager::deleteLayoutById(int layoutId)
{
    qDebug("-=[deleteLayoutById]=-");
    qDebug("layoutId: %d", layoutId);
    int result = WINDOWMANAGER_NO_ERROR;

    if (m_currentLayout == layoutId)
    {
        result = WINDOWMANAGER_ERROR_ID_IN_USE;
    }
    else
    {
        QList<Layout>::iterator i = m_layouts.begin();
        result = WINDOWMANAGER_ERROR_ID_IN_USE;
        while (i != m_layouts.constEnd())
        {
            if (i->id == layoutId)
            {
                m_layouts.erase(i);
                result = WINDOWMANAGER_NO_ERROR;
                break;
            }

            ++i;
        }
    }

    return result;
}


QList<Layout> WindowManager::getAllLayouts()
{
    qDebug("-=[getAllLayouts]=-");

    return m_layouts;
}

#if 0
QList<int> WindowManager::getAllSurfacesOfProcess(int pid)
{
    QList<int> result;
#ifdef HAVE_IVI_LAYERMANAGEMENT_API
    struct ilmSurfaceProperties surfaceProperties;

    for (int i = 0; i < m_appSurfaces.size(); ++i)
    {
        ilm_getPropertiesOfSurface(m_appSurfaces.at(i), &surfaceProperties);
        if (pid == surfaceProperties.creatorPid)
        {
            result.append(m_appSurfaces.at(i));
        }
    }
#endif
    return result;
}
#endif

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

#if 0
QList<int> WindowManager::getAvailableSurfaces()
{
    qDebug("-=[getAvailableSurfaces]=-");

    return m_appSurfaces;
}
#endif

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

void WindowManager::hideLayer(int layer)
{
    qDebug("-=[hideLayer]=-");
    qDebug("layer %d", layer);

#ifdef HAVE_IVI_LAYERMANAGEMENT_API
    // POPUP=0, HOMESCREEN_OVERLAY=1, APPS=2, HOMESCREEN=3
    if (layer >= 0 && layer < WINDOWMANAGER_LAYER_NUM) {
        /* hide target layer */
        m_showLayers[layer] = 0;

        if (layer == WINDOWMANAGER_LAYER_APPLICATIONS) {
            /* clear pending flag */
            m_pending_to_show = -1;
        } else if (m_pending_to_show != -1) {
            /* there is a pending application to show */
            showAppLayer(m_pending_to_show);
            return;
        }

        t_ilm_layer renderOrder[WINDOWMANAGER_LAYER_NUM];
        int num_layers = getLayerRenderOrder(renderOrder);
        ilm_displaySetRenderOrder(m_screenId, renderOrder, num_layers);
        ilm_commitChanges();
    }
#endif
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

void WindowManager::showLayer(int layer)
{
    qDebug("-=[showLayer]=-");
    qDebug("layer %d", layer);

#ifdef HAVE_IVI_LAYERMANAGEMENT_API
    // POPUP=0, HOMESCREEN_OVERLAY=1, APPS=2, HOMESCREEN=3
    if (layer >= 0 && layer < WINDOWMANAGER_LAYER_NUM) {
        static const int layer_id_array[] = {
            WINDOWMANAGER_LAYER_POPUP,
            WINDOWMANAGER_LAYER_HOMESCREEN_OVERLAY,
            WINDOWMANAGER_LAYER_APPLICATIONS,
            WINDOWMANAGER_LAYER_HOMESCREEN,
        };

        m_showLayers[layer] = layer_id_array[layer];

        t_ilm_layer renderOrder[WINDOWMANAGER_LAYER_NUM];
        int num_layers = getLayerRenderOrder(renderOrder);
        ilm_displaySetRenderOrder(m_screenId, renderOrder, num_layers);
        ilm_commitChanges();
    }
#endif
}

void WindowManager::showAppLayer(int pid)
{
    qDebug("-=[showAppLayer]=-");
    qDebug("pid %d", pid);

    if (pid == -1) {
        /* nothing to show */
        return;
    }
#ifdef HAVE_IVI_LAYERMANAGEMENT_API

    /* clear pending flag */
    m_pending_to_show = -1;

    /* search layer id for application to show */
    QMap<pid_t, t_ilm_layer>::const_iterator i = m_appLayers.find(pid);

    if (i != m_appLayers.end()) {
        m_showLayers[2] = m_appLayers.value(pid);
        qDebug("Found layer(%d) to show for app(pid=%d)", m_showLayers[2], pid);
    } else {
        /* Probably app layer hasn't been made yet */
        m_pending_to_show = pid;
        /* hide current app once, back to default screen */
        m_showLayers[2] = 0;

        qDebug("No layer to show for app(pid=%d)", pid);
    }
    t_ilm_layer renderOrder[WINDOWMANAGER_LAYER_NUM];

    int num_layers = getLayerRenderOrder(renderOrder);
    ilm_displaySetRenderOrder(m_screenId, renderOrder, num_layers);
    ilm_commitChanges();
#endif
}
