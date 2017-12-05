package Crawler;

import com.sun.istack.internal.NotNull;

import java.util.concurrent.Semaphore;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.BlockingQueue;


public class BlockThreadPoolExecutor extends ThreadPoolExecutor {
    private final Semaphore semaphore;
    private CountLatch running  = new CountLatch();
    BlockThreadPoolExecutor(int size, long keepAliveTime, @NotNull TimeUnit timeInit, @NotNull BlockingQueue<Runnable> blockingQueue)
    {
        super(size, size, keepAliveTime, timeInit, blockingQueue);
        semaphore = new Semaphore(size-1);
    }

    public synchronized void blockSubmit(final Runnable task) throws InterruptedException{
        semaphore.acquire();
        running.increase();
        super.submit(task);
    }


    @Override
    protected void afterExecute(Runnable r, Throwable t) {
        super.afterExecute(r, t);
        semaphore.release();
        running.decrease();
    }

    public void await()
            throws InterruptedException
    {
        running.await();
    }
}
