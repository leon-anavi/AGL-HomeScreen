#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/SampleMediaApp/main.qml"));
    viewer.setFlags(Qt::FramelessWindowHint);
    viewer.showExpanded();

    return app.exec();
}
