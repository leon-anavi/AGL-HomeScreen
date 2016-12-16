#include <QCoreApplication>
#include <QCommandLineParser>
#include "timedateprovider.hpp"

void noOutput(QtMsgType, const QMessageLogContext &, const QString &)
{
}

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
    QCommandLineOption quietOption(QStringList() << "q" << "quiet",
        QCoreApplication::translate("main", "Be quiet. No outputs."));
    parser.addOption(quietOption);
    parser.process(a);

    if (parser.isSet(quietOption))
    {
        qInstallMessageHandler(noOutput);
    }

    TimeDateProvider *tdp = new TimeDateProvider();
    tdp->start();

    return a.exec();
}
