AGL Home Screen reference implementation


AGL repo for source code:
https://gerrit.automotivelinux.org/gerrit/#/admin/projects/staging/HomeScreen
AGL repo for bitbake recipe:
https://gerrit.automotivelinux.org/gerrit/#/admin/projects/AGL/meta-agl-demo
[...]/recipes-hmi/homescreen



v0.0.1
06/01/2016

#new features
- initial version
- first implementation of home screen
- fix HMI layout (will be configurable in the future)
- dBus as IPC (may change in the future)
- no interaction with application manager so far
- multi-lanuage support
- day/night mode support
- initial status bar implementation (interface will be enhanced in the future)
- initial control bar implementation
- MPLv2.0 license
- input device simulator for developer build (not functional right now)
- system settings simulator for developer build (day/night mode switch functional right now)
- using QSettings for local app setting storage (may change in the future)
- first popup implementation (single popup support for now; multiple popup not defined; interface will change)
- only one instance of the app allowed

#TODOs
- many settings have to be read from the system somewhere during app startup. This is not available right now
- interaction with application manager
- interaction with wayland compositor
- define and implement input device event handler
- replace my HMI design with the official AGL design (graphics/icons...)
- implement example HMI layouts
- rename dBus objects (internal/external)
- fix font issues on target
- more documentation ;)
- add animations
- much more...

Thanks to Umar Irshad for his free iconset available here:
https://dribbble.com/shots/1569128-Freebie-48-Bubbles-Iconset


