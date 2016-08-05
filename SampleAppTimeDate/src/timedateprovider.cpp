#include "timedateprovider.hpp"
#include <QTimerEvent>

TimeDateProvider::TimeDateProvider(QObject *parent) :
    QObject(parent),
    m_secondsTimerId(-1),
    mp_dBusStatusBarProxy(0),
    m_statusBarPlaceholder(-1)
{
    qDebug("D-Bus: register as org.agl.SampleAppTimeDate");
    // dbus setup
    QDBusConnection dbus = QDBusConnection::sessionBus();

    dbus.registerObject("/", this);
    dbus.registerService("org.agl.sampleapptimedate");


    qDebug("D-Bus: connect to org.agl.homescreen /StatusBar");
    mp_dBusStatusBarProxy = new org::agl::statusbar("org.agl.homescreen",
                                              "/StatusBar",
                                              QDBusConnection::sessionBus(),
                                              0);
}

TimeDateProvider::~TimeDateProvider()
{
    stop();

    if (0 != mp_dBusStatusBarProxy)
    {
        mp_dBusStatusBarProxy->setStatusText(1, "");
        delete mp_dBusStatusBarProxy;
    }
}

void TimeDateProvider::start()
{
    qDebug("trying to start timer (if this lasts long, maybe the Home Screen Application is not launched.");
    if ((-1 == m_statusBarPlaceholder) && (0 != mp_dBusStatusBarProxy))
    {
        QList<int> availablePlaceholder = mp_dBusStatusBarProxy->getAvailablePlaceholders();
        if (availablePlaceholder.size() > 0)
        {
            // just take the first available placeholder
            m_statusBarPlaceholder = availablePlaceholder[0];
            qDebug("- using statusbar placeholder %d", m_statusBarPlaceholder);

            qDebug("- timer started");
            // callback every second
            m_secondsTimerId = startTimer(1000);
        }
    }
}

void TimeDateProvider::stop()
{
    if (-1 != m_secondsTimerId)
    {
        killTimer(m_secondsTimerId);
        m_secondsTimerId = -1;
        m_statusBarPlaceholder = -1;
    }
}

void TimeDateProvider::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == m_secondsTimerId)
    {
        if (0 != mp_dBusStatusBarProxy)
        {
            QString toDisplay = QDateTime::currentDateTime().toString("hh:mm");
            qDebug("%s", toDisplay.toStdString().c_str());
            mp_dBusStatusBarProxy->setStatusText(m_statusBarPlaceholder, toDisplay);
        }
    }
}
