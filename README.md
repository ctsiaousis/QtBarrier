# QtBarrier
A very simple barrier implementation written in Qt 5.15.2 that automatically recognizes main thread and calls `processEvents()` in order to have responsive GUI while waiting.

### Usage
Suppose a class `SimpleRunnable` that sleeps for a defined time
```cpp
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
```

We can create as many runnables as we like and wait for them to finish like so :
```cpp
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtBarrier mBar(5); // 4 runnables + 1 main thread

    qDebug() << "About to initiate runnables" << QDateTime::currentDateTime();
    for(int i = 0; i < 4; i++) {
        SimpleRunnable *r = new SimpleRunnable(1000*(i+1), &mBar);
        QThreadPool::globalInstance()->start(r);
    }
    mBar.wait();
    qDebug() << "Runnables finished" << QDateTime::currentDateTime();

    return 0;
}
```

For cases where threads end up calling slots on QObjects that live on the main thread, this implementation may be risky.
For that usecase you can initialize `QtBarrier` with `strict = true` and rely on a separate `QEventLoop` object to have responsive UI.
```cpp
QtBarrier mBar(5, true);
```

