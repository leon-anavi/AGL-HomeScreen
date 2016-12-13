#ifndef LAYOUTHANDLER_H
#define LAYOUTHANDLER_H

#include <QObject>
#include "windowmanager_proxy.h"
#include "popup_proxy.h"

class LayoutHandler : public QObject
{
    Q_OBJECT
public:
    explicit LayoutHandler(QObject *parent = 0);
    ~LayoutHandler();

signals:

public slots:
    void showAppLayer(int pid);
    void hideAppLayer();
    void makeMeVisible(int pid);
private:
    void checkToDoQueue();
public slots:
  //    QList<int> requestGetAllSurfacesOfProcess(int pid);
    int requestGetSurfaceStatus(int surfaceId);
    void requestRenderSurfaceToArea(int surfaceId, int layoutArea);
    bool requestRenderSurfaceToAreaAllowed(int surfaceId, int layoutArea);
    void requestSurfaceIdToFullScreen(int surfaceId);
    void setLayoutByName(QString layoutName);

    // this will receive the surfaceVisibilityChanged signal of the windowmanager
    void requestSurfaceVisibilityChanged(int surfaceId, bool visible);

Q_SIGNALS: // SIGNALS
    void surfaceVisibilityChanged(int surfaceId, bool visible);

protected:
    void timerEvent(QTimerEvent *e);
private:
    int m_secondsTimerId;
    org::agl::windowmanager *mp_dBusWindowManagerProxy;
    org::agl::popup *mp_dBusPopupProxy;

    QList<int> m_requestsToBeVisiblePids;
    QList<int> m_visibleSurfaces;
    QList<int> m_invisibleSurfaces;
    QList<int> m_requestsToBeVisibleSurfaces;
};

#endif // LAYOUTHANDLER_H
