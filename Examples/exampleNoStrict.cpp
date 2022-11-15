#include "QtBarrier.h"

#include <QApplication>
#include <QThreadPool>
#include <QRunnable>
#include <QDateTime>
#include <QDebug>

class SimpleRunnable: public QRunnable
{
    int sleepMs = 0;
    QtBarrier *bar = nullptr;
public:
    SimpleRunnable(int sleepMsIn,
                       QtBarrier *barIn):
        sleepMs(sleepMsIn),
        bar(barIn)
    {

    }

    void run() {
        QThread::currentThread()->msleep(sleepMs);
        if( bar )
          bar->wait();
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtBarrier mBar(5);

    qDebug() << "About to initiate runnables" << QDateTime::currentDateTime();
    for(int i = 0; i < 4; i++) {
        SimpleRunnable *r = new SimpleRunnable(1000*(i+1), &mBar);
        QThreadPool::globalInstance()->start(r);
    }
    mBar.wait();
    qDebug() << "Runnables finished" << QDateTime::currentDateTime();

    return 0;
}
