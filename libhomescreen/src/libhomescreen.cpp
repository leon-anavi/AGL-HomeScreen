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

sRectangle LibHomeScreen::getLayoutRenderAreaForSurfaceId(int surfaceId)
{
    sRectangle result;
    GError *err = NULL;

    GVariant *out_renderArea;

    lib_home_screen_homescreen_call_get_layout_render_area_for_surface_id_sync(
                mp_libHomeScreenHomescreen_Proxy,
                surfaceId,
                &out_renderArea,
                NULL,
                &err);

    if (NULL != err)
    {
        fprintf(stderr, "Unable to call getLayoutRenderAreaForSurfaceId: %s\n", err->message);
    }

    g_variant_get(out_renderArea, "(iiii)", result.x, result.y, result.width, result.height);

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

void LibHomeScreen::renderSurfaceToArea(int surfaceId, const sRectangle &renderArea)
{
    GError *err = NULL;

    GVariant *variant;
    GVariantBuilder *builder;
    builder = g_variant_builder_new(G_VARIANT_TYPE("(iiii)"));
    g_variant_builder_add(builder, "(iiii)", renderArea.x, renderArea.y, renderArea.width, renderArea.height);
    variant = g_variant_new("(iiii)", builder);
    g_variant_builder_unref(builder);

    lib_home_screen_homescreen_call_render_surface_to_area_sync(
                mp_libHomeScreenHomescreen_Proxy,
                surfaceId,
                variant,
                NULL,
                &err);

    if (NULL != err)
    {
        fprintf(stderr, "Unable to call renderSurfaceToArea: %s\n", err->message);
    }
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
