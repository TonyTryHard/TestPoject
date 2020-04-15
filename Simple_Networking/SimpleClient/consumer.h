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
    explicit Consumer(QObject *parent, const QString& address,
                      const QString& pathToFile, int port, double seed);

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
    QString address;
    QString filePath;
    QDataStream in;
    int m_port;
    double m_seed;

};

#endif // CONSUMER_H
