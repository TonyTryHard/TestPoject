#include "server.h"

Server::Server(QObject *parent, int portNumber, long long quantity)
    : QTcpServer(parent), m_port(portNumber), m_quantity(quantity)
{

}

void Server::StartServer()
{
    if (!listen(QHostAddress::Any,m_port)) {
        qDebug() << QString("Unable to start the server: %1.").arg(this->errorString());
    }
    else{
        qDebug() << "Listening on port" << this->serverPort() << this->serverAddress();
    }
}

void Server::incomingConnection(tSocketDescriptor handle)
{
    //handle incoming connection for each client
    Client *myClient = new Client(this, m_quantity);
    myClient->setSocket(handle);
}
