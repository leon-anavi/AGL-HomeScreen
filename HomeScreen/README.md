AGL Home Screen reference implementation


AGL repo for source code:
https://gerrit.automotivelinux.org/gerrit/#/admin/projects/staging/HomeScreen


v0.1.0
06/24/2016

#changes
- reworked status bar
- reviewed D-Bus interfaces, now using signals instead of methods for day/night mode
- created new home screen simulator app
- license changed to Apache 2.0
- put D-Bus introspections in one central place
- disabled "only one instance allowed" for development

v0.0.2
06/13/2016

#changes
- created layout 1. Changed home screen app size to 1080x1920

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
- define and implement control bar D-Bus-interface
- many settings have to be read from the system somewhere during app startup. This is not available right now
- interaction with application manager
- interaction with wayland compositor
- define and implement input device event handler
- replace my HMI design with the official AGL design (graphics/icons...)
- implement example HMI layouts
- fix font issues on target
- more documentation ;)
- add animations
- much more...

Thanks to Umar Irshad for his free iconset available here:
https://dribbble.com/shots/1569128-Freebie-48-Bubbles-Iconset


