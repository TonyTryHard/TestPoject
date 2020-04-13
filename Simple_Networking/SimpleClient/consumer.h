#ifndef CONSUMER_H
#define CONSUMER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QDataStream>

class Consumer : public QObject
{
    Q_OBJECT
public:
    explicit Consumer(QObject *parent = nullptr, QString address = nullptr,
                      int port = 0, double seed = 0, QString pathToFile = nullptr);

signals:
    void finished();

public slots:
    void doWork();
    void onConnection();
    void RequestData();
    void ReceivedData();

private:
    QTcpSocket *tcpSocket = nullptr;
    QTimer *m_timer;
    QDataStream in;
    QString address;
    int m_port;
    double m_seed;
    QString filePath;
};

#endif // CONSUMER_H
