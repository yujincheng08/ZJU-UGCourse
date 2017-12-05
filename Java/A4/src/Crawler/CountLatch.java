package Crawler;


import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class CountLatch {
    private int count;
    private ReentrantLock reentrantLock = new ReentrantLock();
    private Condition condition = reentrantLock.newCondition();

    CountLatch()
    {
        this(0);
    }

    CountLatch(int count)
    {
        this.count = count;
    }
    synchronized void increase(int step)
    {
        count+=step;
    }

    void increase()
    {
        increase(1);
    }

    synchronized void decrease(int step)
    {
        count-=step;
        if(count == 0) {
            reentrantLock.lock();
            condition.signalAll();
            reentrantLock.unlock();
        }
    }

    void decrease()
    {
        decrease(1);
    }

    void await()
            throws InterruptedException
    {
        reentrantLock.lock();
        condition.await();
        reentrantLock.unlock();
    }
}
