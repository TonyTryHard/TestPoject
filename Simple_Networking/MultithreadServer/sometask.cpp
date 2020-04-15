#include "sometask.h"

SomeTask::SomeTask(double seed, long long quantity) : m_seed(seed), m_quantity(quantity)
{

}

//perform all needed action and emit certain signal
void SomeTask::run()
{

    //allocate inafe space for vec, the same as result.resize()
    QVector<double> result(m_quantity);
    //fill in container
    for (auto& doubl : result){
        doubl = ++m_seed;
        qDebug() << doubl;
    }

    //qDebug() << "task is done";

    //and post event about finishing
    emit resultReady(result);
}
