import java.util.concurrent.atomic.AtomicInteger;

public class SpinSemaphore {

    private final AtomicInteger permits;

    public SpinSemaphore(int permits) {
        this.permits = new AtomicInteger(permits);
    }

    public void acquire() {

        while (true) {
            int current = permits.get();

            if (current > 0) {
                if (permits.compareAndSet(current, current - 1)) {
                    System.out.println(Thread.currentThread().getName() +
                            " получил доступ, осталось: " + (current - 1));
                    return;
                }
            }
        }
    }

    public void release() {
        int value = permits.incrementAndGet();
        System.out.println("Освобождение разрешения (spin), теперь: " + value);
    }
}