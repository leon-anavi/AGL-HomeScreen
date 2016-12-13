#include "layouthandler.h"
#include <QTimerEvent>

LayoutHandler::LayoutHandler(QObject *parent) :
    QObject(parent),
    m_secondsTimerId(-1),
    mp_dBusWindowManagerProxy(0),
    mp_dBusPopupProxy(0),
    m_visibleSurfaces(),
    m_invisibleSurfaces(),
    m_requestsToBeVisibleSurfaces()
{
    qDBusRegisterMetaType<SimplePoint>();
    qDBusRegisterMetaType<QList<SimplePoint> >();
    qDBusRegisterMetaType<LayoutArea>();
    qDBusRegisterMetaType<QList<LayoutArea> >();
    qDBusRegisterMetaType<Layout>();
    qDBusRegisterMetaType<QList<Layout> >();

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

    QDBusConnection::sessionBus().connect("org.agl.windowmanager",
                                       "/windowmanager",
                                       "org.agl.windowmanager",
                                       "surfaceVisibilityChanged",
                                       this,
                                       SIGNAL(surfaceVisibilityChanged(int,bool)));

    QList<LayoutArea> surfaceAreas;
    LayoutArea surfaceArea;

    const int SCREEN_WIDTH = 1080;
    const int SCREEN_HEIGHT = 1920;

    const int TOPAREA_HEIGHT = 218;
    const int TOPAREA_WIDTH = SCREEN_WIDTH;
    const int TOPAREA_X = 0;
    const int TOPAREA_Y = 0;
    const int MEDIAAREA_HEIGHT = 215;
    const int MEDIAAREA_WIDTH = SCREEN_WIDTH;
    const int MEDIAAREA_X = 0;
    const int MEDIAAREA_Y = SCREEN_HEIGHT - MEDIAAREA_HEIGHT;


    // only one Layout for CES2017 needed
    // layout 1:
    // one app surface, statusbar, control bar
    surfaceArea.x = 0;
    surfaceArea.y = TOPAREA_HEIGHT;
    surfaceArea.width = SCREEN_WIDTH;
    surfaceArea.height = SCREEN_HEIGHT - TOPAREA_HEIGHT - MEDIAAREA_HEIGHT;

    surfaceAreas.append(surfaceArea);

    mp_dBusWindowManagerProxy->addLayout(1, "one app", surfaceAreas);
}

LayoutHandler::~LayoutHandler()
{
    delete mp_dBusPopupProxy;
    delete mp_dBusWindowManagerProxy;
}

void LayoutHandler::showAppLayer(int pid)
{
    mp_dBusWindowManagerProxy->showAppLayer(pid);
}

void LayoutHandler::hideAppLayer()
{
    // POPUP=0, HOMESCREEN_OVERLAY=1, APPS=2, HOMESCREEN=3
    mp_dBusWindowManagerProxy->hideLayer(2); // TODO: enum
}

void LayoutHandler::makeMeVisible(int pid)
{
    qDebug("makeMeVisible %d", pid);

#if 0
    // if app does not request to be visible
    if (-1 == m_requestsToBeVisiblePids.indexOf(pid))
    {
        m_requestsToBeVisiblePids.append(pid);

        // callback every second
        if (-1 != m_secondsTimerId)
        {
            killTimer(m_secondsTimerId);
            m_secondsTimerId = -1;
        }
        m_secondsTimerId = startTimer(1000);
    }
    else
    {
        checkToDoQueue();
    }
#endif
}

void LayoutHandler::checkToDoQueue()
{
#if 0
    if ((-1 != m_secondsTimerId) && (0 == m_requestsToBeVisiblePids.size()))
    {
        killTimer(m_secondsTimerId);
        m_secondsTimerId = -1;
    }

    if (0 != m_requestsToBeVisiblePids.size())
    {
        int pid = m_requestsToBeVisiblePids.at(0);
        qDebug("pid %d wants to be visible", pid);

        QList<int> allSurfaces;
        allSurfaces = mp_dBusWindowManagerProxy->getAllSurfacesOfProcess(pid);
        if (0 == allSurfaces.size())
        {
            qDebug("no surfaces for pid %d. retrying!", pid);
        }
        else
        {
            m_requestsToBeVisiblePids.removeAt(0);
            qSort(allSurfaces);

            if (0 != allSurfaces.size())
            {
                int firstSurface = allSurfaces.at(0);

                if (-1 != m_visibleSurfaces.indexOf(firstSurface))
                {
                    qDebug("already visible");
                }
                else
                {
                    if (-1 != m_invisibleSurfaces.indexOf(firstSurface))
                    {
                        m_invisibleSurfaces.removeAt(m_invisibleSurfaces.indexOf(firstSurface));
                    }
                    if (-1 == m_requestsToBeVisibleSurfaces.indexOf(firstSurface))
                    {
                        m_requestsToBeVisibleSurfaces.append(firstSurface);
                    }

                    qDebug("before");
                    qDebug(" m_visibleSurfaces %d", m_visibleSurfaces.size());
                    qDebug(" m_invisibleSurfaces %d", m_invisibleSurfaces.size());
                    qDebug(" m_requestsToBeVisibleSurfaces %d", m_requestsToBeVisibleSurfaces.size());

                    QList<int> availableLayouts = mp_dBusWindowManagerProxy->getAvailableLayouts(1); // one app only for CES2017
                    if (1 == availableLayouts.size())
                    {
                        qDebug("active layout: %d", availableLayouts.at(0));
                        m_invisibleSurfaces.append(m_visibleSurfaces);
                        m_visibleSurfaces.clear();
                        m_visibleSurfaces.append(m_requestsToBeVisibleSurfaces);
                        m_requestsToBeVisibleSurfaces.clear();

                        mp_dBusWindowManagerProxy->setLayoutById(availableLayouts.at(0));
                        for (int i = 0; i < m_visibleSurfaces.size(); ++i)
                        {
                            mp_dBusWindowManagerProxy->setSurfaceToLayoutArea(m_visibleSurfaces.at(i), i);
                        }

                        qDebug("after");
                        qDebug(" m_visibleSurfaces %d", m_visibleSurfaces.size());
                        qDebug(" m_invisibleSurfaces %d", m_invisibleSurfaces.size());
                        qDebug(" m_requestsToBeVisibleSurfaces %d", m_requestsToBeVisibleSurfaces.size());
                    }
                    else
                    {
                        qDebug("this should not happen!?");
                    }
                }
            }
        }
    }
#endif
}

#if 0
QList<int> LayoutHandler::requestGetAllSurfacesOfProcess(int pid)
{
    qDebug("requestGetAllSurfacesOfProcess %d", pid);

    return mp_dBusWindowManagerProxy->getAllSurfacesOfProcess(pid);
}
#endif

int LayoutHandler::requestGetSurfaceStatus(int surfaceId)
{
    int result = -1;

    if (-1 != m_visibleSurfaces.indexOf(surfaceId))
    {
        result = 0;
    }
    if (-1 != m_invisibleSurfaces.indexOf(surfaceId))
    {
        result = 1;
    }
    if (-1 != m_requestsToBeVisibleSurfaces.indexOf(surfaceId))
    {
        result = 1;
    }

    return result;
}

void LayoutHandler::requestRenderSurfaceToArea(int surfaceId, int layoutArea)
{
    qDebug("requestRenderSurfaceToArea %d %d", surfaceId, layoutArea);
}

bool LayoutHandler::requestRenderSurfaceToAreaAllowed(int surfaceId, int layoutArea)
{
    qDebug("requestRenderSurfaceToAreaAllowed %d %d", surfaceId, layoutArea);
    bool result = true;
    return result;
}

void LayoutHandler::requestSurfaceIdToFullScreen(int surfaceId)
{
    qDebug("requestSurfaceIdToFullScreen %d", surfaceId);
}

void LayoutHandler::setLayoutByName(QString layoutName)
{
    // switch to new layout
    qDebug("setLayout: switch to new layout %s", layoutName.toStdString().c_str());
    m_visibleSurfaces.append(m_requestsToBeVisibleSurfaces);
    m_requestsToBeVisibleSurfaces.clear();

    mp_dBusWindowManagerProxy->setLayoutByName(layoutName);
    for (int i = 0; i < m_visibleSurfaces.size(); ++i)
    {
        mp_dBusWindowManagerProxy->setSurfaceToLayoutArea(i, i);
    }
}

void LayoutHandler::requestSurfaceVisibilityChanged(int surfaceId, bool visible)
{
    qDebug("requestSurfaceVisibilityChanged %d %s", surfaceId, visible ? "true" : "false");
    emit surfaceVisibilityChanged(surfaceId, visible);
}

void LayoutHandler::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == m_secondsTimerId)
    {
        checkToDoQueue();
    }
}

