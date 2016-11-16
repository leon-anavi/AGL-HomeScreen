extern "C" {
#include "homescreen.h" // generated from xml
}
#include "include/libhomescreen.hpp"

#include <stdio.h>
#include <stdlib.h>

LibHomeScreen::LibHomeScreen()
{
    GError *err = NULL;

    mp_libHomeScreenHomescreen_Proxy = lib_home_screen_homescreen_proxy_new_for_bus_sync(
                G_BUS_TYPE_SESSION,
                G_DBUS_PROXY_FLAGS_NONE,
                "org.agl.homescreen",
                "/HomeScreen",
                NULL,
                &err);

    if (NULL != err)
    {
        fprintf(stderr, "Unable to create proxy: %s\n", err->message);
    }
}

LibHomeScreen::~LibHomeScreen()
{
    g_object_unref(mp_libHomeScreenHomescreen_Proxy);
}

std::vector<int> LibHomeScreen::getAllSurfacesOfProcess(int pid)
{
    std::vector<int> result;

    GError *err = NULL;

    GVariant *out_surfaceIds;

    lib_home_screen_homescreen_call_get_all_surfaces_of_process_sync(
                mp_libHomeScreenHomescreen_Proxy,
                pid,
                &out_surfaceIds,
                NULL,
                &err);

    if (NULL != err)
    {
        fprintf(stderr, "Unable to call getAllSurfacesOfProcess: %s\n", err->message);
    }


    GVariant *element;
    GVariantIter iter;
    int i;

    if (g_variant_iter_init(&iter, out_surfaceIds))
    {
        while ((element = g_variant_iter_next_value(&iter)) != NULL)
        {
            g_variant_get(element, "i", &i);
            result.push_back(i);
            g_variant_unref(element);
        }
    }

    return result;
}

int LibHomeScreen::getSurfaceStatus(int surfaceId)
{
    int result;
    GError *err = NULL;

    GVariant *out_renderArea;

    lib_home_screen_homescreen_call_get_surface_status_sync(
                mp_libHomeScreenHomescreen_Proxy,
                surfaceId,
                &result,
                NULL,
                &err);

    if (NULL != err)
    {
        fprintf(stderr, "Unable to call getSurfaceStatus: %s\n", err->message);
    }

    return result;
}

void LibHomeScreen::hardKeyPressed(int key)
{
    GError *err = NULL;

    lib_home_screen_homescreen_call_hard_key_pressed_sync(
                mp_libHomeScreenHomescreen_Proxy,
                key,
                NULL,
                &err);

    if (NULL != err)
    {
        fprintf(stderr, "Unable to call hardKeyPressed: %s\n", err->message);
    }
}

void LibHomeScreen::renderSurfaceToArea(int surfaceId, int layoutArea)
{
    GError *err = NULL;

    lib_home_screen_homescreen_call_render_surface_to_area_sync(
                mp_libHomeScreenHomescreen_Proxy,
                surfaceId,
                layoutArea,
                NULL,
                &err);

    if (NULL != err)
    {
        fprintf(stderr, "Unable to call renderSurfaceToArea: %s\n", err->message);
    }
}

bool LibHomeScreen::renderAppToAreaAllowed(int appCategory, int layoutArea)
{
    gboolean result = true;
    GError *err = NULL;

    lib_home_screen_homescreen_call_render_app_to_area_allowed_sync(
                mp_libHomeScreenHomescreen_Proxy,
                appCategory,
                layoutArea,
                &result,
                NULL,
                &err);

    if (NULL != err)
    {
        fprintf(stderr, "Unable to call renderAppToAreaAllowed: %s\n", err->message);
    }

    return result;
}

void LibHomeScreen::requestSurfaceIdToFullScreen(int surfaceId)
{
    GError *err = NULL;

    lib_home_screen_homescreen_call_request_surface_id_to_full_screen_sync(
                mp_libHomeScreenHomescreen_Proxy,
                surfaceId,
                NULL,
                &err);

    if (NULL != err)
    {
        fprintf(stderr, "Unable to call requestSurfaceIdToFullScreen: %s\n", err->message);
    }
}
