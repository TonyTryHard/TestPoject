#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    //set up max thread pool count for app
    QThreadPool::globalInstance()->setMaxThreadCount(16);
    //register queued meta type for using in signals/slots
    qRegisterMetaType<QVector<double>>();
}

void Client::setSocket(tSocketDescriptor socketDescriptor)
{
    //initiate socket
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    //and set connects
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));

    qDebug() << "Client connected";
}


void Client::disconnected()
{
    //just for visualiozation
    qDebug() << "Client disconnected";
}

void Client::readyRead()
{
    //when ready get data from users
    QDataStream in;
    in.setDevice(socket);
    //set the same version as client has
    in.setVersion(QDataStream::Qt_5_0);
    //get data
    in >> m_seed;
    //qDebug() << "Have new seed: " << m_seed;
    //create a new instance of QRunnable to perform calculation
    SomeTask *newTask = new SomeTask(m_seed);
    //set flag for autodeleten after job is done
    newTask->setAutoDelete(true);
    //make connect between QRunnable and Client instance, use QueuedConnection, because they living in different threads
    connect(newTask,SIGNAL(resultReady(QVector<double>)),this,SLOT(resultReady(QVector<double>)),Qt::QueuedConnection);
    //start task execution
    QThreadPool::globalInstance()->start(newTask);
}

void Client::resultReady(QVector<double> vec)
{
    //when result is ready, send data to user
    qDebug() << "Send data to client";
    QDataStream out(socket);
    out.setVersion(QDataStream::Qt_5_0);
    out << vec;
    //and disconnect client from host
    socket->disconnectFromHost();
    socket->waitForDisconnected();
}
