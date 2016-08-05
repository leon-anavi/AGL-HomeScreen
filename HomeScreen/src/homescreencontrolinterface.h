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

public slots:

//from homescreen_adapter.h
public Q_SLOTS: // METHODS
    void hardKeyPressed(int key);

private:
    HomescreenAdaptor *mp_homeScreenAdaptor;
    org::agl::appframework *mp_dBusAppFrameworkProxy;
};

#endif // HOMESCREENCONTROLINTERFACE_H
