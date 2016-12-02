#include "applicationlauncher.h"

#include <QtCore/QDebug>

ApplicationLauncher::ApplicationLauncher(QObject *parent)
    : QObject(parent)
{
}

void ApplicationLauncher::launch(const QString &application)
{
    qDebug() << "launch" << application;
}
