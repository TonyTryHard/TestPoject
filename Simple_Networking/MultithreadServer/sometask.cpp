#include "sometask.h"

SomeTask::SomeTask(double seed) : m_seed(seed)
{

}

//perform all needed action and emit certain signal
void SomeTask::run()
{

    //allocate inafe space for vec, the same as result.resize()
    QVector<double> result(1'000'000);
    //fill in container
    for (auto& doubl : result)
        doubl = ++m_seed;

    //qDebug() << "task is done";

    //and post event about finishing
    emit resultReady(result);
}
