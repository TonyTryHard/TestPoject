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
    explicit Server(QObject *parent = nullptr, int portNumber = 0);

public slots:
    void StartServer();

protected:
    void incomingConnection(const tSocketDescriptor handle) override;

private:
    int m_port;

};

#endif // SERVER_H
