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


WindowManager::WindowManager(QObject *parent) :
    QObject(parent)
{
    qDebug("WindowManager");
#ifdef __arm__
    ilmErrorTypes err;
    if (true)//!ilm_isInitialized()) ...crashes...
    {
        err = ilm_init();
        qDebug("ilm_init = %d", err);
    }


    t_ilm_uint screenID = 0;
    t_ilm_uint width;
    t_ilm_uint height;

    err = ilm_getScreenResolution(screenID, &width, &height);
    qDebug("ilm_getScreenResolution = %d", err);
    qDebug("pWidth %d, pHeight %d", width, height);


    t_ilm_layer layerId = 42; // does not matter
    err = ilm_layerCreateWithDimension(&layerId, width, height);
    qDebug("ilm_layerCreateWithDimension = %d", err);
    qDebug("layerId = %d", layerId);

    err = ilm_layerSetVisibility(layerId, true);
    qDebug("ilm_layerSetVisibility = %d", err);

    ilm_commitChanges();

    err = ilm_displaySetRenderOrder(screenID, &layerId, 1);
    qDebug("ilm_displaySetRenderOrder = %d", err);

    ilm_commitChanges();

    err =  ilm_layerSetSourceRectangle(layerId, 0, 0, width, height);
    qDebug("ilm_layerSetSourceRectangle = %d", err);
    err =  ilm_layerSetDestinationRectangle(layerId, 0, 0, width, height);
    qDebug("layerSetDestinationRectangle = %d", err);

    ilm_commitChanges();

    t_ilm_float opacity = 1.0;

    err =  ilm_layerSetOpacity(layerId, opacity);
    qDebug("ilm_layerSetOpacity = %d", err);


    t_ilm_int length;
    t_ilm_surface* pArray;

    err = ilm_getSurfaceIDs(&length, &pArray);
    qDebug("ilm_getSurfaceIDs = %d", err);
    qDebug("length %d pArray[0] %d", length, pArray[0]);

    if (length > 0)
    {
        t_ilm_surface surfaceId = pArray[0];

        err = ilm_layerAddSurface(layerId, surfaceId);
        qDebug("ilm_layerAddSurface = %d", err);

        t_ilm_bool visibility;
        err = ilm_surfaceGetVisibility(surfaceId, &visibility);
        qDebug("ilm_surfaceGetVisibility = %d", err);
        qDebug("visibility %d", visibility);

        err = ilm_surfaceSetVisibility(surfaceId, true);
        qDebug("ilm_surfaceSetVisibility = %d", err);

        err = ilm_surfaceSetOpacity(surfaceId, opacity);
        qDebug("ilm_surfaceSetOpacity = %d", err);

        err = ilm_surfaceSetSourceRectangle(surfaceId, 0, 0, 200, 200);
        qDebug("ilm_surfaceSetSourceRectangle = %d", err);
        err = ilm_surfaceSetDestinationRectangle(surfaceId, 0, 0, 200, 200);
        qDebug("surfaceSetDestinationRectangle = %d", err);


    }

    ilm_commitChanges();

    struct ilmScreenProperties screenProperties;
    struct ilmLayerProperties layerProperties;
    struct ilmSurfaceProperties surfaceProperties;

    err =  ilm_getPropertiesOfScreen(0, &screenProperties);
    qDebug("ilm_getPropertiesOfScreen = %d", err);
    err =  ilm_getPropertiesOfLayer(layerId, &layerProperties);
    qDebug("ilm_getPropertiesOfLayer = %d", err);
    err =  ilm_getPropertiesOfSurface(pArray[0], &surfaceProperties);
    qDebug("ilm_getPropertiesOfSurface = %d", err);


    qDebug("screen");
    qDebug("t_ilm_uint %d", screenProperties.layerCount);          /*!< number of layers displayed on the screen */
    //qDebug("t_ilm_layer* %d", screenProperties.layerIds[0]);          /*!< array of layer ids */
    qDebug("t_ilm_uint %d", screenProperties.harwareLayerCount);   /*!< number of hardware layers */
    qDebug("t_ilm_uint %d", screenProperties.screenWidth);         /*!< width value of screen in pixels */
    qDebug("t_ilm_uint %d", screenProperties.screenHeight);        /*!< height value of screen in pixels */

    qDebug("layer");
    qDebug("t_ilm_float %f", layerProperties.opacity);         /*!< opacity value of the layer */
    qDebug("t_ilm_uint %d", layerProperties.sourceX);          /*!< x source position value of the layer */
    qDebug("t_ilm_uint %d", layerProperties.sourceY);          /*!< y source position value of the layer */
    qDebug("t_ilm_uint %d", layerProperties.sourceWidth);      /*!< source width value of the layer */
    qDebug("t_ilm_uint %d", layerProperties.sourceHeight);     /*!< source height value of the layer */
    qDebug("t_ilm_uint %d", layerProperties.origSourceWidth);  /*!< original source width value of the layer */
    qDebug("t_ilm_uint %d", layerProperties.origSourceHeight); /*!< original source height value of the layer */
    qDebug("t_ilm_uint %d", layerProperties.destX);            /*!< x destination position value of the layer */
    qDebug("t_ilm_uint %d", layerProperties.destY);            /*!< y desitination position value of the layer */
    qDebug("t_ilm_uint %d", layerProperties.destWidth);        /*!< destination width value of the layer */
    qDebug("t_ilm_uint %d", layerProperties.destHeight);       /*!< destination height value of the layer */
    qDebug("ilmOrientation%d",  layerProperties.orientation);  /*!< orientation value of the layer */
    qDebug("t_ilm_bool %d", layerProperties.visibility);       /*!< visibility value of the layer */
    qDebug("t_ilm_uint %d", layerProperties.type);             /*!< type of layer */
    qDebug("t_ilm_int  %d", layerProperties.creatorPid);       /*!< process id of application that created this layer */

    qDebug("surface");
    qDebug("t_ilm_float %f", surfaceProperties.opacity);                    /*!< opacity value of the surface */
    qDebug("t_ilm_uint %d", surfaceProperties.sourceX);                     /*!< x source position value of the surface */
    qDebug("t_ilm_uint %d", surfaceProperties.sourceY);                     /*!< y source position value of the surface */
    qDebug("t_ilm_uint %d", surfaceProperties.sourceWidth);                 /*!< source width value of the surface */
    qDebug("t_ilm_uint %d", surfaceProperties.sourceHeight);                /*!< source height value of the surface */
    qDebug("t_ilm_uint %d", surfaceProperties.origSourceWidth);             /*!< original source width value of the surface */
    qDebug("t_ilm_uint %d", surfaceProperties.origSourceHeight);            /*!< original source height value of the surface */
    qDebug("t_ilm_uint %d", surfaceProperties.destX);                       /*!< x destination position value of the surface */
    qDebug("t_ilm_uint %d", surfaceProperties.destY);                       /*!< y desitination position value of the surface */
    qDebug("t_ilm_uint %d", surfaceProperties.destWidth);                   /*!< destination width value of the surface */
    qDebug("t_ilm_uint %d", surfaceProperties.destHeight);                  /*!< destination height value of the surface */
    qDebug("ilmOrientation %d", surfaceProperties.orientation);             /*!< orientation value of the surface */
    qDebug("t_ilm_bool %d", surfaceProperties.visibility);                  /*!< visibility value of the surface */
    qDebug("t_ilm_uint %d", surfaceProperties.frameCounter);                /*!< already rendered frames of surface */
    qDebug("t_ilm_uint %d", surfaceProperties.drawCounter);                 /*!< content updates of surface */
    qDebug("t_ilm_uint %d", surfaceProperties.updateCounter);               /*!< content updates of surface */
    qDebug("t_ilm_uint %d", surfaceProperties.pixelformat);                 /*!< pixel format of surface */
    qDebug("t_ilm_uint %d", surfaceProperties.nativeSurface);               /*!< native surface handle of surface */
    qDebug("t_ilm_int  %d", surfaceProperties.creatorPid);                  /*!< process id of application that created this surface */
    qDebug("ilmInputDevice %d", surfaceProperties.focus);                   /*!< bitmask of every type of device that this surface has focus in */

    err =  ilm_registerNotification(WindowManager::notificationFunc_static, this);
#endif
}

WindowManager::~WindowManager()
{
#ifdef __arm__
    ilmErrorTypes err;
    if (ilm_isInitialized())
    {
        err = ilm_destroy();
        qDebug("ilm_destroy = %d", err);
    }
#endif
}

#ifdef __arm__
void WindowManager::notificationFunc_non_static(ilmObjectType object,
                                    t_ilm_uint id,
                                    t_ilm_bool created)
{
    qDebug("notificationFunc_non_static");
    if (ILM_SURFACE == object)
    {
        struct ilmSurfaceProperties surfaceProperties;

        if (created)
        {
            qDebug("Surface created, ID: %d", id);

            ilm_layerAddSurface(42 /*always use layer 42 for now*/, id);
            ilm_surfaceSetOpacity(id, 1.0);
            ilm_surfaceSetVisibility(id, true);
            ilm_getPropertiesOfSurface(id, &surfaceProperties);
            ilm_surfaceSetSourceRectangle(id, 0, 0, surfaceProperties.origSourceWidth, surfaceProperties.origSourceHeight);

            ilm_commitChanges();
        }
        else
        {
            qDebug("Surface destroyed, ID: %d", id);
        }

        t_ilm_uint screenID = 0;
        t_ilm_uint width;
        t_ilm_uint height;
        ilm_getScreenResolution(screenID, &width, &height);

        t_ilm_int length;
        t_ilm_surface* pArray;

        ilm_getSurfaceIDs(&length, &pArray);
        qDebug("length %d", length);


        for (int i = 0; i < length; ++i)
        {
            //ilm_getPropertiesOfSurface(pArray[i], &surfaceProperties);
            qDebug("place surface %d at x: %f, y: %f, width: %f, height: %f",
                   pArray[i],
                   i * (width / (1.0 * length)),
                   0,
                   width / (1.0 * length),
                   height);
            ilm_surfaceSetDestinationRectangle(pArray[(int)i],
                    i * (width / (1.0 * length)),
                    0,
                    width / (1.0 * length),
                    height);
        }

        ilm_commitChanges();
    }
    if (ILM_LAYER == object)
    {
        qDebug("Layer.. we don't care...");
    }
}

void WindowManager::notificationFunc_static(ilmObjectType object,
                                            t_ilm_uint id,
                                            t_ilm_bool created,
                                            void* user_data)
{
    qDebug("notificationFunc_static");
    static_cast<WindowManager*>(user_data)->notificationFunc_non_static(object, id, created);
}
#endif
