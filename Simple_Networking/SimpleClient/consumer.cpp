#include <QFile>
#include <QFileInfo>
#include "consumer.h"

Consumer::Consumer(QObject *parent, QString address, int port, double seed, QString pathToFile)
    : QObject(parent),tcpSocket(new QTcpSocket(this)), m_timer(new QTimer(this)),
      address(address), m_port(port), m_seed(seed), filePath(pathToFile)
{
    //set device for QDataStream
    in.setDevice(tcpSocket);
    //set the same version of QDataStream as server has
    in.setVersion(QDataStream::Qt_5_0);
    //connect slots
    connect(tcpSocket, &QAbstractSocket::connected,this,&::Consumer::onConnection);
    connect(tcpSocket,&QIODevice::readyRead, this, &::Consumer::ReceivedData);
}

void Consumer::doWork()
{
    //initiate connection with defined host
    tcpSocket->connectToHost(address,m_port);
}

void Consumer::onConnection()
{
//    qDebug() << "On connection";
    //after socket connection establishment, start single timer
    //and after 3 seconds call slot for data request
    m_timer->singleShot(3'000,this, SLOT(RequestData()));
}

void Consumer::RequestData()
{
//    qDebug() << "Data Requested for " << m_seed;
    // send double value to host
    in << m_seed;
}

void Consumer::ReceivedData()
{
    //begin data transmition
    in.startTransaction();
    QVector<double> vec;
    in >> vec;
    // if have occurred some errors on during read transaction -> return
    if (!in.commitTransaction())
        return;

    //if read vector size equal to null, ask again for data
    if ( !vec.size() ) {
        QTimer::singleShot(0, this, &::Consumer::RequestData);
        return;
    }

    //otherwise save data to file
    QFile file;
    //use predefined file name from config file, if we can
    if (!filePath.isEmpty())
        file.setFileName(filePath);
    // otherwise use some default name, file will be created near the .exe
    else
        file.setFileName("file.dat");

    //use additional class for information about file path
    QFileInfo fi(file);
    if (!file.open(QIODevice::WriteOnly)){
        qDebug() << "Can't open file: " << fi.absoluteFilePath();
        return;
    }
    else {
        QDataStream out( &file );
        out.setVersion(QDataStream::Qt_5_0);
        out << vec;
        qDebug() << "Data successfully was written to file: " << fi.absoluteFilePath();
        file.close();
    }

    emit finished();
}
