#include "include/appframework.hpp"

AppInfo::AppInfo(QObject *parent) :
    QObject(parent)
{
}

AppInfo::AppInfo(const AppInfo &other) :
    QObject(other.parent()),
    name(other.getName()),
    iconPath(other.getIconPath()),
    description(other.getDescription())
{
}

AppInfo& AppInfo::operator=(const AppInfo &other)
{
    setParent(other.parent());
    name = other.getName();
    iconPath = other.getIconPath();
    description = other.getDescription();

    return *this;
}

AppInfo::~AppInfo()
{
}

void AppInfo::registerMetaType()
{
    qRegisterMetaType<AppInfo>("AppInfo");
    qDBusRegisterMetaType<AppInfo>();
}


// Marshall the MyStructure data into a D-Bus argument
QDBusArgument &operator<<(QDBusArgument &argument, const AppInfo &appInfo)
{
    argument.beginStructure();
    argument << appInfo.name << appInfo.iconPath << appInfo.description;
    argument.endStructure();
    qDebug("appInfo.name:<< %s", appInfo.name.toStdString().c_str());
    return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, AppInfo &appInfo)
{
    argument.beginStructure();
    argument >> appInfo.name >> appInfo.iconPath >> appInfo.description;
    argument.endStructure();
    qDebug("appInfo.name:>> %s", appInfo.name.toStdString().c_str());
    return argument;
}
