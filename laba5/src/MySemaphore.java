import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class MySemaphore extends Semaphore {

    private final ReentrantLock lock = new ReentrantLock();
    private final Condition permitsAvailable = lock.newCondition();
    private int permits;

    public MySemaphore(int initialPermits) {
        super(initialPermits);
        lock.lock();
        try {
            permits = initialPermits;
        } finally {
            lock.unlock();
        }
    }

    @Override
    public void acquire() throws InterruptedException {
        lock.lock();
        try {
            while (permits == 0) {
                permitsAvailable.await();
            }
            permits--;
            System.out.println("Захват, свободно: " + permits);

        } finally {
            lock.unlock();
        }
    }

    @Override
    public void release() {
        lock.lock();
        try {
            permits++;
            System.out.println("Освобождение, осталось: " + permits);
            permitsAvailable.signal();

        } finally {
            lock.unlock();
        }
    }

    @Override
    public int availablePermits() {
        return permits;
    }
}