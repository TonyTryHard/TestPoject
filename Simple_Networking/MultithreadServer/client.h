#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QThreadPool>
#include <QDataStream>
#include "sometask.h"

#if QT_VERSION >= 0x050000
    typedef qintptr tSocketDescriptor;
#else
    typedef int tSocketDescriptor;
#endif

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent, long long quantity);

public slots:
    void setSocket(tSocketDescriptor socketDescriptor);
    void disconnected();
    void readyRead();
    void resultReady(QVector<double> vec);

private:
    QTcpSocket *socket;
    double m_seed;
    long long m_quantity;
};

#endif // CLIENT_H
