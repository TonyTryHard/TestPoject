#include <QCoreApplication>
#include <QSettings>
#include <QThread>
#include "consumer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //get connection options from configuration file
    QSettings settings (QSettings::IniFormat, QSettings::UserScope,
                        QStringLiteral("Simple_Networking"), QStringLiteral("client"));

    QString hostAddress=settings.value(QStringLiteral("/Host/Address")).toString();
    QString hostPort=settings.value(QStringLiteral("/Host/Port")).toString();

    //use bool flag for checking conversion result from QVariant to double
    //we can just check !seed (because conversion will return Null if it failed)
    //but if seed value is set up to Null, we will have disagreeable behavior, so
    bool rightConversion = false;
    double seed = settings.value(QStringLiteral("/Client/Seed")).toDouble(&rightConversion);
    QString pathToFile = settings.value(QStringLiteral("/Client/SaveTo")).toString();

    //check connection options and sending value
    if (!rightConversion || hostAddress.isEmpty() || hostPort.isEmpty())
    {
        //if invalid, use some default value
        qDebug() << "Read settings data is failed!";
        hostAddress = QStringLiteral("169.254.189.37");
        hostPort = QStringLiteral("56007");
        seed = 1.1;
        //and save them for next session
        settings.beginGroup(QStringLiteral("Host"));
        settings.setValue(QStringLiteral("Address"),QStringLiteral("169.254.189.37"));
        settings.setValue(QStringLiteral("Port"),QStringLiteral("56007"));
        settings.endGroup();
        settings.beginGroup(QStringLiteral("Client"));
        settings.setValue(QStringLiteral("Seed"),QStringLiteral("1.1"));
        settings.setValue(QStringLiteral("SaveTo"),QStringLiteral("file.dat"));
        settings.endGroup();
    }
    else
    {
        qDebug() << "Settings was read correctly. Address: " << hostAddress << " Port: "
                 << hostPort << " Sending value: " << seed;
    }
    //use Consumer object, which live in separate thread for data exchange with server
    QThread thread;
    Consumer *consumer = new Consumer(nullptr,hostAddress,hostPort.toInt(),seed,pathToFile);
    consumer->moveToThread(&thread);
    QObject::connect(&thread, SIGNAL(started()),consumer, SLOT(doWork()));
    QObject::connect(consumer, SIGNAL(finished()),&a, SLOT(quit()));

    thread.start();
    int nResult = a.exec();

    thread.quit();
    thread.wait();

    return nResult;
}
