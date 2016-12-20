/*
 * Copyright (C) 2016 Mentor Graphics Development (Deutschland) GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <QGuiApplication>
#include <QCommandLineParser>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtQml/qqml.h>

#include "layouthandler.h"
#include "homescreencontrolinterface.h"
#include "applicationlauncher.h"
#include "statusbarmodel.h"
#include "applicationmodel.h"

void noOutput(QtMsgType, const QMessageLogContext &, const QString &)
{
}

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    QCoreApplication::setOrganizationDomain("LinuxFoundation");
    QCoreApplication::setOrganizationName("AutomotiveGradeLinux");
    QCoreApplication::setApplicationName("HomeScreen");
    QCoreApplication::setApplicationVersion("0.7.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("AGL HomeScreen - see wwww... for more details");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption quietOption(QStringList() << "q" << "quiet",
        QCoreApplication::translate("main", "Be quiet. No outputs."));
    parser.addOption(quietOption);
    parser.process(a);

    if (parser.isSet(quietOption))
    {
        qInstallMessageHandler(noOutput);
    }

    qDBusRegisterMetaType<AppInfo>();
    qDBusRegisterMetaType<QList<AppInfo> >();

    qmlRegisterType<ApplicationLauncher>("HomeScreen", 1, 0, "ApplicationLauncher");
    qmlRegisterType<ApplicationModel>("Home", 1, 0, "ApplicationModel");
    qmlRegisterType<StatusBarModel>("HomeScreen", 1, 0, "StatusBarModel");

    QQmlApplicationEngine engine;

    LayoutHandler* layoutHandler = new LayoutHandler();

    HomeScreenControlInterface* hsci = new HomeScreenControlInterface();
    QObject::connect(hsci, SIGNAL(newRequestGetSurfaceStatus(int)), layoutHandler, SLOT(requestGetSurfaceStatus(int)));
    QObject::connect(hsci, SIGNAL(newRequestsToBeVisibleApp(int)), layoutHandler, SLOT(makeMeVisible(int)));
    QObject::connect(hsci, SIGNAL(newRequestRenderSurfaceToArea(int, int)), layoutHandler, SLOT(requestRenderSurfaceToArea(int,int)));
    QObject::connect(hsci, SIGNAL(newRequestRenderSurfaceToAreaAllowed(int, int)), layoutHandler, SLOT(requestRenderSurfaceToAreaAllowed(int,int)));
    QObject::connect(hsci, SIGNAL(newRequestSurfaceIdToFullScreen(int)), layoutHandler, SLOT(requestSurfaceIdToFullScreen(int)));

    engine.rootContext()->setContextProperty("layoutHandler", layoutHandler);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return a.exec();
}
