#include "statusbarserver.h"
#include "statusbar_adaptor.h"

class StatusBarServer::Private
{
public:
    Private(StatusBarServer *parent);
    QString texts[SupportedCount];
    QString icons[SupportedCount];
    StatusbarAdaptor adaptor;
};

StatusBarServer::Private::Private(StatusBarServer *parent)
    : adaptor(parent)
{
    icons[0] = QStringLiteral("./images/Status/HMI_Status_Wifi_Full-01.png");
    icons[1] = QStringLiteral("./images/Status/HMI_Status_Bluetooth_Inactive-01.png");
    icons[2] = QStringLiteral("./images/Status/HMI_Status_Signal_Full-01.png");
}

StatusBarServer::StatusBarServer(QObject *parent)
    : QObject(parent)
    , d(new Private(this))
{
}

StatusBarServer::~StatusBarServer()
{
    delete d;
}

QList<int> StatusBarServer::getAvailablePlaceholders() const
{
    QList<int> ret;
    for (int i = 0; i < SupportedCount; ++i){
        ret.append(i);
    }
    return ret;
}

QString StatusBarServer::getStatusIcon(int placeholderIndex) const
{
    QString ret;
    if (-1 < placeholderIndex && placeholderIndex < SupportedCount)
        ret = d->icons[placeholderIndex];
    return ret;
}

void StatusBarServer::setStatusIcon(int placeholderIndex, const QString &icon)
{
    if (-1 < placeholderIndex && placeholderIndex < SupportedCount) {
        if (d->icons[placeholderIndex] == icon) return;
        d->icons[placeholderIndex] = icon;
        emit statusIconChanged(placeholderIndex, icon);
    }
}

QString StatusBarServer::getStatusText(int placeholderIndex) const
{
    QString ret;
    if (-1 < placeholderIndex && placeholderIndex < SupportedCount) {
        ret = d->texts[placeholderIndex];
    }
    return ret;
}

void StatusBarServer::setStatusText(int placeholderIndex, const QString &text)
{
    if (-1 < placeholderIndex && placeholderIndex < SupportedCount) {
        if (d->texts[placeholderIndex] == text) return;
        d->texts[placeholderIndex] = text;
        emit statusTextChanged(placeholderIndex, text);
    }
}
