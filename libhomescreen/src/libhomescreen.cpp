#include "homescreen.h" // generated from xml
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

void LibHomeScreen::toggleFullScreen()
{
    GError *err = NULL;

    lib_home_screen_homescreen_call_toggle_full_screen_sync(
                mp_libHomeScreenHomescreen_Proxy,
                NULL,
                &err);

    if (NULL != err)
    {
        fprintf(stderr, "Unable to call toggleFullScreen: %s\n", err->message);
    }
}
