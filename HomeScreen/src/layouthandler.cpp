#include "layouthandler.h"

LayoutHandler::LayoutHandler(QObject *parent) :
    QObject(parent),
    mp_dBusWindowManagerProxy(0),
    mp_dBusPopupProxy(0),
    m_visibleApps(),
    m_invisibleApps(),
    m_requestsToBeVisibleApps()
{
    qDBusRegisterMetaType<SimplePoint>();
    qDBusRegisterMetaType<QList<SimplePoint> >();
    qDBusRegisterMetaType<SimpleRect>();
    qDBusRegisterMetaType<QList<SimpleRect> >();

    qDebug("D-Bus: connect to org.agl.windowmanager /windowmanager");
    mp_dBusWindowManagerProxy = new org::agl::windowmanager("org.agl.windowmanager",
                                              "/windowmanager",
                                              QDBusConnection::sessionBus(),
                                              0);
    qDebug("D-Bus: connect to org.agl.homescreen /Popup");
    mp_dBusPopupProxy = new org::agl::popup("org.agl.homescreen",
                                              "/Popup",
                                              QDBusConnection::sessionBus(),
                                              0);
}

LayoutHandler::~LayoutHandler()
{
    delete mp_dBusPopupProxy;
    delete mp_dBusWindowManagerProxy;
}

void LayoutHandler::setUpLayouts()
{
    qDebug("setUpLayouts");
    QList<SimpleRect> surfaceAreas;
    SimpleRect surfaceArea;

    const int SCREEN_WIDTH = 1080;
    const int SCREEN_HEIGHT = 1920;

    const int STATUSBAR_HEIGHT = 60;
    const int STATUSBAR_WIDTH = SCREEN_WIDTH;
    const int STATUSBAR_X = 0;
    const int STATUSBAR_Y = 0;
    const int CONTROLBAR_HEIGHT = 60;
    const int CONTROLBAR_WIDTH = SCREEN_WIDTH;
    const int CONTROLBAR_X = 0;
    const int CONTROLBAR_Y = SCREEN_HEIGHT - CONTROLBAR_HEIGHT;


    // layout 1:
    // one app surface, statusbar, control bar
    surfaceArea.x = 0;
    surfaceArea.y = STATUSBAR_HEIGHT;
    surfaceArea.width = SCREEN_WIDTH;
    surfaceArea.height = SCREEN_HEIGHT - STATUSBAR_HEIGHT - CONTROLBAR_HEIGHT;

    surfaceAreas.append(surfaceArea);

    mp_dBusWindowManagerProxy->addLayout(1, "one app", surfaceAreas);


    surfaceAreas.clear();

    // layout 2:
    // two app surfaces (one on top of the other), statusbar, control bar

    // top surface
    surfaceArea.x = 0;
    surfaceArea.y = STATUSBAR_HEIGHT;
    surfaceArea.width = SCREEN_WIDTH;
    surfaceArea.height = (SCREEN_HEIGHT - STATUSBAR_HEIGHT - CONTROLBAR_HEIGHT) / 2;

    surfaceAreas.append(surfaceArea);

    // bottom surface
    surfaceArea.x = 0;
    surfaceArea.y = SCREEN_HEIGHT / 2;
    surfaceArea.width = SCREEN_WIDTH;
    surfaceArea.height = (SCREEN_HEIGHT - STATUSBAR_HEIGHT - CONTROLBAR_HEIGHT) / 2;

    surfaceAreas.append(surfaceArea);

    mp_dBusWindowManagerProxy->addLayout(2, "top on bottom", surfaceAreas);


    surfaceAreas.clear();

    // layout 3:
    // two app surfaces (one besides the other), statusbar, control bar

    // left surface
    surfaceArea.x = 0;
    surfaceArea.y = STATUSBAR_HEIGHT;
    surfaceArea.width = SCREEN_WIDTH / 2;
    surfaceArea.height = SCREEN_HEIGHT - STATUSBAR_HEIGHT - CONTROLBAR_HEIGHT;

    surfaceAreas.append(surfaceArea);

    // right surface
    surfaceArea.x = SCREEN_WIDTH / 2;
    surfaceArea.y = STATUSBAR_HEIGHT;
    surfaceArea.width = SCREEN_WIDTH / 2;
    surfaceArea.height = SCREEN_HEIGHT - STATUSBAR_HEIGHT - CONTROLBAR_HEIGHT;

    surfaceAreas.append(surfaceArea);

    mp_dBusWindowManagerProxy->addLayout(3, "side by side", surfaceAreas);

}

void LayoutHandler::makeMeVisible(int pid)
{
    qDebug("makeMeVisible %d", pid);
    m_requestsToBeVisibleApps.append(pid);

    qDebug("m_visibleApps %d", m_visibleApps.size());
    qDebug("m_invisibleApps %d", m_invisibleApps.size());
    qDebug("m_requestsToBeVisibleApps %d", m_requestsToBeVisibleApps.size());

    QList<int> availableLayouts = mp_dBusWindowManagerProxy->getAvailableLayouts(m_visibleApps.size() + m_requestsToBeVisibleApps.size());
    if (0 == availableLayouts.size())
    {
        // no layout fits the need!
        // replace the last app
        qDebug("no layout fits the need!");
        qDebug("replace the last app");

        m_invisibleApps.append(m_visibleApps.last());
        m_visibleApps.removeLast();

        m_visibleApps.append(m_requestsToBeVisibleApps);
        m_requestsToBeVisibleApps.clear();

        for (int i = 0; i < m_visibleApps.size(); ++i)
        {
            mp_dBusWindowManagerProxy->setPidToLayoutArea(i, i);
        }
    }
    if (1 == availableLayouts.size())
    {
        // switch to new layout
        qDebug("switch to new layout %d", availableLayouts.at(0));
        m_visibleApps.append(m_requestsToBeVisibleApps);
        m_requestsToBeVisibleApps.clear();

        mp_dBusWindowManagerProxy->setLayoutById(availableLayouts.at(0));
        for (int i = 0; i < m_visibleApps.size(); ++i)
        {
            mp_dBusWindowManagerProxy->setPidToLayoutArea(i, i);
        }
    }
    if (1 < availableLayouts.size())
    {
        // more than one layout possible! Ask user.
        qDebug("more than one layout possible! Ask user.");

        QStringList choices;
        for (int i = 0; i < availableLayouts.size(); ++i)
        {
            choices.append(mp_dBusWindowManagerProxy->getLayoutName(availableLayouts.at(i)));
        }

        mp_dBusPopupProxy->showPopupComboBox("Select Layout", choices);

    }
}

void LayoutHandler::setLayoutByName(QString layoutName)
{
    // switch to new layout
    qDebug("setLayout: switch to new layout %s", layoutName.toStdString().c_str());
    m_visibleApps.append(m_requestsToBeVisibleApps);
    m_requestsToBeVisibleApps.clear();

    mp_dBusWindowManagerProxy->setLayoutByName(layoutName);
    for (int i = 0; i < m_visibleApps.size(); ++i)
    {
        mp_dBusWindowManagerProxy->setPidToLayoutArea(i, i);
    }
}
