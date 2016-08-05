#ifndef TIMEDATEPROVIDER_HPP
#define TIMEDATEPROVIDER_HPP

#include <QObject>
#include "statusbar_proxy.h"

class TimeDateProvider : public QObject
{
    Q_OBJECT
public:
    explicit TimeDateProvider(QObject *parent = 0);
    ~TimeDateProvider();
    void start();
    void stop();
protected:
    void timerEvent(QTimerEvent *e);
private:
    int m_secondsTimerId;
    org::agl::statusbar *mp_dBusStatusBarProxy;
    int m_statusBarPlaceholder;
signals:

public slots:

};

#endif // TIMEDATEPROVIDER_HPP
