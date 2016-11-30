#include "calledbyqml.h"
#include <libhomescreen.hpp>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>


CalledByQml::CalledByQml(QObject *parent) :
    QObject(parent)
{
}

int CalledByQml::consoleout()
{
    qDebug("consoleout method is called");
    LibHomeScreen libHomeScreen;
    libHomeScreen.hardKeyPressed(1);
    return 0;
}

int CalledByQml::showup()
{
    qDebug("showup method is called");
    QGuiApplication app2();
    QQmlApplicationEngine engine2;
    QString target(getenv("AFM_APP_INSTALL_DIR"));
    qDebug("AFM_APP_INSTALL_DIR is " + target.toLatin1());

    if(NULL == target)
    {
        target = ".";
    }

    QString load_path;
    load_path = target + "/main2.qml";
    //engine2.load(QUrl(load_path));
    return 0;
}
