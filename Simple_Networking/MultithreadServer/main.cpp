#include <QCoreApplication>
#include <QSettings>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //use settings file to define port for listening
    QSettings settings (QSettings::IniFormat, QSettings::UserScope,
                        QStringLiteral("Simple_Networking"), QStringLiteral("host"));
    qDebug() << settings.fileName();

    // actually we can use QSettings::value(&key, &defaultValue) for situation,
    // where value wasn't read from configuration file,
    // but we want save some port number in config.file for next server up:
    int hostPort = settings.value(QStringLiteral("/Host/Port")).toInt();
    long long quantity = settings.value(QStringLiteral("/Host/QuantityForAnswer")).toInt();
    if (!hostPort || !quantity){

        quantity = 1'000'000;
        hostPort = 56007; // some predefined number of port
        qDebug() << "Can't read port number from settigns. Use default:" << hostPort;

        //save port number for next time
        settings.beginGroup(QStringLiteral("Host"));
        settings.setValue(QStringLiteral("Port"),hostPort);
        settings.setValue(QStringLiteral("QuantityForAnswer"),quantity);
    }
    //create an instance of our server
    Server myServer(nullptr,hostPort,quantity);
    //and start listening
    myServer.StartServer();
    return a.exec();
}
