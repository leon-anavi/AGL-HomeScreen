This project contains:

HomeScreen: AGL Home Screen reference implementation
HomeScreenSimulator: AGL Home Screen Simulator for development
InputEventManager: AGL Input event manager
interfaces: library with the D-Bus interfaces
SampleAppTimeDate: AGL Sample Application for Home Screen Statusbar
HomeScreenAppFrameworkBinderTizen: Binder for the TIZEN application framework

AGL repo for source code:
https://gerrit.automotivelinux.org/gerrit/#/admin/projects/staging/HomeScreen

AGL repo for bitbake recipe:
https://gerrit.automotivelinux.org/gerrit/#/admin/projects/AGL/meta-agl-demo/recipes-demo-hmi/HomeScreen/HomeScreen_?.bb


Instructions for running on porter board
----------------------------------------

Add "HomeScreen" to your image recipe.
Right now, HomeScreen depends on the TIZEN application manager, because the HomeScreenAppFrameworkBinderTizen makes use of it.
So also add this to your image recipe:
"
    tizen-platform-wrapper \
    tizen-platform-config \
    xdgmime \
    libdlog \
    dlogutil \
    libiri \
    \
    smack \
    libprivilege-control \
    libslp-db-util \
    \
    vconf \
    capi-base-common \
    ail \
    \
    sensor \
    libsf-common \
    \
    iniparser \
    app-svc \
    heynoti \
    notification \
    app-core-efl \
    app-core-common \
    capi-system-info \
    \
    pkgmgr-info \
    librua \
    bundle \
    app-checker \
    \
    libcom-core \
    privacy-manager-server \
    pkgmgr \
    pkgmgr-info \
    pkgmgr-info-parser \
    aul \
    aul-test \
    \
    dlt-daemon \
    dlt-daemon-systemd \
    \
    amhelloworld \
    pkgmgr-first-setup-service \
"


All "HomeScreen" applications can be found in /opt/AGL/HomeScreen.

Make sure, weston is using the IVI shell extension:

/etc/xdg/weston/weston.ini:

[core]
shell=ivi-shell.so

[ivi-shell]
ivi-module=ivi-controller.so
ivi-shell-user-interface=/usr/lib/weston/weston-ivi-shell-user-interface

[output]
name=HDMI-A-1
transform=270



If you want to use the TIZEN application manager to launch apps, you need to execute HomeScreen app with the user "aglglobalapp":

Therefore you need to change the owner of the weston socket:

chown aglglobalapp:aglglobalapp /tmp/wayland-0

P.S.: the path may also something likle /run/user/0, depending on the XDG_RUNTIME_DIR.

If not already stared, launch D-Bus:
export `dbus-launch`

Now you can start the HomeScreen apps:
cd /opt/AGL/HomeScreen
./WindowManager &
./InputEventManager &
./HomeScreenAppFrameworkBinderTizen &
./HomeScreen &


