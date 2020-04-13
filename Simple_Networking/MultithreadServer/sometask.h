#ifndef SOMETASK_H
#define SOMETASK_H

#include <QRunnable>
#include <QObject>
#include <QDebug>

//inherist QObject for signals/slots mechanism using
class SomeTask : public QObject, public QRunnable

{
    Q_OBJECT

public:
    SomeTask(double seed);

protected:
    void run() override;

signals:
    void resultReady(QVector<double> vec);

private:
    double m_seed;
};

#endif // SOMETASK_H
