#include <QCoreApplication>
#include "timedateprovider.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TimeDateProvider *tdp = new TimeDateProvider();
    tdp->start();

    return a.exec();
}
