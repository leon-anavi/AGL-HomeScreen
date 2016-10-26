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

    void setUpLayouts();

signals:

public slots:
    void makeMeVisible(int pid);
    void toggleFullscreen();
    void setLayoutByName(QString layoutName);

private:
    org::agl::windowmanager *mp_dBusWindowManagerProxy;
    org::agl::popup *mp_dBusPopupProxy;

    QList<int> m_visibleApps;
    QList<int> m_invisibleApps;
    QList<int> m_requestsToBeVisibleApps;

};

#endif // LAYOUTHANDLER_H
