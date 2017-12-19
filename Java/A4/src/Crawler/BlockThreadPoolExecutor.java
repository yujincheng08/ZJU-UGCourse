package Crawler;

import com.sun.istack.internal.NotNull;

import java.util.concurrent.*;


public class BlockThreadPoolExecutor extends ThreadPoolExecutor {
    private final Semaphore semaphore;
    private CountLatch running  = new CountLatch();
    BlockThreadPoolExecutor(int size, long keepAliveTime, @NotNull TimeUnit timeInit)
    {
        super(size, Integer.MAX_VALUE, keepAliveTime, timeInit, new LinkedBlockingQueue<>(size));
        semaphore = new Semaphore(size, true);
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
