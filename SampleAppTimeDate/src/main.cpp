#include <QCoreApplication>
#include <QCommandLineParser>
#include "timedateprovider.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setOrganizationDomain("LinuxFoundation");
    QCoreApplication::setOrganizationName("AutomotiveGradeLinux");
    QCoreApplication::setApplicationName("SampleAppTimeDate");
    QCoreApplication::setApplicationVersion("0.7.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("AGL Sample app for StatusBar interface - see wwww... for more details");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(a);

    TimeDateProvider *tdp = new TimeDateProvider();
    tdp->start();

    return a.exec();
}
