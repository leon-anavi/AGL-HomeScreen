#include <QtGui/QGuiApplication>
#include <QtGui/QWindow>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/qqml.h>
#include <QtCore/QDebug>

#include "applicationlauncher.h"
#include "statusbarmodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationDomain(QStringLiteral("automotivelinux.org"));
    QCoreApplication::setOrganizationName(QStringLiteral("AutomotiveGradeLinux"));
    QCoreApplication::setApplicationName(QStringLiteral("HomeScreen"));
    QCoreApplication::setApplicationVersion(QStringLiteral("0.0.1"));

    QGuiApplication app(argc, argv);

    qmlRegisterType<ApplicationLauncher>("HomeScreen", 1, 0, "ApplicationLauncher");
    qmlRegisterType<StatusBarModel>("HomeScreen", 1, 0, "StatusBarModel");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
