#ifndef HOMESCREENCONTROLINTERFACE_H
#define HOMESCREENCONTROLINTERFACE_H

#include <QObject>
#include "include/homescreen.hpp"
#include "homescreen_adapter.h"
#include <include/appframework.hpp>
#include <appframework_proxy.h>

class HomeScreenControlInterface : public QObject
{
    Q_OBJECT
public:
    explicit HomeScreenControlInterface(QObject *parent = 0);
    ~HomeScreenControlInterface();

signals:
    void newRequestsToBeVisibleApp(int pid);

    QList<int> newRequestGetAllSurfacesOfProcess(int pid);
    int newRequestGetSurfaceStatus(int surfaceId);
    void newRequestRenderSurfaceToArea(int surfaceId, int layoutArea);
    bool newRequestRenderSurfaceToAreaAllowed(int surfaceId, int layoutArea);
    void newRequestSurfaceIdToFullScreen(int surfaceId);

//from homescreen_adapter.h
public Q_SLOTS: // METHODS
    QList<int> getAllSurfacesOfProcess(int pid);
    int getSurfaceStatus(int surfaceId);
    void hardKeyPressed(int key);
    void renderSurfaceToArea(int surfaceId, int layoutArea);
    bool renderSurfaceToAreaAllowed(int surfaceId, int layoutArea);
    void requestSurfaceIdToFullScreen(int surfaceId);
private:
    HomescreenAdaptor *mp_homeScreenAdaptor;
    org::agl::appframework *mp_dBusAppFrameworkProxy;
};

#endif // HOMESCREENCONTROLINTERFACE_H
