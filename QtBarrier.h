#ifndef QTBARRIER_H
#define QTBARRIER_H

#include <QMutex>
#include <QThread>
#include <QAtomicInt>
#include <QMutexLocker>
#include <QWaitCondition>


class BarrierData
{
    BarrierData(int count, bool strict) : count(count), strict(strict) {}
public:

    void wait() {
        mutex.lock();
        --count;
        if (count > 0){
            if(strict || QThread::currentThread()->currentThreadId() != QApplication::instance()->thread()->currentThreadId()){
                condition.wait(&mutex);
                mutex.unlock();
            } else { ////////////////////////////// we are the main thread
                mutex.unlock();
                forever {
                    bool sleep = (count > 0);
                    if(!sleep)
                        break;
                    QApplication::processEvents();
                    QThread::currentThread()->msleep(1000/60);//60fps
                }
            }

        } else {
            condition.wakeAll();
            mutex.unlock();
        }
    }
private:
    Q_DISABLE_COPY(BarrierData)
    QAtomicInt count = 0;
    bool strict = false;
    QMutex mutex;
    QWaitCondition condition;
};

class QtBarrier {
public:
    // Create a barrier that will wait for count threads
    // if strict is true, when called on main thread GUI is blocked
    QtBarrier(int count, bool strict = false) : d(new BarrierData(count, strict)) {}
    void wait() {
        d->wait();
    }

private:
    BarrierData * d = nullptr;
};

#endif //QTBARRIER_H
