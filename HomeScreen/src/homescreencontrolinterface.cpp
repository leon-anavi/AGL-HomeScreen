#include "homescreencontrolinterface.h"

HomeScreenControlInterface::HomeScreenControlInterface(QObject *parent) :
    QObject(parent),
    mp_homeScreenAdaptor(0),
    mp_dBusAppFrameworkProxy()
{
    // publish dbus homescreen interface
    mp_homeScreenAdaptor = new HomescreenAdaptor((QObject*)this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/HomeScreen", this);
    dbus.registerService("org.agl.homescreen");

    qDebug("D-Bus: connect to org.agl.homescreenappframeworkbindertizen /AppFramework");
    mp_dBusAppFrameworkProxy = new org::agl::appframework("org.agl.homescreenappframeworkbindertizen",
                                              "/AppFramework",
                                              QDBusConnection::sessionBus(),
                                              0);
}

HomeScreenControlInterface::~HomeScreenControlInterface()
{
    delete mp_dBusAppFrameworkProxy;
    delete mp_homeScreenAdaptor;
}

void HomeScreenControlInterface::hardKeyPressed(int key)
{
    int pid = -1;

    switch (key)
    {
    case InputEvent::HARDKEY_NAV:
        qDebug("hardKeyPressed NAV key pressed!");
        pid = mp_dBusAppFrameworkProxy->launchApp("demoapp");
        qDebug("pid: %d", pid);
        break;
    default:
        qDebug("hardKeyPressed %d", key);
        break;
    }
}
