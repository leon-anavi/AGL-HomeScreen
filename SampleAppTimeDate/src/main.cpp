#include <QCoreApplication>
#include "timedateprovider.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // used for application settings (QSettings)
    QCoreApplication::setOrganizationDomain("LinuxFoundation");
    QCoreApplication::setOrganizationName("AutomotiveGradeLinux");
    QCoreApplication::setApplicationName("SampleAppTimeDate");
    QCoreApplication::setApplicationVersion("0.3.0");

    TimeDateProvider *tdp = new TimeDateProvider();
    tdp->start();

    return a.exec();
}
