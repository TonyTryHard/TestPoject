#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "client.h"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent, int portNumber, long long quantity);

public slots:
    void StartServer();

protected:
    void incomingConnection(const tSocketDescriptor handle) override;

private:
    int m_port;
    long long m_quantity;

};

#endif // SERVER_H
